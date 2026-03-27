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

DTYPE_MAP = {
    "int8_t": np.int8,
    "int16_t": np.int16,
    "half": np.float16,
    "bfloat16_t": np.float16,
    "int32_t": np.int32,
    "float": np.float32
}

FLOAT_DTYPES = {"half", "bfloat16_t", "float"}
INT_DTYPES = {"int8_t", "int16_t", "int32_t"}

RELATIVE_TOL = 1e-6
ABSOLUTE_TOL = 1e-9
ERROR_TOL = 1e-4


def verify_result(output_path, golden_path, dtype_name):
    np_type = DTYPE_MAP.get(dtype_name)
    if np_type is None:
        print(f"Unsupported dtype: {dtype_name}")
        return False
    
    output = np.fromfile(output_path, dtype=np_type).reshape(-1)
    golden = np.fromfile(golden_path, dtype=np_type).reshape(-1)
    
    if dtype_name in FLOAT_DTYPES:
        different_element_results = np.isclose(output,
                                               golden,
                                               rtol=RELATIVE_TOL,
                                               atol=ABSOLUTE_TOL,
                                               equal_nan=True)
    else:
        different_element_results = (output == golden)
    
    different_element_indexes = np.where(different_element_results == False)[0]
    
    for index in range(min(10, len(golden))):
        real_index = index
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f" %
            (real_index, float(golden_data), float(output_data)))
    
    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL))
    return error_ratio <= ERROR_TOL


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Usage: verify_result.py <output> <golden> <dtype>")
        print("  dtype: int8_t, int16_t, half, bfloat16_t, int32_t, float")
        sys.exit(1)
    
    try:
        res = verify_result(sys.argv[1], sys.argv[2], sys.argv[3])
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)