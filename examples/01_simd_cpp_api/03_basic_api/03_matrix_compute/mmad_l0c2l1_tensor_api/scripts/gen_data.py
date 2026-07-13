#!/usr/bin/env python
# -*- coding: UTF-8 -*-
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


def gen_golden_data():
    M = 128
    N = 128
    K = 128

    # 两次矩阵乘 A*B=C, A*C=Out 会把数值放大，输入取较小范围避免 half 溢出。
    input_a = np.random.uniform(-1, 1, [M, K]).astype(np.float16)
    input_b = np.random.uniform(-1, 1, [K, N]).astype(np.float16)

    # 第一次矩阵乘 A*B=C，结果转为 half（对应 Fixpipe F322F16）。
    c = np.matmul(input_a.astype(np.float32), input_b.astype(np.float32)).astype(np.float16)
    # 第二次矩阵乘 A*C=Out，结果输出到 GM 为 half。
    golden = np.matmul(input_a.astype(np.float32), c.astype(np.float32)).astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_a.tofile("./input/input_a.bin")
    # B 按 DN 布局读取，输入文件以转置后的 [N, K] 保存。
    input_b.transpose().tofile("./input/input_b.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
