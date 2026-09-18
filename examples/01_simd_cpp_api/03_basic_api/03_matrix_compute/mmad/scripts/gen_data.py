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

import argparse
import os
import numpy as np


def f32_to_bf16_u16(arr):
    """Pack float32 -> bfloat16 bit patterns without requiring ml_dtypes."""
    f32 = np.asarray(arr, dtype=np.float32)
    return (f32.view(np.uint32) >> 16).astype(np.uint16)


def bf16_u16_to_f32(u16):
    u32 = np.asarray(u16, dtype=np.uint16).astype(np.uint32) << 16
    return u32.view(np.float32)


def _fp8_dtypes():
    try:
        import ml_dtypes

        return ml_dtypes.float8_e4m3fn, ml_dtypes.float8_e5m2
    except ImportError as e:
        raise SystemExit(
            "FP8 golden requires ml_dtypes==0.2.0 "
            "(pip install ml_dtypes==0.2.0)."
        ) from e


def gen_golden_data(scenario_num=1, m=30, n=40, k=70):
    if scenario_num == 1:
        x1_gm = np.random.randint(1, 10, [m, k]).astype(np.int8)
        x2_gm = np.random.randint(1, 10, [k, n]).astype(np.int8)
        bias_gm = np.random.randint(1, 10, [n]).astype(np.int32)
        golden = (
            np.matmul(x1_gm.astype(np.int32), x2_gm.astype(np.int32)).astype(np.int32)
            + bias_gm
        )
    elif scenario_num == 2:
        # Without ml_dtypes, .astype(None) becomes float64 and tofile writes 8B/elem
        # while the kernel reads sizeof(bfloat16)=2 -> garbage / all-zero C.
        x1_f = np.random.uniform(1, 10, [m, k]).astype(np.float32)
        x2_f = np.random.uniform(1, 10, [k, n]).astype(np.float32)
        x1_bits = f32_to_bf16_u16(x1_f)
        x2_bits = f32_to_bf16_u16(x2_f)
        golden = (
            np.matmul(
                bf16_u16_to_f32(x1_bits).reshape(m, k),
                bf16_u16_to_f32(x2_bits).reshape(k, n),
            ).astype(np.float32)
            * 2
        )
        x1_gm = x1_bits.reshape(-1)
        x2_gm = f32_to_bf16_u16(x2_f.T).reshape(-1)
    elif scenario_num == 3:
        x1_gm = np.random.uniform(1, 10, [m, k]).astype(np.float32)
        x2_gm = np.random.uniform(1, 10, [k, n]).astype(np.float32)
        bias_gm = np.random.uniform(1, 10, [n]).astype(np.float32)
        golden = (
            np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(
                np.float32
            )
            + bias_gm
        )
        x1_gm = x1_gm.transpose()
        x2_gm = x2_gm.transpose()
    elif scenario_num == 5:
        # half input, float output; A no-trans, B trans (like load_data_2dv2 s3)
        x1_gm = np.random.uniform(1, 10, [m, k]).astype(np.float16)
        x2_gm = np.random.uniform(1, 10, [k, n]).astype(np.float16)
        golden = np.matmul(
            x1_gm.astype(np.float32), x2_gm.astype(np.float32)
        ).astype(np.float32)
        x2_gm = x2_gm.transpose()
    elif scenario_num in (6, 7):
        # non-MX fp8: A no-trans, B trans, float C, no bias (dav-3510)
        fp8_e4m3, fp8_e5m2 = _fp8_dtypes()
        a_dtype = fp8_e4m3 if scenario_num == 6 else fp8_e5m2
        x1_gm = np.random.uniform(1, 10, [m, k]).astype(a_dtype)
        x2_gm = np.random.uniform(1, 10, [k, n]).astype(a_dtype)
        golden = np.matmul(
            x1_gm.astype(np.float32), x2_gm.astype(np.float32)
        ).astype(np.float32)
        x2_gm = x2_gm.transpose()
    else:
        x1_gm_int8 = np.random.randint(1, 7, [m, k]).astype(np.int8)
        x2_gm_int8 = np.random.randint(1, 7, [k, n]).astype(np.int8)

        golden = np.matmul(
            x1_gm_int8.astype(np.int32), x2_gm_int8.astype(np.int32)
        ).astype(np.int32)
        x2_gm_int8 = x2_gm_int8.transpose()

        x1_gm = np.zeros(shape=[m, k // 2]).astype(np.int8)
        for i in range(m):
            for j in range(k):
                if j % 2 == 0:
                    x1_gm[i][j // 2] = (x1_gm_int8[i][j + 1] << 4) + (
                        x1_gm_int8[i][j] & 0x0F
                    )
        x2_gm = np.zeros(shape=[n, k // 2]).astype(np.int8)
        for i in range(n):
            for j in range(k):
                if j % 2 == 0:
                    x2_gm[i][j // 2] = (x2_gm_int8[i][j + 1] << 4) + (
                        x2_gm_int8[i][j] & 0x0F
                    )

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")
    if scenario_num in [1, 3]:
        bias_gm.tofile("./input/bias_gm.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 8))
    parser.add_argument("-M", type=int, default=30)
    parser.add_argument("-N", type=int, default=40)
    parser.add_argument("-K", type=int, default=70)
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.M, args.N, args.K)
