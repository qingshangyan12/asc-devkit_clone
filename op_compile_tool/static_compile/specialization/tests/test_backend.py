import json
import os
import pathlib
import tempfile
import unittest
from unittest.mock import patch

ROOT = pathlib.Path(__file__).resolve().parents[2]


class BackendTests(unittest.TestCase):
    def _write_aclrtc_diagnostics(self, work):
        directory = work / 'aclrtc-resource-test' / 'custom' / 'materialized'
        directory.mkdir(parents=True)
        paths = []
        for name in ('aclrtc_compile.log', 'aclrtc_replay.sh', 'aclrtc_manifest.json', 'aclrtc_result.json'):
            path = directory / name
            path.write_text('diagnostic content')
            paths.append(path)
        return paths

    def test_direct_opc_failure_preserves_original_compiler_diagnostics(self):
        from static_compile import static_compile_backend as backend

        diagnostics = 'OPC compiler diagnostic: original input validation failed\n'
        for mode in ('tune', 'aoe'):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as temp:
                root = pathlib.Path(temp)
                inp = root / 'input.json'
                inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))

                def fallback(opc_json, **kwargs):
                    (pathlib.Path(kwargs['compile_log_dir']) / 'input_compile_error.log').write_text(diagnostics)
                    meta = pathlib.Path(kwargs['kernel_meta_dir']) / 'kernel_meta_test'
                    meta.mkdir()
                    (meta / 'diagnostic.log').write_text(diagnostics)
                    return 1

                with (
                    patch.object(backend, '_run_worker') as worker,
                    patch('static_compile.static_compilation._opc_compile_single', side_effect=fallback) as opc,
                ):
                    rc = backend.compile_static_kernel_job(
                        str(inp),
                        str(root / 'results'),
                        str(root / 'meta'),
                        'Ascend910B4',
                        'info',
                        str(root / 'logs'),
                        False,
                        compile_mode=mode,
                    )
                self.assertEqual(rc, 1)
                worker.assert_not_called()
                opc.assert_called_once()
                report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
                self.assertEqual(report['backend'], 'opc')
                self.assertEqual(report['returncode'], 1)
                self.assertFalse(report['jit_attempted'])
                self.assertEqual((root / 'logs' / 'input_compile_error.log').read_text(), diagnostics)
                self.assertEqual((root / 'meta' / 'opc_kernel_meta_test' / 'diagnostic.log').read_text(), diagnostics)
                self.assertFalse((root / 'meta' / 'kernel_meta_test').exists())
                self.assertNotIn('work_dir', report)

    def test_backend_routes_before_worker_and_retains_opc_fallback(self):
        from static_compile import static_compile_backend as backend

        def combine(value):
            return {'key': 'super_kernel_sub_combine', 'value': value, 'type': 'addition'}

        sk_info = {
            'key': 'super_kernel_sub_info',
            'type': 'addition',
            'value': {'enable_super_kernel': True, 'super_kernel_sub_loc': 'middle'},
        }
        cases = [
            ('ordinary', 'opc', False, [], True),
            ('aclgraph', 'opc', True, [sk_info, combine(True)], True),
            ('ge', 'opc', True, [sk_info, combine(False)], False),
            ('unspecified_sk', 'opc', True, [sk_info], False),
            ('combine_without_info', 'opc', True, [combine(True)], False),
            ('string_combine', 'opc', True, [sk_info, combine('True')], False),
            ('conflicting_combine', 'opc', True, [sk_info, combine(False), combine(True)], False),
            ('malformed_settings', 'opc', True, {}, False),
            ('tune_ordinary', 'tune', False, [], False),
            ('aoe_ordinary', 'aoe', False, [], False),
            ('tune_aclgraph', 'tune', True, [sk_info, combine(True)], False),
            ('aoe_aclgraph', 'aoe', True, [sk_info, combine(True)], False),
        ]
        for name, mode, enable_sk, settings, attempted in cases:
            with self.subTest(case=name), tempfile.TemporaryDirectory() as temp:
                root = pathlib.Path(temp)
                inp = root / 'input.json'
                inp.write_text(
                    json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test', 'extra_settings': settings}]})
                )

                def fallback(opc_json, **kwargs):
                    self.assertEqual(opc_json, str(inp))
                    self.assertEqual(kwargs['enable_sk'], enable_sk)
                    self.assertEqual(kwargs['compile_res_dir'], str(root / 'results'))
                    self.assertEqual(kwargs['kernel_meta_dir'], str(root / 'meta'))
                    (root / 'meta' / 'kernel_meta_test').mkdir()
                    out = pathlib.Path(kwargs['compile_res_dir'])
                    (out / 'test.o').write_bytes(b'OPC')
                    (out / 'test.json').write_text('{}')
                    return 0

                diagnostics = []

                def failing_worker(command, *args):
                    stage = pathlib.Path(command[command.index('--stage') + 1])
                    diagnostics.extend(self._write_aclrtc_diagnostics(stage.parent))
                    return 1

                with (
                    patch.object(backend, 'prepare_specialization_inputs'),
                    patch.object(backend, '_run_worker', side_effect=failing_worker) as worker,
                    patch('static_compile.static_compilation._opc_compile_single', side_effect=fallback) as opc,
                ):
                    rc = backend.compile_static_kernel_job(
                        str(inp),
                        str(root / 'results'),
                        str(root / 'meta'),
                        'Ascend910B4',
                        'info',
                        str(root / 'logs'),
                        enable_sk,
                        compile_mode=mode,
                    )
                self.assertEqual(rc, 0)
                self.assertEqual(worker.call_count, int(attempted))
                opc.assert_called_once()
                report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
                self.assertIs(report['jit_attempted'], attempted)
                if not attempted:
                    self.assertTrue(report['jit_skip_reason'])
                self.assertEqual(report['backend'], 'opc')
                self.assertFalse(report['jit_success'])
                self.assertEqual((root / 'results' / 'test.o').read_bytes(), b'OPC')
                self.assertEqual(report['output_dir'], str(root / 'results'))
                self.assertTrue((root / 'meta' / 'opc_kernel_meta_test').is_dir())
                self.assertFalse((root / 'meta' / 'kernel_meta_test').exists())
                if attempted:
                    self.assertEqual(report['work_dir'], str(root / 'meta' / 'jit_specialization_kernel_meta_test'))
                    log = (root / 'logs' / 'input_jit.log').read_text()
                    self.assertEqual(len(diagnostics), 4)
                    for path in diagnostics:
                        self.assertTrue(path.is_file())
                        self.assertIn('aclrtc diagnostic file: ' + str(path), log)
                else:
                    self.assertNotIn('work_dir', report)

    def test_successful_jit_does_not_call_opc(self):
        from static_compile import static_compile_backend as backend

        for level, expected in (('debug', '0'), ('INFO', '1'), ('warning', '2'), ('error', '3')):
            with self.subTest(level=level), tempfile.TemporaryDirectory() as temp:
                root = pathlib.Path(temp)
                inp = root / 'input.json'
                inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))
                diagnostics = []

                def worker(command, log, env, timeout):
                    self.assertEqual(env['ASCEND_SLOG_PRINT_TO_STDOUT'], '1')
                    self.assertEqual(env['ASCEND_GLOBAL_LOG_LEVEL'], expected)
                    self.assertIsNot(env, os.environ)
                    stage = pathlib.Path(command[command.index('--stage') + 1])
                    self.assertEqual(stage.parent, root / 'meta' / 'jit_specialization_kernel_meta_test')
                    diagnostics.extend(self._write_aclrtc_diagnostics(stage.parent))
                    (stage / 'test.o').write_bytes(b'JIT')
                    (stage / 'test.json').write_text('{}')
                    return 0

                parent_values = {'ASCEND_SLOG_PRINT_TO_STDOUT': '0', 'ASCEND_GLOBAL_LOG_LEVEL': '7'}
                with (
                    patch.dict(os.environ, parent_values),
                    patch.object(backend, 'prepare_specialization_inputs'),
                    patch.object(backend, '_run_worker', worker),
                    patch('static_compile.static_compilation._opc_compile_single') as opc,
                ):
                    parent_environment = dict(os.environ)
                    rc = backend.compile_static_kernel_job(
                        str(inp), str(root / 'results'), str(root / 'meta'), 'Ascend910B4',
                        level, str(root / 'logs'), False,
                    )
                    self.assertEqual(rc, 0)
                    self.assertEqual(dict(os.environ), parent_environment)
                    opc.assert_not_called()
                report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
                self.assertTrue(report['jit_attempted'])
                self.assertTrue(report['jit_success'])
                self.assertEqual(report['backend'], 'jit')
                self.assertEqual(report['output_dir'], str(root / 'results'))
                self.assertEqual((root / 'results' / 'test.o').read_bytes(), b'JIT')
                self.assertEqual((root / 'results' / 'test.json').read_text(), '{}')
                self.assertFalse(list((root / 'results').glob('jit-*')))
                log = (root / 'logs' / 'input_jit.log').read_text()
                self.assertEqual(len(diagnostics), 4)
                for path in diagnostics:
                    self.assertTrue(path.is_file())
                    self.assertIn('aclrtc diagnostic file: ' + str(path), log)
                self.assertFalse(list(root.glob('specialization-*')))

    def test_missing_aclrtc_diagnostics_are_logged(self):
        from static_compile.static_compile_backend import _log_jit_diagnostics

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            work = root / 'work'
            work.mkdir()
            log = root / 'input_jit.log'
            _log_jit_diagnostics(work, log)
            self.assertIn('No aclrtc diagnostic files found; work_dir=' + str(work), log.read_text())

    def test_empty_work_cleanup_retains_worker_reports(self):
        from static_compile.static_compile_backend import _remove_empty_work_directories

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            empty = root / 'empty'
            (empty / 'nested' / 'empty').mkdir(parents=True)
            _remove_empty_work_directories(empty)
            self.assertFalse(empty.exists())
            retained = root / 'retained'
            (retained / 'empty' / 'nested').mkdir(parents=True)
            report = retained / 'worker_result.json'
            report.write_text('{"success": false}')
            _remove_empty_work_directories(retained)
            self.assertEqual(report.read_text(), '{"success": false}')
            self.assertEqual(list(retained.iterdir()), [report])

    def test_timeout_kills_descendants_even_when_leader_exits_after_term(self):
        import signal
        import subprocess
        from unittest.mock import Mock
        from static_compile.static_compile_backend import _run_worker

        process = Mock(pid=12345)
        process.wait.side_effect = [subprocess.TimeoutExpired('worker', 1), 0, 0]
        with tempfile.TemporaryDirectory() as temp:
            with (
                patch('static_compile.static_compile_backend.subprocess.Popen', return_value=process),
                patch('static_compile.static_compile_backend.os.killpg') as kill,
            ):
                self.assertEqual(_run_worker(['worker'], pathlib.Path(temp) / 'worker.log', {}, 1), 124)
            self.assertEqual(
                [call.args for call in kill.call_args_list], [(12345, signal.SIGTERM), (12345, signal.SIGKILL)]
            )

    def test_backend_module_exists(self):
        self.assertTrue((ROOT / 'static_compile_backend.py').is_file(), 'JIT/OPC dispatcher not implemented')

    def test_missing_inputs_fall_back_once_and_write_pair(self):
        if not (ROOT / 'static_compile_backend.py').is_file():
            self.fail('JIT/OPC dispatcher not implemented')
        from static_compile.static_compile_backend import compile_static_kernel_job

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root / 'input.json'
            inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))
            calls = []

            def fallback(opc_json, **kwargs):
                calls.append(opc_json)
                out = pathlib.Path(kwargs['compile_res_dir'])
                (out / 'test.o').write_bytes(b'\x7fELFtest')
                (out / 'test.json').write_text('{}')
                return 0

            with patch('static_compile.static_compilation._opc_compile_single', fallback):
                rc = compile_static_kernel_job(
                    str(inp),
                    str(root / 'results'),
                    str(root / 'meta'),
                    'Ascend910B4',
                    'debug',
                    str(root / 'logs'),
                    False,
                )
            self.assertEqual(rc, 0)
            self.assertEqual(len(calls), 1)
            self.assertEqual(len(list((root / 'results').rglob('test.o'))), 1)
            self.assertEqual(len(list((root / 'results').rglob('test.json'))), 1)
            report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
            self.assertEqual(report['backend'], 'opc')
            self.assertFalse(report['jit_success'])

    def test_failed_opc_retains_original_output_and_reports_failure(self):
        if not (ROOT / 'static_compile_backend.py').is_file():
            self.fail('JIT/OPC dispatcher not implemented')
        from static_compile.static_compile_backend import compile_static_kernel_job

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root / 'input.json'
            inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))

            def fallback(opc_json, **kwargs):
                (pathlib.Path(kwargs['compile_res_dir']) / 'test.o').write_bytes(b'partial')
                return 1

            with patch('static_compile.static_compilation._opc_compile_single', fallback):
                rc = compile_static_kernel_job(
                    str(inp),
                    str(root / 'results'),
                    str(root / 'meta'),
                    'Ascend910B4',
                    'debug',
                    str(root / 'logs'),
                    False,
                )
            self.assertNotEqual(rc, 0)
            self.assertEqual((root / 'results' / 'test.o').read_bytes(), b'partial')
            report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
            self.assertEqual(report['backend'], 'opc')
            self.assertEqual(report['returncode'], 1)
            self.assertNotIn('output_dir', report)


class PublicationBoundaryTests(unittest.TestCase):
    def test_existing_artifact_aborts_without_opc_overwrite(self):
        from static_compile import static_compile_backend as backend

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root / 'input.json'
            inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))
            result = root / 'results'
            result.mkdir()
            (result / 'test.o').write_bytes(b'keep')

            def worker(command, *args):
                stage = pathlib.Path(command[command.index('--stage') + 1])
                (stage / 'test.o').write_bytes(b'ELF')
                (stage / 'test.json').write_text('{}')
                return 0

            with (
                patch.object(backend, 'prepare_specialization_inputs'),
                patch.object(backend, '_run_worker', worker),
                patch('static_compile.static_compilation._opc_compile_single') as opc,
            ):
                with self.assertRaisesRegex(RuntimeError, 'output already exists; aborting packaging'):
                    backend.compile_static_kernel_job(
                        str(inp),
                        str(root / 'results'),
                        str(root / 'meta'),
                        'Ascend910B4',
                        'info',
                        str(root / 'logs'),
                        False,
                    )
                opc.assert_not_called()
            self.assertEqual((result / 'test.o').read_bytes(), b'keep')
            self.assertFalse((result / 'test.json').exists())
            report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
            self.assertTrue(report['publication_conflict'])
            self.assertEqual(report['returncode'], 1)

    def test_post_rename_failure_never_falls_back(self):
        from static_compile.specialization import artifact_publisher as publisher
        from static_compile.static_compile_backend import compile_static_kernel_job

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root / 'input.json'
            inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))

            def worker(command, *args):
                stage = pathlib.Path(command[command.index('--stage') + 1])
                (stage / 'test.o').write_bytes(b'ELF')
                (stage / 'test.json').write_text('{}')
                return 0

            real_fsync = publisher.os.fsync

            def fsync(fd):
                if (root / 'results' / 'test.o').exists():
                    raise OSError('parent fsync failed after rename')
                return real_fsync(fd)

            with (
                patch('static_compile.static_compile_backend.prepare_specialization_inputs'),
                patch('static_compile.static_compile_backend._run_worker', worker),
                patch.object(publisher.os, 'fsync', side_effect=fsync),
                patch('static_compile.static_compilation._opc_compile_single') as opc,
            ):
                with self.assertRaisesRegex(RuntimeError, 'aborting packaging'):
                    compile_static_kernel_job(
                        str(inp),
                        str(root / 'results'),
                        str(root / 'meta'),
                        'Ascend910B4',
                        'info',
                        str(root / 'logs'),
                        False,
                    )
                opc.assert_not_called()
            self.assertEqual(len(list((root / 'results').rglob('test.o'))), 1)

    def test_second_artifact_rename_failure_never_falls_back(self):
        import ctypes
        import errno
        from types import SimpleNamespace
        from unittest.mock import Mock
        from static_compile import static_compile_backend as backend
        from static_compile.specialization import artifact_publisher as publisher

        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root / 'input.json'
            inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))
            real_rename = ctypes.CDLL(None, use_errno=True).renameat2
            renamed = []

            def rename(*args):
                if renamed:
                    ctypes.set_errno(errno.EIO)
                    return -1
                rc = real_rename(*args)
                self.assertEqual(rc, 0)
                renamed.append(args[1])
                return rc

            def worker(command, *args):
                stage = pathlib.Path(command[command.index('--stage') + 1])
                (stage / 'test.o').write_bytes(b'ELF')
                (stage / 'test.json').write_text('{}')
                return 0

            library = SimpleNamespace(renameat2=Mock(side_effect=rename))
            with (
                patch.object(backend, 'prepare_specialization_inputs'),
                patch.object(backend, '_run_worker', worker),
                patch.object(publisher.ctypes, 'CDLL', return_value=library),
                patch('static_compile.static_compilation._opc_compile_single') as opc,
            ):
                with self.assertRaisesRegex(RuntimeError, 'publication occurred; aborting packaging'):
                    backend.compile_static_kernel_job(
                        str(inp), str(root / 'results'), str(root / 'meta'), 'Ascend910B4',
                        'info', str(root / 'logs'), False,
                    )
                opc.assert_not_called()
            self.assertEqual(len(renamed), 1)
            self.assertEqual((root / 'results' / 'test.json').read_text(), '{}')
            self.assertFalse((root / 'results' / 'test.o').exists())
            report = json.loads((root / 'logs' / 'input_backend_result.json').read_text())
            self.assertTrue(report['publication_uncertain'])
            self.assertEqual(report['returncode'], 1)


if __name__ == '__main__':
    unittest.main()
