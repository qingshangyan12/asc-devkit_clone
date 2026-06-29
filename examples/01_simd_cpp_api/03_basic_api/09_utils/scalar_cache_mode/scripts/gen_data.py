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
CTRL_SIZE = 8


def gen_golden_data():
    """
    生成Golden数据：
    1. input.bin：输入数据（256个half），用于占位
    2. golden_ctrl.bin：寄存器值验证（8个int64）
    
    CTRL寄存器bit映射：
    - READ mode: CTRL[19:16]
    - WRITE mode: CTRL[23:20]
    
    CacheMode对应的bit值：
    - CACHE_MODE_NORMAL:   0 (0b0000)
    - CACHE_MODE_LAST:     1 (0b0001)
    - CACHE_MODE_PERSISTENT: 2 (0b0010)
    - CACHE_MODE_DISABLE:  4 (0b0100)
    
    golden_ctrl数组顺序：
    [0-3]: READ mode四种CacheMode值
    [4-7]: WRITE mode四种CacheMode值
    """
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    src_data = np.zeros(DATA_SIZE, dtype=np.float16)
    src_data.tofile("./input/input.bin")
    
    golden_ctrl = np.zeros(CTRL_SIZE, dtype=np.int64)
    golden_ctrl[0] = 0   # READ: CACHE_MODE_NORMAL
    golden_ctrl[1] = 1   # READ: CACHE_MODE_LAST
    golden_ctrl[2] = 2   # READ: CACHE_MODE_PERSISTENT
    golden_ctrl[3] = 4   # READ: CACHE_MODE_DISABLE
    golden_ctrl[4] = 0   # WRITE: CACHE_MODE_NORMAL
    golden_ctrl[5] = 1   # WRITE: CACHE_MODE_LAST
    golden_ctrl[6] = 2   # WRITE: CACHE_MODE_PERSISTENT
    golden_ctrl[7] = 4   # WRITE: CACHE_MODE_DISABLE
    golden_ctrl.tofile("./output/golden_ctrl.bin")


if __name__ == "__main__":
    gen_golden_data()