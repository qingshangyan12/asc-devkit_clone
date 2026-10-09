"""Deterministic identifiers for documented API prototypes."""

from __future__ import annotations

from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path
import re
from typing import Any, Sequence

import yaml


PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REGISTRY_PATH = (
    PROJECT_ROOT / "config" / "prototype-id" / "type-registry.yaml"
)
PLACEHOLDER_RE = re.compile(r"<(?P<name>[A-Za-z_][A-Za-z0-9_]*)>")
ARCHITECTURE_SUFFIX_RE = re.compile(r"^[0-9]+$")
DOCUMENTED_PLACEHOLDER_RE = re.compile(
    r"(?<![A-Za-z0-9:])<(?P<name>[A-Za-z_][A-Za-z0-9_]*)>"
)
GENERIC_TYPE_RE = re.compile(
    r"(?P<name>[A-Za-z_][A-Za-z0-9_:]*)<(?P<arguments>.*)>$"
)
TEMPLATE_VARIABLE_RE = re.compile(r"[STU](?:[0-9]+)?$")
# C++ subscript overloads occur in Basic API tensor documents. Keep the grammar
# deliberately narrow so arbitrary punctuation is not accepted as a function name.
FUNCTION_SYMBOL_RE = r"(?:[A-Za-z_][A-Za-z0-9_]*|operator\[\])"
MACRO_ID_RE = re.compile(
    r"[A-Za-z_][A-Za-z0-9_]*(?:__[A-Za-z_][A-Za-z0-9_]*)+"
)
TYPE_PREFIX_RE = re.compile(
    r"(?P<qualifier>const|volatile|restrict)\s+"
    r"|(?P<address_space>__[A-Za-z0-9_]+__)(?:\s+|(?=<[A-Za-z_][A-Za-z0-9_]*>))"
)

OPERATORS = {
    "lvalue_reference": "R",
    "rvalue_reference": "O",
    "pointer": "P",
    "const": "C",
    "volatile": "V",
    "restrict": "X",
    "vector": "v",
    "array": "A",
    "parameter_pack": "Q",
}
ADDRESS_SPACES = {
    "__gm__": "G",
    "__ubuf__": "U",
    "__cbuf__": "L1",
    "__fbuf__": "FB",
    "__ca__": "CA",
    "__cb__": "CB",
    "__cc__": "CC",
}
ARRAY_BOUNDS = {
    "ASC_C_API_MRGSORT_ELEMENT_LEN": "mrg",
    "ASC_VDEQ_SIZE": "vdeq",
    "ASC_VREDUCE_ELEMENT_LEN": "reduce",
}
NO_PARAMETERS = "z"


class PrototypeIdError(ValueError):
    """Raised when a prototype cannot be represented by the type registry."""


def architecture_override_id(identifier: str, architecture: str) -> str:
    """Qualify one concrete override ID for an architecture-specific document."""
    if not isinstance(identifier, str) or not identifier or PLACEHOLDER_RE.search(identifier):
        raise PrototypeIdError("architecture suffix requires a concrete override ID")
    if not isinstance(architecture, str) or ARCHITECTURE_SUFFIX_RE.fullmatch(architecture) is None:
        raise PrototypeIdError("architecture suffix must contain only digits")
    return f"{identifier}@{architecture}"


def matches_override_id(candidate: object, canonical: str) -> bool:
    """Return whether ``candidate`` is canonical or its allowed architecture variant."""
    if not isinstance(candidate, str):
        return False
    if candidate == canonical:
        return True
    prefix = f"{canonical}@"
    return (
        candidate.startswith(prefix)
        and ARCHITECTURE_SUFFIX_RE.fullmatch(candidate[len(prefix) :]) is not None
    )


@dataclass(frozen=True)
class TypeRegistryProposal:
    """One safe atomic type addition proposed from documented prototypes."""

    type_name: str
    abbreviation: str
    sources: tuple[str, ...]


def _top_level_parts(value: str) -> tuple[str, ...]:
    parts: list[str] = []
    start = 0
    angle = paren = bracket = 0
    for index, char in enumerate(value):
        if char == "<":
            angle += 1
        elif char == ">" and angle:
            angle -= 1
        elif char == "(":
            paren += 1
        elif char == ")" and paren:
            paren -= 1
        elif char == "[":
            bracket += 1
        elif char == "]" and bracket:
            bracket -= 1
        elif char == "," and not angle and not paren and not bracket:
            parts.append(value[start:index].strip())
            start = index + 1
    tail = value[start:].strip()
    if tail:
        parts.append(tail)
    return tuple(parts)


@lru_cache(maxsize=None)
def load_type_registry(path: Path = DEFAULT_REGISTRY_PATH) -> dict[str, Any]:
    data = yaml.safe_load(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict) or data.get("schema_version") != 1:
        raise PrototypeIdError(f"invalid type registry: {path}")
    required = {"types", "aliases", "exact_types"}
    missing = sorted(required - set(data))
    if missing:
        raise PrototypeIdError(
            f"type registry is missing fields: {', '.join(missing)}"
        )
    unexpected = sorted(set(data) - {"schema_version", *required, "template_types"})
    if unexpected:
        raise PrototypeIdError(
            f"type registry has unsupported fields: {', '.join(unexpected)}"
        )
    for field in ("types", "exact_types"):
        mapping = data[field]
        if not isinstance(mapping, dict) or any(
            not isinstance(name, str)
            or not name
            or not isinstance(code, str)
            or not code
            for name, code in mapping.items()
        ):
            raise PrototypeIdError(
                f"type registry field {field!r} must map names to non-empty strings"
            )
        duplicates = sorted(
            {code for code in mapping.values() if list(mapping.values()).count(code) > 1}
        )
        if duplicates:
            raise PrototypeIdError(
                f"type registry field {field!r} has duplicate abbreviations: "
                + ", ".join(duplicates)
            )
    template_types = data.get("template_types", {})
    if not isinstance(template_types, dict) or any(
        not isinstance(name, str)
        or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_:]*", name)
        or not isinstance(code, str)
        or not code
        for name, code in template_types.items()
    ):
        raise PrototypeIdError(
            "type registry field 'template_types' must map C++ type names to non-empty codes"
        )
    duplicate_template_codes = sorted(
        {
            code
            for code in template_types.values()
            if list(template_types.values()).count(code) > 1
        }
    )
    if duplicate_template_codes:
        raise PrototypeIdError(
            "type registry field 'template_types' has duplicate abbreviations: "
            + ", ".join(duplicate_template_codes)
        )
    type_codes = set(data["types"].values())
    exact_codes = set(data["exact_types"].values())
    template_codes = set(template_types.values())
    shared_abbreviations = sorted(
        (type_codes & exact_codes)
        | (type_codes & template_codes)
        | (exact_codes & template_codes)
    )
    if shared_abbreviations:
        raise PrototypeIdError(
            "type registry has abbreviations shared by types, exact_types, or template_types: "
            + ", ".join(shared_abbreviations)
        )
    aliases = data["aliases"]
    if not isinstance(aliases, dict) or any(
        not isinstance(alias, str)
        or not alias
        or not isinstance(target, str)
        or not target
        for alias, target in aliases.items()
    ):
        raise PrototypeIdError(
            "type registry field 'aliases' must map names to registered type names"
        )
    overlap = sorted(set(aliases) & set(data["types"]))
    if overlap:
        raise PrototypeIdError(
            "type registry aliases also declared as types: " + ", ".join(overlap)
        )
    unknown_targets = sorted(set(aliases.values()) - set(data["types"]))
    if unknown_targets:
        raise PrototypeIdError(
            "type registry aliases reference unknown types: "
            + ", ".join(unknown_targets)
        )
    return data


def _unregistered_type_expressions(value: str, registry: dict[str, Any]) -> tuple[str, ...]:
    """Return type atoms that the normal encoder cannot resolve from ``value``."""
    value = re.sub(r"\s+", " ", _strip_parameter_name(value.strip()))
    if value in registry["exact_types"]:
        return ()

    array_match = re.fullmatch(r"(?P<type>.+?)(?P<arrays>(?:\[[^\]]+\])+)", value)
    if array_match:
        return _unregistered_type_expressions(array_match.group("type"), registry)

    while True:
        suffix_match = re.search(r"(?P<suffix>&&|[&*])\s*$", value)
        if not suffix_match:
            break
        value = value[: suffix_match.start()].strip()

    while True:
        prefix_match = TYPE_PREFIX_RE.match(value)
        if not prefix_match:
            break
        value = value[prefix_match.end() :].strip()

    if PLACEHOLDER_RE.fullmatch(value):
        return ()
    if TEMPLATE_VARIABLE_RE.fullmatch(value):
        return ()
    vector_match = re.fullmatch(r"vector_(?P<element>.+)", value)
    if vector_match:
        return _unregistered_type_expressions(vector_match.group("element"), registry)

    generic_match = GENERIC_TYPE_RE.fullmatch(value)
    if generic_match:
        base = registry["aliases"].get(generic_match.group("name"), generic_match.group("name"))
        if base in registry.get("template_types", {}):
            arguments = _top_level_parts(generic_match.group("arguments"))
            if not arguments:
                return (value,)
            return tuple(
                expression
                for argument in arguments
                for expression in _unregistered_type_expressions(argument, registry)
            )

    canonical = registry["aliases"].get(value, value)
    return () if canonical in registry["types"] else (value,)


def _suggested_type_abbreviation(type_name: str, occupied: set[str]) -> str:
    """Make a readable, deterministic abbreviation without reusing an existing code."""
    normalized = re.sub(r"(?<=[a-z0-9])(?=[A-Z])", "_", type_name)
    if normalized.startswith("asc_"):
        normalized = normalized[4:]
    parts = [part.lower() for part in normalized.split("_") if part]
    if parts and parts[-1] in {"config", "configuration"}:
        parts[-1] = "cfg"
    base = "".join(parts)
    if not base:
        raise PrototypeIdError(f"cannot suggest an abbreviation for {type_name!r}")
    candidate = base
    suffix = 2
    while candidate in occupied:
        candidate = f"{base}{suffix}"
        suffix += 1
    occupied.add(candidate)
    return candidate


def propose_type_registry_extensions(
    functions: Sequence[tuple[str, str | None]],
    *,
    registry_path: Path = DEFAULT_REGISTRY_PATH,
) -> tuple[tuple[TypeRegistryProposal, ...], dict[str, tuple[str, ...]]]:
    """Find safe registry additions and expressions that require manual handling.

    ``functions`` contains canonical prototypes and optional source labels. Simple C/C++
    identifiers can be registered as type atoms. More complex expressions are returned
    separately because they must be reviewed as an ``exact_types`` entry or parsed by a
    new grammar rule.
    """
    registry = load_type_registry(registry_path)
    sources_by_type: dict[str, set[str]] = {}
    manual_sources: dict[str, set[str]] = {}

    for function, source in functions:
        match = re.fullmatch(
            rf"(?P<symbol>{FUNCTION_SYMBOL_RE})\((?P<parameters>.*)\)",
            function.strip(),
        )
        if not match:
            raise PrototypeIdError(f"invalid canonical function signature {function!r}")
        label = source or function
        for parameter in _top_level_parts(match.group("parameters")):
            for expression in _unregistered_type_expressions(parameter, registry):
                destination = (
                    sources_by_type
                    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", expression)
                    else manual_sources
                )
                destination.setdefault(expression, set()).add(label)

    occupied = (
        set(registry["types"].values())
        | set(registry["exact_types"].values())
        | set(registry.get("template_types", {}).values())
    )
    proposals = tuple(
        TypeRegistryProposal(
            type_name=type_name,
            abbreviation=_suggested_type_abbreviation(type_name, occupied),
            sources=tuple(sorted(sources)),
        )
        for type_name, sources in sorted(sources_by_type.items())
    )
    manual = {
        expression: tuple(sorted(sources))
        for expression, sources in sorted(manual_sources.items())
    }
    return proposals, manual


def write_type_registry_extensions(
    proposals: Sequence[TypeRegistryProposal],
    *,
    registry_path: Path = DEFAULT_REGISTRY_PATH,
) -> tuple[TypeRegistryProposal, ...]:
    """Persist reviewed safe proposals while preserving registry invariants."""
    if not proposals:
        return ()
    data = yaml.safe_load(registry_path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise PrototypeIdError(f"invalid type registry: {registry_path}")
    types = data.get("types")
    exact_types = data.get("exact_types")
    aliases = data.get("aliases")
    if (
        not isinstance(types, dict)
        or not isinstance(exact_types, dict)
        or not isinstance(aliases, dict)
    ):
        raise PrototypeIdError(f"invalid type registry: {registry_path}")

    template_types = data.get("template_types", {})
    if not isinstance(template_types, dict):
        raise PrototypeIdError(f"invalid type registry: {registry_path}")
    occupied = set(types.values()) | set(exact_types.values()) | set(template_types.values())
    added = []
    for proposal in proposals:
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", proposal.type_name):
            raise PrototypeIdError(
                f"registry proposal is not a simple type identifier: {proposal.type_name!r}"
            )
        existing = types.get(proposal.type_name)
        if existing is not None:
            if existing != proposal.abbreviation:
                raise PrototypeIdError(
                    f"registry type {proposal.type_name!r} already uses {existing!r}"
                )
            continue
        if proposal.type_name in aliases:
            raise PrototypeIdError(
                f"registry proposal duplicates alias {proposal.type_name!r}"
            )
        if proposal.abbreviation in occupied:
            raise PrototypeIdError(
                f"registry abbreviation already exists: {proposal.abbreviation!r}"
            )
        types[proposal.type_name] = proposal.abbreviation
        occupied.add(proposal.abbreviation)
        added.append(proposal)

    if added:
        registry_path.write_text(
            yaml.safe_dump(
                data,
                allow_unicode=False,
                default_flow_style=False,
                sort_keys=False,
            ),
            encoding="utf-8",
        )
        load_type_registry.cache_clear()
        load_type_registry(registry_path)
    return tuple(added)


def _strip_parameter_name(value: str) -> str:
    array_match = re.fullmatch(
        r"(?P<type>.+?)(?:\s+|(?<=[*&]))[A-Za-z_][A-Za-z0-9_]*"
        r"(?P<array>(?:\[[^\]]*\])+)",
        value,
    )
    if array_match:
        return f"{array_match.group('type').strip()}{array_match.group('array')}"
    pointer_name_match = re.fullmatch(
        r"(?P<type>.+[*&])\s*[A-Za-z_][A-Za-z0-9_]*",
        value,
    )
    if pointer_name_match:
        return pointer_name_match.group("type").strip()
    template_name_match = re.fullmatch(
        r"(?P<type>.+>)\s*[A-Za-z_][A-Za-z0-9_]*",
        value,
    )
    if template_name_match:
        return template_name_match.group("type").strip()
    parameter_pack_name_match = re.fullmatch(
        r"(?P<type>.+?\.\.\.)\s*[A-Za-z_][A-Za-z0-9_]*",
        value,
    )
    if parameter_pack_name_match:
        return parameter_pack_name_match.group("type").strip()
    return value


def _encode_type(
    value: str,
    registry: dict[str, Any],
    template_parameters: frozenset[str] = frozenset(),
) -> str:
    value = re.sub(r"\s+", " ", value.strip())
    exact = registry["exact_types"].get(value)
    if exact:
        return str(exact)

    array_match = re.fullmatch(r"(?P<type>.+?)(?P<arrays>(?:\[[^\]]*\])+)", value)
    if array_match:
        element_type = array_match.group("type").strip()
        try:
            encoded = _encode_type(element_type, registry, template_parameters)
        except PrototypeIdError as original_error:
            # Public callers may provide a declaration with a parameter name
            # (``uint64_t mask[]``), while extracted canonical signatures omit
            # it (``const uint64_t[]``).  Only strip a final identifier after
            # the complete element type has failed to resolve.
            named_element_match = re.fullmatch(
                r"(?P<type>.+?)(?:\s+|(?<=[*&]))"
                r"[A-Za-z_][A-Za-z0-9_]*",
                element_type,
            )
            if named_element_match is None:
                raise
            candidate_type = named_element_match.group("type").strip()
            if candidate_type in {
                "const",
                "volatile",
                "restrict",
                "signed",
                "unsigned",
                "short",
                "long",
            }:
                raise original_error
            encoded = _encode_type(
                candidate_type,
                registry,
                template_parameters,
            )
        for bound in reversed(re.findall(r"\[([^\]]*)\]", array_match.group("arrays"))):
            # C++ adjusts an unbounded array function parameter to a pointer.
            # Preserve bounded dimensions inside it, e.g. T[][N] -> pointer to
            # array[N] of T.
            if not bound.strip():
                encoded = f"{OPERATORS['pointer']}{encoded}"
                continue
            bound_code = ARRAY_BOUNDS.get(bound.strip())
            if not bound_code:
                raise PrototypeIdError(f"unknown array bound {bound!r} in {value!r}")
            encoded = f"{OPERATORS['array']}{bound_code}{encoded}"
        return encoded

    value = _strip_parameter_name(value)
    exact = registry["exact_types"].get(value)
    if exact:
        return str(exact)

    is_parameter_pack = False
    parameter_pack_match = re.search(r"\.\.\.\s*$", value)
    if parameter_pack_match:
        is_parameter_pack = True
        value = value[: parameter_pack_match.start()].strip()

    suffixes: list[str] = []
    while True:
        suffix_match = re.search(r"(?P<suffix>&&|[&*])\s*$", value)
        if not suffix_match:
            break
        suffixes.append(suffix_match.group("suffix"))
        value = value[: suffix_match.start()].strip()

    prefixes: list[str] = []
    while True:
        prefix_match = TYPE_PREFIX_RE.match(value)
        if not prefix_match:
            break
        prefix = prefix_match.group("qualifier") or prefix_match.group("address_space")
        assert prefix is not None
        if prefix in ADDRESS_SPACES:
            prefixes.append(ADDRESS_SPACES[prefix])
        else:
            operator_name = {
                "const": "const",
                "volatile": "volatile",
                "restrict": "restrict",
            }[prefix]
            prefixes.append(OPERATORS[operator_name])
        value = value[prefix_match.end() :].strip()

    exact = registry["exact_types"].get(value)
    placeholder_match = re.fullmatch(r"<(?P<name>[A-Za-z_][A-Za-z0-9_]*)>", value)
    if exact:
        atom = str(exact)
    elif placeholder_match:
        atom = placeholder_match.group(0)
    elif value in template_parameters or TEMPLATE_VARIABLE_RE.fullmatch(value):
        atom = f"p{value}"
    else:
        vector_match = re.fullmatch(r"vector_(?P<element>.+)", value)
        if vector_match:
            element = vector_match.group("element")
            atom = f"{OPERATORS['vector']}{_encode_type(element, registry, template_parameters)}"
        else:
            generic_match = GENERIC_TYPE_RE.fullmatch(value)
            if generic_match:
                base = registry["aliases"].get(
                    generic_match.group("name"), generic_match.group("name")
                )
                template_code = registry.get("template_types", {}).get(base)
                arguments = _top_level_parts(generic_match.group("arguments"))
                if template_code and arguments:
                    argument_codes = tuple(
                        _encode_type(argument, registry, template_parameters)
                        for argument in arguments
                    )
                    atom = (
                        f"{template_code}g{len(argument_codes)}"
                        + "".join(
                            f"x{len(argument_code)}_{argument_code}"
                            for argument_code in argument_codes
                        )
                    )
                else:
                    raise PrototypeIdError(f"unregistered type {value!r}")
            else:
                canonical = registry["aliases"].get(value, value)
                if canonical in registry["types"]:
                    atom = str(registry["types"][canonical])
                else:
                    raise PrototypeIdError(f"unregistered type {value!r}")

    suffix_codes = {
        "&": OPERATORS["lvalue_reference"],
        "&&": OPERATORS["rvalue_reference"],
        "*": OPERATORS["pointer"],
    }
    encoded = "".join(prefixes) + atom
    for suffix in reversed(suffixes):
        encoded = f"{suffix_codes[suffix]}{encoded}"
    if is_parameter_pack:
        encoded = f"{OPERATORS['parameter_pack']}{encoded}"
    return encoded


def prototype_id(
    function: str,
    *,
    source: str | None = None,
    registry_path: Path = DEFAULT_REGISTRY_PATH,
    template_parameters: Sequence[str] = (),
) -> str:
    """Encode one canonical function prototype using the registered type table."""
    match = re.fullmatch(
        rf"(?P<symbol>{FUNCTION_SYMBOL_RE})\((?P<parameters>.*)\)",
        function.strip(),
    )
    if not match:
        raise PrototypeIdError(f"invalid canonical function signature {function!r}")
    if any(
        not isinstance(name, str)
        or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name)
        for name in template_parameters
    ):
        raise PrototypeIdError("template_parameters must contain C++ identifier names")
    template_parameter_names = frozenset(template_parameters)
    registry = load_type_registry(registry_path)
    try:
        parameters = _top_level_parts(match.group("parameters"))
        parameter_code = "_".join(
            _encode_type(parameter, registry, template_parameter_names)
            for parameter in parameters
        )
    except PrototypeIdError as error:
        location = f" in {source}" if source else ""
        remediation = ""
        if str(error).startswith("unregistered type "):
            remediation = (
                "; inspect a safe registry extension with "
                f"python3 tools/maintain_type_registry.py --prototype {function!r}"
            )
        raise PrototypeIdError(
            f"cannot encode {function!r}{location}: {error}{remediation}"
        ) from error
    if not parameter_code:
        parameter_code = NO_PARAMETERS
    return f"{match.group('symbol')}__{parameter_code}"


def identifier_field(
    function: str,
    *,
    template_parameters: Sequence[str] = (),
) -> str:
    """Return the OKF identifier field required by a canonical prototype."""
    template_parameter_names = frozenset(template_parameters)
    placeholders = DOCUMENTED_PLACEHOLDER_RE.findall(function)
    has_documented_placeholder = any(
        TEMPLATE_VARIABLE_RE.fullmatch(placeholder) is None
        and placeholder not in template_parameter_names
        for placeholder in placeholders
    )
    return "prototype_id" if has_documented_placeholder else "override_id"


def signature_identifier(
    function: str,
    *,
    source: str | None = None,
    registry_path: Path = DEFAULT_REGISTRY_PATH,
    template_parameters: Sequence[str] = (),
) -> tuple[str, str]:
    return (
        identifier_field(function, template_parameters=template_parameters),
        prototype_id(
            function,
            source=source,
            registry_path=registry_path,
            template_parameters=template_parameters,
        ),
    )


def validate_prototype_links(
    top_signatures: Sequence[dict[str, Any]],
    local_signatures: Sequence[dict[str, Any]],
    *,
    source: str,
) -> None:
    """Validate top-level prototype references against local declarations."""
    top_links: list[tuple[str, str, tuple[str, ...], tuple[str, ...]]] = []
    for index, entry in enumerate(top_signatures):
        if not isinstance(entry, dict):
            raise PrototypeIdError(f"{source}: signature[{index}] must be an object")
        field, value = _entry_identifier(entry, source, f"signature[{index}]")
        if field == "macro_id":
            _validate_macro_entry(entry, source, f"signature[{index}]")
            archs = ()
            header = ()
        else:
            archs = _entry_string_list(entry, "archs", source, f"signature[{index}]")
            header = _entry_string_list(entry, "header", source, f"signature[{index}]")
        _validate_identifier_kind(field, value, source, f"signature[{index}]")
        top_links.append((field, value, archs, header))
    duplicates = sorted({link for link in top_links if top_links.count(link) > 1})
    if duplicates:
        raise PrototypeIdError(
            f"{source}: duplicate top-level signature entries: "
            + ", ".join(value for _, value, _, _ in duplicates)
        )

    local_links: list[tuple[str, str, tuple[str, ...], tuple[str, ...]]] = []
    for index, entry in enumerate(local_signatures):
        if not isinstance(entry, dict):
            raise PrototypeIdError(f"{source}: api_signature[{index}] must be an object")
        label = f"api_signature[{index}]"
        field, local_id = _entry_identifier(entry, source, label)
        if field == "macro_id":
            _validate_macro_entry(entry, source, label)
            archs = ()
            header = ()
        else:
            archs = _entry_string_list(entry, "archs", source, label)
            header = _entry_string_list(entry, "header", source, label)
        _validate_identifier_kind(field, local_id, source, label)
        if field != "macro_id":
            placeholders = tuple(dict.fromkeys(PLACEHOLDER_RE.findall(local_id)))
            _validate_dtype(entry.get("dtype"), placeholders, source, index)
        local_links.append((field, local_id, archs, header))

    missing_local = sorted(set(top_links) - set(local_links))
    missing_top = sorted(set(local_links) - set(top_links))
    if missing_local or missing_top:
        details = []
        if missing_local:
            details.append(
                "missing local definitions: "
                + ", ".join(value for _, value, _, _ in missing_local)
            )
        if missing_top:
            details.append(
                "missing top-level references: "
                + ", ".join(value for _, value, _, _ in missing_top)
            )
        raise PrototypeIdError(f"{source}: " + "; ".join(details))


def _entry_identifier(
    entry: dict[str, Any], source: str, label: str
) -> tuple[str, str]:
    fields = [
        field
        for field in ("prototype_id", "override_id", "macro_id")
        if field in entry
    ]
    if len(fields) != 1:
        raise PrototypeIdError(
            f"{source}: {label} must contain exactly one of prototype_id, override_id, or macro_id"
        )
    field = fields[0]
    value = entry[field]
    if not isinstance(value, str) or not value:
        raise PrototypeIdError(f"{source}: {label}.{field} must be a non-empty string")
    return field, value


def _entry_string_list(
    entry: dict[str, Any], field: str, source: str, label: str
) -> tuple[str, ...]:
    value = entry.get(field)
    if not isinstance(value, list) or any(
        not isinstance(item, str) or not item for item in value
    ):
        raise PrototypeIdError(f"{source}: {label}.{field} must be a string list")
    if len(value) != len(set(value)):
        raise PrototypeIdError(f"{source}: {label}.{field} contains duplicates")
    return tuple(sorted(value))


def _validate_identifier_kind(
    field: str, value: str, source: str, label: str
) -> None:
    if field == "macro_id":
        if MACRO_ID_RE.fullmatch(value) is None:
            raise PrototypeIdError(
                f"{source}: {label}.macro_id has invalid format: {value!r}"
            )
        return
    expected = "prototype_id" if PLACEHOLDER_RE.search(value) else "override_id"
    if field != expected:
        raise PrototypeIdError(
            f"{source}: {label} uses {field} for {value!r}; expected {expected}"
        )


def _validate_macro_entry(entry: dict[str, Any], source: str, label: str) -> None:
    if set(entry) != {"macro_id"}:
        raise PrototypeIdError(
            f"{source}: {label} with macro_id must not contain other fields"
        )


def _validate_dtype(
    dtype: Any,
    placeholders: Sequence[str],
    source: str,
    index: int,
) -> None:
    if dtype is None:
        if placeholders:
            raise PrototypeIdError(
                f"{source}: api_signature[{index}] is missing dtype for "
                + ", ".join(f"<{name}>" for name in placeholders)
            )
        return
    if not placeholders:
        raise PrototypeIdError(
            f"{source}: api_signature[{index}] has dtype without placeholders"
        )
    if not isinstance(dtype, dict) or set(dtype) not in ({"values"}, {"variants"}):
        raise PrototypeIdError(
            f"{source}: api_signature[{index}].dtype must contain values or variants"
        )
    if "values" in dtype:
        if placeholders != ("dtype",) or not isinstance(dtype["values"], list):
            raise PrototypeIdError(
                f"{source}: api_signature[{index}].dtype.values requires only <dtype>"
            )
        return
    variants = dtype["variants"]
    if not isinstance(variants, list) or any(
        not isinstance(variant, dict) or tuple(variant) != placeholders
        for variant in variants
    ):
        raise PrototypeIdError(
            f"{source}: api_signature[{index}].dtype.variants must bind "
            + ", ".join(placeholders)
        )
