from __future__ import annotations

from pathlib import Path
from typing import Iterable, List


def discover_examples(examples_root: Path) -> List[Path]:
    examples: List[Path] = []
    for readme in examples_root.rglob("README.md"):
        text = readme.read_text(encoding="utf-8", errors="ignore")
        if "## 编译运行" in text:
            examples.append(readme.parent)
    return sorted(examples, key=lambda p: p.relative_to(examples_root).as_posix())


def filter_examples(paths: Iterable[Path], includes: Iterable[str], excludes: Iterable[str]) -> List[Path]:
    include_terms = [x for x in includes if x]
    exclude_terms = [x for x in excludes if x]
    result: List[Path] = []
    for path in paths:
        text = path.as_posix()
        if include_terms and not any(term in text for term in include_terms):
            continue
        if exclude_terms and any(term in text for term in exclude_terms):
            continue
        result.append(path)
    return result
