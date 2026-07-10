#!/usr/bin/python3
# coding=utf-8
import argparse
import sys
import numpy as np


def verify_result(output, golden):
    output_data = np.fromfile(output, dtype=np.float16).astype(np.float32).reshape(-1)
    golden_data = np.fromfile(golden, dtype=np.float16).astype(np.float32).reshape(-1)
    diff = np.abs(output_data - golden_data)
    different_element_results = np.isclose(output_data, golden_data, rtol=1e-2, atol=1e-2, equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    for real_index in different_element_indexes[:100]:
        expected = golden_data[real_index]
        actual = output_data[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
            (real_index, expected, actual, abs(actual - expected) / max(abs(expected), 1e-10)))
    print(f"max diff: {diff.max() if diff.size else 0.0}")
    print(f"error ratio: {float(different_element_indexes.size) / golden_data.size if golden_data.size else 0.0:.4f}")
    return different_element_indexes.size == 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("output", type=str)
    parser.add_argument("golden", type=str)
    args = parser.parse_args()
    try:
        if not verify_result(args.output, args.golden):
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as err:
        print(err)
        sys.exit(1)
