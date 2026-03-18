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
import argparse
import numpy as np


def get_range_by_dtype(input_type):
    """根据数据类型获取取值范围"""
    try:
        if input_type == np.float16 or input_type == np.float32 or input_type == np.float64:
            return np.finfo(input_type).min, np.finfo(input_type).max
        else:
            return np.iinfo(input_type).min, np.iinfo(input_type).max
    except ValueError:
        print(f"Unsupported data type:{input_type}")


def gen_golden_data_simple(scenarioNum, calcSize=None):
    """生成测试数据和真值，支持外部参数配置"""
    # 根据场景设置默认参数
    if scenarioNum >= 1 and scenarioNum <= 3:
        # half类型的Count/Mask模式测试
        default_count = 128
        input_type = np.dtype("float16")
        min_val, max_val = get_range_by_dtype(input_type)
        output_type = input_type
        src_base_addr = 32 * input_type.itemsize
    elif scenarioNum == 4:
        # 大数据量测试 half 类型
        default_count = 1024 * 16
        input_type = np.dtype("float16")
        min_val, max_val = get_range_by_dtype(input_type)
        output_type = input_type
        src_base_addr = 0 * input_type.itemsize
    elif scenarioNum == 5:
        # 大数据量测试 float 类型
        default_count = 1024 * 16
        input_type = np.dtype("float32")
        min_val, max_val = get_range_by_dtype(input_type)
        output_type = input_type
        src_base_addr = 0 * input_type.itemsize
    
    # 应用外部配置（优先使用外部参数）
    count = calcSize if calcSize is not None else default_count
    
    # 生成数据
    input_y = np.arange(0, count * input_type.itemsize, input_type.itemsize, np.uint32)
    input_shape = [(count * input_type.itemsize + src_base_addr) // input_type.itemsize]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    output_shape = [count]
    golden = np.zeros(output_shape).astype(output_type)
    for i in range(count):
        golden[i] = input_x[(src_base_addr + input_y[i]) // input_type.itemsize]

    print(f"Scenario {scenarioNum}:")
    print(f"  Input X shape: {input_x.shape}")
    print(f"  Input Y shape: {input_y.shape}")
    print(f"  Output shape: {golden.shape}")

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 6),
                        help='Scenario number: 1-3=half Count/Mask modes, 4=Large data half, 5=Large data float')
    parser.add_argument('-calcSize', type=int, default=None,
                        help='Calculation size (overrides scenario default)')
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum, args.calcSize)
