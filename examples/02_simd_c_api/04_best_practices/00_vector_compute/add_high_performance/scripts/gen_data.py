#!/usr/bin/python3

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np

M = 8192
N = 8192
ROWS_PER_CHUNK = 128
RANDOM_SEED = 20260806


def gen_golden_data_simple():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    rng = np.random.default_rng(RANDOM_SEED)

    with open("./input/input_x.bin", "wb") as input_x_file, open(
        "./input/input_y.bin", "wb"
    ) as input_y_file, open("./output/golden.bin", "wb") as golden_file:
        for row_start in range(0, M, ROWS_PER_CHUNK):
            row_count = min(ROWS_PER_CHUNK, M - row_start)
            chunk_shape = (row_count, N)
            input_x = rng.uniform(1, 100, chunk_shape).astype(np.float16)
            input_y = rng.uniform(1, 100, chunk_shape).astype(np.float16)
            golden = (input_x + input_y).astype(np.float16)
            input_x.tofile(input_x_file)
            input_y.tofile(input_y_file)
            golden.tofile(golden_file)


if __name__ == "__main__":
    gen_golden_data_simple()
