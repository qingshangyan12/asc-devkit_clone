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
API_ID_ADDS = 3

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "adds",
}

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


def format_value(val, np_type):
    if np_type == np.float16:
        return f"{float(val):.6f}"
    return str(val)


def get_special_names(data_type):
    names = ['min', 'max', '+0', '-0']
    if np.issubdtype(data_type, np.floating):
        names.extend(['NaN', '+INF', '-INF'])
    return names


def verify_single_api(output_type, input_x, api_name="exp"):
    np_type = get_type_from_id(output_type)
    type_name = np_type.__name__
    
    output_non_sat = np.fromfile("./output/output_non_sat.bin", dtype=np_type).reshape(-1)
    output_sat = np.fromfile("./output/output_sat.bin", dtype=np_type).reshape(-1)
    golden = np.fromfile("./output/golden.bin", dtype=np_type).reshape(-1)
    input_x = np.fromfile(input_x, dtype=np_type).reshape(-1)
    
    special_names = get_special_names(np_type)
    
    print(f"\n{'='*116}")
    print(f"API: {api_name} | Data Type: {np_type} | Verification Results")
    print(f"{'='*116}")
    print(f"{'type':<8} | {'src':<26} | {'dst (API Output, Non-Sat)':<26} | {'dst (API Output, Saturation)':<26}")
    print(f"{'-'*116}")
    
    valid_count = 0
    for i in range(len(golden)):
        inp_val = input_x[i]
        actual_non_sat = output_non_sat[i]
        actual_sat = output_sat[i]
        golden_val = golden[i]
        
        if np.isnan(golden_val):
            is_match = np.isnan(actual_non_sat)
        elif np.isinf(golden_val):
            is_match = np.isinf(actual_non_sat) and np.sign(actual_non_sat) == np.sign(golden_val)
        else:
            is_match = np.isclose(actual_non_sat, golden_val, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
        
        if is_match:
            valid_count += 1
        
        if i < len(special_names):
            name = special_names[i]
            inp_str = format_value(inp_val, np_type)
            non_sat_str = format_value(actual_non_sat, np_type)
            sat_str = format_value(actual_sat, np_type)
            print(f"{type_name:<8} | {name:<6}{inp_str:<20} | {non_sat_str:<26} | {sat_str:<26}")
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*116}")
    print(f"Total: {len(golden)}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*116}\n")
    
    return error_ratio <= ERROR_TOL


def verify_binary_api(output_type, input_x, input_y, api_name="mul"):
    np_type = get_type_from_id(output_type)
    type_name = np_type.__name__
    
    output_non_sat = np.fromfile("./output/output_non_sat.bin", dtype=np_type).reshape(-1)
    output_sat = np.fromfile("./output/output_sat.bin", dtype=np_type).reshape(-1)
    golden = np.fromfile("./output/golden.bin", dtype=np_type).reshape(-1)
    input_x = np.fromfile(input_x, dtype=np_type).reshape(-1)
    input_y = np.fromfile(input_y, dtype=np_type).reshape(-1)
    
    special_names = get_special_names(np_type)
    num_special = len(special_names)
    
    print(f"\n{'='*124}")
    print(f"API: {api_name} | Data Type: {np_type} | Verification Results")
    print(f"{'='*124}")
    print(f"{'type':<8} | {'src0':<26} | {'src1':<26} | {'dst (API Output, Non-Sat)':<26} | {'dst (API Output, Saturation)':<26}")
    print(f"{'-'*124}")
    
    valid_count = 0
    printed_count = 0
    for idx in range(len(golden)):
        inp_a = input_x[idx]
        inp_b = input_y[idx]
        actual_non_sat = output_non_sat[idx]
        actual_sat = output_sat[idx]
        golden_val = golden[idx]
        
        if np.isnan(golden_val):
            is_match = np.isnan(actual_non_sat)
        elif np.isinf(golden_val):
            is_match = (np.isinf(actual_non_sat) and np.sign(actual_non_sat) == np.sign(golden_val)) or \
                       (actual_non_sat == 0 and golden_val == np.inf)
        else:
            is_match = np.isclose(actual_non_sat, golden_val, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
        
        if is_match:
            valid_count += 1
        
        if idx < num_special * num_special:
            i = idx // num_special
            j = idx % num_special
            name_a = special_names[i]
            name_b = special_names[j]
            inp_a_str = format_value(inp_a, np_type)
            inp_b_str = format_value(inp_b, np_type)
            non_sat_str = format_value(actual_non_sat, np_type)
            sat_str = format_value(actual_sat, np_type)
            print(f"{type_name:<8} | {name_a:<6}{inp_a_str:<20} | {name_b:<6}{inp_b_str:<20} | {non_sat_str:<26} | {sat_str:<26}")
            printed_count += 1
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*124}")
    print(f"Total: {len(golden)}, Unique: {printed_count}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*124}\n")
    
    return error_ratio <= ERROR_TOL


def verify_result(output_type, api_id, api_type=1, input_x=None, input_y=None):
    api_name = API_NAME_MAP.get(api_id, "unknown")
    try:
        if api_id == API_ID_EXP:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x)
        elif api_id == API_ID_MUL:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_ADDS:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
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
    parser.add_argument('-api_id', type=int, default=1, choices=[1, 2, 3],
                        help="API ID: 1=exp, 2=mul, 3=adds")
    parser.add_argument('-api_type', type=int, default=1, choices=[1, 2, 3],
                        help="API type: 1=Unary, 2=Binary, 3=BinaryScalar")
    parser.add_argument('-output_type', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Output type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-input_x', type=str, default="./input/input_x.bin")
    parser.add_argument('-input_y', type=str, required=False, default="./input/input_y.bin")
    args = parser.parse_args()
    
    res = verify_result(args.output_type, args.api_id, args.api_type, args.input_x, args.input_y)
    if not res:
        sys.exit(1)
