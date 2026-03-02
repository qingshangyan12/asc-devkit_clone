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
    """
    获取指定数据类型的数值范围及特殊值
    """
    if np.issubdtype(data_type, np.floating):
        finfo = np.finfo(data_type)
        return {
            'min': finfo.min,
            'max': finfo.max,
            'pos_zero': np.array(+0.0, dtype=data_type),
            'neg_zero': np.array(-0.0, dtype=data_type),
            'nan': np.array(np.nan, dtype=data_type),
            'pos_inf': np.array(np.inf, dtype=data_type),
            'neg_inf': np.array(-np.inf, dtype=data_type),
            # 'min': 1.0,
            # 'max': 2.0,
            # 'pos_zero': 3.0,
            # 'neg_zero': 4.0,
            # 'nan': 5.0,
            # 'pos_inf': 6.0,
            # 'neg_inf': 7.0,
        }
    elif np.issubdtype(data_type, np.integer):
        iinfo = np.iinfo(data_type)
        return {
            'min': np.array(iinfo.min, dtype=data_type),
            'max': np.array(iinfo.max, dtype=data_type),
            'pos_zero': np.array(+0, dtype=data_type),
            'neg_zero': np.array(-0, dtype=data_type),
            # 'min': 11,
            # 'max': 22,
            # 'pos_zero': 33,
            # 'neg_zero': 44,
        }
    else:
        raise ValueError(f"Unsupported data type: {data_type}")


def construct_special_values(data_type):
    """
    构造包含特殊值的数组
    """
    limits = get_dtype_limits(data_type)
    specials = [limits['min'], limits['max'], limits['pos_zero'], limits['neg_zero']]
    # 浮点额外加上 INF/-INF/NaN
    if np.issubdtype(data_type, np.floating):
        specials.extend([limits['nan'], limits['pos_inf'], limits['neg_inf']])
    return specials


def cartesian_product_inputs(data_type):
    """
    生成两个输入的笛卡尔积（n×n 组）
    """
    vals = construct_special_values(data_type)
    inputs_a = []
    inputs_b = []
    for a in vals:
        for b in vals:
            inputs_a.append(a)
            inputs_b.append(b)
    return np.array(inputs_a, dtype=data_type), np.array(inputs_b, dtype=data_type)


def compute_mul_golden(a, b):
    """
    计算乘积，忽略无效值警告
    """
    with np.errstate(divide='ignore', over='ignore', under='ignore', invalid='ignore'):
        result = np.multiply(a, b)
    return result.astype(a.dtype)


def save_bin(data, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.tofile(path)


def gen_golden_data(input_type=1):
    """
    主函数：根据类型生成输入和mul真值
    input_type: 1=float16, 2=float32, 3=int16, 4=int32
    """
    type_map = {
        1: np.float16,
        2: np.float32,
        3: np.int16,
        4: np.int32,
    }
    data_type = type_map[input_type]
    type_name = np.dtype(data_type).name

    a_arr, b_arr = cartesian_product_inputs(data_type)
    golden_arr = compute_mul_golden(a_arr, b_arr)

    save_bin(a_arr, f"./input/input_x.bin")
    save_bin(b_arr, f"./input/input_y.bin")
    save_bin(golden_arr, f"./output/golden.bin")

    # 打印每种特殊值的两两乘积示例
    special_names = []
    limits = get_dtype_limits(data_type)
    special_names.extend(['min', 'max', '+0', '-0'])
    if np.issubdtype(data_type, np.floating):
        special_names.extend(['NaN', '+INF', '-INF'])

    print(f"\nData type: {type_name}")
    print(f"Total combinations: {len(a_arr)} ({len(special_names)}x{len(special_names)})\n")
    print(f"{'A':>10} | {'B':>10} | {'A*B':>15}")
    print("-" * 45)
    for i, name_a in enumerate(special_names):
        for j, name_b in enumerate(special_names):
            idx = i * len(special_names) + j
            print(f"[{name_a}]{str(a_arr[idx]):>10} | [{name_b}]{str(b_arr[idx]):>10} | {str(golden_arr[idx]):>15}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate cartesian product input and golden data for Mul operator.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input data type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    args = parser.parse_args()

    gen_golden_data(args.inputType)