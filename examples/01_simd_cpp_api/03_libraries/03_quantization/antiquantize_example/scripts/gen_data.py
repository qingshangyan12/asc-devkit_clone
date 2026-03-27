#!/usr/bin/env python3
import numpy as np
import os


def generate_data():
    input_dir = "./input"
    output_dir = "./output"

    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)

    src_shape = [8, 128]
    offset_shape = [1, 128]
    scale_shape = [1, 128]

    src = np.random.randint(-128, 127, src_shape).astype(np.int8)
    offset = np.random.randn(*offset_shape).astype(np.float16)
    scale = np.random.randn(*scale_shape).astype(np.float16)

    src.tofile(os.path.join(input_dir, "input_src.bin"))
    offset.tofile(os.path.join(input_dir, "input_offset.bin"))
    scale.tofile(os.path.join(input_dir, "input_scale.bin"))

    src_fp32 = src.astype(np.float32)
    offset_fp32 = offset.astype(np.float32)
    scale_fp32 = scale.astype(np.float32)

    dst = scale_fp32 * (src_fp32 + offset_fp32)
    dst_half = dst.astype(np.float16)

    dst_half.tofile(os.path.join(output_dir, "golden.bin"))

    print(f"Generated input data:")
    print(f"  src shape: {src.shape}, dtype: {src.dtype}")
    print(f"  offset shape: {offset.shape}, dtype: {offset.dtype}")
    print(f"  scale shape: {scale.shape}, dtype: {scale.dtype}")
    print(f"  golden shape: {dst_half.shape}, dtype: {dst_half.dtype}")


if __name__ == "__main__":
    generate_data()
