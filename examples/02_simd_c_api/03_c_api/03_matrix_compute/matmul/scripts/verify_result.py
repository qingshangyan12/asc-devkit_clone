#!/usr/bin/env python3

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
import sys
from pathlib import Path

import numpy as np


OUTPUT_ELEMENTS = 30 * 70
RTOL = 2.0**-10
ATOL = 2.0**-16
REQUIRED_MATCHED_RATIO = 0.99
MAX_ABS_ERROR_LIMIT = 1.0e-2
MAX_ULP_ERROR_LIMIT = 32


def ordered_float32(values: np.ndarray) -> np.ndarray:
    bits = (
        np.ascontiguousarray(values, dtype=np.float32).view(np.uint32).astype(np.uint64)
    )
    sign = (bits & np.uint64(0x80000000)) != 0
    magnitude = bits & np.uint64(0x7FFFFFFF)
    return np.where(
        sign, np.uint64(0x80000000) - magnitude, np.uint64(0x80000000) + magnitude
    )


def load_exact(
    path: Path, dtype: np.dtype, expected_count: int, label: str
) -> np.ndarray:
    if not path.is_file():
        raise ValueError(f"{label} file does not exist: {path}")
    values = np.fromfile(path, dtype=dtype)
    if values.size != expected_count:
        raise ValueError(
            f"{label}: expected {expected_count} {np.dtype(dtype).name} values, got {values.size}"
        )
    return values


def verify(actual_path: Path, golden_path: Path) -> bool:
    actual = load_exact(actual_path, np.float32, OUTPUT_ELEMENTS, "actual")
    golden = load_exact(golden_path, np.float64, OUTPUT_ELEMENTS, "golden")
    finite = np.isfinite(actual) & np.isfinite(golden)
    non_finite_count = int(OUTPUT_ELEMENTS - np.count_nonzero(finite))

    abs_error = np.full(OUTPUT_ELEMENTS, np.inf, dtype=np.float64)
    abs_error[finite] = np.abs(actual[finite].astype(np.float64) - golden[finite])
    tolerance = ATOL + RTOL * np.abs(golden)
    matched = finite & (abs_error <= tolerance)
    matched_ratio = float(np.count_nonzero(matched) / OUTPUT_ELEMENTS)

    expected_float32 = golden.astype(np.float32)
    ulp_error = np.full(OUTPUT_ELEMENTS, np.iinfo(np.uint64).max, dtype=np.uint64)
    finite_ulp = np.isfinite(actual) & np.isfinite(expected_float32)
    actual_ordered = ordered_float32(actual[finite_ulp])
    expected_ordered = ordered_float32(expected_float32[finite_ulp])
    ulp_error[finite_ulp] = np.maximum(actual_ordered, expected_ordered) - np.minimum(
        actual_ordered, expected_ordered
    )

    worst_index = int(np.argmax(abs_error))
    max_abs_error = float(abs_error[worst_index])
    max_ulp_error = int(ulp_error.max())
    max_abs_error_ulp = int(ulp_error[worst_index])
    maximum_error_ok = (
        max_abs_error <= MAX_ABS_ERROR_LIMIT or max_abs_error_ulp <= MAX_ULP_ERROR_LIMIT
    )
    passed = matched_ratio >= REQUIRED_MATCHED_RATIO and maximum_error_ok

    print(f"matched_ratio={matched_ratio:.6f}")
    print(f"non_finite_count={non_finite_count}")
    print(f"max_abs_error={max_abs_error:.9e}")
    print(f"max_ulp_error={max_ulp_error}")
    print(f"max_abs_error_ulp={max_abs_error_ulp}")
    print(f"worst_index={worst_index}")
    print(f"actual={float(actual[worst_index]):.9e}")
    print(f"golden={float(golden[worst_index]):.9e}")
    print(f"status={'PASSED' if passed else 'FAILED'}")
    return passed


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Verify fixed-shape FP32 MatMul output."
    )
    parser.add_argument(
        "actual", type=Path, help="NPU output file containing 2100 float32 values."
    )
    parser.add_argument(
        "golden", type=Path, help="Golden file containing 2100 float64 values."
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        return 0 if verify(args.actual, args.golden) else 1
    except ValueError as error:
        print(str(error), file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
