#!/usr/bin/python3
# coding=utf-8

import os
import numpy as np
import tensorflow as tf
bfloat16 = tf.bfloat16.as_numpy_dtype

def gen_base(input_dtype, output_dtype, output_format, channel_split):
    M = 128
    K = 128
    N = 256
    kRound = 2

    # 输入数据类型
    if input_dtype == 'half':
        input_type = np.float16
    else:
        input_type = np.int8

    # 输出数据类型
    if output_dtype == 'float32':
        output_type = np.float32
    elif output_dtype == 'bfloat16':
        output_type = bfloat16
    elif output_dtype == 'float16':
        output_type = np.float16
    else:
        output_type = np.float32  # fallback

    # 生成随机输入
    if input_dtype == 'half':
        x1_gm = np.random.uniform(-3.0, 3.0, [M, K]).astype(input_type)
        x2_gm = np.random.uniform(-3.0, 3.0, [K, N]).astype(input_type)
        golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)
    else:  # int8
        x1_gm = np.random.randint(-3, 3, [M, K]).astype(input_type)
        x2_gm = np.random.randint(-3, 3, [K, N]).astype(input_type)
        golden = np.matmul(x1_gm.astype(np.int32), x2_gm.astype(np.int32)).astype(np.int32)
        # int8 * int8 -> int32, 直接作为输出
        if output_dtype == 'int8':
            golden = golden.astype(np.int32)  # 保持 int32 用于后续量化
        else:
            golden = golden.astype(np.float32)

    # 输出转换和布局
    if output_dtype == 'bfloat16':
        golden = golden.astype(bfloat16)
    elif output_dtype == 'float16':
        golden = golden.astype(np.float16)

    # 通道拆分
    if channel_split and output_format == 2 and output_dtype == 'float32':
        block_cols = 8
        golden = golden.reshape((M // 16, 16, N // block_cols, block_cols)).transpose(2, 0, 1, 3).astype(output_type)
    elif output_format == 2:
        block_cols = 16
        golden = golden.reshape((M // 16, 16, N // block_cols, block_cols)).transpose(2, 0, 1, 3).astype(output_type)

    # K轴拆分
    if kRound > 1:
        x1_gm = x1_gm.reshape(M, kRound, K // kRound).transpose(1, 0, 2)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x1_gm.astype(input_type).tofile("./input/x1_gm.bin")
    x2_gm.astype(input_type).tofile("./input/x2_gm.bin")
    golden.astype(output_type).tofile("./output/golden.bin")