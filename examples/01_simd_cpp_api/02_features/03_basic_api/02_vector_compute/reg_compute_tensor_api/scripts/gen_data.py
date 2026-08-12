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

from pathlib import Path
from typing import Union

import numpy as np


VECTOR_LENGTH = 64
INT64_VECTOR_LENGTH = 32
HISTOGRAM_INPUT_LENGTH = 256
HISTOGRAM_INITIAL_VALUE = np.uint16(3)
DEFAULT_PRELU_SLOPE = np.float32(0.25)
DEFAULT_LEAKY_RELU_SLOPE = np.float32(0.01)


def prelu(src: np.ndarray, slope: Union[np.ndarray, np.float32]) -> np.ndarray:
    return np.where(src >= 0.0, src, src * slope).astype(np.float32)


def main() -> None:
    work_dir = Path.cwd()
    input_dir = work_dir / "input"
    output_dir = work_dir / "output"
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    float_a = np.linspace(-2.0, 2.0, VECTOR_LENGTH, dtype=np.float32)
    float_a[VECTOR_LENGTH // 2 - 1] = np.float32(-0.0)
    float_a[VECTOR_LENGTH // 2] = np.float32(0.0)
    float_b = (0.75 + (np.arange(VECTOR_LENGTH) % 7) * 0.125).astype(np.float32)
    float_b[::8] = float_a[::8]
    float_b[VECTOR_LENGTH // 2] = np.float32(1.25)
    float_slope = np.linspace(0.05, 0.35, VECTOR_LENGTH, dtype=np.float32)
    positive = np.abs(float_a) + np.float32(0.5)

    int_a = np.arange(VECTOR_LENGTH, dtype=np.int32)
    int_b = (3 * np.arange(VECTOR_LENGTH) + 1).astype(np.int32)
    int_shift = (np.arange(VECTOR_LENGTH) % 4).astype(np.int32)
    int64_a = (np.arange(INT64_VECTOR_LENGTH, dtype=np.int64) - np.int64(16)) * np.int64(1_000_003)
    int64_b = (np.arange(INT64_VECTOR_LENGTH, dtype=np.int64) % np.int64(9) - np.int64(4)) * np.int64(97)
    histogram_input = (
        np.arange(HISTOGRAM_INPUT_LENGTH, dtype=np.uint16) * np.uint16(37) +
        np.arange(HISTOGRAM_INPUT_LENGTH, dtype=np.uint16) % np.uint16(11)
    ).astype(np.uint8)

    float_results = np.stack(
        [
            np.abs(float_a),
            np.exp(float_a),
            np.sqrt(positive),
            np.log(positive),
            np.log2(positive),
            np.log10(positive),
            -float_a,
            np.maximum(float_a, np.float32(0.0)),
            prelu(float_a, float_slope),
            prelu(float_a, np.float32(0.2)),
            prelu(float_a, DEFAULT_PRELU_SLOPE),
            prelu(float_a, np.float32(0.1)),
            prelu(float_a, DEFAULT_LEAKY_RELU_SLOPE),
            float_a + float_b,
            float_a + np.float32(1.0),
            np.float32(1.0) + float_a,
            float_a - float_b,
            float_a - np.float32(1.0),
            np.float32(1.0) - float_a,
            float_a * float_b,
            float_a * np.float32(2.0),
            np.float32(2.0) * float_a,
            float_a / float_b,
            float_a / np.float32(2.0),
            np.float32(2.0) / positive,
            np.maximum(float_a, float_b),
            np.maximum(float_a, np.float32(0.0)),
            np.maximum(np.float32(0.0), float_a),
            np.minimum(float_a, float_b),
            np.minimum(float_a, np.float32(0.0)),
            np.minimum(np.float32(0.0), float_a),
            (float_a == float_b).astype(np.float32),
            (float_a != np.float32(0.0)).astype(np.float32),
            (np.float32(0.0) < float_a).astype(np.float32),
            (float_a <= np.float32(0.0)).astype(np.float32),
            (np.float32(0.0) > float_a).astype(np.float32),
            (float_a >= float_b).astype(np.float32),
            np.where(float_a > 0.0, float_a + np.float32(1.0), np.float32(0.0)),
            np.float32(0.25) * float_a + float_b,
            np.abs(float_a - float_b),
            np.exp(float_a - float_b),
            float_a * float_b + float_slope,
            float_b * float_slope + float_a,
            (float_a * np.float32(1.5)).astype(np.float16).astype(np.float32),
        ]
    ).astype(np.float32)

    int_results = np.stack(
        [
            (int_a == 0).astype(np.int32),
            int_a & int_b,
            int_a & np.int32(15),
            np.int32(15) & int_a,
            int_a | int_b,
            int_a | np.int32(16),
            np.int32(16) | int_a,
            int_a ^ int_b,
            int_a ^ np.int32(7),
            np.int32(7) ^ int_a,
            int_a << int_shift,
            int_a << np.int32(2),
            int_a >> int_shift,
            int_a >> np.int32(2),
        ]
    ).astype(np.int32)

    int64_results = np.stack(
        [
            np.int64(-3) * int64_a + int64_b,
            np.abs(int64_a - int64_b),
            int64_a * int64_b + int64_b,
        ]
    ).astype(np.int64)

    histogram_frequency = np.bincount(histogram_input, minlength=256).astype(np.uint16)
    histogram_results = np.stack(
        [
            histogram_frequency[:128] + HISTOGRAM_INITIAL_VALUE,
            histogram_frequency[128:] + HISTOGRAM_INITIAL_VALUE,
            np.cumsum(histogram_frequency[:128], dtype=np.uint16) + HISTOGRAM_INITIAL_VALUE,
            np.cumsum(histogram_frequency, dtype=np.uint16)[128:] + HISTOGRAM_INITIAL_VALUE,
        ]
    ).astype(np.uint16)

    float_a.tofile(input_dir / "float_a.bin")
    float_b.tofile(input_dir / "float_b.bin")
    float_slope.tofile(input_dir / "float_slope.bin")
    int_a.tofile(input_dir / "int_a.bin")
    int_b.tofile(input_dir / "int_b.bin")
    int_shift.tofile(input_dir / "int_shift.bin")
    int64_a.tofile(input_dir / "int64_a.bin")
    int64_b.tofile(input_dir / "int64_b.bin")
    histogram_input.tofile(input_dir / "histogram_input.bin")
    float_results.tofile(output_dir / "golden_float.bin")
    int_results.tofile(output_dir / "golden_int.bin")
    int64_results.tofile(output_dir / "golden_int64.bin")
    histogram_results.tofile(output_dir / "golden_histogram.bin")

    print(
        f"Generated {float_results.shape[0]} float rows, {int_results.shape[0]} int32 rows, "
        f"{int64_results.shape[0]} int64 rows, and {histogram_results.shape[0]} histogram rows."
    )


if __name__ == "__main__":
    main()
