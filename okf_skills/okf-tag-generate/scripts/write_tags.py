#!/usr/bin/env python3
"""Validate and write evidence-backed, namespaced OKF tag blocks."""

from __future__ import annotations

import argparse
from pathlib import Path
import json
import re
import sys
from typing import Any

import yaml


SKILL_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_TAXONOMY = SKILL_ROOT / "taxonomy" / "releases" / "core-v9-frozen.json"
API_ROOT = Path("docs/zh/api/SIMD-API")
API_SURFACES = {"c_api", "basic_api", "adv_api", "tensor_api"}
TOP_LEVEL_KEY_RE = re.compile(r"^[^\s#][^:]*:\s*(?:.*)?(?:\n|$)")
TAG_KEY_RE = re.compile(r"^tag:[^\n]*(?:\n|$)")
SAFE_VALUE_RE = re.compile(r"^[a-z0-9_]+$")


class ValidationError(ValueError):
    """Raised when an annotation artifact cannot be safely written."""


def load_taxonomy(path: Path) -> tuple[tuple[str, ...], dict[str, set[str]], dict[str, Any]]:
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise ValidationError(f"cannot read taxonomy: {path}: {exc}") from exc
    if not isinstance(payload, dict):
        raise ValidationError(f"taxonomy must be a JSON object: {path}")
    if payload.get("status") != "frozen":
        raise ValidationError(f"taxonomy is not frozen: {path}")
    namespaces = payload.get("namespaces")
    order = payload.get("namespace_order")
    if not isinstance(namespaces, dict) or not isinstance(order, list) or not order:
        raise ValidationError(f"taxonomy has invalid namespace metadata: {path}")
    if len(order) != len(set(order)) or any(not isinstance(item, str) for item in order):
        raise ValidationError("taxonomy namespace_order must be a unique string list")
    values: dict[str, set[str]] = {}
    for namespace in order:
        definition = namespaces.get(namespace)
        options = definition.get("values") if isinstance(definition, dict) else None
        if not isinstance(options, dict) or any(not isinstance(value, str) for value in options):
            raise ValidationError(f"taxonomy has invalid values for namespace: {namespace}")
        values[namespace] = set(options)
    if set(namespaces) != set(order):
        raise ValidationError("taxonomy namespaces differ from namespace_order")
    return tuple(namespaces), values, payload


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


def valid_relative_api_path(path: str) -> bool:
    candidate = Path(path)
    if candidate.is_absolute() or ".." in candidate.parts or candidate.suffix != ".md":
        return False
    parts = candidate.parts
    try:
        root_index = parts.index("SIMD-API")
    except ValueError:
        return False
    return (
        candidate.as_posix().startswith(API_ROOT.as_posix() + "/")
        and len(parts) > root_index + 1
        and parts[root_index + 1] in API_SURFACES
    )


def selected_keys(tags: dict[str, list[str]]) -> set[str]:
    return {f"{namespace}:{value}" for namespace, values in tags.items() for value in values}


def validate_annotation(
    annotation: Any,
    index: int,
    devkit_root: Path,
    namespaces: tuple[str, ...],
    values_by_namespace: dict[str, set[str]],
    taxonomy_payload: dict[str, Any],
) -> tuple[Path, dict[str, list[str]]]:
    location = f"annotations[{index}]"
    if not isinstance(annotation, dict):
        raise ValidationError(f"{location} must be an object")
    allowed = {"path", "tag", "evidence", "confidence", "proposed_tags"}
    unknown = sorted(set(annotation) - allowed)
    if unknown:
        raise ValidationError(f"{location} has unknown fields: {', '.join(unknown)}")
    path = annotation.get("path")
    if not isinstance(path, str) or not valid_relative_api_path(path):
        raise ValidationError(f"{location}.path is outside the supported API Markdown families")
    document = devkit_root / path
    if not document.is_file():
        raise ValidationError(f"{location}.path does not exist: {path}")
    text = document.read_text(encoding="utf-8")
    split_front_matter(text)

    if annotation.get("proposed_tags", []) != []:
        raise ValidationError(f"{location}.proposed_tags must be empty before writing")
    confidence = annotation.get("confidence")
    if confidence is not None and (
        isinstance(confidence, bool) or not isinstance(confidence, (int, float)) or not 0 <= confidence <= 1
    ):
        raise ValidationError(f"{location}.confidence must be a number between 0 and 1")

    tags = annotation.get("tag")
    if not isinstance(tags, dict) or not set(tags).issubset(set(namespaces)):
        raise ValidationError(f"{location}.tag must contain only frozen namespaces")
    missing = set(taxonomy_payload.get("mandatory_namespaces", ["api_surfaces", "roles"])) - set(tags)
    if missing:
        raise ValidationError(f"{location}.tag is missing required namespaces: {sorted(missing)}")
    normalized: dict[str, list[str]] = {}
    for namespace in tags:
        raw_values = tags[namespace]
        if not isinstance(raw_values, list) or not raw_values or any(
            not isinstance(value, str) or not value for value in raw_values
        ):
            raise ValidationError(f"{location}.tag.{namespace} must be a non-empty string list")
        if len(raw_values) != len(set(raw_values)):
            raise ValidationError(f"{location}.tag.{namespace} contains duplicate values")
        unknown_values = sorted(set(raw_values) - values_by_namespace[namespace])
        if unknown_values:
            raise ValidationError(
                f"{location}.tag.{namespace} contains values outside the frozen taxonomy: "
                + ", ".join(unknown_values)
            )
        normalized[namespace] = sorted(raw_values)

    evidence = annotation.get("evidence")
    if not isinstance(evidence, dict):
        raise ValidationError(f"{location}.evidence must be an object")
    expected_evidence = selected_keys(normalized)
    if set(evidence) != expected_evidence:
        missing = sorted(expected_evidence - set(evidence))
        extra = sorted(set(evidence) - expected_evidence)
        details = []
        if missing:
            details.append("missing " + ", ".join(missing))
        if extra:
            details.append("unknown " + ", ".join(extra))
        raise ValidationError(f"{location}.evidence keys do not match selected tags ({'; '.join(details)})")
    for key, excerpt in evidence.items():
        if not isinstance(excerpt, str) or not excerpt.strip():
            raise ValidationError(f"{location}.evidence.{key} must be a non-empty string")
        namespace, value = key.split(":", 1)
        if namespace == "api_surfaces":
            if excerpt != path or not valid_relative_api_path(excerpt):
                raise ValidationError(f"{location}.evidence.{key} must be the stable resource path")
        elif excerpt not in text:
            raise ValidationError(f"{location}.evidence.{key} is not a verbatim excerpt from the document")
        if namespace == "traits" and value == "deprecated" and not re.search(
            r"\bdeprecated\b|废弃", excerpt, re.IGNORECASE
        ):
            raise ValidationError(f"{location}.evidence.{key} must explicitly state deprecation")
    roles = set(normalized.get("roles", []))
    if roles & {"compute", "data_movement", "data_initialization"} and "operations" not in normalized:
        raise ValidationError(f"{location}.tag.operations is required for executable roles")
    if "compute" in roles and not normalized.get("execution_models"):
        raise ValidationError(f"{location}.tag.execution_models is required for compute APIs")
    return document, normalized


def load_annotations(path: Path) -> list[Any]:
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise ValidationError(f"cannot read annotation artifact: {path}: {exc}") from exc
    if not isinstance(payload, dict) or payload.get("schema_version") != 1:
        raise ValidationError("annotation artifact must have schema_version 1")
    annotations = payload.get("annotations")
    if not isinstance(annotations, list) or not annotations:
        raise ValidationError("annotation artifact must contain a non-empty annotations list")
    return annotations


def render_tag_block(tags: dict[str, list[str]], namespaces: tuple[str, ...]) -> str:
    lines = ["tag:"]
    for namespace in namespaces:
        if namespace not in tags:
            continue
        values = tags[namespace]
        if any(not SAFE_VALUE_RE.fullmatch(value) for value in values):
            raise ValidationError(f"tag value cannot be rendered safely: {namespace}")
        lines.append(f"  - {namespace}: [{', '.join(values)}]")
    return "\n".join(lines) + "\n"


def replace_tag_block(raw_front_matter: str, tag_block: str) -> str:
    lines = raw_front_matter.splitlines(keepends=True)

    def join_with_block(prefix_lines: list[str], suffix_lines: list[str]) -> str:
        prefix = "".join(prefix_lines)
        if prefix and not prefix.endswith("\n"):
            prefix += "\n"
        return prefix + tag_block + "".join(suffix_lines)

    tag_index: int | None = None
    for index, line in enumerate(lines):
        if TAG_KEY_RE.match(line):
            tag_index = index
            break

    if tag_index is None:
        insert_at = len(lines)
        for index, line in enumerate(lines):
            if TOP_LEVEL_KEY_RE.match(line) and line.split(":", 1)[0] in {
                "signature", "dependencies", "examples", "timestamp"
            }:
                insert_at = index
                break
        return join_with_block(lines[:insert_at], lines[insert_at:])

    end = tag_index + 1
    while end < len(lines):
        line = lines[end]
        if TOP_LEVEL_KEY_RE.match(line) and not line.startswith((" ", "\t")):
            break
        end += 1
    return join_with_block(lines[:tag_index], lines[end:])


def prepare_writes(
    annotations: list[Any],
    devkit_root: Path,
    namespaces: tuple[str, ...],
    values_by_namespace: dict[str, set[str]],
    taxonomy_payload: dict[str, Any],
) -> list[tuple[Path, str, str]]:
    prepared: list[tuple[Path, str, str]] = []
    seen: set[str] = set()
    for index, annotation in enumerate(annotations):
        path = annotation.get("path") if isinstance(annotation, dict) else None
        if isinstance(path, str) and path in seen:
            raise ValidationError(f"duplicate annotation path: {path}")
        if isinstance(path, str):
            seen.add(path)
        document, tags = validate_annotation(
            annotation, index, devkit_root, namespaces, values_by_namespace, taxonomy_payload
        )
        original = document.read_text(encoding="utf-8")
        _, raw_front_matter, body = split_front_matter(original)
        updated_front_matter = replace_tag_block(
            raw_front_matter, render_tag_block(tags, namespaces)
        )
        if not updated_front_matter.endswith("\n"):
            updated_front_matter += "\n"
        updated = "---\n" + updated_front_matter + "---\n" + body
        before_data, _, before_body = split_front_matter(original)
        after_data, _, after_body = split_front_matter(updated)
        before_data.pop("tag", None)
        after_data.pop("tag", None)
        if before_data != after_data or before_body != after_body:
            raise ValidationError(f"tag preparation would change non-tag content: {document}")
        prepared.append((document, original, updated))
    return prepared


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--devkit-root", type=Path, required=True)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--taxonomy", type=Path, default=DEFAULT_TAXONOMY)
    parser.add_argument("--write", action="store_true", help="Write only after validation succeeds")
    args = parser.parse_args()

    try:
        devkit_root = args.devkit_root.expanduser().resolve()
        if not devkit_root.is_dir():
            raise ValidationError(f"devkit root does not exist: {devkit_root}")
        namespaces, values_by_namespace, taxonomy_payload = load_taxonomy(args.taxonomy.expanduser().resolve())
        annotations = load_annotations(args.input.expanduser().resolve())
        prepared = prepare_writes(annotations, devkit_root, namespaces, values_by_namespace, taxonomy_payload)
        if args.write:
            for document, original, updated in prepared:
                if document.read_text(encoding="utf-8") != original:
                    raise ValidationError(f"document changed after dry-run: {document}")
                document.write_text(updated, encoding="utf-8")
        result = {
            "validated_annotations": len(prepared),
            "taxonomy": str(args.taxonomy.expanduser().resolve()),
            "written": args.write,
            "documents": [str(document) for document, _, _ in prepared],
        }
        print(json.dumps(result, ensure_ascii=False, indent=2))
        return 0
    except (OSError, ValidationError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
