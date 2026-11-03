import copy
import unittest
from static_compile.specialization.shape_config_adapter import normalize, validate_context


class ConstantNormalizationTests(unittest.TestCase):
    def test_dump_constant_matches_opc_without_mutation(self):
        dump = {'inputs': [[{'const_data': [96], 'param_type': 'dynamic'}]]}
        opc = {'inputs': [[{'const_value': [96], 'paramType': 'dynamic'}]]}
        before = copy.deepcopy(dump)
        validate_context(opc, dump, 'Ascend910B4')
        self.assertEqual(normalize(dump), opc)
        self.assertEqual(dump, before)
        opc['inputs'][0][0]['const_value'] = [95]
        with self.assertRaisesRegex(ValueError, 'inputs'):
            validate_context(opc, dump, 'Ascend910B4')

    def test_conflicting_aliases_are_rejected_in_either_order(self):
        for value in ({'const_data': [96], 'const_value': [95]}, {'const_value': [95], 'const_data': [96]}):
            with self.assertRaisesRegex(ValueError, 'conflicting.*const_value'):
                normalize(value)
        self.assertEqual(normalize({'const_data': [96], 'const_value': [96]}), {'const_value': [96]})
