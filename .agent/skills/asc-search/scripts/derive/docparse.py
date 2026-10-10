"""文档正文解析：标题、导航链接、npu 产品标记。"""

from __future__ import annotations

import re
from typing import Any

NPU_MARKER_RE = re.compile(r"<!--\s*npu=\"(?P<marker>[^\"]+)\"\s+(?P<sid>id\d+)\s*-->")
NPU_END_RE = re.compile(r"<!--\s*end\s+(?P<sid>id\d+)\s*-->")
MARKDOWN_LINK_RE = re.compile(r"\[[^\]]*\]\((?P<target>[^)#]+\.md)(?:#[^)]*)?\)")
H1_RE = re.compile(r"(?m)^#\s+(.+?)\s*$")


def doc_title(body: str, fallback: str) -> str:
    match = H1_RE.search(body)
    return match.group(1).strip() if match else fallback


def doc_navigation(body: str) -> list[str]:
    seen: dict[str, None] = {}
    for match in MARKDOWN_LINK_RE.finditer(body):
        target = match.group("target").strip()
        if target.endswith(".md") and not target.startswith(("http", "/")):
            seen.setdefault(target, None)
    return list(seen)


def doc_product_support(body: str) -> dict[str, dict[str, str]]:
    """解析 npu 筛选标记 → {marker: {support: 支持/不支持}}。

    标记形如 <!-- npu="950" id1 -->…<!-- end id1 -->，正文行含 支持/不支持。
    """
    support: dict[str, dict[str, str]] = {}
    open_markers: dict[str, str] = {}
    for line in body.splitlines():
        for match in NPU_MARKER_RE.finditer(line):
            open_markers[match.group("sid")] = match.group("marker")
        for match in NPU_END_RE.finditer(line):
            open_markers.pop(match.group("sid"), None)
        if not open_markers:
            continue
        if "不支持" in line:
            verdict = "不支持"
        elif "支持" in line:
            verdict = "支持"
        else:
            continue
        for marker in open_markers.values():
            support.setdefault(marker, {"support": verdict, "lines": 0})
            if support[marker]["support"] != verdict:
                support[marker]["support"] = "mixed"
            support[marker]["lines"] += 1
    return support
