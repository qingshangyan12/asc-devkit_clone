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


def gen_golden_data_simple(scenarioNum, size=None, repeatTime=None, 
                          dstBlkStride=None, dstRepStride=None):
    """生成测试数据和真值，支持外部参数配置"""
    # Brcb: 从src取8个数据，每个数据广播成一个32B块
    one_block_size = 32  # 32B = 一个block

    # 根据场景设置默认参数
    if scenarioNum == 1:
        # half 基础测试
        input_type = np.dtype("float16")
        default_src_size = 64
        default_dst_blk_stride = 1
        default_dst_rep_stride = 8
        default_repeat_time = 8
    elif scenarioNum == 2:
        # half 大步长测试
        input_type = np.dtype("float16")
        default_src_size = 64
        default_dst_blk_stride = 2
        default_dst_rep_stride = 16
        default_repeat_time = 8
    elif scenarioNum == 3:
        # half 大数据量测试
        input_type = np.dtype("float16")
        default_src_size = 1024
        default_dst_blk_stride = 1
        default_dst_rep_stride = 8
        default_repeat_time = 128
    elif scenarioNum == 4:
        # float 基础测试
        input_type = np.dtype("float32")
        default_src_size = 64
        default_dst_blk_stride = 1
        default_dst_rep_stride = 8
        default_repeat_time = 8
    elif scenarioNum == 5:
        # float 大数据量测试
        input_type = np.dtype("float32")
        default_src_size = 1024
        default_dst_blk_stride = 1
        default_dst_rep_stride = 8
        default_repeat_time = 128
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")

    # 应用外部配置（优先使用外部参数）
    src_size = size if size is not None else default_src_size
    repeat_time = repeatTime if repeatTime is not None else default_repeat_time
    dst_blk_stride = dstBlkStride if dstBlkStride is not None else default_dst_blk_stride
    dst_rep_stride = dstRepStride if dstRepStride is not None else default_dst_rep_stride

    min_val, max_val = get_range_by_dtype(input_type)
    output_type = input_type

    # 计算输出大小
    # Brcb: 一次repeat处理8个元素，每个元素广播到32B块
    elements_per_block = one_block_size // input_type.itemsize
    dst_size = repeat_time * dst_rep_stride * elements_per_block

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
    parser.add_argument('-size', type=int, default=None,
                        help='Source data size (overrides scenario default)')
    parser.add_argument('-repeatTime', type=int, default=None,
                        help='Repeat time (overrides scenario default)')
    parser.add_argument('-dstBlkStride', type=int, default=None,
                        help='Destination block stride (overrides scenario default)')
    parser.add_argument('-dstRepStride', type=int, default=None,
                        help='Destination repeat stride (overrides scenario default)')
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum, args.size, args.repeatTime, 
                          args.dstBlkStride, args.dstRepStride)
