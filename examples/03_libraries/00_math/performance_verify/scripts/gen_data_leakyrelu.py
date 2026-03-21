#!/usr/bin/env python3
# coding=utf-8
"""
LeakyRelu API 测试数据生成模块
功能：生成 LeakyRelu 指令的输入数据和 golden 参考数据
LeakyRelu: dst = x >= 0 ? x : x × alpha
"""

import numpy as np
from gen_data_utils import DATA_TYPE_NP_MAP, save_bin

# LeakyRelu API 数据范围配置
DATA_RANGE_CONFIG = {
    "half":  {"min": -100.0, "max": 100.0},
    "float": {"min": -100.0, "max": 100.0},
    "int16": {"min": -100,   "max": 100},
    "int32": {"min": -100,   "max": 100},
}


def compute_leakyrelu_golden(a, alpha):
    """使用 NumPy 计算 LeakyRelu 的 golden 结果"""
    result = np.where(a >= 0, a, a * alpha)
    return result.astype(a.dtype)


def gen_leakyrelu_data(input_type, input_size):
    """
    生成 LeakyRelu 测试数据
    
    Args:
        input_type: 数据类型编号 (1=half, 2=float, 3=int16, 4=int32)
        input_size: 元素数量
    """
    data_type = DATA_TYPE_NP_MAP[input_type]
    dtype_name = {1: "half", 2: "float", 3: "int16", 4: "int32"}.get(input_type, "half")
    type_name = np.dtype(data_type).name
    
    config = DATA_RANGE_CONFIG.get(dtype_name, {"min": -100.0, "max": 100.0})
    min_val, max_val = config["min"], config["max"]
    
    # 生成输入和 alpha
    if np.issubdtype(data_type, np.integer):
        input_x = np.random.uniform(int(min_val), int(max_val), input_size).astype(data_type)
        alpha = np.array(1, dtype=data_type)
    else:
        input_x = np.random.uniform(min_val, max_val, input_size).astype(data_type)
        alpha = np.array(0.01, dtype=data_type)
    
    input_y = np.full(input_size, alpha, dtype=data_type)
    golden = compute_leakyrelu_golden(input_x, alpha)
    
    save_bin(input_x, f"./input/input_x.bin")
    save_bin(input_y, f"./input/input_y.bin")
    save_bin(golden, f"./output/golden.bin")
    
    print(f"  Input: {input_x.shape}, dtype={type_name}, range=[{min_val}, {max_val}]")
    print(f"  Alpha: {float(alpha):.4f}")
    print(f"  Output: [{float(golden.min()):.2f}, {float(golden.max()):.2f}]")
