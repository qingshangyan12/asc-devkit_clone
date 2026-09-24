#!/usr/bin/env python3
"""Build the persistent OKF retrieval index from stage-3 YAML front matter.

The index is a pure derivation of the scoped Markdown front-matter blocks
(the whole docs/zh/api/SIMD-API/c_api tree):
parse -> validate -> family rollup -> inverted views -> dependency graph.
Build is deterministic (no wall-clock values); drift is detected via --check.

Markdown files in scope that do not carry an OKF front matter yet are skipped
and counted as pending (scope.pending_no_okf); they join the index once their
stage-3 YAML headers are generated and the index is rebuilt.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path, PurePosixPath
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
TAXONOMY_PATH = SKILL_ROOT.parent / "okf-yaml-generate" / "taxonomy" / "releases" / "core-v9-frozen.json"
INDEX_DIR = SKILL_ROOT / "index"
SCOPE_DIRS = (
    "docs/zh/api/SIMD-API/c_api",
)
OKFTYPES = {"reference.api.cpp", "reference.arch.cpp", "reference.index.cpp", "reference.type.cpp"}
STAGES = ("in", "out", "compute")
ARCH_RE = re.compile(r"(?P<family>.+)_arch_(?P<arch>\d+)(?:_deprecated)?$")
CONVERT_RE = re.compile(r"asc_(?P<src>[a-z0-9]+)2(?P<dst>[a-z0-9]+)$")
DTYPE_ALIASES = {
    "f16": "half", "fp16": "half", "f32": "float", "fp32": "float", "bf16": "bfloat16",
    "int": "int32", "uint": "uint32", "int4b": "int4", "b8": "int8",
    "e4m3": "fp8_e4m3fn", "e5m2": "fp8_e5m2", "e8m0": "fp8_e8m0", "hif8": "hifloat8",
    "e1m2x2": "fp4x2_e1m2", "e2m1x2": "fp4x2_e2m1",
}
PAIR_TOKENS = {
    "half", "float", "bfloat16", "int4", "int8", "uint8",
    "int16", "uint16", "int32", "uint32", "int64", "uint64",
    "fp8_e4m3fn", "fp8_e5m2", "fp8_e8m0", "hifloat8",
    "fp4x2_e1m2", "fp4x2_e2m1",
}
LOCAL_BUFFERS = {
    "unified_buffer", "l1_buffer", "l0a_buffer", "l0b_buffer", "l0c_buffer",
    "bias_table_buffer", "fixpipe_buffer", "vector_register", "register_tensor",
}


class BuildError(ValueError):
    pass


def normalize_dtype(value: str) -> str:
    token = value[:-2] if value.endswith("_t") else value
    if token == "int4b":
        token = "int4"
    return DTYPE_ALIASES.get(token, token)


def load_raw_documents(root: Path, errors: list[str]) -> tuple[dict[str, dict[str, Any]], list[str]]:
    namespaces, tax_values = taxonomy(TAXONOMY_PATH)
    taxonomy_payload = json.loads(TAXONOMY_PATH.read_text(encoding="utf-8"))
    # Only api_surfaces is hard-required here: it is the navigation anchor proved by
    # the resource path and is always rendered. roles (and other mandatory namespaces)
    # are enforced at the generation gate (okf-yaml-generate validate_tags + resolver
    # decisions); nav pages and resolver-sanctioned gaps legitimately omit roles, so
    # the index must not re-enforce its presence on the pure derivation.
    required_namespaces = {"api_surfaces"} & set(
        taxonomy_payload.get("mandatory_namespaces", ["api_surfaces", "roles"])
    )
    documents: dict[str, dict[str, Any]] = {}
    pending: list[str] = []
    for scope in SCOPE_DIRS:
        for path in sorted((root / scope).rglob("*.md")):
            rel = path.relative_to(root).as_posix()
            fm, _, _ = split_front_matter(path.read_text(encoding="utf-8"))
            if not fm:
                pending.append(rel)
                continue
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
            missing_ns = sorted(required_namespaces - set(tags))
            if missing_ns:
                errors.append(f"{rel}: missing tag namespaces: {missing_ns}")
            signatures = []
            for sig in fm.get("signature") or []:
                if not isinstance(sig, dict):
                    errors.append(f"{rel}: signature entry must be an object")
                    continue
                identifier = sig.get("prototype_id") or sig.get("override_id") or sig.get("macro_id")
                if not identifier:
                    errors.append(f"{rel}: signature entry without prototype_id/override_id/macro_id")
                    continue
                sig_headers = []
                for header in sig.get("headers", []):
                    header_value = str(header)
                    for prefix in ("/include/", "include/"):
                        if header_value.startswith(prefix):
                            header_value = header_value[len(prefix):]
                            break
                    if header_value.startswith("/"):
                        header_value = header_value[1:]
                    if not (root / "include" / header_value).is_file():
                        errors.append(f"{rel}: header not found: include/{header_value}")
                        continue
                    sig_headers.append(header_value)
                dtype_field = sig.get("dtype")
                if isinstance(dtype_field, dict):
                    dtype_values = dtype_field.get("values", [])
                else:
                    dtype_values = dtype_field or []
                signatures.append({
                    "prototype_id": identifier,
                    "id_kind": "macro_id" if sig.get("macro_id") else ("override_id" if sig.get("override_id") else "prototype_id"),
                    "headers": sorted(set(sig_headers)),
                    "archs": sorted(sig.get("archs", [])),
                    "dtypes": sorted({normalize_dtype(v) for v in dtype_values}),
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
                if "file" in ex:
                    if not (root / base / str(ex["file"])).is_file():
                        errors.append(f"{rel}: example not found: {base}/{ex['file']}")
                        continue
                    entry = {"path": f"/{base}", "file": str(ex["file"]), "archs": sorted(ex.get("archs", []))}
                else:
                    if not (root / base).is_dir():
                        errors.append(f"{rel}: example directory not found: {base}")
                        continue
                    entry = {"path": f"/{base}", "archs": sorted(ex.get("archs", []))}
                examples.append(entry)
            documents[rel] = {
                "okftype": fm["okftype"], "title": fm["title"], "description": fm["description"],
                "tags": tags, "signatures": signatures, "deps_out": deps, "examples": examples,
                "family": "", "arch_of": "", "category": "", "nav_page": "", "arch_variants": [],
                "standalone_primary": False,
            }
    return documents, pending


def classify(documents: dict[str, dict[str, Any]], errors: list[str], warnings: list[str]) -> None:
    scope_root = SCOPE_DIRS[0]
    root_nav = f"{scope_root}/c_api.md"
    by_dir: dict[str, list[str]] = {}
    for rel in documents:
        by_dir.setdefault(str(PurePosixPath(rel).parent), []).append(rel)
    landing: dict[str, str] = {}
    family_dirs: set[str] = set()
    for directory, members in by_dir.items():
        parts = PurePosixPath(directory).parts
        if len(parts) != 7:
            continue
        sub = parts[6]
        index_pages = [r for r in members if documents[r]["okftype"] == "reference.index.cpp"]
        exact = [r for r in index_pages if PurePosixPath(r).stem == sub]
        if len(exact) > 1:
            errors.append(f"{directory}: multiple landing-page candidates named {sub}.md")
        elif exact:
            landing[directory] = exact[0]
        elif len(index_pages) == 1:
            landing[directory] = index_pages[0]
        elif index_pages:
            errors.append(f"{directory}: cannot determine landing page for sub directory")
        # Directories without any index page (e.g. single-API dirs like reg_compute/vf_call)
        # have no landing page; their members fall back to the category page for navigation.
        if any(
            documents[r]["okftype"] == "reference.arch.cpp" and ARCH_RE.fullmatch(PurePosixPath(r).stem)
            for r in members
        ):
            family_dirs.add(directory)
    for rel, doc in documents.items():
        parts = PurePosixPath(rel).parts
        stem = parts[-1][: -len(".md")]
        if len(parts) not in (6, 7, 8):
            errors.append(f"{rel}: unexpected path depth under {scope_root}")
            continue
        directory = str(PurePosixPath(rel).parent)
        doc["category"] = parts[5] if len(parts) > 6 else "__root__"
        if len(parts) == 6:
            doc["nav_page"] = root_nav
        elif len(parts) == 7:
            doc["nav_page"] = f"{scope_root}/{parts[5]}/{parts[5]}.md"
        else:
            doc["nav_page"] = landing.get(directory, "") or f"{scope_root}/{parts[5]}/{parts[5]}.md"
        if doc["okftype"] == "reference.arch.cpp":
            match = ARCH_RE.fullmatch(stem)
            if not match:
                errors.append(f"{rel}: arch page does not match family_arch_<n> naming")
                continue
            if len(parts) == 8 and (match["family"] == parts[6] or match["family"].startswith(parts[6] + "_")):
                doc["family"] = doc["arch_of"] = parts[6]
            else:
                doc["family"] = doc["arch_of"] = match["family"]
        elif doc["okftype"] == "reference.index.cpp":
            if (len(parts) == 6 and stem == "c_api") or (len(parts) == 7 and stem == parts[5]):
                doc["family"] = "__nav__"
            elif len(parts) == 8 and landing.get(directory) == rel:
                doc["family"] = stem if directory in family_dirs else "__nav__"
            else:
                doc["family"] = stem
        else:
            doc["family"] = stem
        if doc["okftype"] in {"reference.api.cpp", "reference.arch.cpp"} and not doc["signatures"]:
            warnings.append(f"{rel}: api/arch page without signature (sanctioned blank, degraded indexing)")
        if doc["okftype"] == "reference.type.cpp" and any(
            sig.get("id_kind") != "macro_id" for sig in doc["signatures"]
        ):
            errors.append(f"{rel}: type page with signature")


def rollup_families(documents: dict[str, dict[str, Any]]) -> None:
    hosted_families = {
        doc["family"] for doc in documents.values()
        if doc["okftype"] == "reference.index.cpp" and doc["family"] != "__nav__"
    }
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
                doc["tags"][ns] = sorted(set(doc["tags"].get(ns, [])) | set(values))
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
            (e["path"], e.get("file", "")): e for e in doc["examples"]
        }.values())
    for rel, doc in documents.items():
        if doc["okftype"] == "reference.arch.cpp" and doc["family"] not in hosted_families:
            doc["standalone_primary"] = True


def stage_of(doc: dict[str, Any]) -> list[str]:
    tags = doc["tags"]
    roles = set(tags.get("roles", []))
    src = set(tags.get("source_locations", []))
    dst = set(tags.get("destination_locations", []))
    local_src = src & LOCAL_BUFFERS
    local_dst = dst & LOCAL_BUFFERS
    stages = []
    if "data_movement" in roles and (
        ("global_memory" in src and local_dst) or ("unified_buffer" in src and "vector_register" in dst)
    ):
        stages.append("in")
    if "data_movement" in roles and (
        (local_src and "global_memory" in dst) or ("vector_register" in src and "unified_buffer" in dst)
    ):
        stages.append("out")
    if "compute" in roles:
        stages.append("compute")
    return stages


def conversion_pair(rel: str, doc: dict[str, Any]) -> str | None:
    if "conversion" not in doc["tags"].get("operations", []):
        return None
    stem = Path(rel).stem
    match = CONVERT_RE.fullmatch(stem)
    if match:
        src, dst = normalize_dtype(match.group("src")), normalize_dtype(match.group("dst"))
        if src in PAIR_TOKENS and dst in PAIR_TOKENS:
            return f"{src}->{dst}"
    # Greedy backtrack mis-splits packed-dtype names (e.g. bfloat162e1m2x2 -> src=bfloat162e1m,
    # dst=x2), so scan every "2" split and keep the first one where both sides are pair tokens.
    body = stem[len("asc_"):] if stem.startswith("asc_") else ""
    for cut in (i for i, ch in enumerate(body) if ch == "2"):
        src, dst = normalize_dtype(body[:cut]), normalize_dtype(body[cut + 1:])
        if src in PAIR_TOKENS and dst in PAIR_TOKENS:
            return f"{src}->{dst}"
    return None


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
        if doc["standalone_primary"] or (
            doc["okftype"] in {"reference.api.cpp", "reference.index.cpp", "reference.type.cpp"}
            and doc["family"] != "__nav__"
        )
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
        for role in doc["tags"].get("roles", []):
            views["by_role"].setdefault(role, []).append(rel)
        for op in doc["tags"].get("operations", []):
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
        f"- 文档总数：{meta['doc_count']}（主文档 {meta['primary_count']}、导航页 {meta['nav_page_count']}、架构页 {meta['arch_page_count']}）",
        f"- 范围内尚未生成 OKF 头的文档：{meta['pending_no_okf']} 篇（跳过索引，待补齐后重建）",
        f"- signature 降级页（api/arch 页无 signature，仅按 tag/description/依赖检索）：{meta['degraded_no_signature']} 篇",
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
    warnings: list[str] = []
    documents, pending = load_raw_documents(root, errors)
    if documents:
        classify(documents, errors, warnings)
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
            "pending_no_okf": len(pending),
            "degraded_no_signature": len(warnings),
            "degraded_no_signature_docs": sorted(warnings),
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


INFORMATIONAL_META = ("source_commit", "commit_date")


def strip_informational(payload: dict) -> dict:
    stripped = json.loads(json.dumps(payload))
    for key in INFORMATIONAL_META:
        stripped.pop(key, None)
        stripped.get("meta", {}).pop(key, None)
    return stripped


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
                    if target.name.endswith(".json"):
                        committed = strip_informational(json.loads((args.out / target.name).read_text(encoding="utf-8")))
                        rebuilt = strip_informational(json.loads(target.read_text(encoding="utf-8")))
                    else:
                        committed = (args.out / target.name).read_text(encoding="utf-8")
                        rebuilt = target.read_text(encoding="utf-8")
                    if committed != rebuilt:
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
