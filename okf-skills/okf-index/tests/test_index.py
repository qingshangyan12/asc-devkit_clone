"""Tests for okf-index: build determinism, view correctness, and the 20-task dry-run."""

from __future__ import annotations

import json
import shlex
import sys
import tempfile
import unittest
from pathlib import Path

import yaml

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
sys.path.insert(0, str(SCRIPTS))
import build_index  # noqa: E402
import query  # noqa: E402

REPO = build_index.SKILL_ROOT.parent.parent
INDEX_DIR = build_index.INDEX_DIR
TASKS_PATH = REPO / "okf-skills/okf-evaluate/testset/tasks.yaml"
DRYRUN_PATH = Path(__file__).resolve().parent / "dryrun.yaml"


def build_to_tmp() -> Path:
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp)
        build_index.build(REPO, out)
        payload = {p.name: p.read_text(encoding="utf-8") for p in out.iterdir()}
    return payload


class BuildTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.index = json.loads((INDEX_DIR / "index.json").read_text(encoding="utf-8"))
        cls.gaps = json.loads((INDEX_DIR / "gaps.json").read_text(encoding="utf-8"))

    def test_build_deterministic(self):
        first = build_to_tmp()
        second = build_to_tmp()
        self.assertEqual(first, second)

    def test_committed_index_fresh(self):
        committed = {}
        for name in ("index.json", "build-meta.json", "概览.md"):
            text = (INDEX_DIR / name).read_text(encoding="utf-8")
            if name.endswith(".json"):
                committed[name] = json.dumps(build_index.strip_informational(json.loads(text)))
            else:
                committed[name] = text
        fresh = build_to_tmp()
        for name, payload in committed.items():
            if name.endswith(".json"):
                self.assertEqual(payload, json.dumps(build_index.strip_informational(json.loads(fresh[name]))), name)
            else:
                self.assertEqual(payload, fresh[name], name)

    def test_scope_alignment(self):
        meta = self.index["meta"]
        self.assertEqual(meta["doc_count"], 140)
        self.assertEqual(meta["nav_page_count"], 13)
        self.assertEqual(meta["arch_page_count"], 8)
        self.assertEqual(meta["primary_count"], 140 - 13 - 8)
        tasks = yaml.safe_load(TASKS_PATH.read_text(encoding="utf-8"))
        acceptable = set(tasks["acceptable_navigation"])
        derived = (
            set(self.index["scope"]["navigation_pages"])
            | set(self.index["scope"]["arch_pages"])
            | {p for p in self.index["scope"]["primary_docs"] if Path(p).stem == Path(p).parent.name}
        )
        self.assertEqual(derived, acceptable)

    def test_stage_views(self):
        views = self.index["views"]["by_stage"]
        self.assertEqual(
            {Path(p).name for p in views["in"]},
            {"asc_copy_gm2ub.md", "asc_copy_gm2ub_align.md", "asc_ndim_copy_gm2ub.md"},
        )
        self.assertEqual(
            {Path(p).name for p in views["out"]},
            {"asc_copy_ub2gm.md", "asc_copy_ub2gm_align.md"},
        )
        self.assertEqual(len(views["compute"]), 88)

    def test_convert_view(self):
        pair = self.index["views"]["by_dtype_pair"]["half->float"]
        self.assertEqual(pair, [
            "docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md"
        ])

    def test_api_card_variants(self):
        results = query.api_card(self.index, "asc_copy_gm2ub")
        paths = {row["path"] for row in results}
        self.assertIn("docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md", paths)
        for arch in ("arch_2201", "arch_3510"):
            self.assertIn(
                f"docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_{arch}.md",
                paths,
            )

    def test_dependency_queries(self):
        ndim = "docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_gm2ub.md"
        deps = {Path(row["path"]).name for row in query.deps_query(self.index, "asc_ndim_copy_gm2ub", "requires", False)}
        self.assertIn("asc_set_ndim_loop_stride.md", deps)
        self.assertIn("asc_set_ndim_pad_count.md", deps)
        reverse = query.deps_query(self.index, "asc_ndim_copy_gm2ub", "configures", True)
        self.assertIn("asc_set_ndim_loop_stride.md", {Path(r["path"]).name for r in reverse})

    def test_gap_queries(self):
        for keyword, gap_id in (("同步", "pipeline-event-sync"), ("寄存器", "register-load-store"), ("取整", "float-floor")):
            hits = query.gap_query(self.gaps, keyword)
            self.assertTrue(any(h["gap_id"] == gap_id for h in hits), keyword)
        combo = query.gap_query(self.gaps, "同步,寄存器,pipe")
        self.assertEqual({h["gap_id"] for h in combo},
                         {"pipeline-event-sync", "register-load-store"})

    def test_relation_gap_registered(self):
        self.assertEqual(len(self.gaps["relation_gaps"]), 1)


class DryRunTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.index = json.loads((INDEX_DIR / "index.json").read_text(encoding="utf-8"))
        cls.gaps = json.loads((INDEX_DIR / "gaps.json").read_text(encoding="utf-8"))
        cls.tasks = {t["id"]: t for t in yaml.safe_load(TASKS_PATH.read_text(encoding="utf-8"))["tasks"]}
        cls.queries = yaml.safe_load(DRYRUN_PATH.read_text(encoding="utf-8"))["queries"]

    def family_expand(self, paths: set[str]) -> set[str]:
        expanded = set(paths)
        for path in paths:
            doc = self.index["documents"].get(path)
            if doc is None:
                continue
            expanded.update(doc["arch_variants"])
            if doc["family"] and doc["family"] != "__nav__":
                owner = f"{Path(path).parent.as_posix()}/{doc['family']}.md"
                if owner in self.index["documents"]:
                    expanded.add(owner)
        return expanded

    def run_queries(self, task_id: str) -> tuple[set[str], list[dict]]:
        specs = self.queries[task_id]
        self.assertLessEqual(len(specs), 2, f"{task_id} exceeds 2-query budget")
        paths: set[str] = set()
        gap_rows: list[dict] = []
        for spec in specs:
            args = query.parser().parse_args(shlex.split(spec))
            for row in query.run_query(args, self.index, self.gaps):
                if "gap_id" in row:
                    gap_rows.append(row)
                else:
                    paths.add(row["path"])
        return paths, gap_rows

    def test_dryrun_coverage(self):
        failures = []
        for task_id, task in self.tasks.items():
            paths, gap_rows = self.run_queries(task_id)
            expanded = self.family_expand(paths)
            for required in task.get("required", []):
                if required not in expanded:
                    failures.append(f"{task_id}: required not covered: {required}")
            for group in task.get("alternative_groups", []):
                if not any(member in expanded for member in group):
                    failures.append(f"{task_id}: alternative group uncovered: {[Path(m).name for m in group]}")
            if task.get("gap_expected"):
                if not any("无文档" in row["conclusion"] for row in gap_rows):
                    failures.append(f"{task_id}: expected scope-out gap not hit")
        self.assertEqual(failures, [])


if __name__ == "__main__":
    unittest.main()
