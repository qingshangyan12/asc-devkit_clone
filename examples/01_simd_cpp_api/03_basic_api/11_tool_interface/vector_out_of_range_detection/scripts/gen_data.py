# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Generate float32 addition inputs and per-element Inf reference masks."""

from pathlib import Path
import numpy as np


def gen_data():
    Path("input").mkdir(exist_ok=True)
    Path("output").mkdir(exist_ok=True)
    rng = np.random.default_rng(2026)
    x = rng.integers(-100, 101, 256).astype(np.float32)
    y = rng.integers(-100, 101, 256).astype(np.float32)
    maximum = np.finfo(np.float32).max
    # Include byte, vector-repeat and final-element boundaries.
    positions = [0, 1, 7, 8, 31, 32, 63, 64, 127, 128, 191, 192, 254, 255]
    cases = [
        (maximum, maximum),
        (-maximum, -maximum),
        (32, -16),
        (1, -1),
        (np.inf, 1),
        (-np.inf, 1),
        (maximum, 0),
        (-maximum, 0),
        (0, 0),
        (-0.0, -0.0),
        (maximum, -maximum),
        (-32, 16),
        (np.inf, np.inf),
        (-np.inf, -np.inf),
    ]
    for index, (left, right) in zip(positions, cases):
        x[index], y[index] = left, right
    with np.errstate(over="ignore", invalid="raise"):
        z = x + y
    masks = np.stack([np.isposinf(z), np.isneginf(z)])
    x.tofile("input/input_x.bin")
    y.tofile("input/input_y.bin")
    with open("output/golden.bin", "wb") as golden_file:
        z.tofile(golden_file)
        np.packbits(masks, axis=1, bitorder="little").tofile(golden_file)


if __name__ == "__main__":
    gen_data()
