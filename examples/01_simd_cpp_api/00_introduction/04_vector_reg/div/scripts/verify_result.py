#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# ----------------------------------------------------------------------------------------------------------


import sys
import numpy as np


def verify_result(output, golden, test_name):
    output = np.fromfile(output, dtype=np.int64).reshape(-1)
    golden = np.fromfile(golden, dtype=np.int64).reshape(-1)
    different_element_results = (output != golden)
    different_element_indexes = np.where(different_element_results)[0]
    if different_element_indexes.size == 0:
        print(f"[PASS] {test_name}")
        return True
    else:
        print(f"[FAIL] {test_name}: {different_element_indexes.size} mismatches")
        for index in range(len(different_element_indexes)):
            real_index = different_element_indexes[index]
            print("  index: %d, expected: %d, actual: %d" %
                  (real_index, golden[real_index], output[real_index]))
            if index >= 20:
                print("  ... (truncated)")
                break
        return False


if __name__ == '__main__':
    try:
        passed = verify_result(
            sys.argv[1], sys.argv[2], "INT64_MIN divisor")
        res2 = verify_result(
            sys.argv[3], sys.argv[4], "Normal divisor (3)")
        if not passed or not res2:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
