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
import argparse


# 向量寄存器宽度（Bytes）
VECTOR_REG_SIZE = 256
# DataBlock大小（Bytes）
DATA_BLOCK_SIZE = 32
# 总共 block 数量
BLK_NUMS = VECTOR_REG_SIZE // DATA_BLOCK_SIZE
# 每个 block 中的元素个数（根据 dtype 动态确定）
DATA_BLOCK_ELEMENTS = {
    # half 类型（np.float16）占用 2 Bytes，每个 block（32 Bytes）有 16 个 half 类型的元素
    np.float16: DATA_BLOCK_SIZE // 2,
    # float 类型（np.float32）占用 4 Bytes，每个 block（32 Bytes）有 8 个 float 类型的元素
    np.float32: DATA_BLOCK_SIZE // 4,
}


class UnaryRepeatParams:
    """
    用于配置高维切分repeat的参数
    Attributes：
        dst_blk_stride (int): 目标(dst)每个 block 的 stride     （单位：block）
        src_blk_stride (int): 源(src)每个 block 的 stride     （单位：block）
        dst_rep_stride (int): 目标(dst)每个 repeat 的 stride     
        src_rep_stride (int): 源(src)每个 repeat 的 stride     
    """
    def __init__(
        self,
        dst_blk_stride: int = 1,
        src_blk_stride: int = 1,
        dst_rep_stride: int = 8,
        src_rep_stride: int = 8
    ):
        self.dst_blk_stride = dst_blk_stride
        self.src_blk_stride = src_blk_stride
        self.dst_rep_stride = dst_rep_stride
        self.src_rep_stride = src_rep_stride


def build_bit_mask_half(mask):
    # 约束： mask <= 128 （half 类型对应 128 个元素）， mask > 128 按照一次 repeat(128个数)计算
    #        repeat_time > 1时， 所有 repeat 中都是前 mask 参与计算
    if mask > 128:
        mask = 128
    groups = mask // 4  # 完整组数：全部 4 位有效
    remainder = mask % 4  # 尾块剩余位数（0~3）

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


def normal_highdim_scenario(dst, input_x, tmp, value, mask_bit, blk_nums, data_blk_elements, repeat_params, repeat_times, start_idx = 0):
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
                    src_idx = j + src_blk_offset * 16 + src_rep_offset * 16 + start_idx
                    dst_idx = j + dst_blk_offset * 16 + dst_rep_offset * 16 + start_idx

                    if src_idx < len(input_x) and dst_idx < len(dst):
                        tmp[j] =  input_x[src_idx]
                        dst[dst_idx] = tmp[j] + value

            dst_blk_offset += repeat_params.dst_blk_stride
            src_blk_offset += repeat_params.src_blk_stride


def gen_golden_data_simple(scenario):
    """
    scenario:
        0 代表接口传参 + Normal模式 + 高维切分 + 逐 bit 计算  
        1 代表接口传参 + Normal模式 + 高维切分 + 连续计算  
        2 代表接口传参 + Counter模式 + 前 n 个元素计算  
        3 代表接口传参 + Counter模式 + 高维切分 + 连续计算  
        4 代表外部API设置mask + Normal模式 + 高维切分 + 逐 bit 计算  
        5 代表外部API设置mask + Normal模式 + 高维切分 + 连续计算  
        6 代表外部API设置mask + Counter模式 + 前 n 个元素计算  
        7 代表外部API设置mask + Counter模式 + 高维切分 + 连续计算  
    """
    dtype = np.float16
    src_dtype = dtype
    dst_dtype = dtype
    data_shape = 512
    input_x = np.arange(data_shape, dtype=src_dtype)
    add_value = 1.0
    dst = np.zeros(data_shape, dtype=dst_dtype)
    tmp = np.zeros(data_shape, dtype=src_dtype)

    if (scenario == 0):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        # 设置 mask 值
        mask_high = 0x69A569A569A569A5  # mask[1]
        mask_low = 0x69A569A569A569A5  # mask[0]
        mask_bit = (mask_high << 64) | mask_low
        repeat_times = 1
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times)
    
    elif (scenario == 1):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        mask = 130
        repeat_times = 1
        mask_bit = build_bit_mask_half(mask)
        # 说明：mask <= 128 （half 类型对应 128 个元素），mask > 128 按照一次 repeat 计算
        #      repeatTime > 1 时，所有 repeat 中都是前 mask 参与计算
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times)

    elif (scenario == 2):
        src_len = 128

        for j in range(src_len):
            tmp[j] = input_x[j]
            dst[j] = tmp[j] + add_value
    elif (scenario == 3):
        src_len = 130
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        repeat_times = src_len // (BLK_NUMS * DATA_BLOCK_ELEMENTS[dtype])
        mask_bit = build_bit_mask_half(BLK_NUMS * DATA_BLOCK_ELEMENTS[dtype])
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times)
        # 尾块处理
        tail_block_nums = src_len % (BLK_NUMS * DATA_BLOCK_ELEMENTS[dtype])
        tail_mask_bit = build_bit_mask_half(tail_block_nums)
        start_idx = repeat_params.src_rep_stride * DATA_BLOCK_ELEMENTS[dtype]
        normal_highdim_scenario(dst, input_x, tmp, add_value, tail_mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times, start_idx)
    elif (scenario == 4):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        mask_high = 0x69A569A569A569A5
        mask_low = 0x69A569A569A569A5
        mask_bit = (mask_high << 64) | mask_low
        repeat_times = 1
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times)
    elif (scenario == 5):
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        mask = 130
        repeat_times = 1
        mask_bit = build_bit_mask_half(mask)
        # 说明：mask <= 128 （half 类型对应 128 个元素），mask > 128 按照一次 repeat 计算
        #      repeatTime > 1 时，所有 repeat 中都是前 mask 参与计算
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times)
    elif (scenario == 6):
        src_len = 128
        for j in range(src_len):
            tmp[j] = input_x[j]
            dst[j] = tmp[j] + add_value
    elif (scenario == 7):
        src_len = 130
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
        )
        repeat_times = src_len // (BLK_NUMS * DATA_BLOCK_ELEMENTS[dtype])
        mask_bit = build_bit_mask_half(BLK_NUMS * DATA_BLOCK_ELEMENTS[dtype])
        normal_highdim_scenario(dst, input_x, tmp, add_value, mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times)
        # 尾块处理
        tail_block_nums = src_len % (BLK_NUMS * DATA_BLOCK_ELEMENTS[dtype])
        tail_mask_bit = build_bit_mask_half(tail_block_nums)
        start_idx = repeat_params.src_rep_stride * DATA_BLOCK_ELEMENTS[dtype]
        normal_highdim_scenario(dst, input_x, tmp, add_value, tail_mask_bit, BLK_NUMS, DATA_BLOCK_ELEMENTS[dtype], repeat_params, repeat_times, start_idx)
    else:
        print("scenario 值为0-7，请输入正确值")
        sys.exit(1)
    golden = dst.astype(np.float16)

    os.makedirs("input", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(0, 8))
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum)