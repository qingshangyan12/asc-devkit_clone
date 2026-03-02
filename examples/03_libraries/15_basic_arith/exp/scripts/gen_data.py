#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np

# def saturation(value, min_val, max_val, target_type):
#     """
#     将输入的浮点数进行饱和处理，并转换为目标类型
    
#     参数:
#         value: 输入值（标量或数组）
#         min_val: 最小值
#         max_val: 最大值
#         target_type: 目标数据类型
        
#     返回:
#         饱和处理并转换类型后的值
#     """
#     x_clamped = np.clip(value, min_val, max_val)
#     return np.round(x_clamped).astype(target_type)

def get_range_by_dtype(input_type):
    """
    根据NumPy数据类型获取其数值范围（最小值和最大值）
    
    参数:
        input_type: NumPy数据类型或类型对象
        
    返回:
        tuple: (最小值, 最大值)
    """
    try:
        if input_type == np.float16 or input_type == np.float32 or input_type == np.float64:
            return np.finfo(input_type).min, np.finfo(input_type).max
        else:
            return np.iinfo(input_type).min, np.iinfo(input_type).max
    except ValueError as e:
        print(f"Unsupported data type: {input_type}")
        raise e

def gen_golden_data():
    input_type = np.float16
    output_type = input_type
    src_shape = [8192]
    
    # 设置随机种子保证可重复性
    np.random.seed(0)
    
    cal_count = 8192
    
    # # 方法1: 在安全范围内生成输入数据（推荐）
    # safe_min, safe_max = get_safe_exp_range(input_type)
    # src = np.random.uniform(safe_min, safe_max, src_shape).astype(input_type)
    
    # 方法2: 在全范围内生成数据，然后使用saturation处理（替代方案）
    # 获取数据类型完整范围
    min_val, max_val = get_range_by_dtype(input_type)
    src = np.random.uniform(min_val, max_val, src_shape).astype(input_type)
    
    # 分割数据
    src_exp_input = src[:cal_count].copy()
    src_ori = np.zeros(src.size - cal_count, dtype=input_type)
    
    # 计算指数，忽略溢出警告
    with np.errstate(over='ignore', under='ignore'):
        src_exp_result = np.exp(src_exp_input)
    # src_exp_result = np.exp(src_exp_input)
    
    # 使用saturation函数处理溢出
    # 获取输出类型的范围
    output_min, output_max = get_range_by_dtype(output_type)
    
    # 对指数计算结果进行饱和处理
    src_exp_saturated = src_exp_result # 不能进行饱和处理
    # src_exp_saturated = saturation(
    #     src_exp_result, 
    #     output_min, 
    #     output_max, 
    #     output_type
    # )
    
    # 组合结果
    golden = np.concatenate((src_exp_saturated, src_ori), axis=None).astype(output_type)

    os.makedirs("input", exist_ok=True)
    src.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data()
