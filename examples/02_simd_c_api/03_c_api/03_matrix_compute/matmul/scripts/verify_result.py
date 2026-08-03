#!/usr/bin/env python3
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
from array import array
import math
import sys


RTOL = 2**-10
ATOL = 2**-16
REQUIRED_MATCHED_RATIO = 0.99
MAX_ABSOLUTE_ERROR = 1e-2
EXPECTED_ELEMENTS = 30 * 70


def float32_ulp(value):
    value = abs(value)
    if value == 0.0 or value < 2**-126:
        return 2**-149
    _, exponent = math.frexp(value)
    return 2 ** (exponent - 24)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output")
    parser.add_argument("golden")
    args = parser.parse_args()

    actual = array("f")
    expected = array("f")
    with open(args.output, "rb") as file:
        actual.frombytes(file.read())
    with open(args.golden, "rb") as file:
        expected.frombytes(file.read())
    if len(actual) != EXPECTED_ELEMENTS or len(expected) != EXPECTED_ELEMENTS:
        print(
            f"invalid element count: output={len(actual)}, golden={len(expected)}, "
            f"expected={EXPECTED_ELEMENTS}"
        )
        return 1

    failed_indices = [
        index
        for index, (actual_value, expected_value) in enumerate(zip(actual, expected))
        if not (
            math.isfinite(actual_value)
            and math.isfinite(expected_value)
            and abs(actual_value - expected_value) <= ATOL + RTOL * abs(expected_value)
        )
    ]
    absolute_errors = [
        abs(actual_value - expected_value)
        for actual_value, expected_value in zip(actual, expected)
    ]
    relative_errors = [
        absolute_error / max(abs(expected_value), ATOL)
        for absolute_error, expected_value in zip(absolute_errors, expected)
    ]
    matched_ratio = 1.0 - len(failed_indices) / len(expected)
    hard_failed_indices = [
        index
        for index, (absolute_error, expected_value) in enumerate(
            zip(absolute_errors, expected)
        )
        if absolute_error > max(MAX_ABSOLUTE_ERROR, 32 * float32_ulp(expected_value))
    ]

    if failed_indices:
        for index in failed_indices[:10]:
            abs_error = abs(float(actual[index]) - float(expected[index]))
            denominator = max(abs(float(expected[index])), ATOL)
            print(
                f"index={index:04d}, expected={expected[index]:.9g}, "
                f"actual={actual[index]:.9g}, abs_error={abs_error:.6g}, "
                f"relative_error={abs_error / denominator:.6g}"
            )
    print(f"max absolute error: {max(absolute_errors):.9g}")
    print(f"max relative error: {max(relative_errors):.9g}")
    print(f"matched ratio: {matched_ratio:.6f}, required: {REQUIRED_MATCHED_RATIO:.6f}")
    print(f"hard-limit violations: {len(hard_failed_indices)}")
    if matched_ratio < REQUIRED_MATCHED_RATIO or hard_failed_indices:
        print("test failed")
        return 1
    print("test pass!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
