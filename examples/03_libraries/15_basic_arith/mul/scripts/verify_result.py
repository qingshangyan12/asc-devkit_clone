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


import sys
import argparse
import numpy as np


# for float32
RELATIVE_TOL = 1e-6
ABSOLUTE_TOL = 1e-9
ERROR_TOL = 1e-4

TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}

def verify_result(output_type, output, golden, input_x, input_y):
    output_type = TYPE_MAP[output_type]
    output = np.fromfile(output, dtype=output_type).reshape(-1)
    golden = np.fromfile(golden, dtype=output_type).reshape(-1)
    input_x = np.fromfile(input_x, dtype=output_type).reshape(-1)
    input_y = np.fromfile(input_y, dtype=output_type).reshape(-1)
    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=RELATIVE_TOL,
                                           atol=ABSOLUTE_TOL,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        input_x_data = input_x[real_index]
        input_y_data = input_y[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, input_x: %-.9f, input_y: %-.9f" %
            (real_index, golden_data, output_data, input_x_data, input_y_data))
        if index == 100:
            break
    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL))
    return error_ratio <= ERROR_TOL


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-output_type', type=int, default=1, choices=[1, 2, 3, 4])
    parser.add_argument('-output', type=str)
    parser.add_argument('-golden', type=str)
    parser.add_argument('-input_x', type=str)
    parser.add_argument('-input_y', type=str)
    args = parser.parse_args()
    try:
        res = verify_result(args.output_type, args.output, args.golden, args.input_x, args.input_y)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
