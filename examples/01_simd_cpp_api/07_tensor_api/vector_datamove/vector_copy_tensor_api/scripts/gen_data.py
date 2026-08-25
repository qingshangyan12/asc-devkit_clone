#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os

import numpy as np


MATRIX_DIM = 16
PADDED_COLUMNS = 32


def generate_data():
    input_data = np.zeros((MATRIX_DIM, PADDED_COLUMNS), dtype=np.float16)
    for row in range(MATRIX_DIM):
        for column in range(MATRIX_DIM):
            input_data[row, column] = (row + column) % 4 + 1

    golden = np.zeros_like(input_data)
    golden[:, :MATRIX_DIM] = input_data[:, :MATRIX_DIM]

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    generate_data()
