#!/usr/bin/env python3
"""Prepare and validate an automated, evidence-backed API tag research run.

This utility deliberately does not spawn agents, write DevKit documents, update a
release registry, or mutate an existing taxonomy.  The Skill's main agent invokes
the independent workers, then this utility verifies their structured artifacts.
"""

from __future__ import annotations

import argparse
from collections import defaultdict
from copy import deepcopy
from datetime import datetime, timezone
from hashlib import sha256
import importlib
import json
from pathlib import Path
import re
import sys
from typing import Any, Iterable


SCHEMA_VERSION = 1
TAG_RE = re.compile(r"^(?P<namespace>[a-z][a-z0-9_]*):(?P<value>[a-z][a-z0-9_]*)$")
RUN_ID_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]*$")
SKILL_ROOT = Path(__file__).resolve().parents[1]
TAXONOMY_DIR = SKILL_ROOT / "taxonomy"
DEFAULT_TAXONOMY = SKILL_ROOT / "config" / "tag-taxonomy.json"
RELEASE_DIR = TAXONOMY_DIR / "releases"


def read_json(path: Path) -> dict[str, Any]:
    payload = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(payload, dict):
        raise ValueError(f"JSON object expected: {path}")
    return payload


def write_json(path: Path, payload: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def digest_bytes(data: bytes) -> str:
    return sha256(data).hexdigest()


def digest_file(path: Path) -> str:
    return digest_bytes(path.read_bytes())


def strip_front_matter(text: str) -> str:
    if text.startswith("---\n"):
        end = text.find("\n---\n", 4)
        if end >= 0:
            return text[end + len("\n---\n") :]
    return text


def markdown_title(text: str, fallback: str) -> str:
    match = re.search(r"^#\s+(.+?)\s*$", strip_front_matter(text), re.MULTILINE)
    if match is None:
        return fallback
    return re.sub(r"\s+", " ", match.group(1)).strip()


def now_utc() -> str:
    return datetime.now(timezone.utc).replace(microsecond=0).isoformat()


def taxonomy_namespaces(taxonomy: dict[str, Any]) -> dict[str, dict[str, Any]]:
    namespaces = taxonomy.get("namespaces", {})
    extensions = taxonomy.get("extension_namespaces", {})
    if not isinstance(namespaces, dict) or not isinstance(extensions, dict):
        raise ValueError("taxonomy namespaces must be objects")
    result = dict(namespaces)
    overlap = set(result).intersection(extensions)
    if overlap:
        raise ValueError("taxonomy has overlapping namespaces: " + ", ".join(sorted(overlap)))
    result.update(extensions)
    for name, definition in result.items():
        if not isinstance(name, str) or not isinstance(definition, dict):
            raise ValueError("taxonomy namespace is invalid")
        values = definition.get("values")
        if not isinstance(values, dict) or not all(isinstance(value, str) for value in values):
            raise ValueError(f"taxonomy values are invalid: {name}")
    return result


def required_namespaces(taxonomy: dict[str, Any]) -> set[str]:
    namespaces = taxonomy_namespaces(taxonomy)
    required = {
        name for name, definition in namespaces.items() if definition.get("required") is True
    }
    if not required:
        raise ValueError("taxonomy must declare at least one required namespace")
    declared = taxonomy.get("required_namespaces")
    if declared is not None and (not isinstance(declared, list) or set(declared) != required):
        raise ValueError("taxonomy required_namespaces does not match namespace required flags")
    return required


def known_tags(taxonomy: dict[str, Any]) -> set[str]:
    return {
        f"{namespace}:{value}"
        for namespace, definition in taxonomy_namespaces(taxonomy).items()
        for value in definition["values"]
    }


def resolve_inside(root: Path, candidate: Path) -> Path:
    resolved_root = root.resolve()
    resolved = candidate.resolve()
    try:
        resolved.relative_to(resolved_root)
    except ValueError as error:
        raise ValueError(f"path is outside DevKit root: {candidate}") from error
    return resolved


def load_reference_classifier():
    scripts_dir = (SKILL_ROOT / "scripts").resolve()
    if str(scripts_dir) not in sys.path:
        sys.path.insert(0, str(scripts_dir))
    module = importlib.import_module("extract_api_descriptions")
    classifier = getattr(module, "is_api_reference_document", None)
    if not callable(classifier):
        raise ValueError("extract_api_descriptions.is_api_reference_document is unavailable")
    return classifier


def stratified_sample(records: list[dict[str, str]], count: int, salt: str) -> list[dict[str, str]]:
    if count > len(records):
        raise ValueError(f"requested {count} documents but only {len(records)} are available")
    grouped: dict[str, list[dict[str, str]]] = defaultdict(list)
    for record in records:
        grouped[record["group"]].append(record)
    ordered = [
        sorted(group, key=lambda item: digest_bytes((salt + item["path"]).encode("utf-8")))
        for _, group in sorted(grouped.items())
    ]
    selected: list[dict[str, str]] = []
    cursor = 0
    while len(selected) < count:
        added = False
        for group in ordered:
            if cursor < len(group) and len(selected) < count:
                selected.append(group[cursor])
                added = True
        if not added:
            raise ValueError("stratified sampling exhausted the corpus")
        cursor += 1
    return sorted(selected, key=lambda item: item["path"])


def scan_documents(devkit_root: Path, source_dirs: list[Path]) -> list[dict[str, str]]:
    is_reference = load_reference_classifier()
    root = devkit_root.resolve()
    records: list[dict[str, str]] = []
    seen: set[str] = set()
    for source_dir in source_dirs:
        source = resolve_inside(root, source_dir)
        if not source.is_dir():
            raise FileNotFoundError(f"source directory is missing: {source}")
        prefix = source.relative_to(root).as_posix()
        for document in sorted(source.rglob("*.md")):
            raw = document.read_text(encoding="utf-8", errors="replace")
            if not is_reference(raw):
                continue
            relative = document.relative_to(root).as_posix()
            if relative in seen:
                continue
            seen.add(relative)
            rel_parts = document.relative_to(source).parts
            group = f"{source.name}/{rel_parts[0]}" if len(rel_parts) > 1 else source.name
            records.append(
                {
                    "path": relative,
                    "title": markdown_title(raw, document.stem),
                    "sha256": digest_file(document),
                    "group": group,
                }
            )
    if not records:
        raise ValueError(f"scope contains no API reference Markdown: {[str(s) for s in source_dirs]}")
    return records


def run_file(run_dir: Path, relative: str) -> Path:
    return run_dir / relative


def read_manifest(run_dir: Path) -> dict[str, Any]:
    manifest = read_json(run_file(run_dir, "run-manifest.json"))
    if manifest.get("schema_version") != SCHEMA_VERSION or manifest.get("purpose") != "okf_tag_research_run":
        raise ValueError("run manifest is invalid")
    return manifest


def source_documents(manifest: dict[str, Any]) -> dict[str, dict[str, str]]:
    documents = manifest.get("scope", {}).get("documents")
    if not isinstance(documents, list):
        raise ValueError("run manifest has no source documents")
    result: dict[str, dict[str, str]] = {}
    for document in documents:
        if not isinstance(document, dict):
            raise ValueError("source document record is invalid")
        path = document.get("path")
        if not isinstance(path, str) or path in result:
            raise ValueError("source document path is invalid or duplicated")
        result[path] = document
    return result


def validate_source_snapshot(manifest: dict[str, Any]) -> list[str]:
    root = Path(manifest["scope"]["devkit_root"])
    errors: list[str] = []
    for path, document in source_documents(manifest).items():
        candidate = root / path
        if not candidate.is_file():
            errors.append(f"source document is missing: {path}")
        elif digest_file(candidate) != document.get("sha256"):
            errors.append(f"source document changed since prepare: {path}")
    return errors


def assignment(manifest: dict[str, Any], worker: str) -> dict[str, Any]:
    assignments = manifest.get("assignments", {})
    relative = assignments.get(worker)
    if not isinstance(relative, str):
        raise ValueError(f"run manifest has no assignment for {worker}")
    return read_json(Path(manifest["run_dir"]) / relative)


def output_path(manifest: dict[str, Any], worker: str) -> Path:
    output = manifest.get("outputs", {}).get(worker)
    if not isinstance(output, str):
        raise ValueError(f"run manifest has no output path for {worker}")
    return Path(manifest["run_dir"]) / output


def assigned_paths(payload: dict[str, Any]) -> set[str]:
    documents = payload.get("documents")
    if not isinstance(documents, list):
        raise ValueError("assignment has no documents")
    paths = [item.get("path") for item in documents if isinstance(item, dict)]
    if len(paths) != len(documents) or not all(isinstance(path, str) for path in paths):
        raise ValueError("assignment document path is invalid")
    if len(paths) != len(set(paths)):
        raise ValueError("assignment document path is duplicated")
    return set(paths)


def validate_tag(tag: Any, namespaces: set[str]) -> str | None:
    if not isinstance(tag, str):
        return "tag must be a string"
    match = TAG_RE.fullmatch(tag)
    if match is None:
        return f"invalid tag syntax: {tag!r}"
    if match.group("namespace") not in namespaces:
        return f"unknown taxonomy namespace: {match.group('namespace')}"
    return None


def discovery_validation(manifest: dict[str, Any], worker: str) -> dict[str, Any]:
    errors = validate_source_snapshot(manifest)
    result_path = output_path(manifest, worker)
    if not result_path.is_file():
        errors.append(f"worker output is missing: {result_path}")
        return validation_payload(manifest, worker, errors)
    payload = read_json(result_path)
    expected = assigned_paths(assignment(manifest, worker))
    all_docs = source_documents(manifest)
    taxonomy = read_json(Path(manifest["taxonomy"]["path"]))
    namespaces = set(taxonomy_namespaces(taxonomy))
    if payload.get("schema_version") != SCHEMA_VERSION:
        errors.append("schema_version is invalid")
    if payload.get("purpose") != "independent_open_discovery":
        errors.append("purpose must be independent_open_discovery")
    if payload.get("run_id") != manifest.get("run_id"):
        errors.append("run_id does not match the assignment")
    if payload.get("worker") != worker:
        errors.append("worker does not match the assignment")
    reviewed = payload.get("reviewed_documents")
    if not isinstance(reviewed, list) or set(reviewed) != expected or len(reviewed) != len(set(reviewed)):
        errors.append("reviewed_documents do not exactly match the assignment")
    documents = payload.get("documents")
    if not isinstance(documents, list):
        errors.append("documents must be an array")
        documents = []
    by_path: dict[str, dict[str, Any]] = {}
    for item in documents:
        if not isinstance(item, dict) or not isinstance(item.get("path"), str):
            errors.append("discovery document is invalid")
            continue
        path = item["path"]
        if path in by_path:
            errors.append(f"duplicate discovery document: {path}")
            continue
        by_path[path] = item
    if set(by_path) != expected:
        errors.append("discovery documents do not exactly match the assignment")
    peer_required = len(all_docs) >= 3
    root = Path(manifest["scope"]["devkit_root"])
    for path, item in by_path.items():
        if item.get("title") != all_docs[path]["title"]:
            errors.append(f"{path}: title does not match assignment")
        peers = item.get("peer_paths")
        if not isinstance(peers, list):
            errors.append(f"{path}: peer_paths must be an array")
        elif peer_required and (len(peers) < 2 or len(set(peers)) < 2):
            errors.append(f"{path}: requires two distinct peer paths")
        elif any(not isinstance(peer, str) or peer == path or peer not in all_docs for peer in peers):
            errors.append(f"{path}: peer path is invalid")
        tags = item.get("proposed_tags")
        evidence = item.get("evidence")
        if not isinstance(tags, list) or len(tags) != len(set(tags)) or tags != sorted(tags):
            errors.append(f"{path}: proposed_tags must be a sorted unique array")
            continue
        if not isinstance(evidence, dict) or set(evidence) != set(tags):
            errors.append(f"{path}: proposed_tags and evidence keys differ")
            continue
        raw = strip_front_matter((root / path).read_text(encoding="utf-8", errors="replace"))
        for tag in tags:
            message = validate_tag(tag, namespaces)
            if message:
                errors.append(f"{path}: {message}")
                continue
            excerpt = evidence.get(tag)
            if not isinstance(excerpt, str) or not excerpt.strip():
                errors.append(f"{path}: missing evidence for {tag}")
            elif not tag.startswith("api_surfaces:") and excerpt not in raw:
                errors.append(f"{path}: non-verbatim evidence for {tag}")
    return validation_payload(manifest, worker, errors)


def validation_payload(manifest: dict[str, Any], worker: str, errors: list[str]) -> dict[str, Any]:
    return {
        "schema_version": SCHEMA_VERSION,
        "purpose": "worker_validation",
        "run_id": manifest["run_id"],
        "worker": worker,
        "validated_at": now_utc(),
        "errors": errors,
        "valid": not errors,
    }


def require_valid_report(manifest: dict[str, Any], worker: str) -> None:
    report_path = Path(manifest["run_dir"]) / "validation" / f"{worker}.json"
    if not report_path.is_file():
        raise ValueError(f"validation report is missing: {report_path}")
    report = read_json(report_path)
    if not report.get("valid"):
        raise ValueError(f"validation report did not pass: {report_path}")


def discovery_observations(payload: dict[str, Any], worker: str) -> Iterable[tuple[str, str, dict[str, Any]]]:
    for document in payload["documents"]:
        for tag in document["proposed_tags"]:
            yield tag, document["path"], {
                "worker": worker,
                "path": document["path"],
                "excerpt": document["evidence"][tag],
            }


def summarize_discovery(manifest: dict[str, Any]) -> dict[str, Any]:
    require_valid_report(manifest, "discovery-a")
    require_valid_report(manifest, "discovery-b")
    taxonomy = read_json(Path(manifest["taxonomy"]["path"]))
    known = known_tags(taxonomy)
    all_docs = source_documents(manifest)
    observations: dict[str, dict[str, dict[str, dict[str, Any]]]] = defaultdict(lambda: defaultdict(dict))
    for worker in ("discovery-a", "discovery-b"):
        payload = read_json(output_path(manifest, worker))
        for tag, path, observation in discovery_observations(payload, worker):
            observations[tag][worker][path] = observation
    candidates = []
    for tag in sorted(observations):
        namespace = tag.split(":", 1)[0]
        by_worker = observations[tag]
        paths = sorted({path for records in by_worker.values() for path in records})
        directories = sorted({all_docs[path]["group"] for path in paths})
        coverage = len(paths) / len(all_docs)
        workers = sorted(by_worker)
        if tag in known:
            status = "already_frozen"
        elif len(paths) < manifest["thresholds"]["minimum_distinct_documents"]:
            status = "insufficient_document_support"
        elif len(directories) < manifest["thresholds"]["minimum_independent_directories"]:
            status = "insufficient_directory_support"
        elif coverage > manifest["thresholds"]["maximum_coverage"]:
            status = "exceeds_coverage_limit"
        elif len(workers) < 2:
            status = "single_worker_only"
        else:
            status = "requires_main_agent_review"
        evidence = [
            observation
            for worker in sorted(by_worker)
            for _, observation in sorted(by_worker[worker].items())
        ]
        candidates.append(
            {
                "tag": tag,
                "workers": workers,
                "supporting_documents": len(paths),
                "supporting_directories": len(directories),
                "coverage": round(coverage, 6),
                "paths": paths,
                "directories": directories,
                "evidence": evidence,
                "status": status,
            }
        )
    return {
        "schema_version": SCHEMA_VERSION,
        "purpose": "discovery_summary",
        "run_id": manifest["run_id"],
        "candidate_count": len(candidates),
        "candidates": candidates,
    }


def canonical_map(review: dict[str, Any], raw_tags: set[str], namespaces: set[str]) -> dict[str, str]:
    mappings: dict[str, str] = {}
    entries = review.get("normalizations", [])
    if not isinstance(entries, list):
        raise ValueError("normalizations must be an array")
    for item in entries:
        if not isinstance(item, dict):
            raise ValueError("normalization is invalid")
        source, target, reason = item.get("from"), item.get("to"), item.get("reason")
        if source not in raw_tags or target not in raw_tags or source == target:
            raise ValueError("normalization must map one observed tag to another")
        if not isinstance(reason, str) or not reason.strip():
            raise ValueError("normalization requires a reason")
        source_error = validate_tag(source, namespaces)
        target_error = validate_tag(target, namespaces)
        if source_error or target_error or source.split(":", 1)[0] != target.split(":", 1)[0]:
            raise ValueError("normalization must preserve a valid namespace")
        if source in mappings and mappings[source] != target:
            raise ValueError("normalization has conflicting targets")
        mappings[source] = target
    def canonical(tag: str) -> str:
        seen: set[str] = set()
        while tag in mappings:
            if tag in seen:
                raise ValueError("normalization contains a cycle")
            seen.add(tag)
            tag = mappings[tag]
        return tag
    return {tag: canonical(tag) for tag in raw_tags}


def review_validation(manifest: dict[str, Any]) -> dict[str, Any]:
    require_valid_report(manifest, "discovery-a")
    require_valid_report(manifest, "discovery-b")
    errors = validate_source_snapshot(manifest)
    review_path = Path(manifest["run_dir"]) / "main-agent-review.json"
    if not review_path.is_file():
        return {"schema_version": SCHEMA_VERSION, "purpose": "review_validation", "run_id": manifest["run_id"], "errors": errors + ["main-agent-review.json is missing"], "valid": False}
    review = read_json(review_path)
    summary = read_json(Path(manifest["run_dir"]) / "reports" / "discovery-summary.json")
    taxonomy = read_json(Path(manifest["taxonomy"]["path"]))
    minimum_documents = manifest["thresholds"]["minimum_distinct_documents"]
    raw_candidates = {item["tag"]: item for item in summary.get("candidates", []) if isinstance(item, dict) and isinstance(item.get("tag"), str)}
    raw_tags = set(raw_candidates)
    namespaces = set(taxonomy_namespaces(taxonomy))
    known = known_tags(taxonomy)
    if review.get("schema_version") != SCHEMA_VERSION or review.get("purpose") != "main_agent_taxonomy_review":
        errors.append("review schema_version or purpose is invalid")
    if review.get("run_id") != manifest["run_id"] or review.get("reviewer") != "main_agent_rule_based_adjudication":
        errors.append("review run_id or reviewer is invalid")
    expected_paths = assigned_paths(assignment(manifest, "discovery-a"))
    reviewed = review.get("reviewed_documents")
    if not isinstance(reviewed, list) or set(reviewed) != expected_paths or len(reviewed) != len(set(reviewed)):
        errors.append("reviewed_documents do not match the discovery assignment")
    try:
        canonical = canonical_map(review, raw_tags, namespaces)
    except ValueError as error:
        errors.append(str(error))
        canonical = {tag: tag for tag in raw_tags}
    groups: dict[str, list[str]] = defaultdict(list)
    for raw, normalized in canonical.items():
        groups[normalized].append(raw)
    reviewable: set[str] = set()
    for normalized, members in groups.items():
        namespace = normalized.split(":", 1)[0]
        candidate_records = [raw_candidates[member] for member in members]
        paths = {path for item in candidate_records for path in item["paths"]}
        directories = {directory for item in candidate_records for directory in item["directories"]}
        workers = {worker for item in candidate_records for worker in item["workers"]}
        coverage = len(paths) / len(source_documents(manifest))
        if (
            normalized not in known
            and len(paths) >= minimum_documents
            and len(directories) >= manifest["thresholds"]["minimum_independent_directories"]
            and coverage <= manifest["thresholds"]["maximum_coverage"]
            and workers == {"discovery-a", "discovery-b"}
        ):
            reviewable.add(normalized)
    decisions = review.get("decisions")
    if not isinstance(decisions, list):
        errors.append("decisions must be an array")
        decisions = []
    by_tag: dict[str, dict[str, Any]] = {}
    for item in decisions:
        if not isinstance(item, dict) or not isinstance(item.get("tag"), str):
            errors.append("decision is invalid")
            continue
        tag = item["tag"]
        if tag in by_tag:
            errors.append(f"duplicate decision: {tag}")
        by_tag[tag] = item
    if set(by_tag) != reviewable:
        missing = sorted(reviewable - set(by_tag))
        extra = sorted(set(by_tag) - reviewable)
        if missing:
            errors.append("review is missing decisions: " + ", ".join(missing))
        if extra:
            errors.append("review has decisions outside eligible candidates: " + ", ".join(extra))
    root = Path(manifest["scope"]["devkit_root"])
    for tag, item in by_tag.items():
        decision = item.get("decision")
        reason = item.get("reason")
        if decision not in {"approve", "defer", "reject"}:
            errors.append(f"{tag}: decision is invalid")
        if not isinstance(reason, str) or not reason.strip():
            errors.append(f"{tag}: decision requires a reason")
        if decision != "approve":
            continue
        if not isinstance(item.get("definition"), str) or not item["definition"].strip():
            errors.append(f"{tag}: approval requires a definition")
        if not isinstance(item.get("selection_rule"), str) or not item["selection_rule"].strip():
            errors.append(f"{tag}: approval requires a selection rule")
        evidence = item.get("evidence")
        evidence_paths: set[str] = set()
        if not isinstance(evidence, list):
            errors.append(f"{tag}: approval evidence must be an array")
            evidence = []
        for record in evidence:
            if not isinstance(record, dict) or not isinstance(record.get("path"), str) or not isinstance(record.get("excerpt"), str):
                errors.append(f"{tag}: approval evidence is invalid")
                continue
            path, excerpt = record["path"], record["excerpt"]
            if path not in source_documents(manifest) or excerpt not in strip_front_matter((root / path).read_text(encoding="utf-8", errors="replace")):
                errors.append(f"{tag}: approval evidence is not verbatim source text")
            evidence_paths.add(path)
        if len(evidence_paths) < 2:
            errors.append(f"{tag}: approval requires evidence from two documents")
    return {
        "schema_version": SCHEMA_VERSION,
        "purpose": "review_validation",
        "run_id": manifest["run_id"],
        "eligible_candidates": sorted(reviewable),
        "approved_candidates": sorted(tag for tag, item in by_tag.items() if item.get("decision") == "approve"),
        "errors": errors,
        "valid": not errors,
    }


def annotation_coverage_validation(
    manifest: dict[str, Any], annotation_path: Path
) -> dict[str, Any]:
    """Require every scoped API to carry every required concept namespace."""
    errors = validate_source_snapshot(manifest)
    taxonomy = read_json(Path(manifest["taxonomy"]["path"]))
    namespaces = taxonomy_namespaces(taxonomy)
    required = required_namespaces(taxonomy)
    expected = source_documents(manifest)
    if not annotation_path.is_file():
        errors.append(f"annotation artifact is missing: {annotation_path}")
        return {
            "schema_version": SCHEMA_VERSION,
            "purpose": "annotation_coverage_validation",
            "run_id": manifest["run_id"],
            "required_namespaces": sorted(required),
            "annotated_documents": 0,
            "expected_documents": len(expected),
            "errors": errors,
            "valid": False,
        }
    payload = read_json(annotation_path)
    if payload.get("schema_version") != SCHEMA_VERSION:
        errors.append("annotation schema_version is invalid")
    annotations = payload.get("annotations")
    if not isinstance(annotations, list):
        errors.append("annotations must be an array")
        annotations = []
    seen: set[str] = set()
    root = Path(manifest["scope"]["devkit_root"])
    known_values = {
        namespace: set(definition["values"])
        for namespace, definition in namespaces.items()
    }
    for index, item in enumerate(annotations):
        location = f"annotations[{index}]"
        if not isinstance(item, dict):
            errors.append(f"{location} must be an object")
            continue
        path = item.get("path")
        if not isinstance(path, str) or path not in expected:
            errors.append(f"{location}.path is outside the prepared scope")
            continue
        if path in seen:
            errors.append(f"{location}.path is duplicated: {path}")
        seen.add(path)
        tags = item.get("tag")
        evidence = item.get("evidence")
        if not isinstance(tags, dict) or set(tags) != set(namespaces):
            errors.append(f"{location}.tag must contain exactly the taxonomy namespaces")
            continue
        if not isinstance(evidence, dict):
            errors.append(f"{location}.evidence must be an object")
            continue
        raw = strip_front_matter((root / path).read_text(encoding="utf-8", errors="replace"))
        for namespace in sorted(namespaces):
            values = tags[namespace]
            if not isinstance(values, list) or any(not isinstance(value, str) or not value for value in values):
                errors.append(f"{location}.tag.{namespace} must be a non-empty string list")
                continue
            if len(values) != len(set(values)):
                errors.append(f"{location}.tag.{namespace} has duplicate values")
            if namespace in required and not values:
                errors.append(f"{location}.tag.{namespace} is required and cannot be empty")
            for value in values:
                if value not in known_values[namespace]:
                    errors.append(f"{location}.tag.{namespace} contains unknown value: {value}")
                    continue
                key = f"{namespace}:{value}"
                excerpt = evidence.get(key)
                if not isinstance(excerpt, str) or not excerpt.strip():
                    errors.append(f"{location}.evidence is missing {key}")
                elif excerpt not in raw and not namespace == "api_surfaces":
                    errors.append(f"{location}.evidence for {key} is not verbatim source text")
    missing = sorted(set(expected) - seen)
    if missing:
        errors.append("annotation artifact does not cover every scoped API: " + ", ".join(missing))
    return {
        "schema_version": SCHEMA_VERSION,
        "purpose": "annotation_coverage_validation",
        "run_id": manifest["run_id"],
        "required_namespaces": sorted(required),
        "annotated_documents": len(seen),
        "expected_documents": len(expected),
        "errors": errors,
        "valid": not errors,
    }


def provisional_taxonomy(manifest: dict[str, Any]) -> dict[str, Any]:
    require_valid_report(manifest, "main-agent-review")
    base = read_json(Path(manifest["taxonomy"]["path"]))
    review = read_json(Path(manifest["run_dir"]) / "main-agent-review.json")
    result = deepcopy(base)
    result["status"] = "provisional"
    result["base_version"] = Path(manifest["taxonomy"]["path"]).name
    result["provisional_run"] = {
        "run_id": manifest["run_id"],
        "created_at": now_utc(),
        "base_taxonomy_sha256": manifest["taxonomy"]["sha256"],
    }
    additions = list(result.get("approved_additions", []))
    containers = {**result.get("namespaces", {}), **result.get("extension_namespaces", {})}
    for decision in review["decisions"]:
        if decision["decision"] != "approve":
            continue
        namespace, value = decision["tag"].split(":", 1)
        containers[namespace]["values"][value] = decision["definition"]
        additions.append(
            {
                "tag": decision["tag"],
                "definition": decision["definition"],
                "selection_rule": decision["selection_rule"],
                "evidence": decision["evidence"],
            }
        )
    result["approved_additions"] = additions
    return result


def release_bundle(manifest: dict[str, Any]) -> dict[str, Any]:
    require_valid_report(manifest, "main-agent-review")
    source_errors = validate_source_snapshot(manifest)
    if source_errors:
        raise ValueError("cannot build release bundle: " + "; ".join(source_errors))
    provisional_path = Path(manifest["run_dir"]) / "provisional-taxonomy.json"
    if not provisional_path.is_file():
        raise ValueError("provisional taxonomy is missing")
    review = read_json(Path(manifest["run_dir"]) / "main-agent-review.json")
    approved = [item for item in review["decisions"] if item.get("decision") == "approve"]
    return {
        "schema_version": SCHEMA_VERSION,
        "purpose": "human_approval_release_bundle",
        "run_id": manifest["run_id"],
        "created_at": now_utc(),
        "state": "awaiting_human_approval",
        "scope": manifest["scope"],
        "base_taxonomy": manifest["taxonomy"],
        "gates": {
            "discovery_a_valid": True,
            "discovery_b_valid": True,
            "main_agent_review_valid": True,
            "provisional_taxonomy_sha256": digest_file(provisional_path),
        },
        "approved_additions": approved,
        "artifacts": {
            "manifest": "run-manifest.json",
            "discovery_summary": "reports/discovery-summary.json",
            "review": "main-agent-review.json",
            "provisional_taxonomy": "provisional-taxonomy.json",
        },
    }


def command_prepare(args: argparse.Namespace) -> dict[str, Any]:
    run_dir = args.output_dir.resolve()
    if run_dir.exists() and any(run_dir.iterdir()):
        raise ValueError(f"run directory is not empty: {run_dir}")
    if not RUN_ID_RE.fullmatch(args.run_id):
        raise ValueError("run_id may contain only letters, digits, dot, underscore, and hyphen")
    devkit_root = args.devkit_root.resolve()
    source_dirs: list[Path] = []
    for item in args.source_dir:
        source = item if item.is_absolute() else devkit_root / item
        source_dirs.append(resolve_inside(devkit_root, source))
    taxonomy_path = args.taxonomy.resolve()
    taxonomy = read_json(taxonomy_path)
    if taxonomy.get("status") not in {"frozen", "released"}:
        raise ValueError("base taxonomy must be frozen or released")
    required_namespaces(taxonomy)
    minimum_documents = 2
    if args.discovery_count < 1:
        raise ValueError("discovery_count is invalid")
    if not 0 < args.maximum_coverage <= 1 or args.minimum_independent_directories < 1:
        raise ValueError("coverage or independent directory threshold is invalid")
    documents = scan_documents(devkit_root, source_dirs)
    source_rel = [source.relative_to(devkit_root).as_posix() for source in source_dirs]
    discovery = stratified_sample(documents, min(args.discovery_count, len(documents)), "discovery:")
    run_dir.mkdir(parents=True, exist_ok=True)
    base_assignment = {
        "schema_version": SCHEMA_VERSION,
        "run_id": args.run_id,
        "devkit_root": str(devkit_root),
        "source_dir": source_rel[0] if len(source_rel) == 1 else source_rel,
        "sources": {
            "docs": source_rel,
            "include": "include/c_api",
            "examples": "examples/02_simd_c_api/03_c_api",
        },
        "taxonomy": {"path": str(taxonomy_path), "sha256": digest_file(taxonomy_path)},
        "output_contract": "references/artifact-contract.md",
    }
    assignments: dict[str, str] = {}
    outputs: dict[str, str] = {}
    for worker in ("discovery-a", "discovery-b"):
        relative = f"inputs/{worker}.json"
        assignments[worker] = relative
        outputs[worker] = f"outputs/{worker}.json"
        write_json(run_dir / relative, {**base_assignment, "purpose": "independent_open_discovery_assignment", "worker": worker, "documents": discovery})
    manifest = {
        "schema_version": SCHEMA_VERSION,
        "purpose": "okf_tag_research_run",
        "run_id": args.run_id,
        "created_at": now_utc(),
        "run_dir": str(run_dir),
        "isolation_level": args.isolation_level,
        "scope": {
            "devkit_root": str(devkit_root),
            "source_dirs": source_rel,
            "documents": documents,
        },
        "taxonomy": {"path": str(taxonomy_path), "sha256": digest_file(taxonomy_path)},
        "thresholds": {
            "minimum_distinct_documents": minimum_documents,
            "minimum_independent_directories": args.minimum_independent_directories,
            "maximum_coverage": args.maximum_coverage,
        },
        "assignments": assignments,
        "outputs": outputs,
        "status": "workers_pending",
    }
    write_json(run_dir / "run-manifest.json", manifest)
    return {
        "run_id": args.run_id,
        "run_dir": str(run_dir),
        "source_documents": len(documents),
        "discovery_documents": len(discovery),
        "assignments": assignments,
        "isolation_level": args.isolation_level,
    }


def command_validate_discovery(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    if args.worker not in {"discovery-a", "discovery-b"}:
        raise ValueError("worker must be discovery-a or discovery-b")
    result = discovery_validation(manifest, args.worker)
    write_json(Path(manifest["run_dir"]) / "validation" / f"{args.worker}.json", result)
    return result


def command_summarize(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    result = summarize_discovery(manifest)
    write_json(Path(manifest["run_dir"]) / "reports" / "discovery-summary.json", result)
    return result


def command_validate_review(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    result = review_validation(manifest)
    write_json(Path(manifest["run_dir"]) / "validation" / "main-agent-review.json", result)
    return result


def command_validate_annotation(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    result = annotation_coverage_validation(manifest, args.annotations.resolve())
    write_json(Path(manifest["run_dir"]) / "validation" / "annotation-coverage.json", result)
    return result


def command_freeze_provisional(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    taxonomy = provisional_taxonomy(manifest)
    write_json(Path(manifest["run_dir"]) / "provisional-taxonomy.json", taxonomy)
    return {
        "run_id": manifest["run_id"],
        "status": taxonomy["status"],
        "taxonomy": "provisional-taxonomy.json",
    }


def command_bundle(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    result = release_bundle(manifest)
    write_json(Path(manifest["run_dir"]) / "approval" / "release-bundle.json", result)
    return result


def command_approve(args: argparse.Namespace) -> dict[str, Any]:
    manifest = read_manifest(args.run_dir.resolve())
    bundle_path = Path(manifest["run_dir"]) / "approval" / "release-bundle.json"
    if not bundle_path.is_file():
        raise ValueError("release bundle is missing")
    bundle = read_json(bundle_path)
    if bundle.get("state") != "awaiting_human_approval":
        raise ValueError("release bundle is not awaiting human approval")
    approval = read_json(args.approval.resolve())
    if (
        approval.get("schema_version") != SCHEMA_VERSION
        or approval.get("purpose") != "human_taxonomy_approval"
        or approval.get("run_id") != manifest["run_id"]
        or approval.get("decision") != "approve"
    ):
        raise ValueError("approval record does not approve this run")
    if not isinstance(approval.get("approver"), str) or not approval["approver"].strip():
        raise ValueError("approval record has no approver")
    if not isinstance(approval.get("approved_at"), str) or not approval["approved_at"].strip():
        raise ValueError("approval record has no approval time")
    source_errors = validate_source_snapshot(manifest)
    if source_errors:
        raise ValueError("cannot approve changed source documents: " + "; ".join(source_errors))
    provisional = read_json(Path(manifest["run_dir"]) / "provisional-taxonomy.json")
    if provisional.get("status") != "provisional" or provisional.get("provisional_run", {}).get("run_id") != manifest["run_id"]:
        raise ValueError("provisional taxonomy does not belong to this run")
    frozen = deepcopy(provisional)
    frozen["status"] = "frozen"
    frozen["approval"] = approval
    frozen.pop("provisional_run", None)
    gates = bundle.get("gates")
    if not isinstance(gates, dict):
        raise ValueError("release bundle has no integrity gates")
    if gates.get("provisional_taxonomy_sha256") != digest_file(
        Path(manifest["run_dir"]) / "provisional-taxonomy.json"
    ):
        raise ValueError("provisional taxonomy changed after release bundle creation")
    output = (
        args.output_taxonomy.resolve()
        if args.output_taxonomy is not None
        else RELEASE_DIR / f"{manifest['run_id']}-frozen.json"
    )
    for candidate in (output,):
        try:
            candidate.relative_to(RELEASE_DIR.resolve())
        except ValueError as error:
            raise ValueError(
                "approved taxonomy must be written to asc-knowledge-create/taxonomy/releases/"
            ) from error
    if output.exists():
        raise FileExistsError("refusing to overwrite taxonomy")
    write_json(output, frozen)
    return {
        "run_id": manifest["run_id"],
        "status": "frozen",
        "output_taxonomy": str(output),
        "taxonomy_sha256": digest_file(output),
    }


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="准备和校验自动化、证据驱动的 API tag 调研运行。")
    commands = parser.add_subparsers(dest="command", required=True)
    prepare = commands.add_parser("prepare", help="固化范围并写入子代理任务包。")
    prepare.add_argument("--devkit-root", type=Path, required=True)
    prepare.add_argument("--source-dir", type=Path, required=True, action="append",
                         help="调研源目录（可重复传入多个目录）")
    prepare.add_argument("--taxonomy", type=Path, default=DEFAULT_TAXONOMY)
    prepare.add_argument("--run-id", required=True)
    prepare.add_argument("--output-dir", type=Path, required=True)
    prepare.add_argument("--discovery-count", type=int, default=48)
    prepare.add_argument("--minimum-independent-directories", type=int, default=2)
    prepare.add_argument("--maximum-coverage", type=float, default=0.30)
    prepare.add_argument("--isolation-level", choices=("filesystem_sandbox", "logical_shared_workspace"), default="logical_shared_workspace")
    prepare.set_defaults(func=command_prepare)
    for name, func in (("validate-discovery", command_validate_discovery), ("summarize-discovery", command_summarize), ("validate-review", command_validate_review), ("freeze-provisional", command_freeze_provisional), ("build-release-bundle", command_bundle)):
        item = commands.add_parser(name)
        item.add_argument("--run-dir", type=Path, required=True)
        if name == "validate-discovery":
            item.add_argument("--worker", required=True)
        item.set_defaults(func=func)
    validate_annotation = commands.add_parser(
        "validate-annotation", help="校验逐篇标注是否覆盖所有必填概念。"
    )
    validate_annotation.add_argument("--run-dir", type=Path, required=True)
    validate_annotation.add_argument("--annotations", type=Path, required=True)
    validate_annotation.set_defaults(func=command_validate_annotation)
    approve = commands.add_parser("approve", help="冻结已批准的 provisional taxonomy，但不发布。")
    approve.add_argument("--run-dir", type=Path, required=True)
    approve.add_argument("--approval", type=Path, required=True)
    approve.add_argument("--output-taxonomy", type=Path)
    approve.set_defaults(func=command_approve)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    try:
        result = args.func(args)
    except (FileNotFoundError, FileExistsError, ValueError, json.JSONDecodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    print(json.dumps(result, ensure_ascii=False, indent=2))
    return 0 if result.get("valid", True) else 1


if __name__ == "__main__":
    raise SystemExit(main())
