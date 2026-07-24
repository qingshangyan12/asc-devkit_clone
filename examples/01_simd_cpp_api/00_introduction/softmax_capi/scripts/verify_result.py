#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License"). Please refer to the License for details.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import sys

import numpy as np


RELATIVE_TOL = 1e-4
ABSOLUTE_TOL = 1e-5
ERROR_TOL = 1e-4


def verify_result(output, golden):
    output = np.fromfile(output, dtype=np.float32).reshape(-1)
    golden = np.fromfile(golden, dtype=np.float32).reshape(-1)
    if output.size != golden.size:
        raise ValueError("output size %d is not equal to golden size %d" % (output.size, golden.size))

    close_result = np.isclose(output, golden, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL, equal_nan=True)
    diff_indexes = np.where(close_result == False)[0]
    for index in range(min(len(diff_indexes), 100)):
        real_index = diff_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        denominator = max(abs(golden_data), ABSOLUTE_TOL)
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f"
            % (real_index, golden_data, output_data, abs(output_data - golden_data) / denominator)
        )
    error_ratio = float(diff_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL))
    return error_ratio <= ERROR_TOL


if __name__ == "__main__":
    try:
        res = verify_result(sys.argv[1], sys.argv[2])
        if not res:
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
