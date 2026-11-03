"""Public single-shape backend dispatch, JIT supervision and OPC fallback."""

import json
import logging
import os
from pathlib import Path
import signal
import subprocess
import sys
import time

from op_compiler_common import LOG_LEVEL_DICT
from .specialization.artifact_publisher import publish_artifacts
from .specialization.errors import ArtifactCollisionError, SpecializationError
from .specialization.specialization_options import is_aclgraph_sk
from .specialization.shape_config_adapter import prepare_specialization_inputs


logger = logging.getLogger(__name__)
_JIT_LOG_PREFIX = '[op_compiler][JIT static compile]'
_ACLRTC_DIAGNOSTIC_FILES = {
    'aclrtc_compile.log', 'aclrtc_replay.sh', 'aclrtc_manifest.json', 'aclrtc_result.json',
}


def _log_jit_progress(log_path, message, *args):
    """父进程阶段也写入单任务 JIT 日志，便于和 worker 输出连续阅读。"""
    text = message % args if args else message
    logger.info('%s %s', _JIT_LOG_PREFIX, text)
    with log_path.open('a', encoding='utf-8') as stream:
        stream.write(f'{_JIT_LOG_PREFIX} INFO {text}\n')


def _remove_empty_work_directories(work):
    """只删除空目录，不清理 worker 报告、资源或失败产物。"""
    try:
        for directory, _, _ in os.walk(work, topdown=False):
            path = Path(directory)
            if not any(path.iterdir()):
                path.rmdir()
    except OSError as error:
        logger.warning('%s could not remove empty work directory %s: %s', _JIT_LOG_PREFIX, work, error)


def _log_jit_diagnostics(work, jit_log):
    """只记录 aclrtc 已生成的诊断文件路径，不解析或复制其内容。"""
    paths = sorted(path for path in work.rglob('aclrtc_*') if path.name in _ACLRTC_DIAGNOSTIC_FILES and path.is_file())
    if not paths:
        _log_jit_progress(jit_log, 'No aclrtc diagnostic files found; work_dir=%s', work)
        return
    for path in paths:
        _log_jit_progress(jit_log, 'aclrtc diagnostic file: %s', path)


def _select_compile_backend(shape, compile_mode, enable_sk, skip_static_bin_type=True):
    """按编译模式和 SK 配置初选后端，返回直接使用 OPC 的原因。

    返回 None 表示继续检查 JIT 输入条件，不保证 JIT 编译成功。
    """
    if not skip_static_bin_type:
        return '--skip-static-bin-type=false selects OPC directly'
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
    with open(log, 'a', encoding='utf-8') as stream:
        process = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT, env=env, start_new_session=True)
        try:
            return process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            logger.warning('%s worker timed out after %s seconds', _JIT_LOG_PREFIX, timeout)
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


def _publication_occurred(error, published, destination, stage):
    """先采用明确的发布状态，仅对无状态异常根据目录推断是否已发布。"""
    if published:
        return True
    if isinstance(error, SpecializationError):
        # 发布器明确报告未发布时，不把已有目标目录误认为本任务的发布结果。
        return error.published
    return destination is not None and destination.exists() and not stage.exists()


def _write_backend_report(report, logs, job_name, started):
    """在调用方原有的 finally 时点记录当前后端和整个任务耗时。"""
    report['elapsed_seconds'] = time.monotonic() - started
    (logs / (job_name + '_backend_result.json')).write_text(json.dumps(report, indent=2))


def _validate_output_pair(directory, basename, backend):
    """确认后端同时交付同名的二进制和描述文件。"""
    for suffix in ('.o', '.json'):
        if not (directory / (basename + suffix)).is_file():
            raise RuntimeError(backend + ' returned without a complete pair')


def _attempt_jit_compile(
    opc_json, result_root, work, logs, job_name, soc_version, log_level, enable_sk, report, started
):
    """监督一次 JIT 编译；发布成功返回 True，发布前失败返回 False 以回退 OPC。"""
    stage = work / 'jit'
    stage.mkdir()
    report['jit_attempted'] = True
    logger.info('%s starting JIT worker: input=%s, soc=%s', _JIT_LOG_PREFIX, opc_json, soc_version)
    # 仅调整 worker 的环境，临时文件归入本任务，优先加载当前这份 op_compile_tool。
    env = os.environ.copy()
    env['PYTHONDONTWRITEBYTECODE'] = '1'
    env['TMPDIR'] = str(work)
    env['ASCEND_SLOG_PRINT_TO_STDOUT'] = '1'
    env['ASCEND_GLOBAL_LOG_LEVEL'] = str(LOG_LEVEL_DICT[log_level.lower()])
    tool_root = str(Path(__file__).resolve().parents[1])
    env['PYTHONPATH'] = tool_root + os.pathsep + env.get('PYTHONPATH', '')
    jit_log = logs / (job_name + '_jit.log')
    _log_jit_progress(jit_log, 'Starting JIT worker: input=%s, work_dir=%s', opc_json, work)
    _log_jit_progress(jit_log, 'aclrtc logging configured: level=%s, stdout=1', log_level.lower())
    worker_report = work / 'worker_result.json'
    # 真正的 request 构造和 aclrtc 调用在 worker 模块中完成；这里负责启动和监督。
    command = [
        sys.executable,
        '-B',
        '-m',
        'static_compile.specialization.static_kernel_specialization',
        '--input',
        str(Path(opc_json).resolve()),
        '--stage',
        str(stage),
        '--soc',
        soc_version,
        '--log-level',
        log_level,
        '--result',
        str(worker_report),
    ]
    if enable_sk:
        command.append('--enable-sk')
    # 区分尚未发布与已经发布，后者发生错误时不能再混入另一套后端产物。
    destination = None
    published = False
    try:
        # 900 秒只限制 JIT 尝试，不是整个 JIT + OPC 任务的超时。
        rc = _run_worker(command, jit_log, env, 900)
        _log_jit_progress(jit_log, 'JIT worker finished: returncode=%s', rc)
        report['jit_returncode'] = rc
        report['jit_log'] = str(jit_log)
        # worker 崩溃或超时可能来不及写报告，不能假定该文件一定存在。
        if worker_report.is_file():
            report['jit'] = json.loads(worker_report.read_text())
        if rc != 0:
            _log_jit_progress(jit_log, 'JIT attempt failed; falling back to OPC: returncode=%s', rc)
            logger.warning('%s worker failed with return code %s; falling back to OPC', _JIT_LOG_PREFIX, rc)
            return False
        # 退出码成功仍须确认 .o/.json 成对存在；语义校验由 worker 完成。
        _log_jit_progress(jit_log, 'Validating generated ELF/JSON pair: %s', stage)
        _validate_output_pair(stage, report['basename'], 'JIT')
        destination = result_root
        _log_jit_progress(jit_log, 'Publishing JIT artifacts: destination=%s', destination)
        publish_artifacts(stage, destination)
        published = True
        report.update(backend='jit', jit_success=True, output_dir=str(destination), returncode=0)
        _log_jit_progress(jit_log, 'JIT artifacts published successfully: %s', destination)
        logger.info('%s artifacts published: %s', _JIT_LOG_PREFIX, destination)
        (logs / (job_name + '_compile_succ.log')).write_text(
            '[op_compiler][JIT static compile] aclrtc JIT succeeded\n', encoding='utf-8'
        )
        return True
    except Exception as error:
        report['jit_failure'] = str(error)
        if isinstance(error, ArtifactCollisionError):
            report.update(backend='jit', returncode=1, output_dir=str(destination), publication_conflict=True)
            raise RuntimeError('JIT output already exists; aborting packaging') from error
        # rename 成功后，fsync 或日志写入仍可能失败；产物可见后必须中止打包。
        if _publication_occurred(error, published, destination, stage):
            report.update(backend='jit', returncode=1, output_dir=str(destination), publication_uncertain=True)
            logger.error('%s failed after publication: %s', _JIT_LOG_PREFIX, error)
            raise RuntimeError('JIT publication occurred; aborting packaging') from error
        logger.warning('%s failed before publication: %s; falling back to OPC', _JIT_LOG_PREFIX, error)
        _log_jit_progress(jit_log, 'JIT attempt failed before publication; falling back to OPC: %s', error)
        return False
    finally:
        _log_jit_diagnostics(work, jit_log)
        _remove_empty_work_directories(work)
        # 即使上面 return 也会执行 finally；若继续回退，OPC 结束后会更新同一份报告。
        _write_backend_report(report, logs, job_name, started)


def compile_static_kernel_job(
    opc_json, compile_res_dir, kernel_meta_dir, soc_version, log_level, compile_log_dir, enable_sk,
    compile_mode='opc', skip_static_bin_type=True
):
    """统一编译一个 shape：先选择后端，JIT 未发布时失败则回退 OPC。

    保留 _opc_compile_single 的位置参数，增加编译模式供原进程池统一调度：
        opc_json：转换后的单 shape OPC 输入 JSON 路径。
        compile_res_dir：最终产物收集目录。
        kernel_meta_dir：编译中间文件目录。
        soc_version：目标芯片；log_level：日志等级。
        compile_log_dir：编译日志目录；enable_sk：是否要求 SK 能力。
        compile_mode：保留 tune/aoe 直接使用 OPC 的行为。
        skip_static_bin_type：False 时直接使用 OPC；True 时静态采集节点已由任务生成阶段过滤。

    两后端均直接输出到产物收集目录；JIT 部分发布后出错会中止打包。
    """
    # static_compilation 已导入本模块，延迟导入旧函数以避免模块初始化时循环依赖。
    from static_compile.static_compilation import _opc_compile_single

    started = time.monotonic()
    result_root, logs = Path(compile_res_dir), Path(compile_log_dir)
    result_root.mkdir(parents=True, exist_ok=True)
    logs.mkdir(parents=True, exist_ok=True)
    # 从输入描述取产物名；只允许文件名，防止拼接路径时越出任务目录。
    document = json.loads(Path(opc_json).read_text())
    job_name = Path(opc_json).stem
    jit_log = logs / (job_name + '_jit.log')
    _log_jit_progress(jit_log, 'Selecting compile backend: input=%s, enable_sk=%s', opc_json, enable_sk)
    shape = document['op_list'][0]
    basename = shape.get('bin_filename', job_name)
    if Path(basename).name != basename or basename in ('', '.', '..'):
        raise ValueError('bin_filename must be a basename')
    meta_root = Path(kernel_meta_dir)
    meta_root.mkdir(parents=True, exist_ok=True)
    report = {
        'op_type': document['op_type'],
        'basename': basename,
        'jit_attempted': False,
        'jit_success': False,
        'backend': None,
        'compile_mode': compile_mode,
    }
    # 这里是唯一的后端选择位置；不适用的任务不启动 JIT worker。
    skip_reason = _select_compile_backend(shape, compile_mode, enable_sk, skip_static_bin_type)
    if skip_reason is None:
        try:
            _log_jit_progress(jit_log, 'Preparing JIT inputs from collected dump JSON')
            # 父进程只判断准备能否成功，不使用返回上下文；worker 会独立准备并消费结果。
            prepare_specialization_inputs(opc_json, soc_version)
            _log_jit_progress(jit_log, 'JIT input preparation and preflight checks passed')
        except Exception as error:
            # 输入确定不满足条件时直接交给 OPC，不启动 worker 或调用 aclrtc。
            skip_reason = 'JIT input preparation failed: ' + str(error)
    if skip_reason is not None:
        report['jit_skip_reason'] = skip_reason
        logger.info('%s skipped: %s', _JIT_LOG_PREFIX, skip_reason)
        _log_jit_progress(jit_log, 'Skipping JIT; using OPC: %s', skip_reason)
    else:
        # JIT 中间文件统一归入 kernel_meta_static_*，不进入最终产物扫描目录。
        work = meta_root / ('jit_specialization_kernel_meta_' + basename)
        work.mkdir()
        report['work_dir'] = str(work)
        if _attempt_jit_compile(
            opc_json, result_root, work, logs, job_name, soc_version, log_level, enable_sk, report, started
        ):
            return 0

    # 主动跳过 JIT 和 JIT 发布前失败共用此 OPC 分支，原始输入和 SK 配置不变。
    try:
        _log_jit_progress(jit_log, 'Starting OPC compilation: input=%s', opc_json)
        rc = _opc_compile_single(
            opc_json,
            compile_res_dir=str(result_root),
            kernel_meta_dir=kernel_meta_dir,
            soc_version=soc_version,
            log_level=log_level,
            compile_log_dir=str(logs),
            enable_sk=enable_sk,
        )
        # 该目录名由 OPC 内部决定，只在子进程结束后加前缀，保留失败时的诊断文件。
        opc_work = meta_root / ('kernel_meta_' + basename)
        if opc_work.is_dir():
            opc_work.rename(meta_root / ('opc_' + opc_work.name))
        if rc == 0:
            _validate_output_pair(result_root, basename, 'OPC')
            report['output_dir'] = str(result_root)
        _log_jit_progress(jit_log, 'OPC compilation finished: returncode=%s, output_dir=%s', rc, result_root)
        # jit_success 保持 False，最终成功后端为 OPC，不掩盖前面的 JIT 失败。
        report.update(backend='opc', returncode=rc)
    except Exception as error:
        rc = 1
        report.update(backend='opc', returncode=rc, opc_failure=str(error))
    finally:
        # 写最终后端与总耗时，失败标记供原日志汇总流程识别。
        _write_backend_report(report, logs, job_name, started)
        if report.get('returncode') != 0:
            # 原 OPC 已写出的诊断保留原文；只有没有诊断时才补失败标记供汇总读取。
            error_log = logs / (job_name + '_compile_error.log')
            if not error_log.exists():
                error_log.write_text(json.dumps(report, indent=2))
    return rc
