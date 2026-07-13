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

import argparse
import os

import numpy as np


M = 128
K = 128
N = 256


def gen_golden_data(scenario_num=1):
    np.random.seed(9)
    x1_gm = np.random.uniform(-1, 3, [M, K]).astype(np.float16)
    x2_gm = np.random.uniform(-1, 3, [K, N]).astype(np.float16)
    golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if scenario_num == 1:
        # Store both inputs in NZ before they are copied to L1.
        x1_gm = x1_gm.reshape(M // 16, 16, K // 16, 16).transpose(2, 0, 1, 3)
        x2_gm = x2_gm.reshape(K // 16, 16, N // 16, 16).transpose(2, 0, 1, 3)
    elif scenario_num == 3:
        # DN input is the transpose of the corresponding ND matrix.
        x1_gm = x1_gm.transpose(1, 0)
        x2_gm = x2_gm.transpose(1, 0)

    x1_gm.astype(np.float16).tofile("./input/x1_gm.bin")
    x2_gm.astype(np.float16).tofile("./input/x2_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 4))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)

