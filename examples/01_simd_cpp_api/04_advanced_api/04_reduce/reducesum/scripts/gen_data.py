#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# ----------------------------------------------------------------------------------------------------------
"""
ReduceSum 测试数据生成
用法: 从 build 目录运行
    cd build
    python3 ../scripts/gen_data.py 60_31    # 生成 [60,31] 数据 → ./input/input_x.bin + ./output/golden.bin
    python3 ../scripts/gen_data.py 60_64    # 生成 [60,64] 数据 → ./input/input_x.bin + ./output/golden.bin
"""

import os, sys, numpy as np

CASES = {
    "60_31": (60, 31),
    "60_64": (60, 64),
}

def gen_data(r_length, a_length):
    x_extent = r_length * a_length

    np.random.seed(42)
    x_gm = np.random.uniform(-10, 10, (r_length, a_length)).astype(np.float32)
    y_gm = np.sum(x_gm, axis=0).astype(np.float32)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x_gm.tofile("./input/input_x.bin")
    y_gm.tofile("./output/golden.bin")

    print(f"shape=[{r_length},{a_length}]  x_extent={x_extent}  y_extent={a_length}")
    print(f"  input[:5] (row0): {x_gm[0,:5]}")
    print(f"  golden[:5]:       {y_gm[:5]}")

if __name__ == "__main__":
    case = sys.argv[1] if len(sys.argv) > 1 else "60_31"
    if case not in CASES:
        print(f"用法: python3 {sys.argv[0]} [60_31|60_64]")
        sys.exit(1)
    gen_data(*CASES[case])
