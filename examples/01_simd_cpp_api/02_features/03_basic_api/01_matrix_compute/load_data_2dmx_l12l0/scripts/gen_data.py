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
import sys
import numpy as np

try:
    import ml_dtypes
    bfloat16 = ml_dtypes.bfloat16
    fp8_e4m3 = ml_dtypes.float8_e4m3fn
    fp8_e5m2 = ml_dtypes.float8_e5m2
except ImportError:
    bfloat16 = np.float16
    fp8_e4m3 = None
    fp8_e5m2 = None

try:
    import en_dtypes
    fp4_e1m2 = en_dtypes.float4_e1m2
    fp4_e2m1 = en_dtypes.float4_e2m1
except ImportError:
    fp4_e1m2 = None
    fp4_e2m1 = None


FP4_DECODE_TABLE = {
    "e1m2": np.array(
        [0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, -0.0, -0.25, -0.5, -0.75, -1.0, -1.25, -1.5, -1.75],
        dtype=np.float32,
    ),
    "e2m1": np.array(
        [0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, -0.0, -0.5, -1.0, -1.5, -2.0, -3.0, -4.0, -6.0],
        dtype=np.float32,
    ),
}


def build_fp8_decode_table(fp8_format):
    raw = np.arange(256, dtype=np.uint16)
    sign = np.where((raw & 0x80) != 0, -1.0, 1.0).astype(np.float32)

    if fp8_format == "e4m3fn":
        exponent = (raw >> 3) & 0x0F
        mantissa = raw & 0x07
        values = np.zeros(256, dtype=np.float32)

        subnormal_mask = exponent == 0
        normal_mask = (exponent != 0) & ~((exponent == 0x0F) & (mantissa == 0x07))
        nan_mask = (exponent == 0x0F) & (mantissa == 0x07)

        values[subnormal_mask] = sign[subnormal_mask] * mantissa[subnormal_mask].astype(np.float32) * (2.0 ** -9)
        values[normal_mask] = (
            sign[normal_mask]
            * np.exp2(exponent[normal_mask].astype(np.float32) - 7.0)
            * (1.0 + mantissa[normal_mask].astype(np.float32) / 8.0)
        )
        values[nan_mask] = np.nan
        return values

    if fp8_format == "e5m2":
        exponent = (raw >> 2) & 0x1F
        mantissa = raw & 0x03
        values = np.zeros(256, dtype=np.float32)

        subnormal_mask = exponent == 0
        normal_mask = (exponent != 0) & (exponent != 0x1F)
        inf_mask = (exponent == 0x1F) & (mantissa == 0)
        nan_mask = (exponent == 0x1F) & (mantissa != 0)

        values[subnormal_mask] = sign[subnormal_mask] * mantissa[subnormal_mask].astype(np.float32) * (2.0 ** -16)
        values[normal_mask] = (
            sign[normal_mask]
            * np.exp2(exponent[normal_mask].astype(np.float32) - 15.0)
            * (1.0 + mantissa[normal_mask].astype(np.float32) / 4.0)
        )
        values[inf_mask] = sign[inf_mask] * np.inf
        values[nan_mask] = np.nan
        return values

    raise ValueError(f"Invalid fp8 format {fp8_format}")


FP8_DECODE_TABLE = {
    "e4m3fn": build_fp8_decode_table("e4m3fn"),
    "e5m2": build_fp8_decode_table("e5m2"),
}


def cast_to_fp4(values, fp4_dtype, fp4_format):
    if fp4_dtype is not None:
        fp4_data = values.astype(fp4_dtype)
        return fp4_data.view(np.uint8), fp4_data.astype(np.float32)

    decode_table = FP4_DECODE_TABLE[fp4_format]
    values_float = values.astype(np.float32)
    distances = np.abs(values_float[..., None] - decode_table)
    fp4_raw = np.argmin(distances, axis=-1).astype(np.uint8)
    return fp4_raw, decode_table[fp4_raw]


def cast_to_fp8(values, fp8_dtype, fp8_format):
    if fp8_dtype is not None:
        fp8_data = values.astype(fp8_dtype)
        return fp8_data.view(np.uint8), fp8_data.astype(np.float32)

    decode_table = FP8_DECODE_TABLE[fp8_format]
    finite_mask = np.isfinite(decode_table)
    finite_raw = np.nonzero(finite_mask)[0].astype(np.uint8)
    finite_values = decode_table[finite_mask]
    values_float = values.astype(np.float32)
    distances = np.abs(values_float[..., None] - finite_values)
    fp8_raw = finite_raw[np.argmin(distances, axis=-1)]
    return fp8_raw, decode_table[fp8_raw]


def pack_fp4_to_fp4x2(fp4_data):
    row = fp4_data.shape[0]
    col = fp4_data.shape[1]
    fp4_flat = fp4_data.view(np.uint8).reshape(-1) & 0x0F
    low_bits = fp4_flat[::2]
    high_bits = fp4_flat[1::2] << 4
    combined = low_bits | high_bits
    packed = combined.reshape(row, col // 2)
    return packed


def mx_decompress(fp_data, scale_data, block_size=32):
    scale_exp = scale_data.astype(np.float32) - 127.0
    scale_factor = np.power(2.0, scale_exp)
    
    result = np.zeros(fp_data.shape, dtype=np.float32)
    
    for i in range(fp_data.shape[1]):
        block_idx = i // block_size
        result[:, i] = fp_data[:, i].astype(np.float32) * scale_factor[:, block_idx]
    
    return result


def mx_decompress_b(fp_data, scale_data, block_size=32):
    scale_exp = scale_data.astype(np.float32) - 127.0
    scale_factor = np.power(2.0, scale_exp)
    
    result = np.zeros(fp_data.shape, dtype=np.float32)
    
    for row in range(fp_data.shape[0]):
        for col in range(fp_data.shape[1]):
            block_idx = row // block_size
            result[row, col] = fp_data[row, col].astype(np.float32) * scale_factor[block_idx, col]
    
    return result


def layout_scale_a_trans(scale_data):
    m, scale_k = scale_data.shape
    return scale_data.reshape(m, scale_k // 2, 2).transpose(1, 0, 2).copy()


def layout_scale_b_notrans(scale_data):
    scale_k, n = scale_data.shape
    return scale_data.reshape(scale_k // 2, 2, n).transpose(0, 2, 1).copy()


def gen_golden_data_fp4(scenario_num, m, n, k):
    scale_ceil_number = 32
    scale_align_number = 2
    scale_k_unaligned = (k + scale_ceil_number - 1) // scale_ceil_number
    sk = ((scale_k_unaligned + scale_align_number - 1) // scale_align_number) * scale_align_number

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if scenario_num == 1:
        a_dtype = fp4_e1m2
        b_dtype = fp4_e2m1
        a_format = "e1m2"
        b_format = "e2m1"
        is_a_trans = False
        is_b_trans = True
    elif scenario_num == 2:
        a_dtype = fp4_e2m1
        b_dtype = fp4_e1m2
        a_format = "e2m1"
        b_format = "e1m2"
        is_a_trans = True
        is_b_trans = False
    elif scenario_num == 5:
        a_dtype = fp4_e2m1
        b_dtype = fp4_e1m2
        a_format = "e2m1"
        b_format = "e1m2"
        is_a_trans = True
        is_b_trans = False
    else:
        raise ValueError(f"Invalid scenario_num {scenario_num} for FP4")

    x1_data = np.random.uniform(-2, 2, [m, k]).astype(np.float32)
    x2_data = np.random.uniform(-2, 2, [k, n]).astype(np.float32)

    x1_gm, x1_float = cast_to_fp4(x1_data, a_dtype, a_format)
    x2_gm, x2_float = cast_to_fp4(x2_data, b_dtype, b_format)

    # 当scaleK = (k + 32 - 1) // 32为奇数时，根据指令约束需要给scale的k方向填0补齐到偶数，避免脏数据参与运算
    x1_scale_gm = np.random.randint(0, 1, [m, sk]).astype(np.uint8)
    x2_scale_gm = np.random.randint(0, 1, [sk, n]).astype(np.uint8)

    x1_scale_gm_random = np.random.randint(127, 130, [m, scale_k_unaligned]).astype(np.uint8)
    x2_scale_gm_random = np.random.randint(127, 130, [scale_k_unaligned, n]).astype(np.uint8)

    x1_scale_gm[:, :scale_k_unaligned] = x1_scale_gm_random
    x2_scale_gm[:scale_k_unaligned, :] = x2_scale_gm_random

    x1_full = mx_decompress(x1_float, x1_scale_gm, 32)
    x2_full = mx_decompress_b(x2_float, x2_scale_gm, 32)

    golden = np.matmul(x1_full.astype(np.float64), x2_full.astype(np.float64)).astype(np.float32)

    if is_a_trans:
        print("A/scaleA transpose")
        x1_gm = x1_gm.transpose()
        x1_scale_gm = layout_scale_a_trans(x1_scale_gm)
    
    if is_b_trans:
        print("B/scaleB transpose")
        x2_gm = x2_gm.transpose()
        x2_scale_gm = x2_scale_gm.transpose().copy()
    else:
        x2_scale_gm = layout_scale_b_notrans(x2_scale_gm)

    x1_packed = pack_fp4_to_fp4x2(x1_gm)
    x2_packed = pack_fp4_to_fp4x2(x2_gm)

    x1_packed.tofile("./input/x1_gm.bin")
    x2_packed.tofile("./input/x2_gm.bin")
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")


def gen_golden_data_fp8(scenario_num, m, n, k):
    scale_ceil_number = 32
    scale_align_number = 2
    scale_k_unaligned = (k + scale_ceil_number - 1) // scale_ceil_number
    sk = ((scale_k_unaligned + scale_align_number - 1) // scale_align_number) * scale_align_number

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if scenario_num == 3:
        a_dtype = fp8_e4m3
        b_dtype = fp8_e5m2
        a_format = "e4m3fn"
        b_format = "e5m2"
        is_a_trans = False
        is_b_trans = True
    elif scenario_num == 4:
        a_dtype = fp8_e5m2
        b_dtype = fp8_e4m3
        a_format = "e5m2"
        b_format = "e4m3fn"
        is_a_trans = True
        is_b_trans = False
    elif scenario_num == 6:
        a_dtype = fp8_e5m2
        b_dtype = fp8_e4m3
        a_format = "e5m2"
        b_format = "e4m3fn"
        is_a_trans = True
        is_b_trans = False
    else:
        raise ValueError(f"Invalid scenario_num {scenario_num} for FP8")

    x1_data = np.random.uniform(-10, 10, [m, k]).astype(np.float32)
    x2_data = np.random.uniform(-10, 10, [k, n]).astype(np.float32)
    x1_gm, x1_float = cast_to_fp8(x1_data, a_dtype, a_format)
    x2_gm, x2_float = cast_to_fp8(x2_data, b_dtype, b_format)
    
    # 当scaleK = (k + 32 - 1) // 32为奇数时，根据指令约束需要给scale的k方向填0补齐到偶数，避免脏数据参与运算
    x1_scale_gm = np.random.randint(0, 1, [m, sk]).astype(np.uint8)
    x2_scale_gm = np.random.randint(0, 1, [sk, n]).astype(np.uint8)

    x1_scale_gm_random = np.random.randint(127, 130, [m, scale_k_unaligned]).astype(np.uint8)
    x2_scale_gm_random = np.random.randint(127, 130, [scale_k_unaligned, n]).astype(np.uint8)

    x1_scale_gm[:, :scale_k_unaligned] = x1_scale_gm_random
    x2_scale_gm[:scale_k_unaligned, :] = x2_scale_gm_random

    x1_full = mx_decompress(x1_float, x1_scale_gm, 32)
    x2_full = mx_decompress_b(x2_float, x2_scale_gm, 32)

    golden = np.matmul(x1_full.astype(np.float64), x2_full.astype(np.float64)).astype(np.float32)

    if is_a_trans:
        print("A/scaleA transpose")
        x1_gm = x1_gm.transpose()
        x1_scale_gm = layout_scale_a_trans(x1_scale_gm)
    
    if is_b_trans:
        print("B/scaleB transpose")
        x2_gm = x2_gm.transpose()
        x2_scale_gm = x2_scale_gm.transpose().copy()
    else:
        x2_scale_gm = layout_scale_b_notrans(x2_scale_gm)

    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    x1_scale_gm.tofile("./input/x1_scale_gm.bin")
    x2_scale_gm.tofile("./input/x2_scale_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    scenario_num = 1
    if len(sys.argv) > 1:
        scenario_num = int(sys.argv[1].split("=")[1])
    
    m, n, k = 40, 50, 70
    
    if scenario_num in [1, 2, 5]:
        gen_golden_data_fp4(scenario_num, m, n, k)
    elif scenario_num in [3, 4, 6]:
        gen_golden_data_fp8(scenario_num, m, n, k)
    else:
        raise ValueError(f"Invalid scenario_num {scenario_num}")
