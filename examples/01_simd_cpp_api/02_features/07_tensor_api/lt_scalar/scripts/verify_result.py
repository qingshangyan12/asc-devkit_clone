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
import numpy as np


relative_tol = 1e-3
absolute_tol = 1e-5
error_tol = 1e-3


def verify_result(output, golden):
    output = np.fromfile(output, dtype=np.uint8).reshape(-1)
    golden = np.fromfile(golden, dtype=np.uint8).reshape(-1)
    # Comparison results are bit-packed, need to unpack
    # Each byte contains 8 comparison results
    # Use bitorder='little' to match hardware bit order
    output_bits = np.unpackbits(output, bitorder='little')
    golden_bits = np.unpackbits(golden, bitorder='little')
    # Only compare the first TOTAL_LENGTH bits
    output_bits = output_bits[:512]
    golden_bits = golden_bits[:512]
    different_element_results = np.not_equal(output_bits, golden_bits)
    different_element_indexes = np.where(different_element_results)[0]
    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index] if not True else golden_bits[real_index]
        output_data = output[real_index] if not True else output_bits[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
            (real_index, golden_data, output_data,
             abs(output_data - golden_data) / (golden_data + 1e-10)))
        if index == 100:
            break
    error_ratio = float(different_element_indexes.size) / (512 if True else golden.size)
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, error_tol))
    return error_ratio == 0


if __name__ == '__main__':
    try:
        res = verify_result(sys.argv[1], sys.argv[2])
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
