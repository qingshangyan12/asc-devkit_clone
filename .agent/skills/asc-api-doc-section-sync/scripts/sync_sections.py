#!/usr/bin/env python3
"""Safely replace selected level-two sections in a Markdown document."""

from __future__ import annotations

import argparse
import difflib
import os
import re
import stat
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


DEFAULT_SECTIONS = ("功能说明", "函数原型", "参数说明")
HEADING_RE = re.compile(r"^##(?!#)[ \t]+(.+?)[ \t]*$")
FENCE_RE = re.compile(r"^[ \t]*(`{3,}|~{3,})")


@dataclass(frozen=True)
class Document:
    text: str
    newline: str
    has_bom: bool


@dataclass(frozen=True)
class Section:
    title: str
    start: int
    end: int


def read_document(path: Path) -> Document:
    data = path.read_bytes()
    has_bom = data.startswith(b"\xef\xbb\xbf")
    try:
        text = data.decode("utf-8-sig")
    except UnicodeDecodeError as error:
        raise ValueError(f"{path}: 文件不是有效的UTF-8文本：{error}") from error

    newline = "\r\n" if b"\r\n" in data else "\n"
    normalized = text.replace("\r\n", "\n").replace("\r", "\n")
    return Document(normalized, newline, has_bom)


def find_level_two_sections(text: str, path: Path) -> dict[str, Section]:
    lines = text.splitlines(keepends=True)
    headings: list[tuple[str, int]] = []
    fence_marker: str | None = None

    for index, line in enumerate(lines):
        stripped = line.rstrip("\n")
        fence_match = FENCE_RE.match(stripped)
        if fence_match:
            marker = fence_match.group(1)[0]
            if fence_marker is None:
                fence_marker = marker
            elif marker == fence_marker:
                fence_marker = None
            continue
        if fence_marker is not None:
            continue

        heading_match = HEADING_RE.match(stripped)
        if heading_match:
            headings.append((heading_match.group(1).strip(), index))

    sections: dict[str, Section] = {}
    for position, (title, start) in enumerate(headings):
        if title in sections:
            raise ValueError(f"{path}: 二级章节重复：{title}")
        end = headings[position + 1][1] if position + 1 < len(headings) else len(lines)
        sections[title] = Section(title, start, end)
    return sections


def validate_requested_sections(
    requested: list[str], source_sections: dict[str, Section], target_sections: dict[str, Section], source: Path, target: Path
) -> None:
    if len(requested) != len(set(requested)):
        raise ValueError("--sections中存在重复章节名")
    for title in requested:
        if title not in source_sections:
            raise ValueError(f"{source}: 缺少二级章节：{title}")
        if title not in target_sections:
            raise ValueError(f"{target}: 缺少二级章节：{title}")

    default_in_request = [title for title in DEFAULT_SECTIONS if title in requested]
    source_order = sorted(default_in_request, key=lambda title: source_sections[title].start)
    target_order = sorted(default_in_request, key=lambda title: target_sections[title].start)
    if source_order != default_in_request:
        raise ValueError(f"{source}: 默认章节顺序错误，应为：{'、'.join(default_in_request)}")
    if target_order != default_in_request:
        raise ValueError(f"{target}: 默认章节顺序错误，应为：{'、'.join(default_in_request)}")


def normalized_section_block(lines: list[str], section: Section) -> list[str]:
    block = "".join(lines[section.start : section.end]).rstrip("\n")
    return (block + "\n\n").splitlines(keepends=True)


def replace_sections(source: Document, target: Document, source_path: Path, target_path: Path, requested: list[str]) -> str:
    source_sections = find_level_two_sections(source.text, source_path)
    target_sections = find_level_two_sections(target.text, target_path)
    validate_requested_sections(requested, source_sections, target_sections, source_path, target_path)

    source_lines = source.text.splitlines(keepends=True)
    target_lines = target.text.splitlines(keepends=True)
    requested_by_start = sorted(requested, key=lambda title: target_sections[title].start)

    result: list[str] = []
    cursor = 0
    for title in requested_by_start:
        target_section = target_sections[title]
        result.extend(target_lines[cursor : target_section.start])
        result.extend(normalized_section_block(source_lines, source_sections[title]))
        cursor = target_section.end
    result.extend(target_lines[cursor:])
    return "".join(result)


def write_document(path: Path, document: Document, normalized_text: str) -> None:
    output = normalized_text.replace("\n", document.newline).encode("utf-8")
    if document.has_bom:
        output = b"\xef\xbb\xbf" + output

    mode = stat.S_IMODE(path.stat().st_mode)
    temp_name: str | None = None
    try:
        with tempfile.NamedTemporaryFile(dir=path.parent, prefix=f".{path.name}.", delete=False) as temp_file:
            temp_file.write(output)
            temp_name = temp_file.name
        os.chmod(temp_name, mode)
        os.replace(temp_name, path)
    finally:
        if temp_name is not None and os.path.exists(temp_name):
            os.unlink(temp_name)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", required=True, type=Path, help="提供新章节内容的Markdown文档")
    parser.add_argument("--target", required=True, type=Path, help="需要更新的正式Markdown文档")
    parser.add_argument(
        "--sections",
        nargs="+",
        default=list(DEFAULT_SECTIONS),
        help="需要替换的二级章节名（默认：功能说明 函数原型 参数说明）",
    )
    action = parser.add_mutually_exclusive_group()
    action.add_argument("--write", action="store_true", help="写入目标文档；默认只输出差异")
    action.add_argument("--check", action="store_true", help="检查目标是否已同步；存在差异时返回1")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    target_is_symlink = args.target.is_symlink()
    source = args.source.resolve()
    target = args.target.resolve()

    try:
        if source == target:
            raise ValueError("源文档和目标文档不能是同一个文件")
        if not source.is_file():
            raise ValueError(f"源文档不存在：{source}")
        if not target.is_file():
            raise ValueError(f"目标文档不存在：{target}")
        if target_is_symlink:
            raise ValueError(f"目标文档不能是软链接：{args.target}")

        source_document = read_document(source)
        target_document = read_document(target)
        updated_text = replace_sections(source_document, target_document, source, target, args.sections)
    except (OSError, ValueError) as error:
        print(f"错误：{error}", file=sys.stderr)
        return 2

    if updated_text == target_document.text:
        print(f"已同步，无需修改：{target}")
        return 0

    diff = "".join(
        difflib.unified_diff(
            target_document.text.splitlines(keepends=True),
            updated_text.splitlines(keepends=True),
            fromfile=str(target),
            tofile=f"{target}（同步后）",
        )
    )

    if args.check:
        print(diff, end="")
        print(f"未同步章节：{'、'.join(args.sections)}", file=sys.stderr)
        return 1
    if args.write:
        try:
            write_document(target, target_document, updated_text)
        except OSError as error:
            print(f"错误：写入失败：{error}", file=sys.stderr)
            return 2
        print(f"已替换章节：{'、'.join(args.sections)}")
        print(f"目标文档：{target}")
        return 0

    print(diff, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
