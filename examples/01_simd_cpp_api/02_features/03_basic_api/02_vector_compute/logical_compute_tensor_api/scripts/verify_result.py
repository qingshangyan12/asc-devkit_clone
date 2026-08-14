#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.

import sys

import numpy as np


def main() -> None:
    if len(sys.argv) != 3:
        raise ValueError("Usage: verify_result.py <output> <golden>")
    actual = np.fromfile(sys.argv[1], dtype=np.int32)
    golden = np.fromfile(sys.argv[2], dtype=np.int32)
    if actual.shape != golden.shape or not np.array_equal(actual, golden):
        index = int(np.flatnonzero(actual != golden)[0])
        raise ValueError(f"mismatch at index {index}: actual={actual[index]}, golden={golden[index]}")
    print("test pass!")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error)
        sys.exit(1)
