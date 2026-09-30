#!/usr/bin/env python3
"""OKF YAML 生成统一入口（slim 头，5 字段）。

管线：prepare →（LLM 语义标注 artifacts）→ validate → write。
字段与推导契约的唯一事实源是 okf_skills/字段与推导规范.md；本脚本只实现
写入、校验与确定性推导（version ← Release Notes），不承载字段定义。

文档头固定 5 字段：okftype, description, tag, dependencies, version。
可推导字段（signature/archs/header/examples/navigation 等）一律不写。
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path
from typing import Any

import yaml

SCRIPTS_DIR = Path(__file__).resolve().parent
SKILL_ROOT = SCRIPTS_DIR.parent
if str(SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPTS_DIR))

from derive_version import build_version_map, version_for  # noqa: E402
from model_registry import (  # noqa: E402
    Classification,
    ExecutionPlan,
    ModelValidationError,
    classify_document,
    compile_execution_plan,
    document_is_in_scope,
    load_model,
)

API_ROOT = Path("docs/zh/api/SIMD-API")
API_SURFACES = {"c_api"}
TAXONOMY_PATH = SKILL_ROOT.parent / "config" / "tag-taxonomy.json"
VERSION_OVERRIDES_PATH = SKILL_ROOT / "config" / "version-overrides.yaml"
DEPENDENCY_KINDS = {"requires", "configures", "constrained_by", "related", "replaced_by"}
SEMANTIC_PURPOSE = "okf_semantic_annotations"
SEMANTIC_SCHEMA_VERSION = 1
MIN_DESCRIPTION = 8
MAX_DESCRIPTION = 180
DESCRIPTION_FORBIDDEN = ("#", "```", "[", "]", "|")

_MODEL = load_model()
OKF_TYPES = tuple(_MODEL["profiles"])
ALLOWED_FIELDS = tuple(_MODEL["field_order"])
GENERATED_FIELDS = frozenset(ALLOWED_FIELDS)
VALIDATED_FIELDS = frozenset(ALLOWED_FIELDS)
TAXONOMY_NAMESPACES, TAXONOMY_VALUES = ((), {})
CONDITIONAL_OPERATIONS_ROLES = {"compute", "data_movement", "data_initialization"}


class ValidationError(ValueError):
    pass


# ---------------------------------------------------------------- utilities


class InlineList(list):
    """YAML sequence rendered in flow style."""


class DisplayDumper(yaml.SafeDumper):
    def increase_indent(self, flow: bool = False, indentless: bool = False) -> None:
        return super().increase_indent(flow, False)


def represent_inline_list(dumper: yaml.SafeDumper, value: InlineList) -> yaml.nodes.SequenceNode:
    return dumper.represent_sequence("tag:yaml.org,2002:seq", value, flow_style=True)


DisplayDumper.add_representer(InlineList, represent_inline_list)


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
    return data, raw, text[end + len("\n---\n"):]


def read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def write_json(path: Path, value: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def repo_relative(root: Path, path: Path) -> str:
    return path.resolve().relative_to(root.resolve()).as_posix()


def relative_api_path(root: Path, value: str) -> Path:
    candidate = Path(value)
    if candidate.is_absolute() or ".." in candidate.parts or candidate.suffix.lower() != ".md":
        raise ValidationError(f"invalid repository-relative Markdown path: {value!r}")
    path = candidate.as_posix()
    if not document_is_in_scope(path):
        raise ValidationError(f"path is outside registered document roots: {value!r}")
    resolved = (root / candidate).resolve()
    try:
        resolved.relative_to(root.resolve())
    except ValueError as exc:
        raise ValidationError(f"path escapes DevKit root: {value!r}") from exc
    if not resolved.is_file():
        raise ValidationError(f"API document does not exist: {value!r}")
    return resolved


def api_documents(root: Path, source_dirs: list[str] | None, document: list[str] | None) -> list[str]:
    if document and source_dirs:
        raise ValidationError("--document and --source-dir cannot be combined")
    if document:
        return sorted({repo_relative(root, relative_api_path(root, item)) for item in document})
    bases = [Path(item) for item in (source_dirs or [str(API_ROOT / "c_api")])]
    selected: set[str] = set()
    for base in bases:
        if base.is_absolute():
            base = Path(repo_relative(root, base))
        if not document_is_in_scope((base / "placeholder.md").as_posix()):
            raise ValidationError("--source-dir is outside registered document roots")
        directory = (root / base).resolve()
        if not directory.is_dir():
            raise ValidationError(f"source directory does not exist: {base}")
        selected.update(repo_relative(root, path) for path in directory.rglob("*.md"))
    return sorted(selected)


def taxonomy(path: Path = TAXONOMY_PATH) -> tuple[tuple[str, ...], dict[str, set[str]]]:
    """冻结词表：返回全部 namespace 及其合法值（不止 required）。"""
    payload = json.loads(path.read_text(encoding="utf-8"))
    if payload.get("status") != "frozen":
        raise ValidationError("taxonomy is not frozen")
    defs = payload.get("namespaces")
    if not isinstance(defs, dict) or not defs:
        raise ValidationError("taxonomy namespaces are invalid")
    values: dict[str, set[str]] = {}
    for namespace, definition in defs.items():
        options = definition.get("values") if isinstance(definition, dict) else None
        if not isinstance(options, dict):
            raise ValidationError(f"taxonomy values missing: {namespace}")
        values[namespace] = set(options)
    order = tuple(defs)
    return order, values


TAXONOMY_NAMESPACES, TAXONOMY_VALUES = taxonomy()


def first_heading(body: str, fallback: str) -> str:
    match = re.search(r"(?m)^#\s+(.+?)\s*$", body)
    return match.group(1).strip() if match else fallback


def document_body(root: Path, rel: str) -> str:
    _, _, body = split_front_matter((root / rel).read_text(encoding="utf-8"))
    return body


def issue(doc: str, field: str, message: str) -> dict[str, str]:
    return {"document": doc, "field": field, "message": message}


# ---------------------------------------------------------------- prepare


def classify_documents(root: Path, paths: list[str]) -> tuple[list[dict[str, Any]], list[dict[str, str]]]:
    tasks: list[dict[str, Any]] = []
    unclassified: list[dict[str, str]] = []
    for rel in paths:
        body = document_body(root, rel)
        classification: Classification | None = classify_document(rel, body)
        if classification is None:
            unclassified.append({"path": rel, "reason": "insufficient_structure"})
            continue
        plan: ExecutionPlan = compile_execution_plan(classification)
        tasks.append({
            "path": rel,
            "sha256": sha256(root / rel),
            "okftype": plan.okftype,
            "rule": plan.rule,
            "fields": list(plan.fields),
            "required": list(plan.required),
            "needs_annotation": "description" in plan.required or "tag" in plan.required,
        })
    return tasks, unclassified


def command_prepare(args: argparse.Namespace) -> int:
    root = args.devkit_root.resolve()
    run_dir = args.run_dir.resolve()
    if run_dir.exists() and any(run_dir.iterdir()):
        raise ValidationError(f"run directory is not empty: {run_dir}")
    paths = api_documents(root, args.source_dir, args.document)
    tasks, unclassified = classify_documents(root, paths)
    report = {
        "documents": len(paths),
        "classified": len(tasks),
        "unclassified": [{"path": item["path"], "reason": item["reason"]} for item in unclassified],
        "okftype_counts": {},
    }
    for task in tasks:
        report["okftype_counts"][task["okftype"]] = report["okftype_counts"].get(task["okftype"], 0) + 1
    write_json(run_dir / "classification-report.json", report)
    if unclassified:
        print(json.dumps(report, ensure_ascii=False, indent=2))
        raise ValidationError(f"{len(unclassified)} document(s) cannot be classified; run stopped")
    version_map = build_version_map(root, VERSION_OVERRIDES_PATH if VERSION_OVERRIDES_PATH.is_file() else None)
    for task in tasks:
        if "version" in task["required"]:
            task["version"] = version_for(task["path"], version_map)["version"]
    manifest = {
        "schema_version": 2,
        "purpose": "okf_yaml_run",
        "devkit_root": str(root),
        "run_dir": str(run_dir),
        "documents": tasks,
        "taxonomy": {"path": str(TAXONOMY_PATH), "sha256": sha256(TAXONOMY_PATH)},
        "version_overrides": str(VERSION_OVERRIDES_PATH) if VERSION_OVERRIDES_PATH.is_file() else "",
    }
    write_json(run_dir / "manifest.json", manifest)
    write_json(run_dir / "version-map.json", version_map)
    write_json(run_dir / "annotation-task.json", {
        "schema_version": 1,
        "purpose": "okf_semantic_annotation_assignment",
        "run_dir": str(run_dir),
        "documents": [t["path"] for t in tasks if t["needs_annotation"]],
        "contract": "references/annotation-contract.md",
    })
    print(json.dumps({
        "run_dir": str(run_dir),
        "documents": len(tasks),
        "needs_annotation": sum(1 for t in tasks if t["needs_annotation"]),
        "version_map": "version-map.json",
    }, ensure_ascii=False))
    return 0


# ---------------------------------------------------------------- annotations


def load_annotations(run_dir: Path) -> dict[str, dict[str, Any]]:
    merged: dict[str, dict[str, Any]] = {}
    directory = run_dir / "annotations"
    if not directory.is_dir():
        return merged
    for path in sorted(directory.glob("*.json")):
        payload = read_json(path)
        if payload.get("purpose") != SEMANTIC_PURPOSE or payload.get("schema_version") != SEMANTIC_SCHEMA_VERSION:
            raise ValidationError(f"invalid annotation artifact: {path}")
        for annotation in payload.get("annotations", []):
            rel = annotation.get("document_path")
            if not isinstance(rel, str):
                raise ValidationError(f"annotation without document_path: {path}")
            if rel in merged:
                raise ValidationError(f"duplicate annotation for {rel} (in {path})")
            merged[rel] = annotation
    return merged


def validate_description(annotation: dict[str, Any], rel: str, body: str, title: str, stem: str) -> list[dict[str, str]]:
    issues: list[dict[str, str]] = []
    description = annotation.get("description")
    if not isinstance(description, dict):
        return [issue(rel, "description", "description {value, evidence} is required")]
    value = description.get("value")
    if not isinstance(value, str):
        issues.append(issue(rel, "description", "description.value must be a string"))
        value = ""
    value = value.strip()
    if not MIN_DESCRIPTION <= len(value) <= MAX_DESCRIPTION:
        issues.append(issue(rel, "description", f"description length must be {MIN_DESCRIPTION}-{MAX_DESCRIPTION}"))
    if "\n" in value:
        issues.append(issue(rel, "description", "description must be a single paragraph"))
    if any(token in value for token in DESCRIPTION_FORBIDDEN):
        issues.append(issue(rel, "description", "description must not contain Markdown syntax"))
    if value in {title, stem}:
        issues.append(issue(rel, "description", "description must not repeat the title or file name"))
    evidence = description.get("evidence")
    if not isinstance(evidence, list) or not 1 <= len(evidence) <= 3:
        issues.append(issue(rel, "description", "description requires 1-3 evidence excerpts"))
        evidence = []
    for record in evidence:
        excerpt = record.get("excerpt") if isinstance(record, dict) else None
        if not isinstance(excerpt, str) or excerpt not in body:
            issues.append(issue(rel, "description", "description evidence is not verbatim body text"))
    return issues


def validate_tag(annotation: dict[str, Any], rel: str, body: str, required: list[str]) -> list[dict[str, str]]:
    issues: list[dict[str, str]] = []
    tag = annotation.get("tag", {})
    if not isinstance(tag, dict) or not tag:
        if "tag" in required:
            return [issue(rel, "tag", "tag is required for this profile")]
        return []
    unknown_ns = sorted(set(tag) - set(TAXONOMY_NAMESPACES))
    if unknown_ns:
        issues.append(issue(rel, "tag", f"unknown namespaces: {unknown_ns}"))
    for namespace, values in tag.items():
        if not isinstance(values, list) or not values:
            issues.append(issue(rel, "tag", f"namespace {namespace} must be a non-empty list"))
            continue
        unknown = sorted(set(values) - TAXONOMY_VALUES.get(namespace, set()))
        if unknown:
            issues.append(issue(rel, "tag", f"values outside taxonomy in {namespace}: {unknown}"))
    if "roles" in required and "roles" not in tag:
        issues.append(issue(rel, "tag", "roles is required"))
    roles = set(tag.get("roles", []))
    if roles & CONDITIONAL_OPERATIONS_ROLES and "operations" not in tag:
        issues.append(issue(rel, "tag", "operations is required for compute/data_movement/data_initialization roles"))
    evidence = annotation.get("tag_evidence", {})
    if not isinstance(evidence, dict):
        issues.append(issue(rel, "tag", "tag_evidence must be a mapping"))
        evidence = {}
    for namespace, values in tag.items():
        if not isinstance(values, list):
            continue
        for value in values:
            excerpt = evidence.get(f"{namespace}:{value}")
            if not isinstance(excerpt, str) or excerpt not in body:
                issues.append(issue(rel, "tag", f"tag evidence for {namespace}:{value} is not verbatim body text"))
    extra = sorted(set(evidence) - {f"{ns}:{v}" for ns, vs in tag.items() if isinstance(vs, list) for v in vs})
    if extra:
        issues.append(issue(rel, "tag", f"tag_evidence has unknown keys: {extra}"))
    return issues


def validate_dependencies(annotation: dict[str, Any], rel: str, root: Path, body: str) -> list[dict[str, str]]:
    issues: list[dict[str, str]] = []
    dependencies = annotation.get("dependencies")
    if dependencies is None:
        return [issue(rel, "dependencies", "dependencies is required (may be an empty list)")]
    if not isinstance(dependencies, list):
        return [issue(rel, "dependencies", "dependencies must be an array")]
    seen: set[tuple[str, str]] = set()
    for dep in dependencies:
        if not isinstance(dep, dict):
            issues.append(issue(rel, "dependencies", "dependency entry must be an object"))
            continue
        path, kind = dep.get("path"), dep.get("kind")
        if not isinstance(path, str) or not path.endswith(".md") or ".." in Path(path).parts:
            issues.append(issue(rel, "dependencies", f"invalid dependency path: {path!r}"))
            continue
        if path == rel:
            issues.append(issue(rel, "dependencies", "dependency must not reference the document itself"))
            continue
        if kind not in DEPENDENCY_KINDS:
            issues.append(issue(rel, "dependencies", f"invalid dependency kind: {kind!r}"))
            continue
        if not (root / path).is_file():
            issues.append(issue(rel, "dependencies", f"dependency target does not exist: {path}"))
            continue
        evidence = dep.get("evidence")
        excerpt = evidence.get("excerpt") if isinstance(evidence, dict) else None
        source = evidence.get("source_path") if isinstance(evidence, dict) else None
        if source != rel or not isinstance(excerpt, str) or excerpt not in body:
            issues.append(issue(rel, "dependencies", f"dependency evidence for {path} is not verbatim body text"))
        condition = dep.get("condition", "")
        if condition is not None and not isinstance(condition, str):
            issues.append(issue(rel, "dependencies", "condition must be a string"))
        key = (path, kind)
        if key in seen:
            issues.append(issue(rel, "dependencies", f"duplicate dependency: {path} ({kind})"))
        seen.add(key)
    return issues


def check_requires_cycles(annotations: dict[str, dict[str, Any]]) -> list[str]:
    adjacency: dict[str, list[str]] = {}
    for rel, annotation in annotations.items():
        for dep in annotation.get("dependencies") or []:
            if isinstance(dep, dict) and dep.get("kind") == "requires":
                adjacency.setdefault(rel, []).append(dep["path"])
    state: dict[str, str] = {}
    cycles: list[str] = []

    def visit(node: str, stack: list[str]) -> None:
        state[node] = "active"
        for nxt in sorted(adjacency.get(node, [])):
            if state.get(nxt) == "active":
                cycles.append(" -> ".join(stack + [node, nxt]))
            elif state.get(nxt) != "done":
                visit(nxt, stack + [node])
        state[node] = "done"

    for node in sorted(adjacency):
        if state.get(node) != "done":
            visit(node, [])
    return cycles


def validate_run(root: Path, run_dir: Path) -> dict[str, Any]:
    manifest = read_json(run_dir / "manifest.json")
    if manifest.get("purpose") != "okf_yaml_run":
        raise ValidationError("manifest is invalid")
    if sha256(TAXONOMY_PATH) != manifest["taxonomy"]["sha256"]:
        raise ValidationError("taxonomy changed since prepare; rerun prepare")
    annotations = load_annotations(run_dir)
    documents = {task["path"]: task for task in manifest["documents"]}
    issues: list[dict[str, str]] = []
    for rel, task in documents.items():
        path = root / rel
        if not path.is_file() or sha256(path) != task["sha256"]:
            issues.append(issue(rel, "*", "document changed since prepare; rerun prepare"))
            continue
        annotation = annotations.get(rel)
        if task["needs_annotation"] and annotation is None:
            issues.append(issue(rel, "*", "missing semantic annotation"))
            continue
        if annotation is None:
            continue
        if annotation.get("document_sha256") != task["sha256"]:
            issues.append(issue(rel, "*", "annotation sha256 does not match prepare"))
            continue
        body = document_body(root, rel)
        stem = Path(rel).stem
        title = first_heading(body, stem)
        required = task["required"]
        if "description" in required or "description" in annotation:
            issues += validate_description(annotation, rel, body, title, stem)
        if "tag" in required or "tag" in annotation:
            issues += validate_tag(annotation, rel, body, required)
        issues += validate_dependencies(annotation, rel, root, body)
        if "version" in required:
            expected = task.get("version")
            if expected and annotation.get("version") not in (None, expected):
                issues.append(issue(rel, "version", f"annotation version {annotation.get('version')!r} != derived {expected!r}"))
    for rel in sorted(set(annotations) - set(documents)):
        issues.append(issue(rel, "*", "annotation for a document outside this run"))
    for cycle in check_requires_cycles(annotations):
        issues.append(issue("*", "dependencies", f"requires cycle: {cycle}"))
    valid_docs = sorted(
        rel for rel, task in documents.items()
        if task["needs_annotation"] and rel in annotations
        and not any(i["document"] == rel for i in issues)
    )
    return {
        "schema_version": 2,
        "purpose": "okf_yaml_validation",
        "status": "ok" if not issues else "blocked",
        "documents": len(documents),
        "annotated": sum(1 for t in documents.values() if t["needs_annotation"]),
        "ready_documents": valid_docs,
        "issues": issues,
    }


def command_validate(args: argparse.Namespace) -> int:
    root = args.devkit_root.resolve() if args.devkit_root else None
    run_dir = args.run_dir.resolve()
    if root is None:
        manifest = read_json(run_dir / "manifest.json")
        root = Path(manifest["devkit_root"])
    result = validate_run(root, run_dir)
    write_json(run_dir / "validation.json", result)
    print(json.dumps({
        "status": result["status"],
        "documents": result["documents"],
        "ready": len(result["ready_documents"]),
        "issues": len(result["issues"]),
        "report": str(run_dir / "validation.json"),
    }, ensure_ascii=False))
    return 0 if result["status"] == "ok" else 1


# ---------------------------------------------------------------- render & write


def scalar(value: str) -> str:
    return json.dumps(value, ensure_ascii=False)


def render_front_matter(task: dict[str, Any], annotation: dict[str, Any] | None) -> str:
    fields = set(task.get("fields", []))
    lines = [f"okftype: {task['okftype']}"]
    if annotation:
        description = annotation.get("description")
        if isinstance(description, dict) and isinstance(description.get("value"), str):
            lines.append(f"description: {scalar(description['value'].strip())}")
        tag = annotation.get("tag")
        if isinstance(tag, dict) and tag:
            lines.append("tag:")
            for namespace in TAXONOMY_NAMESPACES:
                values = tag.get(namespace)
                if isinstance(values, list) and values:
                    lines.append(f"  - {namespace}: [{', '.join(sorted(set(values)))}]")
        if "dependencies" in fields:
            dependencies = annotation.get("dependencies")
            if isinstance(dependencies, list) and dependencies:
                lines.append("dependencies:")
                for dep in sorted(dependencies, key=lambda d: (d.get("path", ""), d.get("kind", ""))):
                    entry = f"  - path: {dep['path']}"
                    entry += f"\n    kind: {dep['kind']}"
                    if dep.get("condition"):
                        entry += f"\n    condition: {scalar(dep['condition'])}"
                    lines.append(entry)
            else:
                lines.append("dependencies: []")
        if "version" in task.get("required", []):
            lines.append(f"version: {task.get('version', '9.0.0-')}")
    return "---\n" + "\n".join(lines) + "\n---\n"


def apply_front_matter(text: str, front_matter: str) -> str:
    if text.startswith("---\n"):
        end = text.find("\n---\n", 4)
        if end < 0:
            raise ValidationError("front matter is not terminated")
        return front_matter + text[end + len("\n---\n"):]
    return front_matter + text


def command_write(args: argparse.Namespace) -> int:
    run_dir = args.run_dir.resolve()
    result = validate_run(Path(read_json(run_dir / "manifest.json")["devkit_root"]), run_dir)
    write_json(run_dir / "validation.json", result)
    if result["status"] != "ok" and not args.allow_partial:
        raise ValidationError(
            f"validation blocked with {len(result['issues'])} issue(s); fix annotations or rerun prepare; "
            "use --allow-partial to write only ready documents"
        )
    manifest = read_json(run_dir / "manifest.json")
    root = Path(manifest["devkit_root"])
    annotations = load_annotations(run_dir)
    documents = {task["path"]: task for task in manifest["documents"]}
    ready = set(result["ready_documents"]) if args.allow_partial else {
        rel for rel, task in documents.items()
        if not task["needs_annotation"] or rel in annotations
    }
    if result["status"] != "ok":
        blocked = {i["document"] for i in result["issues"]}
        ready = {rel for rel in ready if rel not in blocked}
    written: list[str] = []
    skipped: list[dict[str, str]] = []
    for rel, task in sorted(documents.items()):
        if rel not in ready:
            skipped.append({"path": rel, "reason": "validation issues"})
            continue
        path = root / rel
        if sha256(path) != task["sha256"]:
            skipped.append({"path": rel, "reason": "document changed since prepare"})
            continue
        front_matter = render_front_matter(task, annotations.get(rel))
        if not args.write:
            skipped.append({"path": rel, "reason": "dry-run"})
            continue
        new_text = apply_front_matter(path.read_text(encoding="utf-8"), front_matter)
        path.write_text(new_text, encoding="utf-8")
        if split_front_matter(new_text)[0].get("okftype") != task["okftype"]:
            raise ValidationError(f"post-write parse failed: {rel}")
        written.append(rel)
    payload = {
        "schema_version": 2,
        "purpose": "okf_yaml_write_result",
        "write": args.write,
        "written": written,
        "skipped": skipped,
    }
    write_json(run_dir / "write-result.json", payload)
    print(json.dumps({
        "write": args.write,
        "written": len(written),
        "skipped": len(skipped),
        "result": str(run_dir / "write-result.json"),
    }, ensure_ascii=False))
    return 0


# ---------------------------------------------------------------- check


def field_closure_check() -> dict[str, Any]:
    errors: list[str] = []
    model = load_model()
    fields = model["document"]["field_catalog"]
    if set(model["field_order"]) != set(fields):
        errors.append("field_order must contain every registered field exactly once")
    for name, profile in model["profiles"].items():
        unknown = sorted(set(profile.get("fields", [])) - set(fields))
        if unknown:
            errors.append(f"profile {name} references unknown fields: {unknown}")
    implemented = set(GENERATED_FIELDS)
    missing = sorted(set(fields) - implemented)
    if missing:
        errors.append(f"registered fields without generator/validator: {missing}")
    extra = sorted(implemented - set(fields))
    if extra:
        errors.append(f"implemented fields outside registry: {extra}")
    return {"ok": not errors, "registry": "okf-document-profile-v5", "errors": errors}


def command_check(args: argparse.Namespace) -> int:
    result = field_closure_check()
    print(json.dumps(result, ensure_ascii=False))
    return 0 if result["ok"] else 1


# ---------------------------------------------------------------- CLI


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description=__doc__)
    commands = p.add_subparsers(dest="command", required=True)

    prepare = commands.add_parser("prepare", help="分类文档并生成运行清单与标注任务。")
    prepare.add_argument("--devkit-root", type=Path, required=True)
    prepare.add_argument("--source-dir", action="append", default=None, help="调研源目录（可重复）")
    prepare.add_argument("--document", action="append", default=None)
    prepare.add_argument("--run-dir", type=Path, required=True)
    prepare.set_defaults(func=command_prepare)

    validate = commands.add_parser("validate", help="校验标注工件与文档哈希。")
    validate.add_argument("--devkit-root", type=Path, default=None)
    validate.add_argument("--run-dir", type=Path, required=True)
    validate.set_defaults(func=command_validate)

    write = commands.add_parser("write", help="渲染并写入 slim 头；默认 dry-run。")
    write.add_argument("--run-dir", type=Path, required=True)
    write.add_argument("--write", action="store_true")
    write.add_argument("--allow-partial", action="store_true", help="仅写入校验就绪的文档")
    write.set_defaults(func=command_write)

    check = commands.add_parser("check", help="字段闭包一致性检查。")
    check.set_defaults(func=command_check)
    return p


def main() -> int:
    args = parser().parse_args()
    try:
        return args.func(args)
    except (ValidationError, ModelValidationError, FileNotFoundError, json.JSONDecodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
