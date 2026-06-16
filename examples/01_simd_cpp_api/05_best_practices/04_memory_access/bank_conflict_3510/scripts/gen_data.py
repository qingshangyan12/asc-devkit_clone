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


def gen_golden_add():
    N = 4096
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    return x, y, x + y


def gen_golden_adds():
    N = 4096
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    return x, y, y.astype(np.float32)


def gen_golden_case1():
    N = 4096
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    return x, y, x + y


def gen_golden_case2(repeat=1):
    N = 4096
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    return x, y, x + repeat * y


def gen_golden_dintlv():
    N = 4096
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    half = N // 2
    golden = np.zeros(N, dtype=np.float32)
    golden[:half] = x[::2] + x[1::2]
    golden[half:] = y[::2] + y[1::2]
    return x, y, golden


def gen_golden_gather(case, repeat=1):
    N = 256
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    if case == 7:
        indices = np.zeros(N, dtype=np.uint32)
    elif case == 8:
        indices = np.tile(np.arange(8, dtype=np.uint32), N // 8)[:N]
    elif case == 9:
        indices = np.arange(N, dtype=np.uint32)
    elif case == 10:
        indices = np.empty(N, dtype=np.uint32)
        indices[::2] = 0
        indices[1::2] = 128

    golden = y.copy()
    one_rep = 64
    for b in range(N // one_rep):
        batch_idx = indices[b * one_rep : (b + 1) * one_rep]
        for pos in np.unique(batch_idx):
            golden[pos] = y[pos] + float(repeat)
    return indices, y, golden


def gen_golden_adds_512():
    N = 512
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    return x, y, y.astype(np.float32)


def gen_golden_block_stride0():
    N = 512
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    golden = np.zeros(N, dtype=np.float32)
    for i in range(8):
        for j in range(64):
            golden[i * 64 + j] = y[i * 64 + j % 8]
    return x, y, golden


def gen_golden_repeat_stride0():
    N = 512
    x = np.random.uniform(1, 100, [N]).astype(np.float32)
    y = np.random.uniform(1, 100, [N]).astype(np.float32)
    golden = np.tile(y[:64], 8)
    return x, y, golden


def gen_golden_data(case=1, repeat=1):
    if case == 1:
        x, y, golden = gen_golden_case1()
    elif case == 2:
        x, y, golden = gen_golden_case2(repeat)
    elif case == 4:
        x, y, golden = gen_golden_dintlv()
    elif case in (3, 5, 6):
        x, y, golden = gen_golden_add()
    elif case in (7, 8, 9, 10):
        x, y, golden = gen_golden_gather(case, repeat)
    elif case in (11, 12):
        x, y, golden = gen_golden_adds_512()
    elif case == 13:
        x, y, golden = gen_golden_block_stride0()
    elif case == 14:
        x, y, golden = gen_golden_repeat_stride0()
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
    parser.add_argument("--repeat", "-r", type=int, help="repeat times (for gather/scatter)", default=1)
    args = parser.parse_args()
    gen_golden_data(args.case, args.repeat)
