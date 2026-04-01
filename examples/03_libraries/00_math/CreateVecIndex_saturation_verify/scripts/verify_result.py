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

import sys
import argparse
import numpy as np

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
    if np_type == np.float16:
        return f"{float(val):.6f}"
    return str(val)


def verify_createvecindex(output_type):
    np_type = get_type_from_id(output_type)
    type_name = np_type.__name__
    
    output_non_sat = np.fromfile("../output/output_non_sat.bin", dtype=np_type).reshape(-1)
    output_sat = np.fromfile("../output/output_sat.bin", dtype=np_type).reshape(-1)
    golden = np.fromfile("../output/golden.bin", dtype=np_type).reshape(-1)
    
    print(f"\n{'='*100}")
    print(f"API: CreateVecIndex | Data Type: {np_type} | Verification Results")
    print(f"{'='*100}")
    print(f"{'type':<10} | {'index':<10} | {'golden':<20} | {'dst (Non-Sat)':<20} | {'dst (Saturation)':<20}")
    print(f"{'-'*100}")
    
    valid_count = 0
    for i in range(len(golden)):
        golden_val = golden[i]
        actual_non_sat = output_non_sat[i]
        actual_sat = output_sat[i]
        
        if golden_val == actual_non_sat:
            valid_count += 1
        
        if i < 20:
            golden_str = format_value(golden_val, np_type)
            non_sat_str = format_value(actual_non_sat, np_type)
            sat_str = format_value(actual_sat, np_type)
            print(f"{type_name:<10} | {i:<10} | {golden_str:<20} | {non_sat_str:<20} | {sat_str:<20}")
    
    error_ratio = 1.0 - (valid_count / len(golden))
    print(f"{'-'*100}")
    print(f"Total: {len(golden)}, Matched: {valid_count}, Error Ratio: {error_ratio:.4f}")
    print(f"{'='*100}\n")
    
    return error_ratio <= ERROR_TOL


def verify_result(output_type):
    try:
        res = verify_createvecindex(output_type)
        
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
    parser.add_argument('-output_type', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Output type: 1=float16, 2=float32, 3=int16, 4=int32")
    args = parser.parse_args()
    
    res = verify_result(args.output_type)
    if not res:
        sys.exit(1)