#!/usr/bin/python3

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


def gen_elu_golden(x, alpha, scale, input_scale):
    """计算 ELU golden：x > 0 时取 scale*x，否则取 alpha*scale*(exp(x*inputScale)-1)"""
    x_float = x.astype(np.float32)
    # np.where 会先计算两个分支，正数部分 exp 可能溢出为 inf，但不会被选中，用 errstate 屏蔽告警
    with np.errstate(over="ignore", invalid="ignore"):
        golden = np.where(
            x_float > 0,
            scale * x_float,
            alpha * scale * (np.exp(x_float * input_scale) - 1),
        )
    return golden


SHAPES = [
    1,
    31,
    32,
    100,
    1023,
    1024,
    2047,
    2048,
    8191,
    8195,
    8 * 2048,
    8 * 8192,
    100000,
    1048575,
]


def gen_golden_data():
    alpha = 1.0
    scale = 1.0
    input_scale = 1.0
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    for total_length in SHAPES:
        x = np.random.uniform(-100, 100, [1, total_length]).astype(np.float32)
        golden = gen_elu_golden(x, alpha, scale, input_scale)
        x.tofile(f"./input/input_x_{total_length}.bin")
        golden.tofile(f"./output/golden_{total_length}.bin")
        x_half = x.astype(np.float16)
        golden_half = gen_elu_golden(x_half, alpha, scale, input_scale).astype(
            np.float16
        )
        x_half.tofile(f"./input/input_x_fp16_{total_length}.bin")
        golden_half.tofile(f"./output/golden_fp16_{total_length}.bin")


if __name__ == "__main__":
    gen_golden_data()
