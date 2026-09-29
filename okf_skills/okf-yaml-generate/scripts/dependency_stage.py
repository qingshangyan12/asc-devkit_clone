#!/usr/bin/env python3
"""Validate and write evidence-backed OKF ``dependencies`` fields."""

from __future__ import annotations

import argparse
from copy import deepcopy
import json
from pathlib import Path
import re
import sys
from typing import Any

import yaml

from yaml_deprecation_evidence import explicitly_deprecates_subject


SCHEMA_VERSION = 1
API_ROOT = Path("docs/zh/api/SIMD-API")
API_SURFACES = {"c_api", "basic_api", "adv_api", "tensor_api"}
DEPENDENCY_KINDS = {"requires", "configures", "constrained_by", "related", "replaced_by"}
TOP_LEVEL_KEY_RE = re.compile(r"^(?P<key>[A-Za-z_][A-Za-z0-9_-]*):(?:\s.*)?(?:\n|$)")


class ValidationError(ValueError):
    """Raised when a dependency artifact or target document is invalid."""


def split_front_matter(text: str) -> tuple[dict[str, Any], str, str]:
    if not text.startswith("---\n"):
        raise ValidationError("API document has no YAML front matter")
    end = text.find("\n---\n", 4)
    if end < 0:
        raise ValidationError("API document front matter is not terminated")
    raw = text[4:end]
    try:
        data = yaml.safe_load(raw) or {}
    except yaml.YAMLError as exc:
        raise ValidationError(f"API document front matter is invalid YAML: {exc}") from exc
    if not isinstance(data, dict):
        raise ValidationError("API document front matter must be a mapping")
    return data, raw, text[end + len("\n---\n") :]


def load_artifact(path: Path) -> list[dict[str, Any]]:
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise ValidationError(f"cannot read dependency artifact: {path}: {exc}") from exc
    if not isinstance(payload, dict) or payload.get("schema_version") != SCHEMA_VERSION:
        raise ValidationError("dependency artifact must have schema_version 1")
    unknown = sorted(set(payload) - {"schema_version", "run_id", "annotations"})
    if unknown:
        raise ValidationError("dependency artifact has unknown fields: " + ", ".join(unknown))
    annotations = payload.get("annotations")
    if not isinstance(annotations, list) or not annotations:
        raise ValidationError("dependency artifact must contain a non-empty annotations list")
    if not all(isinstance(item, dict) for item in annotations):
        raise ValidationError("dependency annotations must contain objects")
    return annotations


def ensure_inside(root: Path, value: str, location: str, *, markdown: bool = True) -> Path:
    if not isinstance(value, str) or not value.strip():
        raise ValidationError(f"{location} must be a non-empty repository-relative path")
    candidate_text = value.strip()
    candidate = Path(candidate_text)
    if candidate.is_absolute() or ".." in candidate.parts or "\\" in candidate_text:
        raise ValidationError(f"{location} must be repository-relative: {value!r}")
    if markdown and candidate.suffix.lower() != ".md":
        raise ValidationError(f"{location} must end with .md: {value!r}")
    resolved_root = root.resolve()
    resolved = (resolved_root / candidate).resolve()
    try:
        resolved.relative_to(resolved_root)
    except ValueError as exc:
        raise ValidationError(f"{location} escapes the DevKit root: {value!r}") from exc
    if not resolved.is_file():
        raise ValidationError(f"{location} does not exist: {value!r}")
    return resolved


def valid_api_document_path(path: str) -> bool:
    candidate = Path(path)
    parts = candidate.parts
    return (
        not candidate.is_absolute()
        and ".." not in parts
        and candidate.suffix.lower() == ".md"
        and len(parts) > 4
        and parts[:4] == ("docs", "zh", "api", "SIMD-API")
        and parts[4] in API_SURFACES
    )


def strip_front_matter(text: str) -> str:
    if text.startswith("---\n"):
        end = text.find("\n---\n", 4)
        if end >= 0:
            return text[end + len("\n---\n") :]
    return text


def dependency_identity(dependency: dict[str, Any]) -> tuple[str, str, str]:
    return dependency["path"], dependency["kind"], dependency.get("condition") or ""


def validate_evidence(
    evidence: Any,
    location: str,
    root: Path,
) -> dict[str, str]:
    if not isinstance(evidence, dict) or set(evidence) != {"source_path", "excerpt"}:
        raise ValidationError(f"{location} must contain source_path and excerpt")
    source_path = evidence.get("source_path")
    excerpt = evidence.get("excerpt")
    source = ensure_inside(root, source_path, location + ".source_path")
    if not isinstance(excerpt, str) or not excerpt.strip():
        raise ValidationError(f"{location}.excerpt must be a non-empty string")
    source_text = source.read_text(encoding="utf-8", errors="replace")
    if excerpt not in strip_front_matter(source_text):
        raise ValidationError(f"{location}.excerpt is not verbatim source text")
    return {"source_path": source_path, "excerpt": excerpt}


def validate_dependencies(
    raw: Any,
    location: str,
    root: Path,
    document_path: str,
) -> list[dict[str, Any]]:
    if not isinstance(raw, list):
        raise ValidationError(f"{location} must be an array")
    normalized: list[dict[str, Any]] = []
    seen: set[tuple[str, str, str | None]] = set()
    for index, item in enumerate(raw):
        entry_location = f"{location}[{index}]"
        if not isinstance(item, dict):
            raise ValidationError(f"{entry_location} must be an object")
        unknown = sorted(set(item) - {"path", "kind", "condition", "evidence"})
        if unknown:
            raise ValidationError(f"{entry_location} has unknown fields: {', '.join(unknown)}")
        path = item.get("path")
        target = ensure_inside(root, path, entry_location + ".path")
        normalized_path = target.relative_to(root.resolve()).as_posix()
        if normalized_path == document_path:
            raise ValidationError(f"{entry_location}.path cannot reference the executor itself")
        kind = item.get("kind")
        if not isinstance(kind, str) or kind not in DEPENDENCY_KINDS:
            raise ValidationError(
                f"{entry_location}.kind must be one of {', '.join(sorted(DEPENDENCY_KINDS))}"
            )
        condition = item.get("condition")
        if condition is not None and (not isinstance(condition, str) or not condition.strip()):
            raise ValidationError(f"{entry_location}.condition must be a non-empty string when present")
        identity = (normalized_path, kind, condition.strip() if isinstance(condition, str) else None)
        if identity in seen:
            raise ValidationError(f"{entry_location} duplicates dependency {normalized_path}")
        seen.add(identity)
        normalized_entry: dict[str, Any] = {"path": normalized_path, "kind": kind}
        if identity[2] is not None:
            normalized_entry["condition"] = identity[2]
        normalized_entry["evidence"] = validate_evidence(
            item.get("evidence"), entry_location + ".evidence", root
        )
        if kind == "replaced_by":
            excerpt = normalized_entry["evidence"]["excerpt"]
            source_body = strip_front_matter((root / document_path).read_text(encoding="utf-8", errors="replace"))
            if (
                normalized_entry["evidence"]["source_path"] != document_path
                or not any(explicitly_deprecates_subject(source_body, line) for line in source_body.splitlines())
                or not re.search(r"替代|改用|取代|代替|新接口形式请参见", excerpt)
                or (target.name not in excerpt and target.stem not in excerpt)
            ):
                raise ValidationError(
                    f"{entry_location}.evidence must name the replacement target and state the replacement"
                )
        normalized.append(normalized_entry)
    return sorted(normalized, key=dependency_identity)


def top_level_key(line: str) -> str | None:
    if not line or line[0].isspace():
        return None
    match = TOP_LEVEL_KEY_RE.match(line)
    return match.group("key") if match else None


def remove_top_level_blocks(lines: list[str], names: set[str]) -> list[str]:
    result: list[str] = []
    index = 0
    while index < len(lines):
        key = top_level_key(lines[index])
        if key in names:
            index += 1
            while index < len(lines) and top_level_key(lines[index]) is None:
                index += 1
            continue
        result.append(lines[index])
        index += 1
    return result


def render_dependencies(dependencies: list[dict[str, Any]]) -> str:
    if not dependencies:
        return "dependencies: []\n"
    lines = ["dependencies:\n"]
    for dependency in dependencies:
        lines.append(f"  - path: {json.dumps(dependency['path'], ensure_ascii=False)}\n")
        lines.append(f"    kind: {json.dumps(dependency['kind'], ensure_ascii=False)}\n")
        if "condition" in dependency:
            lines.append(
                f"    condition: {json.dumps(dependency['condition'], ensure_ascii=False)}\n"
            )
    return "".join(lines)


def replace_dependency_block(raw_front_matter: str, dependencies: list[dict[str, Any]]) -> str:
    lines = remove_top_level_blocks(
        raw_front_matter.splitlines(keepends=True), {"dependencies", "usage_dependencies"}
    )
    insertion = len(lines)
    for index, line in enumerate(lines):
        if top_level_key(line) in {"examples", "version"}:
            insertion = index
            break
    rendered = render_dependencies(dependencies)
    prefix = "".join(lines[:insertion])
    if prefix and not prefix.endswith("\n"):
        prefix += "\n"
    return prefix + rendered + "".join(lines[insertion:])


def comparable_document(text: str) -> tuple[dict[str, Any], str]:
    data, _, body = split_front_matter(text)
    comparable = deepcopy(data)
    comparable.pop("dependencies", None)
    comparable.pop("usage_dependencies", None)
    return comparable, body


def load_existing_requires_graph(root: Path) -> dict[str, set[str]]:
    graph: dict[str, set[str]] = {}
    api_root = root / API_ROOT
    if not api_root.is_dir():
        return graph
    for document in api_root.rglob("*.md"):
        relative = document.relative_to(root).as_posix()
        try:
            data, _, _ = split_front_matter(document.read_text(encoding="utf-8", errors="replace"))
        except ValidationError:
            continue
        entries = data.get("dependencies")
        if not isinstance(entries, list):
            continue
        targets: set[str] = set()
        for entry in entries:
            if not isinstance(entry, dict) or entry.get("kind") != "requires":
                continue
            path = entry.get("path")
            if not isinstance(path, str) or not valid_api_document_path(path):
                continue
            try:
                ensure_inside(root, path, "existing dependencies.path")
            except ValidationError:
                continue
            targets.add(path)
        if targets:
            graph[relative] = targets
    return graph


def validate_required_cycles(
    root: Path, replacements: dict[str, list[dict[str, Any]]]
) -> None:
    graph = load_existing_requires_graph(root)
    for document_path, dependencies in replacements.items():
        graph[document_path] = {
            dependency["path"] for dependency in dependencies if dependency["kind"] == "requires"
        }
    state: dict[str, int] = {}
    stack: list[str] = []

    def visit(node: str) -> None:
        state[node] = 1
        stack.append(node)
        for target in sorted(graph.get(node, ())):
            if state.get(target, 0) == 1:
                start = stack.index(target)
                cycle = " -> ".join(stack[start:] + [target])
                raise ValidationError("required dependency cycle: " + cycle)
            if state.get(target, 0) == 0:
                visit(target)
        stack.pop()
        state[node] = 2

    for node in sorted(graph):
        if state.get(node, 0) == 0:
            visit(node)


def prepare_writes(
    annotations: list[dict[str, Any]], root: Path
) -> list[tuple[Path, str, str, list[dict[str, Any]], bool]]:
    prepared: list[tuple[Path, str, str, list[dict[str, Any]], bool]] = []
    replacements: dict[str, list[dict[str, Any]]] = {}
    for index, annotation in enumerate(annotations):
        location = f"annotations[{index}]"
        unknown = sorted(set(annotation) - {"document_path", "dependencies"})
        if unknown:
            raise ValidationError(f"{location} has unknown fields: {', '.join(unknown)}")
        document_path = annotation.get("document_path")
        if not isinstance(document_path, str) or not valid_api_document_path(document_path):
            raise ValidationError(f"{location}.document_path is outside the supported API families")
        document = ensure_inside(root, document_path, location + ".document_path")
        original = document.read_text(encoding="utf-8", errors="replace")
        _, raw_front_matter, body = split_front_matter(original)
        dependencies = validate_dependencies(
            annotation.get("dependencies"), location + ".dependencies", root, document_path
        )
        updated_front_matter = replace_dependency_block(raw_front_matter, dependencies)
        if not updated_front_matter.endswith("\n"):
            updated_front_matter += "\n"
        updated = "---\n" + updated_front_matter + "---\n" + body
        before_comparable = comparable_document(original)
        after_comparable = comparable_document(updated)
        if before_comparable != after_comparable:
            raise ValidationError(f"dependency preparation would change non-dependency content: {document_path}")
        after_data, _, _ = split_front_matter(updated)
        expected_yaml = [
            {key: value for key, value in dependency.items() if key != "evidence"}
            for dependency in dependencies
        ]
        if after_data.get("dependencies") != expected_yaml or "usage_dependencies" in after_data:
            raise ValidationError(f"dependency replacement failed for {document_path}")
        if document_path in replacements:
            raise ValidationError(f"duplicate annotation path: {document_path}")
        replacements[document_path] = dependencies
        prepared.append(
            (
                document,
                original,
                updated,
                dependencies,
                "usage_dependencies" in split_front_matter(original)[0],
            )
        )
    validate_required_cycles(root, replacements)
    return prepared


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--devkit-root", type=Path, required=True)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--write", action="store_true", help="Write only after validation succeeds")
    args = parser.parse_args()

    try:
        root = args.devkit_root.expanduser().resolve()
        if not root.is_dir():
            raise ValidationError(f"DevKit root does not exist: {root}")
        annotations = load_artifact(args.input.expanduser().resolve())
        prepared = prepare_writes(annotations, root)
        if args.write:
            for document, original, updated, _, _ in prepared:
                if document.read_text(encoding="utf-8", errors="replace") != original:
                    raise ValidationError(f"document changed after dry-run: {document}")
                document.write_text(updated, encoding="utf-8")
        result = {
            "validated_annotations": len(prepared),
            "written": args.write,
            "legacy_usage_dependencies_replaced": sum(item[4] for item in prepared),
            "documents": [str(item[0]) for item in prepared],
            "dependency_counts": {
                str(item[0]): len(item[3]) for item in prepared
            },
        }
        print(json.dumps(result, ensure_ascii=False, indent=2))
        return 0
    except (OSError, ValidationError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
