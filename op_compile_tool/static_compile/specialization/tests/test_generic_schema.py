import copy
import unittest
from static_compile.specialization.argument_binding import validate_resource_context


def context(op):
    tensor = dict(name='source', index=0, paramType='required', dtype='float32', format='ND', shape=[-2])
    info = dict(opMode='dynamic', inputs=[tensor, None], outputs=[], attrs=[], deterministic='ignore')
    return dict(op_type=op, supportInfo=info), dict(inputs=[dict(tensor, shape=[2, 3]), None], outputs=[], attrs=[])


class GenericSchemaTests(unittest.TestCase):
    def test_every_operator_uses_resource_schema_including_absent_descriptors(self):
        for op in ('DynamicQuant', 'MoeGatingTopKSoftmaxV2', 'PreviouslyUnseenOp'):
            m, s = context(op)
            original = copy.deepcopy((m, s))
            validate_resource_context(m, s, op, 'Ascend910B4')
            self.assertEqual((m, s), original)
            s['inputs'][1] = dict(s['inputs'][0], index=1)
            with self.assertRaises(ValueError):
                validate_resource_context(m, s, op, 'Ascend910B4')

    def test_missing_identity_needs_actual_collection_for_every_operator(self):
        for op in ('DynamicQuant', 'MoeGatingTopKSoftmaxV2', 'PreviouslyUnseenOp'):
            m, s = context(op)
            m.pop('op_type')
            with self.assertRaisesRegex(ValueError, 'identity'):
                validate_resource_context(m, s, op, 'Ascend910B4')
            validate_resource_context(m, s, op, 'Ascend910B4', selected_from_collection=True)

    def test_float_alias_and_list_attributes(self):
        m, s = context('PreviouslyUnseenOp')
        attrs = [dict(name='scale', dtype='float', value=0.125),
                 dict(name='axes', dtype='list_int', value=[1, 2])]
        m['supportInfo']['attrs'] = copy.deepcopy(attrs)
        s['attrs'] = copy.deepcopy(attrs); s['attrs'][0]['dtype'] = 'float32'
        validate_resource_context(m, s, 'PreviouslyUnseenOp', 'Ascend910B4')
        for bad in ([True], [1.5], '1,2'):
            s['attrs'][1]['value'] = bad
            with self.assertRaisesRegex(ValueError, 'value type'):
                validate_resource_context(m, s, 'PreviouslyUnseenOp', 'Ascend910B4')

    def test_duplicate_or_incomplete_descriptors_rejected(self):
        m,s=context('PreviouslyUnseenOp')
        for field in ('name','index','paramType','dtype','format','shape'):
            bad=copy.deepcopy(m);bad['supportInfo']['inputs'][0].pop(field)
            with self.assertRaises(ValueError):
                validate_resource_context(bad,s,'PreviouslyUnseenOp','Ascend910B4')
        m['supportInfo']['attrs']=[dict(name='same',dtype='int',value=1)]*2
        s['attrs']=copy.deepcopy(m['supportInfo']['attrs'])
        with self.assertRaises(ValueError):
            validate_resource_context(m,s,'PreviouslyUnseenOp','Ascend910B4')
