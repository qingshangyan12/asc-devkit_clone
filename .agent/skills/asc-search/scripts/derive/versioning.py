"""Consume first-support versions and scoped lifecycle events without producer imports."""
from __future__ import annotations

from typing import Any
from .gate import version_key


def event_applies(event: dict[str, Any], identifier: str, arch: str) -> bool:
    scope = event.get("scope", "")
    if not scope.startswith("overload:"):
        return False
    target, _, event_arch = scope[len("overload:"):].partition("@")
    return target == identifier and (not event_arch or event_arch == arch)


def page_available(events: list[dict[str, Any]], target: str) -> bool:
    # A fallback page selector does not establish that the entire API was removed.
    return not any(e.get("kind") == "removed" and e.get("scope") == "page"
                   and e.get("scope_basis") != "fallback"
                   and version_key(e["since"]) <= version_key(target) for e in events)


def signature_available(signature: dict[str, Any], events: list[dict[str, Any]], target: str, arch: str) -> bool:
    relevant = [e for e in events if event_applies(e, signature["override_id"], arch)]
    added = [e["since"] for e in relevant if e["kind"] in {"introduced", "overload_added"}]
    if added and version_key(target) < min(map(version_key, added)):
        return False
    if any(e["kind"] == "removed" and version_key(e["since"]) <= version_key(target) for e in relevant):
        return False
    return True


def deprecation(signature: dict[str, Any], events: list[dict[str, Any]], target: str, arch: str) -> Any:
    value = signature.get("deprecated")
    if value and (not value.get("version") or version_key(value["version"]) <= version_key(target)):
        return value
    candidates = []
    for event in events:
        if event["kind"] != "deprecated" or version_key(event["since"]) > version_key(target):
            continue
        if event_applies(event, signature["override_id"], arch):
            specificity = 0 if "@" in event["scope"] else 1
        elif event["scope"] == "page" and event.get("scope_basis") != "fallback":
            specificity = 2
        else:
            continue
        candidates.append((specificity, version_key(event["since"]), event["since"]))
    if not candidates:
        return None
    return {"version": min(candidates)[2], "replacement": ""}
