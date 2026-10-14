import copy
import json
from pathlib import Path
import unittest

from static_compile.specialization.argument_binding import validate_resource_context

ROOT = Path(__file__).resolve().parent / 'fixtures'


class ResourceContextTests(unittest.TestCase):
    def contexts(self, operation='DynamicQuant'):
        fixture = json.loads((ROOT / (operation + '.json')).read_text())
        fixture['metadata']['op_type'] = operation
        return fixture['metadata'], fixture['shape']

    def validate(self, metadata, shape, operation='DynamicQuant', soc='Ascend910B4'):
        validate_resource_context(metadata, shape, operation, soc)

    def test_real_resources_accept_real_shapes(self):
        for operation in ('DynamicQuant', 'MoeGatingTopKSoftmaxV2'):
            with self.subTest(operation=operation):
                self.validate(*self.contexts(operation), operation)

    def test_only_selected_variant_constrains_determinism(self):
        metadata, shape = self.contexts()
        metadata['deterministic'] = 'true'
        selected = dict(metadata['kernelList'][0], deterministic='false')
        metadata['kernelList'] = [selected, {'kernelName': 'unselected', 'deterministic': 'true'}]
        validate_resource_context(metadata, shape, 'DynamicQuant', 'Ascend910B4', selected)
        selected['deterministic'] = 'true'
        with self.assertRaisesRegex(ValueError, 'determinism'):
            validate_resource_context(metadata, shape, 'DynamicQuant', 'Ascend910B4', selected)

    def test_tensor_and_attribute_mismatches(self):
        edits = [
            lambda m, s: s['inputs'][0].update(dtype='float16'),
            lambda m, s: s['outputs'][0].update(format='NHWC'),
            lambda m, s: s['inputs'].pop(),
            lambda m, s: s['inputs'].__setitem__(0, None),
            lambda m, s: s['inputs'][1].update(name='group_index'),
            lambda m, s: s['inputs'][1].update(param_type='required'),
            lambda m, s: s['attrs'][0].update(value=1),
            lambda m, s: s['attrs'][0].update(value=True),
            lambda m, s: s['attrs'][0].update(dtype='bool'),
            lambda m, s: s['attrs'].clear(),
            lambda m, s: m['supportInfo']['inputs'][0].update(shape=[863, 1024]),
            lambda m, s: m['supportInfo']['inputs'][0].update(shape=[-3]),
            lambda m, s: s['inputs'][0].update(shape=[-1, 1024]),
            lambda m, s: m['supportInfo']['inputs'][0].update(index=1),
        ]
        for index, edit in enumerate(edits):
            with self.subTest(edit=index):
                metadata, shape = self.contexts()
                edit(metadata, shape)
                with self.assertRaises(ValueError):
                    self.validate(metadata, shape)

    def test_concrete_and_symbolic_shapes(self):
        for dimensions in ([864, 1024], [-1, 1024], [-2]):
            metadata, shape = self.contexts()
            metadata['supportInfo']['inputs'][0]['shape'] = dimensions
            self.validate(metadata, shape)

    def test_identity_determinism_and_soc(self):
        edits = [
            lambda m, s: m['supportInfo'].update(simplifiedKey=['Other/d=0']),
            lambda m, s: m.update(op_type='Other'),
            lambda m, s: s.update(op_type='Other'),
            lambda m, s: m.update(socVersion='Ascend310P'),
            lambda m, s: m['supportInfo'].update(deterministic='true'),
            lambda m, s: m['supportInfo'].pop('deterministic'),
            lambda m, s: s.update(deterministic_level=1),
            lambda m, s: s.update(impl_mode='high_precision'),
        ]
        for index, edit in enumerate(edits):
            with self.subTest(edit=index):
                metadata, shape = self.contexts()
                edit(metadata, shape)
                with self.assertRaises(ValueError):
                    self.validate(metadata, shape)
        self.validate(*self.contexts(), soc='Ascend310P')

    def test_collected_dynamic_selection_allows_placeholder_attr_value(self):
        metadata, shape = self.contexts()
        shape['attrs'][0]['value'] = 7
        with self.assertRaisesRegex(ValueError, 'attribute value mismatch'):
            self.validate(metadata, shape)
        validate_resource_context(metadata, shape, 'DynamicQuant', 'Ascend910B4', selected_from_collection=True)
        shape['attrs'][0]['value'] = True
        with self.assertRaisesRegex(ValueError, 'value type'):
            validate_resource_context(metadata, shape, 'DynamicQuant', 'Ascend910B4', selected_from_collection=True)

    def test_default_implementation_mode_is_high_performance(self):
        metadata, shape = self.contexts()
        shape['impl_mode'] = ''
        metadata['supportInfo']['implMode'] = 'high_performance'
        self.validate(metadata, shape)
        metadata['supportInfo']['implMode'] = 'high_precision'
        with self.assertRaisesRegex(ValueError, 'implementation mode'):
            self.validate(metadata, shape)

    def test_optional_placeholder_and_wildcard_attribute(self):
        metadata, shape = self.contexts()
        metadata['supportInfo']['attrs'][0]['value'] = None
        shape['attrs'][0]['value'] = 3
        self.validate(metadata, shape)
        shape['inputs'][1] = None
        with self.assertRaisesRegex(ValueError, 'ABI retains'):
            self.validate(metadata, shape)

    def test_explicit_soc_and_family_match_requested_target(self):
        for soc, family in (
            ('Ascend910B4', 'Ascend910B'),
            ('Ascend310P3', 'Ascend310P'),
            ('Ascend910_9391', 'Ascend910_93'),
        ):
            for declaration in (soc, family):
                metadata, shape = self.contexts()
                metadata['socVersion'] = declaration
                validate_resource_context(metadata, shape, 'DynamicQuant', soc, short_soc=family)
            metadata['socVersion'] = 'Ascend910B1'
            with self.assertRaisesRegex(ValueError, 'SoC'):
                validate_resource_context(metadata, shape, 'DynamicQuant', soc, short_soc=family)


if __name__ == '__main__':
    unittest.main()
