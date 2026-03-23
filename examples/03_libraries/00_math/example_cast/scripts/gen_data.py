#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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


def gen_golden_data(scenario_num=1, size=1024):
    """
    生成测试输入数据和真值数据
    
    参数:
        scenario_num: 测试场景编号
        size: 向量长度
    """
    # 根据场景设置数据类型和参数
    if scenario_num == 1:
        input_dtype = np.float32
        output_dtype = np.float16
    elif scenario_num == 2:
        input_dtype = np.float16
        output_dtype = np.float32
    elif scenario_num == 3:
        input_dtype = np.float32
        output_dtype = np.float32
    elif scenario_num == 4:
        input_dtype = np.float32
        output_dtype = np.float32
    elif scenario_num == 5:
        input_dtype = np.float32
        output_dtype = np.int32
    elif scenario_num == 6:
        input_dtype = np.int32
        output_dtype = np.float32
    else:
        raise ValueError(f"Unsupported scenario_num: {scenario_num}")
    
    # 设置随机种子，确保结果可真现
    np.random.seed(42)
    
    # 生成输入数据
    min_val, max_val = get_range_by_dtype(input_dtype)
    if input_dtype == np.int32:
        input_data = np.random.randint(int(min_val), int(max_val), [size]).astype(input_dtype)
    else:
        # 限制数据范围，避免溢出
        if input_dtype == np.float32:
            min_val = max(min_val, -10000.0)
            max_val = min(max_val, 10000.0)
        elif input_dtype == np.float16:
            min_val = max(min_val, -100.0)
            max_val = min(max_val, 100.0)
        input_data = np.random.uniform(min_val, max_val, [size]).astype(input_dtype)
    
    # 计算真值数据
    golden = input_data.astype(output_dtype)
    
    # 创建目录并保存数据
    os.makedirs("input", exist_ok=True)
    input_data.tofile("./input/input.bin")
    
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate golden data for Cast operator.")
    parser.add_argument('-scenarioNum', type=int, default=1,
                        help="Test scenario number (1=float->half, 2=half->float, 3=float->bfloat16, 4=bfloat16->float, 5=float->int32, 6=int32->float)")
    parser.add_argument('-size', type=int, default=1024,
                        help="Vector size")
    
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.size)
