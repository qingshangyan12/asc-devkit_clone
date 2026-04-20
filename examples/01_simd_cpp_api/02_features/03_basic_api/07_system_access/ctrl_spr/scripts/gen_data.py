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


NUM_BLOCKS = 2
DATA_SIZE = 256
CTRL_SIZE = 8


def gen_golden_data():
    """
    生成Golden数据：
    1. input.bin：输入数据（2个核，每个核256个float）
    2. dst_init.bin：dst初始值（用于初始化dstAtomicAddGm）
    3. golden_ctrl.bin：寄存器值验证
    4. golden_atomic_add.bin：原子累加功能验证
    """
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    src_data = np.arange(NUM_BLOCKS * DATA_SIZE, dtype=np.float32)
    src_data.tofile("./input/input.bin")
    
    dst_init = np.zeros(DATA_SIZE, dtype=np.float32)
    dst_init.tofile("./input/dst_init.bin")
    
    golden_ctrl = np.zeros(NUM_BLOCKS * CTRL_SIZE, dtype=np.int64)
    for block_idx in range(NUM_BLOCKS):
        offset = block_idx * CTRL_SIZE
        golden_ctrl[offset:offset+CTRL_SIZE//2] = 1
        golden_ctrl[offset+CTRL_SIZE//2:offset+CTRL_SIZE] = 0
    golden_ctrl.tofile("./output/golden_ctrl.bin")
    
    golden_atomic_add = np.zeros(DATA_SIZE, dtype=np.float32)
    for block_idx in range(NUM_BLOCKS):
        block_offset = block_idx * DATA_SIZE
        golden_atomic_add += src_data[block_offset:block_offset+DATA_SIZE]
    golden_atomic_add.tofile("./output/golden_atomic_add.bin")


if __name__ == "__main__":
    gen_golden_data()