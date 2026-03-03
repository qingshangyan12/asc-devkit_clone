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

import numpy as np


def verify_result_file(output_path,
                       golden_path,
                       dtype,
                       relative_tol,
                       absolute_tol,
                       error_tol,
                       max_report=100,
                       print_arrays=False):
    """Compare output and golden binary files and return whether they match.

    Args:
        output_path: Path to output binary.
        golden_path: Path to golden binary.
        dtype: Numpy dtype for both files.
        relative_tol: Relative tolerance for np.isclose.
        absolute_tol: Absolute tolerance for np.isclose.
        error_tol: Maximum allowed ratio of mismatched elements.
        max_report: Maximum mismatched elements to print.
        print_arrays: Whether to print full arrays for debugging.
    """
    output = np.fromfile(output_path, dtype=dtype).reshape(-1)
    golden = np.fromfile(golden_path, dtype=dtype).reshape(-1)

    if output.size != golden.size:
        print(f"size mismatch: output={output.size}, golden={golden.size}")
        return False

    is_close = np.isclose(output, golden, rtol=relative_tol, atol=absolute_tol, equal_nan=True)
    different_element_indexes = np.where(is_close == False)[0]
    for index in range(min(len(different_element_indexes), max_report + 1)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        denominator = golden_data if golden_data != 0 else 1.0
        print("data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
              (real_index, golden_data, output_data, abs(output_data - golden_data) / denominator))

    if print_arrays:
        print("golden_data : ", golden)
        print("output : ", output)

    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, error_tol))
    return error_ratio <= error_tol
