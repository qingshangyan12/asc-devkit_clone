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


TOTAL_COUNT = 4
OUTPUT_TOTAL = 8


def gen_golden_data():
    """
    生成Golden数据：
    - [0-3]：SetCtrlSpr(1)后的寄存器值 → 全为1（int64_t）
    - [3-7]：ResetCtrlSpr后的寄存器值 → 全为0（int64_t）
    
    CTRL[8:6]比特位功能说明：
    - 3'b000（值=0）：不使能原子操作
    - 3'b001（值=1）：使能原子操作，数据类型为float
    """
    os.makedirs("output", exist_ok=True)
    
    golden = np.zeros(OUTPUT_TOTAL, dtype=np.int64)
    golden[0:TOTAL_COUNT] = 1
    golden[TOTAL_COUNT:OUTPUT_TOTAL] = 0
    
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
