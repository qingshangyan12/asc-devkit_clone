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

# import os
# import argparse
# import numpy as np

# def get_range_by_dtype(data_type):
#     """
#     根据NumPy数据类型获取其数值范围（最小值和最大值）
    
#     参数:
#         data_type: NumPy数据类型或类型对象
        
#     返回:
#         tuple: (最小值, 最大值)
#     """
#     try:
#         if data_type == np.float16 or data_type == np.float32 or data_type == np.float64:
#             return np.finfo(data_type).min, np.finfo(data_type).max
#         else:
#             return np.iinfo(data_type).min, np.iinfo(data_type).max
#     except ValueError as e:
#         print(f"Unsupported data type: {data_type}")
#         raise e


# def gen_golden_data(input_type=1, input_size=1024):
#     data_type = np.float16 if input_type == 1 else np.float32
#     src_shape = [input_size]
#     np.random.seed(9)
#     min_val, max_val = get_range_by_dtype(data_type)
#     x1_gm = np.random.uniform(min_val, max_val, src_shape).astype(data_type)

#     # 计算自然指数，忽略溢出警告
#     with np.errstate(over='ignore', under='ignore'):
#         golden = np.exp(x1_gm).astype(data_type)

#     os.makedirs("input", exist_ok=True)
#     x1_gm.tofile("./input/input_x.bin")
#     os.makedirs("output", exist_ok=True)
#     golden.tofile("./output/golden.bin")

import os
import argparse
import numpy as np

def get_dtype_limits(data_type):
    """
    获取指定数据类型的数值范围（MIN, MAX），以及特殊值（+0, -0, NaN, +INF, -INF）
    """
    finfo = np.finfo(data_type)
    limits = {
        'min': finfo.min,          # 最小正规格化值      # MIN
        'max': finfo.max,          # 最大正规格化值      # MAX
        'tiny': finfo.tiny,        # 最小正值
        'eps': finfo.eps,          # 精度
        'pos_zero': np.array(+0.0, dtype=data_type),    # +0.0
        'neg_zero': np.array(-0.0, dtype=data_type),    # -0.0
        'nan': np.array(np.nan, dtype=data_type),       # NaN
        'pos_inf': np.array(np.inf, dtype=data_type),   # +INF
        'neg_inf': np.array(-np.inf, dtype=data_type),  # -INF
    }
    return limits


def construct_boundary_inputs(data_type):
    """
    构造包含边界值和特殊值的输入数组
    """
    limits = get_dtype_limits(data_type)
    inputs = [
        limits['min'],      # 最小正规格化值
        limits['max'],      # 最大正规格化值
        limits['tiny'],     # 最小正值
        limits['pos_zero'], # +0
        limits['neg_zero'], # -0
        limits['nan'],      # NaN
        limits['pos_inf'],  # +INF
        limits['neg_inf'],  # -INF
    ]
    return np.array(inputs, dtype=data_type)


def compute_exp_golden(input_array):
    """
    计算自然指数的黄金结果，忽略溢出/下溢警告
    """
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.exp(input_array)
    return result.astype(input_array.dtype)


def save_bin(data, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.tofile(path)


def gen_golden_data(input_type=1):
    """
    主函数：根据类型生成输入和exp真值
    input_type: 1 -> float16, 2 -> float32
    """
    data_type = np.float16 if input_type == 1 else np.float32
    input_array = construct_boundary_inputs(data_type)
    golden_array = compute_exp_golden(input_array)

    # 保存文件
    save_bin(input_array, f"./input/input_{np.dtype(data_type).name}.bin")
    save_bin(golden_array, f"./output/golden_{np.dtype(data_type).name}.bin")

    # 打印对照表方便查看
    print(f"{'Value':>15} | {'Hex':>20} | {'exp(Value)':>15} | {'exp(Hex)':>20}")
    print("-" * 75)
    for val, gold in zip(input_array, golden_array):
        print(f"{val:>15.6g} | {val.tobytes().hex():>20} | {gold:>15.6g} | {gold.tobytes().hex():>20}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate boundary input and golden data for Exp operator.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2],
                        help="Input data type: 1=float16, 2=float32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    args = parser.parse_args()

    gen_golden_data(args.inputType)

# if __name__ == "__main__":
#     parser = argparse.ArgumentParser(description="Generate golden data for Exp operator.")
#     parser.add_argument('-inputType', type=int, default=1, choices=[1, 2],
#                         help="Input data type: 1=float16, 2=float32")
#     parser.add_argument('-inputSize', type=int, required=True,
#                         help="Total number of elements in the tensor")
#     args = parser.parse_args()
#     gen_golden_data(args.inputType, args.inputSize)
