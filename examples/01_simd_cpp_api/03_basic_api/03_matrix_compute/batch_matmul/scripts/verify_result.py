#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import sys
import numpy as np

# FLOAT32 mixed-tolerance thresholds from the ecosystem operator precision standard.
relative_tol = 2**-10
absolute_tol = 2**-16
required_matched_ratio = 0.99
fixed_absolute_error_limit = 1e-2


def verify_result(output, golden):
    output = np.fromfile(output, dtype=np.float32).reshape(-1)
    golden = np.fromfile(golden, dtype=np.float32).reshape(-1)
    if output.size != golden.size or output.size == 0:
        raise ValueError(
            "output and golden must contain the same non-zero number of float32 values"
        )

    output_fp64 = output.astype(np.float64)
    golden_fp64 = golden.astype(np.float64)
    absolute_error = np.abs(output_fp64 - golden_fp64)
    tolerance = absolute_tol + relative_tol * np.abs(golden_fp64)
    finite = np.isfinite(output_fp64) & np.isfinite(golden_fp64)
    matched = finite & (absolute_error <= tolerance)
    different_element_indexes = np.flatnonzero(~matched)
    relative_error = absolute_error / np.maximum(np.abs(golden_fp64), absolute_tol)

    ulp = np.abs(np.spacing(np.abs(golden).astype(np.float32))).astype(np.float64)
    absolute_error_limit = np.maximum(fixed_absolute_error_limit, 32.0 * ulp)
    absolute_error_limit_passed = bool(
        np.all(finite & (absolute_error <= absolute_error_limit))
    )
    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f"
            % (
                real_index,
                golden_data,
                output_data,
                absolute_error[real_index] / max(abs(float(golden_data)), absolute_tol),
            )
        )
        if index == 100:
            break
    matched_ratio = float(np.count_nonzero(matched)) / golden.size
    print("max absolute error: %.9e" % np.max(absolute_error))
    print("max relative error: %.9e" % np.max(relative_error))
    print("matched ratio: %.9e" % matched_ratio)
    print("absolute error hard limit passed: %s" % absolute_error_limit_passed)
    return matched_ratio >= required_matched_ratio and absolute_error_limit_passed


if __name__ == "__main__":
    try:
        res = verify_result(sys.argv[1], sys.argv[2])
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
