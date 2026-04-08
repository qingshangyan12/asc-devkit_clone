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


import os
import sys
import argparse
import numpy as np


def swish(x, beta):
    """Swish激活函数: y = x / (1 + exp(-beta * x))"""
    return x / (1 + np.exp(-beta * x))


def silu(x):
    """Silu激活函数: y = x / (1 + exp(-x)), 即beta=1的Swish"""
    return x / (1 + np.exp(-x))


def swiglu(src0, src1, beta=1):
    """SwiGLU门控线性单元: dst = src0 * Swish(src1)"""
    return src0 * swish(src1, beta)


def gen_golden_data(mode="swish"):
    dtype = np.float32
    np.set_printoptions(threshold=sys.maxsize, suppress=True)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if mode == "swiglu":
        # SwiGLU模式：双输入
        src0 = np.random.uniform(-4.0, 4.0, [32]).astype(dtype)
        src1 = np.random.uniform(-4.0, 4.0, [32]).astype(dtype)
        src0.tofile("./input/input_src0.bin")
        src1.tofile("./input/input_src1.bin")
        golden = swiglu(src0, src1, beta=1)
        print(f"Generated data for SwiGLU mode: input [32], [32], output {golden.shape}")
    elif mode == "silu":
        # Silu模式：单输入，beta=1
        src = np.random.uniform(-4.0, 4.0, [32]).astype(dtype)
        src.tofile("./input/input_src.bin")
        golden = silu(src)
        print(f"Generated data for Silu mode: input [32], output {golden.shape}")
    else:
        # Swish模式（默认）：单输入，beta=1.702
        src = np.random.uniform(-4.0, 4.0, [32]).astype(dtype)
        src.tofile("./input/input_src.bin")
        golden = swish(src, beta=1.702)
        print(f"Generated data for Swish mode: input [32], output {golden.shape}")

    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate test data for Swish/Silu/SwiGLU")
    parser.add_argument("--silu-mode", action="store_true", help="Enable Silu mode")
    parser.add_argument("--swiglu-mode", action="store_true", help="Enable SwiGLU mode")
    args = parser.parse_args()

    if args.swiglu_mode:
        gen_golden_data(mode="swiglu")
    elif args.silu_mode:
        gen_golden_data(mode="silu")
    else:
        gen_golden_data(mode="swish")
