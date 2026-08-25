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


VALUES = np.array([-2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0], dtype=np.float16)
SCENARIOS = {
    1: (2, 16, 16, 32),
    2: (1, 32, 32, 32),
}


def generate_data(scenario_num):
    block_dim, m, n, k = SCENARIOS[scenario_num]
    matrix_a = np.empty((block_dim, m, k), dtype=np.float16)
    matrix_b = np.empty((block_dim, k, n), dtype=np.float16)
    for block in range(block_dim):
        for row in range(m):
            for col in range(k):
                matrix_a[block, row, col] = VALUES[(block * 3 + row * 5 + col * 2) % VALUES.size]
        for row in range(k):
            for col in range(n):
                matrix_b[block, row, col] = VALUES[(block * 4 + row * 3 + col * 6 + 1) % VALUES.size]

    golden = np.matmul(matrix_a.astype(np.float32), matrix_b.astype(np.float32)).astype(np.float32)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    matrix_a.tofile("input/x1_gm.bin")
    matrix_b.tofile("input/x2_gm.bin")
    golden.tofile("output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=SCENARIOS.keys())
    args = parser.parse_args()
    generate_data(args.scenarioNum)
