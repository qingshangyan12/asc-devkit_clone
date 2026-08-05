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
import math
from pathlib import Path

import numpy as np


EDGE_VALUES = np.array(
    [-100.0, -20.0, -1.0, -0.1, -1.0e-4, -0.0, 0.0, 1.0e-4, 0.1, 1.0, 20.0, 100.0],
    dtype=np.float64,
)
POSITIVE_EDGE_VALUES = np.array([1.0e-4, 0.1, 1.0, 20.0, 100.0], dtype=np.float64)
NEGATIVE_EDGE_VALUES = np.array([-1.0e-4, -0.1, -1.0, -20.0, -100.0], dtype=np.float64)


def normalize_dtype(dtype_name):
    if dtype_name in ("float", "float32"):
        return "float32", np.float32
    if dtype_name in ("half", "float16"):
        return "float16", np.float16
    raise ValueError("dtype must be float32 or float16")


def json_scalar(value):
    if math.isnan(value):
        return "nan"
    if math.isinf(value):
        return "inf" if value > 0 else "-inf"
    return value


def generate_case(
    output_root,
    dtype_name,
    length,
    alpha,
    scale,
    input_scale,
    seed=2026,
    values=None,
    profile="mixed",
):
    normalized_dtype, numpy_dtype = normalize_dtype(dtype_name)
    if length < 0:
        raise ValueError("length must be non-negative")

    # Runtime scalar arguments are C++ float values, so golden generation first rounds them to float32.
    alpha_f32 = float(np.float32(alpha))
    scale_f32 = float(np.float32(scale))
    input_scale_f32 = float(np.float32(input_scale))

    if profile not in ("mixed", "positive", "negative"):
        raise ValueError("profile must be mixed, positive, or negative")

    if values is None:
        rng = np.random.default_rng(seed)
        source = rng.uniform(-100.0, 100.0, size=length).astype(numpy_dtype)
        if profile == "positive":
            source = np.maximum(np.abs(source), np_dtype_minimum(numpy_dtype))
            edge_values = POSITIVE_EDGE_VALUES
        elif profile == "negative":
            source = -np.maximum(np.abs(source), np_dtype_minimum(numpy_dtype))
            edge_values = NEGATIVE_EDGE_VALUES
        else:
            edge_values = EDGE_VALUES
        edge_count = min(length, edge_values.size)
        if edge_count:
            source[:edge_count] = edge_values[:edge_count].astype(numpy_dtype)
        input_pattern = f"{profile}_edge_values_then_uniform"
    else:
        values = np.asarray(values, dtype=numpy_dtype)
        if length and values.size == 0:
            raise ValueError("values must not be empty for a non-empty case")
        source = np.resize(values, length).astype(numpy_dtype)
        input_pattern = "explicit_values"

    source_f64 = source.astype(np.float64)
    golden_f64 = np.empty(length, dtype=np.float64)
    positive = source_f64 > 0.0
    golden_f64[positive] = scale_f32 * source_f64[positive]
    with np.errstate(over="ignore", invalid="ignore", under="ignore"):
        golden_f64[~positive] = (
            alpha_f32 * scale_f32 * np.expm1(source_f64[~positive] * input_scale_f32)
        )
    with np.errstate(over="ignore", invalid="ignore"):
        golden = golden_f64.astype(np.float32)

    output_root = Path(output_root)
    input_dir = output_root / "input"
    golden_dir = output_root / "output"
    input_dir.mkdir(parents=True, exist_ok=True)
    golden_dir.mkdir(parents=True, exist_ok=True)
    source.tofile(input_dir / "input_x.bin")
    golden.tofile(golden_dir / "golden.bin")

    metadata = {
        "dtype": normalized_dtype,
        "length": length,
        "alpha": json_scalar(alpha_f32),
        "scale": json_scalar(scale_f32),
        "input_scale": json_scalar(input_scale_f32),
        "seed": seed,
        "input_pattern": input_pattern,
        "input_file": "input/input_x.bin",
        "golden_file": "output/golden.bin",
    }
    (output_root / "case.json").write_text(
        json.dumps(metadata, indent=2) + "\n", encoding="utf-8"
    )
    return metadata


def np_dtype_minimum(numpy_dtype):
    return float(np.finfo(numpy_dtype).tiny)


def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate one ELU input and golden pair."
    )
    parser.add_argument("--dtype", default="float32", choices=("float32", "float16"))
    parser.add_argument("--length", type=int, default=2048)
    parser.add_argument("--alpha", type=float, default=1.0)
    parser.add_argument("--scale", type=float, default=1.0)
    parser.add_argument("--input-scale", type=float, default=1.0)
    parser.add_argument("--seed", type=int, default=2026)
    parser.add_argument("--output-root", default=".")
    parser.add_argument(
        "--profile", choices=("mixed", "positive", "negative"), default="mixed"
    )
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    case = generate_case(
        args.output_root,
        args.dtype,
        args.length,
        args.alpha,
        args.scale,
        args.input_scale,
        args.seed,
        profile=args.profile,
    )
    print(json.dumps(case, sort_keys=True))
