#!/usr/bin/env python3

import sys

import numpy as np

from hif8 import decode


def main(output_path, golden_path):
    output = decode(np.fromfile(output_path, dtype=np.uint8))
    golden = decode(np.fromfile(golden_path, dtype=np.uint8))
    if output.shape != golden.shape:
        raise ValueError(f"shape mismatch: {output.shape} != {golden.shape}")

    equal = np.isclose(output, golden, rtol=1e-3, atol=1e-3, equal_nan=True)
    error_ratio = 1.0 - float(np.count_nonzero(equal)) / equal.size
    print(f"error ratio: {error_ratio:.6f}")
    if error_ratio > 1e-3:
        indices = np.flatnonzero(~equal)[:10]
        for index in indices:
            print(f"index={index}, expected={golden[index]}, actual={output[index]}")
        raise ValueError("hifloat8 output verification failed")
    print("test pass!")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise SystemExit(f"usage: {sys.argv[0]} OUTPUT GOLDEN")
    main(sys.argv[1], sys.argv[2])
