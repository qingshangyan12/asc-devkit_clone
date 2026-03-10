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

DATA_TYPE_MAP = {
        1: np.float16,
        2: np.float32,
        3: np.int16,
        4: np.int32,
    }


def get_range_by_dtype(input_type):
    try:
        if input_type == np.float16 or input_type == np.float32 or input_type == np.float64:
            return np.finfo(input_type).min, np.finfo(input_type).max
        else:
            return np.iinfo(input_type).min, np.iinfo(input_type).max
    except ValueError:
        print(f"Unsupported data type:{input_type}")


def gen_golden_data(scenario_num):
    input_type = DATA_TYPE_MAP[scenario_num]
    output_type = input_type
    # min_val, max_val = get_range_by_dtype(input_type)
    min_val, max_val = -100.0, 100.0
    block_length = 32768 # 输入数据长度
    input_shape = [block_length]
    output_shape = [block_length]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    input_y = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    golden = np.multiply(input_x, input_y)
    golden = golden.astype(output_type)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate cartesian product input and golden data for Mul operator.")
    parser.add_argument('-scenario_num', type=int, default=1, choices=[1, 2, 3, 4])
    args = parser.parse_args()

    gen_golden_data(args.scenario_num)