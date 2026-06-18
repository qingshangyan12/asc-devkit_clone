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


def gen_golden_data(scenario_num):
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(0)

    if scenario_num <= 3:
        # Case 1-3: 抽取自 data_copy 样例，矩阵 [12288, 12288]，half 类型
        m = 12288
        n = 12288
        input_data = rng.integers(-2, 3, size=m * n, dtype=np.int16).astype(np.float16)
        input_data.tofile("./input/input.bin")
        print(f"[INFO] Case {scenario_num}: Generated input.bin with shape [{m}, {n}]")
    elif scenario_num <= 5:
        # Case 4-5: 抽取自 add_high_performance 样例，矩阵 [8192, 8192]，half 类型
        m = 8192
        n = 8192
        input_x = rng.integers(-2, 3, size=m * n, dtype=np.int16).astype(np.float16)
        input_y = rng.integers(-2, 3, size=m * n, dtype=np.int16).astype(np.float16)
        golden = (input_x + input_y).astype(np.float16)
        input_x.tofile("./input/input_x.bin")
        input_y.tofile("./input/input_y.bin")
        golden.tofile("./output/golden.bin")
        print(f"[INFO] Case {scenario_num}: Generated input_x.bin, input_y.bin with shape [{m}, {n}]")
        print(f"[INFO] Case {scenario_num}: Generated golden.bin (x + y) for verification")
    elif scenario_num <= 7:
        # Case 6-7: 合并 Repeat + Add Kernel
        # Add 矩阵 [8192, 8192]，Repeat 矩阵 [12288, 12288]
        add_m = 8192
        add_n = 8192
        repeat_m = 12288
        repeat_n = 12288
        add_x = rng.integers(-2, 3, size=add_m * add_n, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=add_m * add_n, dtype=np.int16).astype(np.float16)
        add_x.tofile("./input/input_x.bin")
        add_y.tofile("./input/input_y.bin")
        golden = (add_x + add_y).astype(np.float16)
        golden.tofile("./output/golden.bin")

        rng2 = np.random.default_rng(42)
        repeat_input = rng2.integers(-2, 3, size=repeat_m * repeat_n, dtype=np.int16).astype(np.float16)
        repeat_input.tofile("./input/repeat_input.bin")

        print(f"[INFO] Case {scenario_num}: Add=[{add_m},{add_n}], Repeat=[{repeat_m},{repeat_n}]")
        print(f"[INFO] Case {scenario_num}: Generated golden.bin (x + y) for verification")
    else:
        # Case 8-9: 双流并行 — Add + DataCopy Repeat 各占一半核
        # Add 矩阵 [8192, 8192]，Repeat 矩阵 [12288, 12288]
        add_m = 8192
        add_n = 8192
        repeat_m = 12288
        repeat_n = 12288
        add_x = rng.integers(-2, 3, size=add_m * add_n, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=add_m * add_n, dtype=np.int16).astype(np.float16)
        add_x.tofile("./input/input_x.bin")
        add_y.tofile("./input/input_y.bin")
        golden = (add_x + add_y).astype(np.float16)
        golden.tofile("./output/golden.bin")

        rng2 = np.random.default_rng(42)
        repeat_input = rng2.integers(-2, 3, size=repeat_m * repeat_n, dtype=np.int16).astype(np.float16)
        repeat_input.tofile("./input/repeat_input.bin")

        print(f"[INFO] Case {scenario_num}: Dual-stream | Add=[{add_m},{add_n}], Repeat=[{repeat_m},{repeat_n}]")
        print(f"[INFO] Case {scenario_num}: Generated golden.bin (x + y) for verification")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1,
                        help="Scenario number (1-9)")
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
