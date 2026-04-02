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
import argparse
import copy
import struct
import numpy as np
np.random.seed(9)

def extract_relu_params(relu_pre):
    relu_pre = int(relu_pre)
    relu_alpha_bits = (relu_pre >> 13) & 0xFFFFF  # 提取M2的20位[31:13]，0xFFFFF是20位掩码
    sign_bit = (relu_alpha_bits >> 18) & 0x1
    exponent = (relu_alpha_bits >> 10) & 0xFF
    mantissa = relu_alpha_bits & 0x3FF
    exponent_bias = 127 # 假设指数偏倚量为127，与float32一致
    relu_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    if relu_alpha == (-1) ** 0 * (1 + 0 / 1024) * (2 ** (0 - exponent_bias)):
        relu_alpha = 0.0
    
    return relu_alpha

def f322f16(data, relu_pre):
    """
    float32 -> float16
    """
    relu_alpha = extract_relu_params(relu_pre)
    data = data.astype(np.float32)
    if data < 0:
        data = data * relu_alpha
    return data.astype(np.float16)

def pre_cast_relu(golden, dst_type, m, n, pre_relu_mode):
    cast_scalar = 1
    norm_relu_alpha = 0

    relu_alpha = 1
    if pre_relu_mode == 0:
        relu_alpha = cast_scalar
    elif pre_relu_mode == 1:
        relu_alpha = norm_relu_alpha
    relu_alpha = struct.unpack('!I', struct.pack('!f', relu_alpha))[0]

    cast_golden = np.zeros((m, n), dtype=np.float16)
    for i in range(m):
        for j in range(n):
            cast_golden[i, j] = f322f16(golden[i, j], relu_alpha)
    return cast_golden

def gen_golden_data(pre_relu_mode):
    M = 128
    K = 128
    N = 256
    kRound = 2 #K轴切分2次

    input_type = np.dtype("float16")
    output_type = np.dtype("float16")
    x1_gm = np.random.uniform(-3.0, 3.0, [M, K]).astype(input_type)
    x2_gm = np.random.uniform(-3.0, 3.0, [K, N]).astype(input_type)
    golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)
    os.system("mkdir -p input")
    os.system("mkdir -p output")

    golden = pre_cast_relu(golden, output_type, M, N, pre_relu_mode)
    block_cols = 16
    golden = golden.reshape((int(M / 16), 16, int(N / block_cols), block_cols)).transpose(2, 0, 1, 3).astype(output_type)

    if kRound > 1:
        # 将K轴外移
        x1_gm = x1_gm.reshape(M, kRound, K//kRound).transpose(1, 0, 2)
    x1_gm.astype(input_type).tofile("./input/x1_gm.bin")
    x2_gm.astype(input_type).tofile("./input/x2_gm.bin")
    golden.astype(output_type).tofile("./output/golden.bin")
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-pre_relu_mode', type=int, default=0, choices=range(0, 2))
    args = parser.parse_args()
    gen_golden_data(args.pre_relu_mode)
