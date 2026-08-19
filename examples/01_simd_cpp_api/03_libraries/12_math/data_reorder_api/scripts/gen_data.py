#!/usr/bin/python3
# coding=utf-8

import argparse
import os

import numpy as np


def generate(scenario_num):
    # dav-3510 has a 256-byte vector register: 64 float elements per register.
    register_length = 64
    src0 = np.arange(register_length, dtype=np.float32)
    src1 = np.arange(1000, 1000 + register_length, dtype=np.float32)

    if scenario_num == 1:
        combined = np.stack((src0, src1), axis=1).reshape(-1)
        golden = combined
    else:
        combined = np.concatenate((src0, src1))
        dst0 = combined[0::2]
        dst1 = combined[1::2]
        golden = np.concatenate((dst0, dst1))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    src0.tofile("input/input_x.bin")
    src1.tofile("input/input_y.bin")
    golden.tofile("output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, required=True, choices=[1, 2])
    generate(parser.parse_args().scenarioNum)

