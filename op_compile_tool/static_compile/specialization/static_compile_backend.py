"""Single-shape JIT attempt with isolated, unchanged OPC fallback."""
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import time
import uuid

from .artifact_publisher import publish_artifacts
from .errors import SpecializationError


def _run_worker(command, log, env, timeout):
    with open(log, 'w', encoding='utf-8') as stream:
        process = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT,
                                   env=env, start_new_session=True)
        try:
            return process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            try:
                os.killpg(process.pid, signal.SIGTERM)
            except ProcessLookupError:
                pass
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                pass
            finally:
                # The leader can exit while compiler children retain this group.
                # Always terminate the group before OPC is allowed to start.
                try:
                    os.killpg(process.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                process.wait()
            return 124



def compile_static_kernel_job(opc_json, compile_res_dir, kernel_meta_dir,
                              soc_version, log_level, compile_log_dir, enable_sk):
    """Match _opc_compile_single's signature/return code; never scan staging."""
    from static_compile.static_compilation import _opc_compile_single
    started = time.monotonic()
    result_root, logs = Path(compile_res_dir), Path(compile_log_dir)
    result_root.mkdir(parents=True, exist_ok=True)
    logs.mkdir(parents=True, exist_ok=True)
    document = json.loads(Path(opc_json).read_text())
    job_name = Path(opc_json).stem
    basename = document['op_list'][0].get('bin_filename', job_name)
    if Path(basename).name != basename or basename in ('', '.', '..'):
        raise ValueError('bin_filename must be a basename')
    # Stage outside result_root: existing scanners recursively accept .json/.o.
    work = Path(tempfile.mkdtemp(prefix='specialization-', dir=Path(kernel_meta_dir).parent))
    stage = work / 'jit'
    stage.mkdir()
    report = {'op_type': document['op_type'], 'basename': basename,
              'jit_success': False, 'backend': None, 'work_dir': str(work)}
    env = os.environ.copy()
    env['PYTHONDONTWRITEBYTECODE'] = '1'
    env['TMPDIR'] = str(work)
    tool_root = str(Path(__file__).resolve().parents[2])
    env['PYTHONPATH'] = tool_root + os.pathsep + env.get('PYTHONPATH', '')
    jit_log = logs / (job_name + '_jit.log')
    worker_report = work / 'worker_result.json'
    command = [sys.executable, '-B', '-m', 'static_compile.specialization.static_kernel_specialization',
               '--input', str(Path(opc_json).resolve()), '--stage', str(stage),
               '--soc', soc_version, '--log-level', log_level, '--result', str(worker_report)]
    if enable_sk:
        command.append('--enable-sk')
    destination = None
    published = False
    try:
        rc = _run_worker(command, jit_log, env, 900)
        report['jit_returncode'] = rc
        report['jit_log'] = str(jit_log)
        if worker_report.is_file():
            report['jit'] = json.loads(worker_report.read_text())
        if rc == 0:
            for suffix in ('.o', '.json'):
                if not (stage / (basename + suffix)).is_file():
                    raise RuntimeError('JIT returned without a complete pair')
            destination = result_root / ('jit-' + uuid.uuid4().hex)
            publish_artifacts(stage, destination)
            published = True
            report.update(backend='jit', jit_success=True, output_dir=str(destination), returncode=0)
            (logs / (job_name + '_compile_succ.log')).write_text('aclrtc JIT succeeded\n', encoding='utf-8')
            return 0
    except Exception as error:
        report['jit_failure'] = str(error)
        # A rename may have succeeded before fsync/logging failed. Never publish
        # a second backend for the same shape after this commit point.
        if published or (isinstance(error, SpecializationError) and error.published) or (
                not isinstance(error, SpecializationError) and destination is not None
                and destination.exists() and not stage.exists()):
            report.update(backend='jit', returncode=1, output_dir=str(destination),
                          publication_uncertain=True)
            raise RuntimeError('JIT publication occurred; aborting packaging') from error
    finally:
        # Final report is written again after OPC, if fallback is required.
        report['elapsed_seconds'] = time.monotonic() - started
        (logs / (job_name + '_backend_result.json')).write_text(json.dumps(report, indent=2))

    opc_stage = work / 'opc'
    opc_stage.mkdir()
    destination = None
    published = False
    try:
        opc_meta = Path(kernel_meta_dir) / ('opc-' + uuid.uuid4().hex)
        opc_meta.mkdir(parents=True)
        rc = _opc_compile_single(opc_json, compile_res_dir=str(opc_stage),
                                 kernel_meta_dir=str(opc_meta), soc_version=soc_version,
                                 log_level=log_level, compile_log_dir=str(logs), enable_sk=enable_sk)
        if rc == 0:
            for suffix in ('.o', '.json'):
                if not (opc_stage / (basename + suffix)).is_file():
                    raise RuntimeError('OPC returned without a complete pair')
            destination = result_root / ('opc-' + uuid.uuid4().hex)
            publish_artifacts(opc_stage, destination)
            published = True
            report['output_dir'] = str(destination)
        report.update(backend='opc', returncode=rc)
    except Exception as error:
        rc = 1
        report.update(backend='opc', returncode=rc, opc_failure=str(error))
        if published or (isinstance(error, SpecializationError) and error.published) or (
                not isinstance(error, SpecializationError) and destination is not None
                and destination.exists() and not opc_stage.exists()):
            report['publication_uncertain'] = True
            raise RuntimeError('OPC publication occurred; aborting packaging') from error
    finally:
        report['elapsed_seconds'] = time.monotonic() - started
        (logs / (job_name + '_backend_result.json')).write_text(json.dumps(report, indent=2))
        if report.get('returncode') != 0:
            (logs / (job_name + '_compile_error.log')).write_text(json.dumps(report, indent=2))
    return rc
