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
import argparse
import numpy as np


RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-4
ERROR_TOL = 1e-4


def verify_result_2d(output, golden):
    """验证场景1: 2D矩阵转置 (16x16)"""
    output_type = np.float16
    shape = (16, 16)
    
    output_data = np.fromfile(output, dtype=output_type).reshape(shape)
    golden_data = np.fromfile(golden, dtype=output_type).reshape(shape)
    
    return compare_data(output_data, golden_data)


def verify_result_nchw2nhwc(output, golden):
    """验证场景2: NCHW -> NHWC"""
    output_type = np.float16
    # NCHW -> NHWC: output shape [N, H, W, C] = [3, 2, 8, 3]
    shape = (3, 2, 8, 3)
    
    output_data = np.fromfile(output, dtype=output_type).reshape(shape)
    golden_data = np.fromfile(golden, dtype=output_type).reshape(shape)
    
    return compare_data(output_data, golden_data)


def verify_result_nhwc2nchw(output, golden):
    """验证场景3: NHWC -> NCHW"""
    output_type = np.float16
    # NHWC -> NCHW: output shape [N, C, H, W] = [3, 3, 2, 8]
    shape = (3, 3, 2, 8)
    
    output_data = np.fromfile(output, dtype=output_type).reshape(shape)
    golden_data = np.fromfile(golden, dtype=output_type).reshape(shape)
    
    return compare_data(output_data, golden_data)


def compare_data(output, golden):
    """比较输出和真值"""
    different_element_results = np.isclose(output, golden,
                                           rtol=RELATIVE_TOL,
                                           atol=ABSOLUTE_TOL,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    
    for index in range(min(len(different_element_indexes), 100)):
        real_index = different_element_indexes[index]
        golden_data = golden.flatten()[real_index]
        output_data = output.flatten()[real_index]
        print("data index: %06d, expected: %-.6f, actual: %-.6f, diff: %-.6f" %
              (real_index, golden_data, output_data, abs(output_data - golden_data)))
    
    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, ERROR_TOL))
    return error_ratio <= ERROR_TOL


def verify_result(scenarioNum, output, golden):
    """根据场景验证结果"""
    if scenarioNum == 1:
        return verify_result_2d(output, golden)
    elif scenarioNum == 2:
        return verify_result_nchw2nhwc(output, golden)
    elif scenarioNum == 3:
        return verify_result_nhwc2nchw(output, golden)
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 4),
                        help='Scenario number: 1=2D, 2=NCHW2NHWC, 3=NHWC2NCHW')
    parser.add_argument('output', type=str, help='Output file path')
    parser.add_argument('golden', type=str, help='Golden file path')
    args = parser.parse_args()
    
    try:
        res = verify_result(args.scenarioNum, args.output, args.golden)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
