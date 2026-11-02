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

"""Read-only Agent interface to a prepared Ascend C API index."""
from __future__ import annotations

import json
from pathlib import Path

import query
import ensure_index

PLATFORMS = {"a2": ("2201", "910b"), "a3": ("2201", "A3"), "950": ("3510", "950"),
             "310b": ("3002", "310b"), "310p": ("2002", "310p"), "910": ("1001", "910")}


def search(args):
    root = args.root.resolve()
    index_dir = args.index_dir
    if index_dir is None:
        raise ValueError("--index-dir is required; prepare an index before searching")
    index, gaps = query.load(index_dir)
    fresh, reasons = ensure_index.light_fresh(index_dir, root, ensure_index.build_index.SCOPES[args.surface])
    if not fresh:
        raise ValueError("stale index; run preparation: " + "; ".join(reasons[:5]))
    if args.version:
        query.version_target(index, args.version)
    if args.platform:
        arch, product = PLATFORMS[args.platform]
        if index["meta"]["arch"] != arch:
            raise ValueError("platform conflicts with prepared index architecture")
        if args.product and product not in query.csv(args.product):
            raise ValueError("--product conflicts with --platform")
        if args.gap is None:
            args.product = product
    keywords = args.keywords or []
    has_query = bool(query._active_filter_dims(args) or args.api or args.deps_of or args.enum or args.param or args.gap)
    rows = query.run_query(args, index, gaps) if has_query else [
        {"path": path, "why": []} for path in index["scope"]["primary_docs"]
    ]
    if not has_query and not keywords:
        raise ValueError("provide keywords or at least one structured query dimension")
    if keywords:
        filtered = []
        for row in rows:
            doc = index["documents"].get(row["path"], {})
            text = " ".join(str(doc.get(k, "")) for k in ("title", "description", "family", "tags")).casefold()
            if all(word.casefold() in text for word in keywords):
                row["why"] += ["keyword:" + word for word in keywords]
                filtered.append(row)
        rows = filtered
    total = len(rows)
    if args.limit < 1:
        raise ValueError("--limit must be positive")
    rows = rows[:args.limit]
    for row in rows:
        rel = row.get("path", "")
        target = (root / rel).resolve()
        target.relative_to(root)
        row["local_path"] = str(target)
        doc = index["documents"].get(rel)
        if doc:
            row.update({key: doc[key] for key in ("title", "description", "version", "deprecated", "headers", "examples", "signatures")})
            row["header_locations"] = [str(root / (p if p.startswith("include/") else "include/" + p)) for p in doc["headers"]]
            row["example_locations"] = [dict(e, local_path=str(root / e["path"].lstrip("/") / e["file"])) for e in doc["examples"]]
    return {"schema": "asc-search-results/1", "source": "asc-devkit",
            "source_commit": index["meta"]["source_commit"], "repository": args.repository,
            "root": str(root), "cann_version": index["meta"]["cann_version"],
            "arch": index["meta"]["arch"], "platform": args.platform,
            "count": len(rows), "matched_count": total, "truncated": total > len(rows), "results": rows}


def main(argv=None):
    parser = query.parser()
    parser.description = __doc__
    parser.add_argument("--root", type=Path, required=True)
    parser.add_argument("--keywords", nargs="+", help="short keywords; all must match title/description/family/tags")
    parser.add_argument("--platform", choices=sorted(PLATFORMS))
    parser.add_argument("--repository", default="https://gitcode.com/cann/asc-devkit.git")
    parser.add_argument("--limit", type=int, default=20)
    try:
        result = search(parser.parse_args(argv))
        print(json.dumps(result, ensure_ascii=False, separators=(",", ":")))
        return 0
    except (ValueError, OSError, KeyError, TypeError) as exc:
        print(json.dumps({"status": "error", "error": str(exc)}, ensure_ascii=False))
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
