#!/usr/bin/python3
# coding=utf-8

# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

import os
import numpy as np


def softmax_flash_v2(x, inmax=None, insum=None, update=False, is_fp16=False):
    
    orig_shape = x.shape

    if update == False:
        if is_fp16:
            x = x.astype(np.float32)
        x_max = np.max(x, axis=-1, keepdims=True)
        x_sub = x - x_max
        x_exp = np.exp(x_sub)
        x_sum = np.sum(x_exp, axis=-1, keepdims=True)
        exp_max = None

        if is_fp16:
            x_exp = x_exp.astype(np.float16)
            x_max = x_max.astype(np.float16)
            x_sum = x_sum.astype(np.float16)

        return x_exp, x_max, x_sum, exp_max
    else:
        if is_fp16:
            x = x.astype(np.float32)
            inmax = inmax.astype(np.float32)
            insum = insum.astype(np.float32)

        x_max = np.max(np.concatenate((inmax, x), axis=-1), axis=-1, keepdims=True)
        x_exp = np.exp(x - x_max)
        exp_max = np.exp(inmax - x_max)
        x_sum = np.sum(x_exp, axis=-1, keepdims=True)

        x_sum_new = exp_max * insum + x_sum

        if is_fp16:
            x_exp = x_exp.astype(np.float16)
            x_max = x_max.astype(np.float16)
            x_sum_new = x_sum_new.astype(np.float16)
            exp_max = exp_max.astype(np.float16)
        return x_exp, x_max, x_sum_new, exp_max


def gen_golden_data_simple():

    height = 128
    width = 128
    shapeinfo_dtype = np.uint32
    shapeinfo_shape = [8]
    shapeinfo = np.random.uniform(1, 1, shapeinfo_shape).astype(shapeinfo_dtype)
    shapeinfo[0] = height
    shapeinfo[1] = width
    dtype = np.float32
    src_dtype = dtype
    max_dtype = dtype
    sum_dtype = dtype
    dst_dtype = dtype

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    shapeinfo.tofile("./input/shapeinfo.bin")
    
    mkShape = [height, width]
    oriWidth = width
    oriHeight = height

    if max_dtype == np.float32:
        sum_width = 8
    else:
        sum_width = 16
    
    orimkShape = [oriHeight, oriWidth]
    if src_dtype == np.float16:
        x1 = np.random.uniform(-10, 10, mkShape).astype(src_dtype)
        x2 = np.random.uniform(-10, 10, mkShape).astype(src_dtype)
        x1 = x1.astype(src_dtype)
        x2 = x2.astype(src_dtype)
        max_front = np.zeros([height, sum_width], dtype=max_dtype)
        sum_front = np.zeros([height, sum_width], dtype=max_dtype)

        orix2 = np.zeros(mkShape, dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(oriWidth):
                orix2[i][j] = x2[i][j]

        clc_x1 = np.zeros(orimkShape, dtype=src_dtype)
        clc_x2 = np.zeros(orimkShape, dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(oriWidth):
                clc_x1[i][j] = x1[i][j]
                clc_x2[i][j] = x2[i][j]

        out_1, max_1, sum_1, exp_max_1 = softmax_flash_v2(clc_x1, max_front, sum_front, update=False, is_fp16=True)
        max_in = np.zeros([height, sum_width], dtype=max_dtype)
        sum_in = np.zeros([height, sum_width], dtype=max_dtype)
        for i in range(oriHeight):
            for j in range(sum_width):
                max_in[i][j] = max_1[i]
                sum_in[i][j] = sum_1[i]

        max_in.tofile("./input/inputMax.bin")
        sum_in.tofile("./input/inputSum.bin")

        out_2, max_2, sum_2, exp_max_2 = softmax_flash_v2(clc_x2, max_1, sum_1, update=True, is_fp16=True)

        sum_2 = sum_2 + sum_front

        newout = np.zeros(mkShape, dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(oriWidth):
                newout[i][j] = out_2[i][j]

        orix2.tofile("./input/input.bin")
        newout.tofile("./output/golden.bin")
        
    else:
        x1 = np.random.uniform(-10, 10, mkShape).astype(src_dtype)
        x2 = np.random.uniform(-10, 10, mkShape).astype(src_dtype)
        max_front = np.zeros([height, 8], dtype=src_dtype)
        sum_front = np.zeros([height, 8], dtype=src_dtype)

        orix2 = np.zeros(mkShape, dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(oriWidth):
                orix2[i][j] = x2[i][j]

        clc_x1 = np.zeros(orimkShape, dtype=src_dtype)
        clc_x2 = np.zeros(orimkShape, dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(oriWidth):
                clc_x1[i][j] = x1[i][j]
                clc_x2[i][j] = x2[i][j]

        out_1, max_1, sum_1, exp_max_1 = softmax_flash_v2(clc_x1, max_front, update=False, is_fp16=False)

        max_in = np.zeros([height, 8], dtype=src_dtype)
        sum_in = np.zeros([height, 8], dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(8):
                max_in[i][j] = max_1[i]
                sum_in[i][j] = sum_1[i]

        max_in.tofile("./input/inputMax.bin")
        sum_in.tofile("./input/inputSum.bin")
        out_2, max_2, sum_2, exp_max_2 = softmax_flash_v2(clc_x2, max_1, sum_1, update=True, is_fp16=False)

        sum_2 = sum_2 + sum_front
        newout = np.zeros(mkShape, dtype=src_dtype)
        for i in range(oriHeight):
            for j in range(oriWidth):
                newout[i][j] = out_2[i][j]

        orix2.tofile("./input/input.bin")
        newout.tofile("./output/golden.bin")
        

if __name__ == "__main__":
    gen_golden_data_simple()