"""可由采集 JSON 和 descriptor 确定的问题应在启动 JIT worker 前回退。"""

import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from static_compile import static_compile_backend as backend
from static_compile.specialization.errors import SpecializationError
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

    def write_inputs(self):
        self.debug.write_text(json.dumps(self.node))
        self.metadata.write_text(json.dumps(self.resource))
        self.job.write_text(json.dumps({'op_type': 'Test', 'op_list': [self.shape]}))

    def run_job(self, *, skipped, reason=None):
        self.write_inputs()

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

    def test_missing_separate_tiling_key_reaches_worker(self):
        self.resource['kernelList'][0].pop('tilingKey')
        self.run_job(skipped=False)

    def test_missing_entry_skips_worker(self):
        self.resource['kernelList'][0]['kernelName'] = 'another_kernel'
        self.run_job(skipped=True, reason='entry must uniquely match')

    def test_unsupported_selected_parameter_mode_skips_worker(self):
        self.resource['kernelList'][0]['dynamicParamMode'] = 'unfolded'
        self.run_job(skipped=True, reason='dynamic parameter mode')

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

    def test_prepared_inputs_include_parsed_runtime_and_selected_resource(self):
        from static_compile.specialization.shape_config_adapter import prepare_specialization_inputs

        self.write_inputs()
        inputs = prepare_specialization_inputs(self.job, 'Ascend910B4')
        self.assertEqual(inputs['shape_config'], self.shape)
        self.assertEqual(inputs['op_type'], 'Test')
        self.assertEqual(inputs['dynamic_elf'], str(self.elf))
        self.assertEqual(inputs['dynamic_json'], str(self.metadata))
        self.assertEqual(inputs['metadata'], self.resource)
        self.assertIs(inputs['selected_kernel'], inputs['metadata']['kernelList'][0])
        self.assertEqual(inputs['tiling'].kernel_name, 'kernel')
        self.assertEqual(inputs['tiling'].tiling_key, 1)
        self.assertEqual(inputs['tiling'].block_dim, 1)
        self.assertEqual(inputs['tiling'].tiling_data, bytes.fromhex('abcd'))
        self.assertEqual(inputs['tiling'].workspaces, [0])

    def test_preparation_retains_verified_parameter_mode(self):
        from static_compile.specialization.shape_config_adapter import prepare_specialization_inputs

        self.resource['kernelList'][0]['dynamicParamMode'] = 'folded_with_desc'
        self.write_inputs()
        inputs = prepare_specialization_inputs(self.job, 'Ascend910B4')
        self.assertEqual(inputs['parameter_mode'], 'folded_with_desc')

    def test_preparation_rejects_ambiguous_or_invalid_resource_json(self):
        from static_compile.specialization.shape_config_adapter import prepare_specialization_inputs

        self.write_inputs()
        for contents, error_type, reason in (
            ('{"kernelList": [], "kernelList": []}', SpecializationError, 'duplicate JSON field kernelList'),
            ('{"kernelList": NaN}', ValueError, 'invalid JSON constant NaN'),
            ('[]', ValueError, 'dynamic JSON must be an object'),
        ):
            with self.subTest(contents=contents):
                self.metadata.write_text(contents)
                with self.assertRaisesRegex(error_type, reason):
                    prepare_specialization_inputs(self.job, 'Ascend910B4')

    def test_worker_revalidates_changed_inputs_before_elf_or_request(self):
        from static_compile.specialization.shape_config_adapter import prepare_specialization_inputs
        from static_compile.specialization import static_kernel_specialization as worker

        self.write_inputs()
        prepare_specialization_inputs(self.job, 'Ascend910B4')
        self.node['tiling_data'] = 'abc'
        self.debug.write_text(json.dumps(self.node))
        job = worker.StaticKernelSpecializationJob(str(self.job), str(self.root / 'stage'), 'Ascend910B4')
        with (
            patch.object(worker, 'read_resource_id') as read_resource,
            patch.object(worker, 'OwnedSpecializationRequest') as request,
        ):
            with self.assertRaisesRegex(SpecializationError, 'tiling_data'):
                worker.specialize_static_kernel(job)
        read_resource.assert_not_called()
        request.assert_not_called()
        self.assertFalse((self.root / 'stage').exists())
