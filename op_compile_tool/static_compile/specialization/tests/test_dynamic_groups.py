"""折叠后的 Tensor 列表无论长度是多少，都占用一个 ABI 槽位。"""

import copy
import unittest
from static_compile.specialization.argument_binding import validate_resource_context


def contexts():
    tensor = dict(name='key', index=0, dtype='float16', format='ND', paramType='dynamic', shape=[-2])
    info = dict(
        opMode='dynamic',
        simplifiedKey=['GroupOp/d=0'],
        dynamicParamMode='folded_with_desc',
        inputs=[[tensor]],
        outputs=[],
        attrs=[],
        deterministic='ignore',
    )
    shape = dict(inputs=[[dict(tensor, shape=[2, 8]), dict(tensor, shape=[3, 8])]], outputs=[], attrs=[])
    return dict(supportInfo=info), shape


class DynamicGroupTests(unittest.TestCase):
    def validate(self, metadata, shape):
        validate_resource_context(metadata, shape, 'GroupOp', 'Ascend910B4')

    def test_group_template_checks_every_member_without_mutation(self):
        metadata, shape = contexts()
        before = copy.deepcopy((metadata, shape))
        self.validate(metadata, shape)
        self.assertEqual((metadata, shape), before)
        shape['inputs'][0][1]['dtype'] = 'float32'
        with self.assertRaisesRegex(ValueError, 'dtype'):
            self.validate(metadata, shape)

    def test_malformed_groups_and_unknown_modes_fail(self):
        mutations = [
            lambda m, s: s['inputs'].__setitem__(0, []),
            lambda m, s: s['inputs'][0].__setitem__(1, None),
            lambda m, s: s['inputs'][0][1].update(index=1),
            lambda m, s: m['supportInfo']['inputs'][0][0].update(paramType='required'),
            lambda m, s: m['supportInfo'].update(dynamicParamMode='unfolded'),
            lambda m, s: m['supportInfo'].pop('dynamicParamMode'),
        ]
        for change in mutations:
            m, s = contexts()
            change(m, s)
            with self.assertRaises(ValueError):
                self.validate(m, s)

    def test_float_and_string_attributes(self):
        m, s = contexts()
        attrs = [dict(name='scale', dtype='float', value=0.125), dict(name='layout', dtype='str', value='BSH')]
        m['supportInfo']['attrs'] = copy.deepcopy(attrs)
        s['attrs'] = attrs
        self.validate(m, s)
        for invalid in (True, '0.125', float('nan'), float('inf')):
            s['attrs'][0]['value'] = invalid
            with self.assertRaises(ValueError):
                self.validate(m, s)

    def test_optional_layout_guard_remains(self):
        m, s = contexts()
        m['supportInfo']['inputs'].append(
            dict(name='optional', index=1, dtype='float16', format='ND', paramType='optional', shape=[-2])
        )
        s['inputs'].append(None)
        with self.assertRaisesRegex(ValueError, 'ABI retains'):
            self.validate(m, s)
