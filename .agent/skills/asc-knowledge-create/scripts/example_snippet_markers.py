"""Plan one source-file range with a shared, paired override_id list.

Range discovery includes all featured calls; IDs come only from concrete
overloads or existing annotations. Planning never writes sample files.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Any, Sequence

from extract_api_yaml_metadata import (
    EXAMPLE_SOURCE_SUFFIXES, EXAMPLE_SNIPPET_MARKER_RE, _example_snippet_markers,
)
from prototype_id import PLACEHOLDER_RE, matches_override_id, signature_identifier


@dataclass(frozen=True)
class MarkerInsertion:
    file_path: Path
    start_offset: int
    end_offset: int
    marker_id: str
    function: str
    api_path: str
    existing_marker_id: str | None = None
    marker_ids: tuple[str, ...] = ()
    covered_calls: int = 0


@dataclass(frozen=True)
class MarkerSkip:
    file_path: Path | None
    marker_id: str
    reason: str
    api_path: str


def _line_bounds(text: str, start_offset: int, end_offset: int) -> tuple[int, int]:
    line_start = text.rfind("\n", 0, start_offset) + 1
    line_end = text.find("\n", end_offset)
    if line_end < 0:
        line_end = len(text)
    else:
        line_end += 1
    return line_start, line_end


def _line_indent(text: str, line_start: int) -> str:
    index = line_start
    while index < len(text) and text[index] in {" ", "\t"}:
        index += 1
    return text[line_start:index]


def _call_override_id(call: dict[str, Any]) -> str | None:
    function = call.get("canonical_id") or call.get("call_template")
    if not isinstance(function, str) or not function:
        return None
    for binding in call.get("placeholder_bindings") or ():
        if isinstance(binding, dict) and "name" in binding and "value" in binding:
            function = function.replace(f"<{binding['name']}>", str(binding["value"]))
    if PLACEHOLDER_RE.search(function):
        return None
    field, value = signature_identifier(function)
    if field != "override_id":
        return None
    override_id = call.get("override_id")
    if override_id is None:
        return value
    if not matches_override_id(override_id, value):
        raise ValueError("example call override_id does not match its concrete function")
    return override_id


def _existing_marker_bounds(text: str, marker: Any) -> tuple[int, int]:
    opening_start = text.rfind("\n", 0, max(marker.start_offset - 1, 0)) + 1
    closing_end = text.find("\n", marker.end_offset)
    return opening_start, len(text) if closing_end < 0 else closing_end + 1


def _candidate_insertions(
    documents: Sequence[tuple[str, Sequence[dict[str, Any]]]],
    repo_root: Path,
    *,
    include_review: bool = False,
) -> tuple[list[MarkerInsertion], list[MarkerSkip]]:
    """Merge featured calls across functions into one range per source file."""
    insertions: list[MarkerInsertion] = []
    skips: list[MarkerSkip] = []
    files: dict[Path, dict[str, Any]] = {}

    for api_path, examples in documents:
        for example in examples:
            if example.get("confidence") == "review" and not include_review:
                continue
            if example.get("issues"):
                continue
            if not example.get("path"):
                continue
            exact_calls = example.get("calls", ())
            range_calls = example.get("range_calls", exact_calls)
            for call in (*exact_calls, *range_calls):
                file_resource = call.get("file")
                start_offset = call.get("call_start_offset")
                end_offset = call.get("call_end_offset")
                if not file_resource:
                    skips.append(MarkerSkip(None, "", "missing source file", api_path))
                    continue
                if not isinstance(start_offset, int) or not isinstance(end_offset, int):
                    skips.append(MarkerSkip(None, "", "missing source offset", api_path))
                    continue

                file_path = (repo_root / file_resource.lstrip("/")).resolve()
                if file_path.suffix.lower() not in EXAMPLE_SOURCE_SUFFIXES:
                    skips.append(MarkerSkip(file_path, "", "not an example source file", api_path))
                    continue
                if not file_path.is_file():
                    skips.append(MarkerSkip(file_path, "", "source file not found", api_path))
                    continue
                info = files.setdefault(file_path, {"calls": {}, "exact": {}, "api_path": api_path})
                key = (start_offset, end_offset)
                info["calls"][key] = call
                if call in exact_calls:
                    marker_id = _call_override_id(call)
                    if marker_id:
                        info["exact"][key] = (marker_id, call)

    for file_path, info in sorted(files.items()):
        text = file_path.read_text(encoding="utf-8", errors="replace")
        markers, issues = _example_snippet_markers(text, str(file_path.relative_to(repo_root)))
        if issues:
            skips.append(MarkerSkip(file_path, "", "invalid existing markers: " + ", ".join(issue.type for issue in issues), info["api_path"]))
            continue
        bounds = []
        for start, end in info["calls"]:
            if not 0 <= start < end <= len(text):
                skips.append(MarkerSkip(file_path, "", "invalid source offset", info["api_path"]))
                break
            bounds.append(_line_bounds(text, start, end))
        else:
            ids = {marker_id for marker_id, _ in info["exact"].values()}
            for marker in markers:
                # Preserve unrelated/orphan IDs; replace an old ID only when its
                # own symbol has a concrete call inside the old range.
                symbol = marker.id.split("__", 1)[0]
                replacements = [
                    marker_id for (start, _), (marker_id, call) in info["exact"].items()
                    if marker.start_offset <= start < marker.end_offset
                    and (call.get("symbol") or marker_id.split("__", 1)[0]) == symbol
                ]
                ids.update(replacements or [marker.id])
                bounds.append(_existing_marker_bounds(text, marker))
            if not ids:
                skips.append(MarkerSkip(file_path, "", "no concrete override_id or existing marker", info["api_path"]))
                continue
            marker_ids = tuple(sorted(ids))
            insertions.append(MarkerInsertion(
                file_path=file_path,
                start_offset=min(start for start, _ in bounds),
                end_offset=max(end for _, end in bounds),
                marker_id=", ".join(marker_ids),
                marker_ids=marker_ids,
                covered_calls=len(info["calls"]),
                function="; ".join(sorted({call.get("canonical_id") or "" for _, call in info["exact"].values()})),
                api_path=info["api_path"],
            ))
    return insertions, skips


def candidate_insertions(
    examples: Sequence[dict[str, Any]],
    repo_root: Path,
    api_path: str,
    *,
    include_review: bool = False,
) -> tuple[list[MarkerInsertion], list[MarkerSkip]]:
    return _candidate_insertions(((api_path, examples),), repo_root, include_review=include_review)


def candidate_insertions_for_documents(
    documents: Sequence[tuple[str, Sequence[dict[str, Any]]]],
    repo_root: Path,
    *,
    include_review: bool = False,
) -> tuple[list[MarkerInsertion], list[MarkerSkip]]:
    return _candidate_insertions(documents, repo_root, include_review=include_review)


def apply_insertions_to_text(text: str, insertions: Sequence[MarkerInsertion]) -> str:
    """Replace scattered marker lines with one shared pair; preserve code bytes."""
    for insertion in sorted(insertions, key=lambda item: item.start_offset, reverse=True):
        segment = text[insertion.start_offset : insertion.end_offset]
        segment = "".join(line for line in segment.splitlines(keepends=True) if not EXAMPLE_SNIPPET_MARKER_RE.match(line.rstrip("\r\n")))
        indent = _line_indent(text, insertion.start_offset)
        newline = "\r\n" if "\r\n" in text else "\n"
        start_marker = f"{indent}//! [{insertion.marker_id}]{newline}"
        end_marker = start_marker
        if segment and not segment.endswith(("\n", "\r")):
            segment += newline
        text = (
            text[: insertion.start_offset]
            + start_marker
            + segment
            + end_marker
            + text[insertion.end_offset :]
        )
    return text
