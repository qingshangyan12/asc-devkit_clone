# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

"""Run frozen lexical cases; expectations never supply query terms."""

import argparse
import importlib.util
import json
from pathlib import Path
import time
import yaml


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--index", type=Path, required=True)
    p.add_argument("--cases", type=Path, default=Path(__file__).with_name("retrieval_cases.yaml"))
    p.add_argument("--report", type=Path)
    a = p.parse_args()
    script = Path(__file__).resolve().parents[1] / "scripts/keyword_search.py"
    spec = importlib.util.spec_from_file_location("keyword_engine", script)
    engine = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(engine)
    reports = []
    for case in yaml.safe_load(a.cases.read_text())["cases"]:
        filters = case.get("filters", {})
        started = time.perf_counter()
        result = engine.search(
            a.index,
            case["query"],
            limit=filters.get("k", 5),
            kinds=filters.get("kind", ()),
            prefix=filters.get("prefix"),
        )
        paths = [h["path"] for h in result["hits"]]
        expect = case["expect"]
        groups = expect.get("any_paths", [])
        ranks = [next((i for i, p in enumerate(paths, 1) if p in group), 0) for group in groups]
        passed = (
            all(ranks)
            and result["status"] == expect["status"]
            and ("count" not in expect or len(paths) == expect["count"])
        )
        reports.append(
            {
                "id": case["id"],
                "query": case["query"],
                "passed": passed,
                "ranks": ranks,
                "paths": paths,
                "status": result["status"],
                "elapsed_ms": 1000 * (time.perf_counter() - started),
                "index": result["index"],
            }
        )
    output = {"passed": sum(r["passed"] for r in reports), "total": len(reports), "cases": reports}
    if a.report:
        a.report.write_text(json.dumps(output, ensure_ascii=False, indent=2) + "\n")
    print(json.dumps({"passed": output["passed"], "total": output["total"]}))
    return int(output["passed"] != output["total"])


if __name__ == "__main__":
    raise SystemExit(main())
