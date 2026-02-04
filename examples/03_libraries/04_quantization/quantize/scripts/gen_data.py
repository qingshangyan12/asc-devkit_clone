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

    src_dtype = np.float16
    dst_dtype = np.int8
    scale_dtype = np.float16
    offset_dtype = np.float16
    golden = np.zeros([32, 288], dtype=np.int8).astype(dst_dtype)
    src = np.random.uniform(-5, 5, [32, 288]).astype(src_dtype)
    scale = np.random.uniform(-5, 5, [32, 1]).astype(scale_dtype)
    offset = np.random.uniform(-5, 5, [32, 1]).astype(offset_dtype)

    os.makedirs("input", exist_ok=True)
    src.tofile("./input/input_src.bin")
    offset.tofile("./input/input_offset.bin")
    scale.tofile("./input/input_scale.bin")

    round_mode = 1
    has_offset = True

    # convert to fp32 to calculate and then convert to int8
    golden = np.zeros([32, 288], dtype=np.int8)
    temp = np.zeros([32, 288], dtype=np.float)

    for i in range(32):
        for j in range(288):
            a = src[i][j].astype(np.float32) * scale[0][0].astype(np.float32)
            b = offset[0][0].astype(np.float32)
            temp[i][j] = (a + b) if (has_offset == True) else a

    # round
    if round_mode == 2:
        golden = np.floor(temp)
    elif round_mode == 3:
        golden = np.ceil(temp)
    elif round_mode == 4:
        for i in range(32):
            for j in range(128):
                tmp = str(temp[i][j]).split('.')
                golden[i][j] = np.round(temp[i][j])
                if len(tmp) == 2 and tmp[1] == "5" and int(tmp[0]) % 2 == 0:
                    if (temp[i][j] > 0):
                        golden[i][j] += 1
                    else:
                        golden[i][j] -= 1
    elif round_mode == 5:
        golden = np.trunc(temp)
    else: 
        golden = np.rint(temp)

    golden = golden.astype(dst_dtype)
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()
