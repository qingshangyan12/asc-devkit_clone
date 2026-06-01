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

    np.random.seed(0)  # 固定随机种子，保证结果可复现
    input_float = np.random.randn(128, 128).astype(np.float32)  # 强制 float32 类型

    def softmax_2d(z):
        max_z = np.max(z, axis=1, keepdims=True)
        exp_z = np.exp(z - max_z)
        sum_exp = np.sum(exp_z, axis=1, keepdims=True)
        return exp_z / sum_exp

    output_truth = softmax_2d(input_float)

    print(output_truth.shape)


    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_float.tofile("./input/input.bin")
    output_truth.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
