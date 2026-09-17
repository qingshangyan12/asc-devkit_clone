#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import os
import numpy as np


def gen_golden_data(batch=4, m=30, k=40, n=70):
    b, m, k, n = (int(value) for value in (batch, m, k, n))
    if min(b, m, k, n) <= 0:
        raise ValueError("batch, m, k, and n must all be positive")
    np.random.seed(0)
    # 带batch的矩阵乘，输入[B, M, K]和[B, K, N]
    input_type = np.dtype("float32")
    x1_gm = np.random.uniform(-100, 100, [b, m, k]).astype(input_type)
    x2_gm = np.random.uniform(-100, 100, [b, k, n]).astype(input_type)
    golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))
    golden = golden.astype(np.float32)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Generate deterministic batch-matmul inputs and golden output."
    )
    parser.add_argument("--batch", type=int, default=4)
    parser.add_argument("--m", type=int, default=30)
    parser.add_argument("--k", type=int, default=40)
    parser.add_argument("--n", type=int, default=70)
    args = parser.parse_args()
    gen_golden_data(args.batch, args.m, args.k, args.n)
