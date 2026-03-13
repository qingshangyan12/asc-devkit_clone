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

def get_range_by_dtype(data_type):
    """
    根据NumPy数据类型获取其数值范围（最小值和最大值）
    
    参数:
        data_type: NumPy数据类型或类型对象
        
    返回:
        tuple: (最小值, 最大值)
    """
    try:
        if data_type == np.float16 or data_type == np.float32 or data_type == np.float64:
            return np.finfo(data_type).min, np.finfo(data_type).max
        else:
            return np.iinfo(data_type).min, np.iinfo(data_type).max
    except ValueError as e:
        print(f"Unsupported data type: {data_type}")
        raise e


def gen_golden_data(input_type=1, input_size=1024):
    data_type = np.float16 if input_type == 1 else np.float32
    src_shape = [input_size]
    np.random.seed(9)
    min_val, max_val = get_range_by_dtype(data_type)
    x1_gm = np.random.uniform(min_val, max_val, src_shape).astype(data_type)

    # 计算自然指数，忽略溢出警告
    with np.errstate(over='ignore', under='ignore'):
        golden = np.exp(x1_gm).astype(data_type)

    os.makedirs("input", exist_ok=True)
    x1_gm.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate golden data for Exp operator.")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2],
                        help="Input data type: 1=float16, 2=float32")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    args = parser.parse_args()
    gen_golden_data(args.inputType, args.inputSize)
