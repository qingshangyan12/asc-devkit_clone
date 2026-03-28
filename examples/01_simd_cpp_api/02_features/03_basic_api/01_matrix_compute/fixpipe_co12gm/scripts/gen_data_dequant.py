#!/usr/bin/python3
# coding=utf-8

import os
import copy
import struct
import numpy as np
import tensorflow as tf
bfloat16 = tf.bfloat16.as_numpy_dtype

def extract_relu_params(relu_pre):
    relu_pre = int(relu_pre)
    relu_alpha_bits = (relu_pre >> 13) & 0xFFFFF
    sign_bit = (relu_alpha_bits >> 18) & 0x1
    exponent = (relu_alpha_bits >> 10) & 0xFF
    mantissa = relu_alpha_bits & 0x3FF
    exponent_bias = 127
    relu_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return relu_alpha

def extract_quant_params(quant_pre):
    quant_pre = int(quant_pre)
    quant_alpha_bits = (quant_pre >> 13) & 0xFFFFF
    mode_control_bit = (quant_pre >> 36) & 0x1
    offset = (quant_pre >> 37) & 0x1FF
    sign = (quant_pre >> 46) & 0x1
    n = (quant_pre >> 32) & 0xF
    sign_bit = (quant_alpha_bits >> 18) & 0x1
    exponent = (quant_alpha_bits >> 10) & 0xFF
    mantissa = quant_alpha_bits & 0x3FF
    exponent_bias = 127
    quant_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return quant_alpha, offset, sign, n, mode_control_bit

def saturation(value, min_val, max_val, target_type):
    x_clamped = np.clip(value, min_val, max_val)
    return np.round(x_clamped).astype(target_type)

def deqf16(data, quant_pre, relu_pre):
    quant_alpha, offset, sign, n_shift, mode_control_bit = extract_quant_params(quant_pre)
    relu_alpha = extract_relu_params(relu_pre)
    if mode_control_bit == 1:
        data = data >> n_shift
        data = saturation(data, np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16)
    data = data.astype(np.float32)
    if data >= 0:
        data = data * quant_alpha
    else:
        data = data * relu_alpha
    quant_data = saturation(data, np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16) + offset
    return saturation(quant_data, np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16)

def gen_dequant(input_dtype, output_dtype, output_format, pre_quant_mode, pre_relu_mode):
    M = 128
    K = 128
    N = 256
    kRound = 2

    # 输入数据类型
    if input_dtype == 'half':
        input_type = np.float16
        # 生成 half 输入（这里实际不会用到，因为反量化输入是 int8，但为了统一逻辑，这里生成 int8 输入）
        # 实际反量化场景输入是 int8，所以我们将输入视为 int8
        input_type = np.int8
    else:
        input_type = np.int8

    # 生成 int8 输入
    x1_gm = np.random.randint(-3, 3, [M, K]).astype(np.int8)
    x2_gm = np.random.randint(-3, 3, [K, N]).astype(np.int8)
    golden = np.matmul(x1_gm.astype(np.int32), x2_gm.astype(np.int32)).astype(np.int32)

    # 量化参数
    quant_scalar = 2.0
    norm_relu_alpha = 0
    relu_alpha_val = 1
    if pre_relu_mode:
        relu_alpha_val = norm_relu_alpha
    else:
        relu_alpha_val = quant_scalar
    relu_alpha = struct.unpack('!I', struct.pack('!f', relu_alpha_val))[0]

    # 构造量化参数向量（每列一个）
    temp_quant_tensor = ((quant_scalar * np.ones((1, N), dtype=np.float32)).astype(np.float32))[0]
    temp_quant_tensor_api = copy.deepcopy(temp_quant_tensor).astype(np.uint64)
    for i, _ in enumerate(temp_quant_tensor_api):
        temp_quant_tensor_api[i] = struct.unpack('!I', struct.pack('!f', temp_quant_tensor[i]))[0]
        if output_dtype == 'float16':
            # 设置 sign 位
            temp_quant_tensor_api[i] = temp_quant_tensor_api[i] | np.uint64(0x400000000000)
    quant_tensor = np.frombuffer(temp_quant_tensor_api, np.uint64)
    quant_tensor = quant_tensor.astype(np.uint64)

    # 反量化真值生成
    dequant_golden = np.zeros((M, N), dtype=np.float16)
    for i in range(M):
        for j in range(N):
            dequant_golden[i, j] = deqf16(golden[i, j], quant_tensor[j], relu_alpha)

    # 转换输出类型
    if output_dtype == 'bfloat16':
        dequant_golden = dequant_golden.astype(bfloat16)
    elif output_dtype == 'float32':
        dequant_golden = dequant_golden.astype(np.float32)
    # float16 保持不变

    # 布局调整
    if output_format == 2:
        block_cols = 16
        dequant_golden = dequant_golden.reshape((M // 16, 16, N // block_cols, block_cols)).transpose(2, 0, 1, 3).astype(dequant_golden.dtype)

    # K轴拆分
    if kRound > 1:
        x1_gm = x1_gm.reshape(M, kRound, K // kRound).transpose(1, 0, 2)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x1_gm.astype(np.int8).tofile("./input/x1_gm.bin")
    x2_gm.astype(np.int8).tofile("./input/x2_gm.bin")
    if pre_quant_mode == 2:
        quant_tensor.tofile("./input/quant_pre.bin")
    dequant_golden.astype(dequant_golden.dtype).tofile("./output/golden.bin")