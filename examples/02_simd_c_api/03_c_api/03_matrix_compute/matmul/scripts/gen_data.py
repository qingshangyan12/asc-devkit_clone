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


def gen_golden_data():
    # 维度需与 matmul.asc 中的 M / K / N 保持一致
    m = 30
    k = 40
    n = 70

    # 取正区间：若取 [-10, 10]，累加结果会集中在 0 附近，
    # np.isclose 的容差退化为 absolute_tol，float32 的累加误差与之同量级，
    # 会让精度比对随机失败。
    x1_gm = np.random.uniform(1, 10, [m, k]).astype(np.float32)
    x2_gm = np.random.uniform(1, 10, [k, n]).astype(np.float32)
    bias_gm = np.random.uniform(1, 10, [n]).astype(np.float32)
    golden = (np.matmul(x1_gm, x2_gm) + bias_gm).astype(np.float32)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    bias_gm.tofile("./input/bias_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
