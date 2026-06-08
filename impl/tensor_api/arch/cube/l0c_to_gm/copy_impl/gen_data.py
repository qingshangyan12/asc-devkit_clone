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
"""Generate input/golden data for batch matmul + vector quant.

A: [B, M, K] int8
B: [B, K, N] int8 (transposed to [B, N, K] when written to disk, matching the kernel's
   DNExt-on-GM convention from the single-matrix mmad_tensor_api sample)
Quant: [B, 1, N] float32 -> uint64 via fixpipe encoding (low-13-bit clear + mode-bit-46 set)
Golden: [B, M, N] half (per-batch int32 mmad result scaled by per-batch quant)
"""

import os
import numpy as np


def gen_golden_data():
    B = 128
    M = 32
    N = 32
    K = 32

    # A: [B, M, K] int8
    input_x = np.random.uniform(-2, 2, [B, M, K]).astype(np.int8)
    # B: [B, K, N] int8
    input_y = np.random.uniform(-2, 2, [B, K, N]).astype(np.int8)
    # Quant: [B, 1, N] float32 -> uint64. Shape mirrors the kernel layout
    # MakeFrameLayout<NDExtLayoutPtn>(B, 1, N).
    quant = np.random.uniform(-2, 2, [B, 1, N]).astype(np.float32)
    uint32_quant = np.frombuffer(quant.tobytes(), np.uint32).reshape(B, 1, N).copy()
    uint32_quant &= 0xFFFFE000
    quant_tensor = uint32_quant.astype(np.uint64)
    quant_tensor |= np.uint64(1) << np.uint64(46)

    # Per-batch matmul on int32 to avoid int8 accumulator overflow
    golden = np.matmul(input_x.astype(np.int32), input_y.astype(np.int32))
    # Apply per-batch quant: [B, 1, N] broadcasts naturally over the M dimension
    golden = golden.astype(np.float32) * quant
    golden = golden.astype(np.half)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    # Per-batch transpose [B, K, N] -> [B, N, K] so the on-disk layout matches the kernel's DN-on-GM view
    np.ascontiguousarray(input_y.transpose(0, 2, 1)).tofile("./input/input_y.bin")
    quant_tensor.tofile("./input/quant.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated batch quant data: B={B}, M={M}, N={N}, K={K}")
    print(f"  input_x.bin: {input_x.nbytes} bytes ({input_x.shape}, int8)")
    print(f"  input_y.bin: {input_y.nbytes} bytes ([B, N, K], int8 transposed)")
    print(f"  quant.bin:   {quant_tensor.nbytes} bytes ({quant_tensor.shape}, uint64)")
    print(f"  golden.bin:  {golden.nbytes} bytes ({golden.shape}, half)")


if __name__ == "__main__":
    gen_golden_data()
