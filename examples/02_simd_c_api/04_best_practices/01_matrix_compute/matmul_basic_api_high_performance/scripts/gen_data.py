#!/usr/bin/env python3
# coding=utf-8

import os

import numpy as np


M = 1024
K = 1024
N = 1024


def main():
    rng = np.random.default_rng(20260806)
    matrix_a = rng.uniform(-0.1, 0.1, (M, K)).astype(np.float16)
    matrix_b = rng.uniform(-0.1, 0.1, (K, N)).astype(np.float16)
    golden = np.matmul(matrix_a.astype(np.float32), matrix_b.astype(np.float32))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    matrix_a.tofile("input/x1_gm.bin")
    matrix_b.transpose().copy().tofile("input/x2_gm.bin")
    golden.astype(np.float32).tofile("output/golden.bin")


if __name__ == "__main__":
    main()
