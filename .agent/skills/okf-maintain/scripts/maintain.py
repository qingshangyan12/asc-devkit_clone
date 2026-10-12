#!/usr/bin/env python3
"""okf-maintain：指定文档的三类头漂移体检与修复。

命令：
  check    : 对指定文档（或目录）体检文档头/ include 注解/ 样例 marker，产出漂移报告。
  update   : 确定性修复（okftype/version 原位刷新 + include/examples 幂等写入）；默认 dry-run。
  baseline : 建立/刷新漂移账本（记录正文哈希与证据；可读取 okf-build controller run 的标注工件）。

依赖同仓建设侧 skill okf-build（.agent/skills/okf-build）的脚本与配置，本 skill 不复制解析实现。
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Sequence

import yaml

SKILL_ROOT = Path(__file__).resolve().parents[1]
SKILLS_ROOT = SKILL_ROOT.parent
BUILD_ROOT = SKILLS_ROOT / "okf-build"
BUILD_SCRIPTS = BUILD_ROOT / "scripts"
BUILD_CONFIG = BUILD_ROOT / "config"
FEATURED_DIR = BUILD_ROOT / "featured"
REPO_ROOT = SKILLS_ROOT.parents[1]
if str(BUILD_SCRIPTS) not in sys.path:
    sys.path.insert(0, str(BUILD_SCRIPTS))

import annotate_sources  # noqa: E402
import controller  # noqa: E402
from derive_version import build_version_map, version_for  # noqa: E402
from doxygen_annotations import apply_records_to_text, validate_record_target  # noqa: E402
from example_snippet_markers import (  # noqa: E402
    apply_insertions_to_text,
    candidate_insertions_for_documents,
)
from extract_api_yaml_metadata import _example_calls  # noqa: E402
from model_registry import (  # noqa: E402
    classify_document,
    compile_execution_plan,
    document_is_in_scope,
)

SURFACE = "c_api"
SURFACE_ROOT = "docs/zh/api/SIMD-API/c_api"
STATE_PATH = SKILL_ROOT / "state" / f"{SURFACE}.json"
TAXONOMY_PATH = controller.TAXONOMY_PATH
VERSION_OVERRIDES_PATH = controller.VERSION_OVERRIDES_PATH
CONFIG_PATHS = {
    "taxonomy_sha256": TAXONOMY_PATH,
    "version_overrides_sha256": VERSION_OVERRIDES_PATH,
    "profile_registry_sha256": BUILD_CONFIG / "document-profile-registry.yaml",
    "type_registry_sha256": BUILD_CONFIG / "prototype-id" / "type-registry.yaml",
}
DOC_FIX_CLASSES = {"doc.okftype", "doc.version", "doc.version_missing"}
NON_DRIFT_SEVERITY = "INFO"


class MaintainError(ValueError):
    pass


# ---------------------------------------------------------------- utilities


def now_iso() -> str:
    return datetime.now(timezone.utc).astimezone().isoformat(timespec="seconds")


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_text(text: str) -> str:
    return sha256_bytes(text.encode("utf-8"))


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes()) if path.is_file() else ""


def read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def write_json(path: Path, value: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=1, sort_keys=True) + "\n", encoding="utf-8")


def load_document(path: Path) -> tuple[dict[str, Any] | None, str, str, str]:
    """返回 (front matter, raw, body, text)；无头/未闭合/解析失败时 front matter 为 None。"""
    text = path.read_text(encoding="utf-8")
    if not text.startswith("---\n"):
        return None, "", text, text
    end = text.find("\n---\n", 4)
    if end < 0:
        return None, "", text, text
    raw = text[4:end]
    try:
        data = yaml.safe_load(raw)
    except yaml.YAMLError:
        return None, raw, text[end + 5:], text
    if not isinstance(data, dict):
        return None, raw, text[end + 5:], text
    return data, raw, text[end + 5:], text


def tag_mapping(data: dict[str, Any]) -> dict[str, list[str]]:
    tag = data.get("tag")
    if isinstance(tag, dict):
        return {namespace: list(values) if isinstance(values, list) else [values] for namespace, values in tag.items()}
    if isinstance(tag, list):
        merged: dict[str, list[str]] = {}
        for item in tag:
            if not isinstance(item, dict):
                continue
            for namespace, values in item.items():
                merged[namespace] = list(values) if isinstance(values, list) else [values]
        return merged
    return {}


def header_values(data: dict[str, Any]) -> dict[str, Any]:
    return {
        "okftype": data.get("okftype"),
        "description": data.get("description"),
        "tag": tag_mapping(data),
        "dependencies": [
            {"path": dep.get("path"), "kind": dep.get("kind")}
            for dep in data.get("dependencies") or []
            if isinstance(dep, dict)
        ],
        "version": data.get("version"),
    }


def finding(kind: str, severity: str, detail: str, **extra: Any) -> dict[str, Any]:
    record = {"class": kind, "severity": severity, "detail": detail}
    record.update(extra)
    return record


def is_missing_required(data: dict[str, Any], plan_required: Sequence[str]) -> list[str]:
    missing = []
    if "description" in plan_required:
        value = data.get("description")
        if not isinstance(value, str) or not value.strip():
            missing.append("description")
    if "tag" in plan_required:
        if not tag_mapping(data):
            missing.append("tag")
    if "dependencies" in plan_required:
        if data.get("dependencies") is None:
            missing.append("dependencies")
    if "version" in plan_required:
        value = data.get("version")
        if not isinstance(value, str) or not value.strip():
            missing.append("version")
    return missing


# ---------------------------------------------------------------- ledger


def empty_ledger() -> dict[str, Any]:
    return {
        "schema_version": 1,
        "surface": SURFACE,
        "surface_root": SURFACE_ROOT,
        "configs": {},
        "documents": {},
    }


def load_ledger() -> dict[str, Any]:
    if not STATE_PATH.is_file():
        return empty_ledger()
    payload = read_json(STATE_PATH)
    if not isinstance(payload, dict) or payload.get("surface") != SURFACE:
        raise MaintainError(f"invalid ledger: {STATE_PATH}")
    payload.setdefault("configs", {})
    payload.setdefault("documents", {})
    return payload


def current_configs() -> dict[str, str]:
    return {key: sha256_file(path) for key, path in CONFIG_PATHS.items()}


def configs_status(ledger: dict[str, Any]) -> dict[str, Any]:
    recorded = ledger.get("configs") or {}
    current = current_configs()
    unrecorded = sorted(key for key in current if not recorded.get(key))
    changed = sorted(key for key, value in current.items() if recorded.get(key) and recorded[key] != value)
    if not recorded:
        status = "absent"
    elif changed or unrecorded:
        status = "changed"
    else:
        status = "match"
    return {"status": status, "changed": changed, "unrecorded": unrecorded}


def annotation_evidence(annotation: dict[str, Any]) -> dict[str, Any]:
    evidence: dict[str, Any] = {}
    description = annotation.get("description")
    if isinstance(description, dict):
        excerpts = [
            item.get("excerpt") for item in description.get("evidence") or []
            if isinstance(item, dict) and isinstance(item.get("excerpt"), str)
        ]
        if excerpts:
            evidence["description"] = excerpts
    tag_evidence = annotation.get("tag_evidence")
    if isinstance(tag_evidence, dict) and tag_evidence:
        evidence["tag"] = {key: value for key, value in tag_evidence.items() if isinstance(value, str)}
    dependencies: dict[str, str] = {}
    for dep in annotation.get("dependencies") or []:
        if not isinstance(dep, dict):
            continue
        record = dep.get("evidence")
        excerpt = record.get("excerpt") if isinstance(record, dict) else None
        if isinstance(excerpt, str):
            dependencies[f"{dep.get('path')}|{dep.get('kind')}"] = excerpt
    if dependencies:
        evidence["dependencies"] = dependencies
    return evidence


def ledger_entry(root: Path, rel: str, data: dict[str, Any], body: str, evidence: dict[str, Any] | None = None) -> dict[str, Any]:
    entry = {
        "body_sha256": sha256_text(body),
        "file_sha256": sha256_file(root / rel),
        "header": header_values(data),
        "evidence": evidence if evidence is not None else {},
        "recorded_at": now_iso(),
    }
    return entry


def load_run_annotations(run_dir: Path) -> dict[str, dict[str, Any]]:
    manifest_path = run_dir / "manifest.json"
    if not manifest_path.is_file():
        raise MaintainError(f"run directory has no manifest.json: {run_dir}")
    annotations: dict[str, dict[str, Any]] = {}
    directory = run_dir / "annotations"
    if directory.is_dir():
        for path in sorted(directory.glob("*.json")):
            payload = read_json(path)
            if payload.get("purpose") != controller.SEMANTIC_PURPOSE:
                raise MaintainError(f"invalid annotation artifact: {path}")
            for annotation in payload.get("annotations", []):
                rel = annotation.get("document_path")
                if isinstance(rel, str):
                    annotations[rel] = annotation
    return annotations


def evidence_excerpts(evidence: dict[str, Any]) -> list[tuple[str, str]]:
    out: list[tuple[str, str]] = []
    for excerpt in evidence.get("description") or []:
        if isinstance(excerpt, str):
            out.append(("description", excerpt))
    for key, excerpt in (evidence.get("tag") or {}).items():
        if isinstance(excerpt, str):
            out.append((f"tag:{key}", excerpt))
    for key, excerpt in (evidence.get("dependencies") or {}).items():
        if isinstance(excerpt, str):
            out.append((f"dependencies:{key}", excerpt))
    return out


# ---------------------------------------------------------------- document header check


def document_findings(root: Path, rel: str, ledger: dict[str, Any], version_map: dict[str, Any]) -> list[dict[str, Any]]:
    path = root / rel
    if not path.is_file():
        return [finding("doc.missing_file", "SEM-HIGH", "目标文档不存在", document=rel)]
    data, raw, body, _ = load_document(path)
    if data is None:
        return [finding(
            "doc.missing_header", "SEM-HIGH", "无 front matter 或无法解析",
            document=rel, action="走 okf-build 标注管线（prepare→标注→validate→write）",
        )]

    classification = classify_document(rel, body)
    if classification is None:
        return [finding("doc.unclassifiable", "SEM-HIGH", "文档结构无法分类", document=rel,
                        action="检查正文结构或登记 document-issues")]
    plan = compile_execution_plan(classification)
    findings: list[dict[str, Any]] = []

    current_fields = list(data.keys())
    expected_fields = [field for field in controller.ALLOWED_FIELDS if field in set(plan.fields)]
    missing = is_missing_required(data, plan.required)
    if current_fields != expected_fields:
        severity = "SEM-HIGH" if missing else "SEM-LOW"
        findings.append(finding(
            "doc.field_schema", severity,
            f"字段/顺序 {current_fields} != 期望 {expected_fields}",
            document=rel, current=current_fields, expected=expected_fields,
            action="按字段契约重渲染" if not missing else "补齐必填语义字段（需标注）",
        ))
    elif missing:
        findings.append(finding(
            "doc.semantic_fields_missing", "SEM-HIGH",
            f"缺少必填字段 {missing}", document=rel,
            action="走 okf-build 标注管线补写",
        ))

    current_okftype = data.get("okftype")
    if current_okftype != plan.okftype:
        fixable = set(current_fields) == set(expected_fields)
        findings.append(finding(
            "doc.okftype", "DET" if fixable else "SEM-HIGH",
            f"okftype {current_okftype!r} != 重分类 {plan.okftype!r}",
            document=rel, current=current_okftype, expected=plan.okftype,
            action="maintain update --write" if fixable else "结构变化影响字段集，走标注管线",
        ))

    if "version" in plan.required:
        expected_version = version_for(rel, version_map)["version"]
        current_version = data.get("version")
        if not isinstance(current_version, str) or not current_version:
            findings.append(finding(
                "doc.version_missing", "DET", f"缺少 version（应为 {expected_version}）",
                document=rel, expected=expected_version, action="maintain update --write",
            ))
        elif current_version != expected_version:
            findings.append(finding(
                "doc.version", "DET", f"version {current_version} != 重推导 {expected_version}",
                document=rel, current=current_version, expected=expected_version,
                action="maintain update --write",
            ))

    tag = tag_mapping(data)
    if "tag" in plan.fields:
        unknown_namespaces = sorted(set(tag) - set(controller.TAXONOMY_NAMESPACES))
        if unknown_namespaces:
            findings.append(finding(
                "doc.tag_namespace", "SEM-HIGH", f"未知 namespace {unknown_namespaces}",
                document=rel, action="按冻结词表重标",
            ))
        for namespace, values in tag.items():
            unknown = sorted(set(values) - controller.TAXONOMY_VALUES.get(namespace, set()))
            if unknown:
                findings.append(finding(
                    "doc.tag_out_of_taxonomy", "SEM-HIGH",
                    f"{namespace} 含词表外值 {unknown}", document=rel,
                    action="按冻结词表重标（词表换版时）",
                ))
        roles = set(tag.get("roles", []))
        if "roles" not in tag and "tag" in plan.required:
            findings.append(finding(
                "doc.tag_missing_roles", "SEM-HIGH", "实体页缺 roles", document=rel,
                action="走标注管线",
            ))
        if roles & controller.CONDITIONAL_OPERATIONS_ROLES and "operations" not in tag:
            findings.append(finding(
                "doc.tag_missing_operations", "SEM-HIGH",
                f"roles {sorted(roles)} 要求 operations", document=rel,
                action="走标注管线",
            ))

    dependencies = data.get("dependencies")
    if "dependencies" in plan.required and dependencies is None:
        findings.append(finding(
            "doc.dependencies_missing", "SEM-HIGH", "缺 dependencies（可为空数组）",
            document=rel, action="走标注管线",
        ))
    elif isinstance(dependencies, list):
        seen: set[tuple[Any, Any]] = set()
        for dep in dependencies:
            if not isinstance(dep, dict):
                findings.append(finding("doc.dependency_invalid", "SEM-LOW", "依赖条目不是对象", document=rel))
                continue
            dep_path, kind = dep.get("path"), dep.get("kind")
            if not isinstance(dep_path, str) or not dep_path.endswith(".md") or ".." in Path(dep_path).parts:
                findings.append(finding("doc.dependency_invalid", "SEM-LOW",
                                        f"依赖路径非法: {dep_path!r}", document=rel))
                continue
            if dep_path == rel:
                findings.append(finding("doc.dependency_invalid", "SEM-LOW", "依赖指向自身", document=rel))
                continue
            if kind not in controller.DEPENDENCY_KINDS:
                findings.append(finding("doc.dependency_kind", "SEM-LOW",
                                        f"依赖 kind 非法: {kind!r}", document=rel))
                continue
            if not (root / dep_path).is_file():
                findings.append(finding(
                    "doc.dependency_target_missing", "SEM-HIGH",
                    f"依赖目标不存在: {dep_path}", document=rel,
                    action="删除/替换该依赖条目（按真实语义）",
                ))
            key = (dep_path, kind)
            if key in seen:
                findings.append(finding("doc.dependency_duplicate", "SEM-LOW",
                                        f"重复依赖: {dep_path} ({kind})", document=rel))
            seen.add(key)

    entry = (ledger.get("documents") or {}).get(rel)
    if entry is None:
        findings.append(finding(
            "doc.unbaselined", "INFO", "账本中没有该文档（无法判定正文变化）",
            document=rel, action="maintain baseline",
        ))
        return findings

    recorded_header = entry.get("header")
    header_diverged = recorded_header is not None and recorded_header != header_values(data)
    if entry.get("body_sha256") != sha256_text(body):
        excerpts = evidence_excerpts(entry.get("evidence") or {})
        if not excerpts:
            findings.append(finding(
                "doc.body_changed_unverified", "SEM-LOW",
                "正文相对账本发生变化，且无证据可复检", document=rel,
                action="人工复核关键字段；需要时走标注管线重标并 baseline 刷新",
            ))
        else:
            lost = [key for key, excerpt in excerpts if excerpt not in body]
            if lost:
                findings.append(finding(
                    "doc.body_changed_evidence_lost", "SEM-HIGH",
                    f"正文变化导致证据失效: {lost}", document=rel,
                    action="按真实语义重写受影响字段（description/tag/dependencies）",
                ))
            else:
                findings.append(finding(
                    "doc.body_changed_evidence_ok", "SEM-LOW",
                    "正文有变化但全部证据仍逐字存活，建议复核后 baseline 刷新",
                    document=rel,
                ))
    if header_diverged and not any(item["severity"] != NON_DRIFT_SEVERITY for item in findings):
        findings.append(finding(
            "doc.header_diverged", "SEM-LOW", "头与账本记录不一致（可能手工修改或未记录）",
            document=rel, action="复核后 baseline 刷新",
        ))
    return findings


# ---------------------------------------------------------------- include check


def plan_include(
    root: Path, documents: Sequence[str],
) -> tuple[list[dict[str, Any]], list[tuple[Path, str]], dict[str, tuple[Any, ...]], dict[str, list[dict[str, Any]]]]:
    dummy_run = SKILL_ROOT / "tmp" / ".no-cache"
    grouped, issues, records_by_header = annotate_sources.collect_signatures(
        root, [SURFACE_ROOT], dummy_run, [], list(documents), refresh=True,
    )
    findings: list[dict[str, Any]] = []
    actions: list[tuple[Path, str]] = []
    for header, records in sorted(records_by_header.items()):
        path = root / header
        docs = sorted({record.get("source_document", "") for record in records})
        if not path.is_file():
            findings.append(finding(
                "include.header_missing", "DET", f"头文件不存在: {header}",
                header=header, documents=docs, action="核对头文件路径",
            ))
            continue
        text = path.read_text(encoding="utf-8")
        valid = []
        for record in records:
            try:
                validate_record_target(text, record)
            except Exception as exc:  # noqa: BLE001
                findings.append(finding(
                    "include.unresolved", "INFO",
                    f"{record.get('override_id') or record.get('macro_id')}: {exc}",
                    header=header, documents=[record.get("source_document", "")],
                ))
                continue
            valid.append(record)
        try:
            new_text, applied = apply_records_to_text(text, valid)
        except Exception as exc:  # noqa: BLE001
            findings.append(finding("include.apply_failed", "INFO", f"{header}: {exc}", header=header))
            continue
        if new_text != text:
            findings.append(finding(
                "include.stale", "DET",
                f"{applied} 条 include 注解待更新/补齐",
                header=header, documents=docs, action="maintain update --write",
            ))
            actions.append((path, new_text))
    for issue in issues:
        findings.append(finding(
            "include.issue", "INFO", str(issue.get("issue", "")),
            document=str(issue.get("document", "")),
            action="核对文档原型/参数表；必要时登记 document-issues",
        ))
    return findings, actions, grouped, records_by_header


# ---------------------------------------------------------------- examples check


def load_featured_entries() -> list[dict[str, Any]]:
    entries: list[dict[str, Any]] = []
    for path in sorted(FEATURED_DIR.glob("*.json")):
        payload = read_json(path)
        records = payload if isinstance(payload, list) else payload.get("examples", [])
        if isinstance(records, list):
            entries.extend(item for item in records if isinstance(item, dict))
    return entries


def plan_examples(
    root: Path,
    grouped: dict[str, tuple[Any, ...]],
    records_by_header: dict[str, list[dict[str, Any]]],
) -> tuple[list[dict[str, Any]], list[tuple[Path, str]]]:
    findings: list[dict[str, Any]] = []
    actions: list[tuple[Path, str]] = []
    symbol_docs: dict[str, list[str]] = {}
    for records in records_by_header.values():
        for record in records:
            symbol = record.get("function") or ""
            document = record.get("source_document") or ""
            symbol_docs.setdefault(symbol, [])
            if document and document not in symbol_docs[symbol]:
                symbol_docs[symbol].append(document)

    for entry in sorted(load_featured_entries(), key=lambda item: str(item.get("example", ""))):
        example = str(entry.get("example", ""))
        apis = [api for api in entry.get("apis", []) if isinstance(api, str) and api]
        scoped = [api for api in apis if api in grouped]
        if not scoped:
            continue
        project = root / example
        related = sorted({doc for api in scoped for doc in symbol_docs.get(api, [])})
        if not project.is_dir():
            findings.append(finding(
                "examples.example_missing", "SEM-LOW", f"样例目录不存在: {example}",
                example=example, documents=related,
            ))
            continue
        symbols = {api: grouped[api] for api in scoped}
        calls, call_issues = _example_calls(project, root, symbols)
        for issue in call_issues:
            findings.append(finding(
                "examples.issue", "INFO", str(getattr(issue, "detail", issue)),
                example=example, documents=related,
            ))
        confirmed = [
            call for call in calls
            if call.overload_status == "exact"
            and isinstance(call.canonical_id, str)
            and isinstance(call.file, str)
            and call.is_statement
            and call.inferred_types is not None
            and all(isinstance(value, str) and value for value in call.inferred_types)
        ]
        if not confirmed:
            continue
        record = {
            "path": example,
            "confidence": "authoritative",
            "issues": (),
            "calls": [asdict(call) for call in confirmed],
        }
        insertions, skips = candidate_insertions_for_documents(
            ((f"featured:{example}", [record]),), root,
        )
        for skip in skips:
            if skip.reason == "already aligned":
                continue
            findings.append(finding(
                "examples.skip", "INFO", f"{skip.reason} (marker {skip.marker_id})",
                example=example, documents=related,
                file=str(skip.file_path.relative_to(root)) if skip.file_path else "",
            ))
        by_file: dict[Path, list[Any]] = {}
        for insertion in insertions:
            by_file.setdefault(insertion.file_path, []).append(insertion)
        for file_path, file_insertions in sorted(by_file.items()):
            text = file_path.read_text(encoding="utf-8", errors="replace")
            new_text = apply_insertions_to_text(text, file_insertions)
            if new_text != text:
                rel_file = file_path.relative_to(root).as_posix()
                findings.append(finding(
                    "examples.stale", "DET", f"{len(file_insertions)} 处 marker 待更新/补齐",
                    example=example, file=rel_file, documents=related,
                    action="maintain update --write",
                ))
                actions.append((file_path, new_text))
    return findings, actions


# ---------------------------------------------------------------- check / update


def resolve_targets(root: Path, documents: Sequence[str], source_dirs: Sequence[str]) -> list[str]:
    if documents and source_dirs:
        raise MaintainError("--document and --source-dir cannot be combined")
    if documents:
        resolved = []
        for item in documents:
            rel = Path(item).as_posix().lstrip("./")
            if not document_is_in_scope(rel):
                raise MaintainError(f"Outside registered document roots: {item!r}")
            if not (root / rel).is_file():
                raise MaintainError(f"Document does not exist: {item!r}")
            resolved.append(rel)
        return sorted(set(resolved))
    bases = list(source_dirs) or [SURFACE_ROOT]
    targets: set[str] = set()
    for base in bases:
        directory = root / Path(base)
        if not directory.is_dir():
            raise MaintainError(f"Source directory does not exist: {base!r}")
        targets.update(path.relative_to(root).as_posix() for path in sorted(directory.rglob("*.md")))
    return sorted(targets)


def parse_checks(value: str) -> tuple[str, ...]:
    checks = tuple(dict.fromkeys(item.strip() for item in value.split(",") if item.strip()))
    unknown = sorted(set(checks) - {"doc", "include", "examples"})
    if unknown or not checks:
        raise MaintainError(f"invalid --checks value: {value!r}")
    return checks


def report_payload(
    root: Path, documents: Sequence[str], findings: Sequence[dict[str, Any]], checks: Sequence[str],
) -> dict[str, Any]:
    ledger = load_ledger()
    summary = {severity: 0 for severity in ("DET", "SEM-HIGH", "SEM-LOW", "INFO")}
    for item in findings:
        summary[item["severity"]] = summary.get(item["severity"], 0) + 1
    return {
        "schema_version": 1,
        "purpose": "okf_maintain_check",
        "generated_at": now_iso(),
        "devkit_root": str(root),
        "surface": SURFACE,
        "surface_root": SURFACE_ROOT,
        "target_count": len(documents),
        "checks": list(checks),
        "configs": configs_status(ledger),
        "ledger": {
            "path": str(STATE_PATH),
            "present": STATE_PATH.is_file(),
            "documents": len(ledger.get("documents") or {}),
        },
        "summary": summary,
        "clean": all(item["severity"] == NON_DRIFT_SEVERITY for item in findings),
        "findings": list(findings),
    }


def render_markdown(report: dict[str, Any]) -> str:
    lines = [
        "# 漂移体检报告",
        "",
        f"- 生成时间：{report['generated_at']}",
        f"- 范围：{report['surface']}（{report['target_count']} 篇）",
        f"- 检查项：{','.join(report['checks'])}",
        f"- 配置指纹：{report['configs']['status']}",
        f"- 账本：{report['ledger']['path']}（{'存在' if report['ledger']['present'] else '缺失'}，{report['ledger']['documents']} 篇）",
        f"- 结论：{'干净' if report['clean'] else '发现漂移'}",
        "",
        "## 汇总",
        "",
        "| 级别 | 数量 | 含义 |",
        "| --- | --- | --- |",
        f"| DET | {report['summary']['DET']} | 确定性漂移，可 update --write 修复 |",
        f"| SEM-HIGH | {report['summary']['SEM-HIGH']} | 语义漂移/数据缺陷，需按真实语义修复 |",
        f"| SEM-LOW | {report['summary']['SEM-LOW']} | 建议复核 |",
        f"| INFO | {report['summary']['INFO']} | 提示（如未入账本） |",
    ]
    if report["configs"]["changed"] or report["configs"]["unrecorded"]:
        lines += [
            "",
            f"- 配置变化：changed={report['configs']['changed']} unrecorded={report['configs']['unrecorded']}",
        ]
    for severity in ("DET", "SEM-HIGH", "SEM-LOW", "INFO"):
        subset = [item for item in report["findings"] if item["severity"] == severity]
        if not subset:
            continue
        lines += ["", f"## {severity}", "", "| 类别 | 对象 | 说明 | 建议 |", "| --- | --- | --- | --- |"]
        for item in subset:
            target = item.get("document") or item.get("header") or item.get("file") or item.get("example") or ""
            lines.append(
                f"| {item['class']} | `{target}` | {item['detail']} | {item.get('action', '')} |"
            )
    lines.append("")
    return "\n".join(lines)


def write_report(report: dict[str, Any], report_dir: Path) -> Path:
    write_json(report_dir / "drift-report.json", report)
    (report_dir / "drift-report.md").write_text(render_markdown(report), encoding="utf-8")
    return report_dir


def default_report_dir() -> Path:
    stamp = datetime.now().strftime("%Y%m%d-%H%M%S")
    return SKILL_ROOT / "tmp" / "maintain" / stamp


def perform_check(
    root: Path, documents: Sequence[str], checks: Sequence[str],
) -> tuple[list[dict[str, Any]], list[tuple[Path, str]], list[tuple[Path, str]]]:
    findings: list[dict[str, Any]] = []
    include_actions: list[tuple[Path, str]] = []
    examples_actions: list[tuple[Path, str]] = []
    if "doc" in checks:
        ledger = load_ledger()
        override = VERSION_OVERRIDES_PATH if VERSION_OVERRIDES_PATH.is_file() else None
        version_map = build_version_map(root, override)
        for rel in documents:
            findings.extend(document_findings(root, rel, ledger, version_map))
    if "include" in checks or "examples" in checks:
        include_findings, include_actions, grouped, records_by_header = plan_include(root, documents)
        if "include" in checks:
            findings.extend(include_findings)
        if "examples" in checks:
            examples_findings, examples_actions = plan_examples(root, grouped, records_by_header)
            findings.extend(examples_findings)
    return findings, include_actions, examples_actions


def command_check(args: argparse.Namespace) -> int:
    root = args.devkit_root.resolve()
    documents = resolve_targets(root, args.document, args.source_dir)
    checks = parse_checks(args.checks)
    findings, _, _ = perform_check(root, documents, checks)
    report = report_payload(root, documents, findings, checks)
    report_dir = (args.report_dir or default_report_dir()).resolve()
    write_report(report, report_dir)
    if args.json:
        print(json.dumps(report, ensure_ascii=False, indent=1))
    else:
        summary = report["summary"]
        print(
            f"[maintain] check: docs={len(documents)} "
            f"DET={summary['DET']} SEM-HIGH={summary['SEM-HIGH']} SEM-LOW={summary['SEM-LOW']} "
            f"INFO={summary['INFO']} clean={report['clean']} report={report_dir}"
        )
    return 0 if report["clean"] else 1


def refresh_front_matter(raw: str, *, okftype: str | None, version: str | None) -> str:
    lines = raw.splitlines()
    output: list[str] = []
    replaced = {"okftype": False, "version": False}
    for line in lines:
        match = re.match(r"^([A-Za-z_][A-Za-z0-9_]*):", line)
        if match and match.group(1) == "okftype" and okftype is not None:
            output.append(f"okftype: {okftype}")
            replaced["okftype"] = True
            continue
        if match and match.group(1) == "version" and version is not None:
            output.append(f'version: "{version}"')
            replaced["version"] = True
            continue
        output.append(line)
    if okftype is not None and not replaced["okftype"]:
        output.insert(0, f"okftype: {okftype}")
    if version is not None and not replaced["version"]:
        output.append(f'version: "{version}"')
    return "\n".join(output)


def apply_document_fixes(root: Path, findings: Sequence[dict[str, Any]], write: bool) -> list[str]:
    fixes: dict[str, dict[str, str]] = {}
    for item in findings:
        if item["severity"] != "DET" or item["class"] not in DOC_FIX_CLASSES:
            continue
        rel = item.get("document")
        expected = item.get("expected")
        if not rel or not expected:
            continue
        key = "okftype" if item["class"] == "doc.okftype" else "version"
        fixes.setdefault(rel, {})[key] = expected
    written: list[str] = []
    ledger = load_ledger()
    for rel, fix in sorted(fixes.items()):
        path = root / rel
        data, raw, body, text = load_document(path)
        if data is None:
            continue
        new_raw = refresh_front_matter(raw, okftype=fix.get("okftype"), version=fix.get("version"))
        new_text = f"---\n{new_raw}\n---\n{body}"
        if new_text == text:
            continue
        if write:
            path.write_text(new_text, encoding="utf-8")
            entry = (ledger.get("documents") or {}).get(rel)
            if entry is not None:
                parsed, _, parsed_body, _ = load_document(path)
                if parsed is None or sha256_text(parsed_body) != sha256_text(body):
                    raise MaintainError(f"post-write verification failed: {rel}")
                entry.update({
                    "file_sha256": sha256_file(path),
                    "body_sha256": sha256_text(body),
                    "header": header_values(parsed),
                    "recorded_at": now_iso(),
                })
        written.append(rel)
    if write and written:
        write_json(STATE_PATH, ledger)
    return written


def command_update(args: argparse.Namespace) -> int:
    root = args.devkit_root.resolve()
    documents = resolve_targets(root, args.document, args.source_dir)
    checks = parse_checks(args.checks)
    findings, include_actions, examples_actions = perform_check(root, documents, checks)
    written_documents = apply_document_fixes(root, findings, args.write)
    written_headers: list[str] = []
    written_files: list[str] = []
    if args.write:
        for path, new_text in include_actions:
            path.write_text(new_text, encoding="utf-8")
            written_headers.append(path.relative_to(root).as_posix())
        for path, new_text in examples_actions:
            path.write_text(new_text, encoding="utf-8")
            written_files.append(path.relative_to(root).as_posix())
    final_findings, _, _ = perform_check(root, documents, checks)
    report = report_payload(root, documents, final_findings, checks)
    report_dir = (args.report_dir or default_report_dir()).resolve()
    write_report(report, report_dir)
    result = {
        "schema_version": 1,
        "purpose": "okf_maintain_update",
        "write": bool(args.write),
        "written_documents": written_documents,
        "written_headers": written_headers,
        "written_files": written_files,
        "remaining": report["summary"],
        "clean": report["clean"],
    }
    write_json(report_dir / "update-result.json", result)
    if args.json:
        print(json.dumps(result, ensure_ascii=False, indent=1))
    else:
        print(
            f"[maintain] update: write={bool(args.write)} docs={len(written_documents)} "
            f"headers={len(written_headers)} files={len(written_files)} "
            f"remaining DET={report['summary']['DET']} SEM-HIGH={report['summary']['SEM-HIGH']} "
            f"SEM-LOW={report['summary']['SEM-LOW']} report={report_dir}"
        )
    return 0 if report["clean"] else 1


# ---------------------------------------------------------------- baseline


def command_baseline(args: argparse.Namespace) -> int:
    root = args.devkit_root.resolve()
    ledger = load_ledger()
    annotations = load_run_annotations(args.from_run.resolve()) if args.from_run else {}
    if args.document or args.source_dir:
        documents = resolve_targets(root, args.document, args.source_dir)
    elif annotations:
        documents = sorted(annotations)
    else:
        documents = resolve_targets(root, [], [])
    updated = 0
    for rel in documents:
        path = root / rel
        if not path.is_file():
            continue
        data, _, body, _ = load_document(path)
        if data is None:
            continue
        evidence = None
        if rel in annotations:
            evidence = annotation_evidence(annotations[rel])
        entry = ledger_entry(root, rel, data, body, evidence)
        if evidence is None and rel in (ledger.get("documents") or {}):
            entry["evidence"] = ledger["documents"][rel].get("evidence") or {}
        ledger.setdefault("documents", {})[rel] = entry
        updated += 1
    ledger["configs"] = current_configs()
    write_json(STATE_PATH, ledger)
    print(json.dumps({
        "ledger": str(STATE_PATH),
        "documents": updated,
        "total": len(ledger.get("documents") or {}),
        "from_run": str(args.from_run) if args.from_run else "",
    }, ensure_ascii=False))
    return 0


# ---------------------------------------------------------------- CLI


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description=__doc__)
    sub = p.add_subparsers(dest="command", required=True)

    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--devkit-root", type=Path, default=REPO_ROOT)
    common.add_argument("--document", action="append", default=[], help="指定文档（仓内相对路径，可多次）")
    common.add_argument("--source-dir", action="append", default=[], help="指定目录（默认全量 c_api）")
    common.add_argument("--report-dir", type=Path, default=None, help="报告目录（默认 tmp/maintain/<ts>）")
    common.add_argument("--checks", default="doc,include,examples",
                        help="检查项：doc/include/examples 逗号组合（默认全部）")
    common.add_argument("--json", action="store_true", help="打印机器可读 JSON")

    check = sub.add_parser("check", parents=[common], help="三类头漂移体检")
    check.set_defaults(func=command_check)

    update = sub.add_parser("update", parents=[common], help="确定性修复（默认 dry-run）")
    update.add_argument("--write", action="store_true", help="落盘；默认只看不写")
    update.set_defaults(func=command_update)

    baseline = sub.add_parser("baseline", parents=[common], help="建立/刷新漂移账本")
    baseline.add_argument("--from-run", type=Path, default=None, help="读取 okf-build controller run 的 annotations 存证据")
    baseline.set_defaults(func=command_baseline)

    return p


def main(argv: Sequence[str] | None = None) -> int:
    args = parser().parse_args(argv)
    try:
        return args.func(args)
    except (MaintainError, controller.ValidationError, OSError, KeyError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
