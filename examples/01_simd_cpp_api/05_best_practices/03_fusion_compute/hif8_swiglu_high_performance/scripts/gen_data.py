#!/usr/bin/env python3

import os

import numpy as np

from hif8 import decode, encode


TOTAL_LENGTH = 2048


def main():
    rng = np.random.default_rng(2026)
    # Keep the source range away from overflow and quantization boundaries.
    g_float = rng.uniform(-3.0, 3.0, TOTAL_LENGTH).astype(np.float32)
    u_float = rng.uniform(-3.0, 3.0, TOTAL_LENGTH).astype(np.float32)
    g_hif8 = encode(g_float)
    u_hif8 = encode(u_float)

    g_decoded = decode(g_hif8)
    u_decoded = decode(u_hif8)
    swish = g_decoded / (1.0 + np.exp(-g_decoded))
    golden = encode(swish * u_decoded)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    g_hif8.tofile("input/g.bin")
    u_hif8.tofile("input/u.bin")
    golden.tofile("output/golden.bin")


if __name__ == "__main__":
    main()
