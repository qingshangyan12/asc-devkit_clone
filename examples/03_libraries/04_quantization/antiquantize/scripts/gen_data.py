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
import sys
import numpy as np


def gen_golden_data_simple():
    np.set_printoptions(threshold = sys.maxsize, suppress = True)
    np.random.seed(19)
    src_dtype = np.int8
    dst_dtype = np.float32
    scale_dtype = np.float32
    offset_dtype = np.float32
    has_offset = True
    
    golden = np.zeros([5, 256], dtype=dst_dtype)
    scale = np.random.uniform(-5, 5, [32, 1]).astype(scale_dtype)
    offset = np.random.uniform(-5, 5, [32, 1]).astype(offset_dtype)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    offset.tofile("./input/input_offset.bin")
    scale.tofile("./input/input_scale.bin")

    src = np.random.uniform(-5, 5, [10, 256]).astype(src_dtype)
    src.tofile("./input/input_src.bin")

    temp = np.zeros([5, 256], dtype=np.float32)
    for i in range(5):
        for j in range(256):
            f32_src = src[i][j].astype(np.int32).astype(np.float32)
            f32_scale = scale[i]
            f32_offset = offset[i]
            temp[i][j] = ((f32_src + f32_offset) * f32_scale) if (has_offset == True) else (f32_src * f32_scale)
    golden = temp.astype(dst_dtype)
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()
