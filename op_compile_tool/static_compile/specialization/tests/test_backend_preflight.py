"""可由采集 JSON 和 descriptor 确定的问题应在启动 JIT worker 前回退。"""

import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from static_compile import static_compile_backend as backend
from static_compile.specialization.static_op_tiling import RUN_INFO_FIELDS


class BackendPreflightTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        debug = self.root / '123_debug'
        debug.mkdir()
        self.debug = debug / 'node.json'
        self.elf = self.root / 'kernel.o'
        self.metadata = self.root / 'kernel.json'
        self.elf.write_bytes(b'ELF content is checked only in the worker')
        self.resource = {
            'supportInfo': {'inputs': [], 'outputs': []},
            'kernelList': [{'kernelName': 'kernel', 'tilingKey': 1}],
        }
        self.shape = dict(
            bin_filename='test',
            inputs=[],
            outputs=[],
            attrs=[],
            deterministic='false',
            impl_mode='',
            origin_path=str(self.debug),
        )
        self.node = dict(
            self.shape,
            op_type='Test',
            kernel_name='kernel',
            tiling_args_index=1,
            tiling_data='abcd',
            tiling_key='1',
            block_dim='1',
            workspace_bytes='0 ',
            clear_atomic=False,
            schedule_mode=0,
            local_memory_size=0,
            bin_info=str(self.elf),
        )
        self.job = self.root / 'input.json'

    def run_job(self, *, skipped, reason=None):
        self.debug.write_text(json.dumps(self.node))
        self.metadata.write_text(json.dumps(self.resource))
        self.job.write_text(json.dumps({'op_type': 'Test', 'op_list': [self.shape]}))

        def opc(opc_json, **kwargs):
            self.assertEqual(opc_json, str(self.job))
            output = Path(kwargs['compile_res_dir'])
            (output / 'test.o').write_bytes(b'OPC')
            (output / 'test.json').write_text('{}')
            return 0

        with (
            patch.object(backend, '_run_worker', return_value=1) as worker,
            patch('static_compile.static_compilation._opc_compile_single', side_effect=opc) as fallback,
        ):
            rc = backend.compile_static_kernel_job(
                str(self.job),
                str(self.root / 'results'),
                str(self.root / 'meta'),
                'Ascend910B4',
                'debug',
                str(self.root / 'logs'),
                False,
            )
        self.assertEqual(rc, 0)
        fallback.assert_called_once()
        self.assertEqual(worker.call_count, 0 if skipped else 1)
        report = json.loads((self.root / 'logs' / 'input_backend_result.json').read_text())
        self.assertIs(report['jit_attempted'], not skipped)
        self.assertEqual(report['backend'], 'opc')
        if reason:
            self.assertIn(reason, report['jit_skip_reason'])
        return report

    def test_missing_runtime_fields_skip_worker(self):
        original = copy.deepcopy(self.node)
        for field in RUN_INFO_FIELDS:
            with self.subTest(field=field):
                self.node = copy.deepcopy(original)
                self.node.pop(field)
                self.run_job(skipped=True, reason=field)

    def test_invalid_runtime_values_skip_worker(self):
        original = copy.deepcopy(self.node)
        for field, value in (
            ('tiling_data', 'abc'),
            ('block_dim', '0'),
            ('workspace_bytes', [0]),
            ('clear_atomic', 'false'),
        ):
            with self.subTest(field=field):
                self.node = dict(original, **{field: value})
                self.run_job(skipped=True, reason=field)

    def test_missing_bin_info_skips_worker(self):
        self.node.pop('bin_info')
        self.run_job(skipped=True, reason='bin_info')

    def test_context_mismatch_skips_worker(self):
        self.node['deterministic'] = 'true'
        self.run_job(skipped=True, reason='context differs')

    def test_missing_dynamic_elf_skips_worker(self):
        self.elf.unlink()
        self.run_job(skipped=True, reason='dynamic ELF')

    def test_missing_dynamic_json_skips_worker(self):
        # run_job 会写默认 metadata；用另一条不存在的资源路径验证配套文件检查。
        alternative = self.root / 'alternative.o'
        alternative.write_bytes(b'ELF')
        self.node['bin_info'] = str(alternative)
        self.run_job(skipped=True, reason='dynamic JSON')

    def test_optional_abi_conflict_skips_worker(self):
        for category in ('inputs', 'outputs'):
            with self.subTest(category=category):
                for key in ('inputs', 'outputs'):
                    self.shape[key] = self.node[key] = []
                    self.resource['supportInfo'][key] = []
                self.shape[category] = [None]
                self.node[category] = [None]
                self.resource['supportInfo'][category] = [{'paramType': 'optional'}]
                self.run_job(skipped=True, reason='optional pointer')

    def test_missing_entry_key_mapping_skips_worker(self):
        self.resource['kernelList'][0].pop('tilingKey')
        self.run_job(skipped=True, reason='entry/key mapping')

    def test_conflicting_entry_key_mapping_skips_worker(self):
        self.resource['kernelList'][0]['tilingKey'] = 2
        self.run_job(skipped=True, reason='entry/key mapping')

    def test_resource_null_slot_is_not_an_optional_conflict(self):
        self.shape['inputs'] = self.node['inputs'] = [None]
        self.resource['supportInfo']['inputs'] = [None]
        self.run_job(skipped=False)

    def test_populated_optional_is_not_rejected(self):
        self.shape['inputs'] = self.node['inputs'] = [{'shape': [1]}]
        self.resource['supportInfo']['inputs'] = [{'paramType': 'optional'}]
        self.run_job(skipped=False)

    def test_valid_inputs_reach_worker_without_elf_parsing(self):
        self.run_job(skipped=False)
