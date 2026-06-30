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
    验证GetScalarCacheMode返回值是否与SetScalarCacheMode设置的值一致：
    
    输出格式（每个测试用Duplicate填充4个元素，保证32字节对齐）：
    - output_ctrl[0-3]: READ CACHE_MODE_NORMAL，匹配输出0
    - output_ctrl[4-7]: READ CACHE_MODE_LAST，匹配输出1
    - output_ctrl[8-11]: READ CACHE_MODE_PERSISTENT，匹配输出2
    - output_ctrl[12-15]: READ CACHE_MODE_DISABLE，匹配输出4
    - output_ctrl[16-19]: WRITE CACHE_MODE_NORMAL，匹配输出0
    - output_ctrl[20-23]: WRITE CACHE_MODE_LAST，匹配输出1
    - output_ctrl[24-27]: WRITE CACHE_MODE_PERSISTENT，匹配输出2
    - output_ctrl[28-31]: WRITE CACHE_MODE_DISABLE，匹配输出4
    
    如果匹配失败，输出-1
    """
    real_result = np.fromfile(real_result, dtype=np.int64)
    golden = np.fromfile(golden, dtype=np.int64)
    
    expected_values = [0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4,
                       0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4]
    mode_names = [
        "READ CACHE_MODE_NORMAL", "READ CACHE_MODE_NORMAL", "READ CACHE_MODE_NORMAL", "READ CACHE_MODE_NORMAL",
        "READ CACHE_MODE_LAST", "READ CACHE_MODE_LAST", "READ CACHE_MODE_LAST", "READ CACHE_MODE_LAST",
        "READ CACHE_MODE_PERSISTENT", "READ CACHE_MODE_PERSISTENT", "READ CACHE_MODE_PERSISTENT", "READ CACHE_MODE_PERSISTENT",
        "READ CACHE_MODE_DISABLE", "READ CACHE_MODE_DISABLE", "READ CACHE_MODE_DISABLE", "READ CACHE_MODE_DISABLE",
        "WRITE CACHE_MODE_NORMAL", "WRITE CACHE_MODE_NORMAL", "WRITE CACHE_MODE_NORMAL", "WRITE CACHE_MODE_NORMAL",
        "WRITE CACHE_MODE_LAST", "WRITE CACHE_MODE_LAST", "WRITE CACHE_MODE_LAST", "WRITE CACHE_MODE_LAST",
        "WRITE CACHE_MODE_PERSISTENT", "WRITE CACHE_MODE_PERSISTENT", "WRITE CACHE_MODE_PERSISTENT", "WRITE CACHE_MODE_PERSISTENT",
        "WRITE CACHE_MODE_DISABLE", "WRITE CACHE_MODE_DISABLE", "WRITE CACHE_MODE_DISABLE", "WRITE CACHE_MODE_DISABLE"
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