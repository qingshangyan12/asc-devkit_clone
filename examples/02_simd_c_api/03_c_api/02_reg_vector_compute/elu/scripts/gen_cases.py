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

from gen_data import generate_case


PRECISION_SHAPES = (
    1,
    31,
    32,
    33,
    255,
    1023,
    1024,
    2048,
    4096,
    5000,
    131072,
    1000001,
    0,
)
PERFORMANCE_SHAPES = (32, 256, 1024, 2048, 4096, 16384, 65536)
DTYPES = ("float32", "float16")
TILE_LENGTH = 2048
MIN_ELEMENTS_PER_BLOCK = 512
MAX_BLOCKS = 56

COMMON_COEFFICIENTS = (
    ("selu", 1.7, 1.05, 1.0, "mixed", "SELU-style coefficients"),
    ("distinct", 0.5, 2.0, 0.5, "mixed", "three distinct runtime scalars"),
    ("near_zero", 1.0, 1.0, 1.0e-3, "mixed", "near-zero expm1 cancellation"),
    ("fast_saturation", 2.5, 0.25, 3.0, "mixed", "fast negative saturation"),
    ("negative_alpha", -1.0, 1.0, 1.0, "mixed", "negative alpha"),
    ("negative_scale", 1.0, -0.5, 1.0, "mixed", "negative scale"),
    ("negative_input_scale", 1.0, 1.0, -0.01, "mixed", "negative input_scale"),
    ("zero_input_scale", 1.0, 1.0, 0.0, "mixed", "zero input_scale"),
)

DTYPE_COEFFICIENTS = {
    "float32": (
        (
            "float_scalar_preserved",
            60000.0,
            1.0,
            2.0e-8,
            "negative",
            "FLOAT scalar must not be narrowed",
        ),
        (
            "host_premultiply_isolated",
            2.0e38,
            2.0,
            1.0,
            "positive",
            "Host premultiply overflow must not pollute the positive branch",
        ),
        (
            "negative_input_exp_overflow",
            1.0,
            1.0,
            -1.0e6,
            "negative",
            "positive exponential overflow propagates with the correct sign",
        ),
        (
            "nan_alpha",
            float("nan"),
            1.0,
            1.0,
            "negative",
            "NaN alpha propagation",
        ),
    ),
    "float16": (
        (
            "float_scalar_preserved",
            60000.0,
            1.0,
            2.0e-8,
            "negative",
            "FLOAT scalar must not be narrowed",
        ),
        (
            "half_premultiply_isolated",
            1.0e10,
            1.0e-3,
            1.0,
            "positive",
            "half premultiply overflow must not pollute the positive branch",
        ),
        (
            "infinite_alpha",
            float("inf"),
            1.0,
            1.0,
            "negative",
            "same-sign infinity propagation",
        ),
        (
            "nan_alpha",
            float("nan"),
            1.0,
            1.0,
            "negative",
            "NaN alpha propagation",
        ),
    ),
}

INPUT_SPECIALS = (
    ("input_nan", [float("nan"), -1.0, -0.0, 0.0, 1.0], "NaN tensor input"),
    (
        "input_infinities",
        [float("inf"), float("-inf"), -1.0, 0.0, 1.0],
        "positive and negative infinity tensor inputs",
    ),
)


def ceil_division(numerator, denominator):
    return numerator // denominator + int(numerator % denominator != 0)


def launch_plan(dtype, length):
    if length == 0:
        return 1, 0
    small_input_limit = 256 if dtype == "float32" else 1024
    alignment_elements = 8 if dtype == "float32" else 16
    requested_blocks = 1
    if length > small_input_limit:
        requested_blocks = min(
            MAX_BLOCKS, ceil_division(length, MIN_ELEMENTS_PER_BLOCK)
        )
    block_length = ceil_division(length, requested_blocks)
    block_length = ceil_division(block_length, alignment_elements) * alignment_elements
    return ceil_division(length, block_length), block_length


def expected_blocks(dtype, length):
    return launch_plan(dtype, length)[0]


def shape_purpose(dtype, length):
    _, block_length = launch_plan(dtype, length)
    tiles_per_block = ceil_division(block_length, TILE_LENGTH) if block_length else 0
    if length == 0:
        return "empty-input no-op coverage"
    if tiles_per_block > 1:
        return f"multi-tile loop coverage ({tiles_per_block} tiles per block)"
    return "shape, boundary, or unaligned-tail coverage"


def precision_definitions():
    cases = []
    seed = 2900
    for dtype in DTYPES:
        for length in PRECISION_SHAPES:
            expected_block_count, expected_block_length = launch_plan(dtype, length)
            cases.append(
                {
                    "name": f"shape_{dtype}_{length}",
                    "dtype": dtype,
                    "length": length,
                    "alpha": 1.0,
                    "scale": 1.0,
                    "input_scale": 1.0,
                    "seed": seed,
                    "profile": "mixed",
                    "purpose": shape_purpose(dtype, length),
                    "expected_blocks": expected_block_count,
                    "expected_block_length": expected_block_length,
                    "expected_tiles_per_block": (
                        ceil_division(expected_block_length, TILE_LENGTH)
                        if expected_block_length
                        else 0
                    ),
                }
            )
            seed += 1

        for name, alpha, scale, input_scale, profile, purpose in COMMON_COEFFICIENTS:
            cases.append(
                {
                    "name": f"coeff_{dtype}_{name}",
                    "dtype": dtype,
                    "length": 2048,
                    "alpha": alpha,
                    "scale": scale,
                    "input_scale": input_scale,
                    "seed": seed,
                    "profile": profile,
                    "purpose": purpose,
                }
            )
            seed += 1

        for name, alpha, scale, input_scale, profile, purpose in DTYPE_COEFFICIENTS[
            dtype
        ]:
            cases.append(
                {
                    "name": f"contract_{dtype}_{name}",
                    "dtype": dtype,
                    "length": 2048,
                    "alpha": alpha,
                    "scale": scale,
                    "input_scale": input_scale,
                    "seed": seed,
                    "profile": profile,
                    "purpose": purpose,
                }
            )
            seed += 1

        for name, values, purpose in INPUT_SPECIALS:
            cases.append(
                {
                    "name": f"nonfinite_{dtype}_{name}",
                    "dtype": dtype,
                    "length": 32,
                    "alpha": 1.0,
                    "scale": 1.0,
                    "input_scale": 1.0,
                    "seed": seed,
                    "profile": "mixed",
                    "values": values,
                    "purpose": purpose,
                }
            )
            seed += 1

    if len(cases) != 54:
        raise RuntimeError(f"precision matrix must contain 54 cases, got {len(cases)}")
    return cases


def performance_definitions():
    cases = []
    seed = 5000
    for dtype in DTYPES:
        for length in PERFORMANCE_SHAPES:
            cases.append(
                {
                    "name": f"perf_{dtype}_{length}",
                    "dtype": dtype,
                    "length": length,
                    "alpha": 1.0,
                    "scale": 1.0,
                    "input_scale": 1.0,
                    "seed": seed,
                    "profile": "mixed",
                    "purpose": "formal performance and block-plan verification",
                    "expected_blocks": expected_blocks(dtype, length),
                }
            )
            seed += 1
    return cases


def write_group(output_root, group_name, definitions):
    group_root = output_root / group_name
    group_root.mkdir(parents=True)
    manifest = []
    for definition in definitions:
        case_root = group_root / definition["name"]
        metadata = generate_case(
            case_root,
            definition["dtype"],
            definition["length"],
            definition["alpha"],
            definition["scale"],
            definition["input_scale"],
            definition["seed"],
            definition.get("values"),
            definition["profile"],
        )
        metadata.update(
            {
                "name": definition["name"],
                "purpose": definition["purpose"],
            }
        )
        for key in (
            "expected_blocks",
            "expected_block_length",
            "expected_tiles_per_block",
        ):
            if key in definition:
                metadata[key] = definition[key]
        manifest.append(metadata)

    (group_root / "manifest.json").write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False, allow_nan=False) + "\n",
        encoding="utf-8",
    )
    return len(manifest)


def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate the ELU precision and performance case matrices."
    )
    parser.add_argument("--output-root", default="cases")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    output_root = Path(args.output_root)
    if output_root.exists():
        raise FileExistsError(f"output root already exists: {output_root}")
    output_root.mkdir(parents=True)
    precision_total = write_group(output_root, "precision", precision_definitions())
    performance_total = write_group(
        output_root, "performance", performance_definitions()
    )
    print(f"PRECISION_CASES total={precision_total}")
    print(f"PERFORMANCE_CASES total={performance_total}")
    print(f"OUTPUT_ROOT path={output_root}")
