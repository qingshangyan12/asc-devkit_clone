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

def gen_golden_data_simple(scenarioNum, size):
    src_dtype = None
    dst_dtype = None
    if scenarioNum >= 1 and scenarioNum <= 6:
        src_dtype = np.float16
        dst_dtype = np.int8
    else:
        src_dtype = np.float16
        dst_dtype = np.uint8
    
    input_x = np.random.uniform(1, 10, [size]).astype(src_dtype)
    input_y = np.random.uniform(1, 10, [size]).astype(src_dtype)
    golden = np.round(input_x * input_y).astype(dst_dtype)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 13))
    parser.add_argument('-size', type=int)
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum, args.size)
