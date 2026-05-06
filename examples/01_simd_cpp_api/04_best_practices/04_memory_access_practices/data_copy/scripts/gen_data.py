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


SCENARIO_CONFIG = {
    "UB": {
        1: (12288, 12288),
        2: (12288, 12288),
        3: (12288, 12288),
        4: (12288, 12287),
        5: (12288, 12287),
        6: (12288, 12287),
        7: (12288, 1024),
        8: (12288, 1024),
        9: (12288, 4096),
        10: (12288, 4096),
    },
    "L1": {
        1: (12288, 12288),
        2: (12288, 12288),
        3: (12288, 12287),
        4: (12288, 12287),
        5: (12288, 12288),
        6: (12288, 12288),
        7: (12288, 4096),
        8: (12288, 4096),
    },
}


def get_shape(copy_dst, scenario_num):
    try:
        return SCENARIO_CONFIG[copy_dst][scenario_num]
    except KeyError as exc:
        raise ValueError(f"Unsupported {copy_dst} scenario: {scenario_num}") from exc


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
