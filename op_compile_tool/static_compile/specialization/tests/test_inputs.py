import pathlib
import unittest
ROOT = pathlib.Path(__file__).resolve().parents[1]
class InputsTests(unittest.TestCase):
    def test_binding_rejects_missing_entry_key_mapping(self):
        self.assertTrue((ROOT/'argument_binding.py').exists(), 'binding missing')
        from static_compile.specialization.argument_binding import resolve_entry
        with self.assertRaises(ValueError):
            resolve_entry({'kernelList':[{'kernelName':'kernel_10'}]}, 'kernel_10', 10)
    def test_compiler_named_entry_mapping_requires_base_and_exact_member(self):
        from static_compile.specialization.argument_binding import resolve_entry
        metadata = {'kernelName':'base_hash', 'kernelList':[{'kernelName':'base_hash_10'}]}
        self.assertEqual(resolve_entry(metadata, 'base_hash_10', 10)['kernelName'], 'base_hash_10')
        with self.assertRaises(ValueError):
            resolve_entry(metadata, 'base_hash_10', 11)
        metadata['kernelList'][0]['tilingKey'] = 11
        with self.assertRaises(ValueError):
            resolve_entry(metadata, 'base_hash_10', 10)

    def test_binding_accepts_explicit_or_singleton_key_mapping(self):
        self.assertTrue((ROOT/'argument_binding.py').exists(), 'binding missing')
        from static_compile.specialization.argument_binding import resolve_entry
        self.assertEqual(resolve_entry({'kernelList':[{'kernelName':'a','tilingKey':10}]}, 'a', 10)['kernelName'],'a')
        self.assertEqual(resolve_entry({'kernelList':[{'kernelName':'b'}], 'supportInfo':{'tilingKey':['10']}}, 'b',10)['kernelName'],'b')
        with self.assertRaises(ValueError):
            resolve_entry({'kernelList':[{'kernelName':'b'}], 'supportInfo':{'tilingKey':['10']}}, 'b',11)
    def test_adapter_rejects_fixture_shape_drift(self):
        self.assertTrue((ROOT/'shape_config_adapter.py').exists(),'adapter missing')
        from static_compile.specialization.shape_config_adapter import validate_context
        with self.assertRaises(ValueError):
            validate_context({'inputs':[{'shape':[2]}]}, {'inputs':[{'shape':[3]}]}, 'Ascend910B4')

if __name__ == '__main__':
    unittest.main()
