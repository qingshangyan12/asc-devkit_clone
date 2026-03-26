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


import os
import numpy as np
import argparse


def gen_golden_data_half(size):
    """场景1: half类型，从0开始的等差序列"""
    dtype = np.float16
    first_value = np.float16(0)
    
    # 生成等差序列 [first_value, first_value+1, first_value+2, ...]
    golden = np.arange(first_value, first_value + size, dtype=dtype)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")
    print(f"Generated create_vec_index data for half: size={size}, first_value={first_value}")


def gen_golden_data_float(size):
    """场景2: float类型，从0开始的等差序列"""
    dtype = np.float32
    first_value = np.float32(0)
    
    # 生成等差序列 [first_value, first_value+1, first_value+2, ...]
    golden = np.arange(first_value, first_value + size, dtype=dtype)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")
    print(f"Generated create_vec_index data for float: size={size}, first_value={first_value}")


def gen_golden_data_int32(size):
    """场景3: int32_t类型，从10开始的等差序列"""
    dtype = np.int32
    first_value = np.int32(10)
    
    # 生成等差序列 [first_value, first_value+1, first_value+2, ...]
    golden = np.arange(first_value, first_value + size, dtype=dtype)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")
    print(f"Generated create_vec_index data for int32: size={size}, first_value={first_value}")


def gen_golden_data_int16(size):
    """场景4: int16_t类型，从5开始的等差序列"""
    dtype = np.int16
    first_value = np.int16(5)
    
    # 生成等差序列 [first_value, first_value+1, first_value+2, ...]
    golden = np.arange(first_value, first_value + size, dtype=dtype)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")
    print(f"Generated create_vec_index data for int16: size={size}, first_value={first_value}")


def gen_golden_data(scenarioNum, size):
    """根据场景生成测试数据"""
    if scenarioNum == 1:
        gen_golden_data_half(size)
    elif scenarioNum == 2:
        gen_golden_data_float(size)
    elif scenarioNum == 3:
        gen_golden_data_int32(size)
    elif scenarioNum == 4:
        gen_golden_data_int16(size)
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5),
                        help='Scenario number: 1=half, 2=float, 3=int32, 4=int16')
    parser.add_argument('-size', type=int, default=256, help='Data size')
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.size)
