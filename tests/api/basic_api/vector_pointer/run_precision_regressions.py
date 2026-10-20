# Copyright (c) 2026 ddplys.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Instrument copies of the actual regression sources; never invent metrics."""

import argparse
import hashlib
import json
import os
import re
from pathlib import Path
import shutil
import subprocess
from source_utils import replace_cpp


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    here = Path(__file__).resolve().parent
    root = here.parents[3]
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    sources = root / "examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic"
    shutil.copy2(here / "precision_metrics.h", out / "precision_metrics.h")
    results = []
    for suite in ["elementwise", "conversion", "index", "transpose", "layout"]:
        original = sources / (
            "vector_pointer_regression.asc" if suite == "elementwise" else f"vector_pointer_{suite}_regression.asc"
        )
        text = original.read_text()
        text = '#include "precision_metrics.h"\n' + text
        # Only host validation loops, never kernel mode loops.
        point = text.index("bool Run(")
        before, host = text[:point], text[point:]
        marker = "for (int mode = 0;"
        assert host.count(marker) == 1, suite
        host = host.replace(marker, "            VectorPrecisionMetrics precision;\n" + marker)
        decl = (
            "DT value = output[mode * CAPACITY + i];"
            if suite == "conversion"
            else "T value = output[(mode*2+side)*CAPACITY+i];"
            if suite == "layout"
            else "T value = output[mode * CAPACITY + i];"
        )
        valid = (
            "i < produced"
            if suite == "index"
            else "(Op >= 34 && Op <= 36) ? i == 0 : i < count"
            if suite == "elementwise"
            else "i < count"
        )
        host = replace_cpp(
            host, decl, decl + "\n                    if (" + valid + ") { precision.Observe(value, expected); }"
        )
        host, count = re.subn(
            r'std::printf\s*\(\s*"PASS ',
            lambda m: "if (!precision.Report(name,count,form)) { return false; }\n" + m[0],
            host,
        )
        assert count == 1, suite
        generated = out / original.name
        generated.write_text(before + host)
        binary = out / (suite + "_precision")
        cmd = ["bisheng", "-x", "asc", "--npu-arch=dav-3510", "-std=c++17", "-O2"]
        cmd += [
            "-iquote" + str(root / d)
            for d in [".", "include", "include/basic_api", "include/utils", "impl", "impl/basic_api"]
        ]
        cmd += [
            "-I" + str(root),
            str(generated),
            "-L" + os.environ["ASCEND_HOME_PATH"] + "/lib64",
            "-lascendcl",
            "-lm",
            "-o",
            str(binary),
        ]
        record = {
            "suite": suite,
            "source": str(generated),
            "source_sha256": hashlib.sha256(generated.read_bytes()).hexdigest(),
            "command": cmd,
        }
        with (out / (suite + "_build.log")).open("w") as f:
            r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT)
        record["build_returncode"] = r.returncode
        if r.returncode == 0:
            with (out / (suite + "_npu.log")).open("w") as f:
                r = subprocess.run([str(binary)], stdout=f, stderr=subprocess.STDOUT, timeout=180)
            record["run_returncode"] = r.returncode
        results.append(record)
        (out / "summary.json").write_text(json.dumps(results, indent=2))
        print(suite, record.get("run_returncode", "build failed"), flush=True)
    if any(r.get("build_returncode") != 0 or r.get("run_returncode") != 0 for r in results):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
