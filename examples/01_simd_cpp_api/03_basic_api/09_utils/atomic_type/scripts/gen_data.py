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


DATA_SIZE = 256
CTRL_SIZE = 32


def gen_golden_data():
    """
    生成Golden数据：
    1. input.bin：输入数据（256个half），用于占位
    2. golden_ctrl.bin：寄存器值验证（32个int64）
    
    原子寄存器格式：
    - atomicType: bit[2:0] (mask=0x7)
    - atomicOp: bit[5:4] (mask=0x3)
    - config = (atomicOp << 4) | atomicType
    
    AtomicDtype对应值：
    - ATOMIC_NONE = 0
    - ATOMIC_F32 = 1
    - ATOMIC_F16 = 2
    - ATOMIC_S16 = 3
    - ATOMIC_S32 = 4
    - ATOMIC_S8 = 5
    - ATOMIC_BF16 = 6
    
    AtomicOp对应值：
    - ATOMIC_SUM = 0
    - ATOMIC_MAX = 1
    - ATOMIC_MIN = 2
    
    golden_ctrl数组顺序（每个测试填充4个元素）：
    [0-3]: SetAtomicType<half> -> type=2, op=0 -> config=2
    [4-7]: SetAtomicType<float> -> type=1, op=0 -> config=1
    [8-11]: SetAtomicType<int32_t> -> type=4, op=0 -> config=4
    [12-15]: DisableDmaAtomic -> type=0, op=0 -> config=0
    [16-19]: SetAtomicMax<half> -> type=2, op=1 -> config=18
    [20-23]: SetAtomicMax<float> -> type=1, op=1 -> config=17
    [24-27]: SetAtomicMin<half> -> type=2, op=2 -> config=34
    [28-31]: SetAtomicMin<float> -> type=1, op=2 -> config=33
    """
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    src_data = np.zeros(DATA_SIZE, dtype=np.float16)
    src_data.tofile("./input/input.bin")
    
    golden_ctrl = np.zeros(CTRL_SIZE, dtype=np.int64)
    golden_ctrl[0:4] = 2    # SetAtomicType<half>: type=2, op=0
    golden_ctrl[4:8] = 1    # SetAtomicType<float>: type=1, op=0
    golden_ctrl[8:12] = 4   # SetAtomicType<int32_t>: type=4, op=0
    golden_ctrl[12:16] = 0  # DisableDmaAtomic: type=0, op=0
    golden_ctrl[16:20] = 18 # SetAtomicMax<half>: type=2, op=1 -> (1<<4)|2=18
    golden_ctrl[20:24] = 17 # SetAtomicMax<float>: type=1, op=1 -> (1<<4)|1=17
    golden_ctrl[24:28] = 34 # SetAtomicMin<half>: type=2, op=2 -> (2<<4)|2=34
    golden_ctrl[28:32] = 33 # SetAtomicMin<float>: type=1, op=2 -> (2<<4)|1=33
    golden_ctrl.tofile("./output/golden_ctrl.bin")


if __name__ == "__main__":
    gen_golden_data()