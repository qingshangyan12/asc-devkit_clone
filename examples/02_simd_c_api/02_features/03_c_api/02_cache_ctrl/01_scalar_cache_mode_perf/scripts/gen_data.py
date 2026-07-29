#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
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
    data_len = 16384

    if scenario_num <= 2:
        # Case 1-2: Read channel tests
        # Generate input.bin with 16384 random float32 values
        input_data = rng.random(size=data_len).astype(np.float32)
        input_data.tofile("./input/input.bin")
        print(f"[INFO] Case {scenario_num}: Generated input.bin with {data_len} float32 values")
    else:
        # Case 3-4: Write channel tests
        # Generate golden.bin: dst[i] = float(i)
        golden = np.arange(data_len, dtype=np.float32)
        golden.tofile("./output/golden.bin")
        print(f"[INFO] Case {scenario_num}: Generated golden.bin with {data_len} float32 values (dst[i] = i)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-scenarioNum", type=int, default=1, help="Scenario number (1-4)"
    )
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
