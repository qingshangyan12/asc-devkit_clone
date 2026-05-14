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
import ml_dtypes
import en_dtypes
np.random.seed(9)

bfloat16 = ml_dtypes.bfloat16
fp4_e1m2x2 = en_dtypes.float4_e1m2

SCENARIOS = {
    1: {
        "m": 256,
        "n": 256,
        "k": 256,
        "trans_a": False,
        "trans_b": True,
    },
    2: {
        "m": 40,
        "n": 80,
        "k": 70,
        "trans_a": False,
        "trans_b": True,
    },
    3: {
        "m": 1024,
        "n": 1024,
        "k": 1024,
        "trans_a": False,
        "trans_b": True,
    },
}


def pack_two_fp4(fp4_matrix):
    row = fp4_matrix.shape[0]
    col = fp4_matrix.shape[1]
    fp4_flat = fp4_matrix.flatten()
    high = fp4_flat[::2].view(np.uint8)
    low = fp4_flat[1::2].view(np.uint8)
    packed = ((low & 0x0F) << 4) | (high & 0x0F)
    return packed.reshape(row, col // 2)


def gen_golden_data(scenario_num=1):
    config = SCENARIOS[scenario_num]
    m, n, k = config["m"], config["n"], config["k"]
    trans_a, trans_b = config["trans_a"], config["trans_b"]
    sk = int(np.ceil(k / 64) * 2)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x1_gm = np.random.randint(-1, 2, [m, k]).astype(fp4_e1m2x2)
    x2_gm = np.random.randint(-1, 2, [k, n]).astype(fp4_e1m2x2)
    x1_scale_gm = np.random.randint(127, 130, [m, sk]).astype(np.uint8)
    x2_scale_gm = np.random.randint(127, 130, [sk, n]).astype(np.uint8)

    x1_mx = 2 ** (x1_scale_gm.astype(np.float64) - 127)
    x2_mx = 2 ** (x2_scale_gm.astype(np.float64) - 127)
    x1_full = np.zeros([m, k], dtype=np.float64)
    x2_full = np.zeros([k, n], dtype=np.float64)

    for i in range(k):
        x1_full[:, i] = x1_gm[:, i] * x1_mx[:, i // 32]
        x2_full[i, :] = x2_gm[i, :] * x2_mx[i // 32, :]

    golden = np.matmul(x1_full, x2_full).astype(bfloat16)

    if trans_a:
        x1_gm = x1_gm.transpose()
        x1_scale_gm = x1_scale_gm.reshape(m, int(sk / 2), 2).transpose(1, 0, 2)

    if trans_b:
        x2_gm = x2_gm.transpose()
        x2_scale_gm = x2_scale_gm.transpose()
    else :
        x2_scale_gm = x2_scale_gm.reshape(int(sk / 2), 2, n).transpose(0, 2, 1)

    pack_two_fp4(x1_gm).tofile("./input/x1_gm.bin")
    pack_two_fp4(x2_gm).tofile("./input/x2_gm.bin")
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=SCENARIOS.keys())
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
