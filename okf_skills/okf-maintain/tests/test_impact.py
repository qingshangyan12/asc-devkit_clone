"""Tests for okf-maintain impact analysis: pure functions + real-repo smoke."""

from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
sys.path.insert(0, str(SCRIPTS))
import impact  # noqa: E402

REPO = Path(__file__).resolve().parents[3]


class DiffDocumentsTest(unittest.TestCase):
    def test_added_removed_modified(self):
        committed = {"a.md": "1", "b.md": "2", "c.md": "3"}
        current = {"a.md": "1", "b.md": "9", "d.md": "4"}
        drift = impact.diff_documents(committed, current)
        self.assertEqual(drift["added"], ["d.md"])
        self.assertEqual(drift["removed"], ["c.md"])
        self.assertEqual(drift["modified"], ["b.md"])

    def test_no_drift(self):
        state = {"a.md": "1", "b.md": "2"}
        drift = impact.diff_documents(state, dict(state))
        self.assertEqual(drift, {"added": [], "removed": [], "modified": []})


class FamilyMapTest(unittest.TestCase):
    def test_groups_members_and_skips_nav(self):
        documents = {
            "x/fam/fam.md": {"family": "fam"},
            "x/fam/fam_arch_2201.md": {"family": "fam"},
            "x/nav.md": {"family": "__nav__"},
            "x/solo.md": {"family": ""},
        }
        family_map = impact.build_family_map(documents)
        self.assertEqual(family_map, {"fam": ["x/fam/fam.md", "x/fam/fam_arch_2201.md"]})


class DocImpactTest(unittest.TestCase):
    def test_known_doc(self):
        documents = {
            "x/a.md": {
                "okftype": "reference.api.cpp", "title": "asc_a", "category": "cat",
                "family": "asc_a", "deps_out": [{"path": "x/b.md", "kind": "requires"}],
            },
            "x/b.md": {"okftype": "reference.api.cpp", "title": "asc_b", "category": "cat", "family": "asc_b", "deps_out": []},
        }
        edges = [{"from": "x/b.md", "to": "x/a.md", "kind": "related"}]
        result = impact.doc_impact("x/a.md", documents, edges, {"asc_a": ["x/a.md"]})
        self.assertTrue(result["known_to_index"])
        self.assertEqual(result["family_members"], [])
        self.assertEqual(result["incoming_deps"], [{"from": "x/b.md", "kind": "related"}])
        self.assertEqual(result["outgoing_deps"], [{"path": "x/b.md", "kind": "requires"}])

    def test_unknown_doc_derives_from_path(self):
        result = impact.doc_impact("docs/zh/api/SIMD-API/c_api/sync/new_api.md", {}, [], {})
        self.assertFalse(result["known_to_index"])
        self.assertEqual(result["family"], "new_api")
        self.assertEqual(result["category"], "sync")


class IdBlockTest(unittest.TestCase):
    def test_split_and_affected_tasks(self):
        text = (
            "tasks:\n"
            "  - id: A1\n"
            "    required:\n"
            "      - docs/c_api/x/old.md\n"
            "  - id: A2\n"
            "    required:\n"
            "      - docs/c_api/y/keep.md\n"
        )
        blocks = impact.split_id_blocks(text)
        self.assertEqual([task_id for task_id, _ in blocks], ["A1", "A2"])
        self.assertEqual(impact.affected_tasks(["docs/c_api/x/old.md"], text), ["A1"])
        self.assertEqual(impact.affected_tasks(["docs/c_api/none.md"], text), [])

    def test_affected_dryrun_matches_family_and_category(self):
        impact_items = [
            {"family": "asc_copy_gm2ub", "category": "vector_datamove"},
        ]
        dryrun = (
            "queries:\n"
            "  B2: [\"--api asc_copy_gm2ub\"]\n"
            "  B1: [\"--kw 非对齐\"]\n"
            "extended:\n"
            "  - id: X3\n"
            "    args: \"--stage in --category vector_datamove\"\n"
        )
        result = impact.affected_dryrun(impact_items, dryrun)
        self.assertEqual(result["queries"], ["B2"])
        self.assertEqual(result["extended"], ["X3"])


class RealRepoSmokeTest(unittest.TestCase):
    def test_analyze_report_schema(self):
        report = impact.analyze(REPO)
        self.assertEqual(report["schema"], "okf-impact/1")
        drift = report["drift"]
        for key in ("added", "removed", "modified"):
            self.assertIsInstance(drift[key], list)
        committed = json.loads(
            (REPO / "okf-skills/okf-index/index/build-meta.json").read_text(encoding="utf-8")
        )["document_sha256"]
        known = set(committed)
        self.assertTrue(set(drift["modified"]) <= known)
        self.assertTrue(set(drift["removed"]) <= known)
        self.assertFalse(set(drift["added"]) & known)
        affected = set(drift["added"]) | set(drift["removed"]) | set(drift["modified"])
        self.assertEqual({item["path"] for item in report["impact"]}, affected)
        self.assertEqual(report["has_drift"], bool(affected) or any(report["fingerprints"].values()))
        impact.text_report(report)


if __name__ == "__main__":
    unittest.main()
