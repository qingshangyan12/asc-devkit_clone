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


import numpy as np


CTRL_SIZE = 16


def verify_ctrl(real_result, golden):
    """
    验证CTRL寄存器的HF32模式设置：
    
    CTRL寄存器bit位置：
    - HF32_MODE_BIT = 46 (CTRL[46])
    - HF32_TRANS_MODE_BIT = 47 (CTRL[47])
    
    输出格式（每个测试用Duplicate填充4个元素，保证32字节对齐）：
    [0-3]: DISABLE + NEAREST_EVEN -> 输出0 (CTRL[46]=0, CTRL[47]=0)
    [4-7]: ENABLE + NEAREST_EVEN -> 输出1 (CTRL[46]=1, CTRL[47]=0)
    [8-11]: ENABLE + NEAREST_ZERO -> 输出2 (CTRL[46]=1, CTRL[47]=1)
    [12-15]: DISABLE + NEAREST_ZERO -> 输出3 (CTRL[46]=0, CTRL[47]=1)
    
    如果匹配失败，输出-1
    """
    real_result = np.fromfile(real_result, dtype=np.int64)
    golden = np.fromfile(golden, dtype=np.int64)
    
    expected_values = [0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3]
    mode_names = [
        "DISABLE+NEAREST_EVEN", "DISABLE+NEAREST_EVEN", "DISABLE+NEAREST_EVEN", "DISABLE+NEAREST_EVEN",
        "ENABLE+NEAREST_EVEN", "ENABLE+NEAREST_EVEN", "ENABLE+NEAREST_EVEN", "ENABLE+NEAREST_EVEN",
        "ENABLE+NEAREST_ZERO", "ENABLE+NEAREST_ZERO", "ENABLE+NEAREST_ZERO", "ENABLE+NEAREST_ZERO",
        "DISABLE+NEAREST_ZERO", "DISABLE+NEAREST_ZERO", "DISABLE+NEAREST_ZERO", "DISABLE+NEAREST_ZERO"
    ]
    
    for i in range(len(real_result)):
        if real_result[i] != expected_values[i]:
            print(f"[ERROR] {mode_names[i]} (index {i}): expected {expected_values[i]}, got {real_result[i]}")
            return False
    
    return True


if __name__ == '__main__':
    ctrl_pass = verify_ctrl("./output/output_ctrl.bin", "./output/golden_ctrl.bin")
    if not ctrl_pass:
        print("[ERROR] output_ctrl.bin failed")
    
    if ctrl_pass:
        print("test pass!")
    else:
        print("[ERROR] test failed")