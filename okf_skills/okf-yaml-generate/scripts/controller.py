#!/usr/bin/env python3
"""Self-contained OKF YAML orchestration, validation, reporting, and writing."""

from __future__ import annotations

import argparse
from copy import deepcopy
from dataclasses import asdict, replace
from datetime import date
from contextlib import contextmanager
import fcntl
import hashlib
import json
from functools import lru_cache
from pathlib import Path
import re
import sys
from typing import Any

import yaml


SCRIPTS_DIR = Path(__file__).resolve().parent
if str(SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPTS_DIR))

from example_snippet_markers import apply_insertions_to_text, candidate_insertions_for_documents
from doxygen_annotations import (
    DoxygenValidationError,
    apply_records_to_text,
    validate_record_target,
)
from extract_api_yaml_metadata import (
    _compare_signature_header_mappings,
    _is_markdown_table_separator,
    _macro_definitions,
    _markdown_table_cells,
    _normalized_parameter_list,
    _parameter_name,
    _parse_prototype_declaration,
    _resource_declares_symbol,
    build_example_discovery_index,
    extract_api_metadata,
    extract_example_references,
    extract_prototype_declarations,
    resolve_signature_header_mappings,
    supported_archs,
)
from prototype_id import (
    MACRO_ID_RE,
    PrototypeIdError,
    architecture_override_id,
    signature_identifier,
)


API_ROOT = Path("docs/zh/api/SIMD-API")
API_SURFACES = {"c_api", "basic_api", "adv_api", "tensor_api"}
OKF_TYPES = {
    "reference.api.cpp", "reference.arch.cpp", "reference.index.cpp", "reference.type.cpp",
}
ALLOWED_FIELDS = (
    "okftype", "title", "resource", "description", "tag", "signature",
    "dependencies", "examples", "headers", "declaration",
    "type_catalog", "timestamp",
)
DEFINITION_CATEGORIES = {"constant", "enum", "macro", "type", "union"}
DEFINITIONS_ROOT = API_ROOT / "c_api" / "defs"
MACRO_DEFINITION_SECTION_RE = re.compile(r"^\s{0,3}#{2,6}\s+宏定义(?:\s|<|$)", re.MULTILINE)
DEPENDENCY_KINDS = {"requires", "configures", "constrained_by", "related"}
SIGNATURE_KEYS = {"prototype_id", "override_id", "macro_id", "archs", "headers", "dtype"}
DEFAULT_WRITE_TARGETS = (
    "front_matter", "tag", "dependencies", "include_doxygen", "example_markers",
)
ISSUE_TYPES = {"document_defect", "source_defect", "tool_defect", "unresolved"}
RESOLUTIONS = {
    "blank_field", "report_source_defect", "report_unresolved", "tool_change_required",
}
TOP_KEY_RE = re.compile(r"^(?P<key>[A-Za-z_][A-Za-z0-9_-]*):(?:\s.*)?(?:\n|$)")
INLINE_API_SIGNATURE_BLOCK_RE = re.compile(
    r"(?ms)^```(?:yaml|yml)[ \t]*\n(?P<payload>.*?)^```[ \t]*(?:\n\n|\n|\Z)"
)
ARCHITECTURE_DOCUMENT_STEM_RE = re.compile(
    r"^(?P<family>.+)_arch_(?P<architecture>[0-9]+)$", re.IGNORECASE
)
ARCHITECTURE_DOCUMENT_TITLE_RE = re.compile(
    r"^(?P<family>.+?)(?:[_\s-]+arch)?[_\s-]+(?P<architecture>[0-9]{4,5})$",
    re.IGNORECASE,
)
PARAMETER_SECTION_RE = re.compile(r"^\s{0,3}##\s+参数说明(?:\s|[<（(]|$)")
FUNCTION_PROTOTYPE_SECTION_RE = re.compile(
    r"^\s{0,3}#{2,6}\s+函数(?:原型|声明)(?:\s|[<（(]|$)", re.MULTILINE
)
ARCH_DOCUMENT_NAME_RE = re.compile(r"_arch_\d+(?:_deprecated)?\.md$")
LEVEL_TWO_HEADING_RE = re.compile(r"^\s{0,3}##(?!#)\s+")
# Keep this aligned with api-yaml-metadata-guard/scripts/prototype_id.py.
PLACEHOLDER_RE = re.compile(r"<(?P<name>[A-Za-z_][A-Za-z0-9_]*)>")
TEMPLATE_VARIABLE_RE = re.compile(r"[STU](?:[0-9]+)?$")
VERIFIED_HEADER_STATUSES = {
    "hint_scope_single",
    "scope_include_single",
    "fallback_single",
}
UNSAFE_HEADER_COMPARISONS = {
    "api_not_found_in_header",
    "not_comparable",
    "parameter_name_difference",
    "parameter_type_mismatch",
    "parameter_count_mismatch",
}


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


def is_definition_index(root: Path, path: Path) -> bool:
    relative = Path(repo_relative(root, path))
    prefix = DEFINITIONS_ROOT.parts
    if relative.parts[: len(prefix)] != prefix:
        return False
    tail = relative.parts[len(prefix) :]
    if len(tail) == 1:
        return relative.stem == "defs"
    return len(tail) == 2 and relative.stem == tail[0]


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
        selected = [
            repo_relative(root, path)
            for path in directory.rglob("*.md")
            if not is_definition_index(root, path)
        ]
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


TAG_RULES_PATH = Path(__file__).resolve().parents[1] / "config" / "tag-rules.yaml"
SOURCE_CONTEXT_PATH = Path(__file__).resolve().parents[1] / "config" / "source-context-v1.yaml"


@lru_cache(maxsize=1)
def load_source_context() -> dict[str, Any]:
    """Load the source-context path-injection config (config/source-context-v1.yaml)."""
    try:
        payload = yaml.safe_load(SOURCE_CONTEXT_PATH.read_text(encoding="utf-8"))
    except (OSError, yaml.YAMLError) as error:
        raise ValidationError(f"cannot read source context: {SOURCE_CONTEXT_PATH}: {error}") from error
    if not isinstance(payload, dict) or payload.get("schema_version") != 1:
        raise ValidationError("source context schema is invalid")
    default_tags = payload.get("default_tags")
    if not isinstance(default_tags, dict) or not default_tags:
        raise ValidationError("source context default_tags is invalid")
    for prefix, tags in default_tags.items():
        if not isinstance(prefix, str) or not isinstance(tags, dict) or not tags:
            raise ValidationError("source context default_tags entries are invalid")
        for namespace, values in tags.items():
            if not isinstance(values, list) or not values:
                raise ValidationError(f"source context default_tags.{prefix}.{namespace} must be a non-empty list")
    return payload


def source_context_default_tags(path: str) -> dict[str, list[str]]:
    """Longest-prefix path-proven tags for a repository-relative document path."""
    matched: dict[str, list[str]] = {}
    best = ""
    for prefix, tags in load_source_context()["default_tags"].items():
        if path.startswith(prefix) and len(prefix) > len(best):
            best = prefix
            matched = tags
    return matched if best else {}


@lru_cache(maxsize=1)
def load_tag_rules() -> dict[str, Any]:
    """Load the machine-readable mandatory/conditional tag rules (config/tag-rules.yaml)."""
    try:
        payload = yaml.safe_load(TAG_RULES_PATH.read_text(encoding="utf-8"))
    except (OSError, yaml.YAMLError) as error:
        raise ValidationError(f"cannot read tag rules: {TAG_RULES_PATH}: {error}") from error
    if not isinstance(payload, dict) or payload.get("schema_version") != 1:
        raise ValidationError("tag rules schema is invalid")
    required = payload.get("required_namespaces")
    if not isinstance(required, dict):
        raise ValidationError("tag rules required_namespaces is invalid")
    always = required.get("always")
    if not isinstance(always, list) or not always or not all(isinstance(item, str) for item in always):
        raise ValidationError("tag rules required_namespaces.always must be a non-empty string list")
    conditional = required.get("conditional", {})
    if not isinstance(conditional, dict):
        raise ValidationError("tag rules required_namespaces.conditional is invalid")
    for namespace, condition in conditional.items():
        when = condition.get("when_roles_in") if isinstance(condition, dict) else None
        if not isinstance(when, list) or not when or not all(isinstance(item, str) for item in when):
            raise ValidationError(f"tag rules conditional.{namespace}.when_roles_in must be a non-empty string list")
    return payload


def taxonomy(path: Path) -> tuple[tuple[str, ...], dict[str, set[str]]]:
    payload = json.loads(path.read_text(encoding="utf-8"))
    if payload.get("status") != "frozen":
        raise ValidationError("taxonomy is not frozen")
    order = payload.get("namespace_order")
    defs = payload.get("namespaces")
    namespaces = list(defs) if isinstance(defs, dict) else None
    if not isinstance(order, list) or not isinstance(namespaces, list) or not isinstance(defs, dict):
        raise ValidationError("taxonomy metadata is invalid")
    if set(order) != set(namespaces):
        raise ValidationError("taxonomy namespace_order does not match namespace keys")
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


def _tag_evidence_excerpt(body: str) -> str:
    """Return a stable source excerpt that supports retained document tags."""
    section = re.search(
        r"(?ms)^\s{0,3}##\s+功能说明(?:\s|$)(.*?)(?=^\s{0,3}##\s+|\Z)",
        body,
    )
    candidates = (section.group(1) if section else body).splitlines()
    in_code = False
    for line in candidates:
        stripped = line.strip()
        if stripped.startswith("```"):
            in_code = not in_code
            continue
        if (
            not in_code
            and stripped
            and not stripped.startswith(("#", "|", "<!--", "- "))
            and not stripped.endswith("：")
        ):
            return stripped
    heading = first_heading(body, "")
    return f"# {heading}" if heading else "# API"


def _retained_tag_evidence(
    path: str, body: str, tags: list[dict[str, list[str]]],
) -> dict[str, list[dict[str, str]]]:
    """Attach verifiable source evidence to non-empty YAML tags retained by a worker."""
    excerpt = _tag_evidence_excerpt(body)
    evidence: dict[str, list[dict[str, str]]] = {}
    for item in tags:
        namespace, selected = next(iter(item.items()))
        for value in selected:
            # API surface is established by the scoped API document path.
            source_excerpt = path if namespace == "api_surfaces" else excerpt
            evidence[f"{namespace}:{value}"] = [{
                "source_path": path,
                "excerpt": source_excerpt,
            }]
    return evidence


def _dependency_evidence_excerpt(
    body: str, target_path: str, condition: Any,
) -> str:
    """Prefer the source line that links a retained dependency target."""
    needles = [Path(target_path).name, Path(target_path).stem]
    if isinstance(condition, str) and condition.strip():
        needles.append(condition.strip())
    for needle in needles:
        for line in body.splitlines():
            if needle and needle in line:
                return line.strip()
    return _tag_evidence_excerpt(body)


def _retained_dependencies_with_evidence(
    path: str, body: str, dependencies: Any,
) -> Any:
    """Add run-local evidence to previously rendered dependency entries."""
    if not isinstance(dependencies, list):
        return dependencies
    result = []
    for item in dependencies:
        if not isinstance(item, dict):
            result.append(item)
            continue
        output = deepcopy(item)
        if not isinstance(output.get("evidence"), dict):
            target_path = output.get("path")
            if isinstance(target_path, str):
                output["evidence"] = {
                    "source_path": path,
                    "excerpt": _dependency_evidence_excerpt(
                        body, target_path, output.get("condition"),
                    ),
                }
        result.append(output)
    return result


def _parameter_direction(value: str) -> str | None:
    normalized = re.sub(r"\s+", "", value)
    if "输入" in normalized and "输出" in normalized:
        return "in,out"
    if "输入" in normalized and "输出" not in normalized:
        return "in"
    if "输出" in normalized and "输入" not in normalized:
        return "out"
    return None


def _parameter_documentation_tables(markdown: str) -> list[dict[str, dict[str, str]]]:
    """Read source-proven Doxygen parameter metadata from ``## 参数说明`` tables."""
    lines = markdown.splitlines()
    start = next((index + 1 for index, line in enumerate(lines) if PARAMETER_SECTION_RE.match(line)), None)
    if start is None:
        return []
    tables: list[dict[str, dict[str, str]]] = []
    for index in range(start, len(lines) - 1):
        if index > start and LEVEL_TWO_HEADING_RE.match(lines[index]):
            break
        headers = _markdown_table_cells(lines[index])
        separators = _markdown_table_cells(lines[index + 1])
        if (
            not headers
            or len(headers) != len(separators)
            or not _is_markdown_table_separator(separators)
        ):
            continue
        normalized_headers = [re.sub(r"\s+", "", header) for header in headers]
        name_index = next(
            (position for position, header in enumerate(normalized_headers) if header in {"参数名", "参数", "名称"}),
            None,
        )
        direction_index = next(
            (position for position, header in enumerate(normalized_headers) if header in {"输入/输出", "输入输出", "方向"}),
            None,
        )
        description_index = next(
            (position for position, header in enumerate(normalized_headers) if "描述" in header or "说明" in header),
            None,
        )
        if name_index is None or direction_index is None or description_index is None:
            continue
        entries: dict[str, dict[str, str]] = {}
        for row_index in range(index + 2, len(lines)):
            cells = _markdown_table_cells(lines[row_index])
            if not cells:
                break
            if len(cells) < len(headers):
                break
            name = cells[name_index].strip()
            direction = _parameter_direction(cells[direction_index])
            description = cells[description_index].strip()
            if not name or not description:
                continue
            entries.setdefault(name, {
                "direction": direction or "",
                "description": description,
                "evidence": lines[row_index].strip(),
            })
        if entries:
            tables.append(entries)
    return tables


def _signature_parameter_names(signature: str) -> tuple[str, ...]:
    declaration = _parse_prototype_declaration(signature) or _parse_prototype_declaration(
        f"__aicore__ inline void {signature};"
    )
    if declaration is None:
        return ()
    return tuple(
        name
        for parameter in _normalized_parameter_list(
            declaration.parameters or "", keep_names=True
        )
        if (name := _parameter_name(parameter))
    )


def _doxygen_params_from_markdown(
    signature: str,
    tables: list[dict[str, dict[str, str]]],
) -> tuple[list[dict[str, Any]], str | None]:
    names = _signature_parameter_names(signature)
    if not names:
        return [], None
    candidates = [table for table in tables if any(name in table for name in names)]
    if not candidates:
        return [], "参数说明表未完整覆盖该声明的命名参数。"
    selected = min(
        candidates,
        key=lambda table: (
            -sum(name in table for name in names),
            len(table) - sum(name in table for name in names),
            tuple(sorted(table)),
        ),
    )
    params = []
    problems = []
    for name in names:
        item = selected.get(name)
        if item is None:
            problems.append(f"参数说明表未覆盖参数 {name}。")
            continue
        if not item["direction"]:
            problems.append(f"参数 {name} 的输入/输出方向无法映射为 in、out 或 in,out。")
            continue
        params.append({
            "name": name,
            "direction": item["direction"],
            "description": item["description"],
            "evidence": [item["evidence"]],
        })
    return params, "；".join(problems) if problems else None


def _architecture_document_identity(document: Path, body: str) -> tuple[str, str] | None:
    """Return the API-family and architecture advertised by an arch-specific document."""
    match = ARCHITECTURE_DOCUMENT_STEM_RE.fullmatch(document.stem)
    if match is None:
        match = ARCHITECTURE_DOCUMENT_TITLE_RE.fullmatch(first_heading(body, document.stem))
    if match is None:
        return None
    family = re.sub(r"[_\s-]+", "_", match.group("family").strip()).casefold()
    architecture = match.group("architecture")
    return (family, architecture) if family else None


def _prototype_override_ids(markdown: str, source: str) -> set[str]:
    """Extract concrete IDs without requiring header or example resolution."""
    identifiers: set[str] = set()
    for declaration in extract_prototype_declarations(markdown):
        if not declaration.prototype_function:
            continue
        try:
            key, identifier = signature_identifier(
                declaration.prototype_function,
                source=source,
                template_parameters=declaration.template_parameters,
            )
        except PrototypeIdError:
            continue
        if key == "override_id":
            identifiers.add(identifier)
    return identifiers


def _architecture_override_suffixes(
    root: Path,
    document: Path,
    body: str,
    signature: list[dict[str, Any]],
) -> dict[str, str]:
    """Suffix only concrete IDs duplicated by sibling architecture documents.

    Architecture-specific documents conventionally end in ``_arch_<number>``.
    The sibling scan keeps a single-document run deterministic while avoiding a
    suffix on overloads that are not shared by another architecture document.
    """
    identity = _architecture_document_identity(document, body)
    if identity is None:
        return {}
    family, architecture = identity
    local_ids = {
        item["override_id"]
        for item in signature
        if isinstance(item, dict) and isinstance(item.get("override_id"), str)
    }
    if not local_ids:
        return {}

    occurrences: dict[str, set[Path]] = {}
    for sibling in sorted(document.parent.glob("*.md")):
        sibling_text = sibling.read_text(encoding="utf-8", errors="replace")
        _, _, sibling_body = split_front_matter(sibling_text)
        sibling_identity = _architecture_document_identity(sibling, sibling_body)
        if sibling_identity is None or sibling_identity[0] != family:
            continue
        if sibling.resolve() == document.resolve():
            identifiers = local_ids
        else:
            identifiers = _prototype_override_ids(
                sibling_body, repo_relative(root, sibling)
            )
        for identifier in identifiers:
            if identifier in local_ids:
                occurrences.setdefault(identifier, set()).add(sibling.resolve())
    return {
        identifier: architecture_override_id(identifier, architecture)
        for identifier, documents in occurrences.items()
        if len(documents) > 1
    }


def _marker_call_override_id(call: dict[str, Any]) -> str | None:
    function = call.get("canonical_id") or call.get("call_template")
    if not isinstance(function, str) or not function:
        return None
    for binding in call.get("placeholder_bindings") or ():
        if isinstance(binding, dict) and "name" in binding and "value" in binding:
            function = function.replace(f"<{binding['name']}>", str(binding["value"]))
    if PLACEHOLDER_RE.search(function):
        return None
    try:
        key, identifier = signature_identifier(function)
    except PrototypeIdError:
        return None
    return identifier if key == "override_id" else None


def _apply_architecture_override_suffixes(
    fields: dict[str, Any],
    doxygen_records: list[dict[str, Any]],
    marker_examples: list[dict[str, Any]],
    suffixes: dict[str, str],
) -> None:
    """Apply derived IDs to YAML and pass them to annotation planning records."""
    if not suffixes:
        return
    for signature in fields.get("signature", []):
        if isinstance(signature, dict) and signature.get("override_id") in suffixes:
            signature["override_id"] = suffixes[signature["override_id"]]
    for example in fields.get("examples", []):
        if isinstance(example, dict) and example.get("override_id") in suffixes:
            example["override_id"] = suffixes[example["override_id"]]
    for record in doxygen_records:
        function = record.get("function")
        templates = record.get("template_parameters", [])
        if not isinstance(function, str) or not isinstance(templates, list):
            continue
        try:
            key, identifier = signature_identifier(
                function, template_parameters=tuple(templates)
            )
        except PrototypeIdError:
            continue
        if key == "override_id" and identifier in suffixes:
            record["override_id"] = suffixes[identifier]
    for example in marker_examples:
        calls = example.get("calls") if isinstance(example, dict) else None
        if not isinstance(calls, list):
            continue
        for call in calls:
            if not isinstance(call, dict):
                continue
            identifier = _marker_call_override_id(call)
            if identifier in suffixes:
                call["override_id"] = suffixes[identifier]


def signature_id_key(identifier: str) -> str:
    """Match the legacy generator's documented-placeholder discriminator."""
    has_documented_placeholder = any(
        TEMPLATE_VARIABLE_RE.fullmatch(name) is None
        for name in PLACEHOLDER_RE.findall(identifier)
    )
    return "prototype_id" if has_documented_placeholder else "override_id"


def dtype_values(value: Any) -> list[str]:
    """Normalize a legacy dtype representation to the canonical string array."""
    if isinstance(value, list):
        candidates = value
    elif isinstance(value, dict):
        candidates = value.get("values")
        if not isinstance(candidates, list):
            candidates = [
                item
                for variant in value.get("variants", [])
                if isinstance(variant, dict)
                for item in variant.values()
            ]
    else:
        candidates = []
    return list(dict.fromkeys(
        item for item in candidates if isinstance(item, str) and item.strip()
    ))


def source_signature_dtypes(markdown: str) -> dict[str, list[str]]:
    """Bind documented dtype values to the exact generic prototype that uses them."""
    values_by_signature: dict[str, list[str]] = {}
    for declaration in extract_prototype_declarations(markdown):
        function = declaration.prototype_function
        if not function:
            continue
        try:
            key, identifier = signature_identifier(
                function,
                template_parameters=declaration.template_parameters,
            )
        except PrototypeIdError:
            continue
        if key != "prototype_id" or "<dtype>" not in identifier:
            continue
        value = dict(declaration.placeholder_bindings).get("dtype")
        if not isinstance(value, str) or not value.strip():
            continue
        values_by_signature.setdefault(identifier, []).append(value)
    return {
        identifier: list(dict.fromkeys(values))
        for identifier, values in values_by_signature.items()
    }


def normalize_signature_dtypes(
    signature: Any,
    source_values: dict[str, list[str]],
    legacy_top_level_dtype: Any,
) -> Any:
    """Place dtype beside its prototype_id and remove obsolete root-level forms."""
    if not isinstance(signature, list):
        return signature
    legacy_top_level_values = dtype_values(legacy_top_level_dtype)
    normalized = []
    for item in signature:
        if not isinstance(item, dict):
            normalized.append(item)
            continue
        output = deepcopy(item)
        identifier = output.get("prototype_id")
        local_values = dtype_values(output.get("dtype"))
        if isinstance(identifier, str) and "<dtype>" in identifier:
            values = source_values.get(identifier) or local_values or legacy_top_level_values
            if values:
                output["dtype"] = values
            else:
                output.pop("dtype", None)
        else:
            output.pop("dtype", None)
        normalized.append(output)
    return normalized


def strip_inline_api_signature_blocks(body: str) -> str:
    """Remove only generated api_signature metadata fences from Markdown bodies."""
    def replacement(match: re.Match[str]) -> str:
        try:
            parsed = yaml.safe_load(match.group("payload"))
        except yaml.YAMLError:
            return match.group(0)
        api_signature = parsed.get("api_signature") if isinstance(parsed, dict) else None
        if isinstance(parsed, dict) and set(parsed) == {"api_signature"} and isinstance(api_signature, dict):
            return ""
        return match.group(0)

    return INLINE_API_SIGNATURE_BLOCK_RE.sub(replacement, body)


def _source_signature_fields(
    root: Path,
    document: Path,
    markdown: str,
    example_discovery_index: Any = None,
) -> tuple[
    list[dict[str, Any]],
    list[dict[str, Any]],
    list[dict[str, Any]],
    list[dict[str, Any]],
    list[dict[str, Any]],
]:
    """Derive only source-proven signatures, headers, and example calls.

    The parsing and header-resolution implementation is vendored from the
    legacy guard.  This controller intentionally publishes only a unique,
    in-scope header association; all other outcomes remain resolver issues.
    """
    try:
        mappings, _ = resolve_signature_header_mappings(document, markdown, root)
    except (PrototypeIdError, ValueError) as exc:
        return [], [], [], [], [issue(
            repo_relative(root, document),
            "signature",
            "document_defect",
            f"函数原型或头文件映射无法解析：{exc}",
        )]

    comparisons = _compare_signature_header_mappings(markdown, mappings, root)
    comparison_statuses: dict[str, set[str]] = {}
    for comparison in comparisons:
        comparison_statuses.setdefault(comparison.document_parameters, set()).add(comparison.status)
    document_archs = tuple(supported_archs(markdown))
    parameter_tables = _parameter_documentation_tables(markdown)

    signatures: list[dict[str, Any]] = []
    doxygen_records: list[dict[str, Any]] = []
    mapping_issues: list[dict[str, Any]] = []
    for mapping in mappings:
        if mapping.macro_id:
            entry = {"macro_id": mapping.macro_id}
        else:
            function = mapping.prototype_function or mapping.canonical_id
            try:
                key, identifier = signature_identifier(
                    function,
                    source=repo_relative(root, document),
                    template_parameters=mapping.template_parameters,
                )
            except PrototypeIdError as exc:
                mapping_issues.append(issue(
                    repo_relative(root, document),
                    "signature",
                    "tool_defect",
                    f"函数原型 ID 无法生成：{exc}",
                ))
                continue
            archs = sorted(dict.fromkeys(mapping.archs or document_archs))
            entry = {key: identifier, "archs": archs}
            statuses = comparison_statuses.get(mapping.document_parameters, set())
            verified = (
                mapping.status in VERIFIED_HEADER_STATUSES
                and not statuses.intersection(UNSAFE_HEADER_COMPARISONS)
            )
            entry["headers"] = sorted(dict.fromkeys(mapping.headers)) if verified else []
        if entry not in signatures:
            signatures.append(entry)
        if (
            not mapping.macro_id
            and mapping.override_id
            and mapping.status in VERIFIED_HEADER_STATUSES
            and len(mapping.headers) == 1
        ):
            params, params_problem = _doxygen_params_from_markdown(
                mapping.document_signature or mapping.canonical_id,
                parameter_tables,
            )
            if params_problem is not None:
                mapping_issues.append(issue(
                    repo_relative(root, document),
                    "include_doxygen",
                    "document_defect",
                    f"{mapping.name} 的 Doxygen 参数无法生成：{params_problem}",
                ))
            doxygen_records.append({
                "source_document": repo_relative(root, document),
                "header": mapping.headers[0].lstrip("/"),
                # A Markdown prototype may use documented placeholders and
                # therefore publish a prototype_id. Its matched declaration
                # is concrete, so include annotations use its override_id.
                "function": mapping.canonical_id,
                "parameters": mapping.document_signature or mapping.canonical_id,
                "override_id": mapping.override_id,
                "archs": archs,
                "template_parameters": [],
                "doxygen": {"params": params} if params else {},
            })
        elif mapping.macro_id and len(mapping.headers) == 1:
            doxygen_records.append({
                "source_document": repo_relative(root, document),
                "header": mapping.headers[0].lstrip("/"),
                "function": mapping.canonical_id,
                "parameters": mapping.document_signature or mapping.canonical_id,
                "macro_id": mapping.macro_id,
                "archs": [],
                "template_parameters": [],
                "doxygen": {},
            })
        if mapping.status not in VERIFIED_HEADER_STATUSES and mapping.status != "compiler_generated_macro":
            mapping_issues.append(issue(
                repo_relative(root, document),
                "signature.headers",
                "unresolved",
                f"{mapping.name} 的头文件映射未唯一确认（{mapping.status}），已保留空 headers。",
            ))

    examples: list[dict[str, Any]] = []
    marker_examples: list[dict[str, Any]] = []
    try:
        references = extract_example_references(
            markdown,
            document,
            mappings,
            supported_archs(markdown),
            root,
            example_discovery_index,
        )
    except (OSError, ValueError) as exc:
        mapping_issues.append(issue(
            repo_relative(root, document),
            "examples",
            "unresolved",
            f"样例关联无法解析：{exc}",
        ))
        return signatures, examples, marker_examples, doxygen_records, mapping_issues

    for reference in references:
        if (
            reference.confidence != "authoritative"
            or reference.issues
            or not reference.path
        ):
            continue
        if document_archs and (
            not reference.archs
            or not set(reference.archs).intersection(document_archs)
        ):
            continue
        confirmed_calls = tuple(
            call
            for call in reference.calls
            if call.overload_status == "exact"
            and isinstance(call.canonical_id, str)
            and isinstance(call.file, str)
            and call.is_statement
            and call.inferred_types is not None
            and all(isinstance(value, str) and value for value in call.inferred_types)
        )
        if not confirmed_calls:
            continue
        marker_examples.append(asdict(replace(
            reference,
            calls=confirmed_calls,
            issues=(),
        )))
        for call in confirmed_calls:
            function = call.canonical_id
            try:
                key, identifier = signature_identifier(function or "")
            except PrototypeIdError:
                continue
            if key != "override_id" or not call.file:
                continue
            file_name = call.file
            if file_name.startswith(f"{reference.path}/"):
                file_name = file_name[len(reference.path) + 1 :]
            entry = {
                "path": reference.path,
                "archs": list(reference.archs),
                "override_id": identifier,
                "file": file_name,
            }
            if entry not in examples:
                examples.append(entry)
    return signatures, examples, marker_examples, doxygen_records, mapping_issues


def _definition_category(root: Path, document: Path) -> str | None:
    """Return the concrete c_api/defs category, never a directory index page."""
    relative = Path(repo_relative(root, document))
    prefix = DEFINITIONS_ROOT.parts
    if relative.parts[: len(prefix)] != prefix or len(relative.parts) != len(prefix) + 2:
        return None
    category = relative.parts[len(prefix)]
    if category not in DEFINITION_CATEGORIES or relative.stem == category:
        return None
    return category


def _definition_header(
    root: Path,
    candidates: Any,
    symbol: str,
    path: str,
) -> tuple[list[str], list[dict[str, Any]]]:
    """Publish a definition header only when it uniquely declares the symbol."""
    resources = sorted(dict.fromkeys(
        resource for resource in candidates if isinstance(resource, str)
    )) if isinstance(candidates, (list, tuple)) else []
    if len(resources) != 1:
        return [], [issue(
            path,
            "headers",
            "unresolved",
            "定义的头文件映射未唯一确认，已保留空 headers。",
        )]
    resource = resources[0]
    if not _resource_declares_symbol(str(root), resource, symbol, None):
        return [], [issue(
            path,
            "headers",
            "source_defect",
            f"{symbol} 未在文档关联的头文件 {resource} 中声明。",
        )]
    return [resource], []


def _macro_definition_section(markdown: str) -> str:
    match = MACRO_DEFINITION_SECTION_RE.search(markdown)
    if match is None:
        return ""
    tail = markdown[match.end() :]
    next_heading = re.search(r"^\s{0,3}#{2,6}\s+", tail, re.MULTILINE)
    return tail[: next_heading.start()] if next_heading is not None else tail


def _macro_declaration_entry(macro: Any) -> dict[str, Any]:
    if macro.parameters is None:
        cpp = f"#define {macro.symbol} {macro.body}".rstrip()
        return {
            "symbol": macro.symbol,
            "cpp": cpp,
            "macro_kind": "object",
            "value": macro.body,
        }
    cpp = f"#define {macro.symbol}({macro.parameters}) {macro.body}".rstrip()
    return {
        "symbol": macro.symbol,
        "cpp": cpp,
        "macro_kind": "function",
        "parameters": [
            parameter.strip() for parameter in macro.parameters.split(",") if parameter.strip()
        ],
    }


def _source_macro_definition_fields(
    root: Path,
    document: Path,
    markdown: str,
    metadata: Any,
) -> dict[str, Any]:
    path = repo_relative(root, document)
    documented = list(_macro_definitions(_macro_definition_section(markdown)))
    issues: list[dict[str, Any]] = []
    if not documented:
        issues.append(issue(path, "declaration", "document_defect", "正文没有可确认的宏定义。"))
        return {
            "headers": [],
            "declaration": None,
            "type_catalog": None,
            "signature": [],
            "doxygen": [],
            "issues": issues,
        }

    candidates = list(metadata.header_candidates)
    headers = sorted(dict.fromkeys(resource for resource in candidates if isinstance(resource, str)))
    header_macros = []
    if len(headers) == 1:
        header_path = root / headers[0].lstrip("/")
        if header_path.is_file():
            header_macros = list(_macro_definitions(header_path.read_text(encoding="utf-8", errors="replace")))
    if len(headers) != 1:
        issues.append(issue(path, "headers", "unresolved", "宏定义的头文件映射未唯一确认，已保留空 headers。"))
        headers = []
    else:
        for macro in documented:
            if not any(
                candidate.symbol == macro.symbol and candidate.parameters == macro.parameters
                for candidate in header_macros
            ):
                issues.append(issue(
                    path,
                    "headers",
                    "source_defect",
                    f"{macro.symbol} 未在文档关联的头文件 {headers[0]} 中以相同形参声明。",
                ))
                headers = []
                break

    signatures: list[dict[str, Any]] = []
    doxygen: list[dict[str, Any]] = []
    for macro in documented:
        if macro.parameters is None:
            continue
        names = tuple(name.strip() for name in macro.parameters.split(",") if name.strip())
        identifier = "__".join((macro.symbol, *names))
        if MACRO_ID_RE.fullmatch(identifier) is None:
            issues.append(issue(
                path,
                "signature",
                "document_defect",
                f"{macro.symbol} 的函数宏形参无法生成有效 macro_id。",
            ))
            continue
        entry = {"macro_id": identifier}
        if entry not in signatures:
            signatures.append(entry)
        if headers:
            doxygen.append({
                "source_document": path,
                "header": headers[0].lstrip("/"),
                "function": f"{macro.symbol}({macro.parameters})",
                "parameters": f"{macro.symbol}({macro.parameters})",
                "macro_id": identifier,
                "archs": [],
                "template_parameters": [],
                "doxygen": {},
            })

    has_object_macro = any(macro.parameters is None for macro in documented)
    if not has_object_macro and signatures:
        return {
            "headers": [],
            "declaration": None,
            "type_catalog": None,
            "signature": signatures,
            "doxygen": doxygen,
            "issues": issues,
        }
    return {
        "headers": headers,
        "declaration": {"macros": [_macro_declaration_entry(macro) for macro in documented]},
        "type_catalog": None,
        "signature": signatures,
        "doxygen": doxygen,
        "issues": issues,
    }


def _source_definition_fields(
    root: Path,
    document: Path,
    markdown: str,
) -> dict[str, Any] | None:
    """Extract source-proven metadata for concrete C API definition pages."""
    category = _definition_category(root, document)
    if category is None:
        return None
    path = repo_relative(root, document)
    try:
        metadata = extract_api_metadata(document, date.today().isoformat(), root)
    except (OSError, ValueError) as exc:
        return {
            "headers": [],
            "declaration": None,
            "type_catalog": None,
            "signature": [],
            "doxygen": [],
            "issues": [issue(path, "declaration", "tool_defect", f"定义解析失败：{exc}")],
        }
    if category == "macro":
        return _source_macro_definition_fields(root, document, markdown, metadata)

    expected_kinds = {
        "constant": {"constant"},
        "enum": {"enum", "type_alias"},
        "type": {"type_catalog"},
        "union": {"struct", "union"},
    }
    definition_kind = metadata.api_kind
    declaration = metadata.declaration
    type_catalog = metadata.type_catalog
    issues: list[dict[str, Any]] = []
    if definition_kind not in expected_kinds[category]:
        declaration = None
        type_catalog = None
        issues.append(issue(path, "declaration", "document_defect", "正文定义形式无法按该分类解析。"))
    elif definition_kind == "type_catalog":
        if not type_catalog:
            issues.append(issue(path, "type_catalog", "document_defect", "正文没有可确认的数据类型分组。"))
    elif not isinstance(declaration, dict):
        issues.append(issue(path, "declaration", "document_defect", "正文没有可确认的定义声明。"))

    headers: list[str] = []
    if definition_kind != "type_catalog" and isinstance(declaration, dict):
        symbol = declaration.get("symbol")
        if isinstance(symbol, str) and symbol:
            headers, header_issues = _definition_header(root, metadata.header_candidates, symbol, path)
            issues.extend(header_issues)
        else:
            issues.append(issue(path, "declaration", "document_defect", "定义声明缺少符号名。"))
    return {
        "headers": headers,
        "declaration": declaration,
        "type_catalog": type_catalog,
        "signature": [],
        "doxygen": [],
        "issues": issues,
    }


def normalize_signature_ids(signature: Any) -> Any:
    """Convert legacy or worker-supplied signature keys to their canonical form."""
    if not isinstance(signature, list):
        return signature
    normalized = []
    for item in signature:
        if not isinstance(item, dict):
            normalized.append(item)
            continue
        output = deepcopy(item)
        prototype_id = output.get("prototype_id")
        override_id = output.get("override_id")
        if isinstance(prototype_id, str) ^ isinstance(override_id, str):
            current_key = "prototype_id" if isinstance(prototype_id, str) else "override_id"
            identifier = output[current_key]
            expected_key = signature_id_key(identifier)
            if current_key != expected_key:
                output[expected_key] = output.pop(current_key)
        normalized.append(output)
    return normalized


def issue(path: str, field: str, issue_type: str, message: str, evidence: list[dict[str, str]] | None = None) -> dict[str, Any]:
    return {
        "issue_id": f"{Path(path).stem}-{field}-{hashlib.sha1((path + message).encode()).hexdigest()[:8]}",
        "document_path": path,
        "field": field,
        "issue_type": issue_type,
        "severity": "warning" if issue_type in {"source_defect", "unresolved"} else "error",
        "message": message,
        "evidence": evidence or [],
    }


def build_worker_document(
    root: Path,
    path: str,
    namespaces: tuple[str, ...],
    example_discovery_index: Any = None,
    semantic: dict[str, Any] | None = None,
) -> dict[str, Any]:
    document = relative_api_path(root, path)
    original = document.read_text(encoding="utf-8", errors="replace")
    data, _, body = split_front_matter(original)
    definition = _source_definition_fields(root, document, body)
    if definition is None:
        source_signature, source_examples, marker_examples, source_doxygen, source_issues = _source_signature_fields(
            root, document, body, example_discovery_index
        )
        source_definition: dict[str, Any] = {
            "headers": None,
            "declaration": None,
            "type_catalog": None,
        }
    else:
        source_signature = definition["signature"]
        source_examples = []
        marker_examples = []
        source_doxygen = definition["doxygen"]
        source_issues = definition["issues"]
        source_definition = definition
    title = first_heading(body, document.stem)
    semantic_verified = semantic is not None
    if semantic is not None:
        # 语义输入是唯一语义来源：description/tag/dependencies 全部来自已验证的 semantic-input，
        # 旧 front matter 的语义值不被继承（结构字段仍由 scaffold 解析）。
        description = semantic["description"]["value"]
        merged_tags: dict[str, list[str]] = {
            namespace: list(values) for namespace, values in source_context_default_tags(path).items()
        }
        for item in semantic["tag"]:
            namespace, selected = next(iter(item.items()))
            merged_tags[namespace] = list(selected)
        semantic_dependencies = semantic["dependencies"]
    else:
        description = data.get("description")
        if not isinstance(description, str):
            description = ""
        tags = normalize_tag(data.get("tag"), namespaces)
        merged_tags = {
            namespace: values for item in tags for namespace, values in item.items()
        }
        # source-context 注入：api_surfaces 等路径可证标签按最长前缀自动注入，语义代理不填。
        for namespace, values in source_context_default_tags(path).items():
            if not merged_tags.get(namespace):
                merged_tags[namespace] = list(values)
        semantic_dependencies = None
    existing_timestamp = data.get("timestamp")
    if isinstance(existing_timestamp, date):
        timestamp = existing_timestamp.isoformat()
    elif isinstance(existing_timestamp, str):
        timestamp = existing_timestamp
    else:
        timestamp = date.today().isoformat()
    # On-demand scaffold: keep only namespaces with values so the artifact the worker
    # edits never carries empty tag entries (最小标签集: 不适用字段省略，不使用空数组).
    # Degradation is signalled by issues for every namespace without values, so the
    # resolver decision flow and issue counts are unchanged.
    filled_tags = [
        {namespace: merged_tags[namespace]} for namespace in namespaces if merged_tags.get(namespace)
    ]
    source_dtypes = source_signature_dtypes(body) if definition is None else {}
    raw_signature = source_signature if definition is not None else source_signature or normalize_signature_ids(
        data.get("signature") if isinstance(data.get("signature"), list) else []
    )
    if definition is not None:
        okftype = "reference.type.cpp"
    elif ARCH_DOCUMENT_NAME_RE.search(document.name):
        okftype = "reference.arch.cpp"
    elif FUNCTION_PROTOTYPE_SECTION_RE.search(body):
        okftype = "reference.api.cpp"
    else:
        okftype = "reference.index.cpp"
    fields: dict[str, Any] = {
        "okftype": okftype,
        "title": title,
        "resource": f"asc-devkit/{path}",
        "description": description,
        "tag": filled_tags,
        "signature": normalize_signature_dtypes(
            raw_signature,
            source_dtypes,
            data.get("dtype"),
        ),
        "dependencies": semantic_dependencies if semantic_dependencies is not None else _retained_dependencies_with_evidence(
            path, body, data.get("dependencies") if isinstance(data.get("dependencies"), list) else [],
        ),
        "examples": source_examples if definition is not None else source_examples or (
            data.get("examples") if isinstance(data.get("examples"), list) else []
        ),
        "headers": source_definition["headers"],
        "declaration": source_definition["declaration"],
        "type_catalog": source_definition["type_catalog"],
        "timestamp": timestamp,
    }
    _apply_architecture_override_suffixes(
        fields,
        source_doxygen,
        marker_examples,
        _architecture_override_suffixes(root, document, body, fields["signature"]),
    )
    issues: list[dict[str, Any]] = list(source_issues)
    if not description:
        issues.append(issue(path, "description", "document_defect", "缺少由 description agent 生成并验证的 API 描述。"))
    if definition is None and not fields["signature"] and not re.search(r"函数原型|函数声明", body):
        issues.append(issue(path, "signature", "document_defect", "正文没有可确认的函数原型信息。"))
    for signature in fields["signature"]:
        identifier_keys = [
            key for key in ("prototype_id", "override_id", "macro_id")
            if isinstance(signature, dict) and isinstance(signature.get(key), str)
        ]
        if len(identifier_keys) != 1:
            issues.append(issue(path, "signature", "tool_defect", "signature 条目必须唯一包含 prototype_id、override_id 或 macro_id，不能写入原始 prototype 字段。"))
            break
    filled_namespaces = {next(iter(item)) for item in fields["tag"]}
    for namespace in namespaces:
        if namespace not in filled_namespaces:
            issues.append(issue(path, f"tag.{namespace}", "unresolved", "当前 worker 没有找到该标签主题的充分正文证据。"))
    # Multiple overloads can produce the same document-level diagnostic. Keep one
    # stable record so every issue ID still has exactly one resolver decision.
    unique_issues: list[dict[str, Any]] = []
    seen_issue_ids: set[str] = set()
    for found in issues:
        if found["issue_id"] not in seen_issue_ids:
            unique_issues.append(found)
            seen_issue_ids.add(found["issue_id"])
    if semantic is not None:
        # 语义证据合并并逐字校验（fail fast；validate 阶段仍会独立复检）。
        for reference in semantic["description"]["evidence"]:
            if reference["excerpt"] not in body:
                raise ValidationError(f"{path}: semantic description evidence is not verbatim")
        evidence: dict[str, Any] = {
            "description": [
                {"source_path": path, "excerpt": reference["excerpt"]}
                for reference in semantic["description"]["evidence"]
            ],
        }
        for key, references in semantic["tag_evidence"].items():
            for reference in references:
                if reference["excerpt"] not in body:
                    raise ValidationError(f"{path}: semantic tag evidence is not verbatim for {key}")
            evidence[key] = [
                {"source_path": path, "excerpt": reference["excerpt"]} for reference in references
            ]
        for namespace, values in source_context_default_tags(path).items():
            for value in values:
                evidence.setdefault(f"{namespace}:{value}", [{"source_path": path, "excerpt": path}])
    else:
        evidence = {"description": [], **_retained_tag_evidence(path, body, fields["tag"])}
    return {
        "path": path,
        "sha256": sha256(document),
        "fields": fields,
        "evidence": evidence,
        "issues": unique_issues,
        "doxygen": source_doxygen,
        "example_markers": marker_examples,
        "semantic_verified": semantic_verified,
    }


def write_json(path: Path, value: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def split_conflicting_doxygen_records(
    by_header: dict[str, list[dict[str, Any]]],
) -> tuple[dict[str, list[dict[str, Any]]], list[dict[str, Any]]]:
    """Keep only Doxygen records whose shared declaration evidence agrees."""
    retained: dict[str, list[dict[str, Any]]] = {}
    conflicts: list[dict[str, Any]] = []
    for header, records in by_header.items():
        groups: dict[tuple[str, str, str, str], list[dict[str, Any]]] = {}
        for record in records:
            key = (
                str(record.get("function", "")),
                str(record.get("parameters", "")),
                str(record.get("macro_id", "")),
                json.dumps(record.get("template_parameters", []), ensure_ascii=False, sort_keys=True),
            )
            groups.setdefault(key, []).append(record)
        kept = []
        for key, group in groups.items():
            evidence = {
                json.dumps(record.get("doxygen", {}), ensure_ascii=False, sort_keys=True)
                for record in group
            }
            if len(evidence) == 1:
                kept.extend(group)
            else:
                conflicts.append({
                    "header": header,
                    "function": key[0],
                    "records": group,
                })
        retained[header] = kept
    return retained, conflicts


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
        "taxonomy": str((Path(__file__).resolve().parents[2] / "okf-tag-generate" / "taxonomy" / "releases" / "core-v9-frozen.json").resolve()),
        "write_targets": list(DEFAULT_WRITE_TARGETS),
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


def load_semantic_input(
    path: Path,
    task: dict[str, Any],
    root: Path,
    namespaces: tuple[str, ...],
    values: dict[str, set[str]],
) -> dict[str, dict[str, Any]]:
    """Load and validate a batch-level semantic-input artifact (see references/semantic-contract.md)."""
    payload = read_json(path)
    if payload.get("schema_version") != 1 or payload.get("purpose") != "okf_yaml_semantic_input":
        raise ValidationError("semantic input schema or purpose is invalid")
    if payload.get("run_id") != task["run_id"] or payload.get("batch_id") != task["batch_id"]:
        raise ValidationError("semantic input run_id/batch_id does not match the task")
    annotations = payload.get("annotations")
    if not isinstance(annotations, list) or not annotations:
        raise ValidationError("semantic input annotations must be a non-empty list")
    expected = set(task["documents"])
    by_path: dict[str, dict[str, Any]] = {}
    for annotation in annotations:
        if not isinstance(annotation, dict):
            raise ValidationError("semantic input annotation must be an object")
        document_path = annotation.get("document_path")
        if not isinstance(document_path, str) or document_path not in expected:
            raise ValidationError(f"semantic input annotation path out of scope: {document_path!r}")
        if document_path in by_path:
            raise ValidationError(f"semantic input duplicate annotation: {document_path}")
        target = root / document_path
        if not target.is_file():
            raise ValidationError(f"semantic input document missing: {document_path}")
        if annotation.get("document_sha256") != sha256(target):
            raise ValidationError(f"semantic input hash mismatch (input changed after prepare): {document_path}")
        description = annotation.get("description")
        if not isinstance(description, dict) or not isinstance(description.get("value"), str):
            raise ValidationError(f"semantic input description is invalid: {document_path}")
        desc_evidence = description.get("evidence")
        if description["value"]:
            if not isinstance(desc_evidence, list) or not desc_evidence:
                raise ValidationError(f"semantic input description evidence is missing: {document_path}")
            for reference in desc_evidence:
                if (
                    not isinstance(reference, dict)
                    or not isinstance(reference.get("excerpt"), str)
                    or reference.get("source_path") != document_path
                ):
                    raise ValidationError(f"semantic input description evidence is invalid: {document_path}")
        elif desc_evidence:
            raise ValidationError(f"semantic input unresolved description must not carry evidence: {document_path}")
        tag = annotation.get("tag")
        if not isinstance(tag, list):
            raise ValidationError(f"semantic input tag is invalid: {document_path}")
        tag_evidence = annotation.get("tag_evidence")
        if not isinstance(tag_evidence, dict):
            raise ValidationError(f"semantic input tag_evidence is invalid: {document_path}")
        for item in tag:
            if not isinstance(item, dict) or len(item) != 1:
                raise ValidationError(f"semantic input tag entries must be single-key objects: {document_path}")
            namespace, selected = next(iter(item.items()))
            if namespace == "api_surfaces":
                raise ValidationError(
                    f"semantic input must not carry api_surfaces (path-injected by source context): {document_path}"
                )
            if namespace not in namespaces:
                raise ValidationError(f"semantic input tag namespace unknown: {namespace}")
            if (
                not isinstance(selected, list)
                or not selected
                or not all(isinstance(value, str) and value for value in selected)
            ):
                raise ValidationError(f"semantic input tag.{namespace} must be a non-empty string list: {document_path}")
            unknown = set(selected) - values.get(namespace, set())
            if unknown:
                raise ValidationError(
                    f"semantic input tag.{namespace} values outside taxonomy {sorted(unknown)}: {document_path}"
                )
            for value in set(selected):
                references = tag_evidence.get(f"{namespace}:{value}")
                if not isinstance(references, list) or not references:
                    raise ValidationError(f"semantic input missing evidence for {namespace}:{value}: {document_path}")
                for reference in references:
                    if (
                        not isinstance(reference, dict)
                        or not isinstance(reference.get("excerpt"), str)
                        or reference.get("source_path") != document_path
                    ):
                        raise ValidationError(f"semantic input evidence for {namespace}:{value} is invalid: {document_path}")
        dependencies = annotation.get("dependencies")
        if not isinstance(dependencies, list):
            raise ValidationError(f"semantic input dependencies is invalid: {document_path}")
        for dep in dependencies:
            if (
                not isinstance(dep, dict)
                or not isinstance(dep.get("path"), str)
                or dep.get("kind") not in DEPENDENCY_KINDS
            ):
                raise ValidationError(f"semantic input dependency entry is invalid: {document_path}")
            evidence = dep.get("evidence")
            if (
                not isinstance(evidence, dict)
                or set(evidence) != {"source_path", "excerpt"}
                or evidence.get("source_path") != document_path
            ):
                raise ValidationError(f"semantic input dependency evidence is invalid: {document_path}")
        by_path[document_path] = annotation
    missing = expected - set(by_path)
    if missing:
        raise ValidationError("semantic input does not cover every task document: " + ", ".join(sorted(missing)))
    return by_path


def worker(args: argparse.Namespace) -> int:
    task = read_json(args.task.expanduser().resolve())
    if task.get("schema_version") != 1:
        raise ValidationError("worker task schema_version must be 1")
    root = Path(task["devkit_root"]).resolve()
    namespaces, values = taxonomy(Path(task["taxonomy"]).resolve())
    example_discovery_index = build_example_discovery_index(root)
    semantic_by_path: dict[str, dict[str, Any]] | None = None
    if args.semantic_input:
        semantic_by_path = load_semantic_input(
            args.semantic_input.expanduser().resolve(), task, root, namespaces, values
        )
    documents = [
        build_worker_document(
            root, path, namespaces, example_discovery_index,
            semantic=semantic_by_path.get(path) if semantic_by_path is not None else None,
        )
        for path in task["documents"]
    ]
    result = {key: task[key] for key in ("schema_version", "run_id", "batch_id")}
    result["worker_id"] = args.worker_id
    result["semantic_input"] = bool(semantic_by_path)
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
            {key: [] if key == namespace else values}
            for item in fields.get("tag", [])
            if isinstance(item, dict)
            for key, values in item.items()
        ]
        if not any(namespace in item for item in updated):
            updated.append({namespace: []})
        fields["tag"] = updated
    elif field in {"signature", "dependencies", "examples"}:
        fields[field] = []
    elif field == "headers":
        fields[field] = []
    elif field in {"declaration", "type_catalog"}:
        fields[field] = None
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


def normalize_examples(root: Path, document_path: str, examples: Any) -> list[dict[str, Any]]:
    """Validate example links emitted by the legacy-compatible source resolver."""
    if not isinstance(examples, list):
        raise ValidationError(f"{document_path}: examples must be an array")
    normalized = []
    seen = set()
    for index, item in enumerate(examples):
        if not isinstance(item, dict):
            raise ValidationError(f"{document_path}: examples[{index}] must be an object")
        unknown = set(item) - {"path", "file", "archs", "override_id"}
        if unknown:
            raise ValidationError(f"{document_path}: examples[{index}] has unsupported fields: {sorted(unknown)}")
        example_path = item.get("path")
        if not isinstance(example_path, str) or not example_path.startswith("/examples/"):
            raise ValidationError(f"{document_path}: examples[{index}].path must start with /examples/")
        relative_path = Path(example_path.lstrip("/"))
        if ".." in relative_path.parts:
            raise ValidationError(f"{document_path}: examples[{index}].path must not escape the repository")
        target = (root / relative_path).resolve()
        try:
            target.relative_to(root.resolve())
        except ValueError as exc:
            raise ValidationError(f"{document_path}: examples[{index}].path escapes the repository") from exc
        if not target.exists():
            raise ValidationError(f"{document_path}: examples[{index}].path does not exist")
        file_name = item.get("file")
        if file_name is not None:
            if not isinstance(file_name, str) or not file_name or Path(file_name).is_absolute() or ".." in Path(file_name).parts:
                raise ValidationError(f"{document_path}: examples[{index}].file must be a relative source path")
            if not (target / file_name).is_file():
                raise ValidationError(f"{document_path}: examples[{index}].file does not exist under its example path")
        archs = item.get("archs")
        if archs is not None and (
            not isinstance(archs, list)
            or not archs
            or any(not isinstance(arch, str) or not arch for arch in archs)
            or len(archs) != len(set(archs))
        ):
            raise ValidationError(f"{document_path}: examples[{index}].archs must be a non-empty unique string array")
        override_id = item.get("override_id")
        if override_id is not None:
            if not isinstance(override_id, str) or signature_id_key(override_id) != "override_id":
                raise ValidationError(f"{document_path}: examples[{index}].override_id must be a concrete override ID")
        output = {"path": example_path}
        if file_name is not None:
            output["file"] = file_name
        if archs is not None:
            output["archs"] = archs
        if override_id is not None:
            output["override_id"] = override_id
        identity = (example_path, file_name or "", override_id or "")
        if identity in seen:
            raise ValidationError(f"{document_path}: duplicate example entry")
        seen.add(identity)
        normalized.append(output)
    return normalized


def validate_base_fields(fields: dict[str, Any], path: str) -> None:
    """Enforce the fixed top-level YAML contract before rendering."""
    if fields.get("okftype") not in OKF_TYPES:
        raise ValidationError(f"{path}: okftype must be one of {sorted(OKF_TYPES)}")
    if not isinstance(fields.get("title"), str) or not fields["title"].strip():
        raise ValidationError(f"{path}: title must be a non-empty string")
    if fields.get("resource") != f"asc-devkit/{path}":
        raise ValidationError(f"{path}: resource must equal asc-devkit/<document path>")
    if not isinstance(fields.get("description"), str):
        raise ValidationError(f"{path}: description must be a string")
    timestamp = fields.get("timestamp")
    if not isinstance(timestamp, str):
        raise ValidationError(f"{path}: timestamp must be an ISO date string")
    try:
        date.fromisoformat(timestamp)
    except ValueError as exc:
        raise ValidationError(f"{path}: timestamp must be an ISO date string") from exc


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
    selected_map = {next(iter(item)): next(iter(item.values())) for item in result}
    rules = load_tag_rules()["required_namespaces"]
    for namespace in rules["always"]:
        if not selected_map.get(namespace) and f"tag.{namespace}" not in allowed_empty:
            raise ValidationError(f"{path}: required tag namespace is empty: {namespace}")
    roles = set(selected_map.get("roles", []))
    for namespace, condition in rules["conditional"].items():
        if roles & set(condition["when_roles_in"]) and not selected_map.get(namespace):
            if f"tag.{namespace}" not in allowed_empty:
                raise ValidationError(
                    f"{path}: tag.{namespace} is required for roles: "
                    + ",".join(sorted(roles & set(condition["when_roles_in"])))
                )
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
        identifier_keys = [
            key for key in ("prototype_id", "override_id", "macro_id")
            if isinstance(item.get(key), str)
        ]
        if len(identifier_keys) != 1:
            raise ValidationError(
                f"{path}: signature[{index}] must contain exactly one prototype_id, override_id or macro_id"
            )
        identifier_key = identifier_keys[0]
        if identifier_key == "macro_id":
            if set(item) != {"macro_id"} or MACRO_ID_RE.fullmatch(item["macro_id"]) is None:
                raise ValidationError(f"{path}: signature[{index}].macro_id must be a valid standalone macro ID")
            continue
        if signature_id_key(item[identifier_key]) != identifier_key:
            expected_key = signature_id_key(item[identifier_key])
            reason = "retains a documented placeholder" if expected_key == "prototype_id" else "has no documented placeholder"
            raise ValidationError(
                f"{path}: signature[{index}] {reason} and must use {expected_key}"
            )
        for key in ("archs", "headers", "dtype"):
            if key in item and (not isinstance(item[key], list) or any(not isinstance(value, str) for value in item[key])):
                raise ValidationError(f"{path}: signature[{index}].{key} must be a string array")
        if "dtype" in item:
            if identifier_key != "prototype_id" or "<dtype>" not in item["prototype_id"]:
                raise ValidationError(
                    f"{path}: signature[{index}].dtype requires a prototype_id containing <dtype>"
                )
            if not item["dtype"] or len(item["dtype"]) != len(set(item["dtype"])):
                raise ValidationError(f"{path}: signature[{index}].dtype must be a non-empty unique string array")
    return signature


def _validate_headers(headers: Any, path: str) -> list[str]:
    if not isinstance(headers, list) or any(not isinstance(item, str) for item in headers):
        raise ValidationError(f"{path}: headers must be a string array")
    if len(headers) != len(set(headers)):
        raise ValidationError(f"{path}: headers must not contain duplicates")
    for header in headers:
        candidate = Path(header)
        if not header.startswith("/include/") or candidate.suffix not in {".h", ".hpp"}:
            raise ValidationError(f"{path}: headers must be /include/ header resources")
        if ".." in candidate.parts:
            raise ValidationError(f"{path}: headers must not escape the repository")
    return headers


def _validate_declaration(definition_kind: str, declaration: Any, path: str) -> None:
    if not isinstance(declaration, dict):
        raise ValidationError(f"{path}: declaration must be an object")
    if definition_kind == "macro":
        macros = declaration.get("macros")
        if set(declaration) != {"macros"} or not isinstance(macros, list) or not macros:
            raise ValidationError(f"{path}: macro declaration must contain non-empty macros")
        for index, macro in enumerate(macros):
            if not isinstance(macro, dict):
                raise ValidationError(f"{path}: declaration.macros[{index}] must be an object")
            allowed = {"symbol", "cpp", "macro_kind", "parameters", "value"}
            if set(macro) - allowed:
                raise ValidationError(f"{path}: declaration.macros[{index}] has unsupported fields")
            if not isinstance(macro.get("symbol"), str) or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", macro["symbol"]):
                raise ValidationError(f"{path}: declaration.macros[{index}].symbol is invalid")
            if not isinstance(macro.get("cpp"), str) or not macro["cpp"].startswith("#define "):
                raise ValidationError(f"{path}: declaration.macros[{index}].cpp must be a macro definition")
            if macro.get("macro_kind") not in {"function", "object"}:
                raise ValidationError(f"{path}: declaration.macros[{index}].macro_kind is invalid")
            if macro["macro_kind"] == "function" and (
                not isinstance(macro.get("parameters"), list)
                or any(not isinstance(item, str) for item in macro["parameters"])
            ):
                raise ValidationError(f"{path}: function macro parameters must be a string array")
        return

    if not isinstance(declaration.get("symbol"), str) or not re.fullmatch(
        r"[A-Za-z_][A-Za-z0-9_]*", declaration["symbol"]
    ):
        raise ValidationError(f"{path}: declaration.symbol is invalid")
    if not isinstance(declaration.get("cpp"), str) or not declaration["cpp"].strip():
        raise ValidationError(f"{path}: declaration.cpp is required")
    allowed = {
        "constant": {"symbol", "cpp", "value", "type"},
        "enum": {"symbol", "cpp", "members", "underlying_type"},
        "struct": {"symbol", "cpp", "fields", "record_kind"},
        "union": {"symbol", "cpp", "fields", "record_kind"},
        "type_alias": {"symbol", "cpp", "target"},
    }[definition_kind]
    if set(declaration) - allowed:
        raise ValidationError(f"{path}: declaration has unsupported fields for {definition_kind}")
    if definition_kind == "type_alias" and not isinstance(declaration.get("target"), str):
        raise ValidationError(f"{path}: type_alias declaration.target is required")
    if definition_kind in {"struct", "union"} and declaration.get("record_kind") != definition_kind:
        raise ValidationError(f"{path}: {definition_kind} declaration.record_kind is invalid")
    collection = "members" if definition_kind == "enum" else "fields" if definition_kind in {"struct", "union"} else None
    if collection is not None:
        values = declaration.get(collection)
        if not isinstance(values, list) or any(not isinstance(item, dict) for item in values):
            raise ValidationError(f"{path}: declaration.{collection} must be an object array")
        for index, item in enumerate(values):
            if not isinstance(item.get("name"), str) or not item["name"]:
                raise ValidationError(f"{path}: declaration.{collection}[{index}].name is required")


def _validate_type_catalog(type_catalog: Any, path: str) -> None:
    if not isinstance(type_catalog, dict) or set(type_catalog) != {"groups"}:
        raise ValidationError(f"{path}: type_catalog must contain groups")
    groups = type_catalog["groups"]
    if not isinstance(groups, list) or not groups:
        raise ValidationError(f"{path}: type_catalog.groups must be non-empty")
    for index, group in enumerate(groups):
        if not isinstance(group, dict) or set(group) != {"name", "types"}:
            raise ValidationError(f"{path}: type_catalog.groups[{index}] is invalid")
        if not isinstance(group["name"], str) or not isinstance(group["types"], list) or not group["types"]:
            raise ValidationError(f"{path}: type_catalog.groups[{index}] must name non-empty types")
        if any(not isinstance(item, str) for item in group["types"]):
            raise ValidationError(f"{path}: type_catalog groups must contain string types")


def _definition_kind_from_fields(fields: dict[str, Any]) -> str | None:
    """Infer the validation shape without emitting a public discriminator field."""
    if fields.get("type_catalog") not in (None, {}):
        return "type_catalog"
    declaration = fields.get("declaration")
    if not isinstance(declaration, dict) or not declaration:
        signature = fields.get("signature")
        if isinstance(signature, list) and signature and all(
            isinstance(item, dict) and isinstance(item.get("macro_id"), str)
            for item in signature
        ):
            return "function_macro"
        return None
    if "macros" in declaration:
        return "macro"
    record_kind = declaration.get("record_kind")
    if record_kind in {"struct", "union"}:
        return record_kind
    if "members" in declaration:
        return "enum"
    if "target" in declaration:
        return "type_alias"
    if "value" in declaration or "type" in declaration:
        return "constant"
    return None


def validate_definition_fields(
    fields: dict[str, Any], path: str, allowed_empty: set[str]
) -> None:
    definition_kind = _definition_kind_from_fields(fields)
    if definition_kind is None:
        if any(fields.get(key) not in (None, [], {}) for key in ("headers", "declaration", "type_catalog")):
            raise ValidationError(f"{path}: unsupported definition metadata structure")
        return
    if definition_kind != "type_catalog" and fields.get("type_catalog") not in (None, {}):
        raise ValidationError(f"{path}: type_catalog is only valid for type catalog metadata")
    headers = fields.get("headers")
    if headers not in (None, []):
        _validate_headers(headers, path)
    if definition_kind == "type_catalog":
        if fields.get("declaration") not in (None, {}):
            raise ValidationError(f"{path}: type_catalog must not contain declaration")
        if not fields.get("type_catalog") and "type_catalog" not in allowed_empty:
            raise ValidationError(f"{path}: type_catalog metadata is required")
        if fields.get("type_catalog"):
            _validate_type_catalog(fields["type_catalog"], path)
        return
    if definition_kind == "function_macro":
        if fields.get("headers") not in (None, []) or fields.get("declaration") not in (None, {}):
            raise ValidationError(f"{path}: function_macro only uses signature metadata")
        if not fields.get("signature") and "signature" not in allowed_empty:
            raise ValidationError(f"{path}: function_macro signature is required")
        return
    if not fields.get("declaration"):
        if "declaration" not in allowed_empty:
            raise ValidationError(f"{path}: definition declaration is required")
        return
    _validate_declaration(definition_kind, fields["declaration"], path)


def render_yaml(
    fields: dict[str, Any], namespaces: tuple[str, ...], values: dict[str, set[str]],
    root: Path, path: str, body: str, evidence: dict[str, Any], allowed_empty: set[str],
) -> str:
    output = {key: deepcopy(fields.get(key)) for key in ALLOWED_FIELDS}
    validate_base_fields(output, path)
    output["signature"] = normalize_signature_ids(output["signature"])
    output["tag"] = validate_tags(output, namespaces, values, path, body, evidence, allowed_empty)
    # Empty namespaces stay in worker artifacts (resolver degradation signal) but are
    # omitted from the rendered front matter: inapplicable fields are omitted, never
    # written as empty arrays (OKF 字段规范 / 最小标签集设计说明).
    output["tag"] = [
        {namespace: InlineList(selected)}
        for item in output["tag"] for namespace, selected in item.items() if selected
    ]
    output["dependencies"] = normalize_dependencies(root, path, output.get("dependencies", []))
    for signature in output["signature"]:
        if isinstance(signature, dict):
            for key in ("archs", "headers", "dtype"):
                if isinstance(signature.get(key), list):
                    signature[key] = InlineList(signature[key])
    for example in output["examples"]:
        if isinstance(example, dict) and isinstance(example.get("archs"), list):
            example["archs"] = InlineList(example["archs"])
    output["signature"] = validate_signature(output["signature"], path)
    output["examples"] = normalize_examples(root, path, output["examples"])
    validate_definition_fields(output, path, allowed_empty)
    if not output["signature"]:
        output.pop("signature")
    for key in ("headers", "declaration", "type_catalog"):
        if output.get(key) in (None, [], {}):
            output.pop(key, None)
    if isinstance(output.get("headers"), list):
        output["headers"] = InlineList(output["headers"])
    return yaml.dump(output, allow_unicode=True, sort_keys=False, default_flow_style=False, Dumper=DisplayDumper)


def updated_document(
    original: str, fields: dict[str, Any], namespaces: tuple[str, ...], values: dict[str, set[str]],
    root: Path, path: str, evidence: dict[str, Any], allowed_empty: set[str],
) -> str:
    _, _, body = split_front_matter(original)
    cleaned_body = strip_inline_api_signature_blocks(body)
    return "---\n" + render_yaml(fields, namespaces, values, root, path, cleaned_body, evidence, allowed_empty) + "---\n" + cleaned_body


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
    file_final: dict[str, dict[str, Any]] = {}
    doxygen_inputs: list[dict[str, Any]] = []
    marker_inputs: list[tuple[str, list[dict[str, Any]]]] = []
    marker_skips: list[dict[str, str]] = []

    def stage_document(rel: str, before: str, content: str) -> None:
        if rel in file_final:
            raise ValidationError(f"multiple document patches for one file: {rel}")
        file_final[rel] = {
            "path": rel,
            "before_sha256": before,
            "content": content,
            "kinds": ["document"],
        }
        patches.append({"kind": "document", "path": rel, "before_sha256": before, "content": content})

    def stage_source(rel: str, before: str, content: str, kind: str) -> None:
        if rel not in file_final:
            file_final[rel] = {
                "path": rel,
                "before_sha256": before,
                "content": content,
                "kinds": [],
            }
        file_final[rel]["content"] = content
        file_final[rel]["kinds"].append(kind)

    for document in documents:
        path = document["path"]
        target = relative_api_path(root, path)
        if sha256(target) != document["sha256"]:
            raise ValidationError(f"input changed after worker read: {path}")
        if not document.get("semantic_verified", False):
            # 语义输入门禁：未经 semantic-input 合并的结果只能用于调试，不得写入 DevKit
            # （构造 worker-result 的控制器单元测试必须显式置 semantic_verified: true）。
            raise ValidationError(
                f"{path}: semantic input missing (semantic_verified=false); "
                "rerun controller.py worker with --semantic-input"
            )
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
            if (
                found["field"] in {"declaration", "type_catalog", "signature"}
                and decision["resolution"] == "blank_field"
            ):
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
        stage_document(path, document["sha256"], updated)
        doxygen = document.get("doxygen", [])
        if not isinstance(doxygen, list) or any(not isinstance(record, dict) for record in doxygen):
            raise ValidationError(f"{path}: doxygen must be structured declaration records")
        for record in doxygen:
            if record.get("source_document") not in {None, path}:
                raise ValidationError(f"{path}: doxygen.source_document must match its worker document")
            staged = deepcopy(record)
            staged["source_document"] = path
            doxygen_inputs.append(staged)
        markers = document.get("example_markers", [])
        if not isinstance(markers, list) or any(not isinstance(marker, dict) for marker in markers):
            raise ValidationError(f"{path}: example_markers must be legacy example-call records")
        marker_inputs.append((path, markers))

    doxygen_by_header: dict[str, list[dict[str, Any]]] = {}
    for record in doxygen_inputs:
        header = record.get("header")
        if not isinstance(header, str) or not header.endswith((".h", ".hpp")):
            raise ValidationError("doxygen.header must be a repository-relative header path")
        header_path = Path(header)
        if header_path.is_absolute() or ".." in header_path.parts:
            raise ValidationError("doxygen.header must be a repository-relative header path")
        target = (root / header_path).resolve()
        try:
            target.relative_to(root.resolve())
        except ValueError as exc:
            raise ValidationError(f"doxygen header escapes DevKit root: {header}") from exc
        if not target.is_file():
            raise ValidationError(f"doxygen header does not exist: {header}")
        doxygen_by_header.setdefault(repo_relative(root, target), []).append(record)
    doxygen_by_header, doxygen_conflicts = split_conflicting_doxygen_records(doxygen_by_header)
    for conflict in doxygen_conflicts:
        message = (
            f"{conflict['header']} 中同一物理声明 {conflict['function']} 的跨架构参数说明不一致，"
            "未写入 Doxygen。"
        )
        for record in conflict["records"]:
            found = issue(record["source_document"], "include_doxygen", "source_defect", message)
            found["decision"] = {
                "classification": "source_defect",
                "resolution": "report_source_defect",
                "field": "include_doxygen",
                "note": "同一头文件声明的跨架构参数语义冲突，等待源文档统一后再生成。",
            }
            issue_report.append(found)
    for rel, records in doxygen_by_header.items():
        if rel in file_final:
            raise ValidationError(f"Doxygen target already has a planned patch: {rel}")
        target = (root / rel).resolve()
        original = target.read_text(encoding="utf-8", errors="replace")
        verified_records = []
        for record in records:
            try:
                validate_record_target(original, record)
            except DoxygenValidationError as exc:
                found = issue(
                    record["source_document"],
                    "include_doxygen",
                    "source_defect",
                    f"{rel} 中的声明无法按文档证据唯一定位：{exc}。未写入 Doxygen。",
                )
                found["decision"] = {
                    "classification": "source_defect",
                    "resolution": "report_source_defect",
                    "field": "include_doxygen",
                    "note": "文档原型或参数与真实头文件声明不一致，等待源文档修正。",
                }
                issue_report.append(found)
                continue
            verified_records.append(record)
        if not verified_records:
            continue
        try:
            updated, count = apply_records_to_text(original, verified_records)
        except DoxygenValidationError as exc:
            raise ValidationError(f"Doxygen validation failed for {rel}: {exc}") from exc
        if updated != original:
            stage_source(rel, sha256(target), updated, "doxygen")
        for record in verified_records:
            patches.append({"kind": "doxygen", "path": rel, "function": record.get("function")})

    insertions, skips = candidate_insertions_for_documents(marker_inputs, root)
    for skipped in skips:
        marker_skips.append({
            "path": repo_relative(root, skipped.file_path) if skipped.file_path else "",
            "marker_id": skipped.marker_id,
            "reason": skipped.reason,
            "document_path": skipped.api_path,
        })
    by_file: dict[Path, list[Any]] = {}
    for insertion in insertions:
        by_file.setdefault(insertion.file_path, []).append(insertion)
    for target, file_insertions in by_file.items():
        rel = repo_relative(root, target)
        if rel in file_final:
            raise ValidationError(f"example marker target already has a planned patch: {rel}")
        original = target.read_text(encoding="utf-8", errors="replace")
        stage_source(
            rel,
            sha256(target),
            apply_insertions_to_text(original, file_insertions),
            "example_markers",
        )
        for insertion in file_insertions:
            patches.append({
                "kind": "example_markers",
                "path": rel,
                "marker_id": insertion.marker_id,
                "existing_marker_id": insertion.existing_marker_id,
            })
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
    files = sorted(file_final.values(), key=lambda item: item["path"])
    write_json(
        run_dir / "write-plan.json",
        {
            "schema_version": 1,
            "run_id": manifest["run_id"],
            "patches": patches,
            "files": files,
            "marker_skips": marker_skips,
        },
    )
    write_json(run_dir / "validation.json", {"schema_version": 1, "run_id": manifest["run_id"], "status": "ok", "patches": len(patches), "files": len(files), "issues": len(issue_report)})
    print(json.dumps({"status": "ok", "patches": len(patches), "files": len(files), "issues": len(issue_report), "report": str(run_dir / 'error-report.md')}, ensure_ascii=False, indent=2))
    return 0


def write(args: argparse.Namespace) -> int:
    run_dir = args.run_dir.expanduser().resolve()
    validation = read_json(run_dir / "validation.json")
    if validation.get("status") != "ok":
        raise ValidationError("run has not passed validation")
    manifest = read_json(run_dir / "manifest.json")
    root = Path(manifest["devkit_root"]).resolve()
    plan = read_json(run_dir / "write-plan.json")
    files = plan.get("files") or []
    if not files:
        raise ValidationError("write plan has no files")
    with write_lock(root):
        for entry in files:
            target = (root / entry["path"]).resolve()
            if sha256(target) != entry["before_sha256"]:
                raise ValidationError(f"input changed after dry-run: {entry['path']}")
        for entry in files:
            target = (root / entry["path"]).resolve()
            target.write_text(entry["content"], encoding="utf-8")
    write_json(run_dir / "write-result.json", {"schema_version": 1, "run_id": manifest["run_id"], "written": len(files)})
    print(json.dumps({"written": len(files), "run_dir": str(run_dir)}, ensure_ascii=False, indent=2))
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
    worker_parser.add_argument("--semantic-input", type=Path, default=None)
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
