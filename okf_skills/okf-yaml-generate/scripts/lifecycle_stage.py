#!/usr/bin/env python3
"""Collect prototype lifecycle facts from unpacked CANN packages and release notes.

This stage only produces JSON evidence. It does not install packages or edit API
documents. Package arguments use ``VERSION=PATH`` and PATH must be an unpacked
CANN tree (or a directory containing its headers).
"""

from __future__ import annotations

import argparse
from collections import defaultdict
import json
from pathlib import Path
import re
from typing import Any, Iterable


VERSION_RE = re.compile(r"(?<!\d)(\d+[._]\d+(?:[._]\d+)?)(?!\d)")
PACKAGE_ARG_RE = re.compile(r"^(?P<version>\d+\.\d+(?:\.\d+)?)=(?P<path>.+)$")
MACRO_RE = re.compile(r"^\s*#\s*define\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?:\((?P<params>[^)]*)\))?", re.MULTILINE)
CALLABLE_RE = re.compile(
    r"(?m)(?<![A-Za-z0-9_])(?P<name>asc_[A-Za-z0-9_]*)\s*"
    r"\((?P<params>[^;{}()]*(?:\([^)]*\)[^;{}()]*)*)\)\s*;"
)
DEPRECATED_SECTION_RE = re.compile(r"(?ms)^##\s+接口废弃\s*(.*?)(?=^##\s+|\Z)")
BACKTICK_TOKEN_RE = re.compile(r"`(?P<token>[^`]+)`")
SYMBOL_RE = re.compile(r"(?<![A-Za-z0-9_])(?P<symbol>asc_[A-Za-z0-9_]*)")
PROTOTYPE_RE = re.compile(
    r"(?P<symbol>asc_[A-Za-z0-9_]*)\((?P<params>[^()]*)\)"
)


class LifecycleError(ValueError):
    """Raised when lifecycle collection input is invalid."""


def version_key(value: str) -> tuple[int, ...]:
    return tuple(int(part) for part in value.split("."))


def extract_version(value: str) -> str | None:
    match = VERSION_RE.search(value)
    return match.group(1).replace("_", ".") if match else None


def normalize_signature(name: str, params: str | None) -> str:
    if params is None:
        return name
    compact = re.sub(r"\s+", " ", params.strip())
    compact = re.sub(r"\s*([,*&<>()[\]])\s*", r"\1", compact)
    return f"{name}({compact})"


def prototype_details(token: str) -> tuple[str, str | None, int | None]:
    match = PROTOTYPE_RE.search(token)
    if match is None:
        symbol_match = SYMBOL_RE.search(token)
        return (symbol_match.group("symbol"), None, None) if symbol_match else ("", None, None)
    params = match.group("params").strip()
    if not params or params == "...":
        return match.group("symbol"), None, None
    count = parameter_count(params)
    return match.group("symbol"), normalize_signature(match.group("symbol"), params), count


def parameter_count(params: str) -> int:
    count = 1
    angle_depth = 0
    paren_depth = 0
    for char in params:
        if char == "<":
            angle_depth += 1
        elif char == ">" and angle_depth:
            angle_depth -= 1
        elif char == "(" and not angle_depth:
            paren_depth += 1
        elif char == ")" and paren_depth:
            paren_depth -= 1
        elif char == "," and angle_depth == 0 and paren_depth == 0:
            count += 1
    return count


def release_targets(line: str) -> list[tuple[str, str | None, int | None]]:
    """Read only the deprecated side of a release-note row."""
    cells = [item.strip() for item in line.split("|")]
    target = cells[1] if line.lstrip().startswith("|") and len(cells) > 1 else line
    result: list[tuple[str, str | None, int | None]] = []
    seen: set[tuple[str, str | None, int | None]] = set()
    prototype_symbols: set[str] = set()
    tokens = [match.group("token") for match in BACKTICK_TOKEN_RE.finditer(target)]
    tokens.extend(match.group(0) for match in PROTOTYPE_RE.finditer(target))
    for token in tokens:
        details = prototype_details(token)
        if details[0] and details not in seen:
            seen.add(details)
            result.append(details)
            if details[1] is not None:
                prototype_symbols.add(details[0])
    for match in SYMBOL_RE.finditer(target):
        symbol = match.group("symbol")
        if symbol in prototype_symbols:
            continue
        details = (symbol, None, None)
        if details not in seen:
            seen.add(details)
            result.append(details)
    return result


def iter_headers(root: Path) -> Iterable[Path]:
    include = root / "include"
    search_root = include if include.is_dir() else root
    yield from sorted(path for path in search_root.rglob("*") if path.suffix.lower() in {".h", ".hpp"})


def scan_package(version: str, root: Path) -> list[dict[str, str]]:
    if not root.is_dir():
        raise LifecycleError(f"package path is not a directory: {root}")
    found: dict[str, dict[str, str]] = {}
    for header in iter_headers(root):
        text = header.read_text(encoding="utf-8", errors="replace")
        for match in MACRO_RE.finditer(text):
            name = match.group("name")
            if not name.startswith("ASC_") and not name.startswith("asc_"):
                continue
            identity = normalize_signature(name, match.group("params"))
            found.setdefault(identity, {
                "identity": identity,
                "symbol": name,
                "kind": "macro",
                "param_count": str(parameter_count(match.group("params") or "")) if match.group("params") else "0",
            })
        for match in CALLABLE_RE.finditer(text):
            name = match.group("name")
            identity = normalize_signature(name, match.group("params"))
            found.setdefault(identity, {
                "identity": identity,
                "symbol": name,
                "kind": "function",
                "param_count": str(parameter_count(match.group("params"))),
            })
    return [{**item, "version": version} for item in found.values()]


def release_events(paths: Iterable[Path]) -> dict[str, dict[str, list[dict[str, str]]]]:
    events: dict[str, dict[str, list[dict[str, str]]]] = defaultdict(lambda: defaultdict(list))
    for path in paths:
        version = extract_version(path.stem)
        if version is None:
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        section = DEPRECATED_SECTION_RE.search(text)
        if section is None:
            continue
        for line in section.group(1).splitlines():
            targets = release_targets(line)
            if not targets:
                continue
            for symbol, prototype, param_count in targets:
                events[symbol]["deprecated"].append({
                    "version": version,
                    "source_path": path.as_posix(),
                    "excerpt": line.strip(),
                    **({"prototype": prototype} if prototype else {}),
                    **({"param_count": str(param_count)} if param_count is not None else {}),
                })
    return events


def collect(packages: list[tuple[str, Path]], notes: Iterable[Path]) -> dict[str, Any]:
    package_records: dict[str, list[dict[str, str]]] = defaultdict(list)
    for version, root in packages:
        for record in scan_package(version, root):
            package_records[record["identity"]].append(record)
    events = release_events(notes)
    output: list[dict[str, Any]] = []
    for identity in sorted(package_records):
        records = package_records[identity]
        symbol = records[0]["symbol"]
        versions = sorted({item["version"] for item in records}, key=version_key)
        candidates = events.get(symbol, {}).get("deprecated", [])
        param_count = int(records[0].get("param_count", "0"))
        deprecated = [
            item for item in candidates
            if (
                item.get("prototype") == identity
                or (item.get("param_count") == str(param_count) and param_count > 0)
                or ("prototype" not in item and "param_count" not in item)
            )
        ]
        deprecated = sorted(deprecated, key=lambda item: version_key(item["version"]))
        start = versions[0] if versions else None
        end = deprecated[0]["version"] if deprecated else None
        lifecycle = "unknown" if start is None else f"{start}-{end or ''}"
        output.append({
            "identity": identity,
            "symbol": symbol,
            "kind": records[0]["kind"],
            "version": lifecycle,
            "package_versions": versions,
            "release_events": deprecated,
        })
    return {"schema_version": 1, "records": output}


def parse_package(value: str) -> tuple[str, Path]:
    match = PACKAGE_ARG_RE.fullmatch(value)
    if match is None:
        raise LifecycleError("--package must use VERSION=PATH")
    return match.group("version"), Path(match.group("path")).expanduser().resolve()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--package", action="append", default=[], help="VERSION=unpacked-CANN-path")
    parser.add_argument("--release-notes", action="append", default=[], help="release note Markdown path")
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    packages = [parse_package(item) for item in args.package]
    if not packages:
        raise LifecycleError("at least one --package is required")
    result = collect(packages, [Path(item).expanduser().resolve() for item in args.release_notes])
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"records": len(result["records"]), "output": str(args.output)}, ensure_ascii=False))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except LifecycleError as exc:
        raise SystemExit(f"error: {exc}")
