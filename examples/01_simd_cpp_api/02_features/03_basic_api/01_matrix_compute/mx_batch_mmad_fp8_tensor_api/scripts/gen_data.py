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
import ml_dtypes

BATCH_SIZE = 2
M_SIZE = 64
N_SIZE = 64
K_SIZE = 128
SCALE_CEIL_NUMBER = 32
SCALE_ALIGN_NUMBER = 2
FRACTAL_FIXED = 16
C0_ELEMENT_SCALE = 2
# Three scenes pair each scale pattern once: (ScaleAND, ScaleBND) / (ScaleADN, ScaleBDN) / (ZZ, NN).
SCENE_NUMBER = 3
# fp8 matrices use the e4m3fn format (one byte per element, no packing).
FP8_DTYPE = ml_dtypes.float8_e4m3fn


def align_up(value, align):
    return (value + align - 1) // align * align


def to_fp8_bytes(values):
    # Round float values to the nearest fp8 e4m3fn, then reinterpret as raw bytes.
    return values.astype(FP8_DTYPE).view(np.uint8)


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
    scale_k = align_up((K_SIZE + SCALE_CEIL_NUMBER - 1) // SCALE_CEIL_NUMBER, SCALE_ALIGN_NUMBER)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    rng = np.random.default_rng(2026)
    # fp8 matrices: draw float values, then round to the nearest e4m3fn so the golden uses
    # exactly the values the hardware sees. Keep the magnitude small to stay well within e4m3 range.
    x1_raw = rng.uniform(-2.0, 2.0, [BATCH_SIZE, M_SIZE, K_SIZE]).astype(FP8_DTYPE)
    x2_raw = rng.uniform(-2.0, 2.0, [BATCH_SIZE, K_SIZE, N_SIZE]).astype(FP8_DTYPE)
    x1_scale_gm = rng.integers(127, 130, [BATCH_SIZE, M_SIZE, scale_k], dtype=np.uint8)
    x2_scale_gm = rng.integers(127, 130, [BATCH_SIZE, scale_k, N_SIZE], dtype=np.uint8)

    x1_mx = 2 ** (x1_scale_gm.astype(np.float64) - 127)
    x2_mx = 2 ** (x2_scale_gm.astype(np.float64) - 127)
    x1_values = x1_raw.astype(np.float64)
    x2_values = x2_raw.astype(np.float64)
    x1_full = np.zeros([BATCH_SIZE, M_SIZE, K_SIZE], dtype=np.float64)
    x2_full = np.zeros([BATCH_SIZE, K_SIZE, N_SIZE], dtype=np.float64)

    for k_idx in range(K_SIZE):
        x1_full[:, :, k_idx] = x1_values[:, :, k_idx] * x1_mx[:, :, k_idx // SCALE_CEIL_NUMBER]
        x2_full[:, k_idx, :] = x2_values[:, k_idx, :] * x2_mx[:, k_idx // SCALE_CEIL_NUMBER, :]

    golden = np.matmul(x1_full, x2_full).astype(np.float32)
    # Each scene is built and run separately; every scene produces this same logical result.

    # A GM ND: row-major [B, M, K]. B GM DN: transposed to [B, N, K] row-major bytes.
    x1_raw.view(np.uint8).tofile("./input/x1_gm.bin")
    np.ascontiguousarray(x2_raw.transpose(0, 2, 1)).view(np.uint8).tofile("./input/x2_gm.bin")

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
