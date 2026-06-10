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

import os
import numpy as np

BATCH_SIZE = 2
M_SIZE = 64
N_SIZE = 64
K_SIZE = 128
SCALE_CEIL_NUMBER = 32
SCALE_ALIGN_NUMBER = 2
FP4_PACKED_FACTOR = 2
FP4_E1M2_VALUES = np.array(
    [0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
     -0.0, -0.25, -0.5, -0.75, -1.0, -1.25, -1.5, -1.75],
    dtype=np.float64,
)


def align_up(value, align):
    return (value + align - 1) // align * align


def pack_two_fp4(tensor):
    tensor_bin = tensor.reshape(-1)
    tensor_high = tensor_bin[::2].view(np.uint8)
    tensor_low = tensor_bin[1::2].view(np.uint8)
    low_bits = (tensor_low & 0x0F) << 4
    high_bits = tensor_high & 0x0F
    combined = low_bits | high_bits
    return combined.reshape(*tensor.shape[:-1], tensor.shape[-1] // FP4_PACKED_FACTOR)


def gen_golden_data():
    scale_k = align_up((K_SIZE + SCALE_CEIL_NUMBER - 1) // SCALE_CEIL_NUMBER, SCALE_ALIGN_NUMBER)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(2026)
    x1_codes = rng.integers(0, 16, [BATCH_SIZE, M_SIZE, K_SIZE], dtype=np.uint8)
    x2_codes = rng.integers(0, 16, [BATCH_SIZE, K_SIZE, N_SIZE], dtype=np.uint8)
    x1_scale_gm = rng.integers(127, 130, [BATCH_SIZE, M_SIZE, scale_k], dtype=np.uint8)
    x2_scale_gm = rng.integers(127, 130, [BATCH_SIZE, scale_k, N_SIZE], dtype=np.uint8)

    x1_mx = 2 ** (x1_scale_gm.astype(np.float64) - 127)
    x2_mx = 2 ** (x2_scale_gm.astype(np.float64) - 127)
    x1_values = FP4_E1M2_VALUES[x1_codes]
    x2_values = FP4_E1M2_VALUES[x2_codes]
    x1_full = np.zeros([BATCH_SIZE, M_SIZE, K_SIZE], dtype=np.float64)
    x2_full = np.zeros([BATCH_SIZE, K_SIZE, N_SIZE], dtype=np.float64)

    for k_idx in range(K_SIZE):
        x1_full[:, :, k_idx] = x1_values[:, :, k_idx] * x1_mx[:, :, k_idx // SCALE_CEIL_NUMBER]
        x2_full[:, k_idx, :] = x2_values[:, k_idx, :] * x2_mx[:, k_idx // SCALE_CEIL_NUMBER, :]

    golden = np.matmul(x1_full, x2_full).astype(np.float32)

    x1_gm_packed = pack_two_fp4(x1_codes)
    x2_gm_packed = pack_two_fp4(x2_codes.transpose(0, 2, 1))
    x2_scale_gm = x2_scale_gm.transpose(0, 2, 1)

    x1_gm_packed.tofile("./input/x1_gm.bin")
    x2_gm_packed.tofile("./input/x2_gm.bin")
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
