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


def gen_golden_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    n = 64  # input element count (int32)
    input_x = np.full(n, 50000, dtype=np.int32)  # 50000 > 32767 (int16 MAX) → overflow
    input_x.tofile("./input/input.bin")

    # Output layout: int32→int16 conversion uses PART_EVEN + MODE_ZEROING.
    # Valid values at even indices, zeros at odd indices.
    # Output count per case = 2 × input count = 128.
    # Total = 4 cases × 128 = 512 int16 values.

    # Case 1: USE_API + non-_sat API → truncation
    # 50000 (0x0000C350) truncated to int16 (low 16 bits) = 0xC350 = -15536
    golden_1 = np.zeros(n * 2, dtype=np.int16)
    golden_1[::2] = -15536

    # Case 2: USE_API + _sat API → saturation
    # 50000 > 32767 → saturated to 32767 (int16 MAX)
    golden_2 = np.zeros(n * 2, dtype=np.int16)
    golden_2[::2] = 32767

    # Case 3: USE_GLOBAL + flag=true (CTRL[59]=0 → saturation) → saturation
    # Flag overrides non-_sat API suffix
    golden_3 = np.zeros(n * 2, dtype=np.int16)
    golden_3[::2] = 32767

    # Case 4: USE_GLOBAL + flag=false (CTRL[59]=1 → truncation) → truncation
    # Flag overrides _sat API suffix
    golden_4 = np.zeros(n * 2, dtype=np.int16)
    golden_4[::2] = -15536

    golden = np.concatenate([golden_1, golden_2, golden_3, golden_4])
    golden.tofile("./output/golden.bin")
    print(f"[INFO] Generated input.bin ({n} int32) and golden.bin ({n * 2 * 4} int16)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-scenarioNum", type=int, default=1, help="Scenario number (unused, kept for consistency)"
    )
    args = parser.parse_args()
    gen_golden_data()
