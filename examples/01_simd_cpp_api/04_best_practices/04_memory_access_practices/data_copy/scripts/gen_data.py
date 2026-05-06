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


def get_shape(copy_dst, scenario_num):
    if copy_dst == "UB":
        if scenario_num in (1, 2, 3):
            return 12288, 12288
        if scenario_num in (4, 5, 6):
            return 12288, 12287
        raise ValueError(f"Unsupported UB scenario: {scenario_num}")

    if copy_dst == "L1":
        if scenario_num in (1, 2):
            return 12288, 12288
        if scenario_num in (3, 4):
            return 12288, 12287
        raise ValueError(f"Unsupported L1 scenario: {scenario_num}")

    raise ValueError(f"Unsupported copy destination: {copy_dst}")


def gen_golden_data(scenario_num, copy_dst):
    m, n = get_shape(copy_dst, scenario_num)
    element_count = m * n

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(0)
    input_data = rng.integers(-2, 3, size=element_count, dtype=np.int16).astype(np.float16)
    input_data.tofile("./input/input.bin")

    if copy_dst == "UB":
        input_data.tofile("./output/golden.bin")
        print(f"[INFO] Generated UB sample with shape [{m}, {n}]")
        return

    for path in ("./output/output.bin", "./output/golden.bin"):
        if os.path.exists(path):
            os.remove(path)
    print(f"[INFO] Generated L1 sample with shape [{m}, {n}]")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, help="Scenario number")
    parser.add_argument("-copyDst", type=str, default="UB", choices=["UB", "L1"], help="Copy destination")
    args = parser.parse_args()

    gen_golden_data(args.scenarioNum, args.copyDst)
