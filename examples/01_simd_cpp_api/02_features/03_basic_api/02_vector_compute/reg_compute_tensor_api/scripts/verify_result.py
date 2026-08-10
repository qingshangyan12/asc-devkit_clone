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

import sys

import numpy as np


VECTOR_LENGTH = 64
FLOAT_RESULT_ROWS = 38
INT_RESULT_ROWS = 14
MATH_ROW_BEGIN = 1
MATH_ROW_END = 6
COMPARE_ROW_BEGIN = 31
COMPARE_ROW_END = 37
MATH_TOLERANCE = 5e-3
STANDARD_TOLERANCE = 1e-4


def load_result(path: str, dtype: np.dtype, rows: int) -> np.ndarray:
    result = np.fromfile(path, dtype=dtype)
    expected_size = rows * VECTOR_LENGTH
    if result.size != expected_size:
        raise ValueError(f"{path} contains {result.size} elements, expected {expected_size}")
    return result.reshape(rows, VECTOR_LENGTH)


def report_mismatch(kind: str, actual: np.ndarray, golden: np.ndarray, matches: np.ndarray) -> None:
    indexes = np.argwhere(~matches)
    for row, lane in indexes[:10]:
        print(
            f"{kind} mismatch at row {row}, lane {lane}: "
            f"actual={actual[row, lane]}, golden={golden[row, lane]}"
        )


def verify_float(actual_path: str, golden_path: str) -> bool:
    actual = load_result(actual_path, np.float32, FLOAT_RESULT_ROWS)
    golden = load_result(golden_path, np.float32, FLOAT_RESULT_ROWS)
    if not np.all(np.isfinite(actual)) or not np.all(np.isfinite(golden)):
        print("Float results contain non-finite values.")
        return False

    absolute_tolerance = np.full((FLOAT_RESULT_ROWS, 1), STANDARD_TOLERANCE, dtype=np.float32)
    relative_tolerance = np.full((FLOAT_RESULT_ROWS, 1), STANDARD_TOLERANCE, dtype=np.float32)
    absolute_tolerance[MATH_ROW_BEGIN:MATH_ROW_END] = MATH_TOLERANCE
    relative_tolerance[MATH_ROW_BEGIN:MATH_ROW_END] = MATH_TOLERANCE
    matches = np.isclose(actual, golden, rtol=relative_tolerance, atol=absolute_tolerance)
    matches[COMPARE_ROW_BEGIN:COMPARE_ROW_END] = (
        actual[COMPARE_ROW_BEGIN:COMPARE_ROW_END] == golden[COMPARE_ROW_BEGIN:COMPARE_ROW_END]
    )
    if not np.all(matches):
        report_mismatch("Float", actual, golden, matches)
        return False

    max_absolute_error = float(np.max(np.abs(actual - golden)))
    print(f"Float results passed, max absolute error: {max_absolute_error:.9g}")
    return True


def verify_int(actual_path: str, golden_path: str) -> bool:
    actual = load_result(actual_path, np.int32, INT_RESULT_ROWS)
    golden = load_result(golden_path, np.int32, INT_RESULT_ROWS)
    matches = actual == golden
    if not np.all(matches):
        report_mismatch("Int32", actual, golden, matches)
        return False
    print("Int32 results passed.")
    return True


def main() -> None:
    if len(sys.argv) != 5:
        raise ValueError(
            "Usage: verify_result.py <float_output> <float_golden> <int_output> <int_golden>"
        )
    passed = verify_float(sys.argv[1], sys.argv[2]) and verify_int(sys.argv[3], sys.argv[4])
    if not passed:
        raise ValueError("[ERROR] result error")
    print("test pass!")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error)
        sys.exit(1)
