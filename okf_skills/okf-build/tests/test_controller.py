"""controller slim 管线测试：纯函数 + 真实文档 dry-run（不写入真实文件）。"""

from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
REPO_ROOT = SCRIPTS.parents[2]
sys.path.insert(0, str(SCRIPTS))

import controller  # noqa: E402
from derive_version import build_version_map, version_for  # noqa: E402


class RenderTests(unittest.TestCase):
    TASK = {
        "okftype": "reference.api",
        "fields": ["okftype", "description", "tag", "dependencies", "version"],
        "required": ["okftype", "description", "tag", "dependencies", "version"],
        "version": "9.0.0-",
    }

    def test_full_header(self) -> None:
        annotation = {
            "description": {"value": "对两个源矢量逐元素求商。"},
            "tag": {"roles": ["compute"], "operations": ["divide"], "traits": ["elementwise"]},
            "dependencies": [{"path": "docs/zh/api/SIMD-API/c_api/a.md", "kind": "constrained_by"}],
        }
        text = controller.render_front_matter(self.TASK, annotation)
        self.assertTrue(text.startswith("---\n"))
        self.assertIn("okftype: reference.api\n", text)
        self.assertIn("- roles: [compute]", text)
        self.assertIn("- operations: [divide]", text)
        self.assertIn("kind: constrained_by", text)
        self.assertIn("version: 9.0.0-", text)
        header = text[4:-5]
        keys = [line.split(":")[0] for line in header.splitlines() if line and not line[0].isspace() and not line.startswith("-")]
        self.assertEqual(keys, ["okftype", "description", "tag", "dependencies", "version"])

    def test_empty_deps_and_no_tag(self) -> None:
        annotation = {"description": {"value": "概述页面描述。"}, "dependencies": []}
        text = controller.render_front_matter(
            {"okftype": "reference.concept", "fields": ["okftype", "description"], "required": ["okftype", "description"]},
            annotation,
        )
        self.assertNotIn("\ntag:", text)
        self.assertNotIn("dependencies:", text)
        self.assertNotIn("version:", text)

    def test_apply_front_matter_replaces_old(self) -> None:
        old = "---\ntitle: x\nresource: y\ntimestamp: '1'\n---\n# Body\n"
        new = controller.apply_front_matter(old, "---\nokftype: reference.api\n---\n")
        self.assertEqual(new, "---\nokftype: reference.api\n---\n# Body\n")

    def test_apply_front_matter_prepends(self) -> None:
        new = controller.apply_front_matter("# Body\n", "---\nokftype: reference.api\n---\n")
        self.assertEqual(new, "---\nokftype: reference.api\n---\n# Body\n")


class ValidationTests(unittest.TestCase):
    BODY = "本接口执行矢量除法运算。支持掩码模式。调用前需先完成配置。"

    def test_description_rules(self) -> None:
        good = {"description": {"value": "对两个源矢量逐元素求商，结果写入目的矢量。", "evidence": [{"excerpt": "本接口执行矢量除法运算。"}]}}
        self.assertEqual(controller.validate_description(good, "a.md", self.BODY, "t", "a"), [])
        short = {"description": {"value": "太短", "evidence": [{"excerpt": "本接口执行矢量除法运算。"}]}}
        self.assertTrue(controller.validate_description(short, "a.md", self.BODY, "t", "a"))
        bad_evidence = {"description": {"value": "对两个源矢量逐元素求商，结果写入目的矢量。", "evidence": [{"excerpt": "正文不存在的话"}]}}
        self.assertTrue(controller.validate_description(bad_evidence, "a.md", self.BODY, "t", "a"))

    def test_tag_rules(self) -> None:
        required = ["okftype", "description", "tag", "version"]
        good = {"tag": {"roles": ["compute"], "operations": ["divide"]},
                "tag_evidence": {"roles:compute": "本接口执行矢量除法运算。", "operations:divide": "本接口执行矢量除法运算。"}}
        self.assertEqual(controller.validate_tag(good, "a.md", self.BODY, required), [])
        missing_ops = {"tag": {"roles": ["compute"]}, "tag_evidence": {"roles:compute": "本接口执行矢量除法运算。"}}
        self.assertTrue(controller.validate_tag(missing_ops, "a.md", self.BODY, required))
        outside = {"tag": {"roles": ["compute"], "operations": ["nope"]},
                   "tag_evidence": {"roles:compute": "本接口执行矢量除法运算。", "operations:nope": "本接口执行矢量除法运算。"}}
        self.assertTrue(controller.validate_tag(outside, "a.md", self.BODY, required))
        derivable = {"tag": {"roles": ["compute"], "operations": ["divide"], "execution_models": ["register_vector"]}, "tag_evidence": {}}
        issues = controller.validate_tag(derivable, "a.md", self.BODY, required)
        self.assertTrue(any("unknown namespaces" in i["message"] for i in issues))

    def test_dependency_rules(self) -> None:
        root = REPO_ROOT
        real_target = "docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy.md"
        good = {"dependencies": [{"path": real_target, "kind": "related",
                                  "evidence": {"source_path": "a.md", "excerpt": "调用前需先完成配置。"}}]}
        self.assertEqual(controller.validate_dependencies(good, "a.md", root, self.BODY), [])
        self_loop = {"dependencies": [{"path": "a.md", "kind": "related", "evidence": {"source_path": "a.md", "excerpt": "x"}}]}
        self.assertTrue(controller.validate_dependencies(self_loop, "a.md", root, self.BODY))
        bad_kind = {"dependencies": [{"path": real_target, "kind": "uses", "evidence": {"source_path": "a.md", "excerpt": "调用前需先完成配置。"}}]}
        self.assertTrue(controller.validate_dependencies(bad_kind, "a.md", root, self.BODY))

    def test_requires_cycle_detection(self) -> None:
        annotations = {
            "a.md": {"dependencies": [{"path": "b.md", "kind": "requires"}]},
            "b.md": {"dependencies": [{"path": "a.md", "kind": "requires"}]},
        }
        self.assertTrue(controller.check_requires_cycles(annotations))


class VersionTests(unittest.TestCase):
    def test_version_map_and_overrides(self) -> None:
        overrides = SCRIPTS.parent / "config" / "version-overrides.yaml"
        vm = build_version_map(REPO_ROOT, overrides if overrides.is_file() else None)
        self.assertEqual(vm["floors"]["asc_fma"], "9.1.0")  # 人工裁决覆盖
        self.assertEqual(vm["floors"]["asc_transto5hd"], "9.2.0")
        self.assertEqual(vm["floors"]["asc_copy_ub2ub"], "9.1.0")
        result = version_for("docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2ub.md", vm)
        self.assertEqual(result["version"], "9.1.0-")

    def test_arch_page_maps_to_family(self) -> None:
        vm = {"floors": {"asc_copy_gm2ub": "9.1.0"}, "deprecated_in": {}}
        result = version_for("docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md", vm)
        self.assertEqual(result["api"], "asc_copy_gm2ub")
        self.assertEqual(result["version"], "9.1.0-")


class PipelineTests(unittest.TestCase):
    """真实文档 prepare→validate→write dry-run；不写真实文件。"""

    RUN = REPO_ROOT / "okf_skills/okf-build/tmp/test-run"

    @classmethod
    def setUpClass(cls) -> None:
        import shutil
        if cls.RUN.exists():
            shutil.rmtree(cls.RUN)
        args = type("Args", (), {
            "devkit_root": REPO_ROOT,
            "source_dir": None,
            "document": ["docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2ub.md"],
            "run_dir": cls.RUN,
        })()
        controller.command_prepare(args)

    def test_prepare_manifest(self) -> None:
        manifest = json.loads((self.RUN / "manifest.json").read_text(encoding="utf-8"))
        self.assertEqual(len(manifest["documents"]), 1)
        task = manifest["documents"][0]
        self.assertEqual(task["okftype"], "reference.api")
        self.assertEqual(task["version"], "9.1.0-")
        self.assertTrue(task["needs_annotation"])

    def test_validate_and_write_dryrun(self) -> None:
        task = json.loads((self.RUN / "manifest.json").read_text(encoding="utf-8"))["documents"][0]
        rel = task["path"]
        body = controller.document_body(REPO_ROOT, rel)
        annotation = {
            "schema_version": 1,
            "purpose": "okf_semantic_annotations",
            "annotations": [{
                "document_path": rel,
                "document_sha256": task["sha256"],
                "description": {"value": "将数据从Unified Buffer搬运到Unified Buffer，支持连续数据块搬运。",
                                "evidence": [{"source_path": rel, "excerpt": body.strip().splitlines()[0][:40]}]},
                "tag": {"roles": ["data_movement"], "operations": ["copy"]},
                "tag_evidence": {},
                "dependencies": [],
            }],
        }
        ann_dir = self.RUN / "annotations"
        ann_dir.mkdir(exist_ok=True)
        (ann_dir / "a.json").write_text(json.dumps(annotation, ensure_ascii=False, indent=2), encoding="utf-8")
        result = controller.validate_run(REPO_ROOT, self.RUN)
        self.assertEqual(result["status"], "blocked")  # tag_evidence 缺失，预期阻断
        args = type("Args", (), {"run_dir": self.RUN, "write": False, "allow_partial": False})()
        with self.assertRaises(controller.ValidationError):
            controller.command_write(args)


if __name__ == "__main__":
    unittest.main()
