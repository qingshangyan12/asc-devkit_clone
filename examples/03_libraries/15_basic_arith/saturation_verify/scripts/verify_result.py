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

import sys
import argparse
import numpy as np

API_ID_EXP = 1
API_ID_MUL = 2

RELATIVE_TOL = 1e-6
ABSOLUTE_TOL = 1e-9
ERROR_TOL = 1e-4


def get_type_from_id(output_type):
    type_map = {
        1: np.float16,
        2: np.float32,
        3: np.int16,
        4: np.int32,
    }
    return type_map.get(output_type, np.float16)


def get_special_names(data_type):
    names = ['min', 'max', '+0', '-0']
    if np.issubdtype(data_type, np.floating):
        names.extend(['NaN', '+INF', '-INF'])
    return names


def verify_single_api(output_type, output, golden, input_x):
    np_type = get_type_from_id(output_type)
    output = np.fromfile(output, dtype=np_type).reshape(-1)
    golden = np.fromfile(golden, dtype=np_type).reshape(-1)
    input_x = np.fromfile(input_x, dtype=np_type).reshape(-1)
    
    special_names = get_special_names(np_type)
    
    print(f"\n{'='*80}")
    print(f"Single API (exp) Verification Results - Non-Saturation Mode")
    print(f"{'='*80}")
    print(f"{'Input':<20} | {'Golden':<20} | {'Actual':<20} | {'Match':<6}")
    print(f"{'-'*75}")
    
    valid_count = 0
    for i in range(len(golden)):
        inp_val = input_x[i]
        golden_val = golden[i]
        actual_val = output[i]
        
        if np.isnan(golden_val):
            is_match = np.isnan(actual_val)
        elif np.isinf(golden_val):
            is_match = np.isinf(actual_val) and np.sign(actual_val) == np.sign(golden_val)
        else:
            is_match = np.isclose(actual_val, golden_val, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
        
        if is_match:
            valid_count += 1
        
        if i < len(special_names):
            name = special_names[i]
            print(f"[{name:<5}] {str(inp_val):<12} | {str(golden_val):<18} | {str(actual_val):<18} | {'YES' if is_match else 'NO'}")
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*75}")
    print(f"Total: {len(golden)}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*80}\n")
    
    return error_ratio <= ERROR_TOL


def verify_binary_api(output_type, output, golden, input_x, input_y):
    np_type = get_type_from_id(output_type)
    output = np.fromfile(output, dtype=np_type).reshape(-1)
    golden = np.fromfile(golden, dtype=np_type).reshape(-1)
    input_x = np.fromfile(input_x, dtype=np_type).reshape(-1)
    input_y = np.fromfile(input_y, dtype=np_type).reshape(-1)
    
    special_names = get_special_names(np_type)
    num_special = len(special_names)
    
    print(f"\n{'='*100}")
    print(f"Binary API (mul) Verification Results - Non-Saturation Mode")
    print(f"{'='*100}")
    print(f"{'Input A':<15} | {'Input B':<15} | {'Golden':<20} | {'Actual':<20} | {'Match':<6}")
    print(f"{'-'*95}")
    
    valid_count = 0
    for idx in range(len(golden)):
        inp_a = input_x[idx]
        inp_b = input_y[idx]
        golden_val = golden[idx]
        actual_val = output[idx]
        
        if np.isnan(golden_val):
            is_match = np.isnan(actual_val)
        elif np.isinf(golden_val):
            is_match = np.isinf(actual_val) and np.sign(actual_val) == np.sign(golden_val)
        else:
            is_match = np.isclose(actual_val, golden_val, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
        
        if is_match:
            valid_count += 1
        
        if idx < num_special * num_special:
            i = idx // num_special
            j = idx % num_special
            name_a = special_names[i]
            name_b = special_names[j]
            print(f"[{name_a:<4}] {str(inp_a):<8} | [{name_b:<4}] {str(inp_b):<8} | {str(golden_val):<18} | {str(actual_val):<18} | {'YES' if is_match else 'NO'}")
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*95}")
    print(f"Total: {len(golden)}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*100}\n")
    
    return error_ratio <= ERROR_TOL


def verify_result(output_type, output, golden, api_id, input_x=None, input_y=None):
    try:
        if api_id == API_ID_EXP:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, output, golden, input_x)
        elif api_id == API_ID_MUL:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, output, golden, input_x, input_y)
        else:
            print(f"Unsupported API ID: {api_id}")
            return False
        
        if not res:
            raise ValueError("[ERROR] Result verification failed")
        else:
            print("Test PASSED!")
        return res
    except Exception as e:
        print(e)
        return False


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-output_type', type=int, default=1, choices=[1, 2, 3, 4])
    parser.add_argument('-output', type=str, default="./output/output.bin")
    parser.add_argument('-golden', type=str, default="./output/golden.bin")
    parser.add_argument('-api_id', type=int, default=1, choices=[1, 2],
                        help="API ID: 1=exp, 2=mul")
    parser.add_argument('-input_x', type=str, default="./input/input_x.bin")
    parser.add_argument('-input_y', type=str, required=False, default="./input/input_y.bin")
    args = parser.parse_args()
    
    res = verify_result(args.output_type, args.output, args.golden, args.api_id, args.input_x, args.input_y)
    if not res:
        sys.exit(1)
