import importlib.util
import json
import pathlib
import tempfile
import unittest
from unittest.mock import patch

ROOT = pathlib.Path(__file__).resolve().parents[1]


class BackendTests(unittest.TestCase):
    def test_timeout_kills_descendants_even_when_leader_exits_after_term(self):
        import signal
        import subprocess
        from unittest.mock import Mock
        from static_compile.specialization.static_compile_backend import _run_worker
        process = Mock(pid=12345)
        process.wait.side_effect = [subprocess.TimeoutExpired('worker', 1), 0, 0]
        with tempfile.TemporaryDirectory() as temp:
            with patch('static_compile.specialization.static_compile_backend.subprocess.Popen', return_value=process), patch('static_compile.specialization.static_compile_backend.os.killpg') as kill:
                self.assertEqual(_run_worker(['worker'], pathlib.Path(temp)/'worker.log', {}, 1), 124)
            self.assertEqual([call.args for call in kill.call_args_list],
                             [(12345, signal.SIGTERM), (12345, signal.SIGKILL)])

    def test_backend_module_exists(self):
        self.assertTrue((ROOT / 'static_compile_backend.py').is_file(), 'JIT/OPC dispatcher not implemented')

    def test_missing_inputs_fall_back_once_and_publish_pair(self):
        if not (ROOT / 'static_compile_backend.py').is_file():
            self.fail('JIT/OPC dispatcher not implemented')
        from static_compile.specialization.static_compile_backend import compile_static_kernel_job
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
        from static_compile.specialization.static_compile_backend import compile_static_kernel_job
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
        from static_compile.specialization import static_compile_backend as backend
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
            with patch.object(backend, '_run_worker', worker), patch.object(backend, 'publish_artifacts', collision), patch('static_compile.static_compilation._opc_compile_single', side_effect=fallback) as opc:
                self.assertEqual(backend.compile_static_kernel_job(str(inp),str(root/'results'),str(root/'meta'),'Ascend910B4','info',str(root/'logs'),False), 0)
                self.assertEqual(opc.call_count, 1)
            self.assertEqual(next((root/'results').rglob('existing.txt')).read_text(), 'keep')
            self.assertEqual(next((root/'results').rglob('test.o')).read_bytes(), b'OPC')

    def test_post_rename_failure_never_falls_back(self):
        from static_compile.specialization.static_compile_backend import compile_static_kernel_job
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
            with patch('static_compile.specialization.static_compile_backend._run_worker',worker), patch('static_compile.specialization.static_compile_backend.publish_artifacts',publish), patch('static_compile.static_compilation._opc_compile_single') as opc:
                with self.assertRaisesRegex(RuntimeError,'aborting packaging'):
                    compile_static_kernel_job(str(inp),str(root/'results'),str(root/'meta'),'Ascend910B4','info',str(root/'logs'),False)
                opc.assert_not_called()
            self.assertEqual(len(list((root/'results').rglob('test.o'))),1)

if __name__ == '__main__':
    unittest.main()
