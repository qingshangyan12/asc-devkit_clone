import copy
import json
from pathlib import Path
import tempfile
import unittest
from static_compile.specialization.shape_config_adapter import adapt_shape_config
from static_compile.specialization.argument_binding import validate_resource_context


class CollectedResourceTests(unittest.TestCase):
    def test_same_collection_debug_resource_needs_no_attachment(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            selected = root / '123_opcompile_selected'
            debug = root / '123_debug'
            selected.mkdir()
            debug.mkdir()
            node = {
                'op_type': 'CustomQuant',
                'inputs': [],
                'outputs': [],
                'attrs': [],
                'deterministic': 'false',
                'impl_mode': 'high_performance',
            }
            runtime = dict(
                kernel_name='kernel',
                tiling_args_index=4,
                tiling_data='abcd',
                tiling_key=1,
                block_dim=1,
                workspace_bytes='',
                clear_atomic=False,
                schedule_mode=0,
                local_memory_size=0,
                aicpu_block_dim=0,
                tiling_cond=0,
            )
            node.update(runtime)
            origin = selected / 'node.json'
            origin.write_text(json.dumps(node))
            record = dict(node, bin_info=str(root / 'actual.o'))
            (debug / origin.name).write_text(json.dumps(record))
            shape = dict(node, origin_path=str(origin))
            job = root / 'job.json'
            job.write_text(json.dumps({'op_type': 'CustomQuant', 'op_list': [shape]}))
            result = adapt_shape_config(job, 'Ascend910B4')
            self.assertEqual(result['dynamic_elf'], str(root / 'actual.o'))
            self.assertEqual(result['run_info'], runtime)
            # 同名但执行输入不同的节点不能关联到一起。
            record['attrs'] = [{'name': 'different', 'value': 1}]
            (debug / origin.name).write_text(json.dumps(record))
            with self.assertRaisesRegex(ValueError, 'context differs'):
                adapt_shape_config(job, 'Ascend910B4')

    def test_custom_schema_requires_resource_identity_and_exact_descriptors(self):
        fixture = json.loads((Path(__file__).parent / 'fixtures/DynamicQuant.json').read_text())
        metadata, shape = fixture['metadata'], fixture['shape']
        for owner in [metadata['supportInfo'], shape]:
            owner['inputs'] = [x for x in owner['inputs'] if x is not None]
            for x in owner['inputs']:
                x['paramType'] = 'required'
                if 'param_type' in x:
                    x['param_type'] = 'required'
        metadata['op_type'] = 'CustomQuant'
        shape['op_type'] = 'CustomQuant'
        metadata['supportInfo'].pop('simplifiedKey', None)
        validate_resource_context(metadata, shape, 'CustomQuant', 'Ascend910B4')
        metadata.pop('op_type')
        with self.assertRaisesRegex(ValueError, 'identity'):
            validate_resource_context(metadata, shape, 'CustomQuant', 'Ascend910B4')
        metadata['op_type'] = 'CustomQuant'
        bad = copy.deepcopy(shape)
        bad['inputs'][1]['name'] = 'other'
        with self.assertRaises(ValueError):
            validate_resource_context(metadata, bad, 'CustomQuant', 'Ascend910B4')
