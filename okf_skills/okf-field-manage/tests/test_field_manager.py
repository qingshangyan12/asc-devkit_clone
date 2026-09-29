import importlib.util
import unittest


SCRIPT = __import__("pathlib").Path(__file__).resolve().parents[1] / "scripts" / "field_manager.py"
spec = importlib.util.spec_from_file_location("field_manager", SCRIPT)
module = importlib.util.module_from_spec(spec)
assert spec.loader is not None
spec.loader.exec_module(module)


class FieldManagerTest(unittest.TestCase):
    def test_current_registry_is_closed_over_controller_capabilities(self):
        result = module.check()
        self.assertTrue(result["ok"], result["errors"])

    def test_unknown_profile_field_is_reported(self):
        original = module.load_registry
        try:
            module.load_registry = lambda: {
                "common_fields": ["okftype"],
                "common_required_fields": ["okftype"],
                "field_order": ["okftype"],
                "field_catalog": {"okftype": {}},
                "profiles": {"reference.api": {"fields": ["missing"]}},
            }
            result = module.check()
            self.assertFalse(result["ok"])
            self.assertTrue(any("unknown fields" in item for item in result["errors"]))
        finally:
            module.load_registry = original


if __name__ == "__main__":
    unittest.main()
