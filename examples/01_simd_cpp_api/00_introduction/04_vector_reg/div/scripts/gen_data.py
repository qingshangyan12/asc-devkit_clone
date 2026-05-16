#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np

I64_MIN = np.iinfo(np.int64).min
I64_MAX = np.iinfo(np.int64).max

def gen_golden_data_simple():
    dividends = np.array([
        I64_MIN,                  # -2^63:  expected 1
        I64_MIN + 1,              #         expected 0
        I64_MIN + 2,              #         expected 0
        I64_MIN + 100,            #         expected 0
        -2147483648,              # -2^31:  expected 0
        -1000000,                 #         expected 0
        -1000,                    #         expected 0
        -100,                     #         expected 0
        -3,                       #         expected 0
        -2,                       #         expected 0
        -1,                       #         expected 0
        0,                        #         expected 0
        1,                        #         expected 0
        2,                        #         expected 0
        3,                        #         expected 0
        7,                        #         expected 0
        42,                       #         expected 0
        100,                      #         expected 0
        1000,                     #         expected 0
        1000000,                  #         expected 0
        2147483647,               # 2^31-1: expected 0
        2147483648,               # 2^31:   expected 0
        I64_MAX - 100,            #         expected 0
        I64_MAX - 2,              #         expected 0
        I64_MAX - 1,              #         expected 0
        I64_MAX,                  # 2^63-1: expected 0
        I64_MIN + 1000000,        #         expected 0
        I64_MIN // 2,             #         expected 0
        I64_MAX // 2,             #         expected 0
        -42,                      #         expected 0
        I64_MIN + 100000,         #         expected 0
        I64_MIN + 3,              #         expected 0
    ], dtype=np.int64)
    # exactly 32 elements

    divisor = I64_MIN
    golden = dividends // divisor  # Python floor division

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    dividends.tofile('./input/input_x.bin')
    golden.tofile('./output/golden.bin')
    print("Generated 32 test values")
    print("INT64_MIN =", I64_MIN)
    print("INT64_MAX =", I64_MAX)
    print("Expected golden values (dividend // INT64_MIN):")
    for i in range(32):
        print(f"  [{i:2d}] {dividends[i]:>20d} // {divisor:>20d} = {golden[i]}")

if __name__ == "__main__":
    gen_golden_data_simple()
