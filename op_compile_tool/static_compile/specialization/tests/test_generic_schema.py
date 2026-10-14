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
            validate_resource_context(m, s, op)
            self.assertEqual((m, s), original)
            s['inputs'][1] = dict(s['inputs'][0], index=1)
            with self.assertRaises(ValueError):
                validate_resource_context(m, s, op)

    def test_missing_identity_needs_actual_collection_for_every_operator(self):
        for op in ('DynamicQuant', 'MoeGatingTopKSoftmaxV2', 'PreviouslyUnseenOp'):
            m, s = context(op)
            m.pop('op_type')
            with self.assertRaisesRegex(ValueError, 'identity'):
                validate_resource_context(m, s, op)
            validate_resource_context(m, s, op, selected_from_collection=True)

    def test_float_alias_and_list_attributes(self):
        m, s = context('PreviouslyUnseenOp')
        attrs = [dict(name='scale', dtype='float', value=0.125), dict(name='axes', dtype='list_int', value=[1, 2])]
        m['supportInfo']['attrs'] = copy.deepcopy(attrs)
        s['attrs'] = copy.deepcopy(attrs)
        s['attrs'][0]['dtype'] = 'float32'
        validate_resource_context(m, s, 'PreviouslyUnseenOp')
        for bad in ([True], [1.5], '1,2'):
            s['attrs'][1]['value'] = bad
            with self.assertRaisesRegex(ValueError, 'value type'):
                validate_resource_context(m, s, 'PreviouslyUnseenOp')

    def test_int64_attribute_alias_on_resource_and_request(self):
        for resource_dtype, request_dtype in (('int64', 'int'), ('int', 'int64'), ('int64', 'int64')):
            with self.subTest(resource_dtype=resource_dtype, request_dtype=request_dtype):
                m, s = context('PreviouslyUnseenOp')
                m['supportInfo']['attrs'] = [dict(name='dim', dtype=resource_dtype, value=-1)]
                s['attrs'] = [dict(name='dim', dtype=request_dtype, value=-1)]
                original = copy.deepcopy((m, s))
                validate_resource_context(m, s, 'PreviouslyUnseenOp')
                self.assertEqual((m, s), original)

    def test_int64_attribute_alias_preserves_value_checks(self):
        m, s = context('PreviouslyUnseenOp')
        m['supportInfo']['attrs'] = [dict(name='dim', dtype='int64', value=-1)]
        s['attrs'] = [dict(name='dim', dtype='int', value=-1)]
        for invalid in (True, -1.0, '-1'):
            with self.subTest(value=invalid):
                s['attrs'][0]['value'] = invalid
                with self.assertRaisesRegex(ValueError, 'value type'):
                    validate_resource_context(m, s, 'PreviouslyUnseenOp')
        s['attrs'][0]['value'] = 0
        with self.assertRaisesRegex(ValueError, 'attribute value mismatch'):
            validate_resource_context(m, s, 'PreviouslyUnseenOp')

    def test_duplicate_or_incomplete_descriptors_rejected(self):
        m, s = context('PreviouslyUnseenOp')
        for field in ('name', 'index', 'paramType', 'dtype', 'format', 'shape'):
            bad = copy.deepcopy(m)
            bad['supportInfo']['inputs'][0].pop(field)
            with self.assertRaises(ValueError):
                validate_resource_context(bad, s, 'PreviouslyUnseenOp')
        m['supportInfo']['attrs'] = [dict(name='same', dtype='int', value=1)] * 2
        s['attrs'] = copy.deepcopy(m['supportInfo']['attrs'])
        with self.assertRaises(ValueError):
            validate_resource_context(m, s, 'PreviouslyUnseenOp')
