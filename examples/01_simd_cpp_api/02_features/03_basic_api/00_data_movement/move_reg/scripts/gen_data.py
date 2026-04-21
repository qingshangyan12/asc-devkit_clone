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

def gen_golden_data_simple():
    total_length = 256
    data_type = np.float32
    # 生成源数据x: 随机浮点数
    x = np.random.uniform(1, 10, [1, total_length]).astype(data_type)
    # 生成备选数据y: 随机浮点数
    y = np.random.uniform(1, 10, [1, total_length]).astype(data_type)
    # 生成条件数据cond: 正负数交替，用于MaskGenWithRegTensor生成mask
    cond = np.zeros([1, total_length], dtype=data_type)
    for i in range(total_length):
        if (i // 4) % 2 == 0:
            cond[0, i] = -1.0  # 负数
        else:
            cond[0, i] = 1.0   # 正数

    # 计算golden:
    # MaskGenWithRegTensor<float, 4> 的实际行为：
    # 从RegTensor中偏移 offset*(VL/32) = 4*8 = 32 个元素处开始，
    # 将8个字节的原始数据直接搬入MaskReg（64 bits）。
    # oneRepeatSize = VL/sizeof(float) = 256/4 = 64 个float元素
    # repeatTimes = ceil(256/64) = 4
    # 每次repeat加载64个float到condReg，然后MaskGenWithRegTensor提取64-bit mask
    #
    # 对于第r次repeat:
    #   condReg中是cond[r*64 : r*64+64]
    #   从element 32开始取8字节作为mask
    #   即 cond_raw_bytes[(r*64 + 32)*4 : (r*64 + 32)*4 + 8] 的64个bit
    #   mask bit i (i=0..63) = raw_bytes的第i个bit (LSB first)
    #
    # Select: mask=1选x，mask=0选y
    one_repeat_size = 64  # VL/sizeof(float) for dav-3510
    repeat_times = (total_length + one_repeat_size - 1) // one_repeat_size

    golden = np.zeros([1, total_length], dtype=data_type)
    cond_flat = cond.flatten()
    cond_bytes = cond_flat.view(np.uint8)

    for r in range(repeat_times):
        # 加载到condReg的数据是 cond[r*64 : r*64+64]
        # MaskGenWithRegTensor从element 32开始取8字节
        byte_start = (r * one_repeat_size + 32) * 4
        raw_bytes = cond_bytes[byte_start:byte_start + 8]
        # 将8字节解释为64-bit LE整数
        mask_val = int.from_bytes(raw_bytes.tolist(), byteorder='little')
        # 提取64个mask bit (LSB first)
        for i in range(one_repeat_size):
            elem_idx = r * one_repeat_size + i
            if elem_idx >= total_length:
                break
            mask_bit = (mask_val >> i) & 1
            if mask_bit == 1:
                golden[0, elem_idx] = x[0, elem_idx]  # mask=1选x
            else:
                golden[0, elem_idx] = y[0, elem_idx]  # mask=0选y

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile('./input/input_x.bin')
    y.tofile('./input/input_y.bin')
    cond.tofile('./input/input_cond.bin')
    golden.tofile('./output/golden.bin')

if __name__ == "__main__":
    gen_golden_data_simple()
