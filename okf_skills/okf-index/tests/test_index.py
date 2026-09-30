"""okf-index 推导引擎、门控与查询测试（不写入已提交索引，用临时目录构建）。"""

from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
REPO_ROOT = SCRIPTS.parents[2]
sys.path.insert(0, str(SCRIPTS))

import build_index  # noqa: E402
from derive import gate  # noqa: E402
from derive.docparse import doc_product_support, doc_title  # noqa: E402
from derive.examples_db import build_examples_db  # noqa: E402
from derive.include_db import build_include_db, declarations_for  # noqa: E402


class GateTests(unittest.TestCase):
    def test_version_covers(self) -> None:
        self.assertTrue(gate.version_covers("9.0.0-", "9.2.0"))
        self.assertTrue(gate.version_covers("9.1.0-", "9.2.0"))
        self.assertTrue(gate.version_covers("9.0.0-9.2.0", "9.2.0"))
        self.assertFalse(gate.version_covers("9.1.0-", "9.0.0"))
        self.assertFalse(gate.version_covers("9.0.0-9.1.0", "9.2.0"))

    def test_arch_label_mapping(self) -> None:
        pm = gate.load_product_map(REPO_ROOT / "okf_skills/config/product-map.yaml")
        self.assertEqual(gate.arch_label_to_number("A5", pm), "3510")
        self.assertEqual(gate.arch_label_to_number("A3", pm), "2201")
        self.assertEqual(gate.npu_to_arch("950", pm), "3510")
        self.assertEqual(gate.npu_to_arch("910b", pm), "2201")
        with self.assertRaises(ValueError):
            gate.arch_label_to_number("A9", pm)

    def test_cann_detection_chain(self) -> None:
        self.assertEqual(gate.detect_cann_version("8.0.0", REPO_ROOT), "8.0.0")
        detected = gate.detect_cann_version(None, REPO_ROOT)
        self.assertRegex(detected, r"^\d+\.\d+\.\d+$")


class DeriveTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.include_db = build_include_db(REPO_ROOT / "include" / "c_api")

    def test_declaration_derivation(self) -> None:
        decls, mode = declarations_for("asc_copy_gm2ub", self.include_db, "vector_datamove")
        self.assertEqual(mode, "exact")
        annotated = [d for d in decls if d.override_id]
        self.assertTrue(any("2201" in d.archs for d in annotated))
        self.assertTrue(any("3510" in d.archs for d in annotated))
        strided = next(d for d in annotated if "u16" in d.override_id and "3510" in d.archs)
        self.assertEqual(strided.locations(), ["global_memory", "unified_buffer"])

    def test_enum_catalog(self) -> None:
        enum = self.include_db["enums"]["asc_cast_round_mode"]
        self.assertIn("RN", enum["values"])
        self.assertTrue(
            any(c["name"] == "ASC_CAST_RN" for c in self.include_db["constants"]),
            "ASC_CAST_RN 常量应入 defs 目录",
        )

    def test_mask_derivation(self) -> None:
        decls, _ = declarations_for("asc_add", self.include_db, "vector_compute")
        self.assertTrue(all(d.masked() for d in decls if d.params and d.params[0].is_vector))

    def test_examples_db(self) -> None:
        db = build_examples_db(REPO_ROOT / "examples/02_simd_c_api/03_c_api")
        self.assertGreater(len(db), 50)
        marker_id = next(iter(db))
        self.assertIn("file", db[marker_id][0])

    def test_product_support_parse(self) -> None:
        body = (REPO_ROOT / "docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md").read_text(encoding="utf-8")
        support = doc_product_support(body.split("---\n", 2)[-1])
        self.assertEqual(support["950"]["support"], "支持")
        self.assertEqual(support["A3"]["support"], "不支持")


class BuildTests(unittest.TestCase):
    """真实构建（临时目录）：门控、推导与确定性。"""

    @classmethod
    def setUpClass(cls) -> None:
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name)
        scope = build_index.SCOPES["c_api"]
        build_index.build(REPO_ROOT, scope, cls.out / "a5", "9.2.0", "A5")
        build_index.build(REPO_ROOT, scope, cls.out / "a3", "9.2.0", "A3")
        cls.a5 = json.loads((cls.out / "a5" / "index.json").read_text(encoding="utf-8"))
        cls.a3 = json.loads((cls.out / "a3" / "index.json").read_text(encoding="utf-8"))

    @classmethod
    def tearDownClass(cls) -> None:
        cls.tmp.cleanup()

    def test_gate_arch(self) -> None:
        self.assertEqual(self.a5["meta"]["arch"], "3510")
        self.assertEqual(self.a3["meta"]["arch"], "2201")
        for path in self.a5["documents"]:
            if "_arch_2201" in path:
                self.fail(f"2201 变体页进入 A5 索引: {path}")
        for path in self.a3["documents"]:
            if "_arch_3510" in path:
                self.fail(f"3510 变体页进入 A3 索引: {path}")
        self.assertGreater(len(self.a3["documents"]), len(self.a5["documents"]))

    def test_gate_version(self) -> None:
        # 试点全部 version 区间覆盖 9.2.0，version 过滤应为 0
        self.assertEqual(self.a5["meta"]["filtered_by_version"], 0)

    def test_arch_page_signatures(self) -> None:
        doc = self.a5["documents"]["docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md"]
        for signature in doc["signatures"]:
            self.assertIn("3510", signature["archs"])

    def test_views(self) -> None:
        for view in ("by_stage", "by_role", "by_operation", "by_dtype", "by_param", "by_enum", "by_feature"):
            self.assertIn(view, self.a5["views"])
        self.assertGreater(len(self.a5["views"]["by_param"]), 10)
        self.assertIn("asc_load_l2_cache_mode", self.a5["views"]["by_enum"])
        self.assertGreater(len(self.a3["views"]["by_dtype_pair"]), 0)
        self.assertIn("low_precision", self.a3["views"]["by_feature"])

    def test_enum_reverse_lookup(self) -> None:
        users = self.a5["views"]["by_enum"]["asc_load_l2_cache_mode"]
        self.assertTrue(users)
        enum = self.a5["defs"]["enums"]["asc_load_l2_cache_mode"]
        self.assertIn("NORMAL_PERSISTENT", enum["values"])

    def test_deterministic_rebuild(self) -> None:
        scope = build_index.SCOPES["c_api"]
        build_index.build(REPO_ROOT, scope, self.out / "a5-again", "9.2.0", "A5")
        first = build_index.strip_informational(self.a5)
        second = build_index.strip_informational(
            json.loads((self.out / "a5-again" / "index.json").read_text(encoding="utf-8"))
        )
        self.assertEqual(first, second)


class DryrunTests(unittest.TestCase):
    """dryrun.yaml 查询清单回归。"""

    @classmethod
    def setUpClass(cls) -> None:
        import yaml
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name)
        scope = build_index.SCOPES["c_api"]
        build_index.build(REPO_ROOT, scope, cls.out / "a5", "9.2.0", "A5")
        build_index.build(REPO_ROOT, scope, cls.out / "a3", "9.2.0", "A3")
        (cls.out / "a5" / "gaps.json").write_text(
            (REPO_ROOT / "okf_skills/okf-index/index/c_api/gaps.json").read_text(encoding="utf-8"), encoding="utf-8"
        )
        (cls.out / "a3" / "gaps.json").write_text(
            (REPO_ROOT / "okf_skills/okf-index/index/c_api/gaps.json").read_text(encoding="utf-8"), encoding="utf-8"
        )
        cls.cases = yaml.safe_load(
            (Path(__file__).parent / "dryrun.yaml").read_text(encoding="utf-8")
        )

    @classmethod
    def tearDownClass(cls) -> None:
        cls.tmp.cleanup()

    def _run(self, arch: str, args: dict) -> list[dict]:
        sys.path.insert(0, str(SCRIPTS))
        import query as query_mod
        index, gaps = query_mod.load(self.out / arch)
        defaults = {
            "stage": None, "operation": None, "category": None, "api_class": None,
            "dtype": None, "kw": None, "convert": None, "feature": None, "version": None,
            "product": None, "api": None, "deps_of": None, "kind": None, "reverse": False,
            "param": None, "enum": None, "gap": None,
        }
        namespace = type("Args", (), {**defaults, **args, "json": True, "surface": "c_api", "index_dir": None})()
        return query_mod.run_query(namespace, index, gaps)

    def test_dryrun_cases(self) -> None:
        for case in self.cases["cases"]:
            with self.subTest(case=case["name"]):
                arch = case.get("arch", "a5")
                results = self._run(arch, case["args"])
                self.assertGreaterEqual(
                    len(results), case.get("min", 1),
                    f"{case['name']}: 期望 ≥{case.get('min', 1)}，实际 {len(results)}",
                )
                if "contains" in case:
                    joined = json.dumps(results, ensure_ascii=False)
                    for token in case["contains"]:
                        self.assertIn(token, joined, f"{case['name']}: 缺少 {token}")


if __name__ == "__main__":
    unittest.main()
