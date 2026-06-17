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

    input_dir = "./input"
    output_dir = "./output"
    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)

    rng = np.random.default_rng(0)

    if scenario_num <= 3:
        # Case 1-3: 重复搬运场景，仅需一个输入矩阵
        repeat_input = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        repeat_input.tofile(f"{input_dir}/repeat_input.bin")
        print(f"[INFO] Case {scenario_num}: Generated repeat_input.bin with shape [{M}, {N}]")

    elif scenario_num <= 5:
        # Case 4-5: Add 算子场景，需要两个输入矩阵
        add_x = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_x.tofile(f"{input_dir}/add_x.bin")
        add_y.tofile(f"{input_dir}/add_y.bin")
        add_output = add_x + add_y
        add_output.tofile(f"{output_dir}/golden_add.bin")
        print(f"[INFO] Case {scenario_num}: Generated add_x.bin, add_y.bin with shape [{M}, {N}]")
        print(f"[INFO] Case {scenario_num}: Generated golden_add.bin (x + y) for verification")

    elif scenario_num <= 7:
        # Case 6-7: 流水线场景（两个独立 kernel 串行）
        add_x = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_x.tofile(f"{input_dir}/add_x.bin")
        add_y.tofile(f"{input_dir}/add_y.bin")
        add_output = add_x + add_y
        add_output.tofile(f"{output_dir}/golden_add.bin")

        rng2 = np.random.default_rng(42)
        repeat_input = rng2.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        repeat_input.tofile(f"{input_dir}/repeat_input.bin")

        print(f"[INFO] Case {scenario_num}: Generated add_x.bin, add_y.bin, repeat_input.bin with shape [{M}, {N}]")
        print(f"[INFO] Case {scenario_num}: Generated golden_add.bin (x + y) for verification")

    else:
        # Case 10-11: 合并 kernel 场景，需要三个输入矩阵
        add_x = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_y = rng.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        add_x.tofile(f"{input_dir}/add_x.bin")
        add_y.tofile(f"{input_dir}/add_y.bin")
        add_output = add_x + add_y
        add_output.tofile(f"{output_dir}/golden_add.bin")

        rng2 = np.random.default_rng(42)
        repeat_input = rng2.integers(-2, 3, size=ELEMENT_COUNT, dtype=np.int16).astype(np.float16)
        repeat_input.tofile(f"{input_dir}/repeat_input.bin")

        print(f"[INFO] Case {scenario_num}: Generated add_x.bin, add_y.bin, repeat_input.bin with shape [{M}, {N}]")
        print(f"[INFO] Case {scenario_num}: Generated golden_add.bin (x + y) for verification")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1,
                        help="Scenario number (1-7, 10, 11)")
    args = parser.parse_args()

    gen_golden_data(args.scenarioNum)
