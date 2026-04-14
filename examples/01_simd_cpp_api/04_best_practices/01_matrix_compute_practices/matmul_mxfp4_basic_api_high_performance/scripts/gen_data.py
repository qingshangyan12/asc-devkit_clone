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

def convert_x1_scale_format(x1_mx_gm, block_size=16, c0_size_mx=2):
    m, k = x1_mx_gm.shape
    pad_m = (block_size - m % block_size) % block_size
    pad_k = (c0_size_mx - k % c0_size_mx) % c0_size_mx
    
    if pad_m > 0 or pad_k > 0:
        padded = np.pad(x1_mx_gm, 
                       ((0, pad_m), (0, pad_k)), 
                       mode='constant',
                       constant_values=0)
    else:
        padded = x1_mx_gm
    
    m_padded = m + pad_m
    k_padded = k + pad_k

    x1_scale_gm = padded.reshape((int(m_padded / block_size), block_size, 
                                 int(k_padded / c0_size_mx), c0_size_mx))
    x1_scale_gm = x1_scale_gm.transpose(0, 2, 1, 3)
    x1_scale_gm = x1_scale_gm.reshape(x1_scale_gm.shape[0] * x1_scale_gm.shape[1], 
                                     x1_scale_gm.shape[2] * x1_scale_gm.shape[3])

    return x1_scale_gm


def convert_x2_scale_format(x2_mx_gm, block_size=16, c0_size_mx=2):
    k, n = x2_mx_gm.shape
    pad_n = (block_size - n % block_size) % block_size
    pad_k = (c0_size_mx - k % c0_size_mx) % c0_size_mx
    
    if pad_n > 0 or pad_k > 0:
        padded = np.pad(x2_mx_gm, 
                       ((0, pad_k), (0, pad_n)),
                       mode='constant',
                       constant_values=0)
    else:
        padded = x2_mx_gm
    
    k_padded, n_padded = padded.shape
    
    x2_scale_gm = padded.reshape((int(k_padded / c0_size_mx), c0_size_mx, int(n_padded / 16), 16)).transpose(2, 0, 3, 1)
    x2_scale_gm = x2_scale_gm.reshape(x2_scale_gm.shape[1] * x2_scale_gm.shape[3], 
                                      x2_scale_gm.shape[0] * x2_scale_gm.shape[2])

    return x2_scale_gm

# fp4_e1m2 codebook (positive branch only, code 0..7)
FP4_E1M2_POS_LUT = np.array([0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75], dtype=np.float64)


def pack_two_fp4_codes(code_matrix):
    rows, cols = code_matrix.shape
    if cols % 2 != 0:
        raise ValueError("K dimension must be even for fp4x2 pack")
    high = code_matrix[:, 0::2] & 0x0F
    low = (code_matrix[:, 1::2] & 0x0F) << 4
    return (low | high).astype(np.uint8)


def make_scale_tile_pattern(rows, cols, tile_h=16, tile_w=2, base=126, cycle=8):
    """
    Generate uint8 scale codes in tiled-constant layout:
      - each [tile_h x tile_w] tile has one constant value
      - next tile uses next code (cyclic), good for dump visibility
    """
    out = np.empty((rows, cols), dtype=np.uint8)
    tile_idx = 0
    for r0 in range(0, rows, tile_h):
        rh = min(tile_h, rows - r0)
        for c0 in range(0, cols, tile_w):
            cw = min(tile_w, cols - c0)
            code = np.uint8(base + (tile_idx % cycle))
            out[r0:r0 + rh, c0:c0 + cw] = code
            tile_idx += 1
    return out


def make_scale_k_only_pattern(rows, cols, tile_w=1, base=126, cycle=8):
    """
    Only varies along K(scaleK) axis:
      all rows share same code at same column.
    """
    out = np.empty((rows, cols), dtype=np.uint8)
    for c0 in range(0, cols, tile_w):
        cw = min(tile_w, cols - c0)
        code = np.uint8(base + ((c0 // tile_w) % cycle))
        out[:, c0:c0 + cw] = code
    return out


def make_scale_m_only_pattern(rows, cols, tile_h=1, base=126, cycle=8):
    """
    Only varies along M/N(row) axis:
      each row has one constant code across all scaleK columns.
    """
    out = np.empty((rows, cols), dtype=np.uint8)
    for r0 in range(0, rows, tile_h):
        rh = min(tile_h, rows - r0)
        code = np.uint8(base + ((r0 // tile_h) % cycle))
        out[r0:r0 + rh, :] = code
    return out


def build_scale_matrix(rows, cols, mode, base=126, cycle=8, tile_h=16, tile_w=2):
    if mode == "const":
        return np.full((rows, cols), base, dtype=np.uint8)
    if mode == "tile16x2":
        return make_scale_tile_pattern(rows, cols, tile_h=tile_h, tile_w=tile_w, base=base, cycle=cycle)
    if mode == "k_only":
        return make_scale_k_only_pattern(rows, cols, tile_w=1, base=base, cycle=cycle)
    if mode == "k_only_pair2":
        # K direction varies per 2 scale elements (b16 pair friendly)
        return make_scale_k_only_pattern(rows, cols, tile_w=2, base=base, cycle=cycle)
    if mode == "m_only":
        return make_scale_m_only_pattern(rows, cols, tile_h=1, base=base, cycle=cycle)
    if mode == "row0_k_only":
        out = np.full((rows, cols), 127, dtype=np.uint8)
        out[0:1, :] = make_scale_k_only_pattern(1, cols, tile_w=1, base=base, cycle=cycle)
        return out
    raise ValueError(f"unsupported scale mode: {mode}")


def gen_golden_data(m=256, n=256, k=256, seed=1234, is_bias=False):
    if k % 2 != 0:
        raise ValueError("k must be even for fp4x2 pack")

    rng = np.random.default_rng(seed)
    sk = (int)(np.ceil(k / 64) * 2)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # A: random fp4 code from positive e1m2 set [0..7]
    x1_code = rng.integers(0, 8, size=(m, k), dtype=np.uint8)
    # B: fixed to 1.0 (code=4) to isolate A-path debugging
    # x2_code = np.full((k, n), 4, dtype=np.uint8)
    x2_code = rng.integers(0, 8, size=(k, n), dtype=np.uint8)

    # Keep scales as 1.0 to isolate matrix path debugging.
    # x1_scale_gm = np.full((m, sk), 127, dtype=np.uint8)
    # x2_scale_gm = np.full((sk, n), 127, dtype=np.uint8)
    # x1_gm = np.random.randint(-1, 2, [m, k]).astype(fp4_e1m2x2)
    # x2_gm = np.random.randint(-1, 2, [k, n]).astype(fp4_e1m2x2)

    # Scale debug patterns:
    # - const   : all same code
    # - tile16x2: each 16x2 tile constant, tile-by-tile cycle
    # - k_only  : only K(scaleK) axis varies (per 1 scale element)
    # - k_only_pair2 : only K varies (per 2 scale elements)
    # - m_only  : only row axis varies
    # - row0_k_only: only row0 varies along K, other rows are all 1.0(127)
    # scale_a_mode = "row0_k_only"
    # scale_b_mode = "const"
    # x1_scale_gm = build_scale_matrix(m, sk, mode=scale_a_mode, base=126, cycle=8, tile_h=16, tile_w=2)
    # x2_scale_gm = build_scale_matrix(sk, n, mode=scale_b_mode, base=127, cycle=8, tile_h=16, tile_w=2)

    x1_scale_gm = np.random.randint(127, 130, [m, sk]).astype(np.uint8)
    x2_scale_gm = np.random.randint(127, 130, [sk, n]).astype(np.uint8)


    ###################### compute ########################
    x1_mx = 2**(x1_scale_gm.astype(np.float64) - 127)
    x2_mx = 2**(x2_scale_gm.astype(np.float64) - 127)
    x1_full = np.zeros([m, k], dtype=np.float64)
    x2_full = np.zeros([k, n], dtype=np.float64)

    for i in range(k):
        x1_full[:, i] = FP4_E1M2_POS_LUT[x1_code[:, i]] * x1_mx[:, i // 32]
        x2_full[i, :] = FP4_E1M2_POS_LUT[x2_code[i, :]] * x2_mx[i // 32, :]
    golden = np.matmul(x1_full.astype(np.float64), x2_full.astype(np.float64))
    golden = golden.astype(ml_dtypes.bfloat16)

    x2_code_t = x2_code.transpose()
    x2_scale_gm = x2_scale_gm.transpose()
    x1_gm_packed = pack_two_fp4_codes(x1_code)
    x2_gm_packed = pack_two_fp4_codes(x2_code_t)
    x1_gm_packed.tofile("./input/x1_gm.bin")
    x2_gm_packed.tofile("./input/x2_gm.bin")
    x1_scale_gm = convert_x1_scale_format(x1_scale_gm, 16, 2)  # to zz
    x2_scale_gm = convert_x2_scale_format(x2_scale_gm, 16, 2)  # to nn
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")
    # print(f"Generated fp4 by codebook (no en_dtypes): m={m}, n={n}, k={k}, seed={seed}, scaleK={sk}")
    # print(f"Scale mode: A={scale_a_mode}, B={scale_b_mode}")


if __name__ == "__main__":
    gen_golden_data()
    
