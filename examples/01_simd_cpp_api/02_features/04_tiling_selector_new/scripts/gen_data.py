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
import sys

import numpy as np


SCENARIO_LENGTHS = {
    0: 256000,
    1: 260096,
    2: 256064,
    3: 258112,
}


def parse_scenario():
    if len(sys.argv) < 2:
        return 0
    scenario = int(sys.argv[1])
    if scenario not in SCENARIO_LENGTHS:
        raise ValueError("SCENARIO_NUM must be 0, 1, 2 or 3")
    return scenario


def gen_golden_data_simple(scenario):
    total_length = SCENARIO_LENGTHS[scenario]
    rng = np.random.default_rng(2026 + scenario)
    input_x = rng.uniform(-8, 8, total_length).astype(np.float16)
    input_y = rng.uniform(-8, 8, total_length).astype(np.float16)
    golden = (input_x + input_y).astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")
    print(f"generated scenario {scenario}, totalLength={total_length}")


if __name__ == "__main__":
    gen_golden_data_simple(parse_scenario())
