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

This script encodes a "shared-quant golden vs perturbed-bin" experiment to verify whether a single
fixpipe instruction reads only one quant slice from FB regardless of the ndNum batch repetition:

  1. Build a single quant slice (shape [1, 1, N]) and broadcast it across all B batches when
     computing golden, so golden assumes every batch uses the *same* quant.
  2. Before writing quant.bin, replace batches [1..B-1] with fresh random quants while keeping
     batch 0 unchanged.
  3. Run kernel. If the hardware truly binds one fixpipe instruction to one FB quant slice
     (read at FB base, no per-batch step), every L0BatchSize-sized chunk of batches will
     dequant using its first batch's quant. The first chunk (batches 0..L0BatchSize-1) reads
     batch 0's quant -- which equals what golden used -- so those batches pass. From the
     second chunk onward (batches L0BatchSize..) the FB-base quant becomes the perturbed
     value of batch L0BatchSize, while golden expected the original shared quant -- so those
     batches fail.

A: [B, M, K] int8
B: [B, K, N] int8 (transposed to [B, N, K] when written to disk)
Quant on disk: [B, 1, N] uint64 (batch 0 = shared, batch 1.. = perturbed)
Golden: [B, M, N] half computed under the *shared* quant assumption
"""

import os
import numpy as np


def gen_golden_data():
    B = 128
    M = 32
    N = 32
    K = 32
    L0_BATCH_SIZE = 4  # mirror the kernel template arg; only used here for the printed prediction

    # A: [B, M, K] int8
    input_x = np.random.uniform(-2, 2, [B, M, K]).astype(np.int8)
    # B: [B, K, N] int8
    input_y = np.random.uniform(-2, 2, [B, K, N]).astype(np.int8)

    # Step 1: shared quant slice [1, 1, N], broadcast to all batches when computing golden.
    shared_quant = np.random.uniform(-2, 2, [1, 1, N]).astype(np.float32)
    shared_quant_broadcast = np.broadcast_to(shared_quant, [B, 1, N]).copy()

    # Per-batch matmul on int32 to avoid int8 accumulator overflow
    matmul_int32 = np.matmul(input_x.astype(np.int32), input_y.astype(np.int32))
    # Golden uses the SHARED quant for every batch.
    golden = (matmul_int32.astype(np.float32) * shared_quant_broadcast).astype(np.half)

    # Step 2: build the on-disk quant tensor. batch 0 keeps the shared quant; batch 1..B-1 are
    # replaced with fresh random values so the kernel actually reads different per-batch quants
    # from L1/FB. Encoding is the standard fixpipe (clear low 13 bits + set mode bit 46).
    perturbed_quant = np.random.uniform(-2, 2, [B, 1, N]).astype(np.float32)
    perturbed_quant[0:1] = shared_quant  # keep batch 0 identical to golden's quant

    uint32_quant = np.frombuffer(perturbed_quant.tobytes(), np.uint32).reshape(B, 1, N).copy()
    uint32_quant &= 0xFFFFE000
    quant_tensor = uint32_quant.astype(np.uint64)
    quant_tensor |= np.uint64(1) << np.uint64(46)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    np.ascontiguousarray(input_y.transpose(0, 2, 1)).tofile("./input/input_y.bin")
    quant_tensor.tofile("./input/quant.bin")
    golden.tofile("./output/golden.bin")

    print(f"Generated shared-quant experiment data: B={B}, M={M}, N={N}, K={K}")
    print(f"  input_x.bin: {input_x.nbytes} bytes ({input_x.shape}, int8)")
    print(f"  input_y.bin: {input_y.nbytes} bytes ([B, N, K], int8 transposed)")
    print(f"  quant.bin:   {quant_tensor.nbytes} bytes ({quant_tensor.shape}, uint64)")
    print(f"               batch 0 = shared (matches golden); batch 1..{B-1} = perturbed")
    print(f"  golden.bin:  {golden.nbytes} bytes ({golden.shape}, half) computed under shared quant")
    print(f"")
    print(f"Prediction (assuming one fixpipe instruction binds to one FB quant slice):")
    print(f"  L0BatchSize = {L0_BATCH_SIZE}")
    print(f"  -> batches [0..{L0_BATCH_SIZE - 1}] should match golden (FB reads batch-0 quant)")
    print(f"  -> batches [{L0_BATCH_SIZE}..{B - 1}] should diverge from golden")


if __name__ == "__main__":
    gen_golden_data()



if __name__ == "__main__":
    gen_golden_data()
