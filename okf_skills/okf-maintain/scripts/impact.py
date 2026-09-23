#!/usr/bin/env python3
"""Impact analysis for OKF knowledge maintenance.

Compares the committed okf-index fingerprints (build-meta.json document
sha256 / taxonomy / gaps) against the current scoped Markdown tree, then
derives the blast radius of every drifted document from the committed
index: family members, reverse dependency edges, and the evaluation tasks
/ dry-run queries that reference the affected paths.

Read-only, stdlib only, writes nothing. Exit codes: 0 = no drift,
1 = index assets missing or corrupt (rerun build_index.py first),
2 = drift detected (report still printed).
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
SCOPE_DIR = "docs/zh/api/SIMD-API/c_api"
ADVICE = {
    "added": "按范围清单.md 角色判定规则定角色，经 okf-yaml-generate 生成 OKF 头",
    "removed": "修正指向该文档的依赖边（见反向依赖），再重跑 build_index.py",
    "modified": "核对 YAML 头与变更后正文的一致性（description/tag/signature/dependencies/examples）",
}


def resolve_paths(root: Path) -> dict[str, Path]:
    okf = root / "okf-skills"
    return {
        "build_meta": okf / "okf-index/index/build-meta.json",
        "index": okf / "okf-index/index/index.json",
        "gaps": okf / "okf-index/index/gaps.json",
        "taxonomy": okf / "okf-yaml-generate/taxonomy/releases/core-v9-frozen.json",
        "tasks": okf / "okf-evaluate/testset/tasks.yaml",
        "dryrun": okf / "okf-index/tests/dryrun.yaml",
        "scope": root / SCOPE_DIR,
    }


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def scan_scope(scope: Path, root: Path) -> dict[str, str]:
    return {
        path.relative_to(root).as_posix(): sha256_file(path)
        for path in sorted(scope.rglob("*.md"))
    }


def diff_documents(committed: dict[str, str], current: dict[str, str]) -> dict[str, list[str]]:
    return {
        "added": sorted(set(current) - set(committed)),
        "removed": sorted(set(committed) - set(current)),
        "modified": sorted(
            path for path in set(committed) & set(current) if committed[path] != current[path]
        ),
    }


def category_of(rel: str) -> str:
    parts = rel.split("/")
    return parts[5] if len(parts) > 6 else "__root__"


def build_family_map(documents: dict[str, dict]) -> dict[str, list[str]]:
    families: dict[str, list[str]] = {}
    for rel, doc in documents.items():
        family = doc.get("family", "")
        if family and family != "__nav__":
            families.setdefault(family, []).append(rel)
    return {family: sorted(members) for family, members in families.items()}


def doc_impact(
    path: str,
    documents: dict[str, dict],
    edges: list[dict],
    family_map: dict[str, list[str]],
) -> dict:
    doc = documents.get(path)
    if doc is None:
        return {
            "path": path, "known_to_index": False, "okftype": "", "title": "",
            "family": Path(path).stem, "category": category_of(path),
            "family_members": [], "incoming_deps": [], "outgoing_deps": [],
        }
    family = doc.get("family", "")
    return {
        "path": path, "known_to_index": True,
        "okftype": doc["okftype"], "title": doc["title"],
        "family": family, "category": doc["category"],
        "family_members": [m for m in family_map.get(family, []) if m != path],
        "incoming_deps": [
            {"from": e["from"], "kind": e["kind"]} for e in edges if e["to"] == path
        ],
        "outgoing_deps": [
            {"path": d["path"], "kind": d["kind"]} for d in doc["deps_out"]
        ],
    }


def split_id_blocks(text: str) -> list[tuple[str, str]]:
    """Split a YAML list-of-mappings into (id, block-text) pairs by `- id:` keys."""
    blocks: list[tuple[str, str]] = []
    current_id, lines = None, []
    for line in text.splitlines():
        match = re.match(r"\s*-\s+id:\s*(\S+)", line)
        if match:
            if current_id:
                blocks.append((current_id, "\n".join(lines)))
            current_id, lines = match.group(1), [line]
        elif current_id:
            lines.append(line)
    if current_id:
        blocks.append((current_id, "\n".join(lines)))
    return blocks


def affected_tasks(paths: list[str], tasks_text: str) -> list[str]:
    return [
        task_id for task_id, block in split_id_blocks(tasks_text)
        if any(path in block for path in paths)
    ]


def affected_dryrun(impact: list[dict], dryrun_text: str) -> dict[str, list[str]]:
    tokens = {
        token for item in impact
        for token in (item["family"], item["category"]) if token and token != "__root__"
    }
    queries = []
    for line in dryrun_text.splitlines():
        match = re.match(r"\s*([A-Z]\d+):\s*\[", line)
        if match and any(token in line for token in tokens):
            queries.append(match.group(1))
    extended = [
        ext_id for ext_id, block in split_id_blocks(dryrun_text)
        if any(token in block for token in tokens)
    ]
    return {"queries": queries, "extended": extended}


def analyze(root: Path) -> dict:
    paths = resolve_paths(root)
    for key in ("build_meta", "index", "scope"):
        if not paths[key].exists():
            raise FileNotFoundError(
                f"{key} asset not found: {paths[key]}（先运行 okf-skills/okf-index/scripts/build_index.py）"
            )
    build_meta = json.loads(paths["build_meta"].read_text(encoding="utf-8"))
    index = json.loads(paths["index"].read_text(encoding="utf-8"))
    committed: dict[str, str] = build_meta["document_sha256"]
    drift = diff_documents(committed, scan_scope(paths["scope"], root))
    fingerprints = {
        "taxonomy_changed": sha256_file(paths["taxonomy"]) != build_meta["taxonomy_sha256"],
        "gaps_changed": sha256_file(paths["gaps"]) != build_meta["gaps_sha256"],
    }
    edges = index["graph"]["edges"]
    family_map = build_family_map(index["documents"])
    affected = sorted(set(drift["added"]) | set(drift["removed"]) | set(drift["modified"]))
    impact = [doc_impact(path, index["documents"], edges, family_map) for path in affected]
    tasks_text = paths["tasks"].read_text(encoding="utf-8") if paths["tasks"].exists() else ""
    dryrun_text = paths["dryrun"].read_text(encoding="utf-8") if paths["dryrun"].exists() else ""
    has_drift = bool(affected) or any(fingerprints.values())
    return {
        "schema": "okf-impact/1",
        "baseline": {
            "source_commit": build_meta["source_commit"],
            "committed_doc_count": len(committed),
        },
        "drift": drift,
        "fingerprints": fingerprints,
        "impact": impact,
        "affected_eval_tasks": affected_tasks(affected, tasks_text),
        "affected_dryrun": affected_dryrun(impact, dryrun_text),
        "has_drift": has_drift,
    }


def text_report(report: dict) -> str:
    drift, fingerprints = report["drift"], report["fingerprints"]
    lines = [
        "OKF 维护影响面分析（okf-impact/1）",
        f"索引基线：{report['baseline']['committed_doc_count']} 篇，源提交 {report['baseline']['source_commit'][:8]}",
        "",
    ]
    if not report["has_drift"]:
        return "\n".join(lines + ["无漂移：文档、词表与缺口登记均与索引基线一致，无需维护动作。"])
    lines.append("一、变更对账")
    for key, label in (("modified", "修改"), ("added", "新增"), ("removed", "删除")):
        if drift[key]:
            lines.append(f"- {label} {len(drift[key])}：{', '.join(drift[key])}")
    if fingerprints["taxonomy_changed"] or fingerprints["gaps_changed"]:
        lines += ["", "二、指纹漂移"]
        if fingerprints["taxonomy_changed"]:
            lines.append("- 词表已变化：core-v9-frozen.json 与 build-meta 不一致，全量文档需按新词表重校验")
        if fingerprints["gaps_changed"]:
            lines.append("- 缺口登记已变化：gaps.json 与 build-meta 不一致，重跑 build_index.py 同步指纹")
    lines += ["", "三、影响面"]
    for item in report["impact"]:
        known = "" if item["known_to_index"] else "（索引中无记录）"
        lines.append(
            f"- {item['path']}{known}：{item['okftype'] or '角色待判定'} /"
            f" family={item['family']} / 分类={item['category']}"
        )
        if item["family_members"]:
            lines.append(f"  - 家族成员：{', '.join(item['family_members'])}")
        if item["incoming_deps"]:
            deps = ", ".join(f"{d['from']}({d['kind']})" for d in item["incoming_deps"])
            lines.append(f"  - 反向依赖（谁依赖它）：{deps}")
        if item["outgoing_deps"]:
            deps = ", ".join(f"{d['path']}({d['kind']})" for d in item["outgoing_deps"])
            lines.append(f"  - 依赖出边：{deps}")
    lines += ["", "四、受影响评测与 dry-run"]
    lines.append(f"- 评测题：{', '.join(report['affected_eval_tasks']) or '无'}")
    dryrun = report["affected_dryrun"]
    lines.append(
        f"- dryrun：queries {', '.join(dryrun['queries']) or '无'}；"
        f"extended {', '.join(dryrun['extended']) or '无'}"
    )
    lines += ["", "五、建议动作"]
    for key in ("added", "removed", "modified"):
        for path in drift[key]:
            lines.append(f"- [{key}] {path}：{ADVICE[key]}")
    if fingerprints["taxonomy_changed"]:
        lines.append("- [taxonomy] 词表变更后重跑 build_index.py（校验不过不出索引）")
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=REPO)
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    args = parser.parse_args()
    try:
        report = analyze(args.root)
    except (OSError, KeyError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    print(json.dumps(report, ensure_ascii=False, indent=1) if args.json else text_report(report))
    return 2 if report["has_drift"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
