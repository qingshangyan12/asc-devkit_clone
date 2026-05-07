#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You can not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import sys
import argparse
import numpy as np


RELATIVE_TOL = 1e-5
ABSOLUTE_TOL = 1e-8
ERROR_TOL = 1e-3

RELATIVE_TOL_CUBE = 1e-5
ABSOLUTE_TOL_CUBE = 1e-3
ERROR_TOL_CUBE = 1e-3


def verify_result_vector(output_file, golden_file):
    output = np.fromfile(output_file, dtype=np.float32).reshape(-1)
    golden = np.fromfile(golden_file, dtype=np.float32).reshape(-1)
    
    assert output.size >= golden.size, f"output size {output.size} < expected {golden.size}"
    
    output = output[:golden.size]
    
    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=RELATIVE_TOL,
                                           atol=ABSOLUTE_TOL,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    
    for index in range(min(len(different_element_indexes), 100)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print("data index: %06d, expected: %-.9f, actual: %-.9f" %
              (real_index, golden_data, output_data))
    
    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL))
    return error_ratio <= ERROR_TOL


def verify_result_cube(output_file, golden_file):
    output = np.fromfile(output_file, dtype=np.float32).reshape(-1)
    golden = np.fromfile(golden_file, dtype=np.float32).reshape(-1)
    
    assert output.size >= golden.size, f"output size {output.size} < expected {golden.size}"
    
    output = output[:golden.size]
    
    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=RELATIVE_TOL_CUBE,
                                           atol=ABSOLUTE_TOL_CUBE,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    
    for index in range(min(len(different_element_indexes), 100)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print("data index: %06d, expected: %-.9f, actual: %-.9f" %
              (real_index, golden_data, output_data))
    
    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL_CUBE))
    return error_ratio <= ERROR_TOL_CUBE


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('output_file', help='Output file path')
    parser.add_argument('golden_file', help='Golden file path')
    parser.add_argument('-scenarioNum', type=int, default=1, choices=[1, 2], help='Scenario number')
    args = parser.parse_args()

    try:
        if args.scenarioNum == 1:
            res = verify_result_vector(args.output_file, args.golden_file)
        else:
            res = verify_result_cube(args.output_file, args.golden_file)
        
        if not res:
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
