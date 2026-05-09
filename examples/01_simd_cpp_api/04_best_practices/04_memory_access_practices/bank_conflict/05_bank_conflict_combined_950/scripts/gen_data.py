#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import numpy as np

MATRIX_H        = 8192
MATRIX_W        = 8192
TILE_H          = 128
TILE_W          = 256
TILE_ROWS       = MATRIX_H // TILE_H    # = 64
TILE_COLS       = MATRIX_W // TILE_W    # = 32
TOTAL_TILES     = TILE_ROWS * TILE_COLS # = 2048
C0_ELEMS        = 16
TILE_C0_COLS    = TILE_W // C0_ELEMS    # = 16
DST_NZ_N_STRIDE = 1
RANDOM_SEED     = 2026
RANDOM_LOW      = -128
RANDOM_HIGH     = 128


def build_golden(x, dst_nz_c0_stride):
    """ND → NZ 格式转换（含分 tile）。stride=129 时列间 1 DataBlock 空洞填 0。"""
    tile_out_c0_blocks = (TILE_H - 1) * DST_NZ_N_STRIDE + (TILE_C0_COLS - 1) * dst_nz_c0_stride + 1
    tile_out_elems     = tile_out_c0_blocks * C0_ELEMS
    out = np.zeros(TOTAL_TILES * tile_out_elems, dtype=np.float16)

    for tile_id in range(TOTAL_TILES):
        tile_row = tile_id // TILE_COLS
        tile_col = tile_id % TILE_COLS
        src      = x[tile_row * TILE_H:(tile_row + 1) * TILE_H,
                     tile_col * TILE_W:(tile_col + 1) * TILE_W]
        tile_out = out[tile_id * tile_out_elems:(tile_id + 1) * tile_out_elems]

        for n in range(TILE_H):
            for c in range(TILE_C0_COLS):
                dst_c0_block = n * DST_NZ_N_STRIDE + c * dst_nz_c0_stride
                tile_out[dst_c0_block * C0_ELEMS : dst_c0_block * C0_ELEMS + C0_ELEMS] = \
                    src[n, c * C0_ELEMS : (c + 1) * C0_ELEMS]
    return out


def main():
    rng = np.random.default_rng(RANDOM_SEED)
    x = rng.integers(RANDOM_LOW, RANDOM_HIGH + 1,
                     size=(MATRIX_H, MATRIX_W), dtype=np.int16).astype(np.float16)

    os.makedirs("input",  exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x.tofile("./input/input.bin")
    # S1 用 stride=128，S2/S3 用 stride=129（VEC_PADDING 不影响输出，S2/S3 golden 相同）
    build_golden(x, 128).tofile("./output/golden_s1.bin")
    build_golden(x, 129).tofile("./output/golden_s2.bin")
    build_golden(x, 129).tofile("./output/golden_s3.bin")

    print(f"generate random input: seed={RANDOM_SEED}, range=[{RANDOM_LOW}, {RANDOM_HIGH}]")
    print(f"input sample: {x.reshape(-1)[:16]}")


if __name__ == "__main__":
    main()
