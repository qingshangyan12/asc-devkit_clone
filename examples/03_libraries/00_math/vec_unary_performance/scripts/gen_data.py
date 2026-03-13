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
    input_type = np.float16 if input_type == 1 else np.float32
    output_type = input_type
    if input_type == np.float16 or input_type == np.float32:
        min_val, max_val = -10.0, 10.0
    else:
        min_val, max_val = -10, 10
    input_shape = [input_size]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    golden = np.exp(input_x).astype(output_type)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_src.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate golden data for operator.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2],
                        help="Input data type: 1=float16, 2=float32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    args = parser.parse_args()

    gen_golden_data(args.inputType, args.inputSize)
