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
M_SIZE = 32
N_SIZE = 48
K_SIZE = 96
SCALE_CEIL_NUMBER = 32
SCALE_ALIGN_NUMBER = 2
K_ALIGN_NUMBER = 64
FRACTAL_FIXED = 16
C0_ELEMENT_SCALE = 2
FP4_PACKED_FACTOR = 2
# Three scenes pair each scale pattern once: (ScaleAND, ScaleBND) / (ScaleADN, ScaleBDN) / (ZZ, NN).
SCENE_NUMBER = 3
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


# Lay a logical [B, rows, cols] scale tensor into GM bytes following each pattern's stride.
# All patterns carry the same logical values; only the byte placement differs.
def layout_scale_and(scale):  # ScaleAND: row-major, addr = m*scaleK + k
    return scale.reshape(BATCH_SIZE, -1)


def layout_scale_adn(scale):  # ScaleADN: addr = (k//2)*(2*rows) + m*2 + (k%2)
    b, rows, cols = scale.shape
    out = np.zeros((b, rows * cols), dtype=scale.dtype)
    for m in range(rows):
        for k in range(cols):
            out[:, (k // 2) * (2 * rows) + m * 2 + (k % 2)] = scale[:, m, k]
    return out


def layout_scale_bnd(scale):  # ScaleBND: addr = (k//2)*(2*cols) + n*2 + (k%2), scale is [B, scaleK, N]
    b, rows, cols = scale.shape  # rows=scaleK, cols=N
    out = np.zeros((b, rows * cols), dtype=scale.dtype)
    for k in range(rows):
        for n in range(cols):
            out[:, (k // 2) * (2 * cols) + n * 2 + (k % 2)] = scale[:, k, n]
    return out


def layout_scale_bdn(scale):  # ScaleBDN: col-major, addr = n*scaleK + k, scale is [B, scaleK, N]
    return scale.transpose(0, 2, 1).reshape(BATCH_SIZE, -1)


def layout_fractal_zz(scale):  # ZZ: addr = (m//16)*(16*alignCol) + (m%16)*c0 + (k//c0)*(16*c0) + (k%c0)
    b, rows, cols = scale.shape
    align_col = align_up(cols, C0_ELEMENT_SCALE)
    out = np.zeros((b, align_up(rows, FRACTAL_FIXED) * align_col), dtype=scale.dtype)
    for m in range(rows):
        for k in range(cols):
            addr = ((m // FRACTAL_FIXED) * (FRACTAL_FIXED * align_col)
                    + (m % FRACTAL_FIXED) * C0_ELEMENT_SCALE
                    + (k // C0_ELEMENT_SCALE) * (FRACTAL_FIXED * C0_ELEMENT_SCALE)
                    + (k % C0_ELEMENT_SCALE))
            out[:, addr] = scale[:, m, k]
    return out


def layout_fractal_nn(scale):  # NN: addr = (n//16)*(alignRow*16) + (n%16)*c0 + (k//c0)*(16*c0) + (k%c0)
    b, rows, cols = scale.shape  # rows=scaleK, cols=N
    align_row = align_up(rows, C0_ELEMENT_SCALE)
    out = np.zeros((b, align_row * align_up(cols, FRACTAL_FIXED)), dtype=scale.dtype)
    for k in range(rows):
        for n in range(cols):
            addr = ((n // FRACTAL_FIXED) * (align_row * FRACTAL_FIXED)
                    + (n % FRACTAL_FIXED) * C0_ELEMENT_SCALE
                    + (k // C0_ELEMENT_SCALE) * (FRACTAL_FIXED * C0_ELEMENT_SCALE)
                    + (k % C0_ELEMENT_SCALE))
            out[:, addr] = scale[:, k, n]
    return out


def gen_golden_data():
    # MX Mmad needs K aligned to 64. Pad matrices/scale along K to k_align; the padded K-tail uses
    # fp4 code 0 (== value 0.0) so it contributes nothing to the matmul, and scale_k follows k_align.
    k_align = align_up(K_SIZE, K_ALIGN_NUMBER)
    scale_k = align_up((k_align + SCALE_CEIL_NUMBER - 1) // SCALE_CEIL_NUMBER, SCALE_ALIGN_NUMBER)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(2026)
    x1_codes = np.zeros([BATCH_SIZE, M_SIZE, k_align], dtype=np.uint8)
    x2_codes = np.zeros([BATCH_SIZE, k_align, N_SIZE], dtype=np.uint8)
    x1_codes[:, :, :K_SIZE] = rng.integers(0, 16, [BATCH_SIZE, M_SIZE, K_SIZE], dtype=np.uint8)
    x2_codes[:, :K_SIZE, :] = rng.integers(0, 16, [BATCH_SIZE, K_SIZE, N_SIZE], dtype=np.uint8)
    x1_scale_gm = rng.integers(127, 130, [BATCH_SIZE, M_SIZE, scale_k], dtype=np.uint8)
    x2_scale_gm = rng.integers(127, 130, [BATCH_SIZE, scale_k, N_SIZE], dtype=np.uint8)

    x1_mx = 2 ** (x1_scale_gm.astype(np.float64) - 127)
    x2_mx = 2 ** (x2_scale_gm.astype(np.float64) - 127)
    x1_values = FP4_E1M2_VALUES[x1_codes]
    x2_values = FP4_E1M2_VALUES[x2_codes]
    x1_full = np.zeros([BATCH_SIZE, M_SIZE, k_align], dtype=np.float64)
    x2_full = np.zeros([BATCH_SIZE, k_align, N_SIZE], dtype=np.float64)

    for k_idx in range(k_align):
        x1_full[:, :, k_idx] = x1_values[:, :, k_idx] * x1_mx[:, :, k_idx // SCALE_CEIL_NUMBER]
        x2_full[:, k_idx, :] = x2_values[:, k_idx, :] * x2_mx[:, k_idx // SCALE_CEIL_NUMBER, :]

    golden = np.matmul(x1_full, x2_full).astype(np.float32)
    # Each scene is built and run separately; every scene produces this same logical result.

    x1_gm_packed = pack_two_fp4(x1_codes)
    x2_gm_packed = pack_two_fp4(x2_codes.transpose(0, 2, 1))

    x1_gm_packed.tofile("./input/x1_gm.bin")
    x2_gm_packed.tofile("./input/x2_gm.bin")

    # ScaleA logical [B, M, scaleK] laid out in three GM patterns.
    layout_scale_and(x1_scale_gm).astype(np.uint8).tofile("./input/x1_scale_AND.bin")
    layout_scale_adn(x1_scale_gm).astype(np.uint8).tofile("./input/x1_scale_ADN.bin")
    layout_fractal_zz(x1_scale_gm).astype(np.uint8).tofile("./input/x1_scale_ZZ.bin")

    # ScaleB logical [B, scaleK, N] laid out in three GM patterns.
    layout_scale_bnd(x2_scale_gm).astype(np.uint8).tofile("./input/x2_scale_BND.bin")
    layout_scale_bdn(x2_scale_gm).astype(np.uint8).tofile("./input/x2_scale_BDN.bin")
    layout_fractal_nn(x2_scale_gm).astype(np.uint8).tofile("./input/x2_scale_NN.bin")

    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
