#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. Please do not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import numpy as np


MATRIX_H = 8192
MATRIX_W = 8192
TILE = 256
TILES_PER_AXIS = MATRIX_H // TILE
C0_ELEMS = 16
RANDOM_SEED = 2026
RANDOM_LOW = -128
RANDOM_HIGH = 128


def build_golden(input_x, dst_nz_c0_stride):
    tile_out_c0_blocks = (TILE - 1) * 1 + (TILE // C0_ELEMS - 1) * dst_nz_c0_stride + 1
    tile_out_elements = tile_out_c0_blocks * C0_ELEMS
    total_tiles = TILES_PER_AXIS * TILES_PER_AXIS
    output = np.zeros(total_tiles * tile_out_elements, dtype=np.float16)

    for tile_id in range(total_tiles):
        tile_row = tile_id // TILES_PER_AXIS
        tile_col = tile_id % TILES_PER_AXIS
        src = input_x[
            tile_row * TILE:(tile_row + 1) * TILE,
            tile_col * TILE:(tile_col + 1) * TILE,
        ]
        tile_out = output[tile_id * tile_out_elements:(tile_id + 1) * tile_out_elements]
        for row_idx in range(TILE):
            row = src[row_idx]
            for block_idx in range(TILE // C0_ELEMS):
                dst_block_idx = row_idx + block_idx * dst_nz_c0_stride
                dst_start = dst_block_idx * C0_ELEMS
                src_start = block_idx * C0_ELEMS
                tile_out[dst_start:dst_start + C0_ELEMS] = row[src_start:src_start + C0_ELEMS]

    return output


def main():
    rng = np.random.default_rng(RANDOM_SEED)
    input_x = rng.integers(
        RANDOM_LOW,
        RANDOM_HIGH + 1,
        size=(MATRIX_H, MATRIX_W),
        dtype=np.int16,
    ).astype(np.float16)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    input_x.tofile("./input/input.bin")
    build_golden(input_x, 256).tofile("./output/golden_s1.bin")
    build_golden(input_x, 257).tofile("./output/golden_s2.bin")
    print(f"generate random input: seed={RANDOM_SEED}, range=[{RANDOM_LOW}, {RANDOM_HIGH}]")
    print(f"input sample: {input_x.reshape(-1)[:16]}")


if __name__ == "__main__":
    main()
