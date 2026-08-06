#!/usr/bin/env python3
# coding=utf-8

import sys

import numpy as np


RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-3
MAX_ERROR_RATIO = 1e-3


def verify_result(output_path, golden_path):
    output = np.fromfile(output_path, dtype=np.float32)
    golden = np.fromfile(golden_path, dtype=np.float32)
    if output.size != golden.size:
        raise ValueError(f"size mismatch: output={output.size}, golden={golden.size}")

    matched = np.isclose(output, golden, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL, equal_nan=True)
    mismatch_indices = np.flatnonzero(~matched)
    for index in mismatch_indices[:10]:
        print(f"index {index}: expected={golden[index]:.9f}, actual={output[index]:.9f}")

    error_ratio = mismatch_indices.size / golden.size
    print(f"error ratio: {error_ratio:.6f}, tolerance: {MAX_ERROR_RATIO:.6f}")
    return error_ratio <= MAX_ERROR_RATIO


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <output.bin> <golden.bin>")
        sys.exit(2)
    if not verify_result(sys.argv[1], sys.argv[2]):
        print("test failed")
        sys.exit(1)
    print("test pass!")
