#!/usr/bin/env python3
# coding=utf-8
"""
Exp API 测试数据生成模块
功能：生成 Exp 指令的输入数据和 golden 参考数据
"""

import numpy as np
from gen_data_utils import DATA_TYPE_NP_MAP, save_bin

# Exp API 数据范围配置 (适配 exp 特性，避免结果全 0 或全 inf)
DATA_RANGE_CONFIG = {
    "half":  {"min": -10.0, "max": 10.0},
    "float": {"min": -10.0, "max": 10.0},
    "int16": {"min": -10,   "max": 10},
    "int32": {"min": -10,   "max": 10},
}


def compute_exp_golden(input_array):
    """使用 NumPy 计算 exp 的 golden 结果"""
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.exp(input_array)
    return result.astype(input_array.dtype)


def gen_exp_data(input_type, input_size):
    """
    生成 Exp 测试数据
    
    Args:
        input_type: 数据类型编号 (1=half, 2=float, 3=int16, 4=int32)
        input_size: 元素数量
    """
    data_type = DATA_TYPE_NP_MAP[input_type]
    dtype_name = {1: "half", 2: "float", 3: "int16", 4: "int32"}.get(input_type, "half")
    type_name = np.dtype(data_type).name
    
    # 根据数据类型获取数据范围
    config = DATA_RANGE_CONFIG.get(dtype_name, {"min": -10.0, "max": 10.0})
    min_val, max_val = config["min"], config["max"]
    
    # 生成随机输入数据
    if np.issubdtype(data_type, np.integer):
        input_x = np.random.uniform(int(min_val), int(max_val), input_size).astype(data_type)
    else:
        input_x = np.random.uniform(min_val, max_val, input_size).astype(data_type)
    
    # 计算 golden 结果
    golden = compute_exp_golden(input_x)
    
    # 保存到文件
    save_bin(input_x, f"./input/input_x.bin")
    save_bin(golden, f"./output/golden.bin")
    
    # 打印生成信息
    print(f"  Input: {input_x.shape}, dtype={type_name}, range=[{min_val}, {max_val}]")
    print(f"  Output: [{float(golden.min()):.2f}, {float(golden.max()):.2f}]")
