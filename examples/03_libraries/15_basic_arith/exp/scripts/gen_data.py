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
import argparse
import numpy as np

def get_dtype_limits(data_type):
    finfo = np.finfo(data_type)
    return {
        'min': finfo.min,
        'max': finfo.max,
        # 'tiny': finfo.tiny,
        'pos_zero': np.array(+0.0, dtype=data_type),
        'neg_zero': np.array(-0.0, dtype=data_type),
        'nan': np.array(np.nan, dtype=data_type),
        'pos_inf': np.array(np.inf, dtype=data_type),
        'neg_inf': np.array(-np.inf, dtype=data_type),
    }


def construct_boundary_inputs(data_type, repeat=32):
    """
    构造包含边界值和特殊值的输入数组 (每个值重复repeat次)
    顺序: min, max, pos_zero, neg_zero, nan, pos_inf, neg_inf
    """
    limits = get_dtype_limits(data_type)
    special_values = [
        limits['min'],
        limits['max'],
        # limits['tiny'],
        limits['pos_zero'],
        limits['neg_zero'],
        limits['nan'],
        limits['pos_inf'],
        limits['neg_inf'],
    ]
    # 每种值重复 repeat 次
    pattern = np.concatenate([[v] * repeat for v in special_values]).astype(data_type)
    return pattern


def compute_exp_golden(input_array):
    """
    计算自然指数的真值，忽略溢出/下溢警告
    """
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.exp(input_array)
    return result.astype(input_array.dtype)


def save_bin(data, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.tofile(path)


def gen_golden_data(input_type=1, repeat=32):
    data_type = np.float16 if input_type == 1 else np.float32
    input_array = construct_boundary_inputs(data_type, repeat)
    golden_array = compute_exp_golden(input_array)

    # 保存文件
    # type_name = np.dtype(data_type).name
    # save_bin(input_array, f"./input/input_{type_name}.bin")
    # save_bin(golden_array, f"./output/golden_{type_name}.bin")
    save_bin(input_array, f"./input/input_x.bin")
    save_bin(golden_array, f"./output/golden.bin")

    # 打印每种特殊值及其对应的 exp 结果
    special_names = ['min', 'max', '+0', '-0', 'NaN', '+INF', '-INF']
    print(f"Constructed input ({len(input_array)} elements, {repeat} each):\n")
    for i, name in enumerate(special_names):
        idx = i * repeat
        inp_val = input_array[idx]
        out_val = golden_array[idx]
        print(f"input({name}) = {inp_val} --> output = {out_val}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate boundary input and golden data for Exp operator.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2],
                        help="Input data type: 1=float16, 2=float32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    # parser.add_argument('-repeat', type=int, default=32,
    #                     help="Number of repetitions per special value")
    args = parser.parse_args()

    gen_golden_data(args.inputType)