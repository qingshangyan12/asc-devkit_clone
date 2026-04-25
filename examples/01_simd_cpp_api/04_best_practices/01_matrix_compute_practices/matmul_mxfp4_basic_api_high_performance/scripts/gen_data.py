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

# fp4_e1m2 codebook (positive branch only, code 0..7)
FP4_E1M2_POS_LUT = np.array([0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75], dtype=np.float64)


def pack_two_fp4_codes(code_matrix):
    rows, cols = code_matrix.shape
    if cols % 2 != 0:
        raise ValueError("K dimension must be even for fp4x2 pack")
    high = code_matrix[:, 0::2] & 0x0F
    low = (code_matrix[:, 1::2] & 0x0F) << 4
    return (low | high).astype(np.uint8)


def gen_golden_data(m=8192, n=8192, k=8192, seed=1234, is_bias=False):
    if k % 2 != 0:
        raise ValueError("k must be even for fp4x2 pack")

    rng = np.random.default_rng(seed)
    sk = (int)(np.ceil(k / 64) * 2)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x1_code = rng.integers(0, 8, size=(m, k), dtype=np.uint8)
    x2_code = rng.integers(0, 8, size=(k, n), dtype=np.uint8)

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
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
    
