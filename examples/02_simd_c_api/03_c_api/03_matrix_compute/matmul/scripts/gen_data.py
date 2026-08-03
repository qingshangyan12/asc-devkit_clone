#!/usr/bin/env python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
from pathlib import Path
import random
import struct


M, K, N = 30, 40, 70
SEED = 20260803


def to_float32(value):
    return struct.unpack("<f", struct.pack("<f", value))[0]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--case", choices=("random", "ones", "zero_bias"), default="random"
    )
    args = parser.parse_args()
    rng = random.Random(SEED)
    if args.case == "ones":
        src0 = [1.0] * (M * K)
        src1 = [1.0] * (K * N)
        weight = [1.0] * N
    else:
        src0 = [to_float32(rng.uniform(-100.0, 100.0)) for _ in range(M * K)]
        src1 = [to_float32(rng.uniform(-100.0, 100.0)) for _ in range(K * N)]
        weight = [to_float32(rng.uniform(-100.0, 100.0)) for _ in range(N)]
        if args.case == "zero_bias":
            weight = [0.0] * N
    golden = []
    for row in range(M):
        for column in range(N):
            value = sum(
                src0[row * K + inner] * src1[inner * N + column] for inner in range(K)
            )
            golden.append(to_float32(value + weight[column]))

    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)
    (input_dir / "src0.bin").write_bytes(struct.pack(f"<{len(src0)}f", *src0))
    (input_dir / "src1.bin").write_bytes(struct.pack(f"<{len(src1)}f", *src1))
    (input_dir / "weight.bin").write_bytes(struct.pack(f"<{len(weight)}f", *weight))
    (output_dir / "golden.bin").write_bytes(struct.pack(f"<{len(golden)}f", *golden))


if __name__ == "__main__":
    main()
