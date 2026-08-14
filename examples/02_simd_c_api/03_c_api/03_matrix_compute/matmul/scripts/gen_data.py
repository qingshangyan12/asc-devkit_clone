#!/usr/bin/env python3

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
CASES = tuple(f"P{index:02d}" for index in range(1, 9))
RANDOM_SEEDS = {"P06": 20260804, "P07": 20260805, "P08": 20260806}


def build_case(case_name: str) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    if case_name == "P01":
        a = np.zeros((M, K), dtype=np.float32)
        b = np.zeros((K, N), dtype=np.float32)
        bias = np.linspace(-100.0, 100.0, N, dtype=np.float32)
    elif case_name == "P02":
        a = np.zeros((M, K), dtype=np.float32)
        b = np.zeros((K, N), dtype=np.float32)
        bias = np.zeros(N, dtype=np.float32)
    elif case_name == "P03":
        pattern = np.array([-100.0, 100.0, 0.0], dtype=np.float32)
        a = np.resize(pattern, M * K).reshape(M, K)
        b = np.resize(pattern[::-1], K * N).reshape(K, N)
        bias = np.resize(pattern, N)
    elif case_name == "P04":
        row_values = np.linspace(1.0, 100.0, M, dtype=np.float32)[:, None]
        signs = np.where(np.arange(K) % 2 == 0, 1.0, -1.0).astype(np.float32)
        a = row_values * signs[None, :]
        b_half = np.linspace(-100.0, 100.0, (K // 2) * N, dtype=np.float32).reshape(
            K // 2, N
        )
        b = np.repeat(b_half, 2, axis=0)
        bias = np.concatenate(
            (
                np.linspace(-100.0, -10.0, N // 2, dtype=np.float32),
                np.linspace(10.0, 100.0, N // 2, dtype=np.float32),
            )
        )
    elif case_name == "P05":
        a = np.arange(M * K, dtype=np.float32).reshape(M, K) % np.float32(101.0)
        b = np.zeros((K, N), dtype=np.float32)
        rows = np.arange(K)
        columns = (rows * 7 + 3) % N
        b[rows, columns] = np.linspace(1.0, 2.0, K, dtype=np.float32)
        bias = np.linspace(-10.0, 10.0, N, dtype=np.float32)
    else:
        rng = np.random.default_rng(RANDOM_SEEDS[case_name])
        a = rng.uniform(-100.0, 100.0, size=(M, K)).astype(np.float32)
        b = rng.uniform(-100.0, 100.0, size=(K, N)).astype(np.float32)
        bias = rng.uniform(-100.0, 100.0, size=N).astype(np.float32)

    return np.ascontiguousarray(a), np.ascontiguousarray(b), np.ascontiguousarray(bias)


def write_case(case_name: str, output_dir: Path) -> None:
    a, b, bias = build_case(case_name)
    golden = a.astype(np.float64) @ b.astype(np.float64) + bias.astype(np.float64)

    output_dir.mkdir(parents=True, exist_ok=True)
    a.tofile(output_dir / "x1_gm.bin")
    b.tofile(output_dir / "x2_gm.bin")
    bias.tofile(output_dir / "bias_gm.bin")
    np.ascontiguousarray(golden, dtype=np.float64).tofile(output_dir / "golden.bin")

    metadata = {
        "case": case_name,
        "dtype": "float32",
        "a_shape": [M, K],
        "b_shape": [K, N],
        "bias_shape": [1, N],
        "output_shape": [M, N],
        "golden_dtype": "float64",
        "seed": RANDOM_SEEDS.get(case_name),
    }
    (output_dir / "case.json").write_text(
        json.dumps(metadata, indent=2) + "\n", encoding="utf-8"
    )

    print(f"case={case_name}")
    print(f"output_dir={output_dir}")
    print(f"a_range=[{float(a.min()):.6f}, {float(a.max()):.6f}]")
    print(f"b_range=[{float(b.min()):.6f}, {float(b.max()):.6f}]")
    print(f"bias_range=[{float(bias.min()):.6f}, {float(bias.max()):.6f}]")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate fixed-shape FP32 MatMul test data."
    )
    parser.add_argument(
        "--case", choices=CASES, required=True, help="Precision case name (P01-P08)."
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        required=True,
        help="Directory for input and golden files.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    write_case(args.case, args.output_dir)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
