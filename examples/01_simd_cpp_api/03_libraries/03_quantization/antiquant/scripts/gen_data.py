#!/usr/bin/python3
# coding=utf-8

import os
import sys
import numpy as np


def gen_golden_data_simple(npu_arch):
    np.random.seed(10)

    N = 128
    K = 8
    offsetLength = 1
    groupSize = K // offsetLength

    src = np.random.uniform(-10, 10, [8, 128]).astype(np.int8)
    offset = np.random.uniform(-10, 10, [1, 128]).astype(np.float16)
    scale = np.random.uniform(-10, 10, [1, 128]).astype(np.float16)

    if npu_arch == "dav-2201":
        dst = np.zeros([8, 128]).astype(np.float32)
        for i in range(0, K):
            for j in range(0, N):
                dst[i][j] = scale[i // groupSize][j] * (
                    src[i][j] + offset[i // groupSize][j]
                )
        dst = dst.astype(np.float16)
    elif npu_arch == "dav-3510":
        src_fp32 = src.astype(np.float32)
        offset_fp32 = offset.astype(np.float32)
        scale_fp32 = scale.astype(np.float32)
        dst = np.zeros([8, 128]).astype(np.float32)
        for i in range(0, K):
            for j in range(0, N):
                dst[i][j] = scale_fp32[i // groupSize][j] * (
                    src_fp32[i][j] + offset_fp32[i // groupSize][j]
                )
        dst = dst.astype(np.float16)
    else:
        raise ValueError(f"Unsupported NPU architecture: {npu_arch}")

    os.makedirs("input", exist_ok=True)
    src.tofile("./input/input_src.bin")
    offset.tofile("./input/input_offset.bin")
    scale.tofile("./input/input_scale.bin")
    os.makedirs("output", exist_ok=True)
    dst.tofile("./output/golden.bin")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法：python3 gen_data.py <npu_arch>")
        sys.exit(1)

    npu_arch = sys.argv[1]
    gen_golden_data_simple(npu_arch)