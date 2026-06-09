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
import struct
import numpy as np


def make_quant_param(scale, signed_output=True):
    quant = np.uint64(struct.unpack("!I", struct.pack("!f", float(scale)))[0])
    if signed_output:
        quant = quant | np.uint64(1 << 46)
    return quant


def extract_quant_params(quant_pre):
    quant_pre = int(quant_pre)
    quant_alpha_bits = (quant_pre >> 13) & 0xFFFFF
    mode_control_bit = (quant_pre >> 36) & 0x1
    offset = (quant_pre >> 37) & 0x1FF
    sign = (quant_pre >> 46) & 0x1
    n_shift = (quant_pre >> 32) & 0xF

    sign_bit = (quant_alpha_bits >> 18) & 0x1
    exponent = (quant_alpha_bits >> 10) & 0xFF
    mantissa = quant_alpha_bits & 0x3FF
    exponent_bias = 127
    quant_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return quant_alpha, offset, sign, n_shift, mode_control_bit


def saturation(value, min_val, max_val, target_type):
    return np.round(np.clip(value, min_val, max_val)).astype(target_type)


def qf322b8_pre(data, quant_pre):
    quant_alpha, offset, sign, n_shift, mode_control_bit = extract_quant_params(quant_pre)
    data = np.float32(data)
    if mode_control_bit == 1:
        data = saturation(data / (2 ** n_shift), -32768, 32767, np.int16)
    quant_data = saturation(np.float32(data) * quant_alpha, -256, 255, np.int16) + offset
    if sign:
        return saturation(quant_data, -128, 127, np.int8)
    return saturation(quant_data, 0, 255, np.uint8)


def ceil_align(value, align):
    return (value + align - 1) // align * align


def to_nz_layout(data, c0=16):
    batch, m_size, n_size = data.shape
    m_align = ceil_align(m_size, 16)
    n_align = ceil_align(n_size, c0)
    nz_data = np.zeros((batch, m_align * n_align), dtype=data.dtype)
    for batch_index in range(batch):
        for row_index in range(m_size):
            for column_index in range(n_size):
                row_inner = row_index % 16
                row_outer = row_index // 16
                column_inner = column_index % c0
                column_outer = column_index // c0
                offset = row_inner * c0 + row_outer * c0 * 16 + column_inner + column_outer * c0 * m_align
                nz_data[batch_index, offset] = data[batch_index, row_index, column_index]
    return nz_data


def gen_case_data(case_name, batch, m_size, k_size, n_size, golden_dtype, output_format="ND", quant_mode=None):
    x1_gm = np.zeros((batch, m_size, k_size), dtype=np.float16)
    x2_gm = np.zeros((batch, k_size, n_size), dtype=np.float16)

    row = np.arange(m_size, dtype=np.float32).reshape(m_size, 1)
    col = np.arange(k_size, dtype=np.float32).reshape(1, k_size)
    for batch_index in range(batch):
        if quant_mode is None:
            data = (batch_index + 1) * 8.0 + row * 0.25 + col * 0.03125
        else:
            data = (batch_index + 1) * 5.0 + (row % 8) + (col % 7)
        x1_gm[batch_index] = data.astype(np.float16)
        x2_gm[batch_index] = np.eye(k_size, n_size, dtype=np.float16)

    golden_float = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32))
    quant_tensor = None
    if quant_mode == "scalar":
        quant_param = make_quant_param(1.0)
        golden = np.empty_like(golden_float, dtype=golden_dtype)
        for index in np.ndindex(golden_float.shape):
            golden[index] = qf322b8_pre(golden_float[index], quant_param)
    elif quant_mode == "vector":
        quant_tensor = np.array([make_quant_param(1.0 + (column % 2)) for column in range(n_size)], dtype=np.uint64)
        golden = np.empty_like(golden_float, dtype=golden_dtype)
        for batch_index in range(batch):
            for row_index in range(m_size):
                for column_index in range(n_size):
                    golden[batch_index, row_index, column_index] = qf322b8_pre(
                        golden_float[batch_index, row_index, column_index], quant_tensor[column_index])
    else:
        golden = golden_float.astype(golden_dtype)

    if output_format == "DN":
        golden = np.ascontiguousarray(golden.transpose(0, 2, 1))
    elif output_format == "NZ":
        golden = to_nz_layout(golden)

    input_dir = os.path.join("input", case_name)
    output_dir = os.path.join("output", case_name)
    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)
    x1_gm.tofile(os.path.join(input_dir, "x1_gm.bin"))
    x2_gm.tofile(os.path.join(input_dir, "x2_gm.bin"))
    if quant_tensor is not None:
        quant_tensor.tofile(os.path.join(input_dir, "quant_gm.bin"))
    golden.tofile(os.path.join(output_dir, "golden.bin"))


def gen_golden_data():
    gen_case_data("case1", 3, 56, 80, 64, np.float32)
    gen_case_data("case2", 3, 64, 64, 64, np.float16)
    gen_case_data("case3", 3, 64, 80, 64, np.float16, "DN")
    gen_case_data("case4", 2, 52, 72, 40, np.float32)
    gen_case_data("case5", 3, 52, 72, 64, np.int8, quant_mode="scalar")
    gen_case_data("case6", 3, 52, 72, 64, np.int8, quant_mode="vector")
    gen_case_data("case7", 3, 64, 64, 64, np.float16, "NZ")


if __name__ == "__main__":
    gen_golden_data()
