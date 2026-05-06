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


import numpy as np
import os
import sys

RELATIVE_TOL = 0.0001
ABSOLUTE_TOL = 0.0001

def verify_result(output_file, golden_file):
    output_exists = os.path.exists(output_file)
    golden_exists = os.path.exists(golden_file)

    if not output_exists and not golden_exists:
        print("[INFO] No output files to verify, skip.")
        return True

    if not output_exists or not golden_exists:
        print(f"[ERROR] Missing file: output_exists={output_exists}, golden_exists={golden_exists}")
        return False

    output = np.fromfile(output_file, dtype=np.float16)
    golden = np.fromfile(golden_file, dtype=np.float16)

    if output.shape != golden.shape:
        print(f"[ERROR] Shape mismatch: output {output.shape}, golden {golden.shape}")
        return False

    diff = np.abs(output - golden)
    mean_diff = np.mean(diff)

    error_ratio = mean_diff / np.mean(np.abs(golden))
    print(f"error ratio: {error_ratio:.4f}, tolerance: {ABSOLUTE_TOL}")

    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=RELATIVE_TOL,
                                           atol=ABSOLUTE_TOL,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    if len(different_element_indexes) > 0:
        for index in range(len(different_element_indexes)):
            real_index = different_element_indexes[index]
            golden_data = golden[real_index]
            output_data = output[real_index]
            print(
                "data index: %06d, expected: %-.9f, actual: %-.9f" %
                (real_index, golden_data, output_data))
            if index == 100:
                break
        return False

    print("test pass!")
    return True

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 verify_result.py <output_file> <golden_file>")
        sys.exit(1)

    output_file = sys.argv[1]
    golden_file = sys.argv[2]

    if not verify_result(output_file, golden_file):
        sys.exit(1)
