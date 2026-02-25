#!/usr/bin/env python3
"""
Verify block_reduce_max output against golden data
"""

import numpy as np
import argparse
import os

relative_tol = 1e-6
absolute_tol = 1e-9
error_tol = 1e-4


def verify_result(scenarioNum, output, golden):
    if scenarioNum <= 2:
        output_type = np.float16
    else:
        output_type = np.float32
    output = np.fromfile(output, dtype=output_type)
    golden = np.fromfile(golden, dtype=output_type)
    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=relative_tol,
                                           atol=absolute_tol,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
            (real_index, golden_data, output_data,
             abs(output_data - golden_data) / golden_data))
        if index == 100:
            break
    error_ratio = float(different_element_indexes.size) / golden.size
    return error_ratio <= error_tol

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5))
    parser.add_argument('output', type=str)
    parser.add_argument('golden', type=str)
    args = parser.parse_args()
    try:
        res = verify_result(args.scenarioNum, args.output, args.golden)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
