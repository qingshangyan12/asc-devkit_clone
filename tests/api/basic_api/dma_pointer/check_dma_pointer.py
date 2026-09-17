#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Compile DMA public calls with real ASC address spaces; never run a kernel."""

import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess


# (official row, architecture, destination space/type, source space/type, call).
# Positions and dtype pairs come from the public API contract, not helper traits.
CASES = [
    (
        1,
        "3510",
        "gm",
        "uint64_t",
        None,
        None,
        "DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(dst);",
    ),
    (
        2,
        "3510",
        "ubuf",
        "uint64_t",
        None,
        None,
        "DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(dst);",
    ),
    (
        3,
        "3510",
        "gm",
        "uint64_t",
        None,
        None,
        "DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE>(dst);",
    ),
    (
        4,
        "3510",
        "gm",
        "uint64_t",
        None,
        None,
        "DataCachePreload<int16_t>(dst, int16_t(0)); DataCachePreload<int64_t>(dst, int64_t(64));",
    ),
    (
        5,
        "2201",
        "cc",
        "bfloat16_t",
        "cbuf",
        "float",
        "DataCopy(dst, src, params, enhanced);",
    ),
    (
        6,
        "3510",
        "ubuf",
        "int16_t",
        "cc",
        "int32_t",
        "DataCopy(dst, src, params, enhanced);",
    ),
    (
        7,
        "3510",
        "ubuf",
        "int8_t",
        "cc",
        "int32_t",
        "DataCopy(dst, src, params, enhanced);",
    ),
    (
        8,
        "3510",
        "ubuf",
        "uint8_t",
        "cc",
        "int32_t",
        "DataCopy(dst, src, params, enhanced);",
    ),
    (9, "3510", "cc", "float", "ubuf", "half", "DataCopy(dst, src, params, enhanced);"),
    (
        10,
        "3510",
        "cc",
        "half",
        "cbuf",
        "float",
        "DataCopy(dst, src, params, enhanced);",
    ),
    (
        11,
        "3510",
        "ubuf",
        "half",
        "cc",
        "int32_t",
        "DataCopy(dst, src, params, enhanced);",
    ),
    (12, "3510", "cbuf", "half", "gm", "half", "DataCopy<half>(dst, src, dn);"),
    (13, "3510", "cbuf", "half", "gm", "half", "DataCopy<half>(dst, src, nd);"),
    (14, "3510", "gm", "half", "cc", "float", "DataCopy<half, float>(dst, src, co);"),
    (15, "3510", "cbuf", "half", "cc", "float", "DataCopy<half, float>(dst, src, co);"),
    (
        16,
        "3510",
        "biasbuf",
        "float",
        "cbuf",
        "half",
        "DataCopy<float, half>(dst, src, params);",
    ),
    (
        17,
        "3510",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopy<float, 2>(dst, src, multi);",
    ),
    (
        18,
        "3510",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopy<float>(dst, src, params, enhanced);",
    ),
    (19, "2201", "cbuf", "half", "gm", "half", "DataCopy<half>(dst, src, nd);"),
    (
        20,
        "3510",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopy<float>(dst, src, slices, slices, 1);",
    ),
    (
        21,
        "3510",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopy<float>(dst, src, uint32_t(32));",
    ),
    (
        22,
        "3510",
        "gm",
        "float",
        "ubuf",
        "float",
        "DataCopy<float>(dst, src, params, enhanced);",
    ),
    (23, "3510", "gm", "float", "ubuf", "float", "DataCopy<float>(dst, src, params);"),
    (24, "3510", "gm", "float", "ubuf", "float", "DataCopy<float>(dst, src, nz);"),
    (
        25,
        "3510",
        "gm",
        "float",
        "ubuf",
        "float",
        "DataCopy<float>(dst, src, slices, slices);",
    ),
    (
        26,
        "3510",
        "gm",
        "float",
        "ubuf",
        "float",
        "DataCopy<float>(dst, src, uint32_t(32));",
    ),
    (
        27,
        "3510",
        "ubuf",
        "float",
        "ubuf",
        "float",
        "DataCopy<float>(dst, src, params, enhanced);",
    ),
    (28, "3510", "cbuf", "half", "ubuf", "half", "DataCopy<half>(dst, src, params);"),
    (29, "3510", "cbuf", "half", "ubuf", "half", "DataCopy<half>(dst, src, nd);"),
    (
        30,
        "3510",
        "cbuf",
        "half",
        "ubuf",
        "half",
        "DataCopy<half>(dst, src, uint32_t(32));",
    ),
    (31, "3510", "ubuf", "float", "gm", "float", "DataCopy<float>(dst, src, params);"),
    (
        32,
        "3510",
        "ubuf",
        "float",
        "cbuf",
        "float",
        "DataCopyL1ToUB<float, 1>(dst, src, params);",
    ),
    (
        33,
        "3510",
        "ubuf",
        "float",
        "cbuf",
        "float",
        "DataCopyL1ToUB<float>(dst, src, uint32_t(32));",
    ),
    (34, "3510", "ubuf", "float", "gm", "float", "DataCopyPad(dst, src, ext, padExt);"),
    (
        35,
        "3510",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopyPad<float, PaddingMode::Compact>(dst, src, ext, padExt);",
    ),
    (
        36,
        "3510",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopyPad<float>(dst, src, params, pad);",
    ),
    (37, "3510", "gm", "float", "ubuf", "float", "DataCopyPad<float>(dst, src, ext);"),
    (
        38,
        "3510",
        "gm",
        "float",
        "ubuf",
        "float",
        "DataCopyPad<float>(dst, src, params);",
    ),
    (
        39,
        "2201",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopyPad<float>(dst, src, ext, padExt);",
    ),
    (
        40,
        "2201",
        "ubuf",
        "float",
        "gm",
        "float",
        "DataCopyPad<float>(dst, src, params, pad);",
    ),
    (41, "2201", "gm", "float", "ubuf", "float", "DataCopyPad<float>(dst, src, ext);"),
    (
        42,
        "2201",
        "gm",
        "float",
        "ubuf",
        "float",
        "DataCopyPad<float>(dst, src, params);",
    ),
    (
        43,
        "3510",
        "cbuf",
        "half",
        "ubuf",
        "half",
        "DataCopyPad<half>(dst, src, ext, nd);",
    ),
    (
        44,
        "3510",
        "cbuf",
        "half",
        "ubuf",
        "half",
        "DataCopyPad<half>(dst, src, params, nd);",
    ),
]

PARAMETERS = """
    DataCopyParams params{1, 1, 0, 0};
    DataCopyEnhancedParams enhanced;
    DataCopyCO12DstParams co;
    DataCopyExtParams ext{1, 32, 0, 0, 0};
    DataCopyPadParams pad;
    DataCopyPadExtParams<float> padExt;
    Nd2NzParams nd;
    Dn2NzParams dn;
    Nz2NdParamsFull nz;
    SliceInfo slices[1] = {{0, 7, 0, 1, 8}};
"""


def operand(name, space, dtype, form, trait=False):
    if form == "P":
        return f"    __{space}__ {dtype}* {name} = nullptr;\n"
    tensor = "GlobalTensor" if space == "gm" else "LocalTensor"
    element = f"TensorTrait<{dtype}>" if trait else dtype
    return f"    {tensor}<{element}> {name};\n"


def source(case, form):
    row, arch, dst_space, dst_type, src_space, src_type, call = case
    declarations = operand("dst", dst_space, dst_type, form[0], row == 34)
    if src_space:
        declarations += operand("src", src_space, src_type, form[1], row == 34)
    params = "" if row <= 4 else PARAMETERS
    if arch == "3510" and row > 4:
        params += "    MultiCopyParams<float, 2> multi;\n"
    header = "cache" if row <= 4 else "data_copy"
    return (
        f'#include "basic_api/kernel_operator_{header}_intf.h"\nusing namespace AscendC;\n'
        'extern "C" __global__ __aicore__ void TestDmaPointer()\n{\n'
        + declarations
        + params
        + "    "
        + call
        + "\n}\n"
    )


def compile_one(compiler, repo, output, name, arch, code, umbrella=False):
    public_headers = [
        "basic_api/kernel_operator_data_copy_intf.h",
        "basic_api/kernel_operator_cache_intf.h",
    ]
    headers = [header for header in public_headers if f'#include "{header}"' in code]
    for header in headers:
        selected = "kernel_operator.h" if umbrella else header
        code = code.replace(
            f'#include "{header}"', f'#include "{repo}/include/{selected}"'
        )
    case_dir = output / name
    case_dir.mkdir(exist_ok=True)
    path = case_dir / (name + ".asc")
    path.write_text(code)
    include_dirs = [
        ".",
        "include",
        "include/basic_api",
        "include/basic_api/reg_compute",
        "include/simt_api",
        "include/simt_api/cpp",
        "impl",
        "impl/basic_api",
        "impl/basic_api/reg_compute",
        "impl/simt_api",
        "build/include",
    ]
    command = [
        compiler,
        "-x",
        "asc",
        f"--npu-arch=dav-{arch}",
        "--aicore-only",
        "-std=c++17",
        "-DASCENDC_DUMP=0",
        "-fsyntax-only",
        "-H",
    ]
    command += [flag for item in include_dirs for flag in ("-iquote", str(repo / item))]
    command += [f"-I{repo / item}" for item in include_dirs]
    command.append(str(path))
    run = subprocess.run(
        command,
        cwd=case_dir,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    (output / (name + ".log")).write_text(run.stdout)
    required_headers = [str(repo / "include" / header) for header in headers]
    verified_headers = all(header in run.stdout for header in required_headers)
    contaminated = any(
        "/basic_api/" in line
        and str(repo) not in line
        and ("/Ascend/" in line or "/ascend/" in line)
        for line in run.stdout.splitlines()
        if line.startswith(".")
    )
    return {
        "id": name,
        "arch": arch,
        "command": command,
        "exit_code": run.returncode,
        "worktree_headers_verified": verified_headers and not contaminated,
        "source_sha256": hashlib.sha256(code.encode()).hexdigest(),
        "log": name + ".log",
        "constraint_diagnostic": bool(
            re.search("no matching function|static assertion failed", run.stdout)
        ),
    }


def extra_cases():
    control = source(CASES[30], "PP")
    yield "LP-C01-control", control, True
    yield (
        "LP-C01-array",
        control.replace("__ubuf__ float* dst = nullptr;", "__ubuf__ float dst[32];"),
        True,
    )
    yield "LP-C01-deduced", control.replace("DataCopy<float>", "DataCopy"), True
    yield (
        "LP-C05-explicit-trait-pointer",
        source(CASES[33], "PP").replace(
            "DataCopyPad(dst", "DataCopyPad<TensorTrait<float>, float>(dst"
        ),
        True,
    )
    yield (
        "LP-C02-const-source",
        control.replace("__gm__ float* src", "__gm__ const float* src"),
        True,
    )
    yield (
        "LP-C02-const-destination",
        control.replace("__ubuf__ float* dst", "__ubuf__ const float* dst"),
        False,
    )
    yield (
        "LP-C03-gm-to-gm",
        control.replace("__ubuf__ float* dst", "__gm__ float* dst"),
        False,
    )
    yield (
        "LP-C03-host-pointer",
        control.replace("__ubuf__ float* dst", "float* dst"),
        False,
    )
    yield (
        "LP-C04-mismatched-type",
        control.replace("__gm__ float* src", "__gm__ half* src"),
        False,
    )
    yield (
        "LP-C13-smallc0",
        source(CASES[12], "PP").replace("DataCopy<half>", "DataCopy<half, true>"),
        True,
    )
    yield (
        "LP-C13-explicit-false-pointer",
        source(CASES[12], "PP").replace("DataCopy<half>", "DataCopy<half, false>"),
        True,
    )
    yield (
        "LP-C13-explicit-false-tensor",
        source(CASES[12], "TT").replace("DataCopy<half>", "DataCopy<half, false>"),
        True,
    )
    yield (
        "LP-C13-explicit-false-mixed",
        source(CASES[12], "TP").replace("DataCopy<half>", "DataCopy<half, false>"),
        True,
    )
    yield (
        "LP-C13-dn-explicit-false-tensor",
        source(CASES[11], "TT").replace("DataCopy<half>", "DataCopy<half, false>"),
        True,
    )
    yield (
        "LP-C13-dn-explicit-false-mixed",
        source(CASES[11], "TP").replace("DataCopy<half>", "DataCopy<half, false>"),
        True,
    )
    yield "B08-nddma-control", source(CASES[16], "PP"), True
    yield (
        "B08-nddma-dim-zero",
        source(CASES[16], "PP").replace("<float, 2>(", "<float, 0>("),
        False,
    )
    yield (
        "B08-nddma-dim-six",
        source(CASES[16], "PP").replace("<float, 2>(", "<float, 6>("),
        False,
    )
    yield "LP-C15-preload-control", source(CASES[3], "P"), True
    yield (
        "LP-C15-preload-offset",
        source(CASES[3], "P").replace(
            "DataCachePreload<int16_t>(dst, int16_t(0)); DataCachePreload<int64_t>(dst, int64_t(64));",
            "DataCachePreload<float>(dst, float(0));",
        ),
        False,
    )


def source_snapshot(repo):
    files = [Path(__file__).resolve()]
    for directory in ("include/basic_api", "impl/basic_api"):
        files.extend(sorted((repo / directory).rglob("*.h")))
    return {
        str(path.relative_to(repo)): hashlib.sha256(path.read_bytes()).hexdigest()
        for path in files
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="bisheng")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--rows", help="Comma-separated official row IDs (1-44)")
    parser.add_argument("--forms", default="TT,PP,TP,PT")
    parser.add_argument("--jobs", type=int, default=1)
    parser.add_argument("--extras", action="store_true")
    parser.add_argument(
        "--umbrella",
        action="store_true",
        help="Also check the complete kernel_operator.h include graph",
    )
    args = parser.parse_args()
    compiler = shutil.which(args.compiler)
    if compiler is None:
        parser.error("ASC compiler not found")
    repo = Path(__file__).resolve().parents[4]
    output = args.output.resolve()
    if output.exists() and any(output.iterdir()):
        parser.error("Use a new empty output directory for each run")
    output.mkdir(parents=True, exist_ok=True)
    snapshot_before = source_snapshot(repo)
    rows = (
        {int(value) for value in args.rows.split(",")}
        if args.rows
        else set(range(1, 45))
    )
    if not rows <= set(range(1, 45)):
        parser.error("Rows must be in 1..44")
    forms = args.forms.split(",")
    if not set(forms) <= {"TT", "PP", "TP", "PT"}:
        parser.error("Forms must be TT, PP, TP, PT")
    results = []
    pending = []
    for case in CASES:
        row, arch = case[:2]
        if row not in rows:
            continue
        selected = list(dict.fromkeys(form[0] for form in forms)) if row <= 4 else forms
        for form in selected:
            name = f"DMA-{row:03d}-{form}"
            if row == 34 and form == "PP":
                results.append(
                    {
                        "id": name,
                        "status": "NOT_APPLICABLE",
                        "reason": "Pure pointers do not retain TensorTrait identity; covered by row 35.",
                    }
                )
                continue
            pending.append((name, arch, source(case, form), True))
    if args.extras:
        pending.extend(
            (name, "3510", code, expected) for name, code, expected in extra_cases()
        )

    def run_case(item):
        name, arch, code, expected = item
        result = compile_one(compiler, repo, output, name, arch, code, args.umbrella)
        result["expected_compile_success"] = expected
        passed = (
            result["exit_code"] == 0
            if expected
            else (result["exit_code"] != 0 and result["constraint_diagnostic"])
        )
        result["status"] = "PASS" if passed else "FAIL"
        if not result["worktree_headers_verified"]:
            result["status"] = "BLOCKED"
            result["reason"] = (
                "Header trace does not prove an uncontaminated worktree include graph."
            )
        print(name, result["status"], flush=True)
        return result

    with ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
        for result in pool.map(run_case, pending):
            results.append(result)
    controls = [item for item in results if item["id"] == "LP-C01-control"]
    if args.extras and (not controls or controls[0]["status"] != "PASS"):
        for result in results:
            if result.get("expected_compile_success") is False:
                result["status"] = "BLOCKED"
                result["reason"] = (
                    "Positive control did not compile; negative result is not evidence."
                )
    version = subprocess.run(
        [compiler, "--version"],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    dependencies = {}
    for log in output.glob("*.log"):
        for line in log.read_text().splitlines():
            if not line.startswith("."):
                continue
            dependency = Path(line.lstrip(". ").split(" (")[0])
            if dependency.is_file() and dependency.resolve().is_relative_to(repo):
                relative = str(dependency.resolve().relative_to(repo))
                if relative not in dependencies:
                    dependencies[relative] = hashlib.sha256(
                        dependency.read_bytes()
                    ).hexdigest()
    snapshot_after = source_snapshot(repo)
    changed = sorted(
        path
        for path in snapshot_before.keys() | snapshot_after.keys()
        if snapshot_before.get(path) != snapshot_after.get(path)
    )
    report = {
        "kind": "target_syntax_compile_only",
        "compiler": compiler,
        "compiler_version": version.stdout,
        "compiler_version_exit_code": version.returncode,
        "ascendc_dump": 0,
        "umbrella": args.umbrella,
        "results": results,
        "source_snapshot_before": snapshot_before,
        "source_snapshot_after": snapshot_after,
        "source_changed_during_run": changed,
        "result_validity": "INVALID_SOURCE_CHANGED" if changed else "SOURCE_STABLE",
        "worktree_dependency_sha256_at_completion": dependencies,
        "device_execution": "NOT_RUN",
        "cpu_execution": "NOT_RUN",
    }
    (output / "results.json").write_text(json.dumps(report, indent=2) + "\n")
    return int(
        bool(changed) or any(item["status"] in {"FAIL", "BLOCKED"} for item in results)
    )


if __name__ == "__main__":
    raise SystemExit(main())
