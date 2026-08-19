#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the license.

import sys

import numpy as np


VECTOR_LENGTH = 64
ROW_NAMES = (
    "abs(reg)",
    "exp(reg)",
    "sqrt(reg)",
    "log(reg)",
    "log2(reg)",
    "log10(reg)",
    "-reg",
    "relu(reg)",
    "prelu(reg, reg)",
    "leaky_relu(reg, scalar)",
    "reg + reg",
    "reg + scalar",
    "scalar + reg",
    "reg - reg",
    "reg - scalar",
    "scalar - reg",
    "reg * reg",
    "reg * scalar",
    "scalar * reg",
    "reg / reg",
    "reg / scalar",
    "scalar / reg",
    "max(reg, reg)",
    "max(reg, scalar)",
    "max(scalar, reg)",
    "min(reg, reg)",
    "min(reg, scalar)",
    "min(scalar, reg)",
)
FLOAT_ROWS = len(ROW_NAMES)
MATH_ROWS = (1, 2, 3, 4, 5)
UINT32_ROW_NAMES = (
    "add low",
    "add carry",
    "add_c low",
    "add_c carry",
    "sub low",
    "sub carry (1 means no borrow)",
    "sub_c low",
    "sub_c carry (1 means no borrow)",
    "mull low",
    "mull high",
)
UINT32_ROWS = len(UINT32_ROW_NAMES)


def main() -> None:
    if len(sys.argv) != 5:
        raise ValueError(
            "Usage: verify_result.py <float_output> <float_golden> "
            "<uint32_output> <uint32_golden>"
        )
    expected_elements = FLOAT_ROWS * VECTOR_LENGTH
    actual = np.fromfile(sys.argv[1], dtype=np.float32)
    golden = np.fromfile(sys.argv[2], dtype=np.float32)
    if actual.size != expected_elements or golden.size != expected_elements:
        raise ValueError(
            f"expected {expected_elements} float32 elements in each file, "
            f"got output={actual.size}, golden={golden.size}"
        )
    actual = actual.reshape(FLOAT_ROWS, VECTOR_LENGTH)
    golden = golden.reshape(FLOAT_ROWS, VECTOR_LENGTH)
    matches = np.isclose(actual, golden, rtol=1e-4, atol=1e-4)
    matches[list(MATH_ROWS)] = np.isclose(
        actual[list(MATH_ROWS)], golden[list(MATH_ROWS)], rtol=5e-3, atol=5e-3
    )
    if not np.all(matches):
        row, lane = np.argwhere(~matches)[0]
        raise ValueError(
            f"mismatch in {ROW_NAMES[row]} (row {row}, lane {lane}): "
            f"actual={actual[row, lane]}, golden={golden[row, lane]}"
        )
    max_error = np.max(np.abs(actual - golden))

    uint32_expected_elements = UINT32_ROWS * VECTOR_LENGTH
    uint32_actual = np.fromfile(sys.argv[3], dtype=np.uint32)
    uint32_golden = np.fromfile(sys.argv[4], dtype=np.uint32)
    if uint32_actual.size != uint32_expected_elements or uint32_golden.size != uint32_expected_elements:
        raise ValueError(
            f"expected {uint32_expected_elements} uint32 elements in each file, "
            f"got output={uint32_actual.size}, golden={uint32_golden.size}"
        )
    uint32_actual = uint32_actual.reshape(UINT32_ROWS, VECTOR_LENGTH)
    uint32_golden = uint32_golden.reshape(UINT32_ROWS, VECTOR_LENGTH)
    if not np.array_equal(uint32_actual, uint32_golden):
        row, lane = np.argwhere(uint32_actual != uint32_golden)[0]
        raise ValueError(
            f"mismatch in {UINT32_ROW_NAMES[row]} (row {row}, lane {lane}): "
            f"actual={uint32_actual[row, lane]}, golden={uint32_golden[row, lane]}"
        )

    total_rows = FLOAT_ROWS + UINT32_ROWS
    total_elements = expected_elements + uint32_expected_elements
    print(f"test pass! verified {total_rows} result rows ({total_elements} elements).")
    print(f"max float absolute error: {max_error:.9g}; uint32 results matched exactly.")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error)
        sys.exit(1)
