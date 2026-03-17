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
    return special_names


def print_unary_api_table(api_name, type_name, special_names, inputs, golden):
    print(f"\n{'='*116}")
    print(f"API: {api_name} | Data Type: {type_name} | Gen Golden")
    print(f"{'='*116}")
    print(f"{'type':<8} | {'src':<26} | {'dst (Numpy-Golden)':<26}")
    print(f"{'-'*116}")
    for i, name in enumerate(special_names):
        inp_val = inputs[i]
        out_val = golden[i]
        print(f"{type_name:<8} | {name:<6}{str(inp_val):<20} | {str(out_val):<26}")
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
            print(f"{type_name:<8} | {name_a:<6}{str(inputs_a[idx]):<20} | {name_b:<6}{str(inputs_b[idx]):<20} | {str(out_val):<26}")
    print(f"{'='*124}\n")
