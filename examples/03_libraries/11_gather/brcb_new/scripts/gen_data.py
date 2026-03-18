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


def gen_golden_data_simple(scenarioNum):
    """生成测试数据和真值"""
    # Brcb: 从src取8个数据，每个数据广播成一个32B块
    one_block_size = 32  # 32B = 一个block

    if scenarioNum == 1:
        # half 基础测试
        input_type = np.dtype("float16")
        src_size = 64
        dst_blk_stride = 1
        dst_rep_stride = 8
        repeat_time = 8
    elif scenarioNum == 2:
        # half 大步长测试
        input_type = np.dtype("float16")
        src_size = 64
        dst_blk_stride = 2
        dst_rep_stride = 16
        repeat_time = 8
    elif scenarioNum == 3:
        # half 大数据量测试
        input_type = np.dtype("float16")
        src_size = 1024
        dst_blk_stride = 1
        dst_rep_stride = 8
        repeat_time = 128
    elif scenarioNum == 4:
        # float 基础测试
        input_type = np.dtype("float32")
        src_size = 64
        dst_blk_stride = 1
        dst_rep_stride = 8
        repeat_time = 8
    elif scenarioNum == 5:
        # float 大数据量测试
        input_type = np.dtype("float32")
        src_size = 1024
        dst_blk_stride = 1
        dst_rep_stride = 8
        repeat_time = 128
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")

    min_val, max_val = get_range_by_dtype(input_type)
    output_type = input_type

    # 计算输出大小
    # Brcb: 一次repeat处理8个元素，每个元素广播到32B块
    elements_per_block = one_block_size // input_type.itemsize
    dst_size = repeat_time * 8 * dst_rep_stride * elements_per_block // dst_rep_stride

    # 生成输入数据
    input_x = np.random.uniform(min_val, max_val, [src_size]).astype(input_type)
    
    # 生成真值
    golden = np.zeros([dst_size]).astype(output_type)
    
    # Brcb计算真值：每个元素广播到一个block
    for rep in range(repeat_time):
        for blk in range(8):  # 每次处理8个元素
            src_idx = rep * 8 + blk
            if src_idx < src_size:
                # 计算目标位置
                dst_block_start = (rep * dst_rep_stride + blk * dst_blk_stride) * elements_per_block
                for i in range(elements_per_block):
                    if dst_block_start + i < dst_size:
                        golden[dst_block_start + i] = input_x[src_idx]

    print(f"Scenario {scenarioNum}:")
    print(f"  Input X shape: {input_x.shape}")
    print(f"  Output shape: {golden.shape}")

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 6),
                        help='Scenario number: 1-3=half tests, 4-5=float tests')
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum)
