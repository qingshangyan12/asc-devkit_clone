#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
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


def get_dtype(scenario_num: int) -> type:
    if 1 <= scenario_num <= 3:
        return np.float16
    elif 4 <= scenario_num <= 6:
        return np.float32
    else:
        raise ValueError(f"Invalid scenario number: {scenario_num}")


def get_element_size(dtype: type) -> int:
    return np.dtype(dtype).itemsize


def build_mask_bit(input_type, mask):
    # 说明： mask <= items_per_iter （例如 half 类型对应 128 个元素）， mask > 128 按照一次 repeat(128个数)计算
    #       repeat_time > 1时， 所有 repeat 中都是前 mask 参与计算
    # 向量寄存器宽度 = 256 Bytes
    vector_reg_size = 256
    # 每个迭代处理的元素个数
    items_per_iter = vector_reg_size // get_element_size(input_type)
    if mask > items_per_iter:
        mask = items_per_iter
    groups = mask // 4  # 完整组数：全部 4 位有效
    remainder = mask % 4  # 尾块剩余位数（0~3）

    result = 0
    # 16进制个数（例如 half类型输入，共 128 个元素，对应128 / 4 = 32个16进制）
    hex_count = items_per_iter // 4
    for i in range(hex_count, -1, -1):
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


def normal_highdim_scenario(input_type, dst, src, mask_bit, repeat_params):
    # 向量寄存器宽度 = 256 Bytes
    vector_reg_size = 256
    # DataBlock的大小为 32 Bytes
    data_block_size = 32
    # 总共 8 个 block
    blk_nums = vector_reg_size // data_block_size
    # half 类型（np.float16）占用 2 Bytes，每个 block（32 Bytes）有 16 个 half 类型的元素，依次类推
    data_blk_elements = data_block_size // get_element_size(input_type)

    # 每个迭代处理的元素个数
    items_per_iter = vector_reg_size // get_element_size(input_type)
    # 重复多少个迭代
    repeat_times = len(src) // items_per_iter

    for m in range(repeat_times):
        dst_rep_offset = m * repeat_params.dst_rep_stride
        src_rep_offset = m * repeat_params.src_rep_stride
        dst_blk_offset = 0
        src_blk_offset = 0

        for i in range(blk_nums):
            for j in range(data_blk_elements):
                idx = j + i * data_blk_elements
                if (1 << idx) & mask_bit:
                    src_idx = j + src_blk_offset * data_blk_elements + src_rep_offset * data_blk_elements
                    dst_idx = j + dst_blk_offset * data_blk_elements + dst_rep_offset * data_blk_elements

                    if src_idx < len(src) and dst_idx < len(dst):
                        dst[dst_idx] = np.exp(src[src_idx])

            dst_blk_offset += repeat_params.dst_blk_stride
            src_blk_offset += repeat_params.src_blk_stride


def gen_golden_data(scenario_num):
    total_length = 1024 # 输入数据长度
    input_type = get_dtype(scenario_num)
    output_type = input_type
    if input_type == np.float16 or input_type == np.float32:
        min_val, max_val = -10.0, 10.0
    else:
        min_val, max_val = -10, 10
    input_shape = [total_length]
    output_shape = [total_length]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    golden = np.zeros(total_length).astype(output_type)
    # 根据不同接口调用场景，构造不同真值
    if scenario_num == 1 or scenario_num == 4:
        half_length = total_length // 2
        for index in range(half_length):
            golden[index] = np.exp(input_x[index])
    elif scenario_num == 2 or scenario_num == 5:
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
 	    )
        mask = 10
        mask_bit = build_mask_bit(input_type, mask)
        normal_highdim_scenario(input_type, golden, input_x, mask_bit, repeat_params)
    else:
        repeat_params = UnaryRepeatParams(
            dst_blk_stride=2,
            src_blk_stride=2,
            dst_rep_stride=16,
            src_rep_stride=16
 	    )
        mask_high = 0x5555555555555555
        mask_low = 0x5555555555555555
        mask_bit = (mask_high << 64) | mask_low
        normal_highdim_scenario(input_type, golden, input_x, mask_bit, repeat_params)
    golden = golden.astype(output_type)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_src.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate golden data for operator.")
    parser.add_argument('-scenario_num', type=int, default=1, choices=list(range(1, 7)))
    args = parser.parse_args()

    gen_golden_data(args.scenario_num)