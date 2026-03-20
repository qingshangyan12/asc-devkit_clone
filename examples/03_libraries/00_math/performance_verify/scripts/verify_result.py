#!/usr/bin/env python3
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

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "adds",
    4: "leakyrelu",
}

RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-5
ERROR_TOL = 1e-3


def get_type_from_id(output_type):
    type_map = {
        1: np.float16,
        2: np.float32,
        3: np.int16,
        4: np.int32,
    }
    return type_map.get(output_type, np.float16)


def verify_result(output_type, api_id, api_type=1):
    np_type = get_type_from_id(output_type)
    type_name = np_type.__name__
    api_name = API_NAME_MAP.get(api_id, "unknown")
    
    try:
        output = np.fromfile("./output/output.bin", dtype=np_type).reshape(-1)
        golden = np.fromfile("./output/golden.bin", dtype=np_type).reshape(-1)
        
        if len(output) != len(golden):
            print(f"[ERROR] Output length mismatch: output={len(output)}, golden={len(golden)}")
            return False
        
        valid_count = 0
        for i in range(len(golden)):
            actual = output[i]
            expected = golden[i]
            
            if np.isnan(expected):
                is_match = np.isnan(actual)
            elif np.isinf(expected):
                is_match = np.isinf(actual) and np.sign(actual) == np.sign(expected)
            else:
                is_match = np.isclose(actual, expected, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL)
            
            if is_match:
                valid_count += 1
        
        error_ratio = 1.0 - (valid_count / len(golden))
        
        print(f"\n{'='*80}")
        print(f"Verification Result | API: {api_name} | Type: {type_name}")
        print(f"{'='*80}")
        print(f"Total elements: {len(golden)}, Matched: {valid_count}, Error Ratio: {error_ratio:.6f}")
        
        if error_ratio <= ERROR_TOL:
            print("Test PASSED!")
            return True
        else:
            print("Test FAILED!")
            return False
            
    except Exception as e:
        print(f"[ERROR] Verification failed: {e}")
        return False


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-api_id', type=int, default=1, choices=[1, 2, 3, 4],
                        help="API ID: 1=exp, 2=mul, 3=adds, 4=leakyrelu")
    parser.add_argument('-api_type', type=int, default=1, choices=[1, 2, 3],
                        help="API type: 1=Unary, 2=Binary, 3=BinaryScalar")
    parser.add_argument('-output_type', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Output type: 1=float16, 2=float32, 3=int16, 4=int32")
    args = parser.parse_args()
    
    res = verify_result(args.output_type, args.api_id, args.api_type)
    if not res:
        sys.exit(1)
