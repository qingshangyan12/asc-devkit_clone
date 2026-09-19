#!/usr/bin/env python3
"""Query the persistent OKF index.

Filter queries (--stage/--operation/--category/--dtype/--kw/--convert) combine
with AND across dimensions and OR within comma-separated values. Standalone
queries (--api/--deps-of/--gap) return structured cards and ignore filters.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

SCRIPTS = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPTS))
from build_index import INDEX_DIR, STAGES, normalize_dtype  # noqa: E402

FILTER_KEYS = ("stage", "operation", "category", "dtype", "kw", "convert")


def load(index_dir: Path) -> tuple[dict[str, Any], dict[str, Any]]:
    index = json.loads((index_dir / "index.json").read_text(encoding="utf-8"))
    gaps = json.loads((index_dir / "gaps.json").read_text(encoding="utf-8"))
    return index, gaps


def csv(value: str | None) -> list[str]:
    return [item.strip() for item in value.split(",") if item.strip()] if value else []


def resolve_api(index: dict[str, Any], name: str) -> str:
    view = index["views"]["by_api"]
    if name in view:
        return view[name][0]
    if f"{name}.md" in {Path(p).name for p in index["documents"]}:
        return next(p for p in index["documents"] if Path(p).name == f"{name}.md")
    for path in index["documents"]:
        if Path(path).stem == name:
            return path
    raise ValueError(f"unknown API or document: {name}")


def why_tags(index: dict[str, Any], path: str, dims: dict[str, list[str]]) -> list[str]:
    doc = index["documents"][path]
    reasons = []
    for stage in dims.get("stage", []):
        if path in index["views"]["by_stage"].get(stage, []):
            reasons.append(f"stage:{stage}")
    for op in dims.get("operation", []):
        if path in index["views"]["by_operation"].get(op, []):
            reasons.append(f"operation:{op}")
    for cat in dims.get("category", []):
        if path in index["views"]["by_category"].get(cat, []):
            reasons.append(f"category:{cat}")
    for dtype in dims.get("dtype", []):
        if path in index["views"]["by_dtype"].get(dtype, []):
            reasons.append(f"dtype:{dtype}")
    for pair in dims.get("pair", []):
        if path in index["views"]["by_dtype_pair"].get(pair, []):
            reasons.append(f"convert:{pair}")
    for kw in dims.get("kw", []):
        haystack = f"{doc['title']} {doc['description']}".lower()
        if kw.lower() in haystack:
            reasons.append(f"kw:{kw}")
    return reasons


def filter_query(index: dict[str, Any], args: argparse.Namespace) -> list[dict[str, Any]]:
    dims: dict[str, list[str]] = {
        "stage": [s for s in csv(args.stage) if s] if args.stage else [],
        "operation": csv(args.operation),
        "category": csv(args.category),
        "dtype": [normalize_dtype(d) for d in csv(args.dtype)],
        "kw": csv(args.kw),
        "pair": [],
    }
    if args.convert:
        src = normalize_dtype(args.convert[0])
        dims["pair"] = [f"{src}->{normalize_dtype(dst)}" for dst in csv(args.convert[1])]
    if not any(dims[key] for key in dims):
        raise ValueError("no filter given; use --stage/--operation/--category/--dtype/--kw/--convert")
    unknown_stage = [s for s in dims["stage"] if s not in STAGES]
    if unknown_stage:
        raise ValueError(f"unknown stage(s): {unknown_stage}; valid: {list(STAGES)}")
    for cat in dims["category"]:
        if cat not in index["views"]["by_category"]:
            raise ValueError(f"unknown category: {cat}; valid: {sorted(index['views']['by_category'])}")
    for op in dims["operation"]:
        if op not in index["views"]["by_operation"]:
            raise ValueError(f"unknown operation: {op}")
    for dtype in dims["dtype"]:
        if dtype not in index["views"]["by_dtype"]:
            raise ValueError(f"unknown dtype: {dtype}")

    candidates: set[str] | None = None
    for key, view_name in (
        ("stage", "by_stage"), ("operation", "by_operation"), ("category", "by_category"),
        ("dtype", "by_dtype"), ("pair", "by_dtype_pair"),
    ):
        values = dims[key]
        if not values:
            continue
        matched: set[str] = set()
        for value in values:
            matched.update(index["views"][view_name].get(value, []))
        candidates = matched if candidates is None else candidates & matched
    if dims["kw"]:
        matched = {
            path for path in (candidates if candidates is not None else index["documents"])
            if any(kw.lower() in f"{index['documents'][path]['title']} {index['documents'][path]['description']}".lower()
                   for kw in dims["kw"])
        }
        candidates = matched
    return [
        {"path": path, "why": why_tags(index, path, dims)}
        for path in sorted(candidates or set())
    ]


def api_card(index: dict[str, Any], name: str) -> list[dict[str, Any]]:
    owner = resolve_api(index, name)
    doc = index["documents"][owner]
    results = [{"path": owner, "why": ["api:owner"]}]
    for variant in doc["arch_variants"]:
        results.append({"path": variant, "why": [f"arch-variant:{Path(variant).stem.split('_arch_')[-1]}"]})
    if doc["nav_page"] != owner:
        results.append({"path": doc["nav_page"], "why": ["category-page"]})
    return results


def deps_query(index: dict[str, Any], target: str, kind: str | None, reverse: bool) -> list[dict[str, Any]]:
    path = resolve_api(index, target)
    doc = index["documents"][path]
    results: list[dict[str, Any]] = []
    if reverse:
        for edge in index["graph"]["edges"]:
            if edge["to"] == path and (not kind or edge["kind"] == kind):
                results.append({"path": edge["from"], "why": [f"reverse:{edge['kind']}"],
                                "condition": edge["condition"], "out_of_scope": False})
    else:
        for dep in doc["deps_out"]:
            if kind and dep["kind"] != kind:
                continue
            out_of_scope = dep["path"] not in index["documents"]
            results.append({
                "path": dep["path"], "why": [f"dep:{dep['kind']}"],
                "condition": dep["condition"], "out_of_scope": out_of_scope,
            })
    return results


def gap_query(gaps: dict[str, Any], keyword: str) -> list[dict[str, Any]]:
    keywords = [k.strip().lower() for k in keyword.split(",") if k.strip()]
    if not keywords:
        raise ValueError("--gap requires a non-empty keyword")

    def hit(haystack: str) -> bool:
        return any(kw in haystack.lower() for kw in keywords)

    results = []
    for gap in gaps["scope_out_gaps"]:
        haystack = " ".join([gap["id"], gap["conclusion"], *gap["keywords"]])
        if hit(haystack):
            results.append({"gap_id": gap["id"], "conclusion": gap["conclusion"], "hints": gap["hints"]})
    for gap in gaps["relation_gaps"]:
        haystack = f"{gap['from']} {gap['note']}"
        if hit(haystack):
            results.append({"gap_id": f"relation:{Path(gap['from']).stem}", "conclusion": gap["note"],
                            "hints": [{"path": gap["from"], "note": "范围内部文档"}]})
    return results


def run_query(args: argparse.Namespace, index: dict[str, Any], gaps: dict[str, Any]) -> list[dict[str, Any]]:
    if args.gap is not None:
        return gap_query(gaps, args.gap)
    if args.api:
        return api_card(index, args.api)
    if args.deps_of:
        return deps_query(index, args.deps_of, args.kind, args.reverse)
    return filter_query(index, args)


def render(index: dict[str, Any], results: list[dict[str, Any]]) -> str:
    if not results:
        return "no match"
    lines = []
    for row in results:
        if "gap_id" in row:
            hints = "; ".join(h["path"] for h in row["hints"])
            lines.append(f"{row['gap_id']}\n  {row['conclusion']}\n  hints: {hints}")
            continue
        doc = index["documents"].get(row["path"])
        name = doc["title"] if doc else Path(row["path"]).stem
        desc = (doc["description"] if doc else "")[:60]
        flags = "".join(" [范围外]" if row.get("out_of_scope") else "")
        condition = f"（{row['condition']}）" if row.get("condition") else ""
        lines.append(f"{row['path']}{flags} | {name} | {','.join(row['why'])}{condition}\n  {desc}")
    return "\n".join(lines)


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--index-dir", type=Path, default=INDEX_DIR)
    p.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    p.add_argument("--stage", help="comma list: in,out,compute")
    p.add_argument("--operation", help="comma list of operations (e.g. divide,gather)")
    p.add_argument("--category", help="comma list of categories (e.g. vector_compare)")
    p.add_argument("--dtype", help="comma list of dtypes (e.g. half,float)")
    p.add_argument("--kw", help="comma list of keywords over title+description")
    p.add_argument("--convert", nargs=2, metavar=("SRC", "DST"), help="dtype conversion pair, DST may be comma list")
    p.add_argument("--api", help="family card: owner + arch variants + category page")
    p.add_argument("--deps-of", dest="deps_of", help="outgoing dependencies of an API name or path")
    p.add_argument("--kind", choices=["requires", "configures", "related", "constrained_by"])
    p.add_argument("--reverse", action="store_true", help="with --deps-of: incoming edges instead")
    p.add_argument("--gap", help="search scope-out gap registry by keyword")
    return p


def main() -> int:
    args = parser().parse_args()
    try:
        index, gaps = load(args.index_dir)
        results = run_query(args, index, gaps)
        print(json.dumps(results, ensure_ascii=False, indent=1) if args.json else render(index, results))
        return 0
    except (ValueError, KeyError, OSError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
