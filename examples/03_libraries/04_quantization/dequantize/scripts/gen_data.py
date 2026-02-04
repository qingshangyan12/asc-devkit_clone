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
    np.set_printoptions(threshold=sys.maxsize, suppress=True)
    np.random.seed(19)
    isPertensor = 0
    dst_dtype = np.float32

    src = np.random.uniform(-10, 10, [7, 128]).astype(np.int32)
    deq_scale = np.random.uniform(-10, 10, [1, 128]).astype(np.float32)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    src.tofile("./input/input_src.bin")
    deq_scale.tofile("./input/input_scale.bin")
    
    m = 7
    n = 128

    if dst_dtype == np.float16: # 输入uint64 输出half
        in_tensor = np.random.uniform(0, 0, [1, n]).astype(np.float32)
        for j in range(n) :
            in_tensor[0][j] = deq_scale[0][2 * j]
        tmp = np.tile(in_tensor, (m))
        tmp = tmp.reshape([m, n])
    else :
        tmp = np.tile(deq_scale, (m))
        tmp = tmp.reshape([m, n])

    src_fp32 = src.astype(np.float32)
    deqscalar_val = 1.19

    if isPertensor:
        deq_scale[0] = deqscalar_val
        golden = src_fp32 * deq_scale[0] # pertensor case
    else:
        golden = src_fp32 * tmp

    golden = golden.astype(dst_dtype) 
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()
