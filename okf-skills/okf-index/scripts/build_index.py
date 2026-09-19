#!/usr/bin/env python3
"""Build the persistent OKF retrieval index from stage-3 YAML front matter.

The index is a pure derivation of the 140 scoped Markdown front-matter blocks:
parse -> validate -> family rollup -> inverted views -> dependency graph.
Build is deterministic (no wall-clock values); drift is detected via --check.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "okf-yaml-generate" / "scripts"))
from controller import (  # noqa: E402
    ALLOWED_FIELDS,
    DEPENDENCY_KINDS,
    ValidationError,
    split_front_matter,
    taxonomy,
)

SKILL_ROOT = Path(__file__).resolve().parents[1]
TAXONOMY_PATH = SKILL_ROOT.parent / "okf-yaml-generate" / "taxonomy" / "core-v8-frozen.json"
INDEX_DIR = SKILL_ROOT / "index"
SCOPE_DIRS = (
    "docs/zh/api/SIMD-API/c_api/vector_datamove",
    "docs/zh/api/SIMD-API/c_api/vector_compute",
)
OKFTYPES = {"reference.api.cpp", "reference.arch.cpp", "reference.index.cpp"}
STAGES = ("in", "out", "compute")
ARCH_RE = re.compile(r"(?P<family>.+)_arch_(?P<arch>\d+)$")
CONVERT_RE = re.compile(r"asc_(?P<src>[a-z0-9]+)2(?P<dst>[a-z0-9]+)$")
DTYPE_ALIASES = {
    "f16": "half", "fp16": "half", "f32": "float", "fp32": "float", "bf16": "bfloat16",
    "int": "int32", "uint": "uint32", "int4b": "int4",
}
PAIR_TOKENS = {
    "half", "float", "bfloat16", "int4", "int8", "uint8",
    "int16", "uint16", "int32", "uint32", "int64",
}


class BuildError(ValueError):
    pass


def normalize_dtype(value: str) -> str:
    token = value[:-2] if value.endswith("_t") else value
    if token == "int4b":
        token = "int4"
    return DTYPE_ALIASES.get(token, token)


def load_raw_documents(root: Path, errors: list[str]) -> dict[str, dict[str, Any]]:
    namespaces, tax_values = taxonomy(TAXONOMY_PATH)
    documents: dict[str, dict[str, Any]] = {}
    for scope in SCOPE_DIRS:
        for path in sorted((root / scope).rglob("*.md")):
            rel = path.relative_to(root).as_posix()
            fm, _, _ = split_front_matter(path.read_text(encoding="utf-8"))
            if set(fm) - set(ALLOWED_FIELDS):
                errors.append(f"{rel}: unknown fields: {sorted(set(fm) - set(ALLOWED_FIELDS))}")
                continue
            if fm["okftype"] not in OKFTYPES:
                errors.append(f"{rel}: invalid okftype {fm['okftype']!r}")
                continue
            if not str(fm["resource"]).endswith(rel):
                errors.append(f"{rel}: resource does not match path")
            tags: dict[str, list[str]] = {}
            for item in fm["tag"]:
                for ns, values in item.items():
                    if ns not in namespaces:
                        errors.append(f"{rel}: unknown tag namespace {ns}")
                        continue
                    unknown = sorted(set(values) - tax_values.get(ns, set()))
                    if unknown:
                        errors.append(f"{rel}: tag {ns} values outside taxonomy: {unknown}")
                    tags[ns] = sorted(set(values))
            missing_ns = sorted(set(namespaces) - set(tags))
            if missing_ns:
                errors.append(f"{rel}: missing tag namespaces: {missing_ns}")
            signatures = []
            for sig in fm.get("signature") or []:
                if not isinstance(sig, dict) or not sig.get("prototype_id"):
                    errors.append(f"{rel}: signature entry without prototype_id")
                    continue
                for header in sig.get("headers", []):
                    if not (root / "include" / header).is_file():
                        errors.append(f"{rel}: header not found: include/{header}")
                signatures.append({
                    "prototype_id": sig["prototype_id"],
                    "headers": sorted(sig.get("headers", [])),
                    "archs": sorted(sig.get("archs", [])),
                    "dtypes": sorted({normalize_dtype(v) for v in sig.get("dtype", {}).get("values", [])}),
                })
            deps = []
            for dep in fm.get("dependencies") or []:
                if dep.get("kind") not in DEPENDENCY_KINDS:
                    errors.append(f"{rel}: invalid dependency kind {dep.get('kind')!r}")
                    continue
                deps.append({"path": dep["path"], "kind": dep["kind"], "condition": dep.get("condition", "")})
            examples = []
            for ex in fm.get("examples") or []:
                base = str(ex["path"]).lstrip("/")
                if not (root / base / str(ex["file"])).is_file():
                    errors.append(f"{rel}: example not found: {base}/{ex['file']}")
                    continue
                examples.append({
                    "path": f"/{base}",
                    "file": str(ex["file"]),
                    "archs": sorted(ex.get("archs", [])),
                })
            documents[rel] = {
                "okftype": fm["okftype"], "title": fm["title"], "description": fm["description"],
                "tags": tags, "signatures": signatures, "deps_out": deps, "examples": examples,
                "family": "", "arch_of": "", "category": "", "nav_page": "", "arch_variants": [],
            }
    return documents


def classify(documents: dict[str, dict[str, Any]], errors: list[str]) -> None:
    for rel, doc in documents.items():
        parts = Path(rel).parts
        stem = parts[-1].removesuffix(".md")
        if parts[5] == "vector_compute":
            if len(parts) == 7:
                doc["category"] = "vector_compute"
                doc["nav_page"] = "docs/zh/api/SIMD-API/c_api/vector_compute/vector_compute.md"
                doc["family"] = "__nav__"
                continue
            doc["category"] = parts[6]
            doc["nav_page"] = f"docs/zh/api/SIMD-API/c_api/vector_compute/{parts[6]}/{parts[6]}.md"
            if stem == parts[6]:
                doc["family"] = "__nav__"
            else:
                doc["family"] = stem
        else:
            doc["category"] = "vector_datamove"
            doc["nav_page"] = "docs/zh/api/SIMD-API/c_api/vector_datamove/vector_datamove.md"
            if len(parts) == 8:
                if stem == parts[6]:
                    doc["family"] = parts[6]
                elif (match := ARCH_RE.fullmatch(stem)) and match.group("family") == parts[6]:
                    doc["family"] = parts[6]
                    doc["arch_of"] = parts[6]
                else:
                    errors.append(f"{rel}: unexpected file in family directory {parts[6]}")
            elif stem == "vector_datamove":
                doc["family"] = "__nav__"
            else:
                doc["family"] = stem
        if doc["okftype"] in {"reference.api.cpp", "reference.arch.cpp"} and not doc["signatures"]:
            errors.append(f"{rel}: api/arch page without signature")


def rollup_families(documents: dict[str, dict[str, Any]]) -> None:
    for rel, doc in documents.items():
        if doc["okftype"] != "reference.index.cpp" or doc["family"] == "__nav__":
            continue
        variants = sorted(
            (path, other) for path, other in documents.items() if other["arch_of"] == doc["family"]
        )
        doc["arch_variants"] = [path for path, _ in variants]
        if not doc["description"] and variants:
            doc["description"] = variants[0][1]["description"]
        for _, variant in variants:
            for ns, values in variant["tags"].items():
                doc["tags"][ns] = sorted(set(doc["tags"][ns]) | set(values))
            doc["signatures"].extend(variant["signatures"])
            doc["deps_out"].extend(variant["deps_out"])
            doc["examples"].extend(variant["examples"])
        doc["signatures"] = list({
            sig["prototype_id"]: sig for sig in doc["signatures"]
        }.values())
        doc["deps_out"] = list({
            (d["path"], d["kind"]): d for d in doc["deps_out"]
        }.values())
        doc["examples"] = list({
            (e["path"], e["file"]): e for e in doc["examples"]
        }.values())


def stage_of(doc: dict[str, Any]) -> list[str]:
    tags = doc["tags"]
    roles, src, dst = set(tags["roles"]), set(tags["source_locations"]), set(tags["destination_locations"])
    stages = []
    if "data_movement" in roles and "global_memory" in src and "unified_buffer" in dst:
        stages.append("in")
    if "data_movement" in roles and "unified_buffer" in src and "global_memory" in dst:
        stages.append("out")
    if "compute" in roles:
        stages.append("compute")
    return stages


def conversion_pair(rel: str, doc: dict[str, Any]) -> str | None:
    if "conversion" not in doc["tags"]["operations"]:
        return None
    match = CONVERT_RE.fullmatch(Path(rel).stem)
    if not match:
        return None
    src, dst = normalize_dtype(match.group("src")), normalize_dtype(match.group("dst"))
    return f"{src}->{dst}" if src in PAIR_TOKENS and dst in PAIR_TOKENS else None


def check_graph(root: Path, documents: dict[str, dict[str, Any]], errors: list[str]) -> list[dict[str, Any]]:
    edges: dict[tuple[str, str], dict[str, Any]] = {}
    for rel, doc in documents.items():
        for dep in doc["deps_out"]:
            out_of_scope = dep["path"] not in documents
            if out_of_scope and not (root / dep["path"]).is_file():
                errors.append(f"{rel}: dependency target not found: {dep['path']}")
                continue
            edges[(rel, dep["path"])] = {
                "from": rel, "to": dep["path"], "kind": dep["kind"],
                "condition": dep["condition"], "out_of_scope": out_of_scope,
            }
    adjacency: dict[str, list[str]] = {}
    for (src, dst), edge in edges.items():
        if edge["kind"] == "requires" and not edge["out_of_scope"]:
            adjacency.setdefault(src, []).append(dst)
    state: dict[str, str] = {}

    def visit(node: str, stack: list[str]) -> None:
        state[node] = "active"
        for nxt in sorted(adjacency.get(node, [])):
            if state.get(nxt) == "active":
                errors.append(f"requires cycle: {' -> '.join(stack + [node, nxt])}")
            elif state.get(nxt) != "done":
                visit(nxt, stack + [node])
        state[node] = "done"

    for node in sorted(adjacency):
        if state.get(node) != "done":
            visit(node, [])
    return [edges[key] for key in sorted(edges)]


def primary_documents(documents: dict[str, dict[str, Any]]) -> list[str]:
    return sorted(
        rel for rel, doc in documents.items()
        if doc["okftype"] in {"reference.api.cpp", "reference.index.cpp"} and doc["family"] != "__nav__"
    )


def build_views(documents: dict[str, dict[str, Any]]) -> dict[str, Any]:
    views: dict[str, dict[str, list[str]]] = {
        "by_stage": {stage: [] for stage in STAGES},
        "by_role": {}, "by_operation": {}, "by_category": {},
        "by_dtype": {}, "by_dtype_pair": {}, "by_prototype_id": {}, "by_header": {}, "by_api": {},
    }
    for rel in primary_documents(documents):
        doc = documents[rel]
        for stage in stage_of(doc):
            views["by_stage"][stage].append(rel)
        for role in doc["tags"]["roles"]:
            views["by_role"].setdefault(role, []).append(rel)
        for op in doc["tags"]["operations"]:
            views["by_operation"].setdefault(op, []).append(rel)
        views["by_category"].setdefault(doc["category"], []).append(rel)
        for sig in doc["signatures"]:
            for dtype in sig["dtypes"]:
                views["by_dtype"].setdefault(dtype, []).append(rel)
            views["by_prototype_id"].setdefault(sig["prototype_id"], []).append(rel)
            for header in sig["headers"]:
                views["by_header"].setdefault(header, []).append(rel)
        pair = conversion_pair(rel, doc)
        if pair:
            views["by_dtype_pair"].setdefault(pair, []).append(rel)
        views["by_api"].setdefault(doc["family"], []).append(rel)
    return {
        name: {key: sorted(set(paths)) for key, paths in view.items()} if name != "by_stage"
        else {stage: sorted(set(paths)) for stage, paths in view.items()}
        for name, view in views.items()
    }


def validate_gaps(root: Path, errors: list[str]) -> dict[str, Any]:
    gaps = json.loads((INDEX_DIR / "gaps.json").read_text(encoding="utf-8"))
    for section in ("scope_out_gaps", "relation_gaps"):
        if section not in gaps:
            errors.append(f"gaps.json: missing section {section}")
    for gap in gaps.get("scope_out_gaps", []):
        for key in ("id", "keywords", "conclusion", "hints", "evidence"):
            if not gap.get(key):
                errors.append(f"gaps.json: gap {gap.get('id')} missing {key}")
        for hint in gap.get("hints", []):
            if not (root / hint["path"]).exists():
                errors.append(f"gaps.json: hint path not found: {hint['path']}")
        for ev in gap.get("evidence", []):
            if not (root / ev["path"]).exists():
                errors.append(f"gaps.json: evidence path not found: {ev['path']}")
    for gap in gaps.get("relation_gaps", []):
        if not (root / gap["from"]).is_file():
            errors.append(f"gaps.json: relation gap source not found: {gap['from']}")
    return gaps


def git_output(root: Path, args: list[str]) -> str:
    return subprocess.run(["git", *args], cwd=root, capture_output=True, text=True, check=True).stdout.strip()


def overview(index: dict[str, Any], gaps: dict[str, Any]) -> str:
    meta = index["meta"]
    lines = [
        "# OKF 索引概览（构建副产物，勿手改）", "",
        f"- 来源提交：`{meta['source_commit']}`（{meta['commit_date']}）",
        f"- 文档总数：{meta['doc_count']}（主文档 {meta['primary_count']}、导航页 {meta['nav_page_count']}、架构页 {meta['arch_page_count']}）",
        f"- 范围外缺口登记：{len(meta['gap_ids'])} 条；关系缺口：{meta['relation_gap_count']} 条", "",
        "## 分阶段候选规模", "", "| 阶段 | 主文档数 |", "| --- | ---: |",
    ]
    for stage, paths in index["views"]["by_stage"].items():
        lines.append(f"| {stage} | {len(paths)} |")
    lines += ["", "## 分类视图", "", "| 分类 | 主文档数 |", "| --- | ---: |"]
    for category, paths in index["views"]["by_category"].items():
        lines.append(f"| {category} | {len(paths)} |")
    lines += ["", "## 范围外缺口（详见 gaps.json）", ""]
    for gap in gaps.get("scope_out_gaps", []):
        lines.append(f"- **{gap['id']}**：{gap['conclusion']}")
    lines.append("")
    return "\n".join(lines)


def build(root: Path, out_dir: Path) -> list[Path]:
    errors: list[str] = []
    documents = load_raw_documents(root, errors)
    if documents:
        classify(documents, errors)
        rollup_families(documents)
        edges = check_graph(root, documents, errors)
    else:
        edges = []
    gaps = validate_gaps(root, errors) if not errors else {"scope_out_gaps": [], "relation_gaps": []}
    if errors:
        raise BuildError(f"{len(errors)} validation error(s):\n" + "\n".join(f"  - {e}" for e in errors[:30]))

    nav_pages = sorted(rel for rel, doc in documents.items() if doc["family"] == "__nav__")
    arch_pages = sorted(rel for rel, doc in documents.items() if doc["arch_of"])
    primary = primary_documents(documents)
    views = build_views(documents)
    reverse: dict[str, dict[str, list[str]]] = {kind: {} for kind in sorted(DEPENDENCY_KINDS)}
    for edge in edges:
        if not edge["out_of_scope"]:
            reverse[edge["kind"]].setdefault(edge["to"], []).append(edge["from"])

    index = {
        "schema": "okf-index/1",
        "meta": {
            "source_commit": git_output(root, ["rev-parse", "HEAD"]),
            "commit_date": git_output(root, ["log", "-1", "--format=%cI"]),
            "doc_count": len(documents),
            "primary_count": len(primary),
            "nav_page_count": len(nav_pages),
            "arch_page_count": len(arch_pages),
            "gap_ids": [gap["id"] for gap in gaps["scope_out_gaps"]],
            "relation_gap_count": len(gaps["relation_gaps"]),
        },
        "documents": {
            rel: {
                "okftype": doc["okftype"], "title": doc["title"], "description": doc["description"],
                "tags": doc["tags"], "signatures": doc["signatures"], "deps_out": doc["deps_out"],
                "examples": doc["examples"], "family": doc["family"], "category": doc["category"],
                "nav_page": doc["nav_page"], "arch_variants": doc["arch_variants"],
            }
            for rel, doc in sorted(documents.items())
        },
        "views": views,
        "graph": {"edges": edges, "reverse": reverse},
        "scope": {
            "all_docs": sorted(documents), "primary_docs": primary,
            "navigation_pages": nav_pages, "arch_pages": arch_pages,
        },
    }
    build_meta = {
        "schema": "okf-index-meta/1",
        "source_commit": index["meta"]["source_commit"],
        "taxonomy_sha256": hashlib.sha256(TAXONOMY_PATH.read_bytes()).hexdigest(),
        "gaps_sha256": hashlib.sha256((INDEX_DIR / "gaps.json").read_bytes()).hexdigest(),
        "document_sha256": {
            rel: hashlib.sha256((root / rel).read_bytes()).hexdigest() for rel in sorted(documents)
        },
    }
    out_dir.mkdir(parents=True, exist_ok=True)
    written: list[Path] = []
    for name, payload in (
        ("index.json", json.dumps(index, ensure_ascii=False, indent=1) + "\n"),
        ("build-meta.json", json.dumps(build_meta, ensure_ascii=False, indent=1) + "\n"),
        ("概览.md", overview(index, gaps)),
    ):
        target = out_dir / name
        target.write_text(payload, encoding="utf-8")
        written.append(target)
    return written


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=SKILL_ROOT.parents[1])
    parser.add_argument("--out", type=Path, default=INDEX_DIR)
    parser.add_argument("--check", action="store_true", help="verify committed index matches a rebuild")
    args = parser.parse_args()
    try:
        if args.check:
            with tempfile.TemporaryDirectory() as tmp:
                fresh = build(args.root, Path(tmp))
                for target in fresh:
                    committed = (args.out / target.name).read_text(encoding="utf-8")
                    if committed != target.read_text(encoding="utf-8"):
                        print(f"drift detected in {target.name}: index is stale, rerun build_index.py", file=sys.stderr)
                        return 1
            print("index is up to date")
            return 0
        for target in build(args.root, args.out):
            print(f"written: {target}")
        return 0
    except (BuildError, ValidationError, subprocess.CalledProcessError, OSError, KeyError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
