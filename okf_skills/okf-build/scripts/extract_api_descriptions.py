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


"""Extract source-derived descriptions from Chinese C API reference documents.

The description is the first prose block below ``## 功能说明``. The extractor does
not summarize text. Standard formula introducers such as ``计算公式如下`` are
removed together with the preceding clause delimiter.
"""

import argparse
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Sequence


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

FUNCTION_SECTION_RE = re.compile(
    r"^\s{0,3}#{2,6}\s+功能说明(?:\s|<|$)", re.IGNORECASE
)
PROTOTYPE_SECTION_RE = re.compile(
    r"^\s{0,3}#{2,6}\s+函数原型(?:\s|[<（(]|$)",
    re.IGNORECASE | re.MULTILINE,
)
HEADING_RE = re.compile(r"^\s{0,3}#{1,6}\s+")
FENCE_RE = re.compile(r"^\s{0,3}(?:`{3,}|~{3,})")
LIST_RE = re.compile(r"^\s*(?:[-*+]\s+|\d+[.)]\s+)")
MARKDOWN_LINK_RE = re.compile(r"\[[^\]]+\]\([^)]+\.md(?:#[^)]*)?\)")
HEADING_ANCHOR_RE = re.compile(r"<[^>]*>")
HEADING_PARENTHETICAL_RE = re.compile(r"[（(].*?[）)]")
API_LIKE_TITLE_RE = re.compile(r"^(?:asc_|ASC_)")
API_DECLARATION_RE = re.compile(r"\b(?:__aicore__|__simd_callee__)\b.*\(")
CONCRETE_API_DOCUMENT_KINDS = frozenset(
    {
        "function_api",
        "enum_api",
        "record_api",
        "constant_api",
        "type_alias_api",
        "type_reference",
        "macro_api",
    }
)
FUNCTION_API_STRUCTURE_HEADINGS = frozenset(
    {
        "产品支持情况",
        "功能说明",
        "参数说明",
        "返回值说明",
        "流水类型",
        "约束说明",
        "调用示例",
    }
)
FUNCTION_API_HEADING_ALIASES = {
    "返回值": "返回值说明",
    "示例代码": "调用示例",
}
REFERENCE_DESCRIPTION_SKIP_HEADINGS = frozenset(
    {
        "产品支持情况",
        "函数原型",
        "参数说明",
        "返回值说明",
        "流水类型",
        "约束说明",
        "调用示例",
        "常量定义",
        "枚举类具体定义",
        "枚举值详解",
        "结构体具体定义",
        "联合体具体定义",
        "字段详解",
    }
)
FORMULA_INTRO_RE = re.compile(
    r"计算公式(?:及图示)?如下|公式如下|计算方法如下|其计算公式为|计算公式为"
)
FORMULA_INTRO_SUFFIX_RE = re.compile(
    r"(?:计算公式(?:及图示)?如下|公式如下|计算方法如下|其计算公式为|计算公式为)[。:：]?$"
)
HEADER_DIRECTIVE_RE = re.compile(r"^\s*头文件路径(?:为)?\s*[：:]")
HEADER_PATH_RE = re.compile(r"([A-Za-z0-9_./-]+\.h)")
SENTENCE_ENDINGS = "。！？!?"
COMMAS = "，,"
EFFECTIVE_PREFIX_PUNCTUATION = SENTENCE_ENDINGS + COMMAS + "；;"


@dataclass(frozen=True)
class DescriptionAnalysis:
    raw_first_prose_block: Optional[str]
    raw_first_block_is_header_directive: bool
    raw_first_block_has_formula_intro: bool
    description_source: Optional[str]
    raw_description_block: Optional[str]
    description: Optional[str]


def _is_description_block_boundary(line: str) -> bool:
    stripped = line.strip()
    return (
        not stripped
        or stripped == "$$"
        or HEADING_RE.match(line) is not None
        or FENCE_RE.match(line) is not None
        or stripped.startswith(("|", "!", ">", "<!--"))
        or LIST_RE.match(line) is not None
    )


def _first_non_empty_index(lines: Sequence[str], start: int) -> Optional[int]:
    for index in range(start, len(lines)):
        if lines[index].strip():
            return index
    return None


def _prose_blocks_below_function_section(markdown_text: str) -> Sequence[str]:
    lines = markdown_text.splitlines()
    section_start = None
    for index, line in enumerate(lines):
        if FUNCTION_SECTION_RE.match(line):
            section_start = index + 1
            break
    if section_start is None:
        return ()

    blocks = []
    cursor = section_start
    while cursor < len(lines):
        first_line_index = _first_non_empty_index(lines, cursor)
        if first_line_index is None:
            break
        first_line = lines[first_line_index]
        if HEADING_RE.match(first_line) is not None:
            break
        if _is_description_block_boundary(first_line):
            break

        paragraph = []
        cursor = first_line_index
        while (
            cursor < len(lines)
            and not _is_description_block_boundary(lines[cursor])
        ):
            paragraph.append(lines[cursor].strip())
            cursor += 1
        block = re.sub(r"\s+", " ", " ".join(paragraph)).strip()
        if block:
            blocks.append(block)
    return tuple(blocks)


def _reference_description_block(markdown_text: str) -> tuple[Optional[str], Optional[str]]:
    """Return the first prose block from a non-function reference page."""
    lines = markdown_text.splitlines()
    title_index = next(
        (
            index
            for index, line in enumerate(lines)
            if re.match(r"^\s{0,3}#(?!#)\s+", line)
        ),
        None,
    )
    if title_index is None:
        return None, None

    source = "reference_title_prose"
    can_select = True
    cursor = title_index + 1
    while cursor < len(lines):
        line = lines[cursor]
        if HEADING_RE.match(line) is not None:
            heading_match = re.match(r"^\s{0,3}(#{1,6})\s+(.+?)\s*$", line)
            if heading_match is None:
                cursor += 1
                continue
            level = len(heading_match.group(1))
            if level <= 2:
                heading = HEADING_ANCHOR_RE.sub("", heading_match.group(2)).strip()
                heading = HEADING_PARENTHETICAL_RE.sub("", heading).rstrip("：:").strip()
                can_select = level == 2 and heading not in REFERENCE_DESCRIPTION_SKIP_HEADINGS
                source = "reference_section_prose"
            cursor += 1
            continue
        if not can_select or not line.strip():
            cursor += 1
            continue
        if _is_description_block_boundary(line):
            cursor += 1
            continue

        paragraph = []
        while cursor < len(lines) and not _is_description_block_boundary(lines[cursor]):
            paragraph.append(lines[cursor].strip())
            cursor += 1
        block = re.sub(r"\s+", " ", " ".join(paragraph)).strip()
        if block and not HEADER_DIRECTIVE_RE.match(block):
            return block, source
    return None, None


def _inline_description_after_header_directive(block: str) -> Optional[str]:
    path_matches = list(HEADER_PATH_RE.finditer(block))
    if not path_matches:
        return None

    trailing_text = block[path_matches[-1].end() :]
    sentence_match = re.search(r"[。.]\s*(\S.*)$", trailing_text)
    if sentence_match is None:
        return None
    return sentence_match.group(1).strip()


def _normalize_terminal_punctuation(description: str) -> str:
    """Return a standalone sentence from a source-derived prose block."""
    if description[-1] in SENTENCE_ENDINGS:
        return description

    last_sentence_end = max(description.rfind(mark) for mark in SENTENCE_ENDINGS)
    if last_sentence_end >= 0:
        return description[: last_sentence_end + 1]

    last_comma = max(description.rfind(mark) for mark in COMMAS)
    if last_comma >= 0:
        return f"{description[:last_comma].rstrip()}。"

    return f"{description}。"


def _trim_formula_intro(description: str) -> str:
    """Remove formula-introduction text and retain the preceding prose."""
    formula_intro = FORMULA_INTRO_RE.search(description)
    if formula_intro is None:
        return description

    prefix = description[: formula_intro.start()].rstrip()
    last_delimiter = max(
        prefix.rfind(mark) for mark in EFFECTIVE_PREFIX_PUNCTUATION
    )
    if last_delimiter < 0:
        return prefix

    delimiter = prefix[last_delimiter]
    prefix = prefix[:last_delimiter].rstrip()
    if delimiter in SENTENCE_ENDINGS:
        return f"{prefix}{delimiter}"
    return prefix


def analyze_description(markdown_text: str) -> DescriptionAnalysis:
    """Analyze raw prose blocks and extract the first semantic description."""
    blocks = _prose_blocks_below_function_section(markdown_text)
    raw_first_prose_block = blocks[0] if blocks else None
    raw_first_block_is_header_directive = bool(
        raw_first_prose_block
        and HEADER_DIRECTIVE_RE.match(raw_first_prose_block)
    )
    raw_first_block_has_formula_intro = bool(
        raw_first_prose_block
        and FORMULA_INTRO_SUFFIX_RE.search(raw_first_prose_block)
    )
    description_source = None
    raw_description_block = raw_first_prose_block

    if raw_first_block_is_header_directive:
        inline_description = _inline_description_after_header_directive(
            raw_first_prose_block
        )
        if inline_description:
            raw_description_block = inline_description
            description_source = "header_directive_inline_description"
        elif len(blocks) > 1:
            raw_description_block = blocks[1]
            description_source = "after_header_directive"
        else:
            raw_description_block = None
    elif raw_description_block is not None:
        description_source = "first_prose_block"
    elif not blocks:
        raw_description_block, description_source = _reference_description_block(
            markdown_text
        )
        raw_first_prose_block = raw_description_block

    description = _trim_formula_intro(raw_description_block) if raw_description_block else None
    return DescriptionAnalysis(
        raw_first_prose_block=raw_first_prose_block,
        raw_first_block_is_header_directive=raw_first_block_is_header_directive,
        raw_first_block_has_formula_intro=raw_first_block_has_formula_intro,
        description_source=description_source,
        raw_description_block=raw_description_block,
        description=(
            _normalize_terminal_punctuation(description) if description else None
        ),
    )


def extract_description(markdown_text: str) -> Optional[str]:
    """Return the normalized first semantic prose block below ``## 功能说明``."""
    return analyze_description(markdown_text).description


def extract_file_description(path: Path) -> Optional[str]:
    return extract_description(path.read_text(encoding="utf-8"))


def _normalized_api_headings(markdown_text: str) -> set[str]:
    """Return normalized API-structure headings at any section level."""
    headings = set()
    for line in markdown_text.splitlines():
        match = re.match(r"^\s{0,3}#{2,6}\s+(.+?)\s*$", line)
        if match is None:
            continue
        heading = HEADING_ANCHOR_RE.sub("", match.group(1)).strip()
        heading = HEADING_PARENTHETICAL_RE.sub("", heading).rstrip("：:").strip()
        heading = FUNCTION_API_HEADING_ALIASES.get(heading, heading)
        if heading:
            headings.add(heading)
    return headings


def _document_title(markdown_text: str) -> str:
    for line in markdown_text.splitlines():
        match = re.match(r"^#\s+(.+?)\s*$", line)
        if match is not None:
            return match.group(1)
    return ""


def is_markdown_link_list(markdown_text: str) -> bool:
    """Return whether a document is a pure Markdown list of local .md links."""
    content_lines = [
        line
        for line in markdown_text.splitlines()
        if line.strip() and HEADING_RE.match(line) is None
    ]
    return bool(content_lines) and all(
        LIST_RE.match(line) is not None
        and MARKDOWN_LINK_RE.search(line) is not None
        for line in content_lines
    )


def classify_api_document(markdown_text: str) -> str:
    """Classify an API-tree Markdown file using only its own title and headings."""
    title = _document_title(markdown_text)
    headings = _normalized_api_headings(markdown_text)
    has_function_prototype = any(
        heading.startswith("函数原型") for heading in headings
    )
    function_structure_count = len(headings & FUNCTION_API_STRUCTURE_HEADINGS)

    if has_function_prototype and function_structure_count >= 4:
        return "function_api"
    if ({"枚举类具体定义", "枚举值详解"} <= headings or
            {"枚举具体定义", "枚举值详解"} <= headings):
        return "enum_api"
    if (
        {"结构体具体定义", "联合体具体定义"} & headings
        and "字段详解" in headings
    ):
        return "record_api"
    if "常量定义" in headings:
        return "constant_api"
    if "类型别名具体定义" in headings:
        return "type_alias_api"
    if "宏定义" in headings:
        return "macro_api"
    if "数据类型" in title and ("数据类型" in headings or len(headings) >= 2):
        return "type_reference"
    if is_markdown_link_list(markdown_text):
        return "index_page"

    # API-like but nonconforming pages must be reviewed rather than skipped.
    if (
        has_function_prototype
        or API_LIKE_TITLE_RE.match(title)
        or API_DECLARATION_RE.search(markdown_text)
    ):
        return "unclassified"
    return "overview_page"


def is_api_reference_document(markdown_text: str) -> bool:
    """Return whether a document is a concrete API entity, based on its content."""
    return classify_api_document(markdown_text) in CONCRETE_API_DOCUMENT_KINDS


def _collect_api_files(directory: Path) -> Sequence[Path]:
    return sorted(
        path
        for path in directory.rglob("*.md")
        if path.is_file()
        and is_api_reference_document(path.read_text(encoding="utf-8"))
    )


def _build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=(
            "Extract the first source-derived description below "
            "'## 功能说明' from Markdown API documents."
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
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = _build_parser().parse_args(argv)
    directory = args.directory.resolve()
    if not directory.is_dir():
        print(f"error: directory does not exist: {directory}", file=sys.stderr)
        return 2

    api_files = _collect_api_files(directory)
    if not api_files:
        print(
            "error: no API documents with a '## 函数原型' section found in: "
            f"{directory}",
            file=sys.stderr,
        )
        return 1

    for path in api_files:
        description = extract_file_description(path)
        value = description if description else "<未提取到功能说明>"
        print(f"- {path.name}: {value}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
