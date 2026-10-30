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


import os
import sys
import numpy as np


RELATIVE_TOL = 1e-6
ABSOLUTE_TOL = 1e-9
ERROR_TOL = 1e-4
EXPECTED_ELEMENTS = 512 * 128


def verify_result(output, golden):
    expected_bytes = EXPECTED_ELEMENTS * np.dtype(np.float16).itemsize
    for path in (output, golden):
        if os.path.getsize(path) != expected_bytes:
            raise ValueError(f"Expected {expected_bytes} bytes: {path}")
    output = np.fromfile(output, dtype=np.float16).reshape(-1)
    golden = np.fromfile(golden, dtype=np.float16).reshape(-1)
    if not np.all(np.isfinite(output)) or not np.all(np.isfinite(golden)):
        raise ValueError("Output and golden data must contain only finite values")
    output_float = output.astype(np.float32)
    golden_float = golden.astype(np.float32)
    different_element_results = np.isclose(output_float, golden_float, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
    different_element_indexes = np.where(different_element_results == False)[0]
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
                abs(float(output_data) - float(golden_data)) / max(abs(float(golden_data)), ABSOLUTE_TOL),
            )
        )
        if index == 100:
            break
    print("golden_data : ", golden)
    print("output : ", output)
    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL))
    return error_ratio <= ERROR_TOL


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
