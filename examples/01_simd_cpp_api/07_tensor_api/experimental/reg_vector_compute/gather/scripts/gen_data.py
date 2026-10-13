import argparse
from pathlib import Path

import numpy as np


def gen_golden_data_gather(case):
    rng = np.random.default_rng(2026)
    total_length = 128
    source = rng.uniform(0, 1, total_length).astype(np.float16)
    if case == "random":
        indexes = rng.integers(0, total_length, total_length, dtype=np.uint16)
        indexes[:2] = [0, total_length - 1]
    elif case == "identity":
        indexes = np.arange(total_length, dtype=np.uint16)
    elif case == "reverse":
        indexes = np.arange(total_length - 1, -1, -1, dtype=np.uint16)
    else:
        indexes = np.full(total_length, total_length - 1, dtype=np.uint16)
    Path("input").mkdir(exist_ok=True)
    Path("output").mkdir(exist_ok=True)
    source.tofile("input/input_x.bin")
    indexes.tofile("input/input_y.bin")
    source[indexes].tofile("output/golden.bin")
    print(f"Generated {case}: half/uint16, {total_length} elements")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--case", choices=["random", "identity", "reverse", "repeated"], default="random")
    gen_golden_data_gather(parser.parse_args().case)
