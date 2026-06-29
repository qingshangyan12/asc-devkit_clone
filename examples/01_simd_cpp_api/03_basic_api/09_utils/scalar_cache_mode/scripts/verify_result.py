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


def verify_ctrl(real_result, golden):
    """
    验证CTRL寄存器值：
    - golden_ctrl[0]: READ CACHE_MODE_NORMAL = 0
    - golden_ctrl[1]: READ CACHE_MODE_LAST = 1
    - golden_ctrl[2]: READ CACHE_MODE_PERSISTENT = 2
    - golden_ctrl[3]: READ CACHE_MODE_DISABLE = 4
    - golden_ctrl[4]: WRITE CACHE_MODE_NORMAL = 0
    - golden_ctrl[5]: WRITE CACHE_MODE_LAST = 1
    - golden_ctrl[6]: WRITE CACHE_MODE_PERSISTENT = 2
    - golden_ctrl[7]: WRITE CACHE_MODE_DISABLE = 4
    """
    real_result = np.fromfile(real_result, dtype=np.int64)
    golden = np.fromfile(golden, dtype=np.int64)
    
    expected_values = [0, 1, 2, 4, 0, 1, 2, 4]
    mode_names = [
        "READ CACHE_MODE_NORMAL", "READ CACHE_MODE_LAST",
        "READ CACHE_MODE_PERSISTENT", "READ CACHE_MODE_DISABLE",
        "WRITE CACHE_MODE_NORMAL", "WRITE CACHE_MODE_LAST",
        "WRITE CACHE_MODE_PERSISTENT", "WRITE CACHE_MODE_DISABLE"
    ]
    
    for i in range(len(real_result)):
        if real_result[i] != expected_values[i]:
            print(f"[ERROR] {mode_names[i]}: expected {expected_values[i]}, got {real_result[i]}")
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