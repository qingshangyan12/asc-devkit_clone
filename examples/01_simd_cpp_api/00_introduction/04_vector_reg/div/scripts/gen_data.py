#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np

I64_MIN = np.iinfo(np.int64).min
I64_MAX = np.iinfo(np.int64).max

def cpp_div(a, b):
    ai = int(a)
    bi = int(b)
    q = ai // bi
    if q * bi != ai:
        if (ai < 0) != (bi < 0):
            q += 1
    return np.int64(q)

def gen_golden_data_simple():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # === INT64_MIN divisor test ===
    dividends_int64min = np.array([
        I64_MIN,
        I64_MIN + 1,
        I64_MIN + 2,
        I64_MIN + 100,
        -2147483648,
        -1000000,
        -1000,
        -100,
        -3,
        -2,
        -1,
        0,
        1,
        2,
        3,
        7,
        42,
        100,
        1000,
        1000000,
        2147483647,
        2147483648,
        I64_MAX - 100,
        I64_MAX - 2,
        I64_MAX - 1,
        I64_MAX,
        I64_MIN + 1000000,
        I64_MIN // 2,
        I64_MAX // 2,
        -42,
        I64_MIN + 100000,
        I64_MIN + 3,
    ], dtype=np.int64)

    divisor_int64min = I64_MIN
    golden_int64min = np.array([cpp_div(d, divisor_int64min) for d in dividends_int64min], dtype=np.int64)

    dividends_int64min.tofile('./input/input_int64min.bin')
    golden_int64min.tofile('./output/golden_int64min.bin')
    print("Generated INT64_MIN divisor test (32 values)")

    # === Normal divisor test (divisor=3) ===
    dividends_normal = np.array([
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12,
        100, -100, 1000, -1000, I64_MAX, I64_MIN, I64_MIN + 1,
    ], dtype=np.int64)

    divisor_normal = np.int64(3)
    golden_normal = np.array([cpp_div(d, divisor_normal) for d in dividends_normal], dtype=np.int64)

    dividends_normal.tofile('./input/input_normal.bin')
    golden_normal.tofile('./output/golden_normal.bin')
    print("Generated normal divisor test (32 values, divisor=3)")
    print()
    print("Normal division golden values:")
    for i in range(32):
        print(f"  [{i:2d}] {dividends_normal[i]:>20d} / 3 = {golden_normal[i]}")

if __name__ == "__main__":
    gen_golden_data_simple()
