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
    return relu_alpha

def extract_quant_params(quant_pre):
    """
    从uint64类型的quant_gm中提取M1、offset、sign参数
    param:
        quant_pre:uint64类型的整数
    return:
        quant_alpha:自定义格式(1,8,10)的浮点数
        offset:9位整数
        sign:1位布尔值(0或1)
    """
    quant_pre = int(quant_pre)
    quant_alpha_bits = (quant_pre >> 13) & 0xFFFFF  # 提取M1的20位[31:13]，0xFFFFF是20位掩码
    mode_control_bit = (quant_pre >> 36) & 0x1  # 提取mode_ctrl bit的一位[36]，0x1是1位掩码
    offset = (quant_pre >> 37) & 0x1FF # 提取offset的9位[45:37]，0x1FF是9位掩码
    sign = (quant_pre >> 46) & 0x1  # 提取sign的一位[46]，0x1是1位掩码
    n = (quant_pre >> 32) & 0xF
    # 解析M1为(1,8,10)格式的浮点数
    sign_bit = (quant_alpha_bits >> 18) & 0x1
    exponent = (quant_alpha_bits >> 10) & 0xFF
    mantissa = quant_alpha_bits & 0x3FF
    exponent_bias = 127 # 假设指数偏倚量为127，与float32一致
    quant_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return quant_alpha, offset, sign, n, mode_control_bit

def saturation(value, min_val, max_val, target_type):
    """
    将输入的浮点数进行饱和处理，并转换为目标类型
    """
    x_clamped = np.clip(value, min_val, max_val)
    return np.round(x_clamped).astype(target_type)

def qf322b8_pre(data, quant_pre, relu_pre):
    """
    float32 -> int8/uint8
    """
    quant_alpha, offset, sign, n_shift, mode_control_bit = extract_quant_params(quant_pre)
    # sign = 0
    relu_alpha = extract_relu_params(relu_pre)
    if mode_control_bit == 1:
        data = data / (2 ** n_shift)
        data = saturation(data, -32768, 32767, np.int16)
    data = data.astype(np.float32)
    if data >= 0:
        data = data * quant_alpha
    else:
        data = data * relu_alpha
    quant_data = saturation(data, -256, 255, np.int16) + offset
    if sign:
        return saturation(quant_data, -128, 127, np.int8)
    else:
        return saturation(quant_data, 0, 255, np.uint8)

def pre_quant_relu(golden, dst_type, m, n, pre_quant_mode, pre_relu_mode):
    quant_scalar = 2
    norm_relu_alpha = 0

    relu_alpha = 1
    if pre_relu_mode == 0:
        relu_alpha = quant_scalar
    elif pre_relu_mode == 1:
        relu_alpha = norm_relu_alpha
    relu_alpha = struct.unpack('!I', struct.pack('!f', relu_alpha))[0]

    # 1 * n 量化系数为全为quant scalar的量化tensor
    temp_quant_tensor = ((quant_scalar * np.ones((1, n), dtype=np.float32)).astype(np.float32))[0]
    temp_quant_tensor_api = copy.deepcopy(temp_quant_tensor).astype(np.uint64)
    for i, _ in enumerate(temp_quant_tensor_api):
        temp_quant_tensor_api[i] = struct.unpack('!I', struct.pack('!f', temp_quant_tensor[i]))[0]
        if dst_type == np.int8:
            # np.int8 sign is true
            temp_quant_tensor_api[i] = temp_quant_tensor_api[i] | np.uint64(0x400000000000)
    quant_tensor = np.frombuffer(temp_quant_tensor_api, np.uint64)
    quant_tensor = quant_tensor.astype(np.uint64)
    if pre_quant_mode == 2:
        quant_tensor.tofile("./input/quant_pre.bin")
    quant_golden = np.zeros((m, n), dtype=np.int8)
    for i in range(m):
        for j in range(n):
            quant_golden[i, j] = qf322b8_pre(golden[i, j], quant_tensor[j], relu_alpha)
    return quant_golden


def ceil_div(a, b):
    return (a + b - 1) // b

BLOCK_SIZE = 16

def tik_conv2d(fm_shape, k_shape, fm_tensor_dtype, weight_tensor_dtype, output_l0c_dtype, output_gm_dtype,
                stride_list, pad_list, pad_value, dilation_list):
    c1, h, w, c0 = fm_shape
    c1, kh, kw, cout, c0 = k_shape
    cin = c0 * c1
    cin_blocks = c1
    cout_blk = cout // 16
    stride_h, stride_w = stride_list
    dilation_h, dilation_w = dilation_list
    pad_left, pad_right, pad_top, pad_bot = pad_list
    kh_dilation = (kh - 1) * dilation_h + 1
    kw_dilation = (kw - 1) * dilation_w + 1
    ho = int(np.ceil((h + pad_top + pad_bot - kh_dilation + 1) / stride_h))
    wo = int(np.ceil((w + pad_right + pad_left - kw_dilation + 1) / stride_w))

    fm = np.random.randint(-5, 5, size=(1, h, w, c1 * c0)).astype(fm_tensor_dtype)
    weight = np.random.randint(-5, 5, size=(kh, kw, c1 * c0, cout)).astype(weight_tensor_dtype)

    fm_padding = (np.ones((1, h + pad_top + pad_bot,
                        w + pad_left + pad_right, cin)) * pad_value).astype(fm_tensor_dtype)
    fm_padding[:, pad_top:pad_top + h, pad_left:pad_left + w, :] = fm
    round_howo = ceil_div(ho * wo, 16) * 16

    load3d_fm_tik = np.zeros((round_howo, cin * kh_dilation * kw_dilation), dtype=fm_tensor_dtype)
    load2d_w_tik = np.zeros((cin * kh_dilation * kw_dilation, cout), dtype=weight_tensor_dtype)

    w_dilation = np.zeros((kh_dilation, kw_dilation, cin, cout), dtype=weight_tensor_dtype)
    h_indices, w_indices = np.meshgrid(np.arange(kh), np.arange(kw), indexing='ij')
    h_new = np.where(h_indices == kh - 1, kh_dilation - 1, h_indices * dilation_h)
    w_new = np.where(w_indices == kw - 1, kw_dilation - 1, w_indices * dilation_w)
    w_dilation[h_new, w_new] = weight[h_indices, w_indices]

    for channel_out in range(cout):
        filter_data = w_dilation[:, :, :, channel_out]
        load2d_w_tik[:, channel_out] = filter_data.flatten()

    source_col_indices = np.arange(kh_dilation * kw_dilation * cin)
    c0_index = source_col_indices % c0
    c1_index = (source_col_indices // c0) % cin_blocks
    kw_index = (source_col_indices // cin) % kw_dilation
    kh_index = (source_col_indices // (cin * kw_dilation)) % kh_dilation
    new_col_indices = (
        c0_index + 
        kw_index * c0 + 
        kh_index * c0 * kw_dilation + 
        c1_index * c0 * kh_dilation * kw_dilation
    )

    channel_data = fm_padding[0, :, :, :]
    for r in range(ho):
        for c in range(wo):
            cur_input = channel_data[stride_h * r: stride_h * r + kh_dilation,
                        stride_w * c:kw_dilation + stride_w * c, :]
            load3d_fm_tik[r * wo + c, :] = cur_input.flatten()

    ret = np.zeros([1, round_howo, cout], dtype=output_l0c_dtype)
    howo_blk = round_howo // 16
    cout_blk = cout // 16

    total_cin_blocks = cin_blocks * kh_dilation * kw_dilation

    fm_block_struct = load3d_fm_tik.reshape(howo_blk, BLOCK_SIZE, total_cin_blocks, c0)

    w_block_struct = load2d_w_tik.reshape(total_cin_blocks, c0, cout_blk, BLOCK_SIZE)

    for i in range(howo_blk):
        for q in range(cout_blk):
            fm_chunk = fm_block_struct[i, :, :, :]
            
            w_chunk = w_block_struct[:, :, q, :]
            
            fm_chunk_aligned = fm_chunk.transpose(1, 0, 2)
            
            partial_sums = np.matmul(fm_chunk_aligned, w_chunk)
            
            conv_sum = np.sum(partial_sums, axis=0)
            
            fm_slice = slice(i * BLOCK_SIZE, (i + 1) * BLOCK_SIZE)
            w_slice = slice(q * BLOCK_SIZE, (q + 1) * BLOCK_SIZE)
            ret[0, fm_slice, w_slice] = (
                ret[0, fm_slice, w_slice].astype("float64") +
                conv_sum.astype("float64")
            ).astype(output_l0c_dtype)

    j_indices, k_indices, m_indices, n_indices = np.meshgrid(
        np.arange(cout_blk),
        np.arange(ho),
        np.arange(wo),
        np.arange(16),
        indexing='ij'
    )

    source_row_indices = k_indices * wo + m_indices
    source_col_indices = j_indices * 16 + n_indices

    ret_z = np.zeros((cout_blk, ho, wo, 16), dtype=output_l0c_dtype)
    ret_z[j_indices, k_indices, m_indices, n_indices] = ret[0, source_row_indices, source_col_indices]

    golden = ret_z.astype("float32")
    golden = golden.astype(output_gm_dtype)
    golden = golden.transpose((1, 2, 0, 3)).reshape(ho * wo, cout_blk * 16).transpose(1, 0)
    return golden


def gen_golden_data(scenarioNum=1):
    M = 128
    K = 128
    N = 256
    kRound = 2 #K轴切分2次

    input_type = np.dtype("float16")
    output_type = np.dtype("float32")
    x1_gm = np.random.uniform(-1, 3, [M, K]).astype(input_type)
    x2_gm = np.random.uniform(-1, 3, [K, N]).astype(input_type)
    golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)
    # print(golden)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if scenarioNum == 1:
        block_cols = 16
        golden = golden.reshape((int(M / 16), 16, int(N / block_cols), block_cols)).transpose(2, 0, 1, 3).astype(output_type)
    elif scenarioNum == 2:
        pass
    elif scenarioNum == 3:
        ho, wo = 8, 16
        kh, kw = 1, 1
        fm_shape = (1, ho, wo, 1)
        k_shape = (1, kh, kw, N, 1)
        fm_tensor_dtype = np.dtype("float16")
        weight_tensor_dtype = np.dtype("float16")
        output_l0c_dtype = np.dtype("float32")
        output_gm_dtype = np.dtype("float32")
        stride_list = (1, 1)
        pad_list = (0, 0, 0, 0)
        pad_value = 0
        dilation_list = (1, 1)
        golden = tik_conv2d(fm_shape, k_shape, fm_tensor_dtype, weight_tensor_dtype,
                            output_l0c_dtype, output_gm_dtype, stride_list, pad_list,
                            pad_value, dilation_list)
    elif scenarioNum == 4:
        output_type = np.dtype("int8")
        golden = pre_quant_relu(golden, output_type, M, N, 1, 0) #scalar
    elif scenarioNum == 5:
        output_type = np.dtype("int8")
        golden = pre_quant_relu(golden, output_type, M, N, 2, 0) #vector
    elif scenarioNum == 6:
        golden = np.maximum(golden, 0).astype(output_type)
    elif scenarioNum == 7:
        block_cols = 8
        golden = golden.reshape((int(M / 16), 16, int(N / block_cols), block_cols)).transpose(2, 0, 1, 3).astype(output_type)

    if kRound > 1:
        # 将K轴外移
        x1_gm = x1_gm.reshape(M, kRound, K//kRound).transpose(1, 0, 2)
    x1_gm.astype(input_type).tofile("./input/x1_gm.bin")
    x2_gm.astype(input_type).tofile("./input/x2_gm.bin")
    golden.astype(output_type).tofile("./output/golden.bin")
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 8))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
