"""样例目录解析：//! [override_id] marker → 样例文件映射。"""

from __future__ import annotations

import re
from pathlib import Path

MARKER_RE = re.compile(r"//!\s*\[(?P<id>[^\]]+)\]")


def build_examples_db(examples_root: Path) -> dict[str, list[dict[str, str]]]:
    """override_id → [{path, file}]；path 为仓内相对目录，file 为样例源文件名。"""
    db: dict[str, list[dict[str, str]]] = {}
    if not examples_root.is_dir():
        return db
    for path in sorted(examples_root.rglob("*.asc")):
        ids = {m.group("id") for m in MARKER_RE.finditer(path.read_text(encoding="utf-8", errors="replace"))}
        entry = {
            "path": f"/{path.parent.as_posix()}",
            "file": path.name,
        }
        for identifier in sorted(ids):
            db.setdefault(identifier, []).append(entry)
    return db
