#!/usr/bin/python3
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
import json
from pathlib import Path

import numpy as np


M = 30
K = 40
N = 70
SUPPORTED_CASES = (
    "uniform",
    "normal",
    "boundary",
    "bias_only",
    "non_aligned_tail",
    "zeros",
)


def _make_inputs(case_name: str, seed: int):
    rng = np.random.default_rng(seed)

    if case_name == "uniform":
        src0 = rng.uniform(-100.0, 100.0, size=(M, K))
        src1 = rng.uniform(-100.0, 100.0, size=(K, N))
        weight = rng.uniform(-100.0, 100.0, size=(1, N))
    elif case_name == "normal":
        src0 = np.clip(rng.normal(0.0, 30.0, size=(M, K)), -100.0, 100.0)
        src1 = np.clip(rng.normal(0.0, 30.0, size=(K, N)), -100.0, 100.0)
        weight = np.clip(rng.normal(0.0, 30.0, size=(1, N)), -100.0, 100.0)
    elif case_name == "boundary":
        src0 = np.resize(np.array([-100.0, 100.0, -1.0, 1.0, 0.0], dtype=np.float64), M * K).reshape(M, K)
        src1 = np.resize(np.array([100.0, -100.0, 0.5, -0.5, 0.0], dtype=np.float64), K * N).reshape(K, N)
        weight = np.linspace(-100.0, 100.0, N, dtype=np.float64).reshape(1, N)
    elif case_name == "bias_only":
        src0 = np.zeros((M, K), dtype=np.float64)
        src1 = np.zeros((K, N), dtype=np.float64)
        weight = np.linspace(-100.0, 100.0, N, dtype=np.float64).reshape(1, N)
    elif case_name == "non_aligned_tail":
        # Touch the final valid M/K/N coordinates directly. This catches lost or mis-addressed tail
        # fractals while keeping all padded Local Memory coordinates outside the GM input tensors.
        src0 = np.zeros((M, K), dtype=np.float64)
        src1 = np.zeros((K, N), dtype=np.float64)
        weight = np.zeros((1, N), dtype=np.float64)
        src0[M - 1, K - 1] = 3.25
        src1[K - 1, N - 1] = -2.0
        weight[0, N - 1] = 1.5
    elif case_name == "zeros":
        src0 = np.zeros((M, K), dtype=np.float64)
        src1 = np.zeros((K, N), dtype=np.float64)
        weight = np.zeros((1, N), dtype=np.float64)
    else:
        raise ValueError(f"unsupported case: {case_name}")

    return src0.astype(np.float32), src1.astype(np.float32), weight.astype(np.float32)


def generate(case_name: str, seed: int, root: Path):
    src0, src1, weight = _make_inputs(case_name, seed)

    # Use float64 on the CPU as the high-precision reference, then round once to the operator output dtype.
    golden = (
        np.matmul(src0.astype(np.float64), src1.astype(np.float64))
        + weight.astype(np.float64)
    ).astype(np.float32)

    input_dir = root / "input"
    output_dir = root / "output"
    input_dir.mkdir(parents=True, exist_ok=True)
    output_dir.mkdir(parents=True, exist_ok=True)

    src0.tofile(input_dir / "src0.bin")
    src1.tofile(input_dir / "src1.bin")
    weight.tofile(input_dir / "weight.bin")
    golden.tofile(output_dir / "golden.bin")

    manifest = {
        "case": case_name,
        "seed": seed,
        "dtype": "float32",
        "src0_shape": [M, K],
        "src1_shape": [K, N],
        "weight_shape": [1, N],
        "dst_shape": [M, N],
        "input_range": [-100.0, 100.0],
        "src0_min": float(src0.min()),
        "src0_max": float(src0.max()),
        "src1_min": float(src1.min()),
        "src1_max": float(src1.max()),
        "weight_min": float(weight.min()),
        "weight_max": float(weight.max()),
    }
    (output_dir / "case_manifest.json").write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )
    print(json.dumps(manifest, indent=2, ensure_ascii=False))


def main():
    parser = argparse.ArgumentParser(description="Generate MatMul FP32 input and golden data.")
    parser.add_argument("--case", choices=SUPPORTED_CASES, default="uniform")
    parser.add_argument("--seed", type=int, default=20260803)
    parser.add_argument("--root", type=Path, default=Path("."))
    args = parser.parse_args()
    generate(args.case, args.seed, args.root)


if __name__ == "__main__":
    main()
