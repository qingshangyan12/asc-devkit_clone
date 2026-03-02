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


def gen_golden_data(input_type=1, input_size=1024):
    input_type_map = {
        1: np.float16,
        2: np.float32,
        3: np.int16,
        4: np.int32
    }
    input_type = input_type_map.get(input_type, np.float16)
    output_type = input_type
    
    if input_type == np.float16 or input_type == np.float32:
        min_val, max_val = -100.0, 100.0
    else:
        min_val, max_val = -100, 100
    
    input_shape = [input_size]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    input_y = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    golden = np.multiply(input_x, input_y).astype(output_type)
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate golden data for Mul operator.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input data type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    args = parser.parse_args()

    gen_golden_data(args.inputType, args.inputSize)
