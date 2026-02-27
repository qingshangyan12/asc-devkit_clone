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
import argparse 

# 实现ND2Nz，将GM中数据排布处理为Nz格式
def nd2nz(src, fractal_shape):
    nSize = src.shape[0]
    dSize = src.shape[1]
    dst = np.zeros(src.shape).astype(src.dtype).reshape(-1)
    for i in range(dSize // fractal_shape[1]):
        tmp_addr = i * nSize * fractal_shape[1]
        for j in range(nSize):
            block_addr = tmp_addr + j * fractal_shape[1]
            dst[block_addr:block_addr + fractal_shape[1]] = src[j][i * fractal_shape[1]:i * fractal_shape[1] + fractal_shape[1]]
    return dst


def gen_golden_data(scenarioNum, m, k, n):
    x1_gm = None
    x2_gm = None
    golden = None
    fractal_shape = None
    if scenarioNum == 1 or scenarioNum == 2:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.int8)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.int8)
        golden = np.matmul(x1_gm.astype(np.int32), x2_gm.astype(np.int32).T).astype(np.int32)
        fractal_shape = [16, 32]
    elif scenarioNum == 3:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.float16)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.float16)
        golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)
        fractal_shape = [16, 16]
    elif scenarioNum == 4:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.float16)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.float16)
        golden = np.matmul(x1_gm.astype(np.float32).T, x2_gm.astype(np.float32)).astype(np.float32)
        fractal_shape = [16, 16]
    elif scenarioNum == 5 or scenarioNum == 7:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.float16)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.float16)
        golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32).T).astype(np.float32)
        fractal_shape = [16, 16]
    elif scenarioNum == 6:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.float16)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.float16)
        golden = np.matmul(x1_gm.astype(np.float32).T, x2_gm.astype(np.float32).T).astype(np.float32)
        fractal_shape = [16, 16]
    elif scenarioNum == 8 or scenarioNum == 9:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.float32)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.float32)
        golden = np.matmul(x1_gm, x2_gm.T).astype(np.float32)
        fractal_shape = [16, 8]

    x1_gm = nd2nz(x1_gm, fractal_shape)
    x2_gm = nd2nz(x2_gm, fractal_shape)
    np.set_printoptions(threshold=np.inf, linewidth=np.inf)
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 14))
    parser.add_argument('-m', type=int)
    parser.add_argument('-k', type=int)
    parser.add_argument('-n', type=int)
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.m, args.k, args.n)