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

    if scenario_num == 1:
        # Case 1: FLOAT — half add overflow
        # x=60000.0(half) + y=10000.0(half) = 70000.0 > 65504(half MAX) → overflow
        # sat-ON:  65504.0 (half MAX)
        # sat-OFF: +inf
        n = 128
        input_x = np.full(n, 60000.0, dtype=np.float16)
        input_y = np.full(n, 10000.0, dtype=np.float16)
        input_data = np.concatenate([input_x, input_y]).tofile("./input/input.bin")

        golden_sat = np.full(n, 65504.0, dtype=np.float16)
        golden_nosat = np.full(n, np.inf, dtype=np.float16)
        golden = np.concatenate([golden_sat, golden_nosat]).astype(np.float16)
        golden.tofile("./output/golden.bin")
        print(f"[INFO] Case 1: Generated input.bin ({n} half x + {n} half y) and golden.bin")

    elif scenario_num == 2:
        # Case 2: INT — int16 add overflow
        # x=30000(int16) + y=30000(int16) = 60000 > 32767(int16 MAX) → overflow
        # sat-ON:  32767 (int16 MAX)
        # sat-OFF: 60000 truncated to int16 = -5536 (0xEA60 signed)
        n = 128
        input_x = np.full(n, 30000, dtype=np.int16)
        input_y = np.full(n, 30000, dtype=np.int16)
        input_data = np.concatenate([input_x, input_y]).tofile("./input/input.bin")

        golden_sat = np.full(n, 32767, dtype=np.int16)
        golden_nosat = np.full(n, -5536, dtype=np.int16)
        golden = np.concatenate([golden_sat, golden_nosat]).astype(np.int16)
        golden.tofile("./output/golden.bin")
        print(f"[INFO] Case 2: Generated input.bin ({n} int16 x + {n} int16 y) and golden.bin")

    elif scenario_num == 3:
        # Case 3: CAST — int32→int16 conversion overflow
        # x=50000(int32) > 32767(int16 MAX) → overflow
        # sat-ON:  32767 (int16 MAX)
        # sat-OFF: 50000 truncated to int16 = -15536 (0xC350 signed)
        #
        # Output layout: int32→int16 conversion writes to even indices (PART_EVEN),
        # odd indices are zeroed (MODE_ZEROING).
        # Output count = 2 × input count (128 = 64 valid + 64 zeros).
        n = 64
        input_x = np.full(n, 50000, dtype=np.int32)
        input_x.tofile("./input/input.bin")

        golden_sat_packed = np.zeros(n * 2, dtype=np.int16)
        golden_sat_packed[::2] = 32767

        golden_nosat_packed = np.zeros(n * 2, dtype=np.int16)
        golden_nosat_packed[::2] = -15536

        golden = np.concatenate([golden_sat_packed, golden_nosat_packed])
        golden.tofile("./output/golden.bin")
        print(f"[INFO] Case 3: Generated input.bin ({n} int32) and golden.bin (packed int16)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-scenarioNum", type=int, default=1, help="Scenario number (1-3)"
    )
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
