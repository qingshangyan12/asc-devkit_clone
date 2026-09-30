"""slim 头（5 字段）解析与冻结词表加载（okf-index 自含工具）。

不依赖建设侧 skill：词表为共享配置 `okf_skills/config/tag-taxonomy.json`
（okf-tag-research 冻结产出的唯一事实源）；头契约见 okf_skills/字段与推导规范.md。
"""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any

import yaml

OKF_ROOT = Path(__file__).resolve().parents[3]
TAXONOMY_PATH = OKF_ROOT / "config" / "tag-taxonomy.json"
DEPENDENCY_KINDS = {"requires", "configures", "constrained_by", "related", "replaced_by"}


class ValidationError(ValueError):
    pass


def split_front_matter(text: str) -> tuple[dict[str, Any], str, str]:
    if not text.startswith("---\n"):
        return {}, "", text
    end = text.find("\n---\n", 4)
    if end < 0:
        raise ValidationError("front matter is not terminated")
    raw = text[4:end]
    data = yaml.safe_load(raw) or {}
    if not isinstance(data, dict):
        raise ValidationError("front matter must be a mapping")
    return data, raw, text[end + len("\n---\n"):]


def taxonomy(path: Path = TAXONOMY_PATH) -> tuple[tuple[str, ...], dict[str, set[str]]]:
    """冻结词表：返回全部 namespace 及其合法值（不止 required）。"""
    payload = json.loads(path.read_text(encoding="utf-8"))
    if payload.get("status") != "frozen":
        raise ValidationError("taxonomy is not frozen")
    defs = payload.get("namespaces")
    if not isinstance(defs, dict) or not defs:
        raise ValidationError("taxonomy namespaces are invalid")
    values: dict[str, set[str]] = {}
    for namespace, definition in defs.items():
        options = definition.get("values") if isinstance(definition, dict) else None
        if not isinstance(options, dict):
            raise ValidationError(f"taxonomy values missing: {namespace}")
        values[namespace] = set(options)
    order = tuple(defs)
    return order, values
