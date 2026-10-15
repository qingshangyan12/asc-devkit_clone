#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


"""Write a Markdown preview of source-derived API YAML metadata."""

import argparse
from functools import lru_cache
import os
import json
import re
import sys
from dataclasses import dataclass, replace
from pathlib import Path
from typing import Any, Mapping, Optional, Sequence
from urllib.parse import unquote, urlsplit

from extract_api_descriptions import (
    _collect_api_files,
    DescriptionAnalysis,
    analyze_description,
    classify_api_document,
    is_markdown_link_list,
)
from prototype_id import PLACEHOLDER_RE, signature_identifier


def _looks_like_asc_devkit(path: Path) -> bool:
    return (
        (path / "docs" / "zh" / "api" / "SIMD-API").is_dir()
        and (path / "include").is_dir()
    )


def _default_repo_root() -> Path:
    env_root = os.environ.get("API_METADATA_REPO_ROOT")
    if env_root:
        return Path(env_root).expanduser().resolve()

    here = Path(__file__).resolve()
    for parent in (Path.cwd().resolve(), *here.parents):
        for candidate in (parent, parent / "asc-devkit"):
            if _looks_like_asc_devkit(candidate):
                return candidate.resolve()
    return here.parents[1]


REPO_ROOT = _default_repo_root()
DEFAULT_API_DIRECTORY = (
    REPO_ROOT
    / "docs"
    / "zh"
    / "api"
    / "SIMD-API"
    / "c_api"
    / "reg"
    / "arithmetic_compute"
)
DEFAULT_OUTPUT_PATH = Path.home() / "Desktop" / "api_yaml_metadata_preview.md"
TITLE_RE = re.compile(r"^\s{0,3}#\s+(.+?)\s*$")
HTML_TAG_RE = re.compile(r"<[^>]*>")
FUNCTION_SECTION_RE = re.compile(r"^\s{0,3}##\s+函数原型(?:\s|[<（(]|$)")
PRODUCT_SUPPORT_SECTION_RE = re.compile(r"^\s{0,3}##\s+产品支持情况(?:\s|<|$)")
EXAMPLE_SECTION_RE = re.compile(r"^\s{0,3}##\s+调用示例(?:\s|<|$)")
CONSTANT_DEFINITION_SECTION_RE = re.compile(r"^\s{0,3}#{2,6}\s+常量定义(?:\s|<|$)")
ENUM_DEFINITION_SECTION_RE = re.compile(r"^\s{0,3}#{2,6}\s+枚举(?:类)?具体定义(?:\s|<|$)")
ENUM_VALUES_SECTION_RE = re.compile(r"^\s{0,3}#{2,6}\s+枚举值详解(?:\s|<|$)")
TYPE_ALIAS_DEFINITION_SECTION_RE = re.compile(r"^\s{0,3}#{2,6}\s+类型别名具体定义(?:\s|<|$)")
RECORD_DEFINITION_SECTION_RE = re.compile(
    r"^\s{0,3}#{2,6}\s+(?:结构体具体定义|联合体具体定义)(?:\s|<|$)"
)
RECORD_FIELDS_SECTION_RE = re.compile(r"^\s{0,3}#{2,6}\s+字段详解(?:\s|<|$)")
SECTION_RE = re.compile(r"^\s{0,3}##\s+")
API_SYMBOL_RE = re.compile(r"(?<![\w:])(~?[A-Za-z_][A-Za-z0-9_]*)\s*\(")
PROTOTYPE_OPERATOR_RE = re.compile(r"\b(operator\s*\[\])\s*\(")
PROTOTYPE_QUALIFIED_SYMBOL_RE = re.compile(
    r"(?:[A-Za-z_][A-Za-z0-9_]*(?:\s*<[^>\n]+>)?::)+"
    r"(~?[A-Za-z_][A-Za-z0-9_]*)\s*\("
)
PROTOTYPE_TEMPLATE_CALL_RE = re.compile(
    r"\b(~?[A-Za-z_][A-Za-z0-9_]*)\s*<[^>\n]+>\s*\("
)
PROTOTYPE_USING_ALIAS_RE = re.compile(r"\busing\s+([A-Za-z_][A-Za-z0-9_]*)\s*=")
PROTOTYPE_REGISTER_API_RE = re.compile(
    r"\bREGISTER_[A-Za-z0-9_]*API\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\b"
)
PROTOTYPE_BARE_MACRO_RE = re.compile(r"^\s*([A-Z][A-Z0-9_]+)\s*$", re.MULTILINE)
PROTOTYPE_TYPE_DECLARATION_RE = re.compile(
    r"\b(?:class|struct|enum(?:\s+class)?)\s+([A-Za-z_][A-Za-z0-9_]*)\b"
)
SUBSECTION_RE = re.compile(r"^\s{0,3}#{3,6}\s+")
FENCE_RE = re.compile(r"^\s*```")
SUPPORTED_NPU_RE = re.compile(
    r'<!--\s*npu="([^"]+)"[^>]*-->\s*\n-\s*[^：:]+[：:]\s*支持',
    re.MULTILINE,
)
NPU_LABEL_TO_ARCH = {
    "950": "3510",
    "A3": "2201",
    "910b": "2201",
    "310b": "3002",
    "310p": "2002",
    "910": "1001",
}
EXAMPLE_SOURCE_SUFFIXES = frozenset(
    {".asc", ".c", ".cc", ".cpp", ".cxx", ".h", ".hpp"}
)
EXAMPLE_CONTROL_KEYWORDS = frozenset(
    {"if", "for", "while", "switch", "catch", "constexpr", "do", "else"}
)
EXAMPLE_SNIPPET_MARKER_RE = re.compile(
    r"^[ \t]*//![ \t]*\[(?P<id>[A-Za-z0-9_.:-]+(?:[ \t]*,[ \t]*[A-Za-z0-9_.:-]+)*)\][ \t]*$"
)
EXAMPLE_SNIPPET_MARKER_LIKE_RE = re.compile(r"^\s*//[!！]\s*\[")
COMPILER_GENERATED_MACRO_SOURCES = (
    "tools/build/asc_op_compile_base/adapter/generate_tiling_code.py",
    "tools/build/asc_op_compile_base/asc_op_compiler/generate_tiling_code.py",
)
# Basic API includes CPU-debug declarations such as ``inline uint8_t*
# GetBaseAddr(...)`` without an execution qualifier.  ``inline`` remains a
# declaration-only cue in a function-prototype code block, so it is safe to
# accept alongside the accelerator qualifiers.
PROTOTYPE_START_RE = re.compile(r"\b(__aicore__|__simd_callee__|inline)\b.*\(")
EXAMPLE_MARK_RE = re.compile(r"(典型示例|调用示例|示例[:：])")
DTYPE_LIST_RE = re.compile(r"\*\*dtype取值如下：\*\*([^\n]+)")
BACKTICK_RE = re.compile(r"`([^`]+)`")
TYPE_TOKEN_RE = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
CONDITIONAL_PARAMETER_TYPE_RE = re.compile(
    r"当\s*`?(?P<placeholder>[A-Za-z_][A-Za-z0-9_]*)`?\s*为\s*"
    r"`(?P<value>[A-Za-z_][A-Za-z0-9_]*)`\s*时[，,]\s*"
    r"(?P<parameters>.+?)的实际类型(?P<all>均)?为\s*"
    r"`(?P<actual_type>[A-Za-z_][A-Za-z0-9_]*)`"
)
CONDITIONAL_TEMPLATE_TYPE_RE = re.compile(
    r"对应(?:矢量(?:数据)?)?寄存器类型(?:为)?\s*"
    r"`(?P<template>[^`]*<(?P<placeholder>[A-Za-z_][A-Za-z0-9_]*)>[^`]*)`"
    r"\s*[，,]\s*(?:其中\s*)?`(?P<value>[A-Za-z_][A-Za-z0-9_]*)`\s*对应\s*"
    r"`(?P<actual_type>[A-Za-z_][A-Za-z0-9_]*)`"
)
CONDITIONAL_REGISTER_DTYPE_TYPE_RE = re.compile(
    r"当\s*`?(?P<placeholder>[A-Za-z_][A-Za-z0-9_]*)`?\s*为\s*"
    r"`(?P<value>[A-Za-z_][A-Za-z0-9_]*)`?\s*时[，,]\s*"
    r"目的矢量(?:数据)?寄存器的实际类型为\s*"
    r"`(?P<actual_type>vector_[A-Za-z_][A-Za-z0-9_]*)`"
)
DTYPE_SUPPORT_SENTENCE_RE = re.compile(
    r"`?(?P<placeholder>[A-Za-z_][A-Za-z0-9_]*)`?"
    r"\s*支持的数据类型为(?P<values>[^。；;\n]+)",
    re.IGNORECASE,
)
PLACEHOLDER_SUPPORT_HEADING_RE = re.compile(
    r"^\s{0,3}#{1,6}\s*`?(?P<placeholder>[A-Za-z_][A-Za-z0-9_]*)`?"
    r"\s*支持(?:的)?数据类型\s*$",
    re.IGNORECASE,
)
PROTOTYPE_DTYPE_SCOPE_RE = re.compile(
    r"通过(?P<kind>函数返回值返回结果|引用参数输出结果)的函数原型中[，,]?\s*"
    r"`?(?P<placeholder>[A-Za-z_][A-Za-z0-9_]*)`?\s*取值为[：:]?\s*"
    r"(?P<values>[^。；;\n]+)"
)
RETURN_STYLE_DTYPE_SCOPE_RE = re.compile(
    r"(?P<kind>返回值类型|无返回值类型)接口支持的数据类型为\s*"
    r"(?P<values>[^。；;\n]+)"
)
CORRELATED_PLACEHOLDER_TUPLES_RE = re.compile(
    r"`(?P<first>[A-Za-z_][A-Za-z0-9_]*)`\s*[、,，]\s*"
    r"`(?P<second>[A-Za-z_][A-Za-z0-9_]*)`\s*[、,，]\s*"
    r"`(?P<third>[A-Za-z_][A-Za-z0-9_]*)`\s*支持以下组合[：:]?\s*"
    r"(?P<values>[^。；;\n]+)"
)
EXAMPLE_INTEGRAL_CONSTANT_RE = re.compile(
    r"\b(?:(?:inline|static)\s+)*constexpr\s+"
    r"(?P<type>std::integral_constant\s*<[^;{}]*?>)\s+"
    r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?:=|;)"
)
EXAMPLE_SCOPED_ENUM_RE = re.compile(
    r"\benum\s+class\s+(?P<type>[A-Za-z_][A-Za-z0-9_]*)"
    r"(?:\s*:\s*[^\{]+)?\s*\{(?P<body>[^{}]*)\}",
    re.DOTALL,
)
EXAMPLE_ENUMERATOR_RE = re.compile(
    r"^\s*(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?:=[^,{}]+)?\s*$"
)
EXAMPLE_UINT32_LITERAL_RE = re.compile(
    r"(?:0[xX][0-9A-Fa-f]+|\d+)[uU]"
)
LEVEL_THREE_SECTION_RE = re.compile(r"^\s{0,3}###(?!#)\s+", re.MULTILINE)
NPU_SCOPED_SECTION_RE = re.compile(
    r'<!--\s*npu="(?P<npu_labels>[^"]+)"[^>]*\bid(?P<scope_id>[A-Za-z0-9_-]+)\b[^>]*-->'
    r"(?P<body>.*?)"
    r"<!--\s*end\s+id(?P=scope_id)\s*-->",
    re.DOTALL,
)
DEPENDENT_PLACEHOLDER_VARIANTS_RE = re.compile(
    r"`(?P<condition_name>[A-Za-z_][A-Za-z0-9_]*)`\s*为\s*"
    r"`(?P<condition_value>[A-Za-z_][A-Za-z0-9_]*)`\s*时[，,]\s*"
    r"`(?P<dependent_name>[A-Za-z_][A-Za-z0-9_]*)`\s*支持"
    r"(?P<dependent_values>[^。\n]+)"
)
EXPLICIT_PLACEHOLDER_RE = re.compile(r"<(?P<name>[A-Za-z_][A-Za-z0-9_]*)>")
EXPLICIT_PLACEHOLDER_VALUE_RE = re.compile(
    r"取值为\s*`(?P<value>_?[a-z][a-z0-9_]*)`"
)

# =============================================================================
# 头文件查找：匹配规则与缓存配置
# =============================================================================
# 这些正则负责从 Markdown 和头文件中识别路径及 include；状态集合定义参数比较结果中
# 哪些可以作为有效映射，缓存配置用于限制批量扫描大量头文件时的重复读取和比较成本。
DOCUMENT_HEADER_LINE_RE = re.compile(r"头文件路径(?:为)?\s*[：:]\s*(.+)")
DOCUMENT_HEADER_PATH_RE = re.compile(r"([A-Za-z0-9_./-]+\.h)")
DOCUMENT_INCLUDE_RE = re.compile(r'#\s*include\s+"([^"]+\.h)"')
HEADER_INCLUDE_RE = re.compile(r'#\s*include\s*[<"]([^>"]+\.h)[>"]')
IGNORED_PROTOTYPE_SYMBOLS = frozenset(
    {
        "if",
        "for",
        "while",
        "switch",
        "return",
        "sizeof",
        "alignas",
        "decltype",
        "static_cast",
        "reinterpret_cast",
        "const_cast",
        "dynamic_cast",
    }
)
API_HEADER_RESOURCE_PREFIXES = (
    "/include/adv_api/",
    "/include/basic_api/",
    "/include/c_api/",
    "/include/tensor_api/",
    "/tools/",
)
IGNORED_MACRO_TARGET_SYMBOLS = IGNORED_PROTOTYPE_SYMBOLS | frozenset(
    {
        "defined",
        "do",
        "else",
        "constexpr",
        "static_assert",
    }
)
HEADER_MAPPING_PARAMETER_STATUSES = frozenset(
    {
        "exact",
        "array_parameter_match",
        "macro_parameter_names_match",
        "object_macro_alias_match",
        "parameter_name_difference",
    }
)
SUSPICIOUS_PARAMETER_STATUSES = frozenset(
    {
        "parameter_type_mismatch",
        "parameter_count_mismatch",
    }
)
HEADER_TEXT_CACHE_SIZE = 4096
SYMBOL_DECLARATION_CACHE_SIZE = 131072
SIGNATURE_STATUS_CACHE_SIZE = 131072
SIGNATURE_COMPARISON_CACHE_SIZE = 32768
SUSPICIOUS_SAME_NAME_CANDIDATE_LIMIT = 8
TYPE_KEYWORDS = frozenset(
    {
        "auto",
        "bool",
        "char",
        "double",
        "float",
        "int",
        "long",
        "short",
        "signed",
        "unsigned",
        "void",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "size_t",
    }
)


@dataclass(frozen=True)
class ApiMetadata:
    filename: str
    title: str
    description: Optional[str]
    resource: Optional[str]
    version: str
    header_candidates: Sequence[str] = ()
    document_path: Optional[Path] = None
    description_analysis: Optional[DescriptionAnalysis] = None
    legacy_include_mappings: Sequence["LegacyIncludeMapping"] = ()
    signature: Sequence["SignatureHeaderMapping"] = ()
    signature_comparisons: Sequence["SignatureComparison"] = ()
    archs: Sequence[str] = ()
    examples: Sequence["ExampleReference"] = ()
    api_kind: str = "function"
    declaration: Optional[Mapping[str, Any]] = None
    type_catalog: Optional[Mapping[str, Any]] = None


@dataclass(frozen=True)
class ExampleCall:
    file: str
    line: int
    symbol: str
    snippet: Optional[str]
    overload_status: str
    snippet_kind: str = "function"
    snippet_start_line: Optional[int] = None
    snippet_end_line: Optional[int] = None
    canonical_id: Optional[str] = None
    candidates: Sequence[str] = ()
    inferred_types: Optional[Sequence[Optional[str]]] = None
    call_template: Optional[str] = None
    placeholder_bindings: Sequence[dict[str, str]] = ()
    placeholder_status: str = "none"
    semantic_snippet: Optional[str] = None
    line_range: Sequence[int] = ()
    condition: Sequence[str] = ()
    role: Sequence[str] = ()
    is_statement: bool = False
    call_start_offset: Optional[int] = None
    call_end_offset: Optional[int] = None


@dataclass(frozen=True)
class ExampleIssue:
    type: str
    message: str
    file: Optional[str] = None
    line: Optional[int] = None
    symbol: Optional[str] = None
    cause: Optional[str] = None


@dataclass(frozen=True)
class ExampleSnippetMarker:
    id: str
    file: str
    start_offset: int
    end_offset: int
    start_line: int
    end_line: int


@dataclass(frozen=True)
class ExampleReference:
    path: Optional[str]
    source_target: str
    archs: Sequence[str] = ()
    calls: Sequence[ExampleCall] = ()
    issues: Sequence[ExampleIssue] = ()
    discovery: str = "document_link"
    confidence: str = "authoritative"
    candidate_paths: Sequence[str] = ()
    category: str = "未分类"
    candidate_categories: Sequence[str] = ()


@dataclass(frozen=True)
class ExampleBacklink:
    project_path: Path
    readme_path: Path
    source_target: str


@dataclass(frozen=True)
class ReviewedExampleMapping:
    document_path: Path
    project_path: Path
    reason: str


@dataclass(frozen=True)
class ExampleDiscoveryIndex:
    backlinks: dict[Path, tuple[ExampleBacklink, ...]]
    source_projects: dict[str, tuple[Path, ...]]
    reviewed_mappings: dict[Path, tuple[ReviewedExampleMapping, ...]]


# =============================================================================
# 头文件查找：声明、比较结果与映射结果数据结构
# =============================================================================
# PrototypeDeclaration 表示从 Markdown 提取并展开后的声明；其余结构依次承载历史路径
# 映射、参数比较详情、最终头文件映射、头文件参数候选和宏定义。
@dataclass(frozen=True)
class PrototypeDeclaration:
    symbol: str
    qualifier: Optional[str]
    parameters: Optional[str] = None
    archs: Sequence[str] = ()
    prototype_function: str = ""
    prototype_document_signature: str = ""
    placeholder_bindings: Sequence[tuple[str, str]] = ()
    function_name_expansion_source: str = ""
    template_parameters: Sequence[str] = ()


@dataclass(frozen=True)
class LegacyIncludeMapping:
    source_kind: str
    source_document_path: Optional[Path]
    source_path: str
    resource: str


@dataclass(frozen=True)
class SignatureComparison:
    resource: str
    symbol: str
    status: str
    document_parameters: str
    header_parameters: Sequence[str] = ()
    header_parameter_source: Optional[str] = None
    header_symbol: Optional[str] = None
    macro_chain: Sequence[str] = ()


@dataclass(frozen=True)
class SignatureHeaderMapping:
    name: str
    headers: Sequence[str]
    status: str
    source: str
    has_header_hint: bool
    include_relations: Sequence[tuple[str, str]] = ()
    include_relation_kind: str = ""
    suspicious_candidates: Sequence["SignatureComparison"] = ()
    document_parameters: str = ""
    canonical_id: str = ""
    override_id: str = ""
    macro_id: str = ""
    scope_headers: Sequence[str] = ()
    archs: Sequence[str] = ()
    document_signature: str = ""
    prototype_function: str = ""
    prototype_document_signature: str = ""
    placeholder_bindings: Sequence[tuple[str, str]] = ()
    template_parameters: Sequence[str] = ()


@dataclass(frozen=True)
class HeaderParameterCandidate:
    parameters: str
    source_kind: str
    source_symbol: str
    macro_chain: Sequence[str] = ()


@dataclass(frozen=True)
class MacroDefinition:
    symbol: str
    parameters: Optional[str]
    body: str


def extract_title(markdown_text: str) -> Optional[str]:
    for line in markdown_text.splitlines():
        match = TITLE_RE.match(line)
        if match:
            return HTML_TAG_RE.sub("", match.group(1)).strip()
    return None


def _section_text(markdown_text: str, section_re: re.Pattern[str]) -> Optional[str]:
    lines = markdown_text.splitlines()
    section_start = None
    for index, line in enumerate(lines):
        if section_re.match(line):
            section_start = index + 1
            break
    if section_start is None:
        return None

    section_lines = []
    for line in lines[section_start:]:
        if SECTION_RE.match(line):
            break
        section_lines.append(line)
    return "\n".join(section_lines)


def _prototype_section_text(markdown_text: str) -> Optional[str]:
    return _section_text(markdown_text, FUNCTION_SECTION_RE)


def supported_archs(markdown_text: str) -> Sequence[str]:
    section_text = _section_text(markdown_text, PRODUCT_SUPPORT_SECTION_RE)
    if section_text is None:
        return ()
    archs = [
        arch
        for label in SUPPORTED_NPU_RE.findall(section_text)
        for arch in _npu_archs(label)
    ]
    return tuple(dict.fromkeys(archs))


def _fenced_code_blocks(section_text: str | None) -> tuple[str, ...]:
    if not section_text:
        return ()
    blocks = []
    in_block = False
    lines = []
    for line in section_text.splitlines():
        if FENCE_RE.match(line):
            if in_block:
                text = "\n".join(lines).strip()
                if text:
                    blocks.append(text)
                lines = []
                in_block = False
            else:
                in_block = True
            continue
        if in_block:
            lines.append(line.rstrip())
    return tuple(blocks)


def _markdown_tables(section_text: str | None) -> tuple[tuple[tuple[str, ...], tuple[tuple[str, ...], ...]], ...]:
    if not section_text:
        return ()
    tables = []
    lines = section_text.splitlines()
    for index in range(len(lines) - 1):
        headers = _markdown_table_cells(lines[index])
        separators = _markdown_table_cells(lines[index + 1])
        if (
            not headers
            or len(headers) != len(separators)
            or not _is_markdown_table_separator(separators)
        ):
            continue
        rows = []
        for line in lines[index + 2 :]:
            row = _markdown_table_cells(line)
            if not row:
                break
            if len(row) >= len(headers):
                rows.append(row[: len(headers)])
        tables.append((headers, tuple(rows)))
    return tuple(tables)


def _named_definition_descriptions(section_text: str | None) -> dict[str, str]:
    descriptions = {}
    for headers, rows in _markdown_tables(section_text):
        normalized = tuple(header.replace(" ", "") for header in headers)
        name_index = next(
            (
                index
                for index, header in enumerate(normalized)
                if header in {"枚举值", "字段名", "字段"}
            ),
            None,
        )
        if name_index is None or len(headers) < 2:
            continue
        description_index = len(headers) - 1
        for row in rows:
            name = row[name_index].strip()
            description = row[description_index].strip()
            if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name) and description:
                descriptions.setdefault(name, description)
    return descriptions


def _balanced_brace_end(text: str, opening: int) -> int | None:
    depth = 0
    for index in range(opening, len(text)):
        character = text[index]
        if character == "{":
            depth += 1
        elif character == "}":
            depth -= 1
            if depth == 0:
                return index
    return None


def _constant_declaration(markdown_text: str, title: str) -> dict[str, Any] | None:
    blocks = _fenced_code_blocks(
        _section_text(markdown_text, CONSTANT_DEFINITION_SECTION_RE)
    )
    constexpr_re = re.compile(
        r"(?m)^\s*(?P<cpp>(?:(?:inline|static|extern)\s+)*(?:constexpr|const)\s+"
        r"(?P<type>.+?)\s+(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*"
        r"(?P<value>[^;]+);)"
    )
    macro_re = re.compile(
        r"(?m)^\s*(?P<cpp>#\s*define\s+(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)"
        r"(?:\s+(?P<value>.*))?)$"
    )
    for block in blocks:
        candidates = [*constexpr_re.finditer(block), *macro_re.finditer(block)]
        candidates.sort(key=lambda match: match.start())
        selected = next(
            (match for match in candidates if match.group("symbol") == title),
            candidates[0] if candidates else None,
        )
        if selected is None:
            continue
        declaration = {
            "symbol": selected.group("symbol"),
            "cpp": selected.group("cpp").strip(),
        }
        value = selected.groupdict().get("value")
        if value:
            declaration["value"] = value.strip()
        value_type = selected.groupdict().get("type")
        if value_type:
            declaration["type"] = re.sub(r"\s+", " ", value_type).strip()
        return declaration
    return None


def _enum_declaration(markdown_text: str, title: str) -> dict[str, Any] | None:
    blocks = _fenced_code_blocks(
        _section_text(markdown_text, ENUM_DEFINITION_SECTION_RE)
    )
    enum_re = re.compile(
        r"\benum(?:\s+(?:class|struct))?\s+"
        r"(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)"
        r"(?:\s*:\s*(?P<underlying>[^\{]+))?\s*\{"
    )
    descriptions = _named_definition_descriptions(
        _section_text(markdown_text, ENUM_VALUES_SECTION_RE)
    )
    for block in blocks:
        candidates = tuple(enum_re.finditer(block))
        match = next(
            (candidate for candidate in candidates if candidate.group("symbol") == title),
            candidates[0] if candidates else None,
        )
        if match is None:
            continue
        opening = block.find("{", match.start())
        closing = _balanced_brace_end(block, opening)
        if closing is None:
            continue
        semicolon = block.find(";", closing)
        cpp = block[match.start() : semicolon + 1 if semicolon >= 0 else closing + 1].strip()
        members = []
        member_body = re.sub(r"//.*?$|/\*.*?\*/", "", block[opening + 1 : closing], flags=re.MULTILINE | re.DOTALL)
        for part in _top_level_parameter_parts(member_body):
            member_match = re.fullmatch(
                r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)(?:\s*=\s*(?P<value>.+))?",
                part.strip(),
                flags=re.DOTALL,
            )
            if member_match is None:
                continue
            member = {"name": member_match.group("name")}
            value = member_match.group("value")
            if value:
                member["value"] = re.sub(r"\s+", " ", value).strip()
            description = descriptions.get(member["name"])
            if description:
                member["description"] = description
            members.append(member)
        declaration = {"symbol": match.group("symbol"), "cpp": cpp, "members": members}
        underlying = match.group("underlying")
        if underlying:
            declaration["underlying_type"] = re.sub(r"\s+", " ", underlying).strip()
        return declaration
    return None


def _type_alias_declaration(markdown_text: str, title: str) -> dict[str, Any] | None:
    blocks = _fenced_code_blocks(
        _section_text(markdown_text, TYPE_ALIAS_DEFINITION_SECTION_RE)
    )
    alias_re = re.compile(
        r"(?m)^\s*(?P<cpp>using\s+(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*"
        r"(?P<target>[^;]+);)"
    )
    for block in blocks:
        candidates = tuple(alias_re.finditer(block))
        match = next(
            (candidate for candidate in candidates if candidate.group("symbol") == title),
            candidates[0] if candidates else None,
        )
        if match is not None:
            return {
                "symbol": match.group("symbol"),
                "cpp": match.group("cpp").strip(),
                "target": re.sub(r"\s+", " ", match.group("target")).strip(),
            }
    return None


def _record_field_type(cpp: str, name: str) -> tuple[str | None, str | None]:
    match = re.search(
        rf"(?m)(?P<type>[A-Za-z_][A-Za-z0-9_:]*(?:\s*<[^;{{}}()]+>)?)"
        rf"\s+{re.escape(name)}\s*(?::\s*(?P<bits>\d+))?",
        cpp,
    )
    if match is None:
        return None, None
    return match.group("type").strip(), match.group("bits")


def _record_declaration(markdown_text: str, title: str) -> dict[str, Any] | None:
    blocks = _fenced_code_blocks(
        _section_text(markdown_text, RECORD_DEFINITION_SECTION_RE)
    )
    record_re = re.compile(
        r"\b(?P<record_kind>struct|union)\s+"
        r"(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)\s*\{"
    )
    descriptions = _named_definition_descriptions(
        _section_text(markdown_text, RECORD_FIELDS_SECTION_RE)
    )
    for block in blocks:
        candidates = tuple(record_re.finditer(block))
        match = next(
            (candidate for candidate in candidates if candidate.group("symbol") == title),
            candidates[0] if candidates else None,
        )
        if match is None:
            continue
        opening = block.find("{", match.start())
        closing = _balanced_brace_end(block, opening)
        if closing is None:
            continue
        semicolon = block.find(";", closing)
        cpp = block[match.start() : semicolon + 1 if semicolon >= 0 else closing + 1].strip()
        fields = []
        for name, description in descriptions.items():
            field = {"name": name, "description": description}
            value_type, bits = _record_field_type(cpp, name)
            if value_type:
                field["type"] = value_type
            if bits:
                field["bits"] = bits
            fields.append(field)
        return {
            "symbol": match.group("symbol"),
            "cpp": cpp,
            "fields": fields,
            "record_kind": match.group("record_kind"),
        }
    return None


def _type_catalog(markdown_text: str) -> dict[str, Any] | None:
    groups = []
    for headers, rows in _markdown_tables(markdown_text):
        normalized = tuple(header.replace(" ", "") for header in headers)
        type_indexes = [
            index
            for index, header in enumerate(normalized)
            if "类型" in header and "类别" not in header
        ]
        category_index = next(
            (index for index, header in enumerate(normalized) if "类别" in header),
            None,
        )
        if len(type_indexes) != 1 or category_index is None:
            continue
        for row in rows:
            category = row[category_index].strip()
            symbols = tuple(
                symbol
                for symbol in re.split(r"[、,，\s]+", row[type_indexes[0]].strip())
                if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol)
            )
            if category and symbols:
                groups.append({"name": category, "types": list(symbols)})
    return {"groups": groups} if groups else None


def _typed_declaration(
    markdown_text: str, api_kind: str, title: str
) -> dict[str, Any] | None:
    if api_kind == "constant":
        return _constant_declaration(markdown_text, title)
    if api_kind == "enum":
        return _enum_declaration(markdown_text, title)
    if api_kind == "type_alias":
        return _type_alias_declaration(markdown_text, title)
    if api_kind == "record":
        return _record_declaration(markdown_text, title)
    return None


def _npu_archs(labels: str) -> tuple[str, ...]:
    return tuple(
        dict.fromkeys(
            NPU_LABEL_TO_ARCH.get(label.strip(), label.strip())
            for label in labels.split(",")
            if label.strip()
        )
    )


def _markdown_link_targets(markdown_text: str) -> tuple[str, ...]:
    targets = []
    index = 0
    while index < len(markdown_text):
        link_start = markdown_text.find("](", index)
        if link_start < 0:
            break
        cursor = link_start + 2
        depth = 1
        escaped = False
        while cursor < len(markdown_text):
            character = markdown_text[cursor]
            if escaped:
                escaped = False
            elif character == "\\":
                escaped = True
            elif character == "(":
                depth += 1
            elif character == ")":
                depth -= 1
                if depth == 0:
                    raw_target = markdown_text[link_start + 2 : cursor].strip()
                    if raw_target.startswith("<") and raw_target.endswith(">"):
                        raw_target = raw_target[1:-1].strip()
                    else:
                        raw_target = re.split(r"\s+[\"']", raw_target, maxsplit=1)[0]
                    if raw_target:
                        targets.append(raw_target)
                    cursor += 1
                    break
            cursor += 1
        index = max(cursor, link_start + 2)
    return tuple(targets)


def _nearest_example_project(readme_path: Path, examples_root: Path) -> Optional[Path]:
    for directory in (readme_path.parent, *readme_path.parents):
        if directory == examples_root.parent:
            break
        if (directory / "CMakeLists.txt").is_file():
            return directory
        if directory == examples_root:
            break
    return None


def build_example_discovery_index(
    repo_root: Path = REPO_ROOT,
    reviewed_mappings: Sequence[ReviewedExampleMapping] = (),
) -> ExampleDiscoveryIndex:
    """Build repository-wide indexes once, then reuse them for every API document."""
    repo_root = repo_root.resolve()
    examples_root = repo_root / "examples"
    backlinks: dict[Path, list[ExampleBacklink]] = {}
    backlink_keys: set[tuple[Path, Path]] = set()

    if examples_root.is_dir():
        readme_paths = sorted(
            path
            for path in examples_root.rglob("README*.md")
            if path.is_file()
        )
        for readme_path in readme_paths:
            project_path = _nearest_example_project(readme_path, examples_root)
            if project_path is None:
                continue
            markdown_text = readme_path.read_text(encoding="utf-8", errors="replace")
            for source_target in _markdown_link_targets(markdown_text):
                parsed = urlsplit(source_target)
                if parsed.scheme or parsed.netloc or not parsed.path:
                    continue
                target_path = (readme_path.parent / unquote(parsed.path)).resolve()
                try:
                    target_path.relative_to(repo_root)
                except ValueError:
                    continue
                if target_path.suffix.lower() != ".md":
                    continue
                key = (target_path, project_path)
                if key in backlink_keys:
                    continue
                backlink_keys.add(key)
                backlinks.setdefault(target_path, []).append(
                    ExampleBacklink(project_path, readme_path, source_target)
                )

    source_projects: dict[str, set[Path]] = {}
    c_api_examples = examples_root / "02_simd_c_api"
    if c_api_examples.is_dir():
        project_paths = sorted(
            cmake_path.parent for cmake_path in c_api_examples.rglob("CMakeLists.txt")
        )
        symbol_re = re.compile(r"\b(asc_[A-Za-z0-9_]+)\s*\(")
        for project_path in project_paths:
            symbols: set[str] = set()
            for source_path in sorted(project_path.rglob("*")):
                if (
                    source_path.is_file()
                    and source_path.suffix.lower() in EXAMPLE_SOURCE_SUFFIXES
                ):
                    source_text = _without_comments_and_literals(
                        source_path.read_text(encoding="utf-8", errors="replace")
                    )
                    symbols.update(symbol_re.findall(source_text))
            for symbol in symbols:
                source_projects.setdefault(symbol, set()).add(project_path)

    reviewed_by_document: dict[Path, list[ReviewedExampleMapping]] = {}
    for mapping in reviewed_mappings:
        reviewed_by_document.setdefault(mapping.document_path.resolve(), []).append(mapping)

    return ExampleDiscoveryIndex(
        backlinks={
            path: tuple(items) for path, items in backlinks.items()
        },
        source_projects={
            symbol: tuple(sorted(paths)) for symbol, paths in source_projects.items()
        },
        reviewed_mappings={
            path: tuple(items) for path, items in reviewed_by_document.items()
        },
    )


def is_authoritative_example(example: ExampleReference) -> bool:
    return example.path is not None and example.confidence == "authoritative"


def public_example_signature_entries(example: ExampleReference) -> list[dict[str, Any]]:
    entries: list[dict[str, Any]] = []
    seen: set[tuple[str, str]] = set()
    for call in example.calls:
        function = call.canonical_id or call.call_template
        if not function:
            continue
        for binding in call.placeholder_bindings or ():
            if isinstance(binding, dict) and "name" in binding and "value" in binding:
                function = function.replace(
                    f"<{binding['name']}>", str(binding["value"])
                )
        if PLACEHOLDER_RE.search(function):
            continue
        field, override_id = signature_identifier(function)
        if field != "override_id":
            continue
        call_file = call.file
        if example.path and call_file and call_file.startswith(f"{example.path}/"):
            call_file = call_file[len(example.path) + 1 :]
        key = (override_id, call_file)
        if key in seen:
            continue
        seen.add(key)
        entries.append({"override_id": override_id, "file": call_file})
    return entries


def example_project_category(project_path: Path, repo_root: Path) -> str:
    """Return the example-family path, excluding the individual project name."""
    try:
        relative = project_path.resolve().relative_to((repo_root / "examples").resolve())
    except ValueError:
        return "未分类"
    parts = relative.parts
    if not parts:
        return "未分类"
    if parts[0] == "02_simd_c_api":
        family = parts[1:-1]
        return "/".join(family) if family else parts[0]
    return "/".join(parts[:-1]) if len(parts) > 1 else parts[0]


def _without_comments_and_literals(source_text: str) -> str:
    output = []
    index = 0
    state = "code"
    quote = ""
    while index < len(source_text):
        character = source_text[index]
        following = source_text[index + 1] if index + 1 < len(source_text) else ""
        if state == "code":
            if character == "/" and following == "/":
                output.extend((" ", " "))
                index += 2
                state = "line_comment"
                continue
            if character == "/" and following == "*":
                output.extend((" ", " "))
                index += 2
                state = "block_comment"
                continue
            if character in {'"', "'"}:
                quote = character
                output.append(" ")
                state = "literal"
            else:
                output.append(character)
        elif state == "line_comment":
            if character == "\n":
                output.append("\n")
                state = "code"
            else:
                output.append(" ")
        elif state == "block_comment":
            if character == "*" and following == "/":
                output.extend((" ", " "))
                index += 2
                state = "code"
                continue
            output.append("\n" if character == "\n" else " ")
        else:
            if character == "\\" and following:
                output.extend((" ", "\n" if following == "\n" else " "))
                index += 2
                continue
            output.append("\n" if character == "\n" else " ")
            if character == quote:
                state = "code"
        index += 1
    return "".join(output)


def _enclosing_example_function(
    source_text: str, call_offset: int
) -> tuple[Optional[str], Optional[str], int]:
    stack: list[tuple[Optional[str], Optional[str], int]] = []
    statement_start = 0
    function_re = re.compile(
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*"
        r"\((?P<parameters>.*)\)\s*(?:const\s*)?(?:noexcept\s*)?$",
        re.DOTALL,
    )
    for index, character in enumerate(source_text[:call_offset]):
        if character == "{":
            header = source_text[statement_start:index].strip()
            match = function_re.search(header)
            name = match.group("name") if match else None
            if name in EXAMPLE_CONTROL_KEYWORDS:
                name = None
            stack.append(
                (
                    name,
                    match.group("parameters") if match and name else None,
                    index + 1,
                )
            )
            statement_start = index + 1
        elif character == "}":
            if stack:
                stack.pop()
            statement_start = index + 1
        elif character == ";":
            statement_start = index + 1
    for name, parameters, body_start in reversed(stack):
        if name:
            return name, parameters, body_start
    return None, None, 0


EXAMPLE_DECLARATION_RE = re.compile(
    r"(?:^|[;{}]\s*)\s*"
    r"(?P<type>(?:(?:const|volatile|constexpr|static|__\w+__)\s+)*"
    r"(?:[A-Za-z_][A-Za-z0-9_:]*)(?:\s*<[^;{}=]+>)?"
    r"(?:\s*[*&]\s*)*)\s+"
    r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*"
    r"(?P<array>(?:\[[^\]]*\])*)\s*(?=[=;,])",
    re.MULTILINE,
)
EXAMPLE_USING_ALIAS_RE = re.compile(
    r"^\s*using\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<type>[^;]+);",
    re.MULTILINE,
)
EXAMPLE_PP_IF_RE = re.compile(r"^\s*#\s*if(?:def|ndef)?\s+(?P<condition>.+)$")
EXAMPLE_PP_ELIF_RE = re.compile(r"^\s*#\s*elif\s+(?P<condition>.+)$")
EXAMPLE_PP_ELSE_RE = re.compile(r"^\s*#\s*else\b")
EXAMPLE_PP_ENDIF_RE = re.compile(r"^\s*#\s*endif\b")
EXAMPLE_CONST_ALIAS_RE = re.compile(
    r"\bconstexpr\s+\w+\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*"
    r"(?P<value>[A-Za-z_][A-Za-z0-9_]*)\s*;"
)


def _example_condition_key(condition: str, const_aliases: dict[str, str]) -> Optional[tuple[str, str]]:
    match = re.search(
        r"\b(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*==\s*(?P<value>[A-Za-z0-9_]+)",
        condition,
    )
    if not match:
        return None
    name = const_aliases.get(match.group("name"), match.group("name"))
    return name, match.group("value")


def _example_const_aliases(source_text: str) -> dict[str, str]:
    return {
        match.group("name"): match.group("value")
        for match in EXAMPLE_CONST_ALIAS_RE.finditer(source_text)
    }


def _example_preprocessor_conditions(source_text: str, offset: int) -> tuple[str, ...]:
    conditions: list[str] = []
    line_offset = 0
    for raw_line in source_text.splitlines(keepends=True):
        if line_offset >= offset:
            break
        line = raw_line.rstrip("\r\n")
        if match := EXAMPLE_PP_IF_RE.match(line):
            conditions.append(match.group("condition").strip())
        elif match := EXAMPLE_PP_ELIF_RE.match(line):
            if conditions:
                conditions[-1] = match.group("condition").strip()
        elif EXAMPLE_PP_ELSE_RE.match(line):
            if conditions:
                conditions[-1] = f"!({conditions[-1]})"
        elif EXAMPLE_PP_ENDIF_RE.match(line):
            if conditions:
                conditions.pop()
        line_offset += len(raw_line)
    return tuple(conditions)


def _example_conditional_type_aliases(
    source_text: str,
) -> dict[str, tuple[tuple[tuple[str, ...], str], ...]]:
    aliases: dict[str, list[tuple[tuple[str, ...], str]]] = {}
    line_offset = 0
    for raw_line in source_text.splitlines(keepends=True):
        line = raw_line.rstrip("\r\n")
        match = EXAMPLE_USING_ALIAS_RE.match(line)
        if match:
            conditions = _example_preprocessor_conditions(source_text, line_offset)
            aliases.setdefault(match.group("name"), []).append(
                (conditions, _normalize_parameter_spacing(match.group("type")))
            )
        line_offset += len(raw_line)
    return {name: tuple(values) for name, values in aliases.items()}


def _condition_matches(
    alias_conditions: Sequence[str],
    call_conditions: Sequence[str],
    const_aliases: dict[str, str],
) -> bool:
    if not alias_conditions:
        return True
    call_keys = {
        key
        for condition in call_conditions
        if (key := _example_condition_key(condition, const_aliases)) is not None
    }
    for alias_condition in alias_conditions:
        alias_key = _example_condition_key(alias_condition, const_aliases)
        if alias_key is not None and alias_key in call_keys:
            return True
    return False


def _resolve_example_type_aliases(
    parameter_type: str,
    aliases: dict[str, tuple[tuple[tuple[str, ...], str], ...]],
    conditions: Sequence[str],
    const_aliases: dict[str, str],
) -> str:
    resolved = parameter_type
    for token in TYPE_TOKEN_RE.findall(parameter_type):
        variants = aliases.get(token)
        if not variants:
            continue
        matching = [
            value
            for alias_conditions, value in variants
            if _condition_matches(alias_conditions, conditions, const_aliases)
        ]
        if len(set(matching)) == 1:
            resolved = re.sub(rf"\b{re.escape(token)}\b", matching[0], resolved)
    return _normalize_parameter_spacing(resolved)


def _masked_top_level_source(source_text: str, end_offset: int) -> str:
    output = []
    depth = 0
    for character in source_text[:end_offset]:
        if character == "{":
            output.append(" ")
            depth += 1
        elif character == "}":
            output.append(" ")
            if depth:
                depth -= 1
        elif depth:
            output.append("\n" if character == "\n" else " ")
        else:
            output.append(character)
    return "".join(output)


def _collect_example_declarations(source_text: str) -> dict[str, str]:
    declared: dict[str, str] = {}
    for match in EXAMPLE_DECLARATION_RE.finditer(source_text):
        parameter_type = re.sub(r"\b(?:constexpr|static)\b\s*", "", match.group("type"))
        parameter_type = _normalize_parameter_spacing(parameter_type)
        if match.group("array"):
            parameter_type = f"{parameter_type}*"
        declared[match.group("name")] = parameter_type
    return declared


def _example_declared_types(
    source_text: str,
    call_offset: int,
    function_parameters: Optional[str],
    body_start: int,
) -> dict[str, str]:
    declared = _collect_example_declarations(
        _masked_top_level_source(source_text, call_offset)
    )
    if function_parameters:
        for parameter in _top_level_parameter_parts(function_parameters):
            name = _parameter_name(parameter)
            if name:
                declared[name] = _normalize_parameter_spacing(
                    _parameter_without_name(_remove_parameter_default(parameter))
                )

    declared.update(
        _collect_example_declarations(source_text[body_start:call_offset])
    )
    return declared


@lru_cache(maxsize=2048)
def _example_header_constant_types(
    source_text: str,
    repo_root: Path,
) -> dict[str, str]:
    direct_resources = []
    for include_match in HEADER_INCLUDE_RE.finditer(source_text):
        resources, _ = _header_path_resolution_details(
            include_match.group(1), repo_root
        )
        direct_resources.extend(resources)
    header_resources = _unique(
        (
            *direct_resources,
            *_resource_include_closure(repo_root, direct_resources),
        )
    )
    declarations: dict[str, list[str]] = {}
    for resource in header_resources:
        header_path = repo_root / resource.lstrip("/")
        if not header_path.is_file():
            continue
        header_text = _without_comments_and_literals(
            _read_header_text(str(header_path))
        )
        for match in EXAMPLE_INTEGRAL_CONSTANT_RE.finditer(
            header_text
        ):
            declarations.setdefault(match.group("name"), []).append(
                _normalize_parameter_spacing(match.group("type"))
            )
        for enum_match in EXAMPLE_SCOPED_ENUM_RE.finditer(header_text):
            enum_type = enum_match.group("type")
            for raw_enumerator in enum_match.group("body").split(","):
                enumerator_match = EXAMPLE_ENUMERATOR_RE.match(raw_enumerator)
                if enumerator_match is None:
                    continue
                declarations.setdefault(
                    f"{enum_type}::{enumerator_match.group('name')}", []
                ).append(enum_type)
    return {
        name: types[0]
        for name, types in declarations.items()
        if len(types) == 1
    }


def _example_argument_type(
    argument: str,
    declared: dict[str, str],
    aliases: Optional[dict[str, tuple[tuple[tuple[str, ...], str], ...]]] = None,
    conditions: Sequence[str] = (),
    const_aliases: Optional[dict[str, str]] = None,
    header_constant_types: Optional[dict[str, str]] = None,
) -> Optional[str]:
    argument = argument.strip()
    if argument in declared:
        declared_type = declared[argument]
        if aliases is not None:
            declared_type = _resolve_example_type_aliases(
                declared_type,
                aliases,
                conditions,
                const_aliases or {},
            )
        type_tokens = TYPE_TOKEN_RE.findall(declared_type)
        if any(token[0].isupper() for token in type_tokens):
            return None
        return declared_type
    pointer_offset_match = re.fullmatch(
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*[+-]\s*.+",
        argument,
    )
    if pointer_offset_match and pointer_offset_match.group("name") in declared:
        declared_type = declared[pointer_offset_match.group("name")]
        if aliases is not None:
            declared_type = _resolve_example_type_aliases(
                declared_type,
                aliases,
                conditions,
                const_aliases or {},
            )
        type_tokens = TYPE_TOKEN_RE.findall(declared_type)
        if declared_type.endswith("*") and not any(
            token[0].isupper() for token in type_tokens
        ):
            return declared_type
    if header_constant_types is not None and argument in header_constant_types:
        return header_constant_types[argument]
    address_match = re.fullmatch(r"&\s*([A-Za-z_][A-Za-z0-9_]*)", argument)
    if address_match and address_match.group(1) in declared:
        return f"{declared[address_match.group(1)]}*"
    cast_match = re.match(
        r"(?:static_cast|reinterpret_cast|const_cast)\s*<(?P<type>[^>]+)>\s*\(",
        argument,
    )
    if cast_match:
        return _normalize_parameter_spacing(cast_match.group("type"))
    c_cast_match = re.match(r"\(\s*(?P<type>[A-Za-z_][\w:\s*&<>]*)\s*\)", argument)
    if c_cast_match:
        return _normalize_parameter_spacing(c_cast_match.group("type"))
    if argument in {"true", "false"}:
        return "bool"
    if EXAMPLE_UINT32_LITERAL_RE.fullmatch(argument):
        return "uint32_t"
    return None


def _canonical_parameter_types(canonical_id: str) -> tuple[str, ...]:
    opening = canonical_id.find("(")
    if opening < 0 or not canonical_id.endswith(")"):
        return ()
    return _top_level_parameter_parts(canonical_id[opening + 1 : -1])


def _example_parameter_type_for_match(parameter_type: str) -> str:
    parameter_type = _without_top_level_cv_qualifiers(
        _normalize_parameter_spacing(parameter_type)
    )
    if parameter_type.endswith("&"):
        parameter_type = parameter_type[:-1]
    return parameter_type


def _example_type_matches(argument_type: str, parameter_type: str) -> bool:
    argument_type = _without_top_level_cv_qualifiers(
        _normalize_parameter_spacing(argument_type)
    )
    return argument_type == _example_parameter_type_for_match(parameter_type)


def _example_overload_status(
    arguments: str,
    declared: dict[str, str],
    canonical_ids: Sequence[str],
    inferred_types: Optional[Sequence[Optional[str]]] = None,
) -> tuple[str, Optional[str], tuple[str, ...]]:
    argument_parts = _top_level_parameter_parts(arguments)
    candidates = tuple(
        dict.fromkeys(
            canonical_id
            for canonical_id in canonical_ids
            if len(_canonical_parameter_types(canonical_id)) == len(argument_parts)
        )
    )
    if not candidates:
        return "mismatch", None, ()

    if inferred_types is None:
        inferred_types = tuple(
            _example_argument_type(argument, declared) for argument in argument_parts
        )
    compatible = tuple(
        canonical_id
        for canonical_id in candidates
        if all(
            inferred is None or _example_type_matches(inferred, parameter)
            for inferred, parameter in zip(
                inferred_types, _canonical_parameter_types(canonical_id)
            )
        )
    )
    if not compatible:
        discriminating_positions = tuple(
            index
            for index, inferred in enumerate(inferred_types)
            if inferred is not None
            and len(
                {
                    _example_parameter_type_for_match(
                        _canonical_parameter_types(canonical_id)[index]
                    )
                    for canonical_id in candidates
                }
            )
            > 1
        )
        conversion_compatible = tuple(
            canonical_id
            for canonical_id in candidates
            if all(
                _example_type_matches(
                    inferred_types[index],
                    _canonical_parameter_types(canonical_id)[index],
                )
                for index in discriminating_positions
            )
        )
        if len(conversion_compatible) == 1:
            return (
                "compatible_conversion",
                conversion_compatible[0],
                conversion_compatible,
            )
        if len(conversion_compatible) > 1:
            return "ambiguous", None, conversion_compatible
        return "unresolved", None, candidates
    if len(compatible) == 1:
        return "exact", compatible[0], compatible
    if all(inferred is not None for inferred in inferred_types):
        return "ambiguous", None, compatible
    if len(compatible) > 1:
        return "ambiguous", None, compatible
    if len(candidates) > 1 and any(inferred is not None for inferred in inferred_types):
        return "exact", compatible[0], compatible
    return "unresolved", None, compatible


def _line_start_offsets(source_text: str) -> tuple[int, ...]:
    offsets = [0]
    for match in re.finditer(r"\n", source_text):
        offsets.append(match.end())
    return tuple(offsets)


def _line_number_for_offset(line_offsets: Sequence[int], offset: int) -> int:
    line = 1
    for index, line_offset in enumerate(line_offsets, start=1):
        if line_offset > offset:
            break
        line = index
    return line


def _call_end_offset(source_text: str, opening_parenthesis_index: int) -> Optional[int]:
    depth = 0
    for index in range(opening_parenthesis_index, len(source_text)):
        character = source_text[index]
        if character == "(":
            depth += 1
        elif character == ")":
            depth -= 1
            if depth == 0:
                semicolon = source_text.find(";", index)
                if semicolon >= 0:
                    return semicolon
                return index
    return None


def _call_is_statement(source_text: str, opening_parenthesis_index: int) -> bool:
    """Return whether the call occupies a complete statement, not an expression."""
    depth = 0
    for index in range(opening_parenthesis_index, len(source_text)):
        character = source_text[index]
        if character == "(":
            depth += 1
        elif character == ")":
            depth -= 1
            if depth == 0:
                cursor = index + 1
                while cursor < len(source_text) and source_text[cursor].isspace():
                    cursor += 1
                return cursor < len(source_text) and source_text[cursor] == ";"
    return False


def _example_statement_line_range(
    source_text: str,
    call_offset: int,
    call_end_offset: int,
    conditions: Sequence[tuple[str, int]],
) -> tuple[int, int]:
    lines = source_text.splitlines()
    start_line = source_text.count("\n", 0, call_offset) + 1
    end_line = source_text.count("\n", 0, call_end_offset) + 1
    include_start = start_line
    index = start_line - 2
    while index >= 0:
        stripped = lines[index].strip()
        if not stripped:
            break
        if re.match(r"(?:constexpr|const)\b", stripped):
            include_start = index + 1
            index -= 1
            continue
        break
    if conditions:
        _, condition_line = conditions[-1]
        if condition_line < include_start and include_start - condition_line <= 4:
            include_start = condition_line
    return include_start, end_line


def _enclosing_example_conditions(
    source_text: str,
    call_offset: int,
) -> tuple[tuple[str, int], ...]:
    stack: list[tuple[Optional[str], int]] = []
    statement_start = 0
    line_offsets = _line_start_offsets(source_text)
    if_re = re.compile(
        r"\b(?:else\s+)?if\s+(?:constexpr\s*)?"
        r"(?:\((?P<paren>.*)\)|(?P<noparen>[A-Z_][A-Z0-9_]*))\s*$",
        re.DOTALL,
    )
    for index, character in enumerate(source_text[:call_offset]):
        if character == "{":
            header = source_text[statement_start:index].strip()
            match = if_re.search(header)
            condition = None
            if match:
                condition = (match.group("paren") or match.group("noparen") or "").strip()
                condition = " ".join(condition.split())
            stack.append((condition, _line_number_for_offset(line_offsets, index)))
            statement_start = index + 1
        elif character == "}":
            if stack:
                stack.pop()
            statement_start = index + 1
        elif character == ";":
            statement_start = index + 1
    return tuple((condition, line) for condition, line in stack if condition)


def _sanitize_snippet_part(value: str) -> str:
    value = value.strip()
    value = re.sub(r"\b(?:true|false)\b", lambda m: m.group(0).lower(), value)
    value = re.sub(r"[^A-Za-z0-9_]+", "_", value)
    value = re.sub(r"_+", "_", value).strip("_").lower()
    return value


def _example_role_and_snippet_parts(
    symbol: str,
    argument_parts: Sequence[str],
    parameter_names: Sequence[str],
    bindings: Sequence[dict[str, str]],
    include_binding_values: bool = False,
) -> tuple[tuple[str, ...], tuple[str, ...]]:
    roles = []
    parts = []
    if include_binding_values:
        for binding in bindings:
            value = binding.get("value")
            if value:
                part = _sanitize_snippet_part(value)
                if part and part not in parts:
                    parts.append(part)
    for index, parameter_name in enumerate(parameter_names):
        if index >= len(argument_parts):
            continue
        name = parameter_name.lower()
        argument = argument_parts[index].strip()
        if "flag_id" in name:
            roles.append("sync_flag")
            if argument:
                parts.append(_sanitize_snippet_part(argument))
        elif name == "pipe" or name.endswith("_pipe") or "pipe" in name:
            roles.append("pipe_sync")
        elif any(token in name for token in ("dst", "src", "matrix")):
            roles.append("data_move" if "copy" in symbol else "data_operand")
    if not roles:
        roles.append("direct_call")
    return tuple(dict.fromkeys(roles)), tuple(part for part in parts if part)


def _semantic_snippet_id(symbol: str, parts: Sequence[str]) -> str:
    suffix = tuple(part for part in parts if part)
    if not suffix:
        return symbol
    return ".".join((symbol, *suffix))


def _semantic_snippet_suffix(symbol: str, semantic_snippet: Optional[str]) -> tuple[str, ...]:
    if not semantic_snippet:
        return ()
    prefix = f"{symbol}."
    if semantic_snippet == symbol:
        return ()
    if semantic_snippet.startswith(prefix):
        return tuple(part for part in semantic_snippet[len(prefix) :].split(".") if part)
    return (_sanitize_snippet_part(semantic_snippet),)


def _condition_snippet_parts(conditions: Sequence[str]) -> tuple[str, ...]:
    parts = []
    for condition in conditions:
        match = re.fullmatch(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*==\s*([A-Za-z0-9_]+)\s*", condition)
        if match:
            part = _sanitize_snippet_part(f"{match.group(1)}_{match.group(2)}")
        else:
            part = _sanitize_snippet_part(condition)
        if part and part not in parts:
            parts.append(part)
    return tuple(parts)


def _binding_snippet_parts(bindings: Sequence[dict[str, str]]) -> tuple[str, ...]:
    parts = []
    for binding in bindings:
        value = binding.get("value")
        if not value:
            continue
        part = _sanitize_snippet_part(value)
        if part and part not in parts:
            parts.append(part)
    return tuple(parts)


def _semantic_collision_symbol_files(calls: Sequence[ExampleCall]) -> set[tuple[str, str]]:
    by_file_snippet: dict[tuple[str, str], set[str]] = {}
    by_file_snippet_symbol: dict[tuple[str, str], str] = {}
    for call in calls:
        if not call.semantic_snippet:
            continue
        key = (call.file, call.semantic_snippet)
        function = call.canonical_id or call.call_template or call.symbol
        by_file_snippet.setdefault(key, set()).add(function)
        by_file_snippet_symbol[key] = call.symbol
    return {
        (file, by_file_snippet_symbol[(file, snippet)])
        for (file, snippet), functions in by_file_snippet.items()
        if len(functions) > 1
    }


def _disambiguate_semantic_snippets(calls: Sequence[ExampleCall]) -> tuple[ExampleCall, ...]:
    result = tuple(calls)
    collision_symbol_files = _semantic_collision_symbol_files(result)
    if collision_symbol_files:
        updated = []
        for call in result:
            if (call.file, call.symbol) not in collision_symbol_files:
                updated.append(call)
                continue
            condition_parts = _condition_snippet_parts(call.condition)
            if not condition_parts:
                updated.append(call)
                continue
            suffix = _semantic_snippet_suffix(call.symbol, call.semantic_snippet)
            parts = (*condition_parts, *(part for part in suffix if part not in condition_parts))
            updated.append(
                replace(call, semantic_snippet=_semantic_snippet_id(call.symbol, parts))
            )
        result = tuple(updated)

    collision_symbol_files = _semantic_collision_symbol_files(result)
    if collision_symbol_files:
        updated = []
        for call in result:
            if (call.file, call.symbol) not in collision_symbol_files:
                updated.append(call)
                continue
            binding_parts = _binding_snippet_parts(call.placeholder_bindings)
            if not binding_parts:
                updated.append(call)
                continue
            suffix = _semantic_snippet_suffix(call.symbol, call.semantic_snippet)
            parts = (*binding_parts, *(part for part in suffix if part not in binding_parts))
            updated.append(
                replace(call, semantic_snippet=_semantic_snippet_id(call.symbol, parts))
            )
        result = tuple(updated)

    return result


def _example_snippet_markers(
    source_text: str,
    file_resource: str,
) -> tuple[tuple[ExampleSnippetMarker, ...], tuple[ExampleIssue, ...]]:
    markers: list[ExampleSnippetMarker] = []
    issues: list[ExampleIssue] = []
    active: Optional[tuple[tuple[str, ...], int, int]] = None
    offset = 0

    for line_number, raw_line in enumerate(source_text.splitlines(keepends=True), 1):
        line_text = raw_line.rstrip("\r\n")
        match = EXAMPLE_SNIPPET_MARKER_RE.match(line_text)
        if not match:
            if EXAMPLE_SNIPPET_MARKER_LIKE_RE.match(line_text):
                issues.append(
                    ExampleIssue(
                        "example_snippet_marker_invalid",
                        "样例代码段 marker 格式无效；请使用 ASCII 格式 //! [id] 或 //! [id1, id2, ...]。",
                        file_resource,
                        line_number,
                    )
                )
            offset += len(raw_line)
            continue

        marker_ids = tuple(sorted(set(part.strip() for part in match.group("id").split(","))))
        line_end_offset = offset + len(raw_line)
        if active is None:
            active = (marker_ids, line_end_offset, line_number)
        else:
            active_ids, start_offset, start_line = active
            if marker_ids != active_ids:
                issues.append(
                    ExampleIssue(
                        "example_snippet_marker_nested",
                        "样例代码段 marker 存在交叉或嵌套，当前版本不做自动配对。",
                        file_resource,
                        line_number,
                        cause=f"{','.join(active_ids)} -> {','.join(marker_ids)}",
                    )
                )
            else:
                if not source_text[start_offset:offset].strip():
                    issues.append(
                        ExampleIssue(
                            "example_snippet_empty",
                            "样例代码段 marker 中间没有有效源码内容。",
                            file_resource,
                            start_line,
                            cause=','.join(marker_ids),
                        )
                    )
                markers.extend(
                    ExampleSnippetMarker(
                        id=marker_id,
                        file=file_resource,
                        start_offset=start_offset,
                        end_offset=offset,
                        start_line=start_line,
                        end_line=line_number,
                    ) for marker_id in marker_ids
                )
                active = None
        offset += len(raw_line)

    if active is not None:
        marker_ids, _, start_line = active
        issues.append(
            ExampleIssue(
                "example_snippet_marker_unclosed",
                "样例代码段 marker 只有开始标记，没有结束标记。",
                file_resource,
                start_line,
                cause=','.join(marker_ids),
            )
        )
    return tuple(markers), tuple(issues)


def _snippet_marker_for_offset(
    markers: Sequence[ExampleSnippetMarker],
    offset: int,
    symbol: Optional[str] = None,
    override_id: Optional[str] = None,
) -> Optional[ExampleSnippetMarker]:
    for marker in markers:
        if marker.start_offset <= offset < marker.end_offset:
            if override_id is not None and marker.id != override_id:
                continue
            if symbol is not None and not (
                marker.id == symbol or marker.id.startswith((f"{symbol}__", f"{symbol}."))
            ):
                continue
            return marker
    return None


def _signature_parameter_names(signature: "SignatureHeaderMapping") -> tuple[str, ...]:
    parts = _top_level_parameter_parts(signature.document_parameters)
    names = []
    for index, parameter in enumerate(parts):
        names.append(_parameter_name(parameter) or f"arg{index}")
    return tuple(names)


def _placeholder_name(raw_name: str, index: int) -> str:
    clean = re.sub(r"\W+", "_", raw_name).strip("_")
    if not clean:
        clean = f"arg{index}"
    if clean.endswith("_type"):
        return clean
    return f"{clean}_type"


def _placeholder_parameter_template(
    parameters: Sequence[str],
    placeholder: str,
) -> tuple[str, Optional[int]]:
    if not parameters:
        return f"<{placeholder}>", None
    first = parameters[0]
    token_matches = [list(TYPE_TOKEN_RE.finditer(parameter)) for parameter in parameters]
    if token_matches and all(len(matches) == len(token_matches[0]) for matches in token_matches):
        for token_index, first_match in enumerate(token_matches[0]):
            values = {
                matches[token_index].group(0)
                for matches in token_matches
            }
            if len(values) > 1:
                return (
                    first[: first_match.start()]
                    + f"<{placeholder}>"
                    + first[first_match.end() :],
                    token_index,
                )
    return f"<{placeholder}>", None


def _placeholder_binding_value(
    inferred_type: str,
    token_index: Optional[int],
) -> str:
    if token_index is None:
        return inferred_type
    matches = list(TYPE_TOKEN_RE.finditer(inferred_type))
    if token_index < len(matches):
        return matches[token_index].group(0)
    return inferred_type


def _example_call_template(
    symbol: str,
    argument_parts: Sequence[str],
    inferred_types: Sequence[Optional[str]],
    arity_signatures: Sequence["SignatureHeaderMapping"],
) -> tuple[Optional[str], tuple[dict[str, str], ...], str]:
    if not arity_signatures:
        return None, (), "none"

    parameter_matrix = [
        _canonical_parameter_types(signature.canonical_id)
        for signature in arity_signatures
    ]
    if not parameter_matrix:
        return None, (), "none"

    first_parameters = parameter_matrix[0]
    parameter_names = _signature_parameter_names(arity_signatures[0])
    if len(parameter_names) != len(first_parameters):
        parameter_names = tuple(f"arg{index}" for index in range(len(first_parameters)))

    templated_parameters: list[str] = []
    bindings: list[dict[str, str]] = []
    for index, first_parameter in enumerate(first_parameters):
        parameter_variants = tuple(parameters[index] for parameters in parameter_matrix)
        if len(set(parameter_variants)) <= 1:
            templated_parameters.append(first_parameter)
            continue
        placeholder = _placeholder_name(parameter_names[index], index)
        templated, token_index = _placeholder_parameter_template(
            parameter_variants,
            placeholder,
        )
        templated_parameters.append(templated)
        binding = {
            "name": placeholder,
            "argument": argument_parts[index].strip() if index < len(argument_parts) else "",
            "status": "unresolved",
        }
        if index < len(inferred_types) and inferred_types[index] is not None:
            binding["value"] = _placeholder_binding_value(
                inferred_types[index] or "",
                token_index,
            )
            binding["status"] = "resolved"
        bindings.append(binding)

    if not bindings:
        return f"{symbol}({','.join(first_parameters)})", (), "none"
    if all(binding["status"] == "resolved" for binding in bindings):
        status = "resolved"
    elif any(binding["status"] == "resolved" for binding in bindings):
        status = "partial"
    else:
        status = "unresolved"
    return f"{symbol}({','.join(templated_parameters)})", tuple(bindings), status


def _example_archs(project_path: Path) -> tuple[str, ...]:
    archs = []
    cmake_files = sorted(project_path.rglob("CMakeLists.txt"))
    for cmake_path in cmake_files:
        text = cmake_path.read_text(encoding="utf-8", errors="replace")
        archs.extend(re.findall(r"\bdav-(\d{4})\b", text))
    if archs:
        return tuple(dict.fromkeys(archs))

    for readme_name in ("README.md", "README_en.md"):
        readme_path = project_path / readme_name
        if not readme_path.is_file():
            continue
        text = readme_path.read_text(encoding="utf-8", errors="replace")
        archs.extend(re.findall(r"\bdav-(\d{4})\b", text))
        for label, arch in NPU_LABEL_TO_ARCH.items():
            if label in text:
                archs.append(arch)
    return tuple(dict.fromkeys(archs))


def _example_calls(
    project_path: Path,
    repo_root: Path,
    signatures_by_symbol: dict[str, tuple["SignatureHeaderMapping", ...]],
) -> tuple[tuple[ExampleCall, ...], tuple[ExampleIssue, ...]]:
    calls = []
    issues: list[ExampleIssue] = []
    symbols = tuple(sorted(signatures_by_symbol, key=len, reverse=True))
    if not symbols:
        return (), ()
    call_re = re.compile(
        rf"\b(?P<symbol>{'|'.join(re.escape(symbol) for symbol in symbols)})\s*\("
    )
    any_api_call_re = re.compile(r"\basc_[A-Za-z0-9_]+\s*\(")
    for source_path in sorted(project_path.rglob("*")):
        if (
            not source_path.is_file()
            or source_path.suffix.lower() not in EXAMPLE_SOURCE_SUFFIXES
        ):
            continue
        file_resource = f"/{source_path.relative_to(repo_root).as_posix()}"
        raw_source_text = source_path.read_text(encoding="utf-8", errors="replace")
        const_aliases = _example_const_aliases(raw_source_text)
        type_aliases = _example_conditional_type_aliases(raw_source_text)
        header_constant_types = _example_header_constant_types(
            raw_source_text, repo_root
        )
        markers, marker_issues = _example_snippet_markers(
            raw_source_text,
            file_resource,
        )
        issues.extend(marker_issues)

        source_text = _without_comments_and_literals(raw_source_text)
        api_call_offsets = [match.start() for match in any_api_call_re.finditer(source_text)]
        for marker in markers:
            if not any(marker.start_offset <= offset < marker.end_offset for offset in api_call_offsets):
                issues.append(
                    ExampleIssue(
                        "example_snippet_no_api_call",
                        "样例代码段 marker 中没有 asc_* API 调用。",
                        marker.file,
                        marker.start_line,
                        cause=marker.id,
                    )
                )

        for match in call_re.finditer(source_text):
            opening = source_text.find("(", match.start())
            arguments = _parenthesized_content(source_text, opening)
            if arguments is None:
                continue
            symbol = match.group("symbol")
            argument_parts = _top_level_parameter_parts(arguments)
            call_end = _call_end_offset(source_text, opening)
            if call_end is None:
                call_end = match.end()
            is_statement = _call_is_statement(source_text, opening)
            block_conditions = _enclosing_example_conditions(source_text, match.start())
            conditions = (
                *_example_preprocessor_conditions(source_text, match.start()),
                *(condition for condition, _ in block_conditions),
            )
            line_range = _example_statement_line_range(
                source_text,
                match.start(),
                call_end,
                block_conditions,
            )
            snippet, function_parameters, body_start = _enclosing_example_function(
                source_text, match.start()
            )
            declared = _example_declared_types(
                source_text, match.start(), function_parameters, body_start
            )
            inferred_types = tuple(
                _example_argument_type(
                    argument,
                    declared,
                    type_aliases,
                    conditions,
                    const_aliases,
                    header_constant_types,
                )
                for argument in argument_parts
            )
            signatures = signatures_by_symbol[symbol]
            canonical_ids = tuple(signature.canonical_id for signature in signatures)
            arity_signatures = tuple(
                signature
                for signature in signatures
                if len(_canonical_parameter_types(signature.canonical_id))
                == len(argument_parts)
            )
            status, canonical_id, candidates = _example_overload_status(
                arguments,
                declared,
                canonical_ids,
                inferred_types,
            )
            call_template, bindings, placeholder_status = _example_call_template(
                symbol,
                argument_parts,
                inferred_types,
                arity_signatures,
            )
            parameter_names = (
                _signature_parameter_names(arity_signatures[0])
                if arity_signatures
                else tuple(f"arg{index}" for index in range(len(argument_parts)))
            )
            roles, snippet_parts = _example_role_and_snippet_parts(
                symbol,
                argument_parts,
                parameter_names,
                bindings,
                include_binding_values=canonical_id is None,
            )
            semantic_snippet = _semantic_snippet_id(symbol, snippet_parts)
            call_override_id = None
            if canonical_id is not None and not PLACEHOLDER_RE.search(canonical_id):
                identifier_field, identifier_value = signature_identifier(canonical_id)
                if identifier_field == "override_id":
                    call_override_id = identifier_value
            marker = _snippet_marker_for_offset(
                markers, match.start(), symbol, call_override_id,
            )
            if marker is not None:
                snippet = marker.id
                snippet_kind = "marker"
                snippet_start_line = marker.start_line
                snippet_end_line = marker.end_line
            else:
                snippet_kind = "function" if snippet is not None else "unresolved"
                snippet_start_line = None
                snippet_end_line = None
            calls.append(
                ExampleCall(
                    file=file_resource,
                    line=source_text.count("\n", 0, match.start()) + 1,
                    symbol=symbol,
                    snippet=snippet,
                    overload_status=status,
                    snippet_kind=snippet_kind,
                    snippet_start_line=snippet_start_line,
                    snippet_end_line=snippet_end_line,
                    canonical_id=canonical_id,
                    candidates=candidates,
                    inferred_types=inferred_types,
                    call_template=call_template,
                    placeholder_bindings=bindings,
                    placeholder_status=placeholder_status,
                    semantic_snippet=semantic_snippet,
                    line_range=line_range,
                    condition=conditions,
                    role=roles,
                    is_statement=is_statement,
                    call_start_offset=match.start(),
                    call_end_offset=call_end,
                )
            )
    return _disambiguate_semantic_snippets(calls), tuple(issues)


def _canonical_ids_by_symbol(
    signatures: Sequence["SignatureHeaderMapping"],
) -> dict[str, tuple[str, ...]]:
    grouped: dict[str, list[str]] = {}
    for signature in signatures:
        grouped.setdefault(signature.name, []).append(signature.canonical_id)
    return {
        symbol: tuple(dict.fromkeys(canonical_ids))
        for symbol, canonical_ids in grouped.items()
    }


def _signatures_by_symbol(
    signatures: Sequence["SignatureHeaderMapping"],
) -> dict[str, tuple["SignatureHeaderMapping", ...]]:
    grouped: dict[str, list[SignatureHeaderMapping]] = {}
    for signature in signatures:
        grouped.setdefault(signature.name, []).append(signature)
    return {
        symbol: tuple(items)
        for symbol, items in grouped.items()
    }


def _validate_example_project(
    project_path: Path,
    source_target: str,
    signatures_by_symbol: dict[str, tuple["SignatureHeaderMapping", ...]],
    document_archs: Sequence[str],
    repo_root: Path,
    discovery: str,
    confidence: str = "authoritative",
) -> ExampleReference:
    project_resource = f"/{project_path.relative_to(repo_root).as_posix()}"
    category = example_project_category(project_path, repo_root)
    issues: list[ExampleIssue] = []
    calls: tuple[ExampleCall, ...] = ()
    archs: tuple[str, ...] = ()
    if not project_path.is_dir():
        issues.append(ExampleIssue("example_path_not_found", "样例工程目录不存在。"))
    else:
        archs = _example_archs(project_path)
        calls, marker_issues = _example_calls(
            project_path,
            repo_root,
            signatures_by_symbol,
        )
        issues.extend(marker_issues)
        if not calls:
            issue_type = (
                "example_backlink_api_not_found"
                if discovery == "readme_backlink"
                else "example_api_not_found"
            )
            issues.append(
                ExampleIssue(
                    issue_type,
                    "样例 README 指向当前 API 文档，但工程源码中没有找到对应调用。"
                    if discovery == "readme_backlink"
                    else "样例工程中没有找到当前 API 文档函数的调用。",
                )
            )
        if not archs:
            issues.append(
                ExampleIssue(
                    "example_arch_unknown",
                    "无法从样例 CMake 或 README 确定 NPU 架构。",
                )
            )
        elif document_archs and not set(archs).intersection(document_archs):
            issues.append(
                ExampleIssue(
                    "example_arch_conflict",
                    "样例架构与 API 文档支持架构没有交集。",
                )
            )
        for call in calls:
            if call.overload_status == "mismatch":
                issues.append(
                    ExampleIssue(
                        "example_signature_mismatch",
                        "样例调用参数与文档中的所有函数原型都不匹配。",
                        call.file,
                        call.line,
                        call.symbol,
                        "参数数量不匹配" if not call.candidates else "参数类型不匹配",
                    )
                )
            elif call.overload_status == "ambiguous":
                issues.append(
                    ExampleIssue(
                        "example_overload_ambiguous",
                        "文本分析无法唯一确定样例调用对应的重载。",
                        call.file,
                        call.line,
                        call.symbol,
                        "多个重载均可匹配",
                    )
                )
            elif call.overload_status == "unresolved":
                issues.append(
                    ExampleIssue(
                        "example_overload_unresolved",
                        "样例调用缺少足够的显式类型信息，无法确认重载。",
                        call.file,
                        call.line,
                        call.symbol,
                        "调用参数缺少可推断类型",
                    )
                )
            if call.snippet is None:
                issues.append(
                    ExampleIssue(
                        "example_snippet_unresolved",
                        "已找到 API 调用，但无法稳定确定所在函数。",
                        call.file,
                        call.line,
                        call.symbol,
                    )
                )
            elif call.snippet_kind == "marker" and (
                call.snippet != call.symbol
                and not call.snippet.startswith(f"{call.symbol}.")
                and not call.snippet.startswith(f"{call.symbol}__")
            ):
                issues.append(
                    ExampleIssue(
                        "example_snippet_marker_symbol_mismatch",
                        "样例代码段 marker ID 第一段应使用函数名。",
                        call.file,
                        call.line,
                        call.symbol,
                        f"{call.snippet} != {call.symbol}",
                    )
                )
    if discovery == "reviewed_mapping" and (
        not project_path.is_dir() or not calls
    ):
        issues.append(
            ExampleIssue(
                "example_mapping_stale",
                "人工确认的样例映射已经无法通过路径或源码调用校验。",
            )
        )
    return ExampleReference(
        path=project_resource,
        source_target=source_target,
        archs=archs,
        calls=calls,
        issues=tuple(issues),
        discovery=discovery,
        confidence=confidence,
        category=category,
    )


def extract_example_references(
    markdown_text: str,
    document_path: Path,
    signatures: Sequence["SignatureHeaderMapping"],
    document_archs: Sequence[str],
    repo_root: Path = REPO_ROOT,
    discovery_index: Optional[ExampleDiscoveryIndex] = None,
) -> tuple[ExampleReference, ...]:
    repo_root = repo_root.resolve()
    document_path = document_path.resolve()
    section_text = _section_text(markdown_text, EXAMPLE_SECTION_RE)
    targets = [
        target
        for target in _markdown_link_targets(section_text or "")
        if "examples/" in unquote(target).replace("\\", "/")
    ]
    references: list[ExampleReference] = []
    by_path: dict[str, int] = {}
    signatures_by_symbol = _signatures_by_symbol(signatures)

    for source_target in targets:
        parsed = urlsplit(source_target)
        if parsed.scheme or parsed.netloc:
            references.append(
                ExampleReference(
                    path=None,
                    source_target=source_target,
                    issues=(
                        ExampleIssue(
                            "example_path_invalid",
                            "样例链接不是仓库内的相对路径。",
                        ),
                    ),
                )
            )
            continue
        resolved = (document_path.parent / unquote(parsed.path)).resolve()
        try:
            relative = resolved.relative_to(repo_root)
        except ValueError:
            references.append(
                ExampleReference(
                    path=None,
                    source_target=source_target,
                    issues=(
                        ExampleIssue(
                            "example_path_invalid",
                            "样例链接解析后位于仓库之外。",
                        ),
                    ),
                )
            )
            continue
        if not relative.parts or relative.parts[0] != "examples":
            references.append(
                ExampleReference(
                    path=None,
                    source_target=source_target,
                    issues=(
                        ExampleIssue(
                            "example_path_invalid",
                            "样例链接没有指向仓库 examples 目录。",
                        ),
                    ),
                )
            )
            continue
        project_path = (
            resolved.parent
            if resolved.name in {"README.md", "README_en.md"}
            else resolved
        )
        project_resource = f"/{project_path.relative_to(repo_root).as_posix()}"
        if project_resource in by_path:
            index = by_path[project_resource]
            current = references[index]
            references[index] = replace(
                current,
                issues=(
                    *current.issues,
                    ExampleIssue(
                        "example_duplicate",
                        "同一文档重复引用了相同的样例工程。",
                    ),
                ),
            )
            continue
        by_path[project_resource] = len(references)
        references.append(
            _validate_example_project(
                project_path,
                source_target,
                signatures_by_symbol,
                document_archs,
                repo_root,
                "document_link",
            )
        )

    if discovery_index is None:
        discovery_index = ExampleDiscoveryIndex({}, {}, {})

    for backlink in discovery_index.backlinks.get(document_path, ()):
        project_resource = f"/{backlink.project_path.relative_to(repo_root).as_posix()}"
        if project_resource in by_path:
            continue
        readme_resource = f"/{backlink.readme_path.relative_to(repo_root).as_posix()}"
        by_path[project_resource] = len(references)
        references.append(
            _validate_example_project(
                backlink.project_path,
                f"{readme_resource} -> {backlink.source_target}",
                signatures_by_symbol,
                document_archs,
                repo_root,
                "readme_backlink",
            )
        )

    for mapping in discovery_index.reviewed_mappings.get(document_path, ()):
        project_path = mapping.project_path.resolve()
        try:
            project_resource = f"/{project_path.relative_to(repo_root).as_posix()}"
        except ValueError:
            references.append(
                ExampleReference(
                    path=None,
                    source_target=mapping.reason,
                    discovery="reviewed_mapping",
                    issues=(
                        ExampleIssue(
                            "example_mapping_stale",
                            "人工确认的样例映射位于仓库之外。",
                        ),
                    ),
                )
            )
            continue
        if project_resource in by_path:
            continue
        by_path[project_resource] = len(references)
        references.append(
            _validate_example_project(
                project_path,
                mapping.reason,
                signatures_by_symbol,
                document_archs,
                repo_root,
                "reviewed_mapping",
            )
        )

    def confirmed_source_reference(
        reference: ExampleReference,
    ) -> Optional[ExampleReference]:
        """Keep only calls proven against this document's declarations."""
        if reference.path is None:
            return None
        if document_archs and (
            not reference.archs
            or not set(reference.archs).intersection(document_archs)
        ):
            return None
        confirmed_calls = tuple(
            call
            for call in reference.calls
            if call.overload_status == "exact"
            and call.canonical_id is not None
            and call.inferred_types is not None
            and all(isinstance(value, str) and value for value in call.inferred_types)
        )
        if not confirmed_calls:
            return None
        if (
            reference.confidence == "authoritative"
            and not reference.issues
            and len(confirmed_calls) == len(reference.calls)
        ):
            return None
        return replace(
            reference,
            source_target=f"{reference.source_target} -> source reverse confirmation",
            calls=confirmed_calls,
            issues=(),
            discovery="source_inference",
            confidence="authoritative",
        )

    # Preserve linked-example diagnostics, but add a clean evidence record for
    # every individual call whose overload is uniquely confirmed.  A warning
    # about a different call in the same project must not suppress this call.
    confirmed_paths = {
        reference.path
        for reference in references
        if reference.path is not None
        and reference.confidence == "authoritative"
        and not reference.issues
    }
    for reference in tuple(references):
        confirmation = confirmed_source_reference(reference)
        if confirmation is not None:
            references.append(confirmation)
            confirmed_paths.add(confirmation.path)

    candidate_projects = {
        project_path
        for symbol in signatures_by_symbol
        for project_path in discovery_index.source_projects.get(symbol, ())
    }
    exact_candidates: list[ExampleReference] = []
    possible_candidates: list[ExampleReference] = []
    for project_path in sorted(candidate_projects):
        project_resource = f"/{project_path.relative_to(repo_root).as_posix()}"
        if project_resource in confirmed_paths:
            continue
        candidate = _validate_example_project(
            project_path,
            "source inference",
            signatures_by_symbol,
            document_archs,
            repo_root,
            "source_inference",
            "review",
        )
        confirmation = confirmed_source_reference(candidate)
        if confirmation is not None:
            exact_candidates.append(confirmation)
        elif any(
            call.overload_status in {"ambiguous", "unresolved"}
            for call in candidate.calls
        ):
            possible_candidates.append(candidate)

    references.extend(exact_candidates)

    # Ambiguous source matches remain review-only and cannot create YAML
    # example associations or snippet markers.
    if not exact_candidates and not references and len(possible_candidates) == 1:
        candidate = possible_candidates[0]
        message = "源码中只找到一个可能的样例工程，但重载类型尚无法确认。"
        references.append(
            replace(
                candidate,
                issues=(*candidate.issues, ExampleIssue(
                    "example_candidate_unresolved", message
                )),
            )
        )
    elif not exact_candidates and not references and len(possible_candidates) > 1:
        candidate_paths = tuple(
            candidate.path
            for candidate in possible_candidates
            if candidate.path is not None
        )
        references.append(
            ExampleReference(
                path=None,
                source_target="source inference",
                discovery="source_inference",
                confidence="review",
                candidate_paths=candidate_paths,
                category="多个工程族",
                candidate_categories=tuple(
                    dict.fromkeys(
                        example_project_category(
                            project_path,
                            repo_root,
                        )
                        for project_path in candidate_projects
                        if f"/{project_path.relative_to(repo_root).as_posix()}"
                        in candidate_paths
                    )
                ),
                issues=(
                    ExampleIssue(
                        "example_candidate_ambiguous",
                        f"源码中找到 {len(candidate_paths)} 个可能的样例工程，不能自动绑定。",
                    ),
                ),
            )
        )
    if not references:
        section_note = (
            "文档没有调用示例章节，未找到可定位的样例工程。"
            if section_text is None
            else "未找到文档正向链接、README 反向链接、人工映射或源码候选样例。"
        )
        references.append(
            ExampleReference(
                path=None,
                source_target="",
                discovery="none",
                confidence="authoritative",
                category="未找到",
                issues=(ExampleIssue("example_not_found", section_note),),
            )
        )
    return tuple(references)


# =============================================================================
# 头文件查找：从 Markdown 提取并展开函数原型
# =============================================================================
# 本阶段解析函数原型代码块、模板和 dtype 占位符，并保留架构条件及原始原型信息。
# 输出的 PrototypeDeclaration 是后续头文件候选匹配的统一输入。
def _parenthesized_content(
    text: str, opening_parenthesis_index: int
) -> Optional[str]:
    depth = 0
    for index in range(opening_parenthesis_index, len(text)):
        character = text[index]
        if character == "(":
            depth += 1
        elif character == ")":
            depth -= 1
            if depth == 0:
                return text[opening_parenthesis_index + 1 : index]
    return None


def _function_parameter_text(text: str, symbol: str) -> Optional[str]:
    if symbol.startswith("operator"):
        pattern = rf"\b{re.escape(symbol)}\s*\("
    else:
        pattern = (
            rf"(?:\b|::){re.escape(symbol)}"
            rf"(?:\s*<[^>\n]+>)?\s*\("
        )
    symbol_match = re.search(pattern, text)
    if symbol_match is None:
        return None
    opening_parenthesis_index = text.find("(", symbol_match.start())
    return _parenthesized_content(text, opening_parenthesis_index)


def _prototype_code_blocks(section_text: str) -> Sequence[str]:
    blocks = []
    in_block = False
    skip_block = False
    block_lines = []
    recent_context = []
    for line in section_text.splitlines():
        if FENCE_RE.match(line):
            if in_block:
                if not skip_block:
                    blocks.append("\n".join(block_lines))
                in_block = False
                skip_block = False
                block_lines = []
            else:
                context = "\n".join(recent_context[-2:])
                skip_block = bool(EXAMPLE_MARK_RE.search(context))
                in_block = True
            continue
        if in_block:
            block_lines.append(line)
            continue
        stripped = line.strip()
        if not stripped:
            continue
        if SUBSECTION_RE.match(line) or stripped.startswith("- "):
            recent_context = [stripped]
        else:
            recent_context.append(stripped)
    return tuple(blocks)


def _placeholder_values(section_text: str) -> dict[str, Sequence[str]]:
    values_by_name = {}
    for match in DTYPE_LIST_RE.finditer(section_text):
        values_by_name["dtype"] = tuple(dict.fromkeys(BACKTICK_RE.findall(match.group(1))))

    support_heading_placeholder = None
    for line in section_text.splitlines():
        support_heading_match = PLACEHOLDER_SUPPORT_HEADING_RE.match(line)
        if support_heading_match is not None:
            support_heading_placeholder = support_heading_match.group("placeholder")
            continue

        if support_heading_placeholder is not None:
            stripped_line = line.strip()
            if not stripped_line:
                continue
            heading_values = BACKTICK_RE.findall(line)
            remaining_text = BACKTICK_RE.sub("", line)
            if (
                not stripped_line.startswith("|")
                and heading_values
                and all(
                    re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", value)
                    for value in heading_values
                )
                and not re.search(r"[A-Za-z0-9_\u4e00-\u9fff]", remaining_text)
            ):
                values_by_name[support_heading_placeholder] = tuple(
                    dict.fromkeys(heading_values)
                )
            support_heading_placeholder = None

        if CORRELATED_PLACEHOLDER_TUPLES_RE.search(line):
            continue
        dtype_support_match = DTYPE_SUPPORT_SENTENCE_RE.search(line)
        if dtype_support_match is not None:
            dtype_values = [
                value
                for value in BACKTICK_RE.findall(dtype_support_match.group("values"))
                if re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", value)
            ]
            if dtype_values:
                values_by_name[dtype_support_match.group("placeholder")] = tuple(
                    dict.fromkeys(dtype_values)
                )
                continue
        value_clause = re.split(r"[。；;]\s*(?=当)", line, maxsplit=1)[0]
        values = BACKTICK_RE.findall(value_clause)
        explicit_placeholder_match = EXPLICIT_PLACEHOLDER_RE.search(value_clause)
        has_explicit_placeholder = explicit_placeholder_match is not None
        if not has_explicit_placeholder and not re.search(
            r"(支持的数据类型|支持数据类型|可取的数据类型|可取值|取值如下|取值为|支持.*类型)",
            line,
        ):
            continue
        if (
            not values
            and "dtype" in line.lower()
            and re.search(
                r"(支持的数据类型|支持数据类型|可取的数据类型|可取值|取值如下|取值为|支持.*类型)",
                line,
            )
        ):
            parts = re.split(r"[:：为]", line, maxsplit=1)
            tail = parts[1] if len(parts) == 2 else ""
            dtype_values = [
                token
                for token in TYPE_TOKEN_RE.findall(tail)
                if token.lower() != "dtype"
            ]
            if dtype_values:
                values_by_name["dtype"] = tuple(dict.fromkeys(dtype_values))
            continue
        if not values:
            continue
        if explicit_placeholder_match and "取值为" in value_clause:
            placeholder_values = []
            if "为空时" in value_clause:
                placeholder_values.append("")
            placeholder_values.extend(
                match.group("value")
                for match in EXPLICIT_PLACEHOLDER_VALUE_RE.finditer(value_clause)
            )
            if placeholder_values:
                values_by_name[explicit_placeholder_match.group("name")] = tuple(
                    dict.fromkeys(placeholder_values)
                )
                continue
        if "dtype" in line.lower() and not any(
            value.strip("<>").lower().endswith("dtype") for value in values
        ):
            values_by_name["dtype"] = tuple(dict.fromkeys(values))
            continue
        for index, token in enumerate(values):
            name = token.strip("<>")
            if token == name or not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", name):
                continue
            placeholder_values = [
                value
                for value in values[index + 1 :]
                if value.strip("<>") != name
                and re.match(r"^[a-z][a-z0-9_]*$", value)
            ]
            if placeholder_values:
                values_by_name[name] = tuple(dict.fromkeys(placeholder_values))
                break
        if len(values) < 2:
            continue
        for index, token in enumerate(values):
            name = token.strip("<>")
            if not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", name):
                continue
            if "dtype" not in name.lower():
                continue
            dtype_values = [
                value
                for value in values[index + 1 :]
                if value.strip("<>") != name
            ]
            if dtype_values:
                values_by_name[name] = tuple(dict.fromkeys(dtype_values))
                break
    # A position placeholder can name a documented enum, rather than list its
    # values inline. Expand only the enum definition explicitly tied to it.
    for match in re.finditer(
        r"`<(?P<name>[A-Za-z_][A-Za-z0-9_]*)>`支持"
        r"`(?P<enum>[A-Za-z_][A-Za-z0-9_]*)`枚举类的[^\n：:]*[：:]\s*"
        r"```(?:c|cpp)?\s*enum\s+class\s+(?P=enum)\s*\{(?P<values>[^}]+)\}\s*;?\s*```",
        section_text,
    ):
        members = [value.strip() for value in match.group("values").split(",") if value.strip()]
        if members and all(re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*(?:\s*=\s*\d+)?", value) for value in members):
            values_by_name[match.group("name")] = tuple(
                f"{match.group('enum')}::{value.split('=')[0].strip()}" for value in members
            )
    return values_by_name


def _prototype_placeholder_values(
    prototype: str,
    section_text: str,
) -> dict[str, Sequence[str]]:
    """Keep explicitly separated return and output-reference dtype lists local."""
    values_by_name = _placeholder_values(section_text)
    before_parameters = prototype.split("(", 1)[0]
    kind = (
        "引用参数输出结果"
        if re.search(r"\bvoid\s+\w+\s*$", before_parameters)
        else "函数返回值返回结果"
    )
    scoped_values = {}
    for match in PROTOTYPE_DTYPE_SCOPE_RE.finditer(section_text):
        if match.group("kind") != kind:
            continue
        values = tuple(dict.fromkeys(BACKTICK_RE.findall(match.group("values"))))
        if values:
            scoped_values[match.group("placeholder")] = values
    return_style_kind = "无返回值类型" if kind == "引用参数输出结果" else "返回值类型"
    for match in RETURN_STYLE_DTYPE_SCOPE_RE.finditer(section_text):
        if match.group("kind") != return_style_kind:
            continue
        values = tuple(dict.fromkeys(BACKTICK_RE.findall(match.group("values"))))
        if values:
            scoped_values["dtype"] = values
    return {**values_by_name, **scoped_values}


def _conditional_parameter_type_overrides(
    section_text: str,
) -> tuple[tuple[str, str, tuple[str, ...], str], ...]:
    overrides = []
    seen = set()
    for match in CONDITIONAL_PARAMETER_TYPE_RE.finditer(section_text):
        parameter_text = match.group("parameters")
        parameter_names = BACKTICK_RE.findall(parameter_text)
        if not parameter_names:
            parameter_names = [
                token
                for token in TYPE_TOKEN_RE.findall(parameter_text)
                if token not in {"和", "与"}
            ]
        if not parameter_names:
            continue
        override = (
            match.group("placeholder"),
            match.group("value"),
            tuple(dict.fromkeys(parameter_names)),
            match.group("actual_type"),
        )
        if override not in seen:
            seen.add(override)
            overrides.append(override)
    return tuple(overrides)


def _conditional_template_type_overrides(
    section_text: str,
) -> tuple[tuple[str, str, str, str], ...]:
    overrides = [
        (
            match.group("placeholder"),
            match.group("value"),
            match.group("template"),
            match.group("actual_type"),
        )
        for match in CONDITIONAL_TEMPLATE_TYPE_RE.finditer(section_text)
    ]
    overrides.extend(
        (
            match.group("placeholder"),
            match.group("value"),
            f"vector_<{match.group('placeholder')}>",
            match.group("actual_type"),
        )
        for match in CONDITIONAL_REGISTER_DTYPE_TYPE_RE.finditer(section_text)
    )
    return tuple(dict.fromkeys(overrides))


def _parameter_name(parameter: str) -> Optional[str]:
    parameter = _remove_parameter_default(parameter)
    match = re.search(
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?:\[[^\]]*\])?\s*$",
        parameter,
    )
    return match.group("name") if match is not None else None


def _replace_parameter_base_type(
    prototype: str,
    parameter_names: Sequence[str],
    actual_type: str,
) -> str:
    opening_index = prototype.find("(")
    if opening_index < 0:
        return prototype
    parameters = _parenthesized_content(prototype, opening_index)
    if parameters is None:
        return prototype
    closing_index = opening_index + len(parameters) + 1
    target_names = set(parameter_names)
    replaced = []
    for parameter in _top_level_parameter_parts(parameters):
        name = _parameter_name(parameter)
        if name not in target_names:
            replaced.append(parameter)
            continue
        name_match = re.search(rf"\b{re.escape(name)}\b", parameter)
        if name_match is None:
            replaced.append(parameter)
            continue
        before_name = parameter[: name_match.start()]
        qualifiers_match = re.match(
            r"\s*(?P<qualifiers>(?:(?:const|volatile|restrict|__[A-Za-z0-9_]+__)\s+)*)",
            before_name,
        )
        qualifiers = qualifiers_match.group("qualifiers") if qualifiers_match else ""
        declarator_match = re.search(r"(?P<declarator>(?:\s*[*&]\s*)*)$", before_name)
        declarator = declarator_match.group("declarator") if declarator_match else " "
        suffix = parameter[name_match.end() :]
        replaced.append(f"{qualifiers}{actual_type}{declarator}{name}{suffix}")
    return (
        prototype[: opening_index + 1]
        + ", ".join(replaced)
        + prototype[closing_index:]
    )


def _apply_conditional_parameter_types(
    prototype: str,
    selected_values: dict[str, str],
    overrides: Sequence[tuple[str, str, tuple[str, ...], str]],
) -> str:
    result = prototype
    for placeholder, value, parameter_names, actual_type in overrides:
        if selected_values.get(placeholder) == value:
            result = _replace_parameter_base_type(
                result,
                parameter_names,
                actual_type,
            )
    return result


def _apply_conditional_template_types(
    prototype: str,
    selected_values: dict[str, str],
    overrides: Sequence[tuple[str, str, str, str]],
) -> str:
    result = prototype
    for placeholder, value, template, actual_type in overrides:
        if selected_values.get(placeholder) != value:
            continue
        expanded_template = template.replace(f"<{placeholder}>", value)
        result = result.replace(expanded_template, actual_type)
    return result


def _markdown_table_cells(line: str) -> tuple[str, ...]:
    stripped = line.strip()
    if not stripped.startswith("|"):
        return ()
    stripped = stripped.strip("|")
    cells = []
    for cell in stripped.split("|"):
        # Keep C++ placeholders and comparisons such as <dtype> and value < VL_T.
        value = re.sub(
            r"</?(?:br|p|div|span|a|b|strong|em|code|sub|sup)\b[^>]*>",
            " ", cell, flags=re.IGNORECASE,
        ).strip()
        value = value.strip("`").strip()
        cells.append(value)
    return tuple(cells)


def _is_markdown_table_separator(cells: Sequence[str]) -> bool:
    return bool(cells) and all(
        re.match(r"^:?-{3,}:?$", cell.replace(" ", "")) for cell in cells
    )


def _placeholder_table_variants(
    section_text: str,
    placeholder_names: Sequence[str],
    returns_value: bool = False,
) -> tuple[dict[str, str], ...]:
    if not placeholder_names:
        return ()
    lines = section_text.splitlines()
    for index in range(len(lines) - 1):
        headers = _markdown_table_cells(lines[index])
        separators = _markdown_table_cells(lines[index + 1])
        if (
            not headers
            or not separators
            or len(headers) != len(separators)
            or not _is_markdown_table_separator(separators)
        ):
            continue

        header_indexes = {
            re.sub(r"\s+", "", header): column_index
            for column_index, header in enumerate(headers)
        }
        placeholder_indexes = {}
        for name in placeholder_names:
            exact_index = header_indexes.get(name)
            if exact_index is not None:
                placeholder_indexes[name] = exact_index
                continue
            if not name.endswith("_dtype"):
                break
            prefix = name[: -len("_dtype")]
            type_column_indexes = tuple(
                column_index
                for column_name, column_index in header_indexes.items()
                if column_name in {f"{prefix}类型", f"{prefix}数据类型"}
            )
            if len(type_column_indexes) != 1:
                break
            placeholder_indexes[name] = type_column_indexes[0]
        if len(placeholder_indexes) != len(placeholder_names):
            continue
        return_value_index = header_indexes.get("函数返回值形式")

        variants = []
        seen = set()
        row_index = index + 2
        while row_index < len(lines):
            cells = _markdown_table_cells(lines[row_index])
            if not cells:
                break
            if len(cells) < len(headers):
                cells = (*cells, *([""] * (len(headers) - len(cells))))
            if (
                returns_value
                and return_value_index is not None
                and "不支持" in cells[return_value_index]
            ):
                row_index += 1
                continue
            variant = {}
            for name in placeholder_names:
                value = cells[placeholder_indexes[name]].strip()
                if not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", value):
                    variant = {}
                    break
                variant[name] = value
            if variant:
                key = tuple((name, variant[name]) for name in placeholder_names)
                if key not in seen:
                    seen.add(key)
                    variants.append(variant)
            row_index += 1
        if variants:
            return tuple(variants)
    return ()


def _placeholder_sentence_variants(
    section_text: str,
    placeholder_names: Sequence[str],
) -> tuple[dict[str, str], ...]:
    required_names = set(placeholder_names)
    if not required_names:
        return ()
    variants = []
    seen = set()
    for match in DEPENDENT_PLACEHOLDER_VARIANTS_RE.finditer(section_text):
        condition_name = match.group("condition_name")
        dependent_name = match.group("dependent_name")
        if {condition_name, dependent_name} != required_names:
            continue
        dependent_values = BACKTICK_RE.findall(match.group("dependent_values"))
        for dependent_value in dependent_values:
            variant = {
                condition_name: match.group("condition_value"),
                dependent_name: dependent_value,
            }
            key = tuple((name, variant[name]) for name in placeholder_names)
            if key in seen:
                continue
            seen.add(key)
            variants.append(variant)
    return tuple(variants)


def _correlated_placeholder_tuple_variants(
    section_text: str,
    placeholder_names: Sequence[str],
) -> tuple[dict[str, str], ...]:
    if len(placeholder_names) != 3:
        return ()
    required_names = set(placeholder_names)
    variants = []
    seen = set()
    for match in CORRELATED_PLACEHOLDER_TUPLES_RE.finditer(section_text):
        names = (match.group("first"), match.group("second"), match.group("third"))
        if set(names) != required_names:
            continue
        for tuple_text in BACKTICK_RE.findall(match.group("values")):
            values = tuple(value.strip() for value in tuple_text.split("/"))
            if len(values) != len(names) or not all(
                re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", value) for value in values
            ):
                continue
            variant = dict(zip(names, values))
            key = tuple((name, variant[name]) for name in placeholder_names)
            if key not in seen:
                seen.add(key)
                variants.append(variant)
    return tuple(variants)


def _placeholder_variants(
    section_text: str,
    placeholder_names: Sequence[str],
    returns_value: bool = False,
) -> tuple[dict[str, str], ...]:
    table_variants = _placeholder_table_variants(
        section_text, placeholder_names, returns_value
    )
    if table_variants:
        return table_variants
    tuple_variants = _correlated_placeholder_tuple_variants(
        section_text, placeholder_names
    )
    if tuple_variants:
        return tuple_variants
    return _placeholder_sentence_variants(section_text, placeholder_names)


def _balance_delta(text: str) -> int:
    return text.count("(") - text.count(")")


def _prototype_texts_from_code_blocks(section_text: str) -> Sequence[str]:
    prototypes = []
    for block in _prototype_code_blocks(section_text):
        current = []
        template_prefix = []
        depth = 0
        for raw_line in block.splitlines():
            line = raw_line.strip()
            if not line or line.startswith("//"):
                continue
            if not current:
                if line.startswith("template"):
                    template_prefix = [line]
                    continue
                if template_prefix and not PROTOTYPE_START_RE.search(line):
                    template_prefix.append(line)
                    continue
                if not PROTOTYPE_START_RE.search(line):
                    template_prefix = []
                    continue
                current.extend(template_prefix)
                template_prefix = []
            current.append(line)
            depth += _balance_delta(line)
            if current and depth <= 0 and ")" in line:
                prototype = re.sub(r"\s+", " ", " ".join(current)).strip().rstrip(";")
                prototypes.append(prototype)
                current = []
                template_prefix = []
                depth = 0
    return tuple(prototypes)


def _function_name_placeholder_match(
    prototype: str,
    placeholder_name: str,
) -> Optional[re.Match[str]]:
    return re.search(
        rf"(?<![A-Za-z0-9_<])(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)"
        rf"<{re.escape(placeholder_name)}>"
        rf"(?=(?:<[A-Za-z_][A-Za-z0-9_]*>)*\s*\()",
        prototype,
    )


def _function_name_expansion_candidates(symbol: str, value: str) -> tuple[str, ...]:
    stripped_value = value.lstrip("_")
    separated = symbol.rstrip("_")
    if stripped_value:
        separated += f"_{stripped_value}"
    return tuple(dict.fromkeys((symbol + value, separated)))


def _function_name_expansion_styles(
    prototype: str,
    placeholder_name: str,
    placeholder_values: Sequence[str],
    section_text: str,
) -> tuple[int, ...]:
    match = _function_name_placeholder_match(prototype, placeholder_name)
    if match is None:
        return (0,)

    symbol = match.group("symbol")
    candidates_by_value = tuple(
        _function_name_expansion_candidates(symbol, value)
        for value in placeholder_values
    )
    if all(len(candidates) == 1 for candidates in candidates_by_value):
        return (0,)

    concrete_symbols = set(
        re.findall(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*(?=\()", section_text)
    )
    scores = tuple(
        len(
            {
                candidates[style]
                for candidates in candidates_by_value
                if style < len(candidates) and candidates[style] in concrete_symbols
            }
        )
        for style in (0, 1)
    )
    if scores[0] != scores[1]:
        return (0,) if scores[0] > scores[1] else (1,)
    if scores[0]:
        raise ValueError(
            f"ambiguous function-name placeholder <{placeholder_name}> in {prototype}"
        )
    return (0, 1)


def _replace_placeholder(
    prototype: str,
    placeholder_name: str,
    value: str,
    function_name_styles: Sequence[int],
) -> tuple[str, ...]:
    function_match = _function_name_placeholder_match(prototype, placeholder_name)
    if function_match is None:
        candidates = (prototype,)
    else:
        symbols = _function_name_expansion_candidates(
            function_match.group("symbol"), value
        )
        candidates = tuple(
            prototype[: function_match.start("symbol")]
            + symbols[min(style, len(symbols) - 1)]
            + prototype[function_match.end() :]
            for style in function_name_styles
        )

    replacements = []
    for candidate in candidates:
        if value.startswith("vector_"):
            candidate = candidate.replace(f"vector_<{placeholder_name}>", value)
        candidate = candidate.replace(f"<{placeholder_name}>", value)
        replacements.append(
            re.sub(
                rf"(?P<prefix>(?:^|[,(]\s*)(?:const\s+)?)"
                rf"{re.escape(placeholder_name)}"
                rf"(?P<suffix>\s*[&*]?\s+[A-Za-z_][A-Za-z0-9_]*)",
                rf"\g<prefix>{value}\g<suffix>",
                candidate,
            )
        )
    return tuple(dict.fromkeys(replacements))


def _expand_placeholders(
    prototype: str,
    values_by_name: dict[str, Sequence[str]],
    section_text: str,
    function_name_evidence_text: Optional[str] = None,
) -> Sequence[tuple[str, dict[str, str]]]:
    explicit_placeholder_names = re.findall(r"<([A-Za-z_][A-Za-z0-9_]*)>", prototype)
    bare_placeholder_names = [
        name
        for name in values_by_name
        if re.search(rf"(?<![A-Za-z0-9_]){re.escape(name)}(?![A-Za-z0-9_])", prototype)
    ]
    placeholder_names = tuple(dict.fromkeys([*explicit_placeholder_names, *bare_placeholder_names]))
    if not placeholder_names:
        return ((prototype, {}),)

    conditional_type_overrides = _conditional_parameter_type_overrides(section_text)
    conditional_template_overrides = _conditional_template_type_overrides(section_text)
    placeholder_variants = _placeholder_variants(
        section_text,
        placeholder_names,
        returns_value=not re.search(
            r"\bvoid\s+\w+\s*$", prototype.split("(", 1)[0]
        ),
    )

    function_name_styles = {
        name: _function_name_expansion_styles(
            prototype,
            name,
            values_by_name.get(name)
            or tuple(
                dict.fromkeys(
                    variant[name]
                    for variant in placeholder_variants
                    if name in variant
                )
            ),
            function_name_evidence_text or section_text,
        )
        for name in placeholder_names
    }

    if placeholder_variants:
        expanded = []
        for variant in placeholder_variants:
            current_values = (prototype,)
            for name in placeholder_names:
                value = variant[name]
                current_values = tuple(
                    replacement
                    for current_value in current_values
                    for replacement in _replace_placeholder(
                        current_value,
                        name,
                        value,
                        function_name_styles[name],
                    )
                )
            expanded.extend(
                (
                    _apply_conditional_template_types(
                        _apply_conditional_parameter_types(
                            current_value,
                            variant,
                            conditional_type_overrides,
                        ),
                        variant,
                        conditional_template_overrides,
                    ),
                    dict(variant),
                )
                for current_value in current_values
            )
        return tuple(
            (
                re.sub(r"(__[A-Za-z0-9_]+__)(?=[A-Za-z_])", r"\1 ", value),
                dict(variant),
            )
            for value, variant in expanded
        )

    if any(name not in values_by_name for name in placeholder_names):
        return ((prototype, {}),)

    expanded = [(prototype, {})]
    for name in placeholder_names:
        next_expanded = []
        for current, selected_values in expanded:
            for value in values_by_name[name]:
                next_expanded.extend(
                    (replacement, {**selected_values, name: value})
                    for replacement in _replace_placeholder(
                        current,
                        name,
                        value,
                        function_name_styles[name],
                    )
                )
        expanded = next_expanded
    return tuple(
        (
            re.sub(
                r"(__[A-Za-z0-9_]+__)(?=[A-Za-z_])",
                r"\1 ",
                _apply_conditional_template_types(
                    _apply_conditional_parameter_types(
                        value,
                        selected_values,
                        conditional_type_overrides,
                    ),
                    selected_values,
                    conditional_template_overrides,
                ),
            ),
            dict(selected_values),
        )
        for value, selected_values in expanded
    )


def _parse_prototype_declaration(prototype: str) -> Optional[PrototypeDeclaration]:
    if "(" not in prototype or ")" not in prototype:
        return None
    function_matches = tuple(
        re.finditer(r"(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)\s*\(", prototype)
    )
    function_match = next(
        (
            match
            for match in function_matches
            if match.group("symbol") not in {"decltype", "operator"}
        ),
        None,
    )
    if function_match is None:
        return None
    before = prototype[: function_match.start("symbol")]
    rest = prototype[function_match.end() :]
    parameters = rest.rsplit(")", 1)[0]
    symbol = function_match.group("symbol")
    if symbol.startswith("~"):
        symbol = symbol[1:]
    if symbol.startswith("__") or symbol in IGNORED_PROTOTYPE_SYMBOLS:
        return None
    if not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", symbol):
        return None
    qualifier_match = re.search(r"\b(?P<qualifier>__[A-Za-z0-9_]+__)\b", before)
    return PrototypeDeclaration(
        symbol=symbol,
        qualifier=qualifier_match.group("qualifier") if qualifier_match else None,
        parameters=parameters,
    )


def _prototype_expansion_scopes(
    section_text: str,
) -> tuple[tuple[str, tuple[str, ...]], ...]:
    matches = tuple(NPU_SCOPED_SECTION_RE.finditer(section_text))
    architecture_scopes = []
    if not matches:
        architecture_scopes.append((section_text, ()))
    else:
        cursor = 0
        for match in matches:
            unscoped_text = section_text[cursor : match.start()]
            if _prototype_texts_from_code_blocks(unscoped_text):
                architecture_scopes.append((unscoped_text, ()))
            architecture_scopes.append(
                (match.group("body"), _npu_archs(match.group("npu_labels")))
            )
            cursor = match.end()
        unscoped_text = section_text[cursor:]
        if _prototype_texts_from_code_blocks(unscoped_text):
            architecture_scopes.append((unscoped_text, ()))

    scopes = []
    for architecture_scope, architecture_archs in architecture_scopes:
        boundaries = [
            match.start() for match in LEVEL_THREE_SECTION_RE.finditer(architecture_scope)
        ]
        boundaries = sorted({0, *boundaries, len(architecture_scope)})
        current_scope = ""
        for start, end in zip(boundaries, boundaries[1:]):
            section = architecture_scope[start:end]
            if _prototype_texts_from_code_blocks(section):
                if current_scope and _prototype_texts_from_code_blocks(current_scope):
                    scopes.append((current_scope, architecture_archs))
                current_scope = section
            elif current_scope:
                current_scope += section
        if current_scope and _prototype_texts_from_code_blocks(current_scope):
            scopes.append((current_scope, architecture_archs))
    return tuple(scopes)


def _prototype_template_type_parameters(prototype: str) -> tuple[str, ...]:
    """Extract documented C++ type template parameters from one prototype block."""
    names = []
    for template_match in re.finditer(r"\btemplate\s*<(?P<body>[^>]*)>", prototype):
        for parameter_match in re.finditer(
            r"\b(?:typename|class)\s*(?:\.\.\.\s*)?"
            r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\b",
            template_match.group("body"),
        ):
            name = parameter_match.group("name")
            if name not in names:
                names.append(name)
    return tuple(names)


def _code_block_prototype_declarations(
    section_text: str,
) -> Sequence[PrototypeDeclaration]:
    declarations_by_key: dict[
        tuple[str, Optional[str], tuple[str, ...]], PrototypeDeclaration
    ] = {}
    for expansion_scope, scope_archs in _prototype_expansion_scopes(section_text):
        for prototype in _prototype_texts_from_code_blocks(expansion_scope):
            template_parameters = _prototype_template_type_parameters(prototype)
            values_by_name = _prototype_placeholder_values(prototype, expansion_scope)
            function_name_expansion_source = ""
            function_name_match = None
            for placeholder_name in re.findall(
                r"<([A-Za-z_][A-Za-z0-9_]*)>", prototype
            ):
                function_name_match = _function_name_placeholder_match(
                    prototype, placeholder_name
                )
                if function_name_match:
                    function_name_expansion_source = prototype
                    break
            prototype_declaration = _parse_prototype_declaration(prototype)
            prototype_function = ""
            prototype_document_signature = ""
            if prototype_declaration is not None:
                prototype_function = _canonical_signature_id(prototype_declaration)
                prototype_function = re.sub(
                    r"(__[A-Za-z0-9_]+__)(?=<)",
                    r"\1 ",
                    prototype_function,
                )
                prototype_document_signature = _document_signature_id(prototype_declaration)
            for expanded, placeholder_bindings in _expand_placeholders(
                prototype,
                values_by_name,
                expansion_scope,
                section_text,
            ):
                declaration = _parse_prototype_declaration(expanded)
                if declaration is None:
                    continue
                local_prototype_function = prototype_function
                local_prototype_document_signature = prototype_document_signature
                if function_name_match is not None:
                    local_prototype = (
                        prototype[: function_name_match.start("symbol")]
                        + declaration.symbol
                        + prototype[function_name_match.end() :]
                    )
                    local_declaration = _parse_prototype_declaration(local_prototype)
                    if local_declaration is not None:
                        local_prototype_function = _canonical_signature_id(
                            local_declaration
                        )
                        local_prototype_function = re.sub(
                            r"(__[A-Za-z0-9_]+__)(?=<)",
                            r"\1 ",
                            local_prototype_function,
                        )
                        local_prototype_document_signature = _document_signature_id(
                            local_declaration
                        )
                declaration = replace(
                    declaration,
                    archs=scope_archs,
                    prototype_function=local_prototype_function,
                    prototype_document_signature=local_prototype_document_signature,
                    placeholder_bindings=tuple(placeholder_bindings.items()),
                    function_name_expansion_source=function_name_expansion_source,
                    template_parameters=template_parameters,
                )
                key = (
                    declaration.symbol,
                    declaration.qualifier,
                    tuple(
                        _normalized_parameter_list(
                            declaration.parameters or "",
                            keep_names=True,
                        )
                    ),
                )
                existing = declarations_by_key.get(key)
                if existing is not None:
                    combined_archs = tuple(
                        dict.fromkeys((*existing.archs, *declaration.archs))
                    )
                    if combined_archs != tuple(existing.archs):
                        declarations_by_key[key] = replace(
                            existing,
                            archs=combined_archs,
                        )
                    continue
                declarations_by_key[key] = declaration
    return tuple(declarations_by_key.values())


def extract_prototype_declarations(
    markdown_text: str,
) -> Sequence[PrototypeDeclaration]:
    """Extract unique API symbols and execution qualifiers from prototypes."""
    section_text = _prototype_section_text(markdown_text)
    if section_text is None:
        return []

    code_block_declarations = _code_block_prototype_declarations(section_text)
    if code_block_declarations:
        return code_block_declarations

    declarations = []
    symbols = []
    for symbol in API_SYMBOL_RE.findall(section_text):
        if symbol.startswith("__") or symbol in IGNORED_PROTOTYPE_SYMBOLS:
            continue
        if symbol.startswith("~"):
            symbol = symbol[1:]
        if symbol not in symbols:
            symbols.append(symbol)
    for symbol in PROTOTYPE_OPERATOR_RE.findall(section_text):
        symbol = re.sub(r"\s+", "", symbol)
        if symbol not in symbols:
            symbols.append(symbol)
    for symbol in PROTOTYPE_QUALIFIED_SYMBOL_RE.findall(section_text):
        if symbol.startswith("~"):
            symbol = symbol[1:]
        if symbol not in symbols:
            symbols.append(symbol)
    for symbol in PROTOTYPE_TEMPLATE_CALL_RE.findall(section_text):
        if symbol.startswith("~"):
            symbol = symbol[1:]
        if symbol not in symbols:
            symbols.append(symbol)
    for symbol in PROTOTYPE_USING_ALIAS_RE.findall(section_text):
        if symbol not in symbols:
            symbols.append(symbol)
    for symbol in PROTOTYPE_REGISTER_API_RE.findall(section_text):
        if symbol not in symbols:
            symbols.append(symbol)
    for symbol in PROTOTYPE_BARE_MACRO_RE.findall(section_text):
        if symbol not in symbols:
            symbols.append(symbol)
    type_declaration_text = re.sub(
        r"\btemplate\s*<[^>]*>", "", section_text, flags=re.DOTALL
    )
    for symbol in PROTOTYPE_TYPE_DECLARATION_RE.findall(type_declaration_text):
        if symbol not in symbols:
            symbols.append(symbol)

    for symbol in symbols:
        if symbol not in [item.symbol for item in declarations]:
            qualifier_match = re.search(
                rf"\b(?P<qualifier>__[A-Za-z0-9_]+__)\b(?:(?!;).)*?"
                rf"\b{re.escape(symbol)}\s*\(",
                section_text,
                re.DOTALL,
            )
            declarations.append(
                PrototypeDeclaration(
                    symbol,
                    qualifier_match.group("qualifier")
                    if qualifier_match is not None
                    else None,
                    _function_parameter_text(section_text, symbol),
                )
            )
    return declarations


def extract_prototype_symbols(markdown_text: str) -> Sequence[str]:
    """Extract unique API symbols from the ``函数原型`` section."""
    return [
        declaration.symbol
        for declaration in extract_prototype_declarations(markdown_text)
    ]


# =============================================================================
# 头文件查找：识别声明并读取候选头文件
# =============================================================================
# 先按函数、宏、类型别名或 REGISTER_*_API 判断符号是否存在；目录枚举和文件读取结果
# 使用缓存，供当前签名映射和旧版文档级映射共同复用。
def _header_declares(
    header_text: str, declaration: PrototypeDeclaration
) -> bool:
    macro_re = re.compile(
        rf"^\s*#\s*define\s+{re.escape(declaration.symbol)}\b",
        re.MULTILINE,
    )
    if macro_re.search(header_text) is not None:
        return True

    definition_re = re.compile(
        rf"\b(?:class|struct|union|using|enum\s+class|enum)\s+"
        rf"{re.escape(declaration.symbol)}\b"
    )
    if definition_re.search(header_text) is not None:
        return True

    typedef_re = re.compile(
        rf"\btypedef\b[^;]*\b{re.escape(declaration.symbol)}\s*;"
    )
    if typedef_re.search(header_text) is not None:
        return True

    constexpr_re = re.compile(
        rf"\b(?:inline\s+)?constexpr\b[^;=]*\b"
        rf"{re.escape(declaration.symbol)}\b\s*(?:=|\{{)"
    )
    if constexpr_re.search(header_text) is not None:
        return True

    registered_api_re = re.compile(
        rf"\bREGISTER_[A-Za-z0-9_]*API\s*\(\s*"
        rf"{re.escape(declaration.symbol)}\b"
    )
    if registered_api_re.search(header_text) is not None:
        return True

    if declaration.qualifier is None:
        return bool(
            re.search(rf"\b{re.escape(declaration.symbol)}\s*\(", header_text)
        )
    declaration_re = re.compile(
        rf"\b{re.escape(declaration.qualifier)}\b(?:(?!;).)*?"
        rf"\b{re.escape(declaration.symbol)}\s*\(",
        re.DOTALL,
    )
    return declaration_re.search(header_text) is not None


@lru_cache(maxsize=None)
def _header_paths(header_directory: str) -> tuple[Path, ...]:
    return tuple(sorted(Path(header_directory).rglob("*.h")))


@lru_cache(maxsize=HEADER_TEXT_CACHE_SIZE)
def _read_header_text(header_path: str) -> str:
    content = Path(header_path).read_bytes()
    try:
        return content.decode("utf-8")
    except UnicodeDecodeError:
        return content.decode("utf-8", errors="replace")


def _matching_headers(
    header_directory: Path, declarations: Sequence[PrototypeDeclaration]
) -> Sequence[Path]:
    matches = []
    for header_path in _header_paths(str(header_directory)):
        header_text = _read_header_text(str(header_path))
        if all(_header_declares(header_text, declaration) for declaration in declarations):
            matches.append(header_path)
    return matches


# =============================================================================
# 头文件查找：规范化并比较函数参数
# =============================================================================
# 参数拆分会跟踪圆括号、模板尖括号、数组和初始化列表的嵌套深度，避免把内部逗号误判
# 为参数边界；比较时分别保留或移除参数名，以区分完全匹配和仅参数名不同。
def _top_level_parameter_parts(parameters: str) -> Sequence[str]:
    parts = []
    part_start = 0
    depths = {"(": 0, "<": 0, "[": 0, "{": 0}
    closing_to_opening = {")": "(", ">": "<", "]": "[", "}": "{"}
    for index, character in enumerate(parameters):
        if character in depths:
            depths[character] += 1
        elif character in closing_to_opening:
            opening = closing_to_opening[character]
            if depths[opening] > 0:
                depths[opening] -= 1
        elif character == "," and not any(depths.values()):
            parts.append(parameters[part_start:index])
            part_start = index + 1
    parts.append(parameters[part_start:])
    return tuple(part.strip() for part in parts if part.strip())


def _remove_parameter_default(parameter: str) -> str:
    depths = {"(": 0, "<": 0, "[": 0, "{": 0}
    closing_to_opening = {")": "(", ">": "<", "]": "[", "}": "{"}
    for index, character in enumerate(parameter):
        if character in depths:
            depths[character] += 1
        elif character in closing_to_opening:
            opening = closing_to_opening[character]
            if depths[opening] > 0:
                depths[opening] -= 1
        elif character == "=" and not any(depths.values()):
            return parameter[:index].strip()
    return parameter.strip()


def _normalize_parameter_spacing(parameter: str) -> str:
    parameter = re.sub(r"/\*.*?\*/", "", parameter, flags=re.DOTALL)
    parameter = re.sub(r"\s+", " ", parameter).strip()
    return re.sub(r"\s*([<>,*&()[\]])\s*", r"\1", parameter)


def _parameter_without_name(parameter: str) -> str:
    array_parameter_match = re.search(
        r"^(?P<type>.+?)(?:\s+|(?<=[*&]))(?P<name>[A-Za-z_][A-Za-z0-9_]*)"
        r"(?P<array>(?:\[[^\]]*\])+)$",
        parameter,
    )
    if array_parameter_match is not None:
        return (
            array_parameter_match.group("type").rstrip()
            + array_parameter_match.group("array")
        )
    parameter_name_match = re.search(
        r"^(?P<type>.+\s)(?P<name>[A-Za-z_][A-Za-z0-9_]*)$",
        parameter,
    )
    if parameter_name_match is None:
        return parameter
    name = parameter_name_match.group("name")
    if name[0].isupper() or name in {
        "bool",
        "char",
        "double",
        "float",
        "int",
        "long",
        "short",
        "signed",
        "unsigned",
        "void",
    }:
        return parameter
    return parameter_name_match.group("type").rstrip()


def _without_top_level_cv_qualifiers(parameter_type: str) -> str:
    if "&" in parameter_type or "[" in parameter_type:
        return parameter_type
    if "*" in parameter_type:
        last_pointer = parameter_type.rfind("*")
        prefix = parameter_type[: last_pointer + 1]
        suffix = re.sub(
            r"^(?:(?:const|volatile)\b\s*)+",
            "",
            parameter_type[last_pointer + 1 :],
        )
        return f"{prefix}{suffix}"
    parameter_type = re.sub(
        r"^(?:(?:const|volatile)\b\s*)+", "", parameter_type
    )
    return re.sub(
        r"\s*\b(?:(?:const|volatile)\b\s*)+$", "", parameter_type
    )


def _normalized_parameter_list(
    parameters: str, keep_names: bool
) -> tuple[str, ...]:
    normalized_parts = []
    for part in _top_level_parameter_parts(parameters):
        part = _remove_parameter_default(part)
        if not keep_names:
            part = _parameter_without_name(part)
        normalized_part = _normalize_parameter_spacing(part)
        normalized_part = _without_top_level_cv_qualifiers(normalized_part)
        if normalized_part and normalized_part != "void":
            normalized_parts.append(normalized_part)
    return tuple(normalized_parts)


def _array_parameter_element_type(parameter: str) -> Optional[str]:
    parameter = _normalize_parameter_spacing(_remove_parameter_default(parameter))
    if not re.search(r"\[[^\]]*\]$", parameter):
        return None
    without_bound = re.sub(r"\[[^\]]*\]$", "", parameter)
    return _parameter_without_name(without_bound).strip()


def _array_parameters_match(
    document_parameters: tuple[str, ...],
    header_parameters: tuple[str, ...],
) -> bool:
    if len(document_parameters) != len(header_parameters):
        return False
    if not document_parameters:
        return False
    for document_parameter, header_parameter in zip(
        document_parameters, header_parameters
    ):
        document_element_type = _array_parameter_element_type(document_parameter)
        header_element_type = _array_parameter_element_type(header_parameter)
        if document_element_type is None or header_element_type is None:
            return False
        if document_element_type != header_element_type:
            return False
    return True


def _canonical_signature_id(declaration: PrototypeDeclaration) -> str:
    if declaration.parameters is None:
        return f"{declaration.symbol}()"
    parameter_types = _normalized_parameter_list(
        declaration.parameters, keep_names=False
    )
    return f"{declaration.symbol}({','.join(parameter_types)})"


def _document_signature_id(declaration: PrototypeDeclaration) -> str:
    if declaration.parameters is None:
        return f"{declaration.symbol}()"
    parameters = _normalized_parameter_list(
        declaration.parameters,
        keep_names=True,
    )
    return f"{declaration.symbol}({','.join(parameters)})"


def _signature_override_id(
    canonical_id: str,
    template_parameters: Sequence[str] = (),
) -> str:
    """Return an override id only for a concrete declaration.

    Parameter templates are valid API prototypes.  They publish through a
    ``prototype_id`` and therefore do not have an ``override_id`` until the
    document evidence expands every placeholder.
    """
    field, value = signature_identifier(
        canonical_id,
        template_parameters=template_parameters,
    )
    return value if field == "override_id" else ""


# =============================================================================
# 头文件查找：解析宏声明并选择最佳签名比较结果
# =============================================================================
# 除普通函数声明外，本阶段还跟踪函数宏、对象宏别名及其目标符号。一个头文件中存在
# 多个同名候选时，按 exact、数组/宏兼容、参数名差异等优先级选择最可信结果。
def _logical_preprocessor_lines(header_text: str) -> Sequence[str]:
    logical_lines = []
    current_line = ""
    for raw_line in header_text.splitlines():
        stripped = raw_line.rstrip()
        continued = stripped.endswith("\\")
        segment = stripped[:-1] if continued else stripped
        if current_line:
            current_line = f"{current_line} {segment.strip()}"
        else:
            current_line = segment.strip()
        if continued:
            continue
        if current_line:
            logical_lines.append(current_line)
        current_line = ""
    if current_line:
        logical_lines.append(current_line)
    return tuple(logical_lines)


def _macro_definitions(header_text: str) -> Sequence[MacroDefinition]:
    definitions = []
    function_macro_re = re.compile(
        r"^\s*#\s*define\s+"
        r"(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)"
        r"\((?P<parameters>[^)]*)\)\s*(?P<body>.*)$"
    )
    object_macro_re = re.compile(
        r"^\s*#\s*define\s+"
        r"(?P<symbol>[A-Za-z_][A-Za-z0-9_]*)\b\s*(?P<body>.*)$"
    )
    for line in _logical_preprocessor_lines(header_text):
        function_match = function_macro_re.match(line)
        if function_match is not None:
            definitions.append(
                MacroDefinition(
                    function_match.group("symbol"),
                    function_match.group("parameters").strip(),
                    function_match.group("body").strip(),
                )
            )
            continue

        object_match = object_macro_re.match(line)
        if object_match is not None:
            definitions.append(
                MacroDefinition(
                    object_match.group("symbol"),
                    None,
                    object_match.group("body").strip(),
                )
            )
    return tuple(definitions)


def _macro_definitions_for_symbol(
    header_text: str, symbol: str
) -> Sequence[MacroDefinition]:
    return tuple(
        macro for macro in _macro_definitions(header_text) if macro.symbol == symbol
    )


def _macro_parameter_names(parameters: str) -> tuple[str, ...]:
    names = []
    for parameter in _top_level_parameter_parts(parameters):
        normalized = parameter.strip().strip("()")
        normalized = normalized.replace("##", "").strip()
        if normalized in {"...", "__VA_ARGS__"}:
            continue
        if normalized.endswith("..."):
            normalized = normalized[:-3].strip()
        if normalized and re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", normalized):
            names.append(normalized)
    return tuple(names)


def _macro_has_variadic_parameters(parameters: str) -> bool:
    return any(
        parameter.strip().strip("()").replace("##", "").strip()
        in {"...", "__VA_ARGS__"}
        or parameter.strip().endswith("...")
        for parameter in _top_level_parameter_parts(parameters)
    )


def _matching_function_macro(
    declaration: PrototypeDeclaration, header_text: str
) -> Optional[MacroDefinition]:
    """Return a function macro whose formal names exactly match the document."""
    document_names = _macro_document_parameter_names(declaration.parameters or "")
    if not document_names:
        return None
    for macro in _macro_definitions_for_symbol(header_text, declaration.symbol):
        if (
            macro.parameters is not None
            and not _macro_has_variadic_parameters(macro.parameters)
            and _macro_parameter_names(macro.parameters) == document_names
        ):
            return macro
    return None


def _macro_id(declaration: PrototypeDeclaration) -> str:
    """Encode a function macro from its spelling, never from parameter types."""
    document_names = _macro_document_parameter_names(declaration.parameters or "")
    if not document_names:
        return ""
    return "__".join((declaration.symbol, *document_names))


def _mapping_has_function_macro(
    mapping: SignatureHeaderMapping,
    declaration: PrototypeDeclaration,
    repo_root: Path,
) -> bool:
    for resource in mapping.headers:
        path = repo_root / resource.lstrip("/")
        if path.is_file() and _matching_function_macro(
            declaration, _read_header_text(str(path))
        ) is not None:
            return True
    return False


def _has_compiler_generated_function_macro(
    declaration: PrototypeDeclaration, repo_root: Path
) -> bool:
    for source in COMPILER_GENERATED_MACRO_SOURCES:
        path = repo_root / source
        if path.is_file() and _matching_function_macro(
            declaration, _read_header_text(str(path))
        ) is not None:
            return True
    return False


def _assign_signature_identifiers(
    mappings: Sequence[SignatureHeaderMapping],
    declarations_by_id: Mapping[str, PrototypeDeclaration],
    repo_root: Path,
) -> tuple[SignatureHeaderMapping, ...]:
    """Choose a type-based ID for functions and a name-based ID for macros."""
    identified = []
    for mapping in mappings:
        declaration = declarations_by_id.get(
            mapping.document_signature or mapping.canonical_id
        )
        if declaration is None:
            identified.append(mapping)
            continue
        macro_id = ""
        generated_macro = False
        if _mapping_has_function_macro(mapping, declaration, repo_root):
            macro_id = _macro_id(declaration)
        elif not mapping.headers and _has_compiler_generated_function_macro(
            declaration, repo_root
        ):
            macro_id = _macro_id(declaration)
            generated_macro = bool(macro_id)
        if macro_id:
            identified.append(
                replace(
                    mapping,
                    override_id="",
                    macro_id=macro_id,
                    status=(
                        "compiler_generated_macro"
                        if generated_macro
                        else mapping.status
                    ),
                    source=(
                        "compiler_generated_macro"
                        if generated_macro
                        else mapping.source
                    ),
                    suspicious_candidates=(
                        () if generated_macro else mapping.suspicious_candidates
                    ),
                )
            )
            continue
        identified.append(
            replace(
                mapping,
                override_id=_signature_override_id(
                    mapping.canonical_id, declaration.template_parameters
                ),
            )
        )
    return tuple(identified)


def _document_parameter_names(parameters: str) -> tuple[Optional[str], ...]:
    names = []
    for parameter in _top_level_parameter_parts(parameters):
        parameter = _remove_parameter_default(parameter).strip()
        match = re.search(
            r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?:\[[^\]]*\])?$",
            parameter,
        )
        if match is None:
            names.append(None)
            continue
        name = match.group("name")
        prefix = parameter[: match.start("name")].strip()
        if not prefix or name in TYPE_KEYWORDS or name[0].isupper():
            names.append(None)
        else:
            names.append(name)
    return tuple(names)


def _macro_document_parameter_names(parameters: str) -> tuple[str, ...]:
    """Return formal names from a type-less documented function macro."""
    names = tuple(
        parameter.strip()
        for parameter in _top_level_parameter_parts(parameters)
    )
    if not names or any(
        re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name) is None for name in names
    ):
        return ()
    return names


def _macro_target_symbols(macro: MacroDefinition) -> Sequence[str]:
    ignored = set(_macro_parameter_names(macro.parameters or ""))
    ignored.add(macro.symbol)
    targets = []
    for target in re.findall(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\(", macro.body):
        if target in ignored or target in IGNORED_MACRO_TARGET_SYMBOLS:
            continue
        if target not in targets:
            targets.append(target)

    if macro.parameters is None and not targets:
        for target in re.findall(r"\b([A-Za-z_][A-Za-z0-9_]*)\b", macro.body):
            if target in ignored or target in IGNORED_MACRO_TARGET_SYMBOLS:
                continue
            targets.append(target)
            break
    return tuple(targets)


def _object_macro_alias_comparison(
    declaration: PrototypeDeclaration,
    document_raw_parameters: tuple[str, ...],
    header_text: str,
    resource: str,
) -> Optional[SignatureComparison]:
    for macro in _macro_definitions_for_symbol(header_text, declaration.symbol):
        if macro.parameters is not None:
            continue
        target_symbols = _macro_target_symbols(macro)
        if not target_symbols:
            continue
        return SignatureComparison(
            resource=resource,
            symbol=declaration.symbol,
            status="object_macro_alias_match",
            document_parameters=", ".join(document_raw_parameters),
            header_parameters=("...",),
            header_parameter_source="object_macro_alias",
            header_symbol=target_symbols[0],
            macro_chain=(macro.symbol,),
        )
    return None


def _header_function_parameter_lists(
    header_text: str, symbol: str
) -> Sequence[str]:
    parameter_lists = []
    for symbol_match in re.finditer(rf"\b{re.escape(symbol)}\s*\(", header_text):
        line_start = header_text.rfind("\n", 0, symbol_match.start()) + 1
        if re.match(r"\s*#\s*define\b", header_text[line_start:symbol_match.start()]):
            continue

        opening_parenthesis_index = header_text.find("(", symbol_match.start())
        parameters = _parenthesized_content(
            header_text, opening_parenthesis_index
        )
        if parameters is None:
            continue
        parameter_lists.append(parameters)
    return tuple(_unique(parameter_lists))


def _header_parameter_candidates(
    header_text: str, symbol: str, macro_chain: Sequence[str] = ()
) -> Sequence[HeaderParameterCandidate]:
    if symbol in macro_chain:
        return ()

    candidates = [
        HeaderParameterCandidate(parameters, "declaration", symbol, macro_chain)
        for parameters in _header_function_parameter_lists(header_text, symbol)
    ]
    next_chain = (*macro_chain, symbol)
    for macro in _macro_definitions_for_symbol(header_text, symbol):
        if macro.parameters is not None:
            candidates.append(
                HeaderParameterCandidate(
                    macro.parameters,
                    "function_macro",
                    macro.symbol,
                    next_chain,
                )
            )
            continue

        for target_symbol in _macro_target_symbols(macro):
            candidates.extend(
                _header_parameter_candidates(header_text, target_symbol, next_chain)
            )
    return tuple(candidates)


def _compare_declaration_candidate(
    declaration: PrototypeDeclaration,
    document_raw_parameters: tuple[str, ...],
    candidate: HeaderParameterCandidate,
    resource: str,
) -> SignatureComparison:
    header_parameters = (candidate.parameters,)
    document_raw_parameters = _normalized_parameter_list(
        declaration.parameters, keep_names=True
    )
    normalized_header_parameters = [
        _normalized_parameter_list(parameters, keep_names=True)
        for parameters in header_parameters
    ]
    rendered_header_parameters = tuple(
        ", ".join(parameters) for parameters in normalized_header_parameters
    )

    if document_raw_parameters in normalized_header_parameters:
        status = "exact"
    else:
        document_types = _normalized_parameter_list(
            declaration.parameters, keep_names=False
        )
        normalized_header_types = [
            _normalized_parameter_list(parameters, keep_names=False)
            for parameters in header_parameters
        ]
        if document_types in normalized_header_types:
            status = "parameter_name_difference"
        elif any(
            _array_parameters_match(document_types, parameters)
            for parameters in normalized_header_types
        ):
            status = "array_parameter_match"
        elif any(
            len(document_types) == len(parameters)
            for parameters in normalized_header_types
        ):
            status = "parameter_type_mismatch"
        else:
            status = "parameter_count_mismatch"
    return SignatureComparison(
        resource=resource,
        symbol=declaration.symbol,
        status=status,
        document_parameters=", ".join(document_raw_parameters),
        header_parameters=rendered_header_parameters,
        header_parameter_source=candidate.source_kind,
        header_symbol=candidate.source_symbol,
        macro_chain=candidate.macro_chain,
    )


def _compare_macro_candidate(
    declaration: PrototypeDeclaration,
    document_raw_parameters: tuple[str, ...],
    candidate: HeaderParameterCandidate,
    resource: str,
) -> SignatureComparison:
    macro_parameter_names = _macro_parameter_names(candidate.parameters)
    document_parameter_names = _document_parameter_names(declaration.parameters or "")
    comparable_document_names = tuple(
        name for name in document_parameter_names if name is not None
    )
    is_variadic = _macro_has_variadic_parameters(candidate.parameters)
    fixed_document_names = document_parameter_names[: len(macro_parameter_names)]

    if (
        macro_parameter_names
        and len(comparable_document_names) == len(document_parameter_names)
        and (
            macro_parameter_names == comparable_document_names
            or (
                is_variadic
                and len(document_parameter_names) >= len(macro_parameter_names)
                and macro_parameter_names == fixed_document_names
            )
        )
    ):
        status = "macro_parameter_names_match"
    elif len(macro_parameter_names) == len(document_raw_parameters):
        status = "parameter_name_difference"
    else:
        status = "parameter_count_mismatch"

    return SignatureComparison(
        resource=resource,
        symbol=declaration.symbol,
        status=status,
        document_parameters=", ".join(document_raw_parameters),
        header_parameters=(", ".join(_macro_parameter_names(candidate.parameters)),),
        header_parameter_source=candidate.source_kind,
        header_symbol=candidate.source_symbol,
        macro_chain=candidate.macro_chain,
    )


def _comparison_rank(comparison: SignatureComparison) -> int:
    return {
        "exact": 0,
        "array_parameter_match": 1,
        "macro_parameter_names_match": 1,
        "object_macro_alias_match": 1,
        "parameter_name_difference": 2,
        "parameter_type_mismatch": 3,
        "parameter_count_mismatch": 4,
        "api_not_found_in_header": 5,
        "not_comparable": 6,
    }.get(comparison.status, 6)


def _compare_function_parameters(
    declaration: PrototypeDeclaration, header_text: str, resource: str
) -> Optional[SignatureComparison]:
    if declaration.parameters is None:
        return None

    document_raw_parameters = _normalized_parameter_list(
        declaration.parameters, keep_names=True
    )
    if not _header_declares(header_text, declaration):
        return SignatureComparison(
            resource=resource,
            symbol=declaration.symbol,
            status="api_not_found_in_header",
            document_parameters=", ".join(document_raw_parameters),
        )
    candidates = _header_parameter_candidates(header_text, declaration.symbol)
    if not candidates:
        alias_comparison = _object_macro_alias_comparison(
            declaration,
            document_raw_parameters,
            header_text,
            resource,
        )
        if alias_comparison is not None:
            return alias_comparison
        return SignatureComparison(
            resource=resource,
            symbol=declaration.symbol,
            status="not_comparable",
            document_parameters=", ".join(document_raw_parameters),
        )

    comparisons = []
    for candidate in candidates:
        if candidate.source_kind == "function_macro":
            comparisons.append(
                _compare_macro_candidate(
                    declaration,
                    document_raw_parameters,
                    candidate,
                    resource,
                )
            )
        else:
            comparisons.append(
                _compare_declaration_candidate(
                    declaration,
                    document_raw_parameters,
                    candidate,
                    resource,
                )
            )
    return sorted(comparisons, key=_comparison_rank)[0]


def _same_parameter_count_same_name_comparisons(
    declaration: PrototypeDeclaration,
    header_text: str,
    resource: str,
) -> tuple[SignatureComparison, ...]:
    if declaration.parameters is None or not _header_declares(header_text, declaration):
        return ()

    document_raw_parameters = _normalized_parameter_list(
        declaration.parameters, keep_names=True
    )
    comparisons = []
    for candidate in _header_parameter_candidates(header_text, declaration.symbol):
        if candidate.source_kind == "function_macro":
            comparison = _compare_macro_candidate(
                declaration,
                document_raw_parameters,
                candidate,
                resource,
            )
        else:
            comparison = _compare_declaration_candidate(
                declaration,
                document_raw_parameters,
                candidate,
                resource,
            )
        if comparison.status == "parameter_type_mismatch":
            comparisons.append(comparison)
    return tuple(comparisons)


def _compare_header_signatures(
    markdown_text: str, resource: Optional[str], repo_root: Path
) -> Sequence[SignatureComparison]:
    if resource is None:
        return ()
    header_path = repo_root / resource.lstrip("/")
    if not header_path.is_file():
        return ()
    header_text = _read_header_text(str(header_path))
    comparisons = []
    for declaration in extract_prototype_declarations(markdown_text):
        comparison = _compare_function_parameters(
            declaration, header_text, resource
        )
        if comparison is not None:
            comparisons.append(comparison)
    return tuple(comparisons)


def _unique(items: Sequence[str]) -> tuple[str, ...]:
    unique_items = []
    for item in items:
        if item not in unique_items:
            unique_items.append(item)
    return tuple(unique_items)


def _unique_legacy_include_mappings(
    mappings: Sequence[LegacyIncludeMapping],
) -> tuple[LegacyIncludeMapping, ...]:
    unique_mappings = []
    for mapping in mappings:
        if mapping not in unique_mappings:
            unique_mappings.append(mapping)
    return tuple(unique_mappings)


# =============================================================================
# 头文件查找：解析文档提示和兼容路径
# =============================================================================
# 将“头文件路径”说明、Markdown 中的 #include 和同目录 *_usage.md 中的 #include
# 统一转换为仓库内 /include/... 资源路径；必要时处理历史前缀和文件名后缀兜底。
@lru_cache(maxsize=None)
def _header_suffix_matches(repo_root: str, suffix: str) -> tuple[Path, ...]:
    include_root = Path(repo_root) / "include"
    if not include_root.is_dir():
        return ()
    return tuple(
        path
        for path in sorted(include_root.rglob(Path(suffix).name))
        if path.is_file()
    )


def _header_resource(full_path: Path, repo_root: Path) -> Optional[str]:
    if not full_path.is_file():
        return None
    try:
        normalized_root = repo_root.resolve()
        normalized_path = full_path.resolve()
        return f"/{normalized_path.relative_to(normalized_root).as_posix()}"
    except ValueError:
        return None


def _header_path_candidates(header_path: str) -> Sequence[tuple[str, str]]:
    normalized_path = header_path.strip().strip("`\"'<>").lstrip("/")
    candidates = [("as_written", normalized_path)]
    if not normalized_path.startswith("include/"):
        candidates.append(("include_prefix", f"include/{normalized_path}"))

    for legacy_prefix in ("lib/", "tiling/"):
        if normalized_path.startswith(legacy_prefix):
            stripped_path = normalized_path[len(legacy_prefix):]
            candidates.append(
                ("legacy_prefix_remap", f"include/adv_api/{stripped_path}")
            )

    unique_candidates = []
    for candidate in candidates:
        if candidate[1] not in [item[1] for item in unique_candidates]:
            unique_candidates.append(candidate)
    return tuple(unique_candidates)


def _candidate_header_paths(header_path: str) -> Sequence[str]:
    return tuple(candidate for _, candidate in _header_path_candidates(header_path))


def _header_path_resolution_details(
    header_path: str, repo_root: Path
) -> tuple[Sequence[str], Sequence[tuple[str, str]]]:
    resources = []
    resolution_rules = []
    for rule, candidate in _header_path_candidates(header_path):
        resource = _header_resource(repo_root / candidate, repo_root)
        if resource is not None:
            resources.append(resource)
            resolution_rules.append((rule, resource))

    if not resources:
        for suffix_match in _header_suffix_matches(str(repo_root), header_path):
            resource = _header_resource(suffix_match, repo_root)
            if resource is not None:
                resources.append(resource)
                resolution_rules.append(("filename_suffix_fallback", resource))
    return _unique(resources), tuple(resolution_rules)


def _header_resources_from_document_path(
    header_path: str, repo_root: Path
) -> Sequence[str]:
    return _header_path_resolution_details(header_path, repo_root)[0]


def _is_api_header_resource(resource: str) -> bool:
    return resource == "/include/kernel_operator.h" or resource.startswith(
        API_HEADER_RESOURCE_PREFIXES
    )


def extract_document_header_resources(
    markdown_text: str, repo_root: Path = REPO_ROOT
) -> Sequence[str]:
    resources = []
    for line in markdown_text.splitlines():
        line_match = DOCUMENT_HEADER_LINE_RE.search(line)
        if line_match is None:
            continue
        for path_match in DOCUMENT_HEADER_PATH_RE.finditer(line_match.group(1)):
            resources.extend(
                _header_resources_from_document_path(path_match.group(1), repo_root)
            )
    return _unique(resources)


def extract_document_include_header_resources(
    markdown_text: str, repo_root: Path = REPO_ROOT
) -> Sequence[str]:
    return _document_include_header_details(markdown_text, repo_root)[0]


def _document_include_header_details(
    markdown_text: str,
    repo_root: Path,
    source_kind: str = "document_include",
    source_document_path: Optional[Path] = None,
) -> tuple[Sequence[str], Sequence[LegacyIncludeMapping]]:
    resources = []
    legacy_mappings = []
    for include_match in DOCUMENT_INCLUDE_RE.finditer(markdown_text):
        source_path = include_match.group(1)
        include_resources, resolution_rules = _header_path_resolution_details(
            source_path, repo_root
        )
        for resource in include_resources:
            if _is_api_header_resource(resource):
                resources.append(resource)
        for rule, resource in resolution_rules:
            if rule == "legacy_prefix_remap" and _is_api_header_resource(resource):
                legacy_mappings.append(
                    LegacyIncludeMapping(
                        source_kind,
                        source_document_path,
                        source_path,
                        resource,
                    )
                )
    return _unique(resources), tuple(legacy_mappings)


def _related_usage_header_resources(
    document_path: Path, repo_root: Path
) -> Sequence[str]:
    return _related_usage_header_details(document_path, repo_root)[0]


def _related_usage_header_details(
    document_path: Path, repo_root: Path
) -> tuple[Sequence[str], Sequence[LegacyIncludeMapping]]:
    resources = []
    legacy_mappings = []
    for usage_path in sorted(document_path.parent.glob("*_usage.md")):
        if usage_path == document_path or not usage_path.is_file():
            continue
        usage_resources, usage_mappings = _document_include_header_details(
            usage_path.read_text(encoding="utf-8"),
            repo_root,
            source_kind="related_usage_include",
            source_document_path=usage_path,
        )
        resources.extend(usage_resources)
        legacy_mappings.extend(usage_mappings)
    return _unique(resources), tuple(legacy_mappings)


# =============================================================================
# 头文件查找：确定 API 家族和候选搜索目录
# =============================================================================
# API 家族取 docs/zh/api/SIMD-API 下的第一级目录，并映射到 include/<family>；adv_api
# 额外允许 basic_api 作为回退。目录名称相似度只用于旧版文档级候选消歧。
def _normalized_directory_name(path: Path) -> str:
    return re.sub(r"[^a-z0-9]", "", path.name.lower())


def _normalized_api_tokens(path: Path) -> set[str]:
    tokens = set()
    raw_parts = [path.stem, *(parent.name for parent in path.parents if parent != Path("."))]
    for raw_part in raw_parts:
        token = _normalized_directory_name(Path(raw_part))
        if not token:
            continue
        tokens.add(token)
        for suffix in ("interface", "kernel", "tiling", "constructor", "compute", "operations", "functions"):
            if token.endswith(suffix):
                tokens.add(token[: -len(suffix)])
        tokens.add(token.replace("backprop", "bp"))
    return {token for token in tokens if token}


def _header_category_score(
    document_path: Path, header_path: Path, repo_root: Path
) -> int:
    simd_api_root = repo_root / "docs" / "zh" / "api" / "SIMD-API"
    try:
        document_relative = document_path.relative_to(simd_api_root)
    except ValueError:
        document_relative = Path(document_path.name)
    document_tokens = _normalized_api_tokens(document_relative)
    header_tokens = _normalized_api_tokens(header_path.relative_to(repo_root))

    score = 0
    for document_token in document_tokens:
        for header_token in header_tokens:
            if document_token == header_token:
                score += 30
            elif len(document_token) >= 4 and document_token in header_token:
                score += 10
            elif len(header_token) >= 4 and header_token in document_token:
                score += 6

    header_stem = _normalized_directory_name(header_path)
    document_stem = _normalized_directory_name(document_path)
    if header_stem == document_stem:
        score += 80
    elif len(document_stem) >= 4 and document_stem in header_stem:
        score += 25

    if any(part in header_stem for part in ("base", "common", "utils", "config", "client")):
        score -= 8
    if header_stem.endswith("intf"):
        score += 4
    return score


def _resolve_by_document_category(
    document_path: Path, matches: Sequence[Path], repo_root: Path
) -> Optional[Path]:
    """Use normalized document-category similarity to break header ties."""
    scored_matches = [
        (_header_category_score(document_path, header, repo_root), header)
        for header in matches
    ]
    if not scored_matches:
        return None

    best_score = max(score for score, _ in scored_matches)
    if best_score <= 0:
        return None
    best_matches = [
        header for score, header in scored_matches if score == best_score
    ]
    return best_matches[0] if len(best_matches) == 1 else None


def _document_api_family(document_path: Path, repo_root: Path) -> Optional[str]:
    simd_api_root = repo_root / "docs" / "zh" / "api" / "SIMD-API"
    try:
        relative_path = document_path.relative_to(simd_api_root)
    except ValueError:
        return None
    if not relative_path.parts:
        return None
    return relative_path.parts[0]


def _header_search_directories(document_path: Path, repo_root: Path) -> Sequence[Path]:
    family = _document_api_family(document_path, repo_root)
    if family in {"adv_api", "basic_api", "c_api", "tensor_api"}:
        header_directory = repo_root / "include" / family
    else:
        header_directory = repo_root / "include" / "c_api"
    return (header_directory,) if header_directory.is_dir() else ()


def _fallback_header_search_directories(
    document_path: Path, repo_root: Path
) -> Sequence[Path]:
    family = _document_api_family(document_path, repo_root)
    fallback_directories = []
    if family == "adv_api":
        fallback_directories.append(repo_root / "include" / "basic_api")
    return tuple(path for path in fallback_directories if path.is_dir())


@lru_cache(maxsize=None)
def _all_include_header_resources(repo_root: str) -> tuple[str, ...]:
    include_root = Path(repo_root) / "include"
    if not include_root.is_dir():
        return ()
    resources = []
    for header_path in sorted(include_root.rglob("*.h")):
        resource = _header_resource(header_path, Path(repo_root))
        if resource is not None:
            resources.append(resource)
    return tuple(resources)


def _search_directory_header_resources(
    document_path: Path, repo_root: Path
) -> tuple[str, ...]:
    resources = []
    for header_directory in (
        *_header_search_directories(document_path, repo_root),
        *_fallback_header_search_directories(document_path, repo_root),
    ):
        for header_path in _header_paths(str(header_directory)):
            resource = _header_resource(header_path, repo_root)
            if resource is not None:
                resources.append(resource)
    return _unique(resources)


# =============================================================================
# 头文件查找：在候选头文件中验证符号和完整签名
# =============================================================================
# 有参数信息时必须命中允许的参数比较状态；只有缺少可比较参数时才退化为符号存在性
# 判断。因此参数类型或数量不一致的同名函数不会被当成有效头文件映射。
@lru_cache(maxsize=SYMBOL_DECLARATION_CACHE_SIZE)
def _resource_declares_symbol(
    repo_root: str,
    resource: str,
    symbol: str,
    qualifier: Optional[str],
) -> bool:
    header_path = Path(repo_root) / resource.lstrip("/")
    if not header_path.is_file():
        return False
    header_text = _read_header_text(str(header_path))
    return _header_declares(
        header_text,
        PrototypeDeclaration(symbol=symbol, qualifier=qualifier),
    )


@lru_cache(maxsize=SIGNATURE_STATUS_CACHE_SIZE)
def _resource_signature_match_status(
    repo_root: str,
    resource: str,
    symbol: str,
    qualifier: Optional[str],
    parameters: str,
) -> str:
    header_path = Path(repo_root) / resource.lstrip("/")
    if not header_path.is_file():
        return "missing_header"
    header_text = _read_header_text(str(header_path))
    comparison = _compare_function_parameters(
        PrototypeDeclaration(symbol=symbol, qualifier=qualifier, parameters=parameters),
        header_text,
        resource,
    )
    return comparison.status if comparison is not None else "not_comparable"


@lru_cache(maxsize=SIGNATURE_COMPARISON_CACHE_SIZE)
def _resource_signature_comparison(
    repo_root: str,
    resource: str,
    symbol: str,
    qualifier: Optional[str],
    parameters: str,
) -> Optional[SignatureComparison]:
    header_path = Path(repo_root) / resource.lstrip("/")
    if not header_path.is_file():
        return SignatureComparison(
            resource=resource,
            symbol=symbol,
            status="missing_header",
            document_parameters=", ".join(
                _normalized_parameter_list(parameters, keep_names=True)
            ),
        )
    header_text = _read_header_text(str(header_path))
    return _compare_function_parameters(
        PrototypeDeclaration(symbol=symbol, qualifier=qualifier, parameters=parameters),
        header_text,
        resource,
    )


def _resource_signature_comparisons(
    declaration: PrototypeDeclaration,
    resources: Sequence[str],
    repo_root: Path,
) -> tuple[SignatureComparison, ...]:
    if declaration.parameters is None:
        return ()
    comparisons = []
    for resource in resources:
        comparison = _resource_signature_comparison(
            str(repo_root),
            resource,
            declaration.symbol,
            declaration.qualifier,
            declaration.parameters,
        )
        if comparison is not None:
            comparisons.append(comparison)
    return tuple(comparisons)


def _resources_declaring(
    declaration: PrototypeDeclaration,
    resources: Sequence[str],
    repo_root: Path,
) -> tuple[str, ...]:
    matches = []
    for resource in resources:
        if declaration.parameters is None:
            declares = _resource_declares_symbol(
                str(repo_root),
                resource,
                declaration.symbol,
                declaration.qualifier,
            )
        else:
            declares = (
                _resource_signature_match_status(
                    str(repo_root),
                    resource,
                    declaration.symbol,
                    declaration.qualifier,
                    declaration.parameters,
                )
                in HEADER_MAPPING_PARAMETER_STATUSES
            )
        if not declares:
            header_path = repo_root / resource.lstrip("/")
            declares = (
                header_path.is_file()
                and _matching_function_macro(
                    declaration, _read_header_text(str(header_path))
                )
                is not None
            )
        if declares:
            matches.append(resource)
    return _unique(matches)


# =============================================================================
# 头文件查找：构建并遍历 include 关系图
# =============================================================================
# 解析候选头文件的直接 #include，并以去重的深度优先遍历形成递归闭包。该闭包用于在
# 文档提示的是聚合头文件时，继续定位真正包含 API 声明的下游头文件。
@lru_cache(maxsize=None)
def _resource_direct_include_resources(repo_root: str, resource: str) -> tuple[str, ...]:
    root = Path(repo_root)
    header_path = root / resource.lstrip("/")
    if not header_path.is_file():
        return ()
    header_text = _read_header_text(str(header_path))
    resources = []
    for include_match in HEADER_INCLUDE_RE.finditer(header_text):
        include_path = include_match.group(1)
        relative_resource = _header_resource(header_path.parent / include_path, root)
        if relative_resource is not None:
            resources.append(relative_resource)
        include_resources, _ = _header_path_resolution_details(include_path, root)
        resources.extend(include_resources)
    return _unique(resources)


def _resource_includes_resource(
    repo_root: Path,
    source_resource: str,
    target_resource: str,
) -> bool:
    seen = set()
    stack = list(_resource_direct_include_resources(str(repo_root), source_resource))
    while stack:
        resource = stack.pop()
        if resource == target_resource:
            return True
        if resource in seen:
            continue
        seen.add(resource)
        stack.extend(_resource_direct_include_resources(str(repo_root), resource))
    return False


def _resource_include_closure(
    repo_root: Path,
    source_resources: Sequence[str],
) -> tuple[str, ...]:
    seen = set(source_resources)
    resources = []
    stack = []
    for source_resource in reversed(source_resources):
        stack.extend(
            reversed(_resource_direct_include_resources(str(repo_root), source_resource))
        )
    while stack:
        resource = stack.pop()
        if resource in seen:
            continue
        seen.add(resource)
        if _is_api_header_resource(resource):
            resources.append(resource)
        stack.extend(
            reversed(_resource_direct_include_resources(str(repo_root), resource))
        )
    return tuple(resources)


def _scope_header_include_relations(
    scope_headers: Sequence[str],
    headers: Sequence[str],
    repo_root: Path,
) -> tuple[tuple[str, str], ...]:
    if not scope_headers or not headers:
        return ()
    return tuple(
        dict.fromkeys(
            (scope_header, header)
            for header in headers
            for scope_header in scope_headers
            if scope_header != header
            and _resource_includes_resource(repo_root, scope_header, header)
        )
    )


def _scope_include_relation_kind(
    headers: Sequence[str],
    include_relations: Sequence[tuple[str, str]],
) -> str:
    if not headers or not include_relations:
        return ""
    included_headers = {header for _, header in include_relations}
    if any(header not in included_headers for header in headers):
        return ""
    include_scopes = {scope for scope, _ in include_relations}
    header_label = "one_header" if len(headers) == 1 else "multiple_headers"
    scope_label = "one_scope" if len(include_scopes) == 1 else "multiple_scopes"
    return f"{header_label}_{scope_label}"


def _combined_hint_header_resources(
    markdown_text: str,
    document_path: Path,
    repo_root: Path,
) -> tuple[str, ...]:
    return _unique(
        (
            *extract_document_header_resources(markdown_text, repo_root),
            *extract_document_include_header_resources(markdown_text, repo_root),
            *_related_usage_header_resources(document_path, repo_root),
        )
    )


# =============================================================================
# 头文件查找：结果状态、候选去重与可疑同名诊断
# =============================================================================
# 多个成功候选会被保留并标记 multiple；完全未命中时，再收集参数类型或数量不一致的
# 同名声明作为诊断信息，但这些可疑候选不会写入最终 headers。
def _mapping_status(single_status: str, multiple_status: str, headers: Sequence[str]) -> str:
    return single_status if len(headers) == 1 else multiple_status


def _comparison_candidate_key(comparison: SignatureComparison) -> tuple[Any, ...]:
    return (
        comparison.resource,
        comparison.header_symbol or comparison.symbol,
        comparison.header_parameter_source,
        tuple(comparison.header_parameters),
        tuple(comparison.macro_chain),
    )


def _used_successful_candidate_keys(
    mappings: Sequence[SignatureHeaderMapping],
    declarations_by_id: dict[str, PrototypeDeclaration],
    repo_root: Path,
) -> dict[str, set[tuple[Any, ...]]]:
    used: dict[str, set[tuple[Any, ...]]] = {}
    for mapping in mappings:
        if not mapping.headers:
            continue
        declaration = declarations_by_id.get(
            mapping.document_signature or mapping.canonical_id
        )
        if declaration is None:
            continue
        for comparison in _resource_signature_comparisons(
            declaration,
            mapping.headers,
            repo_root,
        ):
            if comparison.status not in HEADER_MAPPING_PARAMETER_STATUSES:
                continue
            used.setdefault(declaration.symbol, set()).add(
                _comparison_candidate_key(comparison)
            )
    return used


def _suspicious_same_name_candidates(
    declaration: PrototypeDeclaration,
    resources: Sequence[str],
    repo_root: Path,
    used_candidate_keys: set[tuple[Any, ...]],
) -> tuple[SignatureComparison, ...]:
    candidates = []
    seen = set()
    for resource in resources:
        header_path = repo_root / resource.lstrip("/")
        if not header_path.is_file():
            continue
        header_text = _read_header_text(str(header_path))
        same_count_comparisons = _same_parameter_count_same_name_comparisons(
            declaration,
            header_text,
            resource,
        )
        for comparison in same_count_comparisons:
            key = _comparison_candidate_key(comparison)
            if key in used_candidate_keys or key in seen:
                continue
            seen.add(key)
            candidates.append(comparison)
            if len(candidates) >= SUSPICIOUS_SAME_NAME_CANDIDATE_LIMIT:
                return tuple(sorted(candidates, key=_comparison_rank))
    return tuple(sorted(candidates, key=_comparison_rank))


def _annotate_suspicious_same_name_mappings(
    mappings: Sequence[SignatureHeaderMapping],
    declarations_by_id: dict[str, PrototypeDeclaration],
    suspicious_search_resources: dict[str, Sequence[str]],
    repo_root: Path,
) -> tuple[SignatureHeaderMapping, ...]:
    used_candidates_by_symbol = _used_successful_candidate_keys(
        mappings,
        declarations_by_id,
        repo_root,
    )
    annotated = []
    for mapping in mappings:
        if mapping.status != "not_found":
            annotated.append(mapping)
            continue
        declaration = declarations_by_id.get(
            mapping.document_signature or mapping.canonical_id
        )
        if declaration is None:
            annotated.append(mapping)
            continue
        suspicious_candidates = _suspicious_same_name_candidates(
            declaration,
            suspicious_search_resources.get(mapping.canonical_id, ()),
            repo_root,
            used_candidates_by_symbol.get(declaration.symbol, set()),
        )
        if not suspicious_candidates:
            annotated.append(mapping)
            continue
        annotated.append(
            replace(
                mapping,
                status="suspicious_same_name",
                suspicious_candidates=suspicious_candidates,
            )
        )
    return tuple(annotated)


def _select_function_name_expansion_mappings(
    mappings: Sequence[SignatureHeaderMapping],
    declarations_by_id: dict[str, PrototypeDeclaration],
) -> tuple[SignatureHeaderMapping, ...]:
    groups: dict[
        tuple[str, tuple[tuple[str, str], ...], tuple[str, ...]],
        list[int],
    ] = {}
    for index, mapping in enumerate(mappings):
        declaration = declarations_by_id.get(
            mapping.document_signature or mapping.canonical_id
        )
        if declaration is None or not declaration.function_name_expansion_source:
            continue
        key = (
            declaration.function_name_expansion_source,
            tuple(declaration.placeholder_bindings),
            tuple(declaration.archs),
        )
        groups.setdefault(key, []).append(index)

    discarded_indexes = set()
    for indexes in groups.values():
        successful_indexes = [index for index in indexes if mappings[index].headers]
        if len(successful_indexes) == 1:
            discarded_indexes.update(set(indexes) - set(successful_indexes))
        elif len(successful_indexes) > 1:
            candidates = ", ".join(
                mappings[index].canonical_id for index in successful_indexes
            )
            raise ValueError(
                "ambiguous function-name placeholder expansion; "
                f"multiple concrete declarations matched headers: {candidates}"
            )

    return tuple(
        mapping
        for index, mapping in enumerate(mappings)
        if index not in discarded_indexes
    )


# =============================================================================
# 头文件查找：逐原型执行分层搜索并生成最终映射
# =============================================================================
# 搜索优先级为：文档直接提示 -> 提示头文件的 include 闭包 -> 当前 API 家族目录 ->
# 全量 include 目录。每个函数原型独立解析，成功后立即停止该原型的后续低优先级搜索。
def resolve_signature_header_mappings(
    document_path: Path,
    markdown_text: str,
    repo_root: Path = REPO_ROOT,
) -> tuple[Sequence[SignatureHeaderMapping], Sequence[str]]:
    declarations = extract_prototype_declarations(markdown_text)
    declarations_by_id = {
        _document_signature_id(declaration): declaration
        for declaration in declarations
    }

    # 先建立一次共享候选集合，避免每个函数原型重复解析提示和扫描 include 目录。
    hint_headers = _combined_hint_header_resources(markdown_text, document_path, repo_root)
    hint_include_headers = _resource_include_closure(repo_root, hint_headers)
    hinted_search_headers = _unique((*hint_headers, *hint_include_headers))
    fallback_headers = (
        ()
        if hint_headers
        else _search_directory_header_resources(document_path, repo_root)
    )
    include_headers = _all_include_header_resources(str(repo_root))
    mappings = []
    suspicious_search_resources: dict[str, Sequence[str]] = {}

    for declaration in declarations:
        document_parameters = ""
        if declaration.parameters is not None:
            document_parameters = ", ".join(
                _normalized_parameter_list(declaration.parameters, keep_names=True)
            )
        canonical_id = _canonical_signature_id(declaration)
        document_signature = _document_signature_id(declaration)
        override_id = ""
        prototype_function = declaration.prototype_function or canonical_id
        prototype_document_signature = (
            declaration.prototype_document_signature or document_signature
        )

        if hint_headers:
            # 优先级 1：函数原型直接声明在文档提示的头文件中。
            in_scope_headers = _resources_declaring(declaration, hint_headers, repo_root)
            if in_scope_headers:
                mappings.append(
                    SignatureHeaderMapping(
                        name=declaration.symbol,
                        headers=in_scope_headers,
                        status=_mapping_status(
                            "hint_scope_single",
                            "hint_scope_multiple",
                            in_scope_headers,
                        ),
                        source="hint_scope",
                        has_header_hint=True,
                        document_parameters=document_parameters,
                        canonical_id=canonical_id,
                        override_id=override_id,
                        scope_headers=hint_headers,
                        archs=declaration.archs,
                        document_signature=document_signature,
                        prototype_function=prototype_function,
                        prototype_document_signature=prototype_document_signature,
                        placeholder_bindings=declaration.placeholder_bindings,
                        template_parameters=declaration.template_parameters,
                    )
                )
                continue

            # 优先级 2：提示的是聚合头文件，实际声明位于其递归 include 中。
            included_scope_headers = _resources_declaring(
                declaration,
                hint_include_headers,
                repo_root,
            )
            if included_scope_headers:
                include_relations = _scope_header_include_relations(
                    hint_headers,
                    included_scope_headers,
                    repo_root,
                )
                include_relation_kind = _scope_include_relation_kind(
                    included_scope_headers,
                    include_relations,
                )
                mappings.append(
                    SignatureHeaderMapping(
                        name=declaration.symbol,
                        headers=included_scope_headers,
                        status=_mapping_status(
                            "scope_include_single",
                            "scope_include_multiple",
                            included_scope_headers,
                        ),
                        source="scope_include",
                        has_header_hint=True,
                        include_relations=include_relations,
                        include_relation_kind=include_relation_kind,
                        document_parameters=document_parameters,
                        canonical_id=canonical_id,
                        override_id=override_id,
                        scope_headers=hint_headers,
                        archs=declaration.archs,
                        document_signature=document_signature,
                        prototype_function=prototype_function,
                        prototype_document_signature=prototype_document_signature,
                        placeholder_bindings=declaration.placeholder_bindings,
                        template_parameters=declaration.template_parameters,
                    )
                )
                continue

            # 有提示但提示范围内未命中时，才扫描全量 include 并标记为范围外结果。
            out_of_scope_headers = tuple(
                resource
                for resource in _resources_declaring(
                    declaration, include_headers, repo_root
                )
                if resource not in hinted_search_headers
            )
            if out_of_scope_headers:
                status = _mapping_status(
                    "out_of_scope_single",
                    "out_of_scope_multiple",
                    out_of_scope_headers,
                )
                mappings.append(
                    SignatureHeaderMapping(
                        name=declaration.symbol,
                        headers=out_of_scope_headers,
                        status=status,
                        source="full_include",
                        has_header_hint=True,
                        document_parameters=document_parameters,
                        canonical_id=canonical_id,
                        override_id=override_id,
                        scope_headers=hint_headers,
                        archs=declaration.archs,
                        document_signature=document_signature,
                        prototype_function=prototype_function,
                        prototype_document_signature=prototype_document_signature,
                        placeholder_bindings=declaration.placeholder_bindings,
                        template_parameters=declaration.template_parameters,
                    )
                )
                continue

            suspicious_search_resources[canonical_id] = _unique(
                (*hinted_search_headers, *include_headers)
            )
            mappings.append(
                SignatureHeaderMapping(
                    name=declaration.symbol,
                    headers=(),
                    status="not_found",
                    source="hint_scope",
                    has_header_hint=True,
                    document_parameters=document_parameters,
                    canonical_id=canonical_id,
                    override_id=override_id,
                    scope_headers=hint_headers,
                    archs=declaration.archs,
                    document_signature=document_signature,
                    prototype_function=prototype_function,
                    prototype_document_signature=prototype_document_signature,
                    placeholder_bindings=declaration.placeholder_bindings,
                    template_parameters=declaration.template_parameters,
                )
            )
            continue

        # 优先级 3：没有任何文档提示时，扫描 API 家族对应的 include 目录。
        fallback_matches = _resources_declaring(
            declaration, fallback_headers, repo_root
        )
        if fallback_matches:
            mappings.append(
                SignatureHeaderMapping(
                    name=declaration.symbol,
                    headers=fallback_matches,
                    status=_mapping_status(
                        "fallback_single",
                        "fallback_multiple",
                        fallback_matches,
                    ),
                    source="prototype_fallback",
                    has_header_hint=False,
                    document_parameters=document_parameters,
                    canonical_id=canonical_id,
                    override_id=override_id,
                    scope_headers=fallback_matches,
                    archs=declaration.archs,
                    document_signature=document_signature,
                    prototype_function=prototype_function,
                    prototype_document_signature=prototype_document_signature,
                    placeholder_bindings=declaration.placeholder_bindings,
                    template_parameters=declaration.template_parameters,
                )
            )
            continue

        # 优先级 4：家族目录仍未命中时，扫描其余 include 头文件。
        out_of_scope_headers = tuple(
            resource
            for resource in _resources_declaring(declaration, include_headers, repo_root)
            if resource not in fallback_headers
        )
        if out_of_scope_headers:
            mappings.append(
                SignatureHeaderMapping(
                    name=declaration.symbol,
                    headers=out_of_scope_headers,
                    status=_mapping_status(
                        "out_of_scope_single",
                        "out_of_scope_multiple",
                        out_of_scope_headers,
                    ),
                    source="full_include",
                    has_header_hint=False,
                    document_parameters=document_parameters,
                    canonical_id=canonical_id,
                    override_id=override_id,
                    scope_headers=fallback_headers,
                    archs=declaration.archs,
                    document_signature=document_signature,
                    prototype_function=prototype_function,
                    prototype_document_signature=prototype_document_signature,
                    placeholder_bindings=declaration.placeholder_bindings,
                    template_parameters=declaration.template_parameters,
                )
            )
            continue

        suspicious_search_resources[canonical_id] = _unique(
            (*fallback_headers, *include_headers)
        )
        mappings.append(
            SignatureHeaderMapping(
                name=declaration.symbol,
                headers=(),
                status="not_found",
                source="prototype_fallback",
                has_header_hint=False,
                document_parameters=document_parameters,
                canonical_id=canonical_id,
                override_id=override_id,
                scope_headers=fallback_headers,
                archs=declaration.archs,
                document_signature=document_signature,
                prototype_function=prototype_function,
                prototype_document_signature=prototype_document_signature,
                placeholder_bindings=declaration.placeholder_bindings,
                template_parameters=declaration.template_parameters,
            )
        )

    mappings = list(
        _select_function_name_expansion_mappings(mappings, declarations_by_id)
    )
    mappings = list(
        _annotate_suspicious_same_name_mappings(
            mappings, declarations_by_id, suspicious_search_resources, repo_root
        )
    )
    mappings = list(
        _assign_signature_identifiers(mappings, declarations_by_id, repo_root)
    )
    mapped_headers = _unique(
        tuple(
            resource
            for mapping in mappings
            for resource in mapping.headers
        )
    )
    candidates = mapped_headers if mapped_headers else hint_headers
    return tuple(mappings), candidates


# =============================================================================
# 头文件查找：汇总映射结果及兼容旧版文档级解析
# =============================================================================
# 当前 YAML signature.headers 使用逐原型映射；以下函数负责汇总 API resource、生成详细
# 比较记录，并保留 resolve_header_details/resolve_header_resource 文档级兼容接口。
def _resource_from_signature_mappings(
    mappings: Sequence[SignatureHeaderMapping],
    candidates: Sequence[str],
) -> Optional[str]:
    mapped_headers = _unique(
        tuple(resource for mapping in mappings for resource in mapping.headers)
    )
    if len(mapped_headers) == 1:
        return mapped_headers[0]
    if not mappings and len(candidates) == 1:
        return candidates[0]
    return None


def _reference_definition_header_resources(
    document_path: Path,
    markdown_text: str,
    repo_root: Path,
) -> tuple[str, ...]:
    """Find a uniquely ranked declaration header for a non-function reference."""
    if classify_api_document(markdown_text) not in {
        "enum_api",
        "record_api",
        "constant_api",
        "type_alias_api",
    }:
        return ()
    title = extract_title(markdown_text)
    if title is None or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", title):
        return ()

    resources = tuple(
        resource
        for resource in _all_include_header_resources(str(repo_root))
        if _resource_declares_symbol(str(repo_root), resource, title, None)
    )
    if not resources:
        return ()

    scored_resources = []
    for resource in resources:
        header_path = repo_root / resource.lstrip("/")
        scored_resources.append(
            (_header_category_score(document_path, header_path, repo_root), resource)
        )
    best_score = max(score for score, _ in scored_resources)
    best_resources = tuple(
        resource for score, resource in scored_resources if score == best_score
    )
    if best_score <= 0 or len(best_resources) != 1:
        return ()
    return best_resources


def _compare_signature_header_mappings(
    markdown_text: str,
    mappings: Sequence[SignatureHeaderMapping],
    repo_root: Path,
) -> Sequence[SignatureComparison]:
    declarations = {
        _document_signature_id(declaration): declaration
        for declaration in extract_prototype_declarations(markdown_text)
    }
    comparisons = []
    for mapping in mappings:
        declaration = declarations.get(mapping.document_signature or mapping.canonical_id)
        if declaration is None:
            continue
        for resource in mapping.headers:
            header_path = repo_root / resource.lstrip("/")
            if not header_path.is_file():
                continue
            header_text = _read_header_text(str(header_path))
            comparison = _compare_function_parameters(
                declaration, header_text, resource
            )
            if comparison is not None:
                comparisons.append(comparison)
    return tuple(comparisons)


def _select_public_header_candidate(
    document_path: Path, resources: Sequence[str], repo_root: Path
) -> Optional[str]:
    specific_resources = [
        resource
        for resource in resources
        if resource != "/include/kernel_operator.h"
    ]
    if len(specific_resources) == 1:
        return specific_resources[0]
    if not specific_resources:
        return None

    scored_resources = []
    for resource in specific_resources:
        header_path = repo_root / resource.lstrip("/")
        scored_resources.append(
            (_header_category_score(document_path, header_path, repo_root), resource)
        )
    best_score = max(score for score, _ in scored_resources)
    best_resources = [
        resource for score, resource in scored_resources if score == best_score
    ]
    return best_resources[0] if best_score > 0 and len(best_resources) == 1 else None


def resolve_header_details(
    document_path: Path,
    repo_root: Path = REPO_ROOT,
    context_document_path: Optional[Path] = None,
) -> tuple[Optional[str], Sequence[str]]:
    path_for_header_rules = context_document_path or document_path
    markdown_text = document_path.read_text(encoding="utf-8")
    document_header_resources = extract_document_header_resources(
        markdown_text, repo_root
    )
    if len(document_header_resources) == 1:
        return document_header_resources[0], document_header_resources
    if len(document_header_resources) > 1:
        return None, document_header_resources

    public_header_resources = _unique(
        (
            *extract_document_include_header_resources(markdown_text, repo_root),
            *_related_usage_header_resources(path_for_header_rules, repo_root),
        )
    )

    declarations = extract_prototype_declarations(markdown_text)
    if not declarations:
        reference_headers = _reference_definition_header_resources(
            path_for_header_rules, markdown_text, repo_root
        )
        if reference_headers:
            return reference_headers[0], reference_headers
        public_header = _select_public_header_candidate(
            path_for_header_rules, public_header_resources, repo_root
        )
        return public_header, public_header_resources

    matches = []
    for header_directory in _header_search_directories(path_for_header_rules, repo_root):
        matches.extend(_matching_headers(header_directory, declarations))
    if not matches:
        for header_directory in _fallback_header_search_directories(
            path_for_header_rules, repo_root
        ):
            matches.extend(_matching_headers(header_directory, declarations))
    candidates = tuple(
        f"/{match.relative_to(repo_root).as_posix()}" for match in matches
    )
    if len(matches) == 1:
        return candidates[0], candidates

    category_match = _resolve_by_document_category(
        path_for_header_rules, matches, repo_root
    )
    if category_match is None:
        public_header = _select_public_header_candidate(
            path_for_header_rules, public_header_resources, repo_root
        )
        if public_header is not None:
            return public_header, candidates or public_header_resources
        return None, candidates or public_header_resources
    return f"/{category_match.relative_to(repo_root).as_posix()}", candidates


def resolve_header_resource(
    document_path: Path,
    repo_root: Path = REPO_ROOT,
    context_document_path: Optional[Path] = None,
) -> Optional[str]:
    return resolve_header_details(document_path, repo_root, context_document_path)[0]


def extract_api_metadata(
    document_path: Path,
    version: str,
    repo_root: Path = REPO_ROOT,
    context_document_path: Optional[Path] = None,
    example_discovery_index: Optional[ExampleDiscoveryIndex] = None,
) -> ApiMetadata:
    path_for_header_rules = context_document_path or document_path
    markdown_text = document_path.read_text(encoding="utf-8")
    document_kind = classify_api_document(markdown_text)
    api_kind = {
        "constant_api": "constant",
        "enum_api": "enum",
        "record_api": "record",
        "type_reference": "type_catalog",
        "type_alias_api": "type_alias",
    }.get(document_kind, "function")
    title = extract_title(markdown_text) or path_for_header_rules.stem
    signature_mappings, header_candidates = resolve_signature_header_mappings(
        path_for_header_rules, markdown_text, repo_root
    )
    declaration = _typed_declaration(markdown_text, api_kind, title)
    if api_kind == "record" and declaration is not None:
        api_kind = str(declaration.get("record_kind") or "record")
    if (
        api_kind == "function"
        and signature_mappings
        and all(mapping.macro_id for mapping in signature_mappings)
    ):
        api_kind = "function_macro"
    type_catalog = _type_catalog(markdown_text) if api_kind == "type_catalog" else None
    document_header_resources = extract_document_header_resources(markdown_text, repo_root)
    if len(document_header_resources) == 1:
        resource = document_header_resources[0]
        header_candidates = document_header_resources
    else:
        resource = _resource_from_signature_mappings(
            signature_mappings, header_candidates
        )
        if not signature_mappings and not header_candidates:
            resource, header_candidates = resolve_header_details(
                document_path, repo_root, path_for_header_rules
            )
    description_analysis = analyze_description(markdown_text)
    _, document_legacy_mappings = _document_include_header_details(
        markdown_text,
        repo_root,
        source_document_path=path_for_header_rules,
    )
    _, usage_legacy_mappings = _related_usage_header_details(
        path_for_header_rules, repo_root
    )
    archs = supported_archs(markdown_text)
    examples = (
        ()
        if api_kind not in {"function", "function_macro"}
        else extract_example_references(
            markdown_text,
            path_for_header_rules,
            signature_mappings,
            archs,
            repo_root,
            example_discovery_index,
        )
    )
    return ApiMetadata(
        filename=path_for_header_rules.name,
        title=title,
        description=description_analysis.description,
        resource=resource,
        version=version,
        header_candidates=header_candidates,
        document_path=path_for_header_rules,
        description_analysis=description_analysis,
        legacy_include_mappings=_unique_legacy_include_mappings(
            (
                *document_legacy_mappings,
                *usage_legacy_mappings,
            )
        ),
        signature=signature_mappings,
        signature_comparisons=_compare_signature_header_mappings(
            markdown_text, signature_mappings, repo_root
        ),
        archs=archs,
        examples=examples,
        api_kind=api_kind,
        declaration=declaration,
        type_catalog=type_catalog,
    )


def _yaml_scalar(value: Optional[str]) -> str:
    return "null" if value is None else json.dumps(value, ensure_ascii=False)


def _yaml_inline_list(values: Sequence[str]) -> str:
    return "[" + ", ".join(_yaml_scalar(value) for value in values) + "]"


def _display_path(path: Path, source_directory: Path) -> str:
    try:
        return path.relative_to(source_directory).as_posix()
    except ValueError:
        return path.as_posix()


def _metadata_label(item: ApiMetadata, source_directory: Path) -> str:
    if item.document_path is None:
        return item.filename
    return _display_path(item.document_path, source_directory)


def _comparison_status_label(status: str) -> str:
    return {
        "exact": "exact",
        "array_parameter_match": "array parameter match",
        "macro_parameter_names_match": "macro parameter names match",
        "object_macro_alias_match": "object macro alias match",
        "parameter_name_difference": "parameter names differ",
        "parameter_type_mismatch": "parameter types differ",
        "parameter_count_mismatch": "parameter counts differ",
        "api_not_found_in_header": "no matching API declaration in header",
        "not_comparable": "no parsable header function declaration",
    }[status]


def render_report(
    metadata_items: Sequence[ApiMetadata],
    source_directory: Path,
    markdown_files: Sequence[Path],
    list_documents: Sequence[Path],
    other_documents: Sequence[Path],
) -> str:
    multi_header_items = [
        item for item in metadata_items if len(item.header_candidates) > 1
    ]
    header_directive_first_block_items = [
        item
        for item in metadata_items
        if (
            item.description_analysis is not None
            and item.description_analysis.raw_first_block_is_header_directive
        )
    ]
    raw_formula_intro_first_block_items = [
        item
        for item in metadata_items
        if (
            item.description_analysis is not None
            and item.description_analysis.raw_first_block_has_formula_intro
        )
    ]
    legacy_mapping_consumers = {}
    for item in metadata_items:
        for mapping in item.legacy_include_mappings:
            legacy_mapping_consumers.setdefault(mapping, []).append(item)
    legacy_include_mappings = tuple(legacy_mapping_consumers)
    legacy_mapping_api_documents = {
        item.document_path
        for consumers in legacy_mapping_consumers.values()
        for item in consumers
        if item.document_path is not None
    }
    signature_comparisons = [
        (item, comparison)
        for item in metadata_items
        for comparison in item.signature_comparisons
    ]
    signature_comparison_counts = {
        status: sum(
            comparison.status == status
            for _, comparison in signature_comparisons
        )
        for status in (
            "exact",
            "array_parameter_match",
            "macro_parameter_names_match",
            "object_macro_alias_match",
            "parameter_name_difference",
            "parameter_type_mismatch",
            "parameter_count_mismatch",
            "api_not_found_in_header",
            "not_comparable",
        )
    }
    signature_difference_items = [
        (item, comparison)
        for item, comparison in signature_comparisons
        if comparison.status
        in {
            "parameter_name_difference",
            "parameter_type_mismatch",
            "parameter_count_mismatch",
        }
    ]
    header_api_not_found_items = [
        (item, comparison)
        for item, comparison in signature_comparisons
        if comparison.status == "api_not_found_in_header"
    ]
    missing_header_items = [
        item
        for item in metadata_items
        if item.resource is None and not item.header_candidates
    ]
    example_references = [
        example
        for item in metadata_items
        for example in item.examples
        if is_authoritative_example(example)
    ]
    example_issues = [
        issue
        for item in metadata_items
        for example in item.examples
        for issue in example.issues
    ]
    lines = [
        "# API YAML Metadata Preview",
        "",
        f"Source directory: `{source_directory}`",
        "",
        "Notes: `version` is the API lifecycle range collected from package and release evidence. "
        "`tag` is intentionally omitted until evidence-backed annotation is available.",
        "",
        "## Summary",
        "",
        f"- Markdown files found: `{len(markdown_files)}`",
        f"- Matched API documents (`## 函数原型`): `{len(metadata_items)}`",
        f"- Skipped Markdown link-list documents: `{len(list_documents)}`",
        f"- Skipped other non-API documents: `{len(other_documents)}`",
        f"- APIs declared in multiple headers: `{len(multi_header_items)}`",
        f"- APIs with no matching header: `{len(missing_header_items)}`",
        f"- APIs with a resolved header but no matching API declaration: "
        f"`{len(header_api_not_found_items)}`",
        f"- Raw first prose blocks that are header directives: "
        f"`{len(header_directive_first_block_items)}`",
        f"- Raw first prose blocks ending with formula intro: "
        f"`{len(raw_formula_intro_first_block_items)}`",
        f"- Legacy include source declarations: "
        f"`{len(legacy_include_mappings)}`",
        f"- API documents using legacy include mappings: "
        f"`{len(legacy_mapping_api_documents)}`",
        f"- Function prototype signature comparisons: "
        f"`{len(signature_comparisons)}`",
        f"- Complete example references: `{len(example_references)}`",
        f"- Example diagnostics: `{len(example_issues)}`",
        f"  - Exact: `{signature_comparison_counts['exact']}`",
        f"  - Macro parameter names match: "
        f"`{signature_comparison_counts['macro_parameter_names_match']}`",
        f"  - Object macro alias match: "
        f"`{signature_comparison_counts['object_macro_alias_match']}`",
        f"  - Array parameter match: "
        f"`{signature_comparison_counts['array_parameter_match']}`",
        f"  - Parameter names differ: "
        f"`{signature_comparison_counts['parameter_name_difference']}`",
        f"  - Parameter types differ: "
        f"`{signature_comparison_counts['parameter_type_mismatch']}`",
        f"  - Parameter counts differ: "
        f"`{signature_comparison_counts['parameter_count_mismatch']}`",
        f"  - No matching API declaration in header: "
        f"`{signature_comparison_counts['api_not_found_in_header']}`",
        f"  - Not comparable: "
        f"`{signature_comparison_counts['not_comparable']}`",
        "",
    ]
    if list_documents:
        lines.extend(["## Skipped Markdown Link-List Documents", ""])
        for path in list_documents:
            title = None
            if path.is_file():
                title = extract_title(path.read_text(encoding="utf-8"))
            title_note = f" (title: {title})" if title else ""
            lines.append(
                f"- `{_display_path(path, source_directory)}`{title_note}."
            )
        lines.append("")

    if other_documents:
        lines.extend(["## Skipped Other Non-API Documents", ""])
        for path in other_documents:
            lines.append(f"- `{_display_path(path, source_directory)}`")
        lines.append("")

    if header_directive_first_block_items:
        lines.extend(["## Raw First Prose Blocks That Are Header Directives", ""])
        for item in header_directive_first_block_items:
            label = _metadata_label(item, source_directory)
            analysis = item.description_analysis
            source = analysis.description_source or "no semantic description"
            lines.append(
                f"- `{label}` ({source}): "
                f"{_yaml_scalar(analysis.raw_first_prose_block)}"
            )
        lines.append("")

    if raw_formula_intro_first_block_items:
        lines.extend(["## Raw First Prose Blocks Ending with Formula Intro", ""])
        for item in raw_formula_intro_first_block_items:
            label = _metadata_label(item, source_directory)
            lines.append(
                f"- `{label}`: "
                f"{_yaml_scalar(item.description_analysis.raw_first_prose_block)}"
            )
        lines.append("")

    if legacy_include_mappings:
        lines.extend(["## Legacy Include Path Mappings", ""])
        for mapping in legacy_include_mappings:
            source_document = (
                _display_path(mapping.source_document_path, source_directory)
                if mapping.source_document_path is not None
                else "<unknown source document>"
            )
            consumer_count = len(legacy_mapping_consumers[mapping])
            lines.append(
                f"- `{source_document}` ({mapping.source_kind}): "
                f"`{mapping.source_path}` -> `{mapping.resource}`; "
                f"used by `{consumer_count}` API documents"
            )
        lines.append("")

    if signature_difference_items:
        lines.extend(["## Function Prototype Signature Differences", ""])
        for item, comparison in signature_difference_items:
            lines.append(
                f"- `{_metadata_label(item, source_directory)}` -> "
                f"`{comparison.resource}`: `{comparison.symbol}` "
                f"({_comparison_status_label(comparison.status)})"
            )
            lines.append(
                f"  - document: `{comparison.document_parameters}`"
            )
            for header_parameters in comparison.header_parameters:
                lines.append(f"  - header: `{header_parameters}`")
        lines.append("")

    if header_api_not_found_items:
        lines.extend(
            ["## APIs With Resolved Header But Missing API Declaration", ""]
        )
        for item, comparison in header_api_not_found_items:
            lines.append(
                f"- `{_metadata_label(item, source_directory)}` -> "
                f"`{comparison.resource}`: `{comparison.symbol}` "
                f"({_comparison_status_label(comparison.status)})"
            )
        lines.append("")

    if missing_header_items:
        lines.extend(["## APIs With No Matching Header", ""])
        for item in missing_header_items:
            lines.append(f"- `{_metadata_label(item, source_directory)}`")
        lines.append("")

    warnings = []
    for item in metadata_items:
        label = _metadata_label(item, source_directory)
        if item.description is None:
            warnings.append(f"`{label}`: description was not extracted.")
        if item.resource is None:
            if item.header_candidates:
                warnings.append(
                    f"`{label}`: header resource remained ambiguous across "
                    f"`{len(item.header_candidates)}` matching headers."
                )
            else:
                warnings.append(
                    f"`{label}`: no matching header declaration was found."
                )
        elif any(
            comparison.status == "api_not_found_in_header"
            for comparison in item.signature_comparisons
        ):
            warnings.append(
                f"`{label}`: resolved header does not declare one or more APIs."
            )
        for example in item.examples:
            for issue in example.issues:
                location = (
                    f" ({issue.file}:{issue.line})"
                    if issue.file and issue.line
                    else ""
                )
                warnings.append(
                    f"`{label}`: {issue.type}{location}: {issue.message}"
                )
    lines.extend(["## Warnings", ""])
    if warnings:
        lines.extend(f"- {warning}" for warning in warnings)
    else:
        lines.append("- None.")
    lines.append("")

    if multi_header_items:
        lines.extend(["## APIs Declared in Multiple Headers", ""])
        for item in multi_header_items:
            lines.append(f"- `{_metadata_label(item, source_directory)}`")
            for candidate in item.header_candidates:
                lines.append(f"  - `{candidate}`")
        lines.append("")

    lines.extend(["## Metadata Items", ""])
    for item in metadata_items:
        lines.extend(
            [
                f"- `{_metadata_label(item, source_directory)}`",
                "",
                "  ```yaml",
                "  ---",
                "  okftype: reference.api",
                f"  api_kind: {_yaml_scalar(item.api_kind)}",
                f"  title: {_yaml_scalar(item.title)}",
                f"  path: {_yaml_scalar(_metadata_label(item, REPO_ROOT))}",
                f"  description: {_yaml_scalar(item.description)}",
            ]
        )
        if item.declaration:
            lines.append("  declaration:")
            for key in ("symbol", "cpp", "type", "value", "underlying_type", "record_kind"):
                value = item.declaration.get(key)
                if isinstance(value, str) and value:
                    lines.append(f"    {key}: {_yaml_scalar(value)}")
            for collection in ("members", "fields"):
                values = item.declaration.get(collection)
                if not isinstance(values, list):
                    continue
                lines.append(f"    {collection}:")
                for value in values:
                    if not isinstance(value, Mapping) or not isinstance(value.get("name"), str):
                        continue
                    lines.append(f"      - name: {_yaml_scalar(value['name'])}")
                    for key in ("value", "type", "bits", "description"):
                        item_value = value.get(key)
                        if isinstance(item_value, str) and item_value:
                            lines.append(f"        {key}: {_yaml_scalar(item_value)}")
        if item.type_catalog:
            groups = item.type_catalog.get("groups")
            if isinstance(groups, list):
                lines.extend(["  type_catalog:", "    groups:"])
                for group in groups:
                    if not isinstance(group, Mapping):
                        continue
                    name = group.get("name")
                    types = group.get("types")
                    if isinstance(name, str) and isinstance(types, list) and all(
                        isinstance(value, str) for value in types
                    ):
                        lines.append(f"      - name: {_yaml_scalar(name)}")
                        lines.append(f"        types: {_yaml_inline_list(types)}")
        if item.api_kind not in {"function", "function_macro"}:
            headers = tuple(dict.fromkeys(item.header_candidates))
            lines.append(
                f"  headers: {_yaml_inline_list(headers if len(headers) == 1 else ())}"
            )
        lines.append("  signature:")
        if item.signature:
            for signature in item.signature:
                lines.extend(
                    [
                        f"    - name: {_yaml_scalar(signature.name)}",
                        f"      id: {_yaml_scalar(signature.canonical_id)}",
                        f"      override_id: {_yaml_scalar(signature.override_id)}",
                    ]
                )
                signature_archs = tuple(signature.archs or item.archs)
                lines.append(f"      archs: {_yaml_inline_list(signature_archs)}")
                lines.append(f"      headers: {_yaml_inline_list(signature.headers)}")
                lines.append("      scope_headers:")
                if signature.scope_headers:
                    for header in signature.scope_headers:
                        lines.append(f"        - {_yaml_scalar(header)}")
                else:
                    lines[-1] = "      scope_headers: []"
        else:
            lines.append("    []")
        lines.append("  examples:")
        public_examples = [
            example for example in item.examples if is_authoritative_example(example)
        ]
        rendered_example = False
        if public_examples:
            for example in public_examples:
                overrides = public_example_signature_entries(example)
                for override in overrides:
                    rendered_example = True
                    lines.append(f"    - path: {_yaml_scalar(example.path)}")
                    lines.append(f"      archs: {_yaml_inline_list(example.archs)}")
                    lines.append(
                        f"      override_id: {_yaml_scalar(override['override_id'])}"
                    )
                    lines.append(f"      file: {_yaml_scalar(override['file'])}")
        if not rendered_example:
            lines[-1] = "  examples: []"
        lines.extend(
            [
                f"  version: {item.version}",
                "  ```",
                "",
            ]
        )
    return "\n".join(lines)


def _build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=(
            "Write a Markdown preview of detected API YAML metadata without "
            "printing results to stdout."
        )
    )
    parser.add_argument(
        "directory",
        nargs="?",
        type=Path,
        default=DEFAULT_API_DIRECTORY,
        help=(
            "Directory to scan recursively for Markdown API reference "
            f"documents (default: {DEFAULT_API_DIRECTORY})"
        ),
    )
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=DEFAULT_OUTPUT_PATH,
        help=f"Markdown report path (default: {DEFAULT_OUTPUT_PATH})",
    )
    parser.add_argument(
        "--version",
        default="unknown",
        help="API lifecycle range written into each metadata item",
    )
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = _build_parser().parse_args(argv)
    source_directory = args.directory.resolve()
    output_path = args.output.resolve()

    if not source_directory.is_dir():
        print(
            f"error: directory does not exist: {source_directory}", file=sys.stderr
        )
        return 2

    api_files = _collect_api_files(source_directory)
    if not api_files:
        print(
            "error: no API documents with a '## 函数原型' section found in: "
            f"{source_directory}",
            file=sys.stderr,
        )
        return 1

    markdown_files = sorted(
        path for path in source_directory.rglob("*.md") if path.is_file()
    )
    skipped_files = [path for path in markdown_files if path not in api_files]
    list_documents = [
        path
        for path in skipped_files
        if is_markdown_link_list(path.read_text(encoding="utf-8"))
    ]
    other_documents = [
        path for path in skipped_files if path not in list_documents
    ]
    example_discovery_index = build_example_discovery_index(REPO_ROOT)
    metadata_items = [
        extract_api_metadata(
            path,
            args.version,
            example_discovery_index=example_discovery_index,
        )
        for path in api_files
    ]
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(
        render_report(
            metadata_items,
            source_directory,
            markdown_files,
            list_documents,
            other_documents,
        )
        + "\n",
        encoding="utf-8",
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
