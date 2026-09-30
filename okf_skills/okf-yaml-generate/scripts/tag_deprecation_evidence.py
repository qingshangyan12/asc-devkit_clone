"""Check whether a quotation deprecates the documented subject itself."""

from __future__ import annotations

import re


STATUS_RE = re.compile(r"已废弃|已经废弃|被废弃|\bis deprecated\b|\bhas been deprecated\b", re.IGNORECASE)
HEADING_RE = re.compile(r"^#\s+(.+?)\s*$", re.MULTILINE)
SUBJECT_RE = r"(?:该|本|当前)(?:接口|API|函数|宏|类型|原型)"
DEPRECATED_RE = r"(?:已|已经|被)废弃"


def explicitly_deprecates_subject(body: str, excerpt: str) -> bool:
    heading = HEADING_RE.search(body)
    if heading is None:
        return False
    title = heading.group(1).strip()
    if re.search(r"[（(]废弃[）)]\s*$", title) and title in excerpt:
        return True
    if not STATUS_RE.search(excerpt):
        return False
    if re.search(r"[（(]废弃[）)]\s*$", title):
        return True

    plain = re.sub(r"\[([^]]+)\]\([^)]+\)", r"\1", excerpt).replace("`", "")
    if re.search(SUBJECT_RE + DEPRECATED_RE, plain, re.IGNORECASE):
        return True
    if re.search(
        r"(?:该|本|当前)接口[^。；\n]{0,120}?(?:的)?(?:原型|重载|形式)" + DEPRECATED_RE,
        plain,
    ):
        return True

    title = re.sub(r"[（(]废弃[）)]\s*$", "", title)
    for name in re.split(r"\s*[/、]\s*", title):
        name = name.strip()
        if not name:
            continue
        subject = rf"(?<![A-Za-z0-9_]){re.escape(name)}(?![A-Za-z0-9_])"
        if re.search(subject + r"(?:接口|API|函数|宏|类型|原型)?" + DEPRECATED_RE, plain):
            return True
        if re.search(r"已废弃的[^。；\n]{0,60}?" + subject, plain):
            return True
    return bool(re.search(r"\b(?:this API|this function|this macro) is deprecated\b", plain, re.IGNORECASE))
