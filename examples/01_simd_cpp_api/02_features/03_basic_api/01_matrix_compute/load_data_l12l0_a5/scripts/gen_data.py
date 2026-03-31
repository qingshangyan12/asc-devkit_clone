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
import argparse
import numpy as np
np.random.seed(9)


def gen_golden_data(scenarioNum=1, M=0, K=0, N=0):
    x1_gm = None
    x2_gm = None
    golden = None

    if scenarioNum == 1:
        # int8_t, A不转置B转置: A[M,K], B[N,K]
        x1_gm = np.random.uniform(1, 10, [M, K]).astype(np.int8)
        x2_gm = np.random.uniform(1, 10, [K, N]).astype(np.int8)
        golden = (np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))).astype(np.int32)
        x2_gm = x2_gm.transpose()
    elif scenarioNum == 2:
        # int8_t, A转置B不转置: A[K,M], B[K,N]
        x1_gm = np.random.uniform(1, 10, [M, K]).astype(np.int8)
        x2_gm = np.random.uniform(1, 10, [K, N]).astype(np.int8)
        golden = (np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))).astype(np.int32)
        x1_gm = x1_gm.transpose()
    elif scenarioNum == 3:
        # half, A不转置B转置: A[M,K], B[N,K]
        x1_gm = np.random.uniform(1, 10, [M, K]).astype(np.float16)
        x2_gm = np.random.uniform(1, 10, [K, N]).astype(np.float16)
        golden = (np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))).astype(np.float32)
        x2_gm = x2_gm.transpose()
    elif scenarioNum == 4:
        # half, A转置B不转置: A[K,M], B[K,N]
        x1_gm = np.random.uniform(1, 10, [M, K]).astype(np.float16)
        x2_gm = np.random.uniform(1, 10, [K, N]).astype(np.float16)
        golden = (np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))).astype(np.float32)
        x1_gm = x1_gm.transpose()
    elif scenarioNum == 5:
        # float, A不转置B转置: A[M,K], B[N,K]
        x1_gm = np.random.uniform(1, 10, [M, K]).astype(np.float32)
        x2_gm = np.random.uniform(1, 10, [K, N]).astype(np.float32)
        golden = (np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))).astype(np.float32)
        x2_gm = x2_gm.transpose()
    elif scenarioNum == 6:
        # float, A转置B不转置: A[K,M], B[K,N]
        x1_gm = np.random.uniform(1, 10, [M, K]).astype(np.float32)
        x2_gm = np.random.uniform(1, 10, [K, N]).astype(np.float32)
        golden = (np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))).astype(np.float32)
        x1_gm = x1_gm.transpose()

    if scenarioNum <= 2:
        golden = golden.astype(np.int32)
    else:
        golden = golden.astype(np.float32)

    print("x1_gm == ", x1_gm)
    print("x2_gm == ", x2_gm)

    os.system("mkdir -p input")
    os.system("mkdir -p output")

    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 7))
    parser.add_argument('-m', type=int)
    parser.add_argument('-k', type=int)
    parser.add_argument('-n', type=int)
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.m, args.k, args.n)