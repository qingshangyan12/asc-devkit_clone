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

from pathlib import Path

import numpy as np


MATMUL_M = 16
MATMUL_K = 32
MATMUL_N = 16


def gen_data():
    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    # A[m][k] = m*K + k（0..511），half 可精确表示；B[k][n] = (k*N+n) % 7 + 1（1..7）。
    # 取值互异，便于按物理布局核对 dump 数据。
    a = np.arange(MATMUL_M * MATMUL_K, dtype=np.float32).reshape(MATMUL_M, MATMUL_K)
    b = (np.arange(MATMUL_K * MATMUL_N, dtype=np.float32).reshape(MATMUL_K, MATMUL_N) % 7.0) + 1.0

    a_half = a.astype(np.float16)
    b_half = b.astype(np.float16)

    # Mmad 在 float 累加；A、B 均为可精确表示的小整数，乘加结果仍为 < 2^24 的整数，float32 精确。
    golden = np.matmul(a, b).astype(np.float32)

    a_half.tofile(input_dir / "a_gm.bin")
    b_half.tofile(input_dir / "b_gm.bin")
    golden.tofile(output_dir / "golden.bin")


if __name__ == "__main__":
    gen_data()
