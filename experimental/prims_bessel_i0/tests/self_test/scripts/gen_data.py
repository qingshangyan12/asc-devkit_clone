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

import os
import sys
import numpy as np
from scipy.special import i0

def gen_golden_data_simple(size=2048):
    dtype = np.float32

    input_shape = [size]
    rng = np.random.default_rng(202607)
    input_x = rng.uniform(-10, 10, input_shape).astype(dtype)
    special_values = np.array(
        [0.0, -0.0, 1.0, -1.0, 3.75, -3.75, 9.0, -9.0, 80.0, -80.0, 88.0, -88.0,
         np.nan, np.inf, -np.inf],
        dtype=dtype)
    special_count = min(size, special_values.size)
    input_x[:special_count] = special_values[:special_count]
    golden = i0(input_x).astype(dtype)

    os.makedirs("./input", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    os.makedirs("./output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    size = int(sys.argv[1]) if len(sys.argv) > 1 else 2048
    gen_golden_data_simple(size)
