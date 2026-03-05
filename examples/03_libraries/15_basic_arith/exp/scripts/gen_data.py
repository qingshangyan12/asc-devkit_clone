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


def gen_golden_data(scenario_num=1, input_size=1024):
    input_type = np.float16
    output_type = input_type
    src_shape = [input_size]
    np.random.seed(9)
    min_val, max_val = get_range_by_dtype(input_type)
    if scenario_num == 1:
        print("[INFO] gen_golden_data, scenario_num = {}".format(scenario_num))
        x1_gm = np.random.uniform(min_val, max_val, src_shape).astype(input_type)

        # 分割数据
        cal_count = input_size
        src_exp_input = x1_gm[:cal_count].copy()
        src_ori = np.zeros(x1_gm.size - cal_count, dtype=input_type)
    else:
        pass
    
    # 计算指数，忽略溢出警告
    with np.errstate(over='ignore', under='ignore'):
        src_exp_result = np.exp(src_exp_input)
    
    # 组合结果
    golden = np.concatenate((src_exp_result, src_ori), axis=None).astype(output_type)

    os.makedirs("input", exist_ok=True)
    x1_gm.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 14))
    parser.add_argument('-inputSize', type=int)
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.inputSize)
