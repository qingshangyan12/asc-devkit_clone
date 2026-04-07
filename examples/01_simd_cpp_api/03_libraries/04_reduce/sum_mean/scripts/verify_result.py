#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
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
import argparse


def verify_result(output_file, golden_file, dtype=np.float32, rtol=1e-4, atol=1e-4):
    """
    验证输出结果与真值是否一致
    """
    if not os.path.exists(output_file):
        print("Error: output file not found: {}".format(output_file))
        return False
    
    if not os.path.exists(golden_file):
        print("Error: golden file not found: {}".format(golden_file))
        return False
    
    output_data = np.fromfile(output_file, dtype=dtype)
    golden_data = np.fromfile(golden_file, dtype=dtype)
    
    print("Output shape: {}".format(output_data.shape))
    print("Golden shape: {}".format(golden_data.shape))
    
    if output_data.shape != golden_data.shape:
        print("Error: shape mismatch!")
        return False
    
    diff = np.abs(output_data - golden_data)
    max_diff = np.max(diff)
    mean_diff = np.mean(diff)
    
    print("Max difference: {}".format(max_diff))
    print("Mean difference: {}".format(mean_diff))
    
    if np.allclose(output_data, golden_data, rtol=rtol, atol=atol):
        print("test pass!")
        return True
    else:
        mismatch = ~np.isclose(output_data, golden_data, rtol=rtol, atol=atol)
        mismatch_indices = np.where(mismatch)[0]
        print("Mismatch count: {}".format(len(mismatch_indices)))
        print("test failed!")
        return False


def main():
    parser = argparse.ArgumentParser(description="Verify output result")
    parser.add_argument("output_file", help="Output file path")
    parser.add_argument("golden_file", help="Golden file path")
    parser.add_argument("-scenarioNum", type=int, default=1, help="Scenario number")
    
    args = parser.parse_args()
    
    print("=== Verification ===")
    print("Scenario: {}".format(args.scenarioNum))
    
    result = verify_result(args.output_file, args.golden_file)
    sys.exit(0 if result else 1)


if __name__ == "__main__":
    main()
