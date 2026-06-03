from __future__ import annotations

from typing import Iterable, List, Set

from .model import Suggestion


def dedupe_suggestions(items: Iterable[Suggestion]) -> List[Suggestion]:
    seen: Set[tuple[str, str, str, str]] = set()
    out: List[Suggestion] = []
    for item in items:
        key = (item.example, item.category, item.severity, item.message)
        if key in seen:
            continue
        seen.add(key)
        out.append(item)
    return out
