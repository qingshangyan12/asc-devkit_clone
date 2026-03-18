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
    one_data_block_size = 32  # 一个datablock能处理32B数据

    # 根据场景设置参数
    if scenarioNum == 1:
        input_type = np.dtype("uint16")
        output_type = input_type
        min_val, max_val = get_range_by_dtype(input_type)
        repeat_time = 2
        block_length = 256
        dst_blk_stride = 1
        dst_rep_stride = 8
    elif scenarioNum == 2:
        input_type = np.dtype("uint16")
        output_type = input_type
        min_val, max_val = get_range_by_dtype(input_type)
        repeat_time = 2
        block_length = 512
        dst_blk_stride = 2
        dst_rep_stride = 16
    elif scenarioNum == 3:
        # 大数据量测试 uint16_t
        input_type = np.dtype("uint16")
        output_type = input_type
        min_val, max_val = get_range_by_dtype(input_type)
        repeat_time = 128
        block_length = 16 * 1024
        dst_blk_stride = 1
        dst_rep_stride = 8
    elif scenarioNum == 4:
        # 大数据量测试 uint32_t
        input_type = np.dtype("uint32")
        output_type = input_type
        min_val, max_val = get_range_by_dtype(input_type)
        repeat_time = 128
        block_length = 16 * 1024
        dst_blk_stride = 1
        dst_rep_stride = 8
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")

    offset_size = block_length // dst_blk_stride * input_type.itemsize // one_data_block_size
    input_y = np.arange(0, offset_size * one_data_block_size, one_data_block_size, np.uint32)
    input_shape = [block_length // dst_blk_stride]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    output_shape = [block_length]
    golden = np.zeros(output_shape).astype(output_type)

    # 计算真值
    for i in range(repeat_time):
        for j in range(8):
            src_blk_start_index = input_y[i * 8 + j] // input_type.itemsize
            src_blk_end_index = src_blk_start_index + one_data_block_size // input_type.itemsize
            dst_blk_start_index = (one_data_block_size * i * dst_rep_stride + 
                                   one_data_block_size * j * dst_blk_stride) // input_type.itemsize
            dst_blk_end_index = dst_blk_start_index + one_data_block_size // input_type.itemsize
            golden[dst_blk_start_index : dst_blk_end_index] = input_x[src_blk_start_index : src_blk_end_index]

    print(f"Scenario {scenarioNum}:")
    print(f"  Input X shape: {input_x.shape}")
    print(f"  Input Y (offset) shape: {input_y.shape}")
    print(f"  Output shape: {golden.shape}")

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5),
                        help='Scenario number: 1-2=uint16 modes, 3=Large data uint16_t, 4=Large data uint32_t')
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum)
