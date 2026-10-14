"""Public single-shape backend dispatch, JIT supervision and OPC fallback."""
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import time
import uuid

from .specialization.artifact_publisher import publish_artifacts
from .specialization.errors import SpecializationError
from .specialization.specialization_options import is_aclgraph_sk
from .specialization.shape_config_adapter import preflight_shape_config


def _jit_skip_reason(shape, compile_mode, enable_sk):
    """仅提前排除已知不适用的任务，资源和编译能力仍由 JIT 实际尝试确定。"""
    if compile_mode in ('tune', 'aoe'):
        return 'deprecated compile mode uses OPC directly: ' + compile_mode
    if enable_sk:
        try:
            if not is_aclgraph_sk(shape):
                return 'SK task does not select the ACLGraph combine resource path'
        except ValueError as error:
            return 'ambiguous SK configuration: ' + str(error)
    return None


def _run_worker(command, log, env, timeout):
    """在独立进程组中执行 JIT worker，合并输出并限制等待时间。

    参数：command 为命令参数列表，log 为日志路径，env 为子进程环境，
    timeout 为等待秒数。正常返回进程退出码，超时终止后返回 124。
    启动或信号处理等其他异常交由调用方处理。
    """
    # 日志统一收集 stdout/stderr；新会话使 worker 及其编译子进程可按组终止。
    with open(log, 'w', encoding='utf-8') as stream:
        process = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT,
                                   env=env, start_new_session=True)
        try:
            return process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            # 先请求整个进程组退出，再给组长最多 5 秒的退出时间。
            try:
                os.killpg(process.pid, signal.SIGTERM)
            except ProcessLookupError:
                pass
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                pass
            finally:
                # 组长退出后仍可能遗留编译子进程，因此无论组长是否退出都向组发 SIGKILL。
                # 注意：下面的 wait 只回收组长，当前尚未确认整个进程组已全部退出。
                try:
                    os.killpg(process.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                process.wait()
            return 124



def compile_static_kernel_job(opc_json, compile_res_dir, kernel_meta_dir,
                              soc_version, log_level, compile_log_dir, enable_sk,
                              compile_mode='opc'):
    """统一编译一个 shape：先选择后端，JIT 未发布时失败则回退 OPC。

    保留 _opc_compile_single 的位置参数，增加编译模式供原进程池统一调度：
        opc_json：转换后的单 shape OPC 输入 JSON 路径。
        compile_res_dir：最终产物收集目录。
        kernel_meta_dir：编译中间文件目录。
        soc_version：目标芯片；log_level：日志等级。
        compile_log_dir：编译日志目录；enable_sk：是否要求 SK 能力。
        compile_mode：保留 tune/aoe 直接使用 OPC 的行为。

    成功返回 0，失败通常返回非零值；发布后出错等情况抛异常以中止打包。
    最终目录只接收发布后的产物，日志记录实际后端，避免将 OPC 回退计为 JIT 成功。
    """
    # static_compilation 已导入本模块，延迟导入旧函数以避免模块初始化时循环依赖。
    from static_compile.static_compilation import _opc_compile_single
    # 单调时钟用于统计整个任务（含可能的 OPC 回退）的耗时。
    started = time.monotonic()
    result_root, logs = Path(compile_res_dir), Path(compile_log_dir)
    result_root.mkdir(parents=True, exist_ok=True)
    logs.mkdir(parents=True, exist_ok=True)
    # 从输入描述取产物名；只允许文件名，防止拼接路径时越出任务目录。
    document = json.loads(Path(opc_json).read_text())
    job_name = Path(opc_json).stem
    shape = document['op_list'][0]
    basename = shape.get('bin_filename', job_name)
    if Path(basename).name != basename or basename in ('', '.', '..'):
        raise ValueError('bin_filename must be a basename')
    # 原流程会递归扫描最终目录中的 .o/.json，半成品必须留在独立暂存区。
    # 此处依赖调用方的目录布局，将工作目录放在 kernel_meta_dir 的父目录下。
    work = Path(tempfile.mkdtemp(prefix='specialization-', dir=Path(kernel_meta_dir).parent))
    stage = work / 'jit'
    report = {'op_type': document['op_type'], 'basename': basename,
              'jit_attempted': False, 'jit_success': False, 'backend': None,
              'compile_mode': compile_mode, 'work_dir': str(work)}
    # 这里是唯一的后端选择位置；不适用的任务不启动 JIT worker。
    skip_reason = _jit_skip_reason(shape, compile_mode, enable_sk)
    if skip_reason is None:
        try:
            preflight_shape_config(opc_json, soc_version)
        except Exception as error:
            # 输入确定不满足条件时直接交给 OPC，不启动 worker 或调用 aclrtc。
            skip_reason = 'JIT input preflight failed: ' + str(error)
    if skip_reason is not None:
        report['jit_skip_reason'] = skip_reason
    else:
        stage.mkdir()
        report['jit_attempted'] = True
        # 仅调整 worker 的环境，临时文件归入本任务，优先加载当前这份 op_compile_tool。
        env = os.environ.copy()
        env['PYTHONDONTWRITEBYTECODE'] = '1'
        env['TMPDIR'] = str(work)
        tool_root = str(Path(__file__).resolve().parents[1])
        env['PYTHONPATH'] = tool_root + os.pathsep + env.get('PYTHONPATH', '')
        jit_log = logs / (job_name + '_jit.log')
        worker_report = work / 'worker_result.json'
        # 真正的 request 构造和 aclrtc 调用在 worker 模块中完成；这里负责启动和监督。
        command = [sys.executable, '-B', '-m', 'static_compile.specialization.static_kernel_specialization',
                   '--input', str(Path(opc_json).resolve()), '--stage', str(stage),
                   '--soc', soc_version, '--log-level', log_level, '--result', str(worker_report)]
        if enable_sk:
            command.append('--enable-sk')
        # 区分尚未发布与已经发布，后者发生错误时不能再混入另一套后端产物。
        destination = None
        published = False
        try:
            # 900 秒只限制 JIT 尝试，不是整个 JIT + OPC 任务的超时。
            rc = _run_worker(command, jit_log, env, 900)
            report['jit_returncode'] = rc
            report['jit_log'] = str(jit_log)
            # worker 崩溃或超时可能来不及写报告，不能假定该文件一定存在。
            if worker_report.is_file():
                report['jit'] = json.loads(worker_report.read_text())
            if rc == 0:
                # 退出码成功仍须确认 .o/.json 成对存在；语义校验由 worker 完成。
                for suffix in ('.o', '.json'):
                    if not (stage / (basename + suffix)).is_file():
                        raise RuntimeError('JIT returned without a complete pair')
                # 发布整个暂存目录：同文件系统 rename、禁止覆盖，并由发布器执行 fsync。
                destination = result_root / ('jit-' + uuid.uuid4().hex)
                publish_artifacts(stage, destination)
                published = True
                report.update(backend='jit', jit_success=True, output_dir=str(destination), returncode=0)
                (logs / (job_name + '_compile_succ.log')).write_text('aclrtc JIT succeeded\n', encoding='utf-8')
                return 0
        except Exception as error:
            report['jit_failure'] = str(error)
            # rename 成功后，fsync 或日志写入仍可能失败。此时产物已对外可见，必须中止，
            # 不能回退后又发布一套 OPC 结果。优先使用发布器的状态，其他异常保守检查目录。
            if published or (isinstance(error, SpecializationError) and error.published) or (
                    not isinstance(error, SpecializationError) and destination is not None
                    and destination.exists() and not stage.exists()):
                report.update(backend='jit', returncode=1, output_dir=str(destination),
                              publication_uncertain=True)
                raise RuntimeError('JIT publication occurred; aborting packaging') from error
        finally:
            # 即使上面 return 也会执行 finally；若继续回退，OPC 结束后会更新同一份报告。
            report['elapsed_seconds'] = time.monotonic() - started
            (logs / (job_name + '_backend_result.json')).write_text(json.dumps(report, indent=2))

    # 主动跳过 JIT 和 JIT 发布前失败共用此 OPC 分支，原始输入和 SK 配置不变。
    opc_stage = work / 'opc'
    opc_stage.mkdir()
    destination = None
    published = False
    try:
        # 保持原 OPC 编译逻辑和输入，仅隔离本次输出及中间目录；这里未增加 OPC 超时。
        opc_meta = Path(kernel_meta_dir) / ('opc-' + uuid.uuid4().hex)
        opc_meta.mkdir(parents=True)
        rc = _opc_compile_single(opc_json, compile_res_dir=str(opc_stage),
                                 kernel_meta_dir=str(opc_meta), soc_version=soc_version,
                                 log_level=log_level, compile_log_dir=str(logs), enable_sk=enable_sk)
        if rc == 0:
            for suffix in ('.o', '.json'):
                if not (opc_stage / (basename + suffix)).is_file():
                    raise RuntimeError('OPC returned without a complete pair')
            # OPC 也要求完整文件对，并通过同一发布器进入最终目录。
            destination = result_root / ('opc-' + uuid.uuid4().hex)
            publish_artifacts(opc_stage, destination)
            published = True
            report['output_dir'] = str(destination)
        # jit_success 保持 False，最终成功后端为 OPC，不掩盖前面的 JIT 失败。
        report.update(backend='opc', returncode=rc)
    except Exception as error:
        rc = 1
        report.update(backend='opc', returncode=rc, opc_failure=str(error))
        # OPC 发布后发生异常，同样中止打包，不尝试覆盖或伪装成未发布。
        if published or (isinstance(error, SpecializationError) and error.published) or (
                not isinstance(error, SpecializationError) and destination is not None
                and destination.exists() and not opc_stage.exists()):
            report['publication_uncertain'] = True
            raise RuntimeError('OPC publication occurred; aborting packaging') from error
    finally:
        # 写最终后端与总耗时，失败标记供原日志汇总流程识别。
        report['elapsed_seconds'] = time.monotonic() - started
        (logs / (job_name + '_backend_result.json')).write_text(json.dumps(report, indent=2))
        if report.get('returncode') != 0:
            # 原 OPC 已写出的诊断保留原文；只有没有诊断时才补失败标记供汇总读取。
            error_log = logs / (job_name + '_compile_error.log')
            if not error_log.exists():
                error_log.write_text(json.dumps(report, indent=2))
    return rc
