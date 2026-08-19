#!/usr/bin/python3
# coding=utf-8

import argparse
import os

import numpy as np

def gen_golden_data_simple():
    register_length = 256
    src0 = np.arange(register_length, dtype=np.float32)
    src1 = np.arange(1000, 1000 + register_length, dtype=np.float32)
    combined = np.concatenate((src0, src1))
    dst0 = combined[0::2]
    dst1 = combined[1::2]

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    src0.tofile("input/input_src0.bin")
    src1.tofile("input/input_src1.bin")
    dst0.tofile("output/golden_dst0.bin")
    dst1.tofile("output/golden_dst1.bin")


if __name__ == "__main__":
    gen_golden_data_simple()

