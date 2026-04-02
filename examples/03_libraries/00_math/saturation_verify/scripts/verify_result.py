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
API_ID_LEAKYRELU = 4
API_ID_LN = 5
API_ID_ABS = 6
API_ID_RECIPROCAL = 7
API_ID_SQRT = 8
API_ID_RSQRT = 9
API_ID_RELU = 10
API_ID_ADD = 11
API_ID_SUB = 12
API_ID_DIV = 13
API_ID_MAX = 14
API_ID_MIN = 15
API_ID_MULS = 16
API_ID_MAXS = 17
API_ID_MINS = 18

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "adds",
    4: "leakyrelu",
    5: "ln",
    6: "abs",
    7: "reciprocal",
    8: "sqrt",
    9: "rsqrt",
    10: "relu",
    11: "add",
    12: "sub",
    13: "div",
    14: "max",
    15: "min",
    16: "muls",
    17: "maxs",
    18: "mins",
}

RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-6
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
    # if np_type == np.float16:
    #     return f"{float(val):.6f}"
    # return str(val)
    if np_type == np.float16:
        return f"{float(val):.8f}"
    return str(val)


def get_special_names(data_type):
    names = ['min', 'max', '+0', '-0']
    if np.issubdtype(data_type, np.floating):
        names.extend(['NaN', '+INF', '-INF'])
        # names.extend(['NaN', '+INF', '-INF', 'min_normal', 'neg_min_normal'])
    return names


def verify_single_api(output_type, input_x, api_name="exp"):
    np_type = get_type_from_id(output_type)
    type_name = np_type.__name__
    
    output_non_sat = np.fromfile("./output/output_non_sat.bin", dtype=np_type).reshape(-1)
    output_sat = np.fromfile("./output/output_sat.bin", dtype=np_type).reshape(-1)
    golden = np.fromfile("./output/golden.bin", dtype=np_type).reshape(-1)
    input_x = np.fromfile(input_x, dtype=np_type).reshape(-1)
    
    special_names = get_special_names(np_type)
    
    print(f"\n{'='*130}")
    print(f"API: {api_name} | Data Type: {np_type} | Verification Results")
    print(f"{'='*130}")
    print(f"{'type':<8} | {'src':<32} | {'dst (API Output, Non-Sat)':<32} | {'dst (API Output, Saturation)':<32}")
    print(f"{'-'*130}")
    
    valid_count = 0
    for i in range(len(golden)):
        inp_val = input_x[i]
        actual_non_sat = output_non_sat[i]
        actual_sat = output_sat[i]
        golden_val = golden[i]
        
        if np.isnan(golden_val):
            is_match = np.isnan(actual_non_sat)
        elif np.isinf(golden_val):
            is_match = np.isinf(actual_non_sat) and (np.sign(actual_non_sat) == np.sign(golden_val) or 
                        (golden_val == np.inf and actual_non_sat == -np.inf) or
                        (golden_val == -np.inf and actual_non_sat == np.inf))
        elif golden_val == 0 or golden_val == -0 or actual_non_sat == 0 or actual_non_sat == -0:
            is_match = golden_val == actual_non_sat or (np.sign(golden_val) == np.sign(actual_non_sat))
        else:
            is_match = np.isclose(actual_non_sat, golden_val, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
        
        if is_match:
            valid_count += 1
        
        if i < len(special_names):
            name = special_names[i]
            inp_str = format_value(inp_val, np_type)
            non_sat_str = format_value(actual_non_sat, np_type)
            sat_str = format_value(actual_sat, np_type)
            print(f"{type_name:<8} | {name:<14}{inp_str:<18} | {non_sat_str:<32} | {sat_str:<32}")
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*130}")
    print(f"Total: {len(golden)}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*130}\n")
    
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
    
    print(f"\n{'='*138}")
    print(f"API: {api_name} | Data Type: {np_type} | Verification Results")
    print(f"{'='*138}")
    print(f"{'type':<8} | {'src0':<32} | {'src1':<32} | {'dst (API Output, Non-Sat)':<32} | {'dst (API Output, Saturation)':<32}")
    print(f"{'-'*138}")
    
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
            print(f"{type_name:<8} | {name_a:<14}{inp_a_str:<18} | {name_b:<14}{inp_b_str:<18} | {non_sat_str:<32} | {sat_str:<32}")
            printed_count += 1
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*138}")
    print(f"Total: {len(golden)}, Unique: {printed_count}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*138}\n")
    
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
        elif api_id == API_ID_LEAKYRELU:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_LN:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x, api_name)
        elif api_id == API_ID_ABS:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x, api_name)
        elif api_id == API_ID_RECIPROCAL:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x, api_name)
        elif api_id == API_ID_SQRT:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x, api_name)
        elif api_id == API_ID_RSQRT:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x, api_name)
        elif api_id == API_ID_RELU:
            if input_x is None:
                input_x = "./input/input_x.bin"
            res = verify_single_api(output_type, input_x, api_name)
        elif api_id == API_ID_ADD:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_SUB:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_DIV:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_MAX:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_MIN:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_MULS:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_MAXS:
            if input_x is None:
                input_x = "./input/input_x.bin"
            if input_y is None:
                input_y = "./input/input_y.bin"
            res = verify_binary_api(output_type, input_x, input_y, api_name)
        elif api_id == API_ID_MINS:
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
    parser.add_argument('-api_id', type=int, default=1, choices=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18],
                        help="API ID: 1=exp, 2=mul, 3=adds, 4=leakyrelu, 5=ln, 6=abs, 7=reciprocal, 8=sqrt, 9=rsqrt, 10=relu, 11=add, 12=sub, 13=div, 14=max, 15=min, 16=muls, 17=maxs, 18=mins")
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
