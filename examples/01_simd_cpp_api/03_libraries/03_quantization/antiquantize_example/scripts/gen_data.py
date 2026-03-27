#!/usr/bin/env python3
import numpy as np
import os


def generate_data():
    input_dir = "./input"
    output_dir = "./output"

    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)

    K = 8
    N = 128
    offsetLength = 1
    groupSize = K // offsetLength

    src_shape = [8, 128]
    offset_shape = [1, 128]
    scale_shape = [1, 128]

    src = np.random.uniform(-10, 10, [8, 128]).astype(np.int8)
    offset = np.random.uniform(-10, 10, [1, 128]).astype(np.float16)
    scale = np.random.uniform(-10, 10, [1, 128]).astype(np.float16)

    src.tofile(os.path.join(input_dir, "input_src.bin"))
    offset.tofile(os.path.join(input_dir, "input_offset.bin"))
    scale.tofile(os.path.join(input_dir, "input_scale.bin"))

    src_fp16 = src.astype(np.float16)
    dst = np.zeros([8, 128]).astype(np.float16)

    for i in range(0, K):
        for j in range(0, N):
            dst[i][j] = scale[i // groupSize][j] * (
                src_fp16[i][j] + offset[i // groupSize][j]
            )

    dst = dst.astype(np.float16)

    dst.tofile(os.path.join(output_dir, "golden.bin"))

    print(f"Generated input data:")
    print(f"  src shape: {src.shape}, dtype: {src.dtype}")
    print(f"  offset shape: {offset.shape}, dtype: {offset.dtype}")
    print(f"  scale shape: {scale.shape}, dtype: {scale.dtype}")
    print(f"  golden shape: {dst.shape}, dtype: {dst.dtype}")


if __name__ == "__main__":
    generate_data()
