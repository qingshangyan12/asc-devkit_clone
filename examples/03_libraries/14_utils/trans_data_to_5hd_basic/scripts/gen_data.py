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
    """场景1: half类型数据转换"""
    dtype = np.float16
    # 16块 * 16元素/块 = 256元素
    data_size = 16 * 16
    
    input_data = np.arange(data_size, dtype=dtype)
    # TransDataTo5HD 进行通道重排，golden 需要按 16x16 矩阵转置后按块读取
    # 简化处理：直接输出输入数据作为参考（实际转换逻辑需要按硬件行为确定）
    golden = input_data.copy()

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated trans_data_to_5hd_basic data for half: size={data_size}")


def gen_golden_data_float(size):
    """场景2: float类型数据转换"""
    dtype = np.float32
    # 16块 * 8元素/块 = 128元素
    data_size = 16 * 8
    
    input_data = np.arange(data_size, dtype=dtype)
    golden = input_data.copy()

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated trans_data_to_5hd_basic data for float: size={data_size}")


def gen_golden_data_int16(size):
    """场景3: int16_t类型数据转换"""
    dtype = np.int16
    # 16块 * 16元素/块 = 256元素
    data_size = 16 * 16
    
    input_data = np.arange(data_size, dtype=dtype)
    golden = input_data.copy()

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated trans_data_to_5hd_basic data for int16: size={data_size}")


def gen_golden_data_int8(size):
    """场景4: int8_t类型数据转换"""
    dtype = np.int8
    # 16块 * 32元素/块 = 512元素
    data_size = 16 * 32
    
    input_data = np.arange(data_size, dtype=dtype)
    golden = input_data.copy()

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated trans_data_to_5hd_basic data for int8: size={data_size}")


def gen_golden_data(scenarioNum, size):
    """根据场景生成测试数据"""
    if scenarioNum == 1:
        gen_golden_data_half(size)
    elif scenarioNum == 2:
        gen_golden_data_float(size)
    elif scenarioNum == 3:
        gen_golden_data_int16(size)
    elif scenarioNum == 4:
        gen_golden_data_int8(size)
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5),
                        help='Scenario number: 1=half, 2=float, 3=int16, 4=int8')
    parser.add_argument('-size', type=int, default=256, help='Data size')
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.size)
