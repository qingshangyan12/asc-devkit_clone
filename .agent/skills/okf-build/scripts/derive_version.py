"""Derive per-API CANN version ranges from Release Notes.

规则（references/头字段与标注规范.md 第 3 节）：
- CANN 9.0.0 首次提供 C API，未点名者 floor 一律为 9.0.0；
- CANN_9_1_0 / CANN_9_2_0「新增特性」小节的条目名，若未在任何更早版本
  Release Notes 中出现过，floor 抬升至该版本；
- 条目描述形如「新增……系列产品支持」的是架构支持扩展而非新接口，
  floor 维持 9.0.0 并进入 review 残留（交由 derive-overrides 裁决）；
- 「接口废弃」仅对 deprecated_interface 文档闭合区间（整接口废弃）；
  当前文档的旧重载废弃不闭合 API 级区间。
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any

import yaml

RELEASE_NOTES_DIR = Path("docs/zh/api/appendix/Release_Notes")
RELEASE_FILES = ("CANN_9_0_0.md", "CANN_9_1_0.md", "CANN_9_2_0.md")
NAME_RE = re.compile(r"\b(asc_[a-z0-9_]+)\b")
DEPRECATED_DOC_RE = re.compile(r"^(?P<name>asc_[a-z0-9_]+)_deprecated$")
ARCH_SUPPORT_RE = re.compile(r"新增.{0,24}(?:系列产品|Ascend|Atlas).{0,8}支持")


def _split_sections(text: str) -> dict[str, str]:
    """按二级标题切分；三级子标题内容归属其父节。"""
    sections: dict[str, str] = {}
    current = ""
    chunks: list[str] = []
    for line in text.splitlines(keepends=True):
        if re.match(r"^##\s", line):
            if current:
                sections[current] = "".join(chunks)
            current = line.strip().lstrip("#").strip()
            chunks = []
        elif current:
            chunks.append(line)
    if current:
        sections[current] = "".join(chunks)
    return sections


def _feature_entries(section_text: str) -> list[tuple[str, str]]:
    """提取「新增特性」条目：(条目名集合所在首段, 条目全文)。"""
    entries: list[tuple[str, str]] = []
    for line in section_text.splitlines():
        stripped = line.strip()
        if not stripped.startswith(("- ", "* ", "+ ")):
            continue
        if "：" not in stripped and ":" not in stripped:
            continue
        head, description = re.split(r"[:：]", stripped, maxsplit=1)
        entries.append((head, description))
    return entries


def _deprecation_names(section_text: str) -> set[str]:
    """提取「接口废弃」表第一列（废弃接口或重载）中的接口名。"""
    names: set[str] = set()
    for line in section_text.splitlines():
        stripped = line.strip()
        if not stripped.startswith("|") or stripped.startswith("| ---"):
            continue
        cells = stripped.split("|")
        if len(cells) < 3:
            continue
        names.update(NAME_RE.findall(cells[1]))
    return names


def apply_overrides(version_map: dict[str, Any], overrides_path: Path | None) -> None:
    if overrides_path is None or not overrides_path.is_file():
        return
    payload = yaml.safe_load(overrides_path.read_text(encoding="utf-8")) or {}
    for api, entry in (payload.get("floors") or {}).items():
        if isinstance(entry, dict) and entry.get("floor"):
            version_map["floors"][api] = str(entry["floor"])


def build_version_map(devkit_root: Path, overrides_path: Path | None = None) -> dict[str, Any]:
    notes_dir = devkit_root / RELEASE_NOTES_DIR
    texts = {name: (notes_dir / name).read_text(encoding="utf-8") for name in RELEASE_FILES}
    versions = {name: name[len("CANN_"): -len(".md")].replace("_", ".") for name in RELEASE_FILES}
    ordered = sorted(versions, key=lambda name: versions[name])
    floors: dict[str, str] = {}
    review: list[dict[str, str]] = []
    deprecated_in: dict[str, str] = {}
    baseline_names: set[str] = set()
    for name in ordered:
        version = versions[name]
        sections = _split_sections(texts[name])
        earlier_text = "".join(texts[prev] for prev in ordered[: ordered.index(name)])
        for heading, text in sections.items():
            if version != "9.0.0" and heading == "新增特性":
                for head, description in _feature_entries(text):
                    for api in NAME_RE.findall(head):
                        if re.search(rf"\b{re.escape(api)}\b", earlier_text):
                            continue
                        if ARCH_SUPPORT_RE.search(description):
                            review.append({"api": api, "version": version, "reason": "架构支持扩展而非新接口", "bullet": head[:120]})
                            continue
                        floors[api] = version
            elif heading == "接口废弃":
                for api in _deprecation_names(text):
                    deprecated_in.setdefault(api, version)
            elif version == "9.0.0" and heading == "特性":
                for head, _ in _feature_entries(text):
                    baseline_names.update(NAME_RE.findall(head))
    version_map = {
        "floors": floors,
        "review": review,
        "deprecated_in": deprecated_in,
        "baseline": {"version": "9.0.0", "names": sorted(baseline_names)},
    }
    apply_overrides(version_map, overrides_path)
    return version_map


def api_name_for(doc_rel: str) -> str:
    """文档路径 → API 名（去 _arch_NNNN / _deprecated 后缀）。"""
    stem = Path(doc_rel).stem
    stem = re.sub(r"_arch_\d+(?:_deprecated)?$", "", stem)
    match = DEPRECATED_DOC_RE.fullmatch(stem)
    if match:
        return match.group("name")
    return stem


def version_for(doc_rel: str, version_map: dict[str, Any]) -> dict[str, Any]:
    """文档 → version 字符串与依据；deprecated 文档才闭合区间。"""
    name = api_name_for(doc_rel)
    floor = version_map["floors"].get(name, "9.0.0")
    is_deprecated_doc = "/deprecated_interface/" in doc_rel
    ceil = version_map["deprecated_in"].get(name) if is_deprecated_doc else None
    value = f">={floor}" if ceil is None or ceil <= floor else f">={floor},<={ceil}"
    return {"api": name, "version": value, "floor": floor, "ceil": ceil}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--devkit-root", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--overrides", type=Path, default=None, help="人工裁决覆盖 YAML（可选）")
    args = parser.parse_args()
    overrides = args.overrides
    if overrides is None:
        candidate = Path(__file__).resolve().parents[1] / "config" / "version-overrides.yaml"
        overrides = candidate if candidate.is_file() else None
    version_map = build_version_map(args.devkit_root.resolve(), overrides)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(
        json.dumps(version_map, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )
    print(json.dumps({
        "output": str(args.output),
        "floor_overrides": len(version_map["floors"]),
        "review_residuals": len(version_map["review"]),
        "deprecated_names": len(version_map["deprecated_in"]),
        "baseline_names": len(version_map["baseline"]["names"]),
    }, ensure_ascii=False))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
