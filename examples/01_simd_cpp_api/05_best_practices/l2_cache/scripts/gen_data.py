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

# 所有 case 统一使用 [12288, 12288] 的 half 类型矩阵
M = 12288
N = 12288
ELEMENT_COUNT = M * N


def gen_golden_data(scenario_num):
    """根据 scenario 编号生成对应的输入数据文件"""

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(0)

    if scenario_num <= 3:
        # Case 1-3: 重复搬运场景，仅需一个输入矩阵
        repeat_input = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        repeat_input.tofile("./input/repeat_input.bin")
        print(f"[INFO] Case {scenario_num}: Generated repeat_input.bin with shape [{M}, {N}]")

    elif scenario_num <= 5:
        # Case 4-5: Add 算子场景，需要两个输入矩阵
        add_x = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_x.tofile("./input/add_x.bin")
        add_y.tofile("./input/add_y.bin")
        add_output = add_x + add_y
        add_output.tofile("./output/golden_add.bin")
        print(f"[INFO] Case {scenario_num}: Generated add_x.bin, add_y.bin with shape [{M}, {N}]")
        print(f"[INFO] Case {scenario_num}: Generated golden_add.bin (x + y) for verification")

    else:
        # Case 6-7: 流水线场景
        # 生成 Add 算子的两个输入矩阵
        add_x = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_x.tofile("./input/add_x.bin")
        add_y.tofile("./input/add_y.bin")
        add_output = add_x + add_y
        add_output.tofile("./output/golden_add.bin")

        # 生成 Repeat kernel 的输入矩阵（与 Add 不同的数据，用于展示 L2 污染的影响）
        rng2 = np.random.default_rng(42)  # 使用不同的种子，确保数据不同
        repeat_input = rng2.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        repeat_input.tofile("./input/repeat_input.bin")

        print(f"[INFO] Case {scenario_num}: Generated add_x.bin, add_y.bin, repeat_input.bin with shape [{M}, {N}]")
        print(f"[INFO] Case {scenario_num}: Generated golden_add.bin (x + y) for verification")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 8),
                        help="Scenario number (1-7)")
    args = parser.parse_args()

    gen_golden_data(args.scenarioNum)
