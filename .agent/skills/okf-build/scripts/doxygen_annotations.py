"""Header Doxygen planning rules aligned with api-yaml-metadata-guard.

Records identify a concrete declaration or a function macro.  Callers never
provide source offsets, anchor text, or a rendered comment: those are derived
from the verified declaration in the current header text.
"""

from __future__ import annotations

from functools import lru_cache
from pathlib import Path
import re
from typing import Any, Mapping, Sequence

from extract_api_yaml_metadata import (
    _canonical_signature_id,
    _normalized_parameter_list,
    _parameter_name,
    _parse_prototype_declaration,
    _top_level_parameter_parts,
    _without_comments_and_literals,
)
from prototype_id import MACRO_ID_RE, matches_override_id, signature_identifier


class DoxygenValidationError(ValueError):
    pass


def api_group(path: str) -> str:
    parts = Path(path).parts
    try:
        index = parts.index("SIMD-API")
    except ValueError:
        values = [re.sub(r"[^A-Za-z0-9_]+", "_", part).strip("_").lower() for part in parts[:-1]]
        return ".".join(value for value in values if value) or "api"
    remainder = parts[index + 1 : -1]
    if remainder and remainder[0] == "c_api":
        prefix = ["simd_c_api"]
        group_parts = remainder[1:]
    else:
        prefix = ["simd_api"]
        group_parts = remainder
    values = [
        re.sub(r"[^A-Za-z0-9_]+", "_", part).strip("_").lower()
        for part in (*prefix, *group_parts)
    ]
    return ".".join(value for value in values if value)


def _macro_parameter_names(value: str) -> tuple[str, ...]:
    if "(" not in value or not value.endswith(")"):
        return ()
    _, parameters = value.split("(", 1)
    names = tuple(parameter.strip() for parameter in _top_level_parameter_parts(parameters[:-1]))
    if not names or any(re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name) is None for name in names):
        return ()
    return names


def _signature_parameter_names(value: str) -> tuple[str, ...]:
    declaration = _parse_signature(value)
    if declaration is None:
        return ()
    return tuple(
        _parameter_name(parameter)
        for parameter in _normalized_parameter_list(declaration.parameters or "", keep_names=True)
    )


def _parse_signature(value: str) -> Any | None:
    return _parse_prototype_declaration(value) or _parse_prototype_declaration(
        f"__aicore__ inline void {value};"
    )


def _signature_type_key(declaration: Any) -> str:
    """Return the signature identity used for header matching.

    Markdown signatures may retain parameter names while declarations parsed
    from headers may not.  Normalize both through the same parameter parser;
    names are checked separately by ``_signature_parameter_names``.
    """
    parameters = []
    for parameter in _normalized_parameter_list(declaration.parameters or "", keep_names=True):
        name = _parameter_name(parameter)
        if name and re.search(rf"(?:[*&]\s*|\s+){re.escape(name)}$", parameter):
            parameter = parameter[:-len(name)].rstrip()
        parameters.append(parameter)
    return f"{declaration.symbol}({','.join(parameters)})"


@lru_cache(maxsize=256)
def _all_header_declaration_ranges(
    header_text: str,
) -> tuple[tuple[str, int, int, str], ...]:
    """Parse accelerator declarations once for every immutable header text."""
    source_text = _without_comments_and_literals(header_text)
    pattern = re.compile(
        r"\b(?:__aicore__|__simd_callee__)\b(?:(?!;).)*?"
        r"\b(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)\s*\(",
        re.DOTALL,
    )
    ranges = []
    for match in pattern.finditer(source_text):
        statement_start = source_text.rfind("\n", 0, match.start()) + 1
        opening = source_text.find("(", match.start())
        depth = 0
        end = None
        for index in range(opening, len(source_text)):
            character = source_text[index]
            if character == "(":
                depth += 1
            elif character == ")":
                depth -= 1
                if depth == 0:
                    semicolon = source_text.find(";", index)
                    brace = source_text.find("{", index)
                    candidates = [value for value in (semicolon, brace) if value >= 0]
                    end = min(candidates) + 1 if candidates else index + 1
                    break
        if end is not None:
            ranges.append((
                match.group("symbol"),
                statement_start,
                end,
                header_text[statement_start:end],
            ))
    return tuple(ranges)


def _header_declaration_ranges(header_text: str, symbol: str) -> list[tuple[int, int, str]]:
    return [
        (start, end, text)
        for found_symbol, start, end, text in _all_header_declaration_ranges(header_text)
        if found_symbol == symbol
    ]


def _matching_declarations(header_text: str, function: str, parameters: str | None) -> list[tuple[int, int, str]]:
    declaration = _parse_signature(function)
    if declaration is None:
        return []
    # Header parsing canonicalizes parameter types without parameter names,
    # while the Markdown mapping may retain names in its canonical string.
    # Compare parsed canonical signatures so both paths identify the same
    # declaration; parameter names remain checked separately below.
    canonical_function = _signature_type_key(declaration)
    desired_names = _signature_parameter_names(parameters or "")
    matches = []
    for start, end, text in _header_declaration_ranges(header_text, declaration.symbol):
        parsed = _parse_prototype_declaration(re.sub(r"\s+", " ", text).strip().rstrip(";{"))
        parsed_function = _signature_type_key(parsed) if parsed is not None else None
        if parsed is None or parsed_function != canonical_function:
            continue
        if desired_names and _signature_parameter_names(text) != desired_names:
            continue
        matches.append((start, end, text))
    return matches


def _matching_macros(header_text: str, function: str, parameters: str | None) -> list[tuple[int, int, str]]:
    if "(" not in function or not function.endswith(")"):
        return []
    symbol, _ = function.split("(", 1)
    expected_names = _macro_parameter_names(parameters or function)
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol) is None or not expected_names:
        return []
    pattern = re.compile(
        rf"^[ \t]*#\s*define\s+{re.escape(symbol)}\s*\((?P<parameters>[^)\n]*)\)",
        re.MULTILINE,
    )
    matches = []
    for match in pattern.finditer(header_text):
        if _macro_parameter_names(f"{symbol}({match.group('parameters')})") != expected_names:
            continue
        line_end = header_text.find("\n", match.end())
        matches.append((match.start(), len(header_text) if line_end < 0 else line_end + 1, match.group(0)))
    return matches


def _matching_targets(header_text: str, function: str, parameters: str | None, macro_id: str | None) -> list[tuple[int, int, str]]:
    return _matching_macros(header_text, function, parameters) if macro_id else _matching_declarations(header_text, function, parameters)


def _doxygen_block_start(text: str, declaration_start: int) -> int | None:
    prefix = text[:declaration_start]
    block_start = prefix.rfind("/**")
    if block_start < 0:
        return None
    block_end = prefix.find("*/", block_start)
    if block_end < 0 or re.fullmatch(r"[ \t]*(?:\r?\n)?[ \t]*", prefix[block_end + 2 :]) is None:
        return None
    return block_start


def _render_base_block(group: str, identifier: str, archs: Sequence[str], identifier_tag: str) -> str:
    lines = ["/**", f" * @ingroup {group}", f" * {identifier_tag} {identifier}"]
    if archs:
        lines.append(f" * @archs {' '.join(archs)}")
    lines.append(" */")
    return "\n".join(lines) + "\n"


def _replace_base_tags(block: str, group: str, identifier: str, archs: Sequence[str], identifier_tag: str) -> str:
    desired = {"@ingroup": f" * @ingroup {group}", identifier_tag: f" * {identifier_tag} {identifier}"}
    if archs:
        desired["@archs"] = f" * @archs {' '.join(archs)}"
    present: set[str] = set()
    lines = []
    for line in block.splitlines():
        stripped = line.strip()
        tag_text = stripped[1:].strip() if stripped.startswith("*") else stripped
        tag = next((name for name in ("@ingroup", "@override_id", "@macro_id", "@archs") if tag_text.startswith(name)), None)
        if tag_text.startswith("@function"):
            tag = identifier_tag
        if tag is not None:
            replacement = desired.get(tag)
            if replacement is not None:
                lines.append(replacement)
                present.add(tag)
        else:
            lines.append(line)
    insert_at = next((index for index in range(len(lines) - 1, -1, -1) if lines[index].strip() == "*/"), len(lines))
    lines[insert_at:insert_at] = [
        desired[tag] for tag in ("@ingroup", identifier_tag, "@archs") if tag in desired and tag not in present
    ]
    return "\n".join(lines) + "\n"


def _validate_evidence(value: object, location: str) -> list[str]:
    if not isinstance(value, list) or not value or any(not isinstance(item, str) or not item.strip() for item in value):
        raise DoxygenValidationError(f"{location}.evidence must be a non-empty string list")
    return value


def _validate_doxygen(value: object, location: str) -> dict[str, Any]:
    if value in ({}, None):
        return {}
    if not isinstance(value, dict) or not value:
        raise DoxygenValidationError(f"{location}.doxygen must be an object")
    if unknown := sorted(set(value) - {"level", "brief", "params"}):
        raise DoxygenValidationError(f"{location}.doxygen has unknown fields: {', '.join(unknown)}")
    normalized: dict[str, Any] = {}
    level = value.get("level")
    if level is not None:
        if not isinstance(level, dict) or set(level) - {"value", "evidence", "confidence"}:
            raise DoxygenValidationError(f"{location}.doxygen.level has invalid fields")
        if not isinstance(level.get("value"), int) or level["value"] < 0:
            raise DoxygenValidationError(f"{location}.doxygen.level.value must be a non-negative integer")
        if not isinstance(level.get("confidence"), (int, float)) or not 0 <= level["confidence"] <= 1:
            raise DoxygenValidationError(f"{location}.doxygen.level.confidence must be between 0 and 1")
        _validate_evidence(level.get("evidence"), f"{location}.doxygen.level")
        normalized["level"] = level
    brief = value.get("brief")
    if brief is not None:
        if not isinstance(brief, dict) or set(brief) - {"text", "evidence"}:
            raise DoxygenValidationError(f"{location}.doxygen.brief has invalid fields")
        if not isinstance(brief.get("text"), str) or not brief["text"].strip() or "\n" in brief["text"]:
            raise DoxygenValidationError(f"{location}.doxygen.brief.text must be a non-empty single line")
        _validate_evidence(brief.get("evidence"), f"{location}.doxygen.brief")
        normalized["brief"] = brief
    params = value.get("params")
    if params is not None:
        if not isinstance(params, list) or not params:
            raise DoxygenValidationError(f"{location}.doxygen.params must be a non-empty list")
        names: set[str] = set()
        normalized_params = []
        for index, parameter in enumerate(params):
            item_location = f"{location}.doxygen.params[{index}]"
            if not isinstance(parameter, dict) or set(parameter) - {"name", "direction", "description", "evidence"}:
                raise DoxygenValidationError(f"{item_location} has invalid fields")
            if not isinstance(parameter.get("name"), str) or re.fullmatch(r"[A-Za-z_]\w*", parameter["name"]) is None or parameter["name"] in names:
                raise DoxygenValidationError(f"{item_location}.name must be a unique C++ parameter name")
            if parameter.get("direction") not in {"in", "out", "in,out"}:
                raise DoxygenValidationError(f"{item_location}.direction must be in, out, or in,out")
            if not isinstance(parameter.get("description"), str) or not parameter["description"].strip() or "\n" in parameter["description"]:
                raise DoxygenValidationError(f"{item_location}.description must be a non-empty single line")
            _validate_evidence(parameter.get("evidence"), item_location)
            names.add(parameter["name"])
            normalized_params.append(parameter)
        normalized["params"] = normalized_params
    return normalized


def _insert_missing_tags(block: str, doxygen: Mapping[str, Any]) -> str:
    tags = {match.group(1) for match in re.finditer(r"^\s*\*\s*(@\w+)\b", block, re.MULTILINE)}
    existing_params = {
        match.group(1)
        for match in re.finditer(r"\s*\*\s*@param(?:\s+\[[^]]+\])?\s+(\S+)", block)
    }
    additions = []
    if (level := doxygen.get("level")) is not None and "@level" not in tags:
        additions.append(f" * @level {level['value']}")
    if (brief := doxygen.get("brief")) is not None and "@brief" not in tags:
        additions.append(f" * @brief {brief['text'].strip()}")
    for parameter in doxygen.get("params", []):
        if parameter["name"] not in existing_params:
            additions.append(f" * @param [{parameter['direction']}] {parameter['name']} {parameter['description'].strip()}")
    if not additions:
        return block
    lines = block.splitlines()
    insert_at = next((index for index in range(len(lines) - 1, -1, -1) if lines[index].strip() == "*/"), None)
    if insert_at is None:
        raise DoxygenValidationError("broken Doxygen block")
    lines[insert_at:insert_at] = additions
    return "\n".join(lines) + "\n"


def validate_record_target(header_text: str, record: Mapping[str, Any]) -> None:
    """Validate one record against the unmodified header declaration.

    Controller preflight needs to isolate records that cannot be located, but
    does not need to render a comment for each one.  Keeping this check
    read-only avoids repeatedly reparsing a large header after a temporary
    Doxygen insertion.
    """
    allowed = {
        "source_document", "header", "function", "parameters", "macro_id",
        "override_id", "archs", "template_parameters", "doxygen",
    }
    if unknown := sorted(set(record) - allowed):
        raise DoxygenValidationError(
            f"doxygen target has unknown fields: {', '.join(unknown)}"
        )
    source = record.get("source_document")
    function = record.get("function")
    parameters = record.get("parameters")
    macro_id = record.get("macro_id")
    override_id = record.get("override_id")
    archs = record.get("archs", [])
    templates = record.get("template_parameters", [])
    if not isinstance(source, str) or not source.endswith(".md"):
        raise DoxygenValidationError("doxygen target source_document must be a Markdown path")
    if not isinstance(function, str) or _parse_signature(function) is None:
        raise DoxygenValidationError("doxygen target function must be a canonical function signature")
    if not isinstance(parameters, str) or _parse_signature(parameters) is None:
        raise DoxygenValidationError("doxygen target parameters must be a function declaration")
    if not isinstance(archs, list) or any(not isinstance(value, str) for value in archs):
        raise DoxygenValidationError("doxygen target archs must be a string list")
    if not isinstance(templates, list) or any(
        re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", str(value)) is None
        for value in templates
    ):
        raise DoxygenValidationError("doxygen target template_parameters must be C++ identifiers")
    if macro_id is not None and (
        not isinstance(macro_id, str) or MACRO_ID_RE.fullmatch(macro_id) is None
    ):
        raise DoxygenValidationError("doxygen target macro_id has invalid format")
    if override_id is not None and not isinstance(override_id, str):
        raise DoxygenValidationError("doxygen target override_id must be a string")
    if macro_id is not None and override_id is not None:
        raise DoxygenValidationError("doxygen target cannot contain both macro_id and override_id")
    doxygen = _validate_doxygen(record.get("doxygen"), "doxygen target")
    matches = _matching_targets(header_text, function, parameters, macro_id)
    if len(matches) != 1:
        raise DoxygenValidationError(
            f"doxygen target is {'unmatched' if not matches else 'ambiguous'}: {function}"
        )
    if macro_id:
        header_parameters = set(_macro_parameter_names(function))
    else:
        header_parameters = set(_signature_parameter_names(matches[0][2]))
        key, canonical_identifier = signature_identifier(
            function, template_parameters=tuple(templates)
        )
        if key != "override_id":
            raise DoxygenValidationError("doxygen target cannot annotate a non-concrete header function")
        if override_id is not None and not matches_override_id(
            override_id, canonical_identifier
        ):
            raise DoxygenValidationError(
                "doxygen target override_id does not match the concrete header function"
            )
    unknown_params = [
        item["name"]
        for item in doxygen.get("params", [])
        if item["name"] not in header_parameters
    ]
    if unknown_params:
        raise DoxygenValidationError(
            "doxygen target params reference unknown header parameter: "
            + ", ".join(unknown_params)
        )


def apply_records_to_text(header_text: str, records: Sequence[Mapping[str, Any]]) -> tuple[str, int]:
    """Apply base linkage and missing evidence-backed tags to one header text."""
    text = header_text
    applied = 0
    used_targets: dict[tuple[str, str], dict[str, Any]] = {}
    for index, record in enumerate(records):
        location = f"doxygen[{index}]"
        allowed = {"source_document", "header", "function", "parameters", "macro_id", "override_id", "archs", "template_parameters", "doxygen"}
        if unknown := sorted(set(record) - allowed):
            raise DoxygenValidationError(f"{location} has unknown fields: {', '.join(unknown)}")
        source = record.get("source_document")
        function = record.get("function")
        parameters = record.get("parameters")
        macro_id = record.get("macro_id")
        override_id = record.get("override_id")
        archs = record.get("archs", [])
        templates = record.get("template_parameters", [])
        if not isinstance(source, str) or not source.endswith(".md"):
            raise DoxygenValidationError(f"{location}.source_document must be a Markdown path")
        if not isinstance(function, str) or _parse_signature(function) is None:
            raise DoxygenValidationError(f"{location}.function must be a canonical function signature")
        if not isinstance(parameters, str) or _parse_signature(parameters) is None:
            raise DoxygenValidationError(f"{location}.parameters must be a function declaration")
        if not isinstance(archs, list) or any(not isinstance(value, str) for value in archs):
            raise DoxygenValidationError(f"{location}.archs must be a string list")
        if not isinstance(templates, list) or any(re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", str(value)) is None for value in templates):
            raise DoxygenValidationError(f"{location}.template_parameters must be C++ identifiers")
        if macro_id is not None and (not isinstance(macro_id, str) or MACRO_ID_RE.fullmatch(macro_id) is None):
            raise DoxygenValidationError(f"{location}.macro_id has invalid format")
        if override_id is not None and not isinstance(override_id, str):
            raise DoxygenValidationError(f"{location}.override_id must be a string")
        if macro_id is not None and override_id is not None:
            raise DoxygenValidationError(f"{location} cannot contain both macro_id and override_id")
        doxygen = _validate_doxygen(record.get("doxygen"), location)
        matches = _matching_targets(text, function, parameters, macro_id)
        if len(matches) != 1:
            raise DoxygenValidationError(f"{location} target is {'unmatched' if not matches else 'ambiguous'}: {function}")
        declaration_start = matches[0][0]
        declaration_identity = re.sub(r"\s+", " ", matches[0][2]).strip()
        target_key = (declaration_identity, str(macro_id or function))
        if macro_id:
            canonical_identifier, identifier_tag = macro_id, "@macro_id"
        else:
            key, canonical_identifier = signature_identifier(function, template_parameters=tuple(templates))
            if key != "override_id":
                raise DoxygenValidationError(f"{location} cannot annotate a non-concrete header function")
            if override_id is not None and not matches_override_id(override_id, canonical_identifier):
                raise DoxygenValidationError(
                    f"{location}.override_id does not match the concrete header function"
                )
            identifier_tag = "@override_id"
        identifier = override_id or canonical_identifier
        previous = used_targets.get(target_key)
        if previous is not None:
            if (
                previous["canonical_identifier"] != canonical_identifier
                or previous["identifier_tag"] != identifier_tag
                or previous["doxygen"] != doxygen
            ):
                raise DoxygenValidationError(f"{location} duplicates a header declaration target")
            archs = sorted(set(previous["archs"]) | set(archs))
            # One physical declaration is shared across architecture documents.
            # Preserve the legacy canonical include identity and aggregate archs.
            identifier = canonical_identifier
            previous["archs"] = archs
        else:
            used_targets[target_key] = {
                "archs": list(archs),
                "canonical_identifier": canonical_identifier,
                "identifier_tag": identifier_tag,
                "doxygen": doxygen,
            }
        block_start = _doxygen_block_start(text, declaration_start)
        group = api_group(source)
        if block_start is None:
            text = text[:declaration_start] + _render_base_block(group, identifier, archs, identifier_tag) + text[declaration_start:]
            declaration_start += len(_render_base_block(group, identifier, archs, identifier_tag))
            block_start = declaration_start - len(_render_base_block(group, identifier, archs, identifier_tag))
        else:
            block_end = text.find("*/", block_start) + 2
            while block_end < len(text) and text[block_end] in " \t":
                block_end += 1
            if block_end < len(text) and text[block_end] == "\n":
                block_end += 1
            text = text[:block_start] + _replace_base_tags(text[block_start:block_end], group, identifier, archs, identifier_tag) + text[block_end:]
        if doxygen:
            block_end = text.find("*/", block_start) + 2
            while block_end < len(text) and text[block_end] in " \t":
                block_end += 1
            if block_end < len(text) and text[block_end] == "\n":
                block_end += 1
            header_parameters = set(
                _macro_parameter_names(function)
                if macro_id
                else _signature_parameter_names(matches[0][2])
            )
            unknown_params = [item["name"] for item in doxygen.get("params", []) if item["name"] not in header_parameters]
            if unknown_params:
                raise DoxygenValidationError(f"{location}.doxygen.params references unknown header parameter: {', '.join(unknown_params)}")
            text = text[:block_start] + _insert_missing_tags(text[block_start:block_end], doxygen) + text[block_end:]
        applied += 1
    return text, applied
