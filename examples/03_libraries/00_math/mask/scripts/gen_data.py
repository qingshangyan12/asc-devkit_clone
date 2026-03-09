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
import sys
import numpy as np
from dataclasses import dataclass


class UnaryRepeatParams:
    """
    用于配置高维切分repeat的参数
    Attributes：
        dst_blk_stride (int): 目标(dst)每个 block 的 stride     （单位：block）
        src_blk_stride (int): 目标(src)每个 block 的 stride     （单位：block）
        dst_rep_stride (int): 目标(dst)每个 repeat 的 stride     
        src_rep_stride (int): 目标(src)每个 repeat 的 stride     
    """
    def __init__(
        self,
        dst_blk_stride: int = 1,
        src_blk_stride: int = 1,
        dst_rep_stride: int = 8,
        src_rep_stride: int = 8,
    ):
        self.dst_blk_stride = dst_blk_stride
        self.src_blk_stride = src_blk_stride
        self.dst_rep_stride = dst_rep_stride
        self.src_rep_stride = src_rep_stride


def build_blt_mask(mask):
    # 约束： mask <= 128 （half 类型对应 128 个元素）， mask > 128 按照一次 repeat(128个数)计算
    #        repeat_time > 1时， 所有 repeat 中都是前 mask 参与计算
    if mask > 128:
        mask = 128
    groups = mask // 4
    remainder = mask % 4

    result = 0
    # 32 表示 32 个 16 进制位，共 128 个元素
    for i in range(32, -1, -1):
        if i < groups:
            result = (result << 4) | 0xF
        elif i == groups:
            # 尾块：只设置前 remainder 位
            if remainder > 0:
                # remainder = 1 -> 设置 0x1
                # remainder = 2 -> 设置 0x3
                # remainder = 3 -> 设置 0x7
                result = (result << 4) | ((1 << remainder) - 1)
            else:
                result = (result << 4) | 0x0
        else:
            result = (result << 4) | 0x0
    return result


def normal_highdim_scenario(dst, input_x, tmp, value, mask_bit, blk_nums, data_blk_elements, repeat_params, repeat_times):
    """
    """
    for m in range(repeat_times):
        dst_rep_offset = m * repeat_params.dst_rep_stride
        src_rep_offset = m * repeat_params.src_rep_stride
        dst_blk_offset = 0
        src_blk_offset = 0

        for i in range(blk_nums):
            for j in range(data_blk_elements):
                idx = j + i * data_blk_elements
                if (1 << idx) & mask_bit:
                    src_idx = j + src_blk_offset * 16 + src_rep_offset * 16
                    dst_idx = j + dst_blk_offset * 16 + dst_rep_offset * 16

                    if src_idx < len(input_x) and dst_idx < len(dst):
                        tmp[j] =  input_x[src_idx]
                        dst[dst_idx] = tmp[j] + value

            dst_blk_offset += repeat_params.dst_blk_stride
            src_blk_offset += repeat_params.src_blk_stride


def gen_golden_data_simple(scenario):
    dtype = np.float16
    src_dtype = dtype
    dst_dtype = dtype
    input_x = np.arrange(512, dtype=src_dtype)
    add_value = 1.0
    dst = np.zeros(512, dtype=dst_dtype)

    ## 向量寄存器宽度 = 256 Bytes
    vector_reg_size = 256
    ## DataBlock的大小位 32 Bytes
    data_block_size = 32
    # 总共 8 个 block
    blk_nums = vector_reg_size // data_block_size

    if dtype == np.float16:
        # half 类型（np.float16）占用 2 Bytes，每个 block（32 Bytes）有 16 个 half 类型的元素 
        data_blk_elements = data_block_size // 2
    else:
        # float 类型（np.float32）占用 4 Bytes，每个 block（32 Bytes）有 8 个 float 类型的元素 
        data_blk_elements = data_block_size // 4
    tmp = np.zeros(512, dtype=src_dtype)

    if (scenario == 0):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=1,
            src_blk_stride=1,
            dst_rep_stride=8,
            src_rep_stride=8
        )
    
        # 设置 mask 值
        mask_high = 0xFFFFFFFFFFFFFFFF
        mask_low = 0xFFFFFFFFFFFFFFFF
        mask_bit = (mask_high << 64) | mask_low
        repeat_times = 1
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, blk_nums, data_blk_elements, repeat_params, repeat_times)
    
    elif (scenario == 1):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        mask = 130
        repeat_times = 1
        mask_bit = build_blt_mask(mask)
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, blk_nums, data_blk_elements, repeat_params, repeat_times)

    elif (scenario == 2):
        src_len = 130

        for j in range(src_len):
            tmp[j] = input_x[j]
            dst[j] = tmp[j] + add_value
    elif (scenario == 3):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        mask_high = 0xFFFFFFFFFFFFFFFF
        mask_low = 0xFFFFFFFFFFFFFFFF
        mask_bit = (mask_high << 64) | mask_low
        repeat_times = 1
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, blk_nums, data_blk_elements, repeat_params, repeat_times)
    elif (scenario == 4):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        mask = 130
        repeat_times = 1
        mask_bit = build_blt_mask(mask)
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, blk_nums, data_blk_elements, repeat_params, repeat_times)
    elif (scenario == 5):
        src_len = 128
        add_value = 1.0
        for j in range(src_len):
            tmp[j] = input_x[j]
            dst[j] = tmp[j] + add_value
    else:
        print("scenario 值为0-5，请输入正确值")
        sys.exit(1)
    golden = dst.astype(np.float16)

    os.makedirs("input", exist_ok=True)
    input_x.tofile("./input/input_X.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 gen_data.py <scenario>")
        sys.exit(1)
    try:
        scenario = int(sys.argv[1])
        gen_golden_data_simple(scenario)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
