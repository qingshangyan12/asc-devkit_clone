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
import sys
import numpy as np


def verify_result(output_file, golden_file, input_type=1):
    if not os.path.exists(output_file):
        print(f"Error: Output file {output_file} not found!")
        return False
    if not os.path.exists(golden_file):
        print(f"Error: Golden file {golden_file} not found!")
        return False

    input_type_map = {
        1: np.float16,
        2: np.float32,
        3: np.int16,
        4: np.int32
    }
    dtype = input_type_map.get(input_type, np.float16)
    
    output_data = np.fromfile(output_file, dtype=dtype)
    golden_data = np.fromfile(golden_file, dtype=dtype)
    
    if output_data.shape != golden_data.shape:
        print(f"Error: Shape mismatch! Output: {output_data.shape}, Golden: {golden_data.shape}")
        return False
    
    if dtype == np.float16:
        eps = 1e-3
    elif dtype == np.float32:
        eps = 1e-4
    else:
        eps = 1
    
    diff = np.abs(output_data - golden_data)
    max_diff = np.max(diff)
    mean_diff = np.mean(diff)
    
    print(f"Max diff: {max_diff}")
    print(f"Mean diff: {mean_diff}")
    
    if np.any(diff > eps):
        print("Verification FAILED!")
        return False
    
    print("Verification PASSED!")
    return True


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 verify_result.py <output_file> <golden_file> [input_type]")
        sys.exit(1)
    
    output_file = sys.argv[1]
    golden_file = sys.argv[2]
    input_type = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    
    result = verify_result(output_file, golden_file, input_type)
    sys.exit(0 if result else 1)
