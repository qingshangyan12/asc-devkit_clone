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

import numpy as np
import os
import argparse

TOTAL_LENGTH = 4096

def gen_golden_add():
    x = np.random.uniform(1, 100, [TOTAL_LENGTH]).astype(np.float32)
    y = np.random.uniform(1, 100, [TOTAL_LENGTH]).astype(np.float32)
    return x, y, x + y

def gen_golden_data(case=1):
    if case in (1, 2, 3, 4, 5, 6, 7, 8, 9):
        x, y, golden = gen_golden_add()
    else:
        raise ValueError(f"Unknown case: {case}")

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile("./input/input_x.bin")
    y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate test data")
    parser.add_argument("--case", "-c", type=int, help="case number", default=1)
    args = parser.parse_args()
    gen_golden_data(args.case)
