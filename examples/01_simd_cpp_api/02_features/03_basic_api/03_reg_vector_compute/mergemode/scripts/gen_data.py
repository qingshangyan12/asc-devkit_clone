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


def gen_golden_data_simple():
    total_length = 200
    data_type = np.float32
    # 生成一个 [1, 200] 向量（非VL整数倍，VL=64 for float）
    x = np.random.uniform(-100, 100, [1, total_length]).astype(data_type)
    # 计算逐元素Max(x, x) = x，再ReduceMax = max(x)
    # golden 就是输入向量的最大值
    golden_val = np.max(x)
    # 输出对齐到 32B（8个float），golden 也按 8 个元素存放
    golden = np.zeros(8, dtype=data_type)
    golden[0] = golden_val
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile('./input/input_x.bin')
    golden.tofile('./output/golden.bin')


if __name__ == "__main__":
    gen_golden_data_simple()
