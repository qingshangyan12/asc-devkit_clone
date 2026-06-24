#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


MARKDOWN_EXTS = (".md", ".markdown", ".mdx")
FENCE_RE = re.compile(r"^(?P<indent>[ \t]*)(?P<fence>`{3,}|~{3,})(?P<info>.*)$")
FORMAT_LANGS = {
    "c": ".c",
    "cc": ".cc",
    "cpp": ".cpp",
    "c++": ".cpp",
    "cxx": ".cxx",
    "h": ".h",
    "hh": ".hh",
    "hpp": ".hpp",
    "h++": ".hpp",
    "hxx": ".hxx",
    "asc": ".asc",
}
TEXT_LANGS = {
    "text",
    "txt",
    "plain",
    "plaintext",
    "console",
    "terminal",
    "shell-session",
    "output",
    "log",
    "none",
}
KNOWN_LANGS = FORMAT_LANGS.keys() | TEXT_LANGS | {
    "bash",
    "sh",
    "shell",
    "cmake",
    "python",
    "py",
    "json",
    "jsonc",
    "yaml",
    "yml",
    "toml",
    "ini",
    "xml",
    "html",
    "md",
    "markdown",
    "diff",
    "patch",
    "dot",
}


@dataclass(frozen=True)
class CodeBlock:
    path: Path
    start_line: int
    body_start: int
    body_end: int
    lang: str
    code: str


@dataclass(frozen=True)
class Replacement:
    body_start: int
    body_end: int
    text: str


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Check fenced code blocks in Markdown files.")
    parser.add_argument("--full", "--all", action="store_true", dest="full", help="scan all tracked Markdown files")
    parser.add_argument("files", nargs="*", help="Markdown files to check")
    return parser.parse_args(argv)


def repo_root() -> Path:
    proc = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"],
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    )
    if proc.returncode == 0:
        return Path(proc.stdout.strip())
    return Path.cwd()


def to_rel_posix(path: str, root: Path) -> str | None:
    p = Path(path)
    if p.is_absolute():
        try:
            return p.resolve().relative_to(root.resolve()).as_posix()
        except ValueError:
            return None
    return PurePosixPath(path).as_posix()


def is_markdown(path: str) -> bool:
    return path.lower().endswith(MARKDOWN_EXTS)


def tracked_markdown_files(root: Path) -> list[Path]:
    proc = subprocess.run(
        ["git", "ls-files", "*.md", "*.markdown", "*.mdx"],
        cwd=root,
        check=True,
        stdout=subprocess.PIPE,
        text=True,
    )
    return [root / line for line in proc.stdout.splitlines() if line]


def markdown_files_under(path: Path) -> list[Path]:
    if path.is_file():
        return [path] if is_markdown(path.as_posix()) else []
    if path.is_dir():
        return sorted(child for child in path.rglob("*") if child.is_file() and is_markdown(child.as_posix()))
    return []


def input_markdown_files(files: list[str], root: Path, full: bool) -> list[Path]:
    if full or os.environ.get("MARKDOWN_CODE_BLOCK_CHECK_FULL") == "1":
        return tracked_markdown_files(root)

    paths: list[Path] = []
    seen: set[str] = set()
    for raw in files:
        raw_path = Path(raw)
        if raw_path.is_absolute():
            path = raw_path
        else:
            rel = to_rel_posix(raw, root)
            if rel is None:
                continue
            path = root / rel
        for markdown_path in markdown_files_under(path):
            key = markdown_path.as_posix()
            if key not in seen:
                paths.append(markdown_path)
                seen.add(key)
    return paths


def normalize_lang(info: str) -> str:
    parts = info.strip().split(maxsplit=1)
    if not parts:
        return ""
    return parts[0].lower()


def closing_fence(line: str, opener: str) -> bool:
    match = FENCE_RE.match(line.rstrip("\n"))
    if not match:
        return False
    fence = match.group("fence")
    return fence[0] == opener[0] and len(fence) >= len(opener)


def iter_code_blocks(path: Path) -> tuple[list[CodeBlock], list[str]]:
    errors: list[str] = []
    try:
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    except UnicodeDecodeError:
        errors.append(f"{path}: failed to decode file as UTF-8")
        return [], errors
    except OSError as exc:
        errors.append(f"{path}: failed to read file: {exc}")
        return [], errors

    blocks: list[CodeBlock] = []
    i = 0
    while i < len(lines):
        match = FENCE_RE.match(lines[i].rstrip("\n"))
        if not match:
            i += 1
            continue

        opener = match.group("fence")
        start_line = i + 1
        lang = normalize_lang(match.group("info"))
        body_start = i + 1
        i = body_start
        while i < len(lines) and not closing_fence(lines[i], opener):
            i += 1

        code = "".join(lines[body_start:i])
        if not lang:
            errors.append(f"{path}:{start_line}: fenced code block must specify a language, use text for plain output")
        elif lang not in KNOWN_LANGS:
            errors.append(f"{path}:{start_line}: unsupported fenced code block language: {lang}")
        else:
            blocks.append(
                CodeBlock(path=path, start_line=start_line, body_start=body_start, body_end=i, lang=lang, code=code)
            )

        if i < len(lines):
            i += 1

    return blocks, errors


def clang_format_bin() -> str | None:
    configured = os.environ.get("MARKDOWN_CODE_BLOCK_CHECK_CLANG_FORMAT")
    if configured:
        return configured
    return shutil.which("clang-format")


def format_code(code: str, lang: str, clang: str, tmp_dir: Path, index: int) -> tuple[str | None, str | None]:
    suffix = FORMAT_LANGS[lang]
    code_file = tmp_dir / f"block_{index}{suffix}"
    code_file.write_text(code, encoding="utf-8")
    proc = subprocess.run(
        [clang, "--style=file", "-i", str(code_file)],
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    if proc.returncode != 0:
        return None, (proc.stderr or proc.stdout).strip()
    return code_file.read_text(encoding="utf-8"), None


def format_markdown_file(path: Path, blocks: list[CodeBlock], clang: str, start_index: int) -> tuple[bool, list[str], int]:
    try:
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    except OSError as exc:
        return False, [f"{path}: failed to read file: {exc}"], start_index

    replacements: list[Replacement] = []
    errors: list[str] = []
    index = start_index
    with tempfile.TemporaryDirectory(prefix="markdown-code-block-") as tmp:
        tmp_dir = Path(tmp)
        for block in blocks:
            formatted, error = format_code(block.code, block.lang, clang, tmp_dir, index)
            index += 1
            if error is not None:
                errors.append(f"{block.path}:{block.start_line}: failed to format {block.lang} code block")
                errors.append(error)
                continue
            if formatted is not None and formatted != block.code:
                replacements.append(Replacement(block.body_start, block.body_end, formatted))

    for replacement in sorted(replacements, key=lambda item: item.body_start, reverse=True):
        lines[replacement.body_start:replacement.body_end] = replacement.text.splitlines(keepends=True)

    if replacements:
        path.write_text("".join(lines), encoding="utf-8")
    return bool(replacements), errors, index


def format_blocks(blocks: list[CodeBlock]) -> tuple[list[str], list[Path]]:
    targets = [block for block in blocks if block.lang in FORMAT_LANGS]
    if not targets:
        return [], []

    clang = clang_format_bin()
    if clang is None:
        return ["clang-format not found; install clang-format or set MARKDOWN_CODE_BLOCK_CHECK_CLANG_FORMAT"], []

    errors: list[str] = []
    changed_files: list[Path] = []
    index = 0
    for path in dict.fromkeys(block.path for block in targets):
        path_blocks = [block for block in targets if block.path == path]
        changed, file_errors, index = format_markdown_file(path, path_blocks, clang, index)
        errors.extend(file_errors)
        if changed:
            changed_files.append(path)
    return errors, changed_files


def main(argv: list[str] | None = None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    root = repo_root()
    files = input_markdown_files(args.files, root, args.full)
    if not files:
        return 0

    errors: list[str] = []
    blocks: list[CodeBlock] = []
    for path in files:
        file_blocks, file_errors = iter_code_blocks(path)
        blocks.extend(file_blocks)
        errors.extend(file_errors)
    format_errors, changed_files = format_blocks(blocks)
    errors.extend(format_errors)
    for path in changed_files:
        errors.append(f"{path}: formatted fenced code blocks")

    if errors:
        print("[Markdown code block check] Found issues:", file=sys.stderr)
        for error in errors:
            print(error, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
