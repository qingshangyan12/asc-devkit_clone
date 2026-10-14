import json
import pathlib
import tempfile
import unittest
from unittest.mock import patch

ROOT = pathlib.Path(__file__).resolve().parents[2]


class BackendTests(unittest.TestCase):
    def test_direct_opc_failure_preserves_original_compiler_diagnostics(self):
        from static_compile import static_compile_backend as backend
        diagnostics = 'OPC compiler diagnostic: original input validation failed\n'
        for mode in ('tune', 'aoe'):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as temp:
                root = pathlib.Path(temp)
                inp = root/'input.json'
                inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [
                    {'bin_filename': 'test'}]}))

                def fallback(opc_json, **kwargs):
                    (pathlib.Path(kwargs['compile_log_dir'])/'input_compile_error.log').write_text(
                        diagnostics)
                    return 1

                with patch.object(backend, '_run_worker') as worker, \
                     patch('static_compile.static_compilation._opc_compile_single',
                           side_effect=fallback) as opc:
                    rc = backend.compile_static_kernel_job(
                        str(inp), str(root/'results'), str(root/'meta'),
                        'Ascend910B4', 'info', str(root/'logs'), False,
                        compile_mode=mode)
                self.assertEqual(rc, 1)
                worker.assert_not_called()
                opc.assert_called_once()
                report = json.loads((root/'logs'/'input_backend_result.json').read_text())
                self.assertEqual(report['backend'], 'opc')
                self.assertEqual(report['returncode'], 1)
                self.assertFalse(report['jit_attempted'])
                self.assertEqual((root/'logs'/'input_compile_error.log').read_text(), diagnostics)

    def test_backend_routes_before_worker_and_retains_opc_fallback(self):
        from static_compile import static_compile_backend as backend

        combine = lambda value: {'key': 'super_kernel_sub_combine',
                                 'value': value, 'type': 'addition'}
        sk_info = {'key': 'super_kernel_sub_info', 'type': 'addition',
                   'value': {'enable_super_kernel': True,
                             'super_kernel_sub_loc': 'middle'}}
        cases = [
            ('ordinary', 'opc', False, [], True),
            ('aclgraph', 'opc', True, [sk_info, combine(True)], True),
            ('ge', 'opc', True, [sk_info, combine(False)], False),
            ('unspecified_sk', 'opc', True, [sk_info], False),
            ('combine_without_info', 'opc', True, [combine(True)], False),
            ('string_combine', 'opc', True, [sk_info, combine('True')], False),
            ('conflicting_combine', 'opc', True,
             [sk_info, combine(False), combine(True)], False),
            ('malformed_settings', 'opc', True, {}, False),
            ('tune_ordinary', 'tune', False, [], False),
            ('aoe_ordinary', 'aoe', False, [], False),
            ('tune_aclgraph', 'tune', True, [sk_info, combine(True)], False),
            ('aoe_aclgraph', 'aoe', True, [sk_info, combine(True)], False),
        ]
        for name, mode, enable_sk, settings, attempted in cases:
            with self.subTest(case=name), tempfile.TemporaryDirectory() as temp:
                root = pathlib.Path(temp)
                inp = root/'input.json'
                inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{
                    'bin_filename': 'test', 'extra_settings': settings}]}))

                def fallback(opc_json, **kwargs):
                    self.assertEqual(opc_json, str(inp))
                    self.assertEqual(kwargs['enable_sk'], enable_sk)
                    out = pathlib.Path(kwargs['compile_res_dir'])
                    (out/'test.o').write_bytes(b'OPC')
                    (out/'test.json').write_text('{}')
                    return 0

                with patch.object(backend, 'preflight_shape_config'), \
                     patch.object(backend, '_run_worker', return_value=1) as worker, \
                     patch('static_compile.static_compilation._opc_compile_single',
                           side_effect=fallback) as opc:
                    rc = backend.compile_static_kernel_job(
                        str(inp), str(root/'results'), str(root/'meta'),
                        'Ascend910B4', 'info', str(root/'logs'), enable_sk,
                        compile_mode=mode)
                self.assertEqual(rc, 0)
                self.assertEqual(worker.call_count, int(attempted))
                opc.assert_called_once()
                report = json.loads((root/'logs'/'input_backend_result.json').read_text())
                self.assertIs(report['jit_attempted'], attempted)
                if not attempted:
                    self.assertTrue(report['jit_skip_reason'])
                self.assertEqual(report['backend'], 'opc')
                self.assertFalse(report['jit_success'])
                self.assertEqual(next((root/'results').rglob('test.o')).read_bytes(), b'OPC')

    def test_successful_jit_does_not_call_opc(self):
        from static_compile import static_compile_backend as backend
        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root/'input.json'
            inp.write_text(json.dumps({'op_type': 'Test', 'op_list': [{'bin_filename': 'test'}]}))

            def worker(command, *args):
                stage = pathlib.Path(command[command.index('--stage')+1])
                (stage/'test.o').write_bytes(b'JIT')
                (stage/'test.json').write_text('{}')
                return 0

            with patch.object(backend, 'preflight_shape_config'), \
                 patch.object(backend, '_run_worker', worker), \
                 patch('static_compile.static_compilation._opc_compile_single') as opc:
                self.assertEqual(backend.compile_static_kernel_job(
                    str(inp), str(root/'results'), str(root/'meta'),
                    'Ascend910B4', 'info', str(root/'logs'), False), 0)
                opc.assert_not_called()
            report = json.loads((root/'logs'/'input_backend_result.json').read_text())
            self.assertTrue(report['jit_attempted'])
            self.assertTrue(report['jit_success'])
            self.assertEqual(report['backend'], 'jit')

    def test_timeout_kills_descendants_even_when_leader_exits_after_term(self):
        import signal
        import subprocess
        from unittest.mock import Mock
        from static_compile.static_compile_backend import _run_worker
        process = Mock(pid=12345)
        process.wait.side_effect = [subprocess.TimeoutExpired('worker', 1), 0, 0]
        with tempfile.TemporaryDirectory() as temp:
            with patch('static_compile.static_compile_backend.subprocess.Popen', return_value=process), patch('static_compile.static_compile_backend.os.killpg') as kill:
                self.assertEqual(_run_worker(['worker'], pathlib.Path(temp)/'worker.log', {}, 1), 124)
            self.assertEqual([call.args for call in kill.call_args_list],
                             [(12345, signal.SIGTERM), (12345, signal.SIGKILL)])

    def test_backend_module_exists(self):
        self.assertTrue((ROOT / 'static_compile_backend.py').is_file(), 'JIT/OPC dispatcher not implemented')

    def test_missing_inputs_fall_back_once_and_publish_pair(self):
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
                rc = compile_static_kernel_job(str(inp), str(root/'results'), str(root/'meta'),
                                               'Ascend910B4', 'debug', str(root/'logs'), False)
            self.assertEqual(rc, 0)
            self.assertEqual(len(calls), 1)
            self.assertEqual(len(list((root/'results').rglob('test.o'))), 1)
            self.assertEqual(len(list((root/'results').rglob('test.json'))), 1)
            report = json.loads((root/'logs'/'input_backend_result.json').read_text())
            self.assertEqual(report['backend'], 'opc')
            self.assertFalse(report['jit_success'])

    def test_failed_opc_never_publishes_half_pair(self):
        if not (ROOT / 'static_compile_backend.py').is_file():
            self.fail('JIT/OPC dispatcher not implemented')
        from static_compile.static_compile_backend import compile_static_kernel_job
        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root/'input.json'
            inp.write_text(json.dumps({'op_type':'Test','op_list':[{'bin_filename':'test'}]}))
            def fallback(opc_json, **kwargs):
                (pathlib.Path(kwargs['compile_res_dir'])/'test.o').write_bytes(b'partial')
                return 1
            with patch('static_compile.static_compilation._opc_compile_single', fallback):
                rc = compile_static_kernel_job(str(inp),str(root/'results'),str(root/'meta'),
                                               'Ascend910B4','debug',str(root/'logs'),False)
            self.assertNotEqual(rc,0)
            self.assertFalse(list((root/'results').rglob('*.o')))


class PublicationBoundaryTests(unittest.TestCase):
    def test_preexisting_destination_is_not_this_job_commit(self):
        from static_compile import static_compile_backend as backend
        from static_compile.specialization.errors import SpecializationError
        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            inp = root/'input.json'
            inp.write_text(json.dumps({'op_type':'Test','op_list':[{'bin_filename':'test'}]}))
            def worker(command, *args):
                stage = pathlib.Path(command[command.index('--stage')+1])
                (stage/'test.o').write_bytes(b'ELF')
                (stage/'test.json').write_text('{}')
                return 0
            real_publish = backend.publish_artifacts
            def collision(stage, dest):
                if stage.name == 'jit':
                    dest.mkdir()
                    (dest/'existing.txt').write_text('keep')
                    raise SpecializationError('publish', 'destination already exists')
                return real_publish(stage, dest)
            def fallback(*args, **kwargs):
                output = pathlib.Path(kwargs['compile_res_dir'])
                (output/'test.o').write_bytes(b'OPC')
                (output/'test.json').write_text('{}')
                return 0
            with patch.object(backend, 'preflight_shape_config'), patch.object(backend, '_run_worker', worker), patch.object(backend, 'publish_artifacts', collision), patch('static_compile.static_compilation._opc_compile_single', side_effect=fallback) as opc:
                self.assertEqual(backend.compile_static_kernel_job(str(inp),str(root/'results'),str(root/'meta'),'Ascend910B4','info',str(root/'logs'),False), 0)
                self.assertEqual(opc.call_count, 1)
            self.assertEqual(next((root/'results').rglob('existing.txt')).read_text(), 'keep')
            self.assertEqual(next((root/'results').rglob('test.o')).read_bytes(), b'OPC')

    def test_post_rename_failure_never_falls_back(self):
        from static_compile.static_compile_backend import compile_static_kernel_job
        with tempfile.TemporaryDirectory() as temp:
            root=pathlib.Path(temp)
            inp=root/'input.json'
            inp.write_text(json.dumps({'op_type':'Test','op_list':[{'bin_filename':'test'}]}))
            def worker(command, *args):
                stage=pathlib.Path(command[command.index('--stage')+1])
                (stage/'test.o').write_bytes(b'ELF')
                (stage/'test.json').write_text('{}')
                return 0
            def publish(stage,dest):
                stage.rename(dest)
                raise OSError('parent fsync failed after rename')
            with patch('static_compile.static_compile_backend.preflight_shape_config'), patch('static_compile.static_compile_backend._run_worker',worker), patch('static_compile.static_compile_backend.publish_artifacts',publish), patch('static_compile.static_compilation._opc_compile_single') as opc:
                with self.assertRaisesRegex(RuntimeError,'aborting packaging'):
                    compile_static_kernel_job(str(inp),str(root/'results'),str(root/'meta'),'Ascend910B4','info',str(root/'logs'),False)
                opc.assert_not_called()
            self.assertEqual(len(list((root/'results').rglob('test.o'))),1)

if __name__ == '__main__':
    unittest.main()
