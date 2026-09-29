import importlib.util
from pathlib import Path
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "scripts" / "lifecycle_stage.py"


def load_module():
    spec = importlib.util.spec_from_file_location("okf_lifecycle_stage", SCRIPT)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(module)
    return module


class LifecycleStageTest(unittest.TestCase):
    def test_collects_prototype_presence_and_deprecation_range(self):
        module = load_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            package_90 = root / "cann-9.0" / "include"
            package_92 = root / "cann-9.2" / "include"
            package_90.mkdir(parents=True)
            package_92.mkdir(parents=True)
            header = """
            __aicore__ inline void asc_add(int32_t* dst, int32_t* src);
            #define ASC_ADD(dst, src) asc_add(dst, src)
            """
            (package_90 / "api.h").write_text(header, encoding="utf-8")
            (package_92 / "api.h").write_text(header, encoding="utf-8")
            notes = root / "CANN_9_2_0.md"
            notes.write_text(
                "## 接口废弃\n\n| `asc_add` | `asc_new_add` | 迁移。 |\n",
                encoding="utf-8",
            )
            result = module.collect(
                [("9.0.0", package_90.parent), ("9.2.0", package_92.parent)],
                [notes],
            )
            functions = [item for item in result["records"] if item["kind"] == "function"]
            self.assertEqual(len(functions), 1)
            self.assertEqual(functions[0]["version"], "9.0.0-9.2.0")
            self.assertEqual(functions[0]["package_versions"], ["9.0.0", "9.2.0"])

    def test_open_ended_lifecycle_for_active_prototype(self):
        module = load_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            include = root / "include"
            include.mkdir()
            (include / "api.h").write_text("#define ASC_ACTIVE(x) (x)\n", encoding="utf-8")
            result = module.collect([("9.1.0", root)], [])
            self.assertEqual(result["records"][0]["version"], "9.1.0-")

    def test_deprecation_row_can_target_one_overload(self):
        module = load_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            include = root / "include"
            include.mkdir()
            (include / "api.h").write_text(
                "void asc_over(int value);\nvoid asc_over(int value, int other);\n",
                encoding="utf-8",
            )
            notes = root / "CANN_9_2_0.md"
            notes.write_text(
                "## 接口废弃\n\n| `asc_over(int value)` | `asc_new` | 迁移。 |\n",
                encoding="utf-8",
            )
            result = module.collect([("9.0.0", root)], [notes])
            records = {item["identity"]: item for item in result["records"]}
            self.assertEqual(records["asc_over(int value)"]["version"], "9.0.0-9.2.0")
            self.assertEqual(records["asc_over(int value,int other)"]["version"], "9.0.0-")
