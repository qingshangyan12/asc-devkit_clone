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
    
    验证逻辑：
    - SetScalarCacheMode设置CacheMode
    - GetScalarCacheMode获取CacheMode
    - 比较获取的mode是否与设置的mode一致
    
    Duplicate需要32字节对齐，每个测试用Duplicate填充4个元素（4 * int64_t = 32字节）
    
    rwMode有2种（READ/WRITE），cacheMode有4种，共8种组合：
    [0-3]:   READ CACHE_MODE_NORMAL，输出0
    [4-7]:   READ CACHE_MODE_LAST，输出1
    [8-11]:  READ CACHE_MODE_PERSISTENT，输出2
    [12-15]: READ CACHE_MODE_DISABLE，输出4
    [16-19]: WRITE CACHE_MODE_NORMAL，输出0
    [20-23]: WRITE CACHE_MODE_LAST，输出1
    [24-27]: WRITE CACHE_MODE_PERSISTENT，输出2
    [28-31]: WRITE CACHE_MODE_DISABLE，输出4
    """
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    src_data = np.zeros(DATA_SIZE, dtype=np.float16)
    src_data.tofile("./input/input.bin")
    
    golden_ctrl = np.zeros(CTRL_SIZE, dtype=np.int64)
    golden_ctrl[0:4] = 0    # READ CACHE_MODE_NORMAL
    golden_ctrl[4:8] = 1    # READ CACHE_MODE_LAST
    golden_ctrl[8:12] = 2   # READ CACHE_MODE_PERSISTENT
    golden_ctrl[12:16] = 4  # READ CACHE_MODE_DISABLE
    golden_ctrl[16:20] = 0  # WRITE CACHE_MODE_NORMAL
    golden_ctrl[20:24] = 1  # WRITE CACHE_MODE_LAST
    golden_ctrl[24:28] = 2  # WRITE CACHE_MODE_PERSISTENT
    golden_ctrl[28:32] = 4  # WRITE CACHE_MODE_DISABLE
    golden_ctrl.tofile("./output/golden_ctrl.bin")


if __name__ == "__main__":
    gen_golden_data()