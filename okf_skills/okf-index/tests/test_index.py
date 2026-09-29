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
TASKS_PATH = REPO / "okf_skills/okf-evaluate/testset/tasks.yaml"
DRYRUN_PATH = Path(__file__).resolve().parent / "dryrun.yaml"
BASE = "docs/zh/api/SIMD-API/c_api"

# E 类任务原登记的范围外缺口（pipeline-event-sync / register-load-store / float-floor）
# 已随 c_api 全量入范围；缺口登记清空后，E 类改为断言对应能力文档可检索。
RESOLVED_GAP_DOCS = {
    "E1": {
        f"{BASE}/sync/intra_core_sync/asc_sync_notify.md",
        f"{BASE}/sync/intra_core_sync/asc_sync_wait.md",
    },
    "E2": {f"{BASE}/reg_compute/load/asc_loadalign.md"},
    "E3": {f"{BASE}/reg_compute/reg_convert/asc_floor.md"},
}


def build_to_tmp() -> Path:
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp)
        build_index.build(REPO, out)
        payload = {p.name: p.read_text(encoding="utf-8") for p in out.iterdir()}
    return payload


class EmptyTagGuardTest(unittest.TestCase):
    def test_empty_tag_namespace_rejected_and_clean_doc_passes(self):
        front_matter = (
            "okftype: reference.api\n"
            "title: {title}\n"
            "resource: asc-devkit/docs/zh/api/SIMD-API/c_api/foo.md\n"
            "description: 测试。\n"
            "tag:\n"
            "  - api_surfaces: [c_api]{extra_tag}\n"
            "  - roles: [compute]\n"
            "signature: []\n"
            "dependencies: []\n"
            "examples: []\n"
            "timestamp: '2026-09-24'\n"
        )
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            doc = root / BASE / "foo.md"
            doc.parent.mkdir(parents=True)
            doc.write_text(
                "---\n" + front_matter.format(title="foo", extra_tag="") + "---\n\n# foo\n",
                encoding="utf-8",
            )
            errors: list[str] = []
            documents, _ = build_index.load_raw_documents(root, errors)
            self.assertEqual(errors, [])
            self.assertEqual(documents[doc.relative_to(root).as_posix()]["tags"]["api_surfaces"], ["c_api"])

            doc.write_text(
                "---\n" + front_matter.format(title="foo", extra_tag="\n  - families: []") + "---\n\n# foo\n",
                encoding="utf-8",
            )
            errors = []
            build_index.load_raw_documents(root, errors)
            self.assertTrue(
                any("empty tag namespace rendered: families" in error for error in errors), errors
            )


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
        scope = self.index["scope"]
        self.assertEqual(meta["doc_count"], 838)
        self.assertEqual(meta["nav_page_count"], 65)
        self.assertEqual(meta["arch_page_count"], 59)
        self.assertEqual(meta["primary_count"], 773)
        self.assertEqual(len(scope["all_docs"]), 838)
        nav = set(scope["navigation_pages"])
        arch = set(scope["arch_pages"])
        primary = set(scope["primary_docs"])
        all_docs = set(scope["all_docs"])
        self.assertTrue(nav.isdisjoint(primary))
        self.assertTrue(nav.isdisjoint(arch))
        self.assertEqual(nav | arch | primary, all_docs)
        self.assertEqual(meta["gap_ids"], [])
        self.assertEqual(meta["relation_gap_count"], 1)
        # 旧基线的合法导航命中（阶段1测试集）在新口径下仍为导航页/架构页/家族主页。
        tasks = yaml.safe_load(TASKS_PATH.read_text(encoding="utf-8"))
        acceptable = set(tasks["acceptable_navigation"])
        derived = nav | arch | {p for p in primary if Path(p).stem == Path(p).parent.name}
        self.assertTrue(acceptable <= derived)

    def test_stage_views(self):
        views = self.index["views"]["by_stage"]
        self.assertEqual({k: len(v) for k, v in views.items()}, {"in": 71, "out": 36, "compute": 385})
        self.assertIn(f"{BASE}/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md", views["in"])
        self.assertIn(f"{BASE}/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md", views["out"])
        # cube 搬运（gm→l0a/l1 入、l0c→gm 出）与寄存器 load/store 进入阶段视图。
        self.assertIn(f"{BASE}/cube_datamove/asc_copy_gm2l0a.md", views["in"])
        self.assertIn(f"{BASE}/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1.md", views["in"])
        self.assertIn(f"{BASE}/cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm.md", views["out"])
        self.assertIn(f"{BASE}/reg_compute/load/asc_loadalign.md", views["in"])
        self.assertIn(f"{BASE}/reg_compute/store/asc_storealign.md", views["out"])
        # 矩阵计算与同步类主文档。
        self.assertIn(f"{BASE}/cube_compute/asc_mmad.md", views["compute"])
        # 纯 L1→L0A 搬运与配置/查询类不进入任何阶段桶。
        self.assertNotIn(f"{BASE}/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md",
                         views["in"] + views["out"] + views["compute"])

    def test_operation_and_category_views(self):
        views = self.index["views"]
        self.assertIn(f"{BASE}/cube_compute/asc_mmad.md", views["by_operation"]["matrix_multiply_accumulate"])
        self.assertIn(f"{BASE}/cube_compute/asc_mmad_mx.md", views["by_operation"]["matrix_multiply_accumulate"])
        self.assertIn(f"{BASE}/sync/intra_core_sync/asc_sync_notify.md", views["by_operation"]["synchronization"])
        self.assertIn(f"{BASE}/sync/intra_core_sync/asc_sync_wait.md", views["by_operation"]["synchronization"])
        # 类型页（reference.type）参与 defs 分类检索，但不进入任何阶段桶。
        self.assertIn(f"{BASE}/defs/union/asc_store_atomic_config.md", views["by_category"]["defs"])
        self.assertIn(f"{BASE}/defs/enum/asc_saturation_mode.md", views["by_category"]["defs"])
        self.assertEqual(len(views["by_category"]["defs"]), 24)
        self.assertNotIn(f"{BASE}/defs/union/asc_store_atomic_config.md",
                         views["by_stage"]["in"] + views["by_stage"]["out"] + views["by_stage"]["compute"])
        # deprecated 散置架构页（无家族主页承接）作为独立主文档，可经 by_api 查到。
        self.assertEqual(views["by_api"]["asc_copy_gm2ub_sync"], [
            f"{BASE}/deprecated_interface/asc_copy_gm2ub_sync_arch_2201_deprecated.md",
            f"{BASE}/deprecated_interface/asc_copy_gm2ub_sync_arch_3510_deprecated.md",
        ])

    def test_convert_view(self):
        pairs = self.index["views"]["by_dtype_pair"]
        self.assertEqual(pairs["half->float"], [
            f"{BASE}/reg_compute/reg_convert/asc_half2float.md",
            f"{BASE}/vector_compute/vector_convert/asc_half2float.md",
        ])
        self.assertEqual(pairs["bfloat16->float"], [
            f"{BASE}/reg_compute/reg_convert/asc_bfloat162float.md",
            f"{BASE}/vector_compute/vector_convert/asc_bfloat162float.md",
        ])
        # fp8 / hifloat8 / fp4x2 新 token 与 packed 命名拆分。
        self.assertEqual(pairs["float->fp8_e4m3fn"], [f"{BASE}/reg_compute/reg_convert/asc_float2e4m3.md"])
        self.assertEqual(pairs["float->hifloat8"], [f"{BASE}/reg_compute/reg_convert/asc_float2hif8.md"])
        self.assertEqual(pairs["bfloat16->fp4x2_e1m2"], [f"{BASE}/reg_compute/reg_convert/asc_bfloat162e1m2x2.md"])
        self.assertEqual(pairs["bfloat16->fp4x2_e2m1"], [f"{BASE}/reg_compute/reg_convert/asc_bfloat162e2m1x2.md"])
        self.assertEqual(pairs["fp4x2_e1m2->bfloat16"], [f"{BASE}/reg_compute/reg_convert/asc_e1m2x22bfloat16.md"])

    def test_api_card_variants(self):
        results = query.api_card(self.index, "asc_copy_gm2ub")
        paths = {row["path"] for row in results}
        self.assertIn(f"{BASE}/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md", paths)
        for arch in ("arch_2201", "arch_3510"):
            self.assertIn(f"{BASE}/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_{arch}.md", paths)
        card = query.api_card(self.index, "asc_copy_l12l0a")
        self.assertEqual({row["path"] for row in card}, {
            f"{BASE}/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md",
            f"{BASE}/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md",
            f"{BASE}/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_2d_arch_3510.md",
            f"{BASE}/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_3d_arch_3510.md",
        })

    def test_dependency_queries(self):
        ndim = "docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_gm2ub.md"
        deps = {Path(row["path"]).name for row in query.deps_query(self.index, "asc_ndim_copy_gm2ub", "requires", False)}
        self.assertIn("asc_set_ndim_loop_stride.md", deps)
        self.assertIn("asc_set_ndim_pad_count.md", deps)
        reverse = query.deps_query(self.index, "asc_ndim_copy_gm2ub", "configures", True)
        self.assertIn("asc_set_ndim_loop_stride.md", {Path(r["path"]).name for r in reverse})

    def test_gap_queries(self):
        # 三条范围外缺口已全部入范围，scope_out_gaps 为空：旧关键词不再命中缺口登记。
        self.assertEqual(self.gaps["scope_out_gaps"], [])
        for keyword in ("同步", "寄存器", "取整", "floor"):
            self.assertEqual(query.gap_query(self.gaps, keyword), [], keyword)
        # 关系缺口（asc_get_cmp_mask 配合对象未定）仍可检索。
        hits = query.gap_query(self.gaps, "compare")
        self.assertEqual([h["gap_id"] for h in hits], ["relation:asc_get_cmp_mask"])

    def test_relation_gap_registered(self):
        self.assertEqual(len(self.gaps["relation_gaps"]), 1)


class DryRunTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.index = json.loads((INDEX_DIR / "index.json").read_text(encoding="utf-8"))
        cls.gaps = json.loads((INDEX_DIR / "gaps.json").read_text(encoding="utf-8"))
        cls.tasks = {t["id"]: t for t in yaml.safe_load(TASKS_PATH.read_text(encoding="utf-8"))["tasks"]}
        payload = yaml.safe_load(DRYRUN_PATH.read_text(encoding="utf-8"))
        cls.queries = payload["queries"]
        cls.extended = payload["extended"]

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
                if self.gaps["scope_out_gaps"]:
                    if not any("无文档" in row["conclusion"] for row in gap_rows):
                        failures.append(f"{task_id}: expected scope-out gap not hit")
                else:
                    expected = RESOLVED_GAP_DOCS.get(task_id, set())
                    if not expected <= expanded:
                        failures.append(
                            f"{task_id}: former scope-out capability not retrieved: {sorted(expected - expanded)}"
                        )
        self.assertEqual(failures, [])

    def test_extended_queries(self):
        for spec in self.extended:
            args = query.parser().parse_args(shlex.split(spec["args"]))
            results = query.run_query(args, self.index, self.gaps)
            if spec.get("empty"):
                self.assertEqual(results, [], spec["id"])
                continue
            paths = {row["path"] for row in results}
            for required in spec.get("include", []):
                self.assertIn(required, paths, spec["id"])


if __name__ == "__main__":
    unittest.main()
