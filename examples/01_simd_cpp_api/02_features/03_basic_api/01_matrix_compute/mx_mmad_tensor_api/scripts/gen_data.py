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

M_SIZE = 128
N_SIZE = 128
K_SIZE = 256
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


def pack_two_fp4(matrix):
    matrix_row = matrix.shape[0]
    matrix_col = matrix.shape[1]
    matrix_bin = matrix.flatten()
    matrix_high = matrix_bin[::2].view(np.uint8)
    matrix_low = matrix_bin[1::2].view(np.uint8)
    low_bits = (matrix_low & 0x0F) << 4
    high_bits = matrix_high & 0x0F
    combined = low_bits | high_bits
    return combined.reshape(matrix_row, matrix_col // FP4_PACKED_FACTOR)


def gen_golden_data():
    scale_k = align_up((K_SIZE + SCALE_CEIL_NUMBER - 1) // SCALE_CEIL_NUMBER, SCALE_ALIGN_NUMBER)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(2026)
    x1_codes = rng.integers(0, 16, [M_SIZE, K_SIZE], dtype=np.uint8)
    x2_codes = rng.integers(0, 16, [K_SIZE, N_SIZE], dtype=np.uint8)
    x1_scale_gm = rng.integers(127, 130, [M_SIZE, scale_k], dtype=np.uint8)
    x2_scale_gm = rng.integers(127, 130, [scale_k, N_SIZE], dtype=np.uint8)

    x1_mx = 2 ** (x1_scale_gm.astype(np.float64) - 127)
    x2_mx = 2 ** (x2_scale_gm.astype(np.float64) - 127)
    x1_values = FP4_E1M2_VALUES[x1_codes]
    x2_values = FP4_E1M2_VALUES[x2_codes]
    x1_full = np.zeros([M_SIZE, K_SIZE], dtype=np.float64)
    x2_full = np.zeros([K_SIZE, N_SIZE], dtype=np.float64)

    for k_idx in range(K_SIZE):
        x1_full[:, k_idx] = x1_values[:, k_idx] * x1_mx[:, k_idx // SCALE_CEIL_NUMBER]
        x2_full[k_idx, :] = x2_values[k_idx, :] * x2_mx[k_idx // SCALE_CEIL_NUMBER, :]

    golden = np.matmul(x1_full, x2_full).astype(np.float32)

    x1_gm_packed = pack_two_fp4(x1_codes)
    x2_gm_packed = pack_two_fp4(x2_codes.transpose())
    x2_scale_gm = x2_scale_gm.transpose()

    x1_gm_packed.tofile("./input/x1_gm.bin")
    x2_gm_packed.tofile("./input/x2_gm.bin")
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
