#!/usr/bin/env python3
"""Self-contained OKF YAML orchestration, validation, reporting, and writing."""

from __future__ import annotations

import argparse
from copy import deepcopy
from datetime import date
from contextlib import contextmanager
import fcntl
import hashlib
import json
from pathlib import Path
import re
import sys
from typing import Any

import yaml


API_ROOT = Path("docs/zh/api/SIMD-API")
API_SURFACES = {"c_api", "basic_api", "adv_api", "tensor_api"}
ALLOWED_FIELDS = (
    "okftype", "title", "resource", "description", "tag", "signature",
    "dependencies", "examples", "timestamp",
)
DEPENDENCY_KINDS = {"requires", "configures", "constrained_by", "related"}
SIGNATURE_KEYS = {"prototype_id", "override_id", "archs", "headers", "dtype"}
ISSUE_TYPES = {"document_defect", "source_defect", "tool_defect", "unresolved"}
RESOLUTIONS = {
    "blank_field", "report_source_defect", "report_unresolved", "tool_change_required",
}
TOP_KEY_RE = re.compile(r"^(?P<key>[A-Za-z_][A-Za-z0-9_-]*):(?:\s.*)?(?:\n|$)")
COMMENT_RE = re.compile(r"^\s*(?://|/\*|\*)")


class InlineList(list):
    """A YAML sequence rendered in flow style for compact OKF display fields."""


class DisplayDumper(yaml.SafeDumper):
    def increase_indent(self, flow: bool = False, indentless: bool = False) -> None:
        return super().increase_indent(flow, False)


def represent_inline_list(dumper: yaml.SafeDumper, value: InlineList) -> yaml.nodes.SequenceNode:
    return dumper.represent_sequence("tag:yaml.org,2002:seq", value, flow_style=True)


DisplayDumper.add_representer(InlineList, represent_inline_list)


class ValidationError(ValueError):
    pass


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def split_front_matter(text: str) -> tuple[dict[str, Any], str, str]:
    if not text.startswith("---\n"):
        return {}, "", text
    end = text.find("\n---\n", 4)
    if end < 0:
        raise ValidationError("front matter is not terminated")
    raw = text[4:end]
    data = yaml.safe_load(raw) or {}
    if not isinstance(data, dict):
        raise ValidationError("front matter must be a mapping")
    return data, raw, text[end + len("\n---\n") :]


def relative_api_path(root: Path, value: str) -> Path:
    candidate = Path(value)
    if candidate.is_absolute() or ".." in candidate.parts or candidate.suffix.lower() != ".md":
        raise ValidationError(f"invalid repository-relative Markdown path: {value!r}")
    if len(candidate.parts) < 5 or candidate.parts[:4] != API_ROOT.parts:
        raise ValidationError(f"path is outside supported API families: {value!r}")
    if candidate.parts[4] not in API_SURFACES:
        raise ValidationError(f"unsupported API surface: {value!r}")
    resolved = (root / candidate).resolve()
    try:
        resolved.relative_to(root.resolve())
    except ValueError as exc:
        raise ValidationError(f"path escapes DevKit root: {value!r}") from exc
    if not resolved.is_file():
        raise ValidationError(f"API document does not exist: {value!r}")
    return resolved


def repo_relative(root: Path, path: Path) -> str:
    return path.resolve().relative_to(root.resolve()).as_posix()


def api_documents(root: Path, source_dir: str | None, document: list[str] | None) -> list[str]:
    if document and source_dir:
        raise ValidationError("--document and --source-dir cannot be combined")
    if document:
        selected = [repo_relative(root, relative_api_path(root, item)) for item in document]
    else:
        base = Path(source_dir or str(API_ROOT / "c_api"))
        if base.is_absolute():
            base = Path(repo_relative(root, base))
        if base.parts[:4] != API_ROOT.parts or len(base.parts) < 5 or base.parts[4] not in API_SURFACES:
            raise ValidationError("--source-dir is outside supported API families")
        directory = (root / base).resolve()
        if not directory.is_dir():
            raise ValidationError(f"source directory does not exist: {base}")
        selected = [repo_relative(root, path) for path in directory.rglob("*.md")]
    return sorted(set(selected))


def top_level_key(line: str) -> str | None:
    if not line or line[0].isspace():
        return None
    match = TOP_KEY_RE.match(line)
    return match.group("key") if match else None


def normalize_tag(value: Any, namespaces: tuple[str, ...]) -> list[dict[str, list[str]]]:
    if isinstance(value, list):
        result: dict[str, list[str]] = {}
        for item in value:
            if isinstance(item, dict) and len(item) == 1:
                key, values = next(iter(item.items()))
                if isinstance(values, list):
                    result[key] = [str(item) for item in values]
    elif isinstance(value, dict):
        result = {key: list(values) for key, values in value.items() if isinstance(values, list)}
    else:
        result = {}
    return [{namespace: sorted(set(result.get(namespace, [])))} for namespace in namespaces]


def taxonomy(path: Path) -> tuple[tuple[str, ...], dict[str, set[str]]]:
    payload = json.loads(path.read_text(encoding="utf-8"))
    if payload.get("status") != "frozen":
        raise ValidationError("taxonomy is not frozen")
    namespaces = payload.get("required_namespaces")
    defs = payload.get("namespaces")
    if not isinstance(namespaces, list) or not isinstance(defs, dict):
        raise ValidationError("taxonomy metadata is invalid")
    values = {}
    for namespace in namespaces:
        definition = defs.get(namespace, {})
        options = definition.get("values") if isinstance(definition, dict) else None
        if not isinstance(options, dict):
            raise ValidationError(f"taxonomy values missing: {namespace}")
        values[namespace] = set(options)
    return tuple(namespaces), values


def first_heading(body: str, fallback: str) -> str:
    match = re.search(r"(?m)^#\s+(.+?)\s*$", body)
    return match.group(1).strip() if match else fallback


def description_from_body(body: str) -> str:
    section = re.search(r"(?ms)^##\s*(?:功能说明|功能描述|说明)\s*$\n(.*?)(?=^##\s|\Z)", body)
    text = section.group(1) if section else body
    for line in text.splitlines():
        value = line.strip()
        if value and not value.startswith(("#", "```", "<!--", "|", "- ")):
            return value
    return ""


def issue(path: str, field: str, issue_type: str, message: str, evidence: list[dict[str, str]] | None = None) -> dict[str, Any]:
    return {
        "issue_id": f"{Path(path).stem}-{field}-{hashlib.sha1(message.encode()).hexdigest()[:8]}",
        "document_path": path,
        "field": field,
        "issue_type": issue_type,
        "severity": "warning" if issue_type in {"source_defect", "unresolved"} else "error",
        "message": message,
        "evidence": evidence or [],
    }


def build_worker_document(root: Path, path: str, namespaces: tuple[str, ...]) -> dict[str, Any]:
    document = relative_api_path(root, path)
    original = document.read_text(encoding="utf-8", errors="replace")
    data, _, body = split_front_matter(original)
    title = data.get("title") or first_heading(body, document.stem)
    description = data.get("description")
    if not isinstance(description, str):
        description = description_from_body(body)
    tags = normalize_tag(data.get("tag"), namespaces)
    fields: dict[str, Any] = {
        "okftype": data.get("okftype") or "reference.api.cpp",
        "title": title,
        "resource": data.get("resource") or f"asc-devkit/{path}",
        "description": description,
        "tag": tags,
        "signature": data.get("signature") if isinstance(data.get("signature"), list) else [],
        "dependencies": data.get("dependencies") if isinstance(data.get("dependencies"), list) else [],
        "examples": data.get("examples") if isinstance(data.get("examples"), list) else [],
        "timestamp": data.get("timestamp") or date.today().isoformat(),
    }
    issues: list[dict[str, Any]] = []
    if not description:
        issues.append(issue(path, "description", "document_defect", "正文没有可确认的功能说明。"))
    if not fields["signature"] and not re.search(r"api_signature:|函数原型|函数声明", body):
        issues.append(issue(path, "signature", "document_defect", "正文没有可确认的函数原型信息。"))
    for signature in fields["signature"]:
        if not isinstance(signature, dict) or not (isinstance(signature.get("prototype_id"), str) ^ isinstance(signature.get("override_id"), str)):
            issues.append(issue(path, "signature", "tool_defect", "signature 条目必须包含 prototype_id 或 override_id，不能写入原始 prototype 字段。"))
            break
    for item in fields["tag"]:
        namespace, selected = next(iter(item.items()))
        if not selected:
            issues.append(issue(path, f"tag.{namespace}", "unresolved", "当前 worker 没有找到该标签主题的充分正文证据。"))
    return {
        "path": path,
        "sha256": sha256(document),
        "fields": fields,
        "evidence": {"description": ([{"source_path": path, "excerpt": description}] if description else [])},
        "issues": issues,
        "doxygen": [],
        "example_markers": [],
    }


def write_json(path: Path, value: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def prepare(args: argparse.Namespace) -> int:
    root = args.devkit_root.expanduser().resolve()
    if not root.is_dir():
        raise ValidationError(f"DevKit root does not exist: {root}")
    selected = api_documents(root, args.source_dir, args.document)
    run_dir = args.run_dir.expanduser().resolve()
    run_dir.mkdir(parents=True, exist_ok=True)
    batches = [selected[index:index + 25] for index in range(0, len(selected), 25)]
    manifest = {
        "schema_version": 1,
        "run_id": run_dir.name,
        "devkit_root": str(root),
        "documents": selected,
        "batches": [],
        "taxonomy": str((Path(__file__).resolve().parents[1] / "taxonomy" / "core-v8-frozen.json").resolve()),
    }
    for index, batch in enumerate(batches, 1):
        batch_id = f"batch-{index:02d}"
        batch_dir = run_dir / "batches" / batch_id
        task = {
            "schema_version": 1,
            "run_id": run_dir.name,
            "batch_id": batch_id,
            "devkit_root": str(root),
            "documents": batch,
            "taxonomy": manifest["taxonomy"],
        }
        task_path = batch_dir / "task.json"
        write_json(task_path, task)
        manifest["batches"].append({"id": batch_id, "documents": batch, "task": str(task_path)})
    write_json(run_dir / "manifest.json", manifest)
    print(json.dumps({"run_dir": str(run_dir), "documents": len(selected), "batches": len(batches)}, ensure_ascii=False, indent=2))
    return 0


def worker(args: argparse.Namespace) -> int:
    task = read_json(args.task.expanduser().resolve())
    if task.get("schema_version") != 1:
        raise ValidationError("worker task schema_version must be 1")
    root = Path(task["devkit_root"]).resolve()
    namespaces, _ = taxonomy(Path(task["taxonomy"]).resolve())
    documents = [build_worker_document(root, path, namespaces) for path in task["documents"]]
    result = {key: task[key] for key in ("schema_version", "run_id", "batch_id")}
    result["worker_id"] = args.worker_id
    result["documents"] = documents
    write_json(args.output.expanduser().resolve(), result)
    print(json.dumps({"output": str(args.output), "documents": len(documents)}, ensure_ascii=False, indent=2))
    return 0


def blank_field(fields: dict[str, Any], field: str) -> None:
    if field == "tag":
        fields["tag"] = [{key: []} for item in fields.get("tag", []) if isinstance(item, dict) for key in item]
    elif field.startswith("tag."):
        namespace = field.split(".", 1)[1]
        updated = [
            {namespace: [] if key == namespace else values}
            for item in fields.get("tag", [])
            if isinstance(item, dict)
            for key, values in item.items()
        ]
        if not any(namespace in item for item in updated):
            updated.append({namespace: []})
        fields["tag"] = updated
    elif field in {"signature", "dependencies", "examples"}:
        fields[field] = []
    elif field in fields:
        fields[field] = ""


def collect_documents(manifest: dict[str, Any], run_dir: Path) -> list[dict[str, Any]]:
    expected = set(manifest["documents"])
    results: list[dict[str, Any]] = []
    seen: set[str] = set()
    for result_path in sorted((run_dir / "batches").glob("batch-*/worker-result*.json")):
        result = read_json(result_path)
        for document in result.get("documents", []):
            path = document.get("path")
            if path not in expected or path in seen:
                raise ValidationError(f"worker result has duplicate or out-of-scope document: {path}")
            seen.add(path)
            results.append(document)
    if seen != expected:
        missing = sorted(expected - seen)
        raise ValidationError("missing worker results: " + ", ".join(missing))
    return sorted(results, key=lambda item: item["path"])


def resolve_issues(args: argparse.Namespace) -> int:
    run_dir = args.run_dir.expanduser().resolve()
    manifest = read_json(run_dir / "manifest.json")
    documents = collect_documents(manifest, run_dir)
    issue_records = [item for document in documents for item in document.get("issues", [])]
    write_json(
        run_dir / "issues.json",
        {"schema_version": 1, "run_id": manifest["run_id"], "issues": issue_records},
    )
    if args.decisions is None:
        resolution_input = {
            "schema_version": 1,
            "run_id": manifest["run_id"],
            "issues": [
                {
                    "issue_id": item["issue_id"],
                    "document_path": item["document_path"],
                    "field": item["field"],
                    "issue_type": item["issue_type"],
                    "message": item["message"],
                    "evidence": item.get("evidence", []),
                }
                for item in issue_records
            ],
        }
        write_json(run_dir / "resolution-input.json", resolution_input)
        print(json.dumps({"issues": len(issue_records), "output": str(run_dir / 'resolution-input.json')}, ensure_ascii=False, indent=2))
        return 0
    decisions = read_json(args.decisions.expanduser().resolve())
    if decisions.get("schema_version") != 1 or decisions.get("run_id") != manifest["run_id"]:
        raise ValidationError("resolution decisions do not belong to this run")
    issue_map = {item["issue_id"]: item for document in documents for item in document.get("issues", [])}
    seen: set[str] = set()
    for decision in decisions.get("decisions", []):
        issue_id = decision.get("issue_id")
        if issue_id not in issue_map or issue_id in seen:
            raise ValidationError(f"invalid or duplicate issue decision: {issue_id}")
        if decision.get("classification") not in ISSUE_TYPES or decision.get("resolution") not in RESOLUTIONS:
            raise ValidationError(f"invalid decision for {issue_id}")
        if decision["classification"] != issue_map[issue_id]["issue_type"]:
            raise ValidationError(f"classification disagrees with issue: {issue_id}")
        if decision.get("field") not in {None, issue_map[issue_id]["field"]}:
            raise ValidationError(f"decision field disagrees with issue: {issue_id}")
        seen.add(issue_id)
    expected = set(issue_map)
    if seen != expected:
        raise ValidationError("every discovered issue needs one resolution decision")
    write_json(run_dir / "resolution-decisions.json", decisions)
    print(json.dumps({"issues": len(seen), "output": str(run_dir / 'resolution-decisions.json')}, ensure_ascii=False, indent=2))
    return 0


def normalize_dependencies(root: Path, document_path: str, dependencies: Any) -> list[dict[str, Any]]:
    if not isinstance(dependencies, list):
        raise ValidationError(f"{document_path}: dependencies must be an array")
    normalized = []
    seen = set()
    for index, item in enumerate(dependencies):
        if not isinstance(item, dict):
            raise ValidationError(f"{document_path}: dependencies[{index}] must be an object")
        path = item.get("path")
        if not isinstance(path, str):
            raise ValidationError(f"{document_path}: dependencies[{index}].path is required")
        target = relative_api_path(root, path)
        normalized_path = repo_relative(root, target)
        if normalized_path == document_path:
            raise ValidationError(f"{document_path}: self dependency is not allowed")
        kind = item.get("kind")
        if kind not in DEPENDENCY_KINDS:
            raise ValidationError(f"{document_path}: invalid dependency kind {kind!r}")
        condition = item.get("condition")
        if condition is not None and (not isinstance(condition, str) or not condition.strip()):
            raise ValidationError(f"{document_path}: dependency condition must be non-empty")
        evidence = item.get("evidence")
        if not isinstance(evidence, dict) or set(evidence) != {"source_path", "excerpt"}:
            raise ValidationError(f"{document_path}: every dependency requires source_path and excerpt evidence")
        source_path = evidence.get("source_path")
        excerpt = evidence.get("excerpt")
        source = relative_api_path(root, source_path)
        source_body = split_front_matter(source.read_text(encoding="utf-8", errors="replace"))[2]
        if not isinstance(excerpt, str) or not excerpt.strip() or excerpt not in source_body:
            raise ValidationError(f"{document_path}: dependency evidence excerpt is not verbatim source text")
        identity = (normalized_path, kind, condition or "")
        if identity in seen:
            raise ValidationError(f"{document_path}: duplicate dependency")
        seen.add(identity)
        output = {"path": normalized_path, "kind": kind}
        if condition:
            output["condition"] = condition
        normalized.append(output)
    return sorted(normalized, key=lambda item: (item["path"], item["kind"], item.get("condition", "")))


def validate_tags(
    fields: dict[str, Any],
    namespaces: tuple[str, ...],
    values: dict[str, set[str]],
    path: str,
    body: str,
    evidence: dict[str, Any],
    allowed_empty: set[str],
) -> list[dict[str, list[str]]]:
    result = normalize_tag(fields.get("tag"), namespaces)
    for item in result:
        namespace, selected = next(iter(item.items()))
        if not selected and f"tag.{namespace}" not in allowed_empty:
            raise ValidationError(f"{path}: required tag namespace is empty: {namespace}")
        unknown = set(selected) - values[namespace]
        if unknown:
            raise ValidationError(f"{path}: tag {namespace} has unknown values: {sorted(unknown)}")
        for selected_value in selected:
            key = f"{namespace}:{selected_value}"
            references = evidence.get(key)
            if not isinstance(references, list) or not references:
                raise ValidationError(f"{path}: missing verbatim evidence for tag {key}")
            for reference in references:
                if not isinstance(reference, dict) or not isinstance(reference.get("excerpt"), str):
                    raise ValidationError(f"{path}: invalid evidence for tag {key}")
                if namespace != "api_surfaces" and reference["excerpt"] not in body:
                    raise ValidationError(f"{path}: tag evidence is not verbatim for {key}")
    return result


def validate_signature(signature: Any, path: str) -> list[dict[str, Any]]:
    if not isinstance(signature, list):
        raise ValidationError(f"{path}: signature must be an array")
    for index, item in enumerate(signature):
        if not isinstance(item, dict):
            raise ValidationError(f"{path}: signature[{index}] must be an object")
        unknown = set(item) - SIGNATURE_KEYS
        if unknown:
            raise ValidationError(f"{path}: signature[{index}] has unsupported fields: {sorted(unknown)}")
        if not (isinstance(item.get("prototype_id"), str) ^ isinstance(item.get("override_id"), str)):
            raise ValidationError(f"{path}: signature[{index}] must contain exactly one prototype_id or override_id")
        for key in ("archs", "headers"):
            if key in item and (not isinstance(item[key], list) or any(not isinstance(value, str) for value in item[key])):
                raise ValidationError(f"{path}: signature[{index}].{key} must be a string array")
        if "dtype" in item and not isinstance(item["dtype"], (dict, list)):
            raise ValidationError(f"{path}: signature[{index}].dtype must be a mapping or array")
    return signature


def render_yaml(
    fields: dict[str, Any], namespaces: tuple[str, ...], values: dict[str, set[str]],
    root: Path, path: str, body: str, evidence: dict[str, Any], allowed_empty: set[str],
) -> str:
    output = {key: deepcopy(fields.get(key)) for key in ALLOWED_FIELDS}
    output["tag"] = validate_tags(output, namespaces, values, path, body, evidence, allowed_empty)
    output["tag"] = [
        {namespace: InlineList(selected)} for item in output["tag"] for namespace, selected in item.items()
    ]
    output["dependencies"] = normalize_dependencies(root, path, output.get("dependencies", []))
    for signature in output["signature"]:
        if isinstance(signature, dict):
            for key in ("archs", "headers"):
                if isinstance(signature.get(key), list):
                    signature[key] = InlineList(signature[key])
    for example in output["examples"]:
        if isinstance(example, dict) and isinstance(example.get("archs"), list):
            example["archs"] = InlineList(example["archs"])
    output["signature"] = validate_signature(output["signature"], path)
    if not isinstance(output["examples"], list):
        raise ValidationError(f"{path}: examples must be an array")
    if not output["signature"]:
        output.pop("signature")
    if not output["examples"]:
        output.pop("examples")
    return yaml.dump(output, allow_unicode=True, sort_keys=False, default_flow_style=False, Dumper=DisplayDumper)


def updated_document(
    original: str, fields: dict[str, Any], namespaces: tuple[str, ...], values: dict[str, set[str]],
    root: Path, path: str, evidence: dict[str, Any], allowed_empty: set[str],
) -> str:
    _, _, body = split_front_matter(original)
    return "---\n" + render_yaml(fields, namespaces, values, root, path, body, evidence, allowed_empty) + "---\n" + body


def validate_auxiliary_patch(root: Path, patch: dict[str, Any], key: str) -> tuple[Path, str]:
    path = patch.get("path")
    anchor = patch.get("anchor")
    text = patch.get("text")
    if not all(isinstance(item, str) and item for item in (path, anchor, text)):
        raise ValidationError(f"{key} patch requires path, anchor and text")
    target = (root / Path(path)).resolve()
    try:
        target.relative_to(root.resolve())
    except ValueError as exc:
        raise ValidationError(f"{key} patch escapes DevKit root: {path}") from exc
    if not target.is_file() or target.suffix.lower() not in {".h", ".hpp", ".inc", ".asc", ".cpp", ".c"}:
        raise ValidationError(f"{key} patch target is not an existing source file: {path}")
    if key == "doxygen" and not text.lstrip().startswith("/**"):
        raise ValidationError("Doxygen patch must insert a /** comment")
    if key == "example_markers" and not COMMENT_RE.match(text):
        raise ValidationError("example marker must be a comment")
    source = target.read_text(encoding="utf-8", errors="replace")
    if source.count(anchor) != 1:
        raise ValidationError(f"{key} anchor must occur exactly once: {path}")
    return target, source.replace(anchor, text + "\n" + anchor, 1)


def report_markdown(report: dict[str, Any]) -> str:
    lines = ["# OKF YAML 生成错误报告", "", f"运行：`{report['run_id']}`", ""]
    grouped: dict[str, list[dict[str, Any]]] = {}
    for item in report["issues"]:
        grouped.setdefault(item["document_path"], []).append(item)
    if not grouped:
        lines.append("本次运行未发现问题。")
        return "\n".join(lines) + "\n"
    for path in sorted(grouped):
        lines.extend([f"## {path}", "", "### 问题"])
        for item in grouped[path]:
            lines.append(f"- `{item['field']}`：{item['message']}（类型：{item['issue_type']}）")
            for evidence in item.get("evidence", []):
                lines.append(f"  - 证据：`{evidence.get('source_path', '-')}`：{evidence.get('excerpt', '-')}")
        lines.extend(["", "### 处理结果"])
        for item in grouped[path]:
            decision = item.get("decision", {})
            lines.append(f"- `{item['field']}`：{decision.get('resolution', '未处理')}；{decision.get('note', '')}")
        lines.append("")
    return "\n".join(lines)


def validate(args: argparse.Namespace) -> int:
    run_dir = args.run_dir.expanduser().resolve()
    manifest = read_json(run_dir / "manifest.json")
    root = Path(manifest["devkit_root"]).resolve()
    namespaces, values = taxonomy(Path(manifest["taxonomy"]).resolve())
    documents = collect_documents(manifest, run_dir)
    decisions_path = run_dir / "resolution-decisions.json"
    decisions = read_json(decisions_path) if decisions_path.is_file() else {"schema_version": 1, "run_id": manifest["run_id"], "decisions": []}
    decision_map = {item["issue_id"]: item for item in decisions.get("decisions", [])}
    issue_report: list[dict[str, Any]] = []
    patches: list[dict[str, Any]] = []
    requires_graph: dict[str, set[str]] = {}
    for document in documents:
        path = document["path"]
        target = relative_api_path(root, path)
        if sha256(target) != document["sha256"]:
            raise ValidationError(f"input changed after worker read: {path}")
        fields = deepcopy(document["fields"])
        allowed_empty: set[str] = set()
        for found in document.get("issues", []):
            decision = decision_map.get(found["issue_id"])
            if decision is None:
                raise ValidationError(f"missing issue resolution: {found['issue_id']}")
            item = deepcopy(found)
            item["decision"] = decision
            issue_report.append(item)
            if found["field"].startswith("tag.") and decision["resolution"] in {
                "blank_field", "report_source_defect", "report_unresolved"
            }:
                allowed_empty.add(found["field"])
            if decision["resolution"] == "blank_field":
                blank_field(fields, found["field"])
            if decision["resolution"] == "tool_change_required":
                raise ValidationError(f"tool change required: {found['issue_id']}")
        normalized_dependencies = normalize_dependencies(root, path, fields.get("dependencies", []))
        requires_graph[path] = {
            item["path"] for item in normalized_dependencies if item["kind"] == "requires"
        }
        updated = updated_document(
            target.read_text(encoding="utf-8", errors="replace"), fields, namespaces, values,
            root, path, document.get("evidence", {}), allowed_empty,
        )
        patches.append({"kind": "document", "path": path, "before_sha256": document["sha256"], "content": updated})
        for key in ("doxygen", "example_markers"):
            for auxiliary in document.get(key, []):
                target_path, content = validate_auxiliary_patch(root, auxiliary, key)
                patches.append({"kind": key, "path": repo_relative(root, target_path), "before_sha256": sha256(target_path), "content": content})
    states: dict[str, int] = {}
    stack: list[str] = []

    def visit(node: str) -> None:
        states[node] = 1
        stack.append(node)
        for target_path in sorted(requires_graph.get(node, ())):
            if states.get(target_path, 0) == 1:
                start = stack.index(target_path)
                raise ValidationError("required dependency cycle: " + " -> ".join(stack[start:] + [target_path]))
            if states.get(target_path, 0) == 0:
                visit(target_path)
        stack.pop()
        states[node] = 2

    for node in sorted(requires_graph):
        if states.get(node, 0) == 0:
            visit(node)
    report = {"schema_version": 1, "run_id": manifest["run_id"], "issues": issue_report, "documents": manifest["documents"]}
    write_json(run_dir / "error-report.json", report)
    (run_dir / "error-report.md").write_text(report_markdown(report), encoding="utf-8")
    write_json(run_dir / "write-plan.json", {"schema_version": 1, "run_id": manifest["run_id"], "patches": patches})
    write_json(run_dir / "validation.json", {"schema_version": 1, "run_id": manifest["run_id"], "status": "ok", "patches": len(patches), "issues": len(issue_report)})
    print(json.dumps({"status": "ok", "patches": len(patches), "issues": len(issue_report), "report": str(run_dir / 'error-report.md')}, ensure_ascii=False, indent=2))
    return 0


def write(args: argparse.Namespace) -> int:
    run_dir = args.run_dir.expanduser().resolve()
    validation = read_json(run_dir / "validation.json")
    if validation.get("status") != "ok":
        raise ValidationError("run has not passed validation")
    manifest = read_json(run_dir / "manifest.json")
    root = Path(manifest["devkit_root"]).resolve()
    plan = read_json(run_dir / "write-plan.json")
    with write_lock(root):
        for patch in plan.get("patches", []):
            target = (root / patch["path"]).resolve()
            if sha256(target) != patch["before_sha256"]:
                raise ValidationError(f"input changed after dry-run: {patch['path']}")
        for patch in plan.get("patches", []):
            target = (root / patch["path"]).resolve()
            target.write_text(patch["content"], encoding="utf-8")
    write_json(run_dir / "write-result.json", {"schema_version": 1, "run_id": manifest["run_id"], "written": len(plan.get("patches", []))})
    print(json.dumps({"written": len(plan.get("patches", [])), "run_dir": str(run_dir)}, ensure_ascii=False, indent=2))
    return 0


@contextmanager
def write_lock(root: Path):
    lock_name = hashlib.sha1(str(root.resolve()).encode("utf-8")).hexdigest()[:16]
    lock_path = Path("/tmp") / f"okf-yaml-generate-{lock_name}.lock"
    with lock_path.open("w", encoding="utf-8") as handle:
        fcntl.flock(handle.fileno(), fcntl.LOCK_EX)
        try:
            yield
        finally:
            fcntl.flock(handle.fileno(), fcntl.LOCK_UN)


def parser() -> argparse.ArgumentParser:
    root = argparse.ArgumentParser(description=__doc__)
    sub = root.add_subparsers(dest="command", required=True)
    prepare_parser = sub.add_parser("prepare")
    prepare_parser.add_argument("--devkit-root", type=Path, required=True)
    prepare_parser.add_argument("--source-dir")
    prepare_parser.add_argument("--document", action="append")
    prepare_parser.add_argument("--run-dir", type=Path, required=True)
    worker_parser = sub.add_parser("worker")
    worker_parser.add_argument("--task", type=Path, required=True)
    worker_parser.add_argument("--output", type=Path, required=True)
    worker_parser.add_argument("--worker-id", default="worker-a")
    resolve_parser = sub.add_parser("resolve-issues")
    resolve_parser.add_argument("--run-dir", type=Path, required=True)
    resolve_parser.add_argument("--decisions", type=Path)
    for name, function in (("validate", validate), ("write", write)):
        command = sub.add_parser(name)
        command.add_argument("--run-dir", type=Path, required=True)
        command.set_defaults(handler=function)
    return root


def main() -> int:
    args = parser().parse_args()
    try:
        if args.command == "prepare":
            return prepare(args)
        if args.command == "worker":
            return worker(args)
        if args.command == "resolve-issues":
            return resolve_issues(args)
        return args.handler(args)
    except (OSError, KeyError, TypeError, ValueError, ValidationError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
