#!/usr/bin/env python
# -*- coding: UTF-8 -*-
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

try:
    import en_dtypes
    HIFP4_DTYPE = en_dtypes.float4_e1m2
except ImportError:
    HIFP4_DTYPE = np.uint8

M = 128
N = 128
K = 256

SCALE_CEIL_NUMBER = 64
SCALE_ALIGN_NUMBER = 2
FP4_PACKED_FACTOR = 2


def align_up(value, align):
    return (value + align - 1) // align * align


def scale_k_size(k):
    return align_up((k + SCALE_CEIL_NUMBER - 1) // SCALE_CEIL_NUMBER, SCALE_ALIGN_NUMBER)


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


def e6m2_to_float(e6m2_value):
    exponent = (e6m2_value >> 2) & 0x3F
    mantissa = e6m2_value & 0x03
    if exponent == 0 and mantissa == 0:
        return 0.0
    return (1 + mantissa / 4.0) * (2 ** (exponent - 48))


# scale [m, k/64] (uint32 hif4_scale word) -> [m, k] fp32. Layout of one 32-bit word:
#   bits[ 7: 0] = E6M2 ; bits[15: 8] = E1_8 ; bits[31:16] = E1_16
# scale_factor[i] = E6M2 * 2^(E1_8[i // 8] + E1_16[i // 4])
def repack_hif4_scale(scale):
    original_shape = scale.shape
    new_shape = list(original_shape[:-1]) + [original_shape[-1] * SCALE_CEIL_NUMBER]
    result = np.zeros(new_shape, dtype=np.float32)

    for idx in np.ndindex(original_shape):
        scale_value = int(scale[idx])
        e6m2_byte = scale_value & 0xFF
        e1_8_byte = (scale_value >> 8) & 0xFF
        e1_16_word = (scale_value >> 16) & 0xFFFF

        e6m2_value = e6m2_to_float(e6m2_byte)
        for k in range(SCALE_CEIL_NUMBER):
            secd_bit = (e1_8_byte >> (k // 8)) & 0x1
            third_bit = (e1_16_word >> (k // 4)) & 0x1
            new_idx = list(idx)
            new_idx[-1] = idx[-1] * SCALE_CEIL_NUMBER + k
            result[tuple(new_idx)] = np.float32(e6m2_value * (2 ** (secd_bit + third_bit)))
    return result


def gen_golden_data():
    np.random.seed(19)
    rng = np.random.default_rng(2026)

    scale_k = scale_k_size(K)

    x1_gm = rng.uniform(-1, 2, [M, K]).astype(HIFP4_DTYPE)
    x2_gm = rng.uniform(-1, 2, [K, N]).astype(HIFP4_DTYPE)

    e6m2_a = rng.integers(196, 202, size=(M, scale_k), dtype=np.uint32)
    e6m2_b = rng.integers(196, 202, size=(scale_k, N), dtype=np.uint32)
    micro_a = rng.integers(0, 0xFFFFFF + 1, size=(M, scale_k), dtype=np.uint32)
    micro_b = rng.integers(0, 0xFFFFFF + 1, size=(scale_k, N), dtype=np.uint32)
    x1_scale_word = (e6m2_a | (micro_a << 8)).astype(np.uint32)
    x2_scale_word = (e6m2_b | (micro_b << 8)).astype(np.uint32)

    x1_scale_full = repack_hif4_scale(x1_scale_word).astype(np.float64)
    x2_scale_full = repack_hif4_scale(x2_scale_word.transpose()).transpose().astype(np.float64)

    x1_full = x1_gm.astype(np.float64) * x1_scale_full[:, :K]
    x2_full = x2_gm.astype(np.float64) * x2_scale_full[:K, :]
    golden = np.matmul(x1_full, x2_full).astype(np.float32)

    # A 矩阵以 [M, K] (ND) 存储；B 矩阵按 DN 布局存储，即在 GM 上保存 [N, K]。
    x1_bin = pack_two_fp4(x1_gm)
    x2_bin = pack_two_fp4(x2_gm.transpose())

    x2_scale_word_t = x2_scale_word.transpose()
    x1_scale_bytes = np.ascontiguousarray(x1_scale_word).astype('<u4').view(np.uint8)
    x2_scale_bytes = np.ascontiguousarray(x2_scale_word_t).astype('<u4').view(np.uint8)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x1_bin.tofile("./input/x1_gm.bin")
    x2_bin.tofile("./input/x2_gm.bin")
    x1_scale_bytes.tofile("./input/scaleA.bin")
    x2_scale_bytes.tofile("./input/scaleB.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
