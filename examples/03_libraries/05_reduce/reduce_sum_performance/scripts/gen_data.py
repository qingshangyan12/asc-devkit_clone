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


def gen_golden_data_simple(input_type=1, input_size=1024):
    input_type = None
    output_type = None

    if input_type == 1:
        input_type = np.float16
        output_type = np.float16
    else:
        input_type = np.float32
        output_type = np.float32

    block_length = input_size
    input_shape = [block_length]
    output_shape = [16]
    input_x = np.ones(input_shape).astype(input_type)
    golden = np.zeros(output_shape).astype(output_type)
    golden[0] = np.sum(input_x)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2],
 	                        help="Input data type: 1=float16, 2=float32")
    parser.add_argument('-inputSize', type=int, required=True,
 	                        help="Total number of elements in the tensor")
    args = parser.parse_args()
    gen_golden_data_simple(args.inputType, args.inputSize)
