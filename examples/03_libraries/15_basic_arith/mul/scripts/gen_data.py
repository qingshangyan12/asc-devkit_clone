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


def get_dtype(scenario_num: int) -> type:
    if 1 <= scenario_num <= 4:
        return np.float16
    elif 5 <= scenario_num <= 8:
        return np.float32
    elif 9 <= scenario_num <= 12:
        return np.int16
    elif 13 <= scenario_num <= 16:
        return np.int32
    else:
        raise ValueError(f"Invalid scenario number: {scenario_num}")

def get_element_size(dtype: type) -> int:
    return np.dtype(dtype).itemsize

def gen_golden_data(scenario_num):
    total_length = 1024 # 输入数据长度
    input_type = get_dtype(scenario_num)
    output_type = input_type
    if input_type == np.float16 or input_type == np.float32:
        min_val, max_val = -100.0, 100.0
    else:
        min_val, max_val = -100, 100
    input_shape = [total_length]
    output_shape = [total_length]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    input_y = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    golden = np.zeros(total_length).astype(output_type)
    # 根据不同接口调用场景，构造不同真值
    if scenario_num == 1 or scenario_num == 2 or scenario_num == 5 or scenario_num == 6 or \
        scenario_num == 9 or scenario_num == 10 or scenario_num == 13 or scenario_num == 14:
        golden = np.multiply(input_x, input_y)
    elif scenario_num == 3 or scenario_num == 7 or scenario_num == 11 or scenario_num == 15:
        count = 256 / get_element_size(input_type)
        for index in range(len(input_x)):
            if (index % count) < (count / 2):
                golden[index] = input_x[index] * input_y[index]
            # print("index % count = {}, count / 2 = {}".format(index % 256, count / 2))
            # print(
            # "data index: %06d, count: %d, golden: %-.9f, input_x: %-.9f, input_y: %-.9f" %
            # (index, count, golden[index], input_x[index], input_y[index]))
    else:
        count = 256 / get_element_size(input_type)
        for index in range(len(input_x)):
            if index % 2 == 0:
                golden[index] = input_x[index] * input_y[index]
    golden = golden.astype(output_type)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate cartesian product input and golden data for Mul operator.")
    parser.add_argument('-scenario_num', type=int, default=1, choices=list(range(1, 17)))
    args = parser.parse_args()

    gen_golden_data(args.scenario_num)