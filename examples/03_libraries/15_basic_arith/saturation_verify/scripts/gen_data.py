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
import numpy as np

API_ID_EXP = 1
API_ID_MUL = 2

TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}

API_NAME_MAP = {
    1: "exp",
    2: "mul",
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


def compute_exp_golden(input_array):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.exp(input_array)
    return result.astype(input_array.dtype)


def compute_mul_golden(a, b):
    with np.errstate(divide='ignore', over='ignore', under='ignore', invalid='ignore'):
        result = np.multiply(a, b)
    return result.astype(a.dtype)


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


def print_single_api_table(api_name, type_name, special_names, inputs, golden):
    print(f"\n{'='*80}")
    print(f"API: {api_name} | Data Type: {type_name} | Single Input Special Values")
    print(f"{'='*80}")
    print(f"{'Input Value':<25} | {'API Output (Non-Sat)':<25}")
    print(f"{'-'*60}")
    for i, name in enumerate(special_names):
        inp_val = inputs[i]
        out_val = golden[i]
        print(f"[{name:<5}] {str(inp_val):<20} | {str(out_val):<25}")
    print(f"{'='*80}\n")


def print_binary_api_table(api_name, type_name, special_names, inputs_a, inputs_b, golden):
    print(f"\n{'='*80}")
    print(f"API: {api_name} | Data Type: {type_name} | Binary Input Combinations (Non-Sat Mode)")
    print(f"{'='*80}")
    print(f"{'Input A':<20} | {'Input B':<20} | {'A op B (Non-Sat)':<25}")
    print(f"{'-'*75}")
    for i, name_a in enumerate(special_names):
        for j, name_b in enumerate(special_names):
            idx = i * len(special_names) + j
            print(f"[{name_a:<5}] {str(inputs_a[idx]):<12} | [{name_b:<5}] {str(inputs_b[idx]):<12} | {str(golden[idx]):<25}")
    print(f"{'='*80}\n")


def gen_exp_data(input_type, input_size):
    data_type = TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    special_vals = construct_special_values(data_type)
    inputs = np.array(special_vals, dtype=data_type)
    golden = compute_exp_golden(inputs)
    
    save_bin(inputs, f"./input/input_x.bin")
    save_bin(golden, f"./output/golden.bin")
    
    special_names = ['min', 'max', '+0', '-0']
    if np.issubdtype(data_type, np.floating):
        special_names.extend(['NaN', '+INF', '-INF'])
    
    print_single_api_table("exp", type_name, special_names, inputs, golden)


def gen_mul_data(input_type, input_size):
    data_type = TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    inputs_a, inputs_b = cartesian_product_inputs(data_type)
    golden = compute_mul_golden(inputs_a, inputs_b)
    
    save_bin(inputs_a, f"./input/input_x.bin")
    save_bin(inputs_b, f"./input/input_y.bin")
    save_bin(golden, f"./output/golden.bin")
    
    special_names = ['min', 'max', '+0', '-0']
    if np.issubdtype(data_type, np.floating):
        special_names.extend(['NaN', '+INF', '-INF'])
    
    print_binary_api_table("mul", type_name, special_names, inputs_a, inputs_b, golden)


def main():
    parser = argparse.ArgumentParser(description="Generate golden data for saturation verify.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input data type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    parser.add_argument('-apiId', type=int, default=1, choices=[1, 2],
                        help="API ID: 1=exp, 2=mul")
    args = parser.parse_args()

    api_name = API_NAME_MAP.get(args.apiId, "unknown")
    type_name = str(TYPE_MAP[args.inputType].__name__)
    print(f"\nGenerating test data for: API={api_name}, Type={type_name}, Size={args.inputSize}")

    if args.apiId == API_ID_EXP:
        gen_exp_data(args.inputType, args.inputSize)
    elif args.apiId == API_ID_MUL:
        gen_mul_data(args.inputType, args.inputSize)
    else:
        raise ValueError(f"Unsupported API ID: {args.apiId}")

    print("\nData generation completed successfully!")


if __name__ == "__main__":
    main()
