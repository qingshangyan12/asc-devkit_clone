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


fractal_size = 512
fractal_shape = [16, 16]

# 实现nd2nz
def nd2nz(src):
    nSize = src.shape[0]
    dSize = src.shape[1]
    dst = np.zeros(src.shape).reshape(-1)
    for i in range(dSize // fractal_shape[1]):
        tmp_addr = i * nSize * fractal_shape[1]
        for j in range(nSize):
            block_addr = tmp_addr + j * fractal_shape[1]
            dst[block_addr:block_addr + fractal_shape[1]] = src[j][i * fractal_shape[1]:i * fractal_shape[1] + fractal_shape[1]]
    return dst

def gen_golden_data():
    m = 32
    n = 32
    k = 32
    x1_gm = np.random.randint(1, 10, [m, k]).astype(np.float16)
    x2_gm = np.random.randint(1, 10, [k, n]).astype(np.float16)
    golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)
    x1_gm = nd2nz(x1_gm).astype(np.float16)
    x2_gm = nd2nz(x2_gm).astype(np.float16)
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()