"""Load the flat OKF profile and classify documents from their structure."""

from __future__ import annotations

from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path
import re
from typing import Any

import yaml


SKILL_ROOT = Path(__file__).resolve().parents[1]
CONFIG_ROOT = SKILL_ROOT / "config"
PROFILE_PATH = CONFIG_ROOT / "document-profile-registry.yaml"
SOURCE_CONTEXT_PATH = CONFIG_ROOT / "source-context-v1.yaml"


class ModelValidationError(ValueError):
    """Raised when the flat profile or source context is inconsistent."""


@dataclass(frozen=True)
class Classification:
    okftype: str
    rule: str


@dataclass(frozen=True)
class ExecutionPlan:
    okftype: str
    rule: str
    fields: tuple[str, ...]
    required: tuple[str, ...]
    outputs: tuple[str, ...]


def _load_yaml(path: Path) -> dict[str, Any]:
    try:
        value = yaml.safe_load(path.read_text(encoding="utf-8"))
    except (OSError, yaml.YAMLError) as exc:
        raise ModelValidationError(f"cannot load registry {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise ModelValidationError(f"registry must be a YAML mapping: {path}")
    return value


def _string_list(value: Any, location: str, *, allow_empty: bool = False) -> list[str]:
    if not isinstance(value, list) or any(not isinstance(item, str) or not item for item in value):
        raise ModelValidationError(f"{location} must be a string list")
    if not allow_empty and not value:
        raise ModelValidationError(f"{location} must not be empty")
    if len(value) != len(set(value)):
        raise ModelValidationError(f"{location} must not contain duplicates")
    return list(value)


@lru_cache(maxsize=1)
def load_model() -> dict[str, Any]:
    document = _load_yaml(PROFILE_PATH)
    source = _load_yaml(SOURCE_CONTEXT_PATH)
    if document.get("schema_version") != 5 or document.get("status") != "active":
        raise ModelValidationError("document profile registry must be active schema version 5")
    if source.get("schema_version") != 1:
        raise ModelValidationError("source context registry must use schema version 1")

    profiles = document.get("profiles")
    fields = document.get("field_catalog")
    common = _string_list(document.get("common_required_fields"), "common_required_fields")
    common_fields = _string_list(document.get("common_fields"), "common_fields")
    field_order = _string_list(document.get("field_order"), "field_order")
    output_contracts = document.get("output_contracts")
    if not isinstance(profiles, dict) or not profiles:
        raise ModelValidationError("profiles must be a non-empty mapping")
    if not isinstance(fields, dict) or not fields:
        raise ModelValidationError("field_catalog must be a non-empty mapping")
    if not isinstance(output_contracts, dict) or not output_contracts:
        raise ModelValidationError("output_contracts must be a non-empty mapping")
    if any(field not in fields for field in common):
        raise ModelValidationError("common_required_fields contains an unregistered field")
    if set(field_order) != set(fields):
        raise ModelValidationError("field_order must contain every registered field exactly once")

    for name, profile in profiles.items():
        if not isinstance(name, str) or len(name.split(".")) != 2 or any(not part for part in name.split(".")):
            raise ModelValidationError(f"profile key must be a complete okftype: {name!r}")
        if not isinstance(profile, dict):
            raise ModelValidationError(f"profile {name} must be a mapping")
        profile_fields = _string_list(profile.get("fields", []), f"profile {name}.fields", allow_empty=True)
        unknown_fields = sorted(set(profile_fields) - set(fields))
        if unknown_fields:
            raise ModelValidationError(f"profile {name} has unknown fields: {', '.join(unknown_fields)}")
        outputs = _string_list(profile.get("outputs", []), f"profile {name}.outputs")
        unknown_outputs = sorted(set(outputs) - set(output_contracts))
        if unknown_outputs:
            raise ModelValidationError(f"profile {name} has unknown outputs: {', '.join(unknown_outputs)}")
        required = _string_list(profile.get("required", []), f"profile {name}.required", allow_empty=True)
        available = set(common) | set(profile_fields)
        unknown_required = sorted(set(required) - available)
        if unknown_required:
            raise ModelValidationError(f"profile {name} requires unregistered fields: {', '.join(unknown_required)}")

    roots = _string_list(source.get("document_roots"), "source_context.document_roots")
    context = source.get("context")
    if not isinstance(context, dict):
        raise ModelValidationError("source_context.context must be a mapping")
    family = context.get("family")
    if not isinstance(family, str) or not family:
        raise ModelValidationError("source_context.context requires family")
    if set(context) != {"family"}:
        raise ModelValidationError("source_context.context only supports family")
    if source.get("default_tags"):
        raise ModelValidationError("source_context no longer supports default_tags (derivable dimensions)")

    return {
        "document": document,
        "source": source,
        "roots": tuple(roots),
        "common_fields": tuple(common_fields),
        "common_required_fields": tuple(common),
        "field_order": tuple(field_order),
        "profiles": profiles,
        "output_contracts": output_contracts,
        "context": {"family": family},
    }


def document_is_in_scope(path: str, model: dict[str, Any] | None = None) -> bool:
    model = model or load_model()
    return any(path.startswith(root.rstrip("/") + "/") for root in model["roots"])


FUNCTION_SECTION_RE = re.compile(
    r"^\s{0,3}#{2,6}\s+函数(?:原型|声明)(?:\s|[<（(]|$)", re.MULTILINE
)
MARKDOWN_LINK_RE = re.compile(r"\[[^\]]+\]\([^)]+\.md(?:#[^)]*)?\)")
NAVIGATION_ENTRY_RE = re.compile(
    r"(?m)^\s*(?:[-*+]\s+|\d+[.)]\s+|\|).*\[[^\]]+\]\([^)]+\.md(?:#[^)]*)?\)"
)
DEFINITION_SECTION_RE = re.compile(
    r"^\s{0,3}#{2,6}\s+(?:宏定义|枚举类具体定义|结构体定义|联合体定义|类型定义|常量定义)",
    re.MULTILINE,
)
CONCEPT_HEADING_RE = re.compile(
    r"^\s{0,3}#{1,6}[^\n]*(?:概述|概念|特性|约束|说明|机制|简介|overview|feature|constraint|description)",
    re.IGNORECASE | re.MULTILINE,
)
def _prose_lines(body: str) -> list[str]:
    in_code = False
    lines = []
    for line in body.splitlines():
        stripped = line.strip()
        if stripped.startswith("```"):
            in_code = not in_code
            continue
        if in_code or not stripped or stripped.startswith(("#", "|", "- ", "* ", "+ ", ">", "<!--", "![")):
            continue
        if re.match(r"\d+[.)]\s+", stripped):
            continue
        prose = MARKDOWN_LINK_RE.sub("", stripped).strip(" `*_[]():.。；;，,")
        if len(prose) >= 8:
            lines.append(prose)
    return lines


def classify_document(path: str, body: str, model: dict[str, Any] | None = None) -> Classification | None:
    """Return one flat okftype using universal document-structure signals."""
    model = model or load_model()
    if not document_is_in_scope(path, model):
        return None
    kind, rule = _classify_kind(path, body)
    context = model["context"]
    okftype = f"{context['family']}.{kind}"
    if okftype not in model["profiles"]:
        return None
    return Classification(okftype=okftype, rule=rule)


def _classify_kind(path: str, body: str) -> tuple[str | None, str]:
    relative = Path(path)
    parts = relative.parts
    if (
        len(parts) >= 4
        and parts[-4:-2] == ("c_api", "defs")
        and parts[-2] in {"constant", "enum", "macro", "type", "union"}
        and relative.stem != parts[-2]
    ):
        if parts[-2] == "type" and not re.search(r"(?m)^\|\s*b(?:8|16|32|64)\s*\|", body):
            return "concept", "prose_structure"
        return parts[-2], "definition_structure"
    if FUNCTION_SECTION_RE.search(body) or DEFINITION_SECTION_RE.search(body) and re.search(r"#define|\benum\b|\b(struct|union|typedef|using)\b", body):
        return "api", "callable_structure"
    has_sections = len(re.findall(r"(?m)^##\s+", body)) >= 2
    prose = _prose_lines(body)
    has_prose = bool(prose)
    if any(len(line) >= 50 for line in prose) or (CONCEPT_HEADING_RE.search(body) and has_prose):
        return "concept", "concept_structure"
    if has_sections and has_prose:
        return "concept", "concept_structure"
    if NAVIGATION_ENTRY_RE.search(body):
        return "index", "navigation_structure"
    if has_prose and not MARKDOWN_LINK_RE.search(body):
        return "concept", "prose_structure"
    return None, "insufficient_structure"


def compile_execution_plan(
    classification: Classification,
    *,
    model: dict[str, Any] | None = None,
) -> ExecutionPlan:
    """Compile one flat Profile into fields and output contracts."""
    model = model or load_model()
    profile = model["profiles"].get(classification.okftype)
    if not isinstance(profile, dict):
        raise ModelValidationError(f"unknown profile: {classification.okftype}")
    selected = set(model["common_required_fields"])
    selected.update(profile.get("fields", []))
    fields = [field for field in model["field_order"] if field in selected]
    required = set(model["common_required_fields"])
    required.update(profile.get("required", []))
    return ExecutionPlan(
        okftype=classification.okftype,
        rule=classification.rule,
        fields=tuple(_ordered_unique(fields)),
        required=tuple(f for f in model["field_order"] if f in required),
        outputs=tuple(profile["outputs"]),
    )


def plan_to_dict(plan: ExecutionPlan) -> dict[str, Any]:
    return {
        "okftype": plan.okftype,
        "rule": plan.rule,
        "fields": list(plan.fields),
        "required": list(plan.required),
        "outputs": list(plan.outputs),
    }


def _ordered_unique(values: list[str]) -> list[str]:
    return list(dict.fromkeys(values))
