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
CTRL_SIZE = 16


def gen_golden_data():
    """
    生成Golden数据：
    1. input.bin：输入数据（256个half），用于占位
    2. golden_ctrl.bin：寄存器值验证（16个int64）
    
    CTRL寄存器bit位置：
    - HF32_MODE_BIT = 46 (CTRL[46])
    - HF32_TRANS_MODE_BIT = 47 (CTRL[47])
    
    HF32Mode枚举：
    - HF32Mode::ENABLE -> CTRL[46]=1
    - HF32Mode::DISABLE -> CTRL[46]=0
    
    HF32TransMode枚举：
    - HF32TransMode::NEAREST_ZERO -> CTRL[47]=1
    - HF32TransMode::NEAREST_EVEN -> CTRL[47]=0
    
    输出值定义：
    - 0: DISABLE + NEAREST_EVEN (CTRL[46]=0, CTRL[47]=0)
    - 1: ENABLE + NEAREST_EVEN (CTRL[46]=1, CTRL[47]=0)
    - 2: ENABLE + NEAREST_ZERO (CTRL[46]=1, CTRL[47]=1)
    - 3: DISABLE + NEAREST_ZERO (CTRL[46]=0, CTRL[47]=1)
    
    golden_ctrl数组顺序（每个测试填充4个元素）：
    [0-3]: DISABLE + NEAREST_EVEN -> 输出0
    [4-7]: ENABLE + NEAREST_EVEN -> 输出1
    [8-11]: ENABLE + NEAREST_ZERO -> 输出2
    [12-15]: DISABLE + NEAREST_ZERO -> 输出3
    """
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    src_data = np.zeros(DATA_SIZE, dtype=np.float16)
    src_data.tofile("./input/input.bin")
    
    golden_ctrl = np.zeros(CTRL_SIZE, dtype=np.int64)
    golden_ctrl[0:4] = 0    # DISABLE + NEAREST_EVEN
    golden_ctrl[4:8] = 1    # ENABLE + NEAREST_EVEN
    golden_ctrl[8:12] = 2   # ENABLE + NEAREST_ZERO
    golden_ctrl[12:16] = 3  # DISABLE + NEAREST_ZERO
    golden_ctrl.tofile("./output/golden_ctrl.bin")


if __name__ == "__main__":
    gen_golden_data()