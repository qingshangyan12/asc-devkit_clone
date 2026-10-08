"""Exact example snippet-marker rules vendored from api-yaml-metadata-guard.

Only concrete calls receive a paired ``//! [override_id]`` marker.  This module
keeps marker planning content-addressable so the YAML controller can include it
in one write plan instead of writing sample files during validation.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Any, Sequence

from extract_api_yaml_metadata import EXAMPLE_SOURCE_SUFFIXES, _example_snippet_markers
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


def _marker_for_offset(markers: Sequence[Any], offset: int) -> Any | None:
    return next(
        (marker for marker in markers if marker.start_offset <= offset < marker.end_offset),
        None,
    )


def _overlaps(first: tuple[int, int], second: tuple[int, int]) -> bool:
    return first[0] < second[1] and second[0] < first[1]


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
    """Plan exactly the legacy marker updates for a complete worker batch."""
    insertions: list[MarkerInsertion] = []
    skips: list[MarkerSkip] = []
    used_ranges: dict[Path, list[tuple[int, int]]] = {}
    marker_cache: dict[Path, tuple[Any, ...]] = {}
    text_cache: dict[Path, str] = {}

    for api_path, examples in documents:
        for example in examples:
            if example.get("confidence") == "review" and not include_review:
                continue
            if example.get("issues"):
                continue
            if not example.get("path"):
                continue
            for call in example.get("calls", ()):
                marker_id = _call_override_id(call)
                file_resource = call.get("file")
                start_offset = call.get("call_start_offset")
                end_offset = call.get("call_end_offset")
                function = call.get("canonical_id") or call.get("call_template") or ""
                if not marker_id or not file_resource:
                    skips.append(MarkerSkip(None, marker_id or "", "missing marker id or file", api_path))
                    continue
                if not isinstance(start_offset, int) or not isinstance(end_offset, int):
                    skips.append(MarkerSkip(None, marker_id, "missing source offset", api_path))
                    continue

                file_path = (repo_root / file_resource.lstrip("/")).resolve()
                if file_path.suffix.lower() not in EXAMPLE_SOURCE_SUFFIXES:
                    skips.append(MarkerSkip(file_path, marker_id, "not an example source file", api_path))
                    continue
                if not file_path.is_file():
                    skips.append(MarkerSkip(file_path, marker_id, "source file not found", api_path))
                    continue

                source_text = text_cache.setdefault(
                    file_path,
                    file_path.read_text(encoding="utf-8", errors="replace"),
                )
                if file_path not in marker_cache:
                    file_resource_for_parse = f"/{file_path.relative_to(repo_root).as_posix()}"
                    markers, _ = _example_snippet_markers(source_text, file_resource_for_parse)
                    marker_cache[file_path] = markers

                existing_marker = _marker_for_offset(marker_cache[file_path], start_offset)
                if existing_marker is not None:
                    if existing_marker.id == marker_id:
                        skips.append(MarkerSkip(file_path, marker_id, "already aligned", api_path))
                        continue
                    marker_start, marker_end = _existing_marker_bounds(source_text, existing_marker)
                    target_range = (marker_start, marker_end)
                    if any(_overlaps(target_range, used) for used in used_ranges.get(file_path, ())):
                        skips.append(MarkerSkip(file_path, marker_id, "overlaps another insertion", api_path))
                        continue
                    used_ranges.setdefault(file_path, []).append(target_range)
                    insertions.append(
                        MarkerInsertion(
                            file_path=file_path,
                            start_offset=marker_start,
                            end_offset=marker_end,
                            marker_id=marker_id,
                            function=function,
                            api_path=api_path,
                            existing_marker_id=existing_marker.id,
                        )
                    )
                    continue

                line_start, line_end = _line_bounds(source_text, start_offset, end_offset)
                target_range = (line_start, line_end)
                if any(_overlaps(target_range, used) for used in used_ranges.get(file_path, ())):
                    skips.append(MarkerSkip(file_path, marker_id, "overlaps another insertion", api_path))
                    continue
                used_ranges.setdefault(file_path, []).append(target_range)
                insertions.append(
                    MarkerInsertion(
                        file_path=file_path,
                        start_offset=line_start,
                        end_offset=line_end,
                        marker_id=marker_id,
                        function=function,
                        api_path=api_path,
                    )
                )
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
    """Apply the legacy paired-marker rendering to one source-file string."""
    for insertion in sorted(insertions, key=lambda item: item.start_offset, reverse=True):
        if insertion.existing_marker_id is not None:
            segment = text[insertion.start_offset : insertion.end_offset]
            old_marker = f"//! [{insertion.existing_marker_id}]"
            new_marker = f"//! [{insertion.marker_id}]"
            text = text[: insertion.start_offset] + segment.replace(old_marker, new_marker) + text[insertion.end_offset :]
            continue
        indent = _line_indent(text, insertion.start_offset)
        start_marker = f"{indent}//! [{insertion.marker_id}]\n"
        end_marker = f"{indent}//! [{insertion.marker_id}]\n"
        text = (
            text[: insertion.start_offset]
            + start_marker
            + text[insertion.start_offset : insertion.end_offset]
            + end_marker
            + text[insertion.end_offset :]
        )
    return text
