#!/usr/bin python3
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
import tensorflow as tf

bfloat16 = tf.bfloat16.as_numpy_dtype

DTYPE_NAME_MAP = {
    "int8_t": np.int8,
    "int16_t": np.int16,
    "half": np.float16,
    "bfloat16_t": bfloat16,
    "int32_t": np.int32,
    "float": np.float32,
}


def get_dtype_limits(data_type):
    if data_type == bfloat16:
        return {
            'min': np.array(-3.389e38, dtype=data_type),
            'max': np.array(3.389e38, dtype=data_type),
            'pos_zero': np.array(+0.0, dtype=data_type),
            'neg_zero': np.array(-0.0, dtype=data_type),
            'nan': np.array(np.nan, dtype=data_type),
            'pos_inf': np.array(np.inf, dtype=data_type),
            'neg_inf': np.array(-np.inf, dtype=data_type),
        }
    elif np.issubdtype(data_type, np.floating):
        finfo = np.finfo(data_type)
        return {
            # 用于边界值/特殊值测试
            'min': finfo.min,
            'max': finfo.max,
            'pos_zero': np.array(+0.0, dtype=data_type),
            'neg_zero': np.array(-0.0, dtype=data_type),
            'nan': np.array(np.nan, dtype=data_type),
            'pos_inf': np.array(np.inf, dtype=data_type),
            'neg_inf': np.array(-np.inf, dtype=data_type),

            # 用于正常值测试（验证API基本功能）
            # 'min': 1.234,
            # 'max': -0.399,
            # 'pos_zero': 9.99,
            # 'neg_zero': -1.88,
            # 'nan': -6.68,
            # 'pos_inf': 0.111,
            # 'neg_inf': 7.777,
        }
    elif np.issubdtype(data_type, np.integer):
        iinfo = np.iinfo(data_type)
        return {
            # 用于边界值/特殊值测试
            'min': np.array(iinfo.min, dtype=data_type),
            'max': np.array(iinfo.max, dtype=data_type),
            'pos_zero': np.array(+0, dtype=data_type),
            'neg_zero': np.array(-0, dtype=data_type),

            # 用于正常值测试（验证API基本功能）
            # 'min': 99,
            # 'max': -88,
            # 'pos_zero': 6,
            # 'neg_zero': -3,
        }
    else:
        raise ValueError(f"Unsupported data type: {data_type}")


def generate_one_input(data_type):
    limits = get_dtype_limits(data_type)
    specials = [limits['min'], limits['max'], limits['pos_zero'], limits['neg_zero']]
    if np.issubdtype(data_type, np.floating):
        specials.extend([limits['nan'], limits['pos_inf'], limits['neg_inf']])
    return specials


def generate_two_inputs(data_type):
    vals = generate_one_input(data_type)
    inputs_a = []
    inputs_b = []
    for a in vals:
        for b in vals:
            inputs_a.append(a)
            inputs_b.append(b)
    return np.array(inputs_a, dtype=data_type), np.array(inputs_b, dtype=data_type)


def save_bin(data, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.tofile(path)


def get_special_names(data_type):
    special_names = ['min', 'max', '+0', '-0']
    if np.issubdtype(data_type, np.floating):
        special_names.extend(['NaN', '+INF', '-INF'])
    return special_names


def format_float16(val):
    if np.issubdtype(val.dtype, np.floating):
        return f"{float(val):.6f}"
    return str(val)


def print_unary_api_table(api_name, type_name, special_names, inputs, golden):
    print(f"\n{'='*116}")
    print(f"API: {api_name} | Data Type: {type_name} | Gen Golden")
    print(f"{'='*116}")
    print(f"{'type':<8} | {'src':<26} | {'dst (Numpy-Golden)':<26}")
    print(f"{'-'*116}")
    for i, name in enumerate(special_names):
        inp_val = inputs[i]
        out_val = golden[i]
        inp_str = format_float16(inp_val) if inputs.dtype == np.float16 else str(inp_val)
        out_str = format_float16(out_val) if golden.dtype == np.float16 else str(out_val)
        print(f"{type_name:<8} | {name:<6}{inp_str:<20} | {out_str:<26}")
    print(f"{'='*116}\n")


def print_binary_api_table(api_name, type_name, special_names, inputs_a, inputs_b, golden):
    print(f"\n{'='*124}")
    print(f"API: {api_name} | Data Type: {type_name} | Gen Golden")
    print(f"{'='*124}")
    print(f"{'type':<8} | {'src0':<26} | {'src1':<26} | {'dst (Numpy-Golden)':<26}")
    print(f"{'-'*124}")
    for i, name_a in enumerate(special_names):
        for j, name_b in enumerate(special_names):
            idx = i * len(special_names) + j
            out_val = golden[idx]
            inp_a = inputs_a[idx]
            inp_b = inputs_b[idx]
            inp_a_str = format_float16(inp_a) if inputs_a.dtype == np.float16 else str(inp_a)
            inp_b_str = format_float16(inp_b) if inputs_b.dtype == np.float16 else str(inp_b)
            out_str = format_float16(out_val) if golden.dtype == np.float16 else str(out_val)
            print(f"{type_name:<8} | {name_a:<6}{inp_a_str:<20} | {name_b:<6}{inp_b_str:<20} | {out_str:<26}")
    print(f"{'='*124}\n")