#!/usr/bin/env python3
"""Read-only OKF tag research; writes only the explicitly requested result artifact.

Run from any directory: python3 reproduce.py [--output results.json]
Requires the repository's existing PyYAML dependency. No production index rebuild.
"""
from __future__ import annotations

import argparse
import collections
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import yaml

ROOT = Path(__file__).resolve().parents[3]
INDEX_ROOT = ROOT / "okf_skills/okf-index"
SCOPE = "docs/zh/api/SIMD-API/c_api/"
CORE = {"roles", "operations"}
OPTIONAL = {"execution_models", "source_locations", "destination_locations", "traits"}
RETAINED = CORE | OPTIONAL
sys.dont_write_bytecode = True
sys.path.insert(0, str(INDEX_ROOT / "scripts"))
import build_index  # noqa: E402
import query  # noqa: E402


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_document(path):
    text = path.read_text(encoding="utf-8")
    if text.startswith("---\n"):
        _, header, body = text.split("---", 2)
        return yaml.safe_load(header), body
    return {}, text


def tag_map(fm):
    return {ns: values for row in fm.get("tag", []) for ns, values in row.items()}


def stats(documents, namespaces):
    return {
        ns: {
            "present": sum(ns in d["tags"] for d in documents.values()),
            "nonempty": sum(bool(d["tags"].get(ns)) for d in documents.values()),
            "value_occurrences": sum(len(d["tags"].get(ns, [])) for d in documents.values()),
            "values": dict(sorted(collections.Counter(
                value for d in documents.values() for value in d["tags"].get(ns, [])
            ).items())),
        } for ns in namespaces
    }


def footprint(documents, keep):
    tags = [{ns: vs for ns, vs in d["tags"].items() if ns in keep and vs}
            for d in documents.values()]
    return {
        "nonempty_fields": sum(len(t) for t in tags),
        "values": sum(len(v) for t in tags for v in t.values()),
        "mean_nonempty_fields": round(sum(len(t) for t in tags) / len(tags), 4),
    }


def select(documents, candidates, facets=None, keywords=(), available=None):
    """Exact-positive facet experiment, AND dimensions, OR values/keywords.

    A globally ablated dimension is skipped (wider candidate set), not treated
    as negative evidence. Per-document missing values do NOT prove incapability;
    this experiment reports positive matches, not a production fallback policy.
    """
    facets = facets or {}
    return sorted(p for p in candidates if all(
        (available is not None and ns not in available)
        or bool(set(values) & set(documents[p]["tags"].get(ns, [])))
        for ns, values in facets.items()
    ) and (not keywords or any(
        word.lower() in (documents[p]["title"] + " " + documents[p]["description"]).lower()
        for word in keywords
    )))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    index, gaps = query.load(INDEX_ROOT / "index")
    documents = index["documents"]
    taxonomy_path = ROOT / "okf_skills/okf-tag-generate/taxonomy/releases/core-v9-frozen.json"
    tax = json.loads(taxonomy_path.read_text())
    namespaces = list(tax["namespaces"])
    paths = sorted((ROOT / SCOPE).rglob("*.md"))
    raw = {}
    for path in paths:
        fm, _ = read_document(path)
        if fm.get("okftype"):
            raw[path.relative_to(ROOT).as_posix()] = {"tags": tag_map(fm), "okftype": fm["okftype"]}
    meta = json.loads((INDEX_ROOT / "index/build-meta.json").read_text())
    stale = [p for p, h in meta["document_sha256"].items() if digest(ROOT / p) != h]
    primary = index["scope"]["primary_docs"]
    # Serialization omits this builder-only flag. Restore it from the stored
    # primary set, including standalone architecture pages if present.
    for path, doc in documents.items():
        doc["standalone_primary"] = path in primary and doc["okftype"] == "reference.arch.cpp"
    reduced = copy.deepcopy(documents)
    for doc in reduced.values():
        doc["tags"] = {ns: values for ns, values in doc["tags"].items() if ns in RETAINED}
    old_views = build_index.build_views(documents)
    new_views = build_index.build_views(reduced)
    assert old_views == new_views, "Projection changed existing retrieval views"
    # Check against persisted views too, rather than only comparing our two builds.
    assert old_views == index["views"], "Current view builder differs from stored index"
    reduced_index = {**index, "documents": reduced, "views": new_views}

    cli_specs = [
        ["--operation", "copy"],
        ["--stage", "in"],
        ["--operation", "add"],
        ["--operation", "add", "--kw", "两个源矢量"],
        ["--stage", "out"],
        ["--stage", "in", "--kw", "非对齐"],
        ["--deps-of", "asc_copy_gm2ub_align", "--kind", "configures", "--reverse"],
        ["--gap", "同步"],
        ["--convert", "bfloat16", "float"],
        ["--api", "asc_copy_gm2ub"],
    ]
    cli = []
    for flags in cli_specs:
        command = [sys.executable, str(INDEX_ROOT / "scripts/query.py"), *flags, "--json"]
        run = subprocess.run(command, cwd=ROOT, text=True, capture_output=True, check=True)
        rows = json.loads(run.stdout)
        parsed = query.parser().parse_args(flags)
        assert rows == query.run_query(parsed, reduced_index, gaps), flags
        cli.append({"args": flags, "count": len(rows), "results": rows})

    cases = [
        {"id": "add", "facets": {"operations": ["add"]}},
        {"id": "gm_to_ub", "facets": {
            "operations": ["copy"], "roles": ["data_movement"],
            "source_locations": ["global_memory"], "destination_locations": ["unified_buffer"]}},
        {"id": "ub_to_gm", "facets": {
            "operations": ["copy"], "roles": ["data_movement"],
            "source_locations": ["unified_buffer"], "destination_locations": ["global_memory"]}},
        {"id": "unaligned_gm_to_ub", "facets": {
            "operations": ["copy"], "source_locations": ["global_memory"],
            "destination_locations": ["unified_buffer"], "traits": ["unaligned_transfer"]}},
        {"id": "movement_configuration", "facets": {"roles": ["data_movement_configuration"]}},
        {"id": "elementwise_add", "facets": {"operations": ["add"], "traits": ["elementwise"]}},
    ]
    facet_results = []
    for case in cases:
        baseline = select(documents, primary, case["facets"])
        recommended = select(reduced, primary, case["facets"])
        assert baseline == recommended
        facet_results.append({**case, "nine_field": baseline, "recommended": recommended,
                              "operations_only": select(documents, primary, case["facets"], available={"operations"}),
                              "ablation": {ns: select(documents, primary, case["facets"], available=RETAINED - {ns})
                                           for ns in namespaces}})

    # Transparent lexical baselines: literal request words and expanded aliases.
    lexical_specs = {
        "add_literal": ["加法"], "add_expanded": ["加法", "相加", "求和"],
        "add_specific": ["两个源矢量"], "copy_literal": ["搬运"],
        "route_literal": ["GM→UB"],
        "route_expanded": ["从GM搬运到UB", "从Global Memory搬运到Unified Buffer"],
        "unaligned": ["非对齐"], "movement_config_literal": ["配置"],
        "movement_config_expanded": ["设置", "配置"],
    }
    lexical = {key: {"keywords": words, "paths": select(documents, primary, keywords=words)}
               for key, words in lexical_specs.items()}

    # A1 from the repository testset: compare candidate retrieval, not final answers.
    testset = yaml.safe_load((ROOT / "okf_skills/okf-evaluate/testset/tasks.yaml").read_text())
    a1 = next(t for t in testset["tasks"] if t["id"] == "A1")
    a1_results = {}
    for name, selected in {
        "existing_cli": {r["path"] for i in (1, 2, 4) for r in cli[i]["results"]},
        "recommended_facets": set(facet_results[0]["recommended"] + facet_results[1]["recommended"] + facet_results[2]["recommended"]),
        "operations_only": set(facet_results[0]["operations_only"] + facet_results[1]["operations_only"] + facet_results[2]["operations_only"]),
        "description_literal": set(lexical["add_literal"]["paths"] + lexical["copy_literal"]["paths"]),
        "description_expanded": set(lexical["add_expanded"]["paths"] + lexical["copy_literal"]["paths"]),
    }.items():
        required = set(a1["required"])
        allowed = required | set(a1.get("optional", []))
        a1_results[name] = {"paths": sorted(selected), "required_hits": len(selected & required),
                            "required_total": len(required), "outside_required_optional": sorted(selected - allowed)}

    # Two real same-name docs, one outside the current index. This is a separate
    # evidence-checked design probe; never merge its counts into 140-doc results.
    memory = SCOPE + "vector_compute/vector_arith/asc_add.md"
    register = SCOPE + "reg_compute/reg_arith/asc_add.md"
    probes = [
        {"path": memory, "tag": "execution_models:memory_vector",
         "excerpt": "__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)"},
        {"path": register, "tag": "execution_models:register_vector",
         "excerpt": "本接口为Reg矢量计算接口，仅在AIV上生效。"},
        {"path": SCOPE + "sync/intra_core_sync/asc_sync_pipe.md", "tag": "operations:synchronization",
         "excerpt": "完成同一流水内的同步控制，用于在同一流水内部约束执行顺序："},
        {"path": SCOPE + "defs/union/asc_ndim_pad_count_config.md", "tag": "no_callable_tags",
         "excerpt": "union asc_ndim_pad_count_config {"},
    ]
    for probe in probes:
        _, body = read_document(ROOT / probe["path"])
        assert probe["excerpt"] in body, probe
        probe["sha256"] = digest(ROOT / probe["path"])
    # Replay the A1 verification reads independently of the corpus statistics.
    # Full file bytes and extracted section bytes are separate metrics.
    read_paths = [memory,
        SCOPE + "vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_2201.md",
        SCOPE + "vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_2201.md",
        SCOPE + "vector_datamove/asc_ndim_copy_gm2ub.md",
        SCOPE + "vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_2201.md",
        SCOPE + "vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_2201.md"]
    reads = []
    for path in read_paths:
        _, body = read_document(ROOT / path)
        sections = {}
        for heading in ("功能说明", "约束说明"):
            marker = f"## {heading}\n"
            if marker in body:
                sections[heading] = body.split(marker, 1)[1].split("\n## ", 1)[0].strip()
        reads.append({"path": path, "file_bytes": (ROOT / path).stat().st_size,
                      "section_bytes": sum(len(v.encode()) for v in sections.values()), "sections": sections})
    probe_docs = {memory: {"title": "asc_add", "description": "", "tags": {
        "operations": ["add"], "execution_models": ["memory_vector"]}},
        register: {"title": "asc_add", "description": "", "tags": {
            "operations": ["add"], "execution_models": ["register_vector"]}}}
    model_facets = {"operations": ["add"], "execution_models": ["register_vector"]}
    hashes = {str(p.relative_to(ROOT)): digest(p) for p in paths}
    relevant_files = [taxonomy_path, INDEX_ROOT / "index/index.json",
                      INDEX_ROOT / "scripts/build_index.py", INDEX_ROOT / "scripts/query.py",
                      ROOT / "okf_skills/okf-evaluate/testset/tasks.yaml", Path(__file__)]
    result = {
        "schema": "okf-tag-minimal-research/1", "date": "2026-09-23",
        "source_commit": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip(),
        "method": "deterministic metadata projection and lexical/facet candidate retrieval; not agent end-to-end benchmarking",
        "scope": {"markdown": len(paths), "okf": len(raw), "indexed": len(documents),
                  "primary": len(primary), "no_okf": len(paths)-len(raw), "stale_indexed_documents": stale,
                  "indexed_types": dict(collections.Counter(d["okftype"] for d in documents.values()))},
        "raw_stats": stats(raw, namespaces), "rolled_up_index_stats": stats(documents, namespaces),
        "taxonomy_value_counts": {ns: len(v["values"]) for ns, v in tax["namespaces"].items()},
        "footprint": {"old_key_slots": sum(len(d["tags"]) for d in raw.values()),
                      "old": footprint(raw, set(namespaces)), "core_only": footprint(raw, CORE),
                      "core_plus_all_existing_optional": footprint(raw, RETAINED)},
        "existing_views_preserved": sorted(old_views),
        "cli_trace": cli, "facet_cases": facet_results, "lexical_cases": lexical,
        "a1": {"prompt": a1["prompt"], "required": a1["required"], "results": a1_results},
        "a1_verification_reads": reads,
        "outside_index_probes": {"evidence": probes,
            "with_model": select(probe_docs, probe_docs, model_facets),
            "without_model": select(probe_docs, probe_docs, model_facets, available={"operations"})},
        "input_sha256": hashes | {str(p.relative_to(ROOT)): digest(p) for p in relevant_files},
    }
    encoded = json.dumps(result, ensure_ascii=False, indent=2) + "\n"
    if args.output:
        target = args.output.resolve()
        # Keep generated output confined to this report's directory.
        if target.parent != Path(__file__).resolve().parent or target.suffix != ".json":
            parser.error("--output must be a .json artifact beside this script")
        target.write_text(encoded, encoding="utf-8")
        print(json.dumps({"output": str(target), "scope": result["scope"],
                          "footprint": result["footprint"], "views_preserved": len(old_views)}, ensure_ascii=False, indent=2))
    else:
        print(encoded, end="")


if __name__ == "__main__":
    main()
