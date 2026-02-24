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
import numpy as np
import argparse

fractal_size = 512
fractal_shape = [16, 16]

def gen_golden_data(scenarioNum=1, m=32, n=32, k=32):
    src_dtype = None
    dst_dtype = None
    if (scenarioNum >= 1 and scenarioNum <= 4):
        src_dtype = np.float16
        dst_dtype = np.float32
    elif (scenarioNum > 4 and scenarioNum <= 6):
        src_dtype = np.int8
        dst_dtype = np.int32
    x1_gm = np.ones([m, k]).astype(src_dtype)
    x2_gm = np.ones([k, n]).astype(src_dtype)
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 13))
    parser.add_argument('-m', type=int)
    parser.add_argument('-k', type=int)
    parser.add_argument('-n', type=int)
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.m, args.n, args.k)
