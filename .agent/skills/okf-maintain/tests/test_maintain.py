"""okf-maintain 测试：纯函数 + 合成仓漂移用例（真实仓只读复制，不写入真实文件）。"""

from __future__ import annotations

import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
REPO_ROOT = SCRIPTS.parents[3]
sys.path.insert(0, str(SCRIPTS))

import maintain  # noqa: E402

SURFACE_ROOT = "docs/zh/api/SIMD-API/c_api"
ASC_ADD_DOC = "docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md"
HEADER = "include/c_api/vector_compute/compute/vector_arith.h"
RELEASE_NOTES = "docs/zh/api/appendix/Release_Notes"
CONSTRAINT_DOC = "docs/zh/api/SIMD-API/c_api/general_description_and_constraints.md"
EXAMPLE = "examples/02_simd_c_api/00_introduction/01_add/c_api_async_add"
EXAMPLE_SOURCE = f"{EXAMPLE}/c_api_add.asc"


def parse(*argv: str):
    return maintain.parser().parse_args(list(argv))


def body_of(text: str) -> str:
    if not text.startswith("---\n"):
        return text
    end = text.find("\n---\n", 4)
    return text if end < 0 else text[end + 5:]


class FrontMatterTests(unittest.TestCase):
    def test_refresh_replaces_okftype_and_version(self) -> None:
        raw = 'okftype: reference.api\ndescription: "x"\ntag:\n  - roles: [compute]\nversion: ">=9.0.0"'
        new = maintain.refresh_front_matter(raw, okftype="reference.concept", version=">=9.1.0")
        self.assertIn("okftype: reference.concept", new)
        self.assertIn('version: ">=9.1.0"', new)
        self.assertIn("- roles: [compute]", new)
        keys = [line.split(":")[0] for line in new.splitlines() if line and not line[0].isspace() and not line.startswith("-")]
        self.assertEqual(keys, ["okftype", "description", "tag", "version"])

    def test_refresh_appends_missing_version(self) -> None:
        new = maintain.refresh_front_matter("okftype: reference.api", okftype=None, version=">=9.0.0")
        self.assertTrue(new.endswith('version: ">=9.0.0"'))


class LedgerTests(unittest.TestCase):
    def test_annotation_evidence(self) -> None:
        annotation = {
            "description": {"value": "x", "evidence": [{"excerpt": "e1"}, {"excerpt": "e2"}]},
            "tag_evidence": {"roles:compute": "e3"},
            "dependencies": [{"path": "a.md", "kind": "related", "evidence": {"excerpt": "e4"}}],
        }
        evidence = maintain.annotation_evidence(annotation)
        self.assertEqual(evidence["description"], ["e1", "e2"])
        excerpts = maintain.evidence_excerpts(evidence)
        self.assertIn(("tag:roles:compute", "e3"), excerpts)
        self.assertIn(("dependencies:a.md|related", "e4"), excerpts)

    def test_configs_status(self) -> None:
        ledger = maintain.empty_ledger()
        status = maintain.configs_status(ledger)
        self.assertEqual(status["status"], "absent")
        ledger["configs"] = maintain.current_configs()
        self.assertEqual(maintain.configs_status(ledger)["status"], "match")
        ledger["configs"]["taxonomy_sha256"] = "stale"
        self.assertEqual(maintain.configs_status(ledger)["changed"], ["taxonomy_sha256"])


class SyntheticRepo(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmp.cleanup)
        self.root = Path(self._tmp.name)
        self._orig_state = maintain.STATE_PATH
        self._orig_featured = maintain.FEATURED_DIR
        maintain.STATE_PATH = self.root / "state.json"
        self.addCleanup(self._restore)
        for rel in (ASC_ADD_DOC, HEADER, EXAMPLE_SOURCE, CONSTRAINT_DOC):
            self._copy(rel)
        for path in (REPO_ROOT / RELEASE_NOTES).glob("*.md"):
            self._copy(path.relative_to(REPO_ROOT).as_posix())

    def _restore(self) -> None:
        maintain.STATE_PATH = self._orig_state
        maintain.FEATURED_DIR = self._orig_featured

    def _copy(self, rel: str) -> None:
        target = self.root / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(REPO_ROOT / rel, target)

    def _baseline(self, documents: list[str] | None = None) -> None:
        args = parse("baseline", "--devkit-root", str(self.root), "--document", *(documents or [ASC_ADD_DOC]))
        self.assertEqual(0, maintain.command_baseline(args))

    def _check(self, checks: str = "doc", documents: list[str] | None = None):
        return maintain.perform_check(
            self.root, documents or [ASC_ADD_DOC], maintain.parse_checks(checks),
        )

    def _update(self, checks: str = "doc", write: bool = True) -> int:
        args = parse(
            "update", "--devkit-root", str(self.root),
            "--document", ASC_ADD_DOC, "--checks", checks,
        )
        args.write = write
        return maintain.command_update(args)

    def _non_info(self, findings):
        return [item for item in findings if item["severity"] != "INFO"]

    def test_version_drift_roundtrip(self) -> None:
        self._baseline()
        self.assertEqual([], self._non_info(self._check()[0]))

        path = self.root / ASC_ADD_DOC
        original = path.read_text(encoding="utf-8")
        self.assertIn('version: ">=9.0.0"', original)
        path.write_text(original.replace('version: ">=9.0.0"', 'version: ">=9.2.0"'), encoding="utf-8")

        findings = self._non_info(self._check()[0])
        self.assertEqual(["doc.version"], [item["class"] for item in findings])
        self.assertEqual(">=9.0.0", findings[0]["expected"])

        self.assertEqual(0, self._update())
        updated = path.read_text(encoding="utf-8")
        self.assertIn('version: ">=9.0.0"', updated)
        self.assertEqual(body_of(original), body_of(updated))
        self.assertEqual([], self._non_info(self._check()[0]))

    def test_okftype_drift_roundtrip(self) -> None:
        self._baseline()
        path = self.root / ASC_ADD_DOC
        original = path.read_text(encoding="utf-8")
        path.write_text(original.replace("okftype: reference.api", "okftype: reference.constant"), encoding="utf-8")

        findings = self._non_info(self._check()[0])
        self.assertEqual(["doc.okftype"], [item["class"] for item in findings])
        self.assertEqual(0, self._update())
        self.assertIn("okftype: reference.api", path.read_text(encoding="utf-8"))
        self.assertEqual(body_of(original), body_of(path.read_text(encoding="utf-8")))

    def test_body_evidence_semantics(self) -> None:
        self._baseline()
        path = self.root / ASC_ADD_DOC
        original = path.read_text(encoding="utf-8")
        body = body_of(original)
        excerpt = next(
            line.strip() for line in body.splitlines()
            if len(line.strip()) >= 12 and not line.strip().startswith(("#", "<", "-", "|", "```"))
        )
        state = json.loads((self.root / "state.json").read_text(encoding="utf-8"))
        state["documents"][ASC_ADD_DOC]["evidence"] = {"description": [excerpt]}
        (self.root / "state.json").write_text(json.dumps(state, ensure_ascii=False), encoding="utf-8")

        path.write_text(original + "\n补充说明。\n", encoding="utf-8")
        findings = self._non_info(self._check()[0])
        self.assertEqual(["doc.body_changed_evidence_ok"], [item["class"] for item in findings])

        path.write_text(original.replace(excerpt, "……", 1), encoding="utf-8")
        findings = self._non_info(self._check()[0])
        self.assertEqual(["doc.body_changed_evidence_lost"], [item["class"] for item in findings])
        self.assertEqual("SEM-HIGH", findings[0]["severity"])

    def test_include_drift_roundtrip(self) -> None:
        self._baseline()
        self.assertEqual([], self._non_info(self._check("doc,include")[0]))

        path = self.root / HEADER
        original = path.read_text(encoding="utf-8")
        marker = " * @archs "
        self.assertIn(marker, original)
        path.write_text(original.replace(marker, " * @archs 9999,", 1), encoding="utf-8")

        findings = self._non_info(self._check("doc,include")[0])
        self.assertIn("include.stale", [item["class"] for item in findings])
        self.assertEqual(0, self._update("doc,include"))
        self.assertEqual(original, path.read_text(encoding="utf-8"))
        self.assertEqual([], self._non_info(self._check("doc,include")[0]))

    def test_examples_drift_roundtrip(self) -> None:
        featured = self.root / "featured"
        featured.mkdir()
        (featured / "c_api_async_add.json").write_text(json.dumps({
            "examples": [{"example": EXAMPLE, "apis": ["asc_add"], "evidence": ["test fixture"]}],
        }, ensure_ascii=False), encoding="utf-8")
        maintain.FEATURED_DIR = featured

        path = self.root / EXAMPLE_SOURCE
        original = path.read_text(encoding="utf-8")
        marker = "    //! [asc_add__PUf32_PUf32_PUf32_u32]\n"
        self.assertEqual(2, original.count(marker))
        path.write_text(original.replace(marker, "", 2), encoding="utf-8")

        findings = self._non_info(self._check("examples")[0])
        self.assertIn("examples.stale", [item["class"] for item in findings])
        self.assertEqual(0, self._update("examples"))
        self.assertEqual(original, path.read_text(encoding="utf-8"))
        self.assertEqual([], self._non_info(self._check("examples")[0]))

    def test_baseline_from_run_records_evidence(self) -> None:
        run = self.root / "run"
        (run / "annotations").mkdir(parents=True)
        (run / "manifest.json").write_text(json.dumps({"purpose": "okf_yaml_run"}), encoding="utf-8")
        (run / "annotations" / "batch.json").write_text(json.dumps({
            "schema_version": 1,
            "purpose": maintain.controller.SEMANTIC_PURPOSE,
            "annotations": [{
                "document_path": ASC_ADD_DOC,
                "description": {"value": "x", "evidence": [{"excerpt": "证据一"}]},
                "tag_evidence": {"roles:compute": "证据二"},
            }],
        }, ensure_ascii=False), encoding="utf-8")
        args = parse("baseline", "--devkit-root", str(self.root), "--from-run", str(run))
        self.assertEqual(0, maintain.command_baseline(args))
        evidence = json.loads((self.root / "state.json").read_text(encoding="utf-8"))["documents"][ASC_ADD_DOC]["evidence"]
        self.assertEqual(["证据一"], evidence["description"])
        self.assertEqual({"roles:compute": "证据二"}, evidence["tag"])


if __name__ == "__main__":
    unittest.main()
