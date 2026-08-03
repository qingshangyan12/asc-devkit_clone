#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import json
import sys
from pathlib import Path

import numpy as np


EXPECTED_ELEMENTS = 30 * 70
RTOL = 2.0**-10
ATOL = 2.0**-16
REQUIRED_MATCHED_RATIO = 0.99
BASE_MAX_ABS_ERROR_LIMIT = 1.0e-2


def _load(path: Path):
    data = np.fromfile(path, dtype=np.float32)
    if data.size != EXPECTED_ELEMENTS:
        raise ValueError(
            f"{path} contains {data.size} float32 elements; expected {EXPECTED_ELEMENTS}"
        )
    return data


def verify(output_path: Path, golden_path: Path):
    output = _load(output_path)
    golden = _load(golden_path)

    abs_error = np.abs(output.astype(np.float64) - golden.astype(np.float64))
    matched = np.isclose(output, golden, rtol=RTOL, atol=ATOL, equal_nan=True)
    matched_ratio = float(np.count_nonzero(matched)) / float(golden.size)

    finite_golden = np.abs(golden[np.isfinite(golden)])
    if finite_golden.size:
        ulp_limit = float(np.max(32.0 * np.spacing(finite_golden).astype(np.float64)))
    else:
        ulp_limit = 0.0
    max_abs_error_limit = max(BASE_MAX_ABS_ERROR_LIMIT, ulp_limit)

    max_abs_index = int(np.argmax(abs_error))
    max_abs_error = float(abs_error[max_abs_index])
    passed = matched_ratio >= REQUIRED_MATCHED_RATIO and max_abs_error <= max_abs_error_limit

    mismatch_indices = np.flatnonzero(~matched)
    for index in mismatch_indices[:20]:
        tolerance = ATOL + RTOL * abs(float(golden[index]))
        print(
            f"index={index:04d}, expected={golden[index]: .9e}, actual={output[index]: .9e}, "
            f"abs_error={abs_error[index]:.9e}, mixed_tolerance={tolerance:.9e}"
        )

    report = {
        "passed": passed,
        "element_count": int(golden.size),
        "matched_count": int(np.count_nonzero(matched)),
        "matched_ratio": matched_ratio,
        "required_matched_ratio": REQUIRED_MATCHED_RATIO,
        "rtol": RTOL,
        "atol": ATOL,
        "max_abs_error": max_abs_error,
        "max_abs_error_index": max_abs_index,
        "max_abs_error_expected": float(golden[max_abs_index]),
        "max_abs_error_actual": float(output[max_abs_index]),
        "base_max_abs_error_limit": BASE_MAX_ABS_ERROR_LIMIT,
        "ulp_32_limit": ulp_limit,
        "effective_max_abs_error_limit": max_abs_error_limit,
    }
    return passed, report


def main():
    parser = argparse.ArgumentParser(description="Verify MatMul output using the opbase FP32 precision standard.")
    parser.add_argument("output", type=Path)
    parser.add_argument("golden", type=Path)
    parser.add_argument("--report", type=Path, default=Path("output/verify_report.json"))
    args = parser.parse_args()

    try:
        passed, report = verify(args.output, args.golden)
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        print(json.dumps(report, indent=2))
        if not passed:
            raise ValueError("result does not satisfy the FP32 precision standard")
        print("test pass!")
    except Exception as error:
        print(f"[ERROR] {error}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
