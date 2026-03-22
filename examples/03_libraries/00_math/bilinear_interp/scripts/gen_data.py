#!/usr/bin/env python3
import numpy as np
import os

SRC_SIZE = 512
DST_SIZE = 128
OFFSET_SIZE = 128
WEIGHT_SIZE = 128

def generate_data():
    input_dir = "./input"
    output_dir = "./output"
    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)

    np.random.seed(123)

    src = np.random.rand(SRC_SIZE).astype(np.float16)
    src.tofile(os.path.join(input_dir, "input_src.bin"))

    weight = np.random.rand(WEIGHT_SIZE).astype(np.float16)
    weight.tofile(os.path.join(input_dir, "input_weight.bin"))

    offset = np.arange(OFFSET_SIZE, dtype=np.uint32)
    offset.tofile(os.path.join(input_dir, "input_offset.bin"))

    print("Data generation completed!")
    print(f"  src size: {SRC_SIZE}")
    print(f"  weight size: {WEIGHT_SIZE}")
    print(f"  offset size: {OFFSET_SIZE}")
    print(f"  dst size: {DST_SIZE}")

if __name__ == "__main__":
    generate_data()
