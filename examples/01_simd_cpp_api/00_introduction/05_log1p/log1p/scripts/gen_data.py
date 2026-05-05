#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np


def gen_log1p_test_data():
    """Generate test data for log1p covering float and half with emphasis on near-zero values."""

    # ---- Region 1: Very small positive values (1e-7 to 1e-3) ----
    # Exercise polynomial approximation path
    x_very_small_pos = np.logspace(-7, -3, 200, dtype=np.float64)

    # ---- Region 2: Very small negative values (-1e-3 to -1e-7) ----
    x_very_small_neg = -np.logspace(-3, -7, 200, dtype=np.float64)

    # ---- Region 3: Small values in polynomial range (0.001 to 0.12) ----
    x_small_pos = np.linspace(0.001, 0.12, 200, dtype=np.float64)

    # ---- Region 4: Small negative values in polynomial range (-0.12 to -0.001) ----
    x_small_neg = np.linspace(-0.12, -0.001, 200, dtype=np.float64)

    # ---- Region 5: Boundary values around threshold 0.125 ----
    x_boundary = np.array([
        -0.126, -0.1255, -0.125, -0.1245, -0.124,
        0.124, 0.1245, 0.125, 0.1255, 0.126
    ], dtype=np.float64)

    # ---- Region 6: Medium values in ln path (0.13 to 1.0) ----
    x_medium_pos = np.linspace(0.13, 1.0, 150, dtype=np.float64)

    # ---- Region 7: Medium negative values in ln path (-0.9 to -0.13) ----
    x_medium_neg = np.linspace(-0.9, -0.13, 150, dtype=np.float64)

    # ---- Region 8: Large values (1.0 to 10.0) ----
    x_large = np.linspace(1.0, 10.0, 150, dtype=np.float64)

    # ---- Region 9: Values near -1 (domain limit of log1p) ----
    x_near_neg1 = np.array([-0.9, -0.95, -0.99, -0.999], dtype=np.float64)

    # ---- Region 10: Exact 0 and subnormal region ----
    x_near_zero = np.concatenate([
        np.linspace(-1e-5, -1e-7, 50),
        np.array([0.0], dtype=np.float64),
        np.linspace(1e-7, 1e-5, 50)
    ])

    # ---- Region 11: Special floating-point values ----
    x_special = np.array([-0.5, 0.5, 1.0, 2.0, 5.0, -0.1, 0.1, -0.25, 0.25],
                         dtype=np.float64)

    # Combine all regions
    x_all = np.concatenate([
        x_very_small_pos, x_very_small_neg,
        x_small_pos, x_small_neg,
        x_boundary,
        x_medium_pos, x_medium_neg,
        x_large,
        x_near_neg1,
        x_near_zero,
        x_special
    ])

    # Sort and remove duplicates
    x_all = np.unique(x_all)
    x_all.sort()

    # Ensure exactly 2048 elements: pad with random values from existing set
    TARGET_COUNT = 2048
    rng = np.random.RandomState(42)
    actual = len(x_all)
    if actual < TARGET_COUNT:
        extra = rng.choice(x_all, size=TARGET_COUNT - actual, replace=True)
        x_all = np.concatenate([x_all, extra])
    elif actual > TARGET_COUNT:
        idx = rng.choice(actual, size=TARGET_COUNT, replace=False)
        x_all = x_all[idx]
    x_all.sort()

    print(f"Generated {len(x_all)} test values (target={TARGET_COUNT})")
    print(f"  Range: [{x_all[0]:.2e}, {x_all[-1]:.2e}]")
    num_poly = np.sum(np.abs(x_all) < 0.125)
    num_ln = len(x_all) - num_poly
    print(f"  Poly path (|x|<0.125): {num_poly} elements")
    print(f"  Ln   path (|x|>=0.125): {num_ln} elements")

    # ---- Generate float data ----
    x_float = x_all.astype(np.float32)
    golden_float = np.log1p(x_float.astype(np.float64)).astype(np.float32)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    x_float.tofile('./input/input_x_float.bin')
    golden_float.tofile('./output/golden_float.bin')

    # ---- Generate half data ----
    x_half = x_all.astype(np.float16)
    golden_half = np.log1p(x_half.astype(np.float64)).astype(np.float16)

    x_half.tofile('./input/input_x_half.bin')
    golden_half.tofile('./output/golden_half.bin')

    print("Generated files:")
    print(f"  ./input/input_x_float.bin  ({x_float.nbytes} bytes)")
    print(f"  ./output/golden_float.bin  ({golden_float.nbytes} bytes)")
    print(f"  ./input/input_x_half.bin   ({x_half.nbytes} bytes)")
    print(f"  ./output/golden_half.bin   ({golden_half.nbytes} bytes)")


if __name__ == "__main__":
    gen_log1p_test_data()
