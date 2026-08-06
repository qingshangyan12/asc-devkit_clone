#!/usr/bin/env python3

import argparse
import re
import sys
from pathlib import Path


HEADING_RE = re.compile(r"^#{3,4} (模板原型（占位符形式）|典型示例)$")
DECLARATION_RE = re.compile(r"\basc_[A-Za-z0-9_]+\s*\(")


def collect_markdown_files(paths):
    files = []
    for raw_path in paths:
        path = Path(raw_path)
        if path.is_dir():
            files.extend(sorted(path.rglob("*.md")))
        elif path.is_file() and path.suffix == ".md":
            files.append(path)
        else:
            raise ValueError(f"not a Markdown file or directory: {path}")
    return sorted(set(files))


def split_params(text):
    params = []
    start = 0
    depths = {"angle": 0, "paren": 0, "bracket": 0, "brace": 0}
    pairs = {
        "<": ("angle", 1),
        ">": ("angle", -1),
        "(": ("paren", 1),
        ")": ("paren", -1),
        "[": ("bracket", 1),
        "]": ("bracket", -1),
        "{": ("brace", 1),
        "}": ("brace", -1),
    }
    for index, char in enumerate(text):
        if char in pairs:
            name, delta = pairs[char]
            depths[name] = max(0, depths[name] + delta)
        elif char == "," and all(depth == 0 for depth in depths.values()):
            params.append(text[start:index].strip())
            start = index + 1
    params.append(text[start:].strip())
    return [param for param in params if param]


def format_declaration(lines):
    joined = " ".join(line.strip() for line in lines)
    joined = re.sub(r"\s+", " ", joined)
    declaration_match = DECLARATION_RE.search(joined)
    if declaration_match is None:
        raise ValueError("cannot locate the Ascend C API function name")
    open_index = joined.find("(", declaration_match.start())
    close_index = joined.rfind(")")
    if open_index < 0 or close_index < open_index:
        raise ValueError("cannot locate the declaration parameter list")

    prefix = joined[: open_index + 1]
    params = split_params(joined[open_index + 1 : close_index])
    suffix = joined[close_index + 1 :]
    if len(params) <= 1:
        return [f"{prefix}{params[0] if params else ''}){suffix}"]

    indent = " " * len(prefix)
    formatted = []
    for index, param in enumerate(params):
        line_prefix = prefix if index == 0 else indent
        line_suffix = f"){suffix}" if index == len(params) - 1 else ","
        formatted.append(f"{line_prefix}{param}{line_suffix}")
    return formatted


def check_block(path, lines, start_line):
    errors = []
    index = 0
    while index < len(lines):
        if DECLARATION_RE.search(lines[index]) is None:
            index += 1
            continue

        declaration_start = index
        balance = 0
        saw_open = False
        declaration = []
        while index < len(lines):
            line = lines[index]
            declaration.append(line)
            balance += line.count("(") - line.count(")")
            saw_open = saw_open or "(" in line
            index += 1
            if saw_open and balance == 0:
                break

        line_number = start_line + declaration_start
        if not saw_open or balance != 0:
            errors.append(f"{path}:{line_number}: unbalanced template declaration")
            continue
        try:
            expected = format_declaration(declaration)
        except ValueError as error:
            errors.append(f"{path}:{line_number}: {error}")
            continue
        if declaration != expected:
            errors.append(
                f"{path}:{line_number}: parameters must occupy one line each and align with the first parameter"
            )
    return errors


def check_file(path):
    lines = path.read_text(encoding="utf-8").splitlines()
    errors = []
    index = 0
    while index < len(lines):
        if not HEADING_RE.match(lines[index]):
            index += 1
            continue

        fence_index = index + 1
        while fence_index < len(lines) and not lines[fence_index].strip():
            fence_index += 1
        if fence_index >= len(lines) or lines[fence_index].strip() != "```cpp":
            errors.append(f"{path}:{index + 1}: prototype or example heading must be followed by a cpp code block")
            index += 1
            continue

        block_start = fence_index + 1
        fence_end = block_start
        while fence_end < len(lines) and lines[fence_end].strip() != "```":
            fence_end += 1
        if fence_end >= len(lines):
            errors.append(f"{path}:{fence_index + 1}: unclosed template code block")
            break
        errors.extend(check_block(path, lines[block_start:fence_end], block_start + 1))
        index = fence_end + 1
    return errors


def main():
    parser = argparse.ArgumentParser(
        description="Check parameter wrapping in template prototype and typical-example code blocks."
    )
    parser.add_argument("paths", nargs="+", help="Markdown files or directories to check")
    args = parser.parse_args()

    try:
        files = collect_markdown_files(args.paths)
    except ValueError as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 2

    errors = []
    for path in files:
        errors.extend(check_file(path))
    if errors:
        print("\n".join(errors), file=sys.stderr)
        return 1

    print(f"PASS: prototype and typical-example format in {len(files)} Markdown file(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
