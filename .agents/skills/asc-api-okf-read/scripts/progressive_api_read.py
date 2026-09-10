#!/usr/bin/env python3
"""Read Ascend API documentation through bounded progressive-disclosure layers."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import sys
from typing import Any

import yaml


HARD_MAX_CHARS = 12_000
HARD_MAX_LINES = 200
API_ROOT = Path("docs/zh/api/SIMD-API/c_api")
HEADING_RE = re.compile(r"^\s{0,3}(#{1,6})\s+(.+?)\s*$")
FENCE_RE = re.compile(r"^\s{0,3}(`{3,}|~{3,})")
DOXYGEN_RE = re.compile(r"/\*\*.*?\*/", re.DOTALL)


class ReaderError(Exception):
    def __init__(self, code: str, message: str, **details: Any) -> None:
        super().__init__(message)
        self.code = code
        self.message = message
        self.details = details


def _default_repo_root() -> Path:
    return Path(__file__).resolve().parents[4]


def _within(path: Path, root: Path) -> bool:
    try:
        path.relative_to(root)
    except ValueError:
        return False
    return True


def _repo_relative(path: Path, root: Path) -> str:
    return path.relative_to(root).as_posix()


def _resolve_repo_root(raw: str | None) -> Path:
    root = Path(raw).expanduser() if raw else _default_repo_root()
    root = root.resolve()
    if not (root / API_ROOT).is_dir():
        raise ReaderError(
            "api_root_missing",
            f"Ascend C API root is missing below repository root: {API_ROOT}",
            repo_root=str(root),
        )
    return root


def _resolve_document(root: Path, raw: str) -> Path:
    supplied = Path(raw)
    if supplied.is_absolute() or ".." in supplied.parts:
        raise ReaderError("unsafe_document_path", "document must be a repository-relative path")
    path = (root / supplied).resolve()
    api_root = (root / API_ROOT).resolve()
    if not _within(path, api_root):
        raise ReaderError("unsafe_document_path", "document must remain inside the C API root")
    if path.suffix.lower() != ".md" or not path.is_file():
        raise ReaderError("document_not_found", f"API Markdown document does not exist: {raw}")
    return path


def _resolve_declared_path(
    root: Path, raw: Any, allowed_root: str, *, expect_directory: bool = False
) -> Path:
    if not isinstance(raw, str) or not raw.strip():
        raise ReaderError("invalid_evidence_path", "OKF evidence path is missing or invalid")
    relative = Path(raw.lstrip("/"))
    if relative.is_absolute() or ".." in relative.parts or not relative.parts:
        raise ReaderError("unsafe_evidence_path", f"unsafe OKF evidence path: {raw}")
    if relative.parts[0] != allowed_root:
        raise ReaderError(
            "unsafe_evidence_path",
            f"OKF evidence path must be below /{allowed_root}: {raw}",
        )
    path = (root / relative).resolve()
    allowed = (root / allowed_root).resolve()
    if not _within(path, allowed):
        raise ReaderError("unsafe_evidence_path", f"OKF evidence path escapes /{allowed_root}: {raw}")
    exists_as_expected = path.is_dir() if expect_directory else path.is_file()
    if not exists_as_expected:
        kind = "directory" if expect_directory else "file"
        raise ReaderError(
            "evidence_file_missing",
            f"OKF-declared evidence {kind} does not exist: {raw}",
        )
    return path


def _read_frontmatter(path: Path) -> tuple[dict[str, Any], str, str, int]:
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines(keepends=True)
    if not lines or lines[0].strip() != "---":
        raise ReaderError("frontmatter_missing", "document has no YAML front matter")
    closing = next((index for index, line in enumerate(lines[1:], 1) if line.strip() == "---"), None)
    if closing is None:
        raise ReaderError("frontmatter_malformed", "YAML front matter has no closing delimiter")
    raw_yaml = "".join(lines[1:closing])
    try:
        metadata = yaml.safe_load(raw_yaml)
    except yaml.YAMLError as error:
        raise ReaderError("frontmatter_malformed", f"cannot parse YAML front matter: {error}") from error
    if not isinstance(metadata, dict):
        raise ReaderError("frontmatter_malformed", "YAML front matter must be a mapping")
    return metadata, raw_yaml, "".join(lines[closing + 1 :]), closing + 2


def _read_body(path: Path) -> tuple[str, int]:
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines(keepends=True)
    if not lines or lines[0].strip() != "---":
        return text, 1
    closing = next((index for index, line in enumerate(lines[1:], 1) if line.strip() == "---"), None)
    if closing is None:
        return text, 1
    return "".join(lines[closing + 1 :]), closing + 2


def _normalize_heading(value: str) -> str:
    value = re.sub(r"\s+#+\s*$", "", value.strip())
    return " ".join(value.split()).casefold()


def _headings(body: str, first_line: int) -> list[dict[str, Any]]:
    results: list[dict[str, Any]] = []
    fenced_by: str | None = None
    offset = 0
    for index, line in enumerate(body.splitlines(keepends=True)):
        fence = FENCE_RE.match(line)
        if fence:
            marker = fence.group(1)[0]
            if fenced_by is None:
                fenced_by = marker
            elif fenced_by == marker:
                fenced_by = None
            offset += len(line)
            continue
        if fenced_by is None:
            match = HEADING_RE.match(line.rstrip("\r\n"))
            if match:
                results.append(
                    {
                        "level": len(match.group(1)),
                        "title": re.sub(r"\s+#+\s*$", "", match.group(2)).strip(),
                        "line": first_line + index,
                        "offset": offset,
                    }
                )
        offset += len(line)
    return results


def _one_based(items: Any, raw_index: int, label: str) -> Any:
    if not isinstance(items, list):
        raise ReaderError("metadata_shape_invalid", f"OKF field for {label} must be a list")
    if raw_index < 1 or raw_index > len(items):
        raise ReaderError(
            "index_out_of_range",
            f"{label} index {raw_index} is outside 1..{len(items)}",
        )
    return items[raw_index - 1]


def _json_size(payload: dict[str, Any]) -> int:
    return len(json.dumps(payload, ensure_ascii=False, separators=(",", ":"))) + 1


def _bounded_payload(
    payload: dict[str, Any],
    content: str,
    cursor: int,
    max_chars: int,
    max_lines: int,
) -> dict[str, Any]:
    if cursor < 0 or cursor > len(content):
        raise ReaderError(
            "cursor_out_of_range",
            f"cursor must be between 0 and {len(content)}",
            total_chars=len(content),
        )
    candidate = content[cursor:]
    line_parts = candidate.splitlines(keepends=True)
    if len(line_parts) > max_lines:
        candidate = "".join(line_parts[:max_lines])

    result = dict(payload)
    result["content"] = candidate
    result["range"] = {
        "start": cursor,
        "end": cursor + len(candidate),
        "total_chars": len(content),
    }
    result["truncated"] = cursor + len(candidate) < len(content)
    if result["truncated"]:
        result["next_cursor"] = cursor + len(candidate)

    if _json_size(result) <= max_chars:
        return result

    low, high = 0, len(candidate)
    while low < high:
        middle = (low + high + 1) // 2
        result["content"] = candidate[:middle]
        result["range"]["end"] = cursor + middle
        result["truncated"] = cursor + middle < len(content)
        if result["truncated"]:
            result["next_cursor"] = cursor + middle
        else:
            result.pop("next_cursor", None)
        if _json_size(result) <= max_chars:
            low = middle
        else:
            high = middle - 1

    result["content"] = candidate[:low]
    result["range"]["end"] = cursor + low
    result["truncated"] = cursor + low < len(content)
    if result["truncated"]:
        result["next_cursor"] = cursor + low
    else:
        result.pop("next_cursor", None)
    if low == 0 or _json_size(result) > max_chars:
        raise ReaderError("output_limit_too_small", "max-chars is too small for the response envelope")
    return result


def _simple_payload(payload: dict[str, Any], max_chars: int) -> dict[str, Any]:
    if _json_size(payload) > max_chars:
        raise ReaderError(
            "output_limit_exceeded",
            "structured result exceeds max-chars; narrow the request",
        )
    return payload


def _base(layer: str, root: Path, document: Path | None = None) -> dict[str, Any]:
    payload: dict[str, Any] = {
        "schema_version": "asc-api-progressive-read.v1",
        "ok": True,
        "layer": layer,
    }
    if document is not None:
        payload["document"] = _repo_relative(document, root)
    return payload


def _locate(args: argparse.Namespace, root: Path) -> dict[str, Any]:
    requested = args.api.removesuffix(".md")
    candidates: list[dict[str, Any]] = []
    for path in sorted((root / API_ROOT).rglob(f"{requested}.md")):
        try:
            metadata, _, _, _ = _read_frontmatter(path)
        except ReaderError:
            metadata = {}
        title = metadata.get("title")
        if path.stem != requested and title != args.api:
            continue
        candidates.append(
            {
                "path": _repo_relative(path, root),
                "title": title if isinstance(title, str) else path.stem,
                "has_okf": bool(metadata),
            }
        )
    payload = _base("locate", root)
    payload["query"] = args.api
    payload["candidates"] = candidates
    payload["ambiguous"] = len(candidates) > 1
    if not candidates:
        payload["ok"] = False
        payload["error"] = {
            "code": "api_not_found",
            "message": f"no exact API document found for {args.api}",
        }
    return payload


def _metadata(args: argparse.Namespace, root: Path) -> tuple[dict[str, Any], str]:
    document = _resolve_document(root, args.document)
    metadata, raw_yaml, _, _ = _read_frontmatter(document)
    fields = list(metadata)
    payload = _base("metadata", root, document)
    payload["available_fields"] = fields
    if args.field:
        missing = [field for field in args.field if field not in metadata]
        if missing:
            raise ReaderError(
                "metadata_field_missing",
                "requested OKF field is absent",
                missing_fields=missing,
                available_fields=fields,
            )
        selected = {field: metadata[field] for field in args.field}
        content = yaml.safe_dump(selected, allow_unicode=True, sort_keys=False)
        payload["selected_fields"] = args.field
    else:
        content = raw_yaml
        payload["selected_fields"] = fields
    return payload, content


def _section_index(args: argparse.Namespace, root: Path) -> dict[str, Any]:
    document = _resolve_document(root, args.document)
    body, first_line = _read_body(document)
    payload = _base("section-index", root, document)
    payload["headings"] = [
        {key: item[key] for key in ("level", "title", "line")}
        for item in _headings(body, first_line)
    ]
    return payload


def _section(args: argparse.Namespace, root: Path) -> tuple[dict[str, Any], str]:
    document = _resolve_document(root, args.document)
    body, first_line = _read_body(document)
    headings = _headings(body, first_line)
    normalized = _normalize_heading(args.heading)
    matches = [item for item in headings if _normalize_heading(item["title"]) == normalized]
    if not matches:
        raise ReaderError(
            "section_not_found",
            f"no exact Markdown heading matches: {args.heading}",
            available_headings=[item["title"] for item in headings],
        )
    if len(matches) > 1:
        raise ReaderError(
            "section_ambiguous",
            f"multiple Markdown headings match: {args.heading}",
            matches=[{"title": item["title"], "line": item["line"]} for item in matches],
        )
    match = matches[0]
    following = [
        item for item in headings
        if item["offset"] > match["offset"] and item["level"] <= match["level"]
    ]
    end = following[0]["offset"] if following else len(body)
    payload = _base("section", root, document)
    payload["heading"] = match["title"]
    payload["heading_line"] = match["line"]
    return payload, body[match["offset"] : end]


def _header_evidence(
    args: argparse.Namespace, root: Path, document: Path, metadata: dict[str, Any]
) -> tuple[dict[str, Any], str]:
    signature = _one_based(metadata.get("signature"), args.signature_index, "signature")
    if not isinstance(signature, dict):
        raise ReaderError("metadata_shape_invalid", "signature entry must be a mapping")
    header = _one_based(signature.get("headers"), args.header_index, "header")
    if not isinstance(header, dict):
        raise ReaderError("metadata_shape_invalid", "header entry must be a mapping")
    path_value = header.get("header") or header.get("scope")
    path = _resolve_declared_path(root, path_value, "include")
    canonical = signature.get("function")
    if not isinstance(canonical, str) or not canonical.strip():
        raise ReaderError("signature_function_missing", "selected OKF signature has no canonical function")

    expected_functions = [canonical.strip()]
    dtype = signature.get("dtype")
    dtype_values = dtype.get("values") if isinstance(dtype, dict) else None
    if "<dtype>" in canonical and isinstance(dtype_values, list):
        expected_functions = [
            canonical.replace("<dtype>", str(value)).strip()
            for value in dtype_values
            if isinstance(value, (str, int))
        ]
    if args.dtype:
        if "<dtype>" not in canonical:
            raise ReaderError(
                "dtype_not_applicable",
                "--dtype cannot select a signature without a <dtype> placeholder",
            )
        selected = canonical.replace("<dtype>", args.dtype).strip()
        if selected not in expected_functions:
            raise ReaderError(
                "dtype_not_declared",
                f"dtype is not declared by the selected OKF signature: {args.dtype}",
                available_dtypes=dtype_values if isinstance(dtype_values, list) else [],
            )
        expected_functions = [selected]
    expected = set(expected_functions)

    text = path.read_text(encoding="utf-8")
    matches: list[tuple[re.Match[str], str]] = []
    for comment in DOXYGEN_RE.finditer(text):
        functions = re.findall(r"@function\s+([^\r\n*]+)", comment.group(0))
        matched_function = next((value.strip() for value in functions if value.strip() in expected), None)
        if matched_function is not None:
            matches.append((comment, matched_function))
    if not matches:
        raise ReaderError(
            "header_marker_missing",
            "OKF-declared header has no exact @function marker for the selected signature",
            header=_repo_relative(path, root),
            function=canonical,
        )
    found_functions = [function for _, function in matches]
    duplicates = sorted({function for function in found_functions if found_functions.count(function) > 1})
    if duplicates:
        raise ReaderError(
            "header_marker_ambiguous",
            "OKF-declared header has duplicate exact @function markers",
            header=_repo_relative(path, root),
            functions=duplicates,
        )
    blocks: list[str] = []
    for match, _ in matches:
        semicolon = text.find(";", match.end())
        if semicolon < 0 or semicolon - match.end() > HARD_MAX_CHARS * 2:
            raise ReaderError("header_declaration_missing", "cannot bound the declaration after @function")
        blocks.append(text[match.start() : semicolon + 1])
    payload = _base("header-evidence", root, document)
    payload.update(
        {
            "evidence_path": _repo_relative(path, root),
            "function": canonical,
            "matched_functions": found_functions,
            "unmatched_functions": [
                function for function in expected_functions if function not in found_functions
            ],
            "signature_index": args.signature_index,
            "header_index": args.header_index,
            "dtype": args.dtype,
        }
    )
    return payload, "\n\n".join(blocks)


def _example_evidence(
    args: argparse.Namespace, root: Path, document: Path, metadata: dict[str, Any]
) -> tuple[dict[str, Any], str]:
    example = _one_based(metadata.get("examples"), args.example_index, "example")
    if not isinstance(example, dict):
        raise ReaderError("metadata_shape_invalid", "example entry must be a mapping")
    project_path = _resolve_declared_path(
        root, example.get("path"), "examples", expect_directory=True
    )
    signatures = example.get("signature")
    if signatures is None:
        signatures = example.get("signatures")
    signature = _one_based(signatures, args.signature_index, "example signature")
    if not isinstance(signature, dict):
        raise ReaderError("metadata_shape_invalid", "example signature entry must be a mapping")
    snippet = _one_based(signature.get("snippets"), args.snippet_index, "snippet")
    if not isinstance(snippet, dict):
        raise ReaderError("metadata_shape_invalid", "snippet entry must be a mapping")
    snippet_id = snippet.get("id")
    file_value = snippet.get("file")
    if not isinstance(snippet_id, str) or not snippet_id:
        raise ReaderError("snippet_id_missing", "selected OKF snippet has no id")
    if not isinstance(file_value, str) or not file_value:
        raise ReaderError("snippet_file_missing", "selected OKF snippet has no file")
    file_relative = Path(file_value)
    if file_relative.is_absolute() or ".." in file_relative.parts:
        raise ReaderError("unsafe_evidence_path", f"unsafe OKF snippet file: {file_value}")
    source = (project_path / file_relative).resolve()
    if not _within(source, project_path.resolve()) or not source.is_file():
        raise ReaderError("evidence_file_missing", f"OKF-declared snippet file does not exist: {file_value}")

    text = source.read_text(encoding="utf-8")
    marker_re = re.compile(rf"^\s*//!\s*\[{re.escape(snippet_id)}\]\s*$", re.MULTILINE)
    markers = list(marker_re.finditer(text))
    if len(markers) != 2:
        raise ReaderError(
            "snippet_marker_invalid",
            "OKF snippet id must have exactly one paired marker block",
            evidence_path=_repo_relative(source, root),
            snippet_id=snippet_id,
            marker_count=len(markers),
        )
    line_end = text.find("\n", markers[1].end())
    end = len(text) if line_end < 0 else line_end + 1
    payload = _base("example-evidence", root, document)
    payload.update(
        {
            "evidence_path": _repo_relative(source, root),
            "snippet_id": snippet_id,
            "example_index": args.example_index,
            "signature_index": args.signature_index,
            "snippet_index": args.snippet_index,
        }
    )
    return payload, text[markers[0].start() : end]


def _evidence(args: argparse.Namespace, root: Path) -> tuple[dict[str, Any], str]:
    document = _resolve_document(root, args.document)
    metadata, _, _, _ = _read_frontmatter(document)
    if args.kind == "header":
        return _header_evidence(args, root, document, metadata)
    return _example_evidence(args, root, document, metadata)


def _parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", help="DevKit repository root")
    parser.add_argument("--max-chars", type=int, default=HARD_MAX_CHARS)
    parser.add_argument("--max-lines", type=int, default=HARD_MAX_LINES)
    subparsers = parser.add_subparsers(dest="command", required=True)

    locate = subparsers.add_parser("locate", help="find exact API document candidates")
    locate.add_argument("--api", required=True)

    metadata = subparsers.add_parser("metadata", help="read YAML front matter only")
    metadata.add_argument("--document", required=True)
    metadata.add_argument("--field", action="append")
    metadata.add_argument("--cursor", type=int, default=0)

    sections = subparsers.add_parser("sections", help="list Markdown headings only")
    sections.add_argument("--document", required=True)

    section = subparsers.add_parser("section", help="read one exact Markdown section")
    section.add_argument("--document", required=True)
    section.add_argument("--heading", required=True)
    section.add_argument("--cursor", type=int, default=0)

    evidence = subparsers.add_parser("evidence", help="read exact OKF-declared evidence")
    evidence.add_argument("--document", required=True)
    evidence.add_argument("--kind", required=True, choices=("header", "example"))
    evidence.add_argument("--signature-index", type=int, default=1)
    evidence.add_argument("--header-index", type=int, default=1)
    evidence.add_argument("--dtype", help="select one dtype expansion for header evidence")
    evidence.add_argument("--example-index", type=int, default=1)
    evidence.add_argument("--snippet-index", type=int, default=1)
    evidence.add_argument("--cursor", type=int, default=0)
    return parser


def _error_payload(error: ReaderError, max_chars: int) -> dict[str, Any]:
    payload: dict[str, Any] = {
        "schema_version": "asc-api-progressive-read.v1",
        "ok": False,
        "error": {"code": error.code, "message": error.message},
    }
    if error.details:
        payload["error"]["details"] = error.details
    if _json_size(payload) > max_chars:
        payload["error"].pop("details", None)
    return payload


def main(argv: list[str] | None = None) -> int:
    parser = _parser()
    args = parser.parse_args(argv)
    max_chars = args.max_chars
    max_lines = args.max_lines
    if max_chars < 512 or max_chars > HARD_MAX_CHARS:
        parser.error(f"--max-chars must be between 512 and {HARD_MAX_CHARS}")
    if max_lines < 1 or max_lines > HARD_MAX_LINES:
        parser.error(f"--max-lines must be between 1 and {HARD_MAX_LINES}")

    try:
        root = _resolve_repo_root(args.repo_root)
        if args.command == "locate":
            payload = _simple_payload(_locate(args, root), max_chars)
        elif args.command == "metadata":
            base, content = _metadata(args, root)
            payload = _bounded_payload(base, content, args.cursor, max_chars, max_lines)
        elif args.command == "sections":
            payload = _simple_payload(_section_index(args, root), max_chars)
        elif args.command == "section":
            base, content = _section(args, root)
            payload = _bounded_payload(base, content, args.cursor, max_chars, max_lines)
        else:
            base, content = _evidence(args, root)
            payload = _bounded_payload(base, content, args.cursor, max_chars, max_lines)
    except ReaderError as error:
        print(json.dumps(_error_payload(error, max_chars), ensure_ascii=False, separators=(",", ":")))
        return 2

    print(json.dumps(payload, ensure_ascii=False, separators=(",", ":")))
    return 0 if payload.get("ok") else 2


if __name__ == "__main__":
    sys.exit(main())
