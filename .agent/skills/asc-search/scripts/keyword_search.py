#!/usr/bin/env python3
# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

"""Deterministic keyword retrieval over an Ascend C package; no language model."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import sqlite3
import sys
import tempfile

SCHEMA = "asc-keyword-index.v1"
ROOTS = {"docs": "doc", "include": "include", "examples": "example", ".agent/skills": "skill"}
SUFFIXES = {".md", ".h", ".hpp", ".c", ".cc", ".cpp", ".cxx", ".asc", ".py", ".yaml", ".yml", ".json"}
SKIP = {".git", "__pycache__", "build", "index", "node_modules"}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def files(root):
    for directory, kind in ROOTS.items():
        start = root / directory
        if not start.is_dir() or start.is_symlink():
            continue
        for parent, dirs, names in os.walk(start, followlinks=False):
            dirs[:] = sorted(d for d in dirs if d not in SKIP and not (Path(parent) / d).is_symlink())
            for name in sorted(names):
                path = Path(parent) / name
                if path.suffix.lower() in SUFFIXES and not path.is_symlink():
                    yield path, kind


def record(root, path, kind):
    raw = path.read_bytes()
    body = raw.decode("utf-8", errors="replace")
    title = re.search(r"(?m)^#\s+(.+)$", body)
    description = re.search(r"(?m)^description:\s*(.+)$", body)
    return (
        path.relative_to(root).as_posix(),
        kind,
        title.group(1).strip() if title else path.stem,
        description.group(1).strip() if description else "",
        body,
        digest(raw),
    )


def build(root, target, source_id):
    root, target = Path(root).resolve(), Path(target).resolve()
    if not root.is_dir():
        raise ValueError("corpus root does not exist")
    if target.is_relative_to(root):
        raise ValueError("index must be outside the corpus")
    target.parent.mkdir(parents=True, exist_ok=True)
    fd, temporary = tempfile.mkstemp(prefix=target.name + ".", dir=target.parent)
    os.close(fd)
    try:
        with sqlite3.connect(temporary) as db:
            db.execute(
                "CREATE TABLE documents(path TEXT PRIMARY KEY, kind TEXT, title TEXT, description TEXT, body TEXT, sha256 TEXT)"
            )
            db.execute("CREATE TABLE metadata(payload TEXT)")
            fingerprints = []
            counts = {}
            for path, kind in files(root):
                row = record(root, path, kind)
                db.execute("INSERT INTO documents VALUES(?,?,?,?,?,?)", row)
                fingerprints.append((row[0], row[-1]))
                counts[kind] = counts.get(kind, 0) + 1
            meta = {
                "schema_version": SCHEMA,
                "root": str(root),
                "source_id": source_id,
                "content_fingerprint": digest(json.dumps(sorted(fingerprints), ensure_ascii=False).encode()),
                "engine_fingerprint": digest(Path(__file__).read_bytes()),
                "document_count": len(fingerprints),
                "counts": counts,
            }
            db.execute("INSERT INTO metadata VALUES(?)", (json.dumps(meta, ensure_ascii=False),))
        os.replace(temporary, target)
        return meta
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)


def open_index(path):
    path = Path(path).resolve(strict=True)
    db = sqlite3.connect(path.as_uri() + "?mode=ro", uri=True)
    db.row_factory = sqlite3.Row
    try:
        meta = json.loads(db.execute("SELECT payload FROM metadata").fetchone()[0])
        if meta["schema_version"] != SCHEMA:
            raise ValueError("unsupported keyword index schema")
        if meta["engine_fingerprint"] != digest(Path(__file__).read_bytes()):
            raise ValueError("keyword engine changed; prepare the index again")
        return db, meta
    except Exception:
        db.close()
        raise


def search(index, query, *, limit=10, kinds=(), prefix=None):
    """All whitespace-separated literal terms must match; never expand synonyms."""
    terms = list(dict.fromkeys(query.lower().split()))
    if not terms or limit < 1:
        raise ValueError("nonempty keywords and positive limit are required")
    if set(kinds) - set(ROOTS.values()):
        raise ValueError("unknown resource kind")
    db, meta = open_index(index)
    try:
        filters, params = [], []
        if kinds:
            filters.append("kind IN (" + ",".join("?" for _ in kinds) + ")")
            params.extend(kinds)
        if prefix:
            if Path(prefix).is_absolute() or ".." in Path(prefix).parts:
                raise ValueError("prefix must be corpus-relative")
            filters.append("substr(path,1,?) = ?")
            params.extend([len(prefix), prefix])
        base = " AND ".join(filters) or "1"
        coverage = db.execute("SELECT count(*) FROM documents WHERE " + base, params).fetchone()[0]
        for term in terms:
            filters.append("instr(lower(path || ' ' || title || ' ' || description || ' ' || body), ?) > 0")
            params.append(term)
        rows = db.execute("SELECT * FROM documents WHERE " + " AND ".join(filters), params)
        candidates = []
        root = Path(meta["root"])
        for row in rows:
            score = sum(
                12 * (term == Path(row["path"]).stem.lower())
                + 6 * (term in row["path"].lower())
                + 4 * (term in row["title"].lower())
                + 2 * (term in row["description"].lower())
                + 1
                for term in terms
            )
            candidates.append(
                {
                    "path": row["path"],
                    "kind": row["kind"],
                    "title": row["title"],
                    "description": row["description"],
                    "score": score,
                    "sha256": row["sha256"],
                    "match_basis": "keyword",
                }
            )
        candidates.sort(key=lambda item: (-item["score"], item["path"]))
        hits = candidates[:limit]
        for hit in hits:
            path = root / hit["path"]
            if not path.resolve().is_relative_to(root.resolve()) or path.is_symlink():
                raise ValueError("candidate escaped corpus")
            if digest(path.read_bytes()) != hit["sha256"]:
                raise ValueError("candidate changed; prepare the index again")
            hit["local_path"] = str(path)
        return {
            "status": "no_coverage" if not coverage else ("ok" if hits else "no_match"),
            "coverage_count": coverage,
            "count": len(hits),
            "hits": hits,
            "index": meta,
        }
    finally:
        db.close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    prepare = commands.add_parser("build")
    prepare.add_argument("--root", type=Path, required=True)
    prepare.add_argument("--index", type=Path, required=True)
    prepare.add_argument("--source-id", required=True)
    query = commands.add_parser("search")
    query.add_argument("--index", type=Path, required=True)
    query.add_argument("--query", required=True)
    query.add_argument("--kind", action="append", choices=sorted(set(ROOTS.values())), default=[])
    query.add_argument("--prefix")
    query.add_argument("-k", type=int, default=10)
    query.add_argument("--json", action="store_true", help="JSON array of paths only")
    args = parser.parse_args()
    try:
        if args.command == "build":
            print(json.dumps(build(args.root, args.index, args.source_id), ensure_ascii=False))
        else:
            result = search(args.index, args.query, limit=args.k, kinds=args.kind, prefix=args.prefix)
            paths = [hit["path"] for hit in result["hits"]]
            if args.json:
                print(json.dumps(paths, ensure_ascii=False))
            elif paths:
                print("\n".join(paths))
            if result["status"] != "ok":
                print(result["status"], file=sys.stderr)
            return 3 if result["status"] == "no_coverage" else 0
        return 0
    except (OSError, ValueError, KeyError, sqlite3.Error) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
