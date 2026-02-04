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


def gen_golden_data_simple():
    dtype = np.float32
    np.random.seed(123321)
    src_type = dtype
    src_shape = [16]
    calcount = 16
    api_mode = 0

    src0 = np.random.uniform(-10000,10000, src_shape).astype(src_type)
    src1 = np.random.uniform(-10000,10000, src_shape).astype(src_type)
    tmp_golden = np.zeros(src_shape).astype(src_type)
    golden = np.zeros(16).astype(src_type)

    count = len(src0)
    if api_mode == 1 or api_mode == 2:
        count = calcount

    for i in range(count):
        tmp_golden[i] = np.bitwise_xor(src0[i], src1[i])
    sum = np.sum(tmp_golden)
    min_val = np.iinfo(src_type).min
    max_val = np.iinfo(src_type).max
    sum_clipped = np.clip(sum, min_val, max_val)
    golden[0] = sum_clipped

    os.makedirs("input", exist_ok=True)
    src0.tofile("./input/input_src0.bin")
    src1.tofile("./input/input_src1.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
