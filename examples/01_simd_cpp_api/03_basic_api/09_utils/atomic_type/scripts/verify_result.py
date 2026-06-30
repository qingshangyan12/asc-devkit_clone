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


CTRL_SIZE = 32


def verify_ctrl(real_result, golden):
    """
    验证原子操作寄存器配置值：
    
    原子寄存器格式：
    - atomicType: bit[2:0]
    - atomicOp: bit[5:4]
    - config = (atomicOp << 4) | atomicType
    
    输出格式（每个测试用Duplicate填充4个元素，保证32字节对齐）：
    [0-3]: SetAtomicType<half> -> config=2
    [4-7]: SetAtomicType<float> -> config=1
    [8-11]: SetAtomicType<int32_t> -> config=4
    [12-15]: DisableDmaAtomic -> config=0
    [16-19]: SetAtomicMax<half> -> config=18
    [20-23]: SetAtomicMax<float> -> config=17
    [24-27]: SetAtomicMin<half> -> config=34
    [28-31]: SetAtomicMin<float> -> config=33
    
    如果匹配失败，输出-1
    """
    real_result = np.fromfile(real_result, dtype=np.int64)
    golden = np.fromfile(golden, dtype=np.int64)
    
    expected_values = [2, 2, 2, 2, 1, 1, 1, 1, 4, 4, 4, 4, 0, 0, 0, 0,
                       18, 18, 18, 18, 17, 17, 17, 17, 34, 34, 34, 34, 33, 33, 33, 33]
    mode_names = [
        "SetAtomicType<half>", "SetAtomicType<half>", "SetAtomicType<half>", "SetAtomicType<half>",
        "SetAtomicType<float>", "SetAtomicType<float>", "SetAtomicType<float>", "SetAtomicType<float>",
        "SetAtomicType<int32_t>", "SetAtomicType<int32_t>", "SetAtomicType<int32_t>", "SetAtomicType<int32_t>",
        "DisableDmaAtomic", "DisableDmaAtomic", "DisableDmaAtomic", "DisableDmaAtomic",
        "SetAtomicMax<half>", "SetAtomicMax<half>", "SetAtomicMax<half>", "SetAtomicMax<half>",
        "SetAtomicMax<float>", "SetAtomicMax<float>", "SetAtomicMax<float>", "SetAtomicMax<float>",
        "SetAtomicMin<half>", "SetAtomicMin<half>", "SetAtomicMin<half>", "SetAtomicMin<half>",
        "SetAtomicMin<float>", "SetAtomicMin<float>", "SetAtomicMin<float>", "SetAtomicMin<float>"
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