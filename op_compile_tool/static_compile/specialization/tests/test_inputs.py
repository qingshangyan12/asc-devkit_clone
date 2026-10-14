import pathlib
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]


class InputsTests(unittest.TestCase):
    def test_binding_accepts_entry_without_separate_tiling_key(self):
        from static_compile.specialization.argument_binding import resolve_entry

        item = {'kernelName': 'kernel_10'}
        self.assertIs(resolve_entry({'kernelList': [item]}, 'kernel_10'), item)

    def test_binding_requires_exact_entry_match(self):
        from static_compile.specialization.argument_binding import resolve_entry

        metadata = {'kernelName': 'base_hash', 'kernelList': [{'kernelName': 'base_hash_11', 'tilingKey': 10}]}
        with self.assertRaises(ValueError):
            resolve_entry(metadata, 'base_hash_10')

    def test_binding_rejects_duplicate_entries(self):
        from static_compile.specialization.argument_binding import resolve_entry

        metadata = {'kernelList': [{'kernelName': 'kernel_10'}, {'kernelName': 'kernel_10'}]}
        with self.assertRaises(ValueError):
            resolve_entry(metadata, 'kernel_10')

    def test_adapter_rejects_fixture_shape_drift(self):
        self.assertTrue((ROOT / 'shape_config_adapter.py').exists(), 'adapter missing')
        from static_compile.specialization.shape_config_adapter import validate_context

        with self.assertRaises(ValueError):
            validate_context({'inputs': [{'shape': [2]}]}, {'inputs': [{'shape': [3]}]}, 'Ascend910B4')


if __name__ == '__main__':
    unittest.main()
