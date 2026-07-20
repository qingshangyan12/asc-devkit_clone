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


RELATIVE_TOLERANCE = 1e-6
ABSOLUTE_TOLERANCE = 1e-9
ERROR_RATIO_TOLERANCE = 1e-4


def verify_result(output_path, golden_path):
    output_size = os.path.getsize(output_path)
    golden_size = os.path.getsize(golden_path)
    if output_size != golden_size:
        print(
            "output and golden file sizes differ: %d != %d" % (output_size, golden_size)
        )
        return False
    if golden_size == 0 or golden_size % np.dtype(np.float32).itemsize != 0:
        print("output and golden files must contain complete float32 data")
        return False

    output = np.fromfile(output_path, dtype=np.float32)
    golden = np.fromfile(golden_path, dtype=np.float32)
    close_results = np.isclose(
        output,
        golden,
        rtol=RELATIVE_TOLERANCE,
        atol=ABSOLUTE_TOLERANCE,
        equal_nan=True,
    )
    different_indices = np.where(~close_results)[0]
    for index in different_indices[:101]:
        golden_value = golden[index]
        output_value = output[index]
        relative_difference = abs(output_value - golden_value) / max(
            abs(golden_value), ABSOLUTE_TOLERANCE
        )
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f"
            % (index, golden_value, output_value, relative_difference)
        )

    error_ratio = float(different_indices.size) / golden.size
    return error_ratio <= ERROR_RATIO_TOLERANCE


if __name__ == "__main__":
    try:
        if not verify_result(sys.argv[1], sys.argv[2]):
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as error:
        print(error)
        sys.exit(1)
