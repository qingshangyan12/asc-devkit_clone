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


DATA_TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}

NUM_BLOCKS = 8
BLOCK_SIZE = 128


def get_dtype_limits(data_type):
    if np.issubdtype(data_type, np.floating):
        finfo = np.finfo(data_type)
        return {
            'min': finfo.min,
            'max': finfo.max,
            'pos_zero': np.array(+0.0, dtype=data_type),
            'neg_zero': np.array(-0.0, dtype=data_type),
            'nan': np.array(np.nan, dtype=data_type),
            'pos_inf': np.array(np.inf, dtype=data_type),
            'neg_inf': np.array(-np.inf, dtype=data_type),
        }
    elif np.issubdtype(data_type, np.integer):
        iinfo = np.iinfo(data_type)
        return {
            'min': np.array(iinfo.min, dtype=data_type),
            'max': np.array(iinfo.max, dtype=data_type),
            'pos_zero': np.array(+0, dtype=data_type),
            'neg_zero': np.array(-0, dtype=data_type),
        }
    else:
        raise ValueError(f"Unsupported data type: {data_type}")


def construct_special_values(data_type):
    limits = get_dtype_limits(data_type)
    specials = [limits['min'], limits['max'], limits['pos_zero'], limits['neg_zero']]
    if np.issubdtype(data_type, np.floating):
        specials.extend([limits['nan'], limits['pos_inf'], limits['neg_inf']])
    return specials


def construct_block_data(data_type):
    specials = construct_special_values(data_type)
    block = np.ones(BLOCK_SIZE, dtype=data_type)
    
    idx = 0
    for special in specials:
        repeat_count = 16
        block[idx:idx + repeat_count] = special
        idx += repeat_count
    
    return block


def construct_multi_block_data(data_type):
    blocks = []
    for _ in range(NUM_BLOCKS):
        block = construct_block_data(data_type)
        blocks.append(block)
    return np.concatenate(blocks)


def get_representative_values(data_type):
    specials = construct_special_values(data_type)
    return specials[:7] if len(specials) >= 7 else specials


def cartesian_product_inputs(data_type):
    vals = construct_special_values(data_type)
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
        # special_names.extend(['NaN', '+INF', '-INF', 'min_normal', 'neg_min_normal'])
    return special_names


def format_value(val):
    # if np.issubdtype(val.dtype, np.floating):
    #     return f"{float(val):.6f}"
    # return str(val)
    if val.dtype == np.float16:
        return f"{float(val):.8f}"
    else:
        return str(val)


def print_unary_api_table(api_name, type_name, special_names, inputs, golden):
    print(f"\n{'='*130}")
    print(f"API: {api_name} | Data Type: {type_name} | Gen Golden")
    print(f"{'='*130}")
    print(f"{'type':<8} | {'src':<32} | {'dst (Numpy-Golden)':<32}")
    print(f"{'-'*130}")
    for i in range(len(golden) // 4):
        inp_val = inputs[i]
        out_val = golden[i]
        inp_str = format_value(inp_val)
        out_str = format_value(out_val)
        print(f"{i:<8} | {inp_str:<32} | {out_str:<32}")
    print(f"{'='*130}\n")


def print_binary_api_table(api_name, type_name, special_names, inputs_a, inputs_b, golden):
    print(f"\n{'='*138}")
    print(f"API: {api_name} | Data Type: {type_name} | Gen Golden")
    print(f"{'='*138}")
    print(f"{'type':<8} | {'src0':<32} | {'src1':<32} | {'dst (Numpy-Golden)':<32}")
    print(f"{'-'*138}")
    for i in range(len(golden) // 4):
        inp_a = inputs_a[i]
        inp_b = inputs_b[i]
        out_val = golden[i]
        inp_a_str = format_value(inp_a)
        inp_b_str = format_value(inp_b)
        out_str = format_value(out_val)
        print(f"{i:<8} | {inp_a_str:<32} | {inp_b_str:<32} | {out_str:<32}")
    print(f"{'='*138}\n")
