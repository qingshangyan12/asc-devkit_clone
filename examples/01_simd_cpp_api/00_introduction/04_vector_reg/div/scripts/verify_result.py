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


import sys
import numpy as np


def verify_result(output, golden):
    output = np.fromfile(output, dtype=np.int64).reshape(-1)
    golden = np.fromfile(golden, dtype=np.int64).reshape(-1)
    print("output:", output[0:10], output[-10:])
    print("golden:", golden[0:10], golden[-10:])
    different_element_results = (output != golden)
    different_element_indexes = np.where(different_element_results)[0]
    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %d, actual: %d" %
            (real_index, golden_data, output_data))
        if index == 100:
            break
    return different_element_indexes.size == 0


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
