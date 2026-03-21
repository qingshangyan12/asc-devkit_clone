#!/usr/bin/env python3
# coding=utf-8
"""
Ascend C API 性能测试 - 公共配置和工具函数
包含：API映射、数据类型映射、理论性能配置、通用工具函数
"""

import numpy as np

# ============================================================================
# API 配置
# ============================================================================

API_ID_EXP = 1        # Exp: 单目运算，dst = e^src
API_ID_MUL = 2        # Mul: 双目运算，dst = src0 × src1
API_ID_ADDS = 3       # Adds: 双目标量，dst = src + scalar
API_ID_LEAKYRELU = 4  # LeakyRelu: 双目标量，dst = x >= 0 ? x : x × alpha

# API 编号 -> 名称 映射
API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "adds",
    4: "leakyrelu",
}

# ============================================================================
# 数据类型配置
# ============================================================================

# 数据类型编号 -> 名称 映射
DATA_TYPE_ID_MAP = {
    1: "half",   # float16
    2: "float",  # float32
    3: "int16",
    4: "int32",
}

# 数据类型编号 -> NumPy dtype 映射
DATA_TYPE_NP_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}

# ============================================================================
# 理论性能配置 (element/cycle @ AI Core 频率)
# ============================================================================

THEORETICAL_PERFORMANCE = {
    ("exp", "half"): 32,
    ("exp", "float"): 32,
    ("mul", "half"): 128,
    ("mul", "float"): 64,
    ("mul", "int16"): 128,
    ("mul", "int32"): 64,
    ("adds", "half"): 128,
    ("adds", "float"): 64,
    ("adds", "int16"): 128,
    ("adds", "int32"): 64,
    ("leakyrelu", "half"): 128,
    ("leakyrelu", "float"): 64,
}

# ============================================================================
# 测试配置
# ============================================================================

REPEAT_TIMES = 1000  # 性能测试时指令重复执行次数


# ============================================================================
# 工具函数
# ============================================================================

def get_dtype_limits(data_type):
    """获取数据类型的取值范围 (min, max)"""
    if np.issubdtype(data_type, np.floating):
        finfo = np.finfo(data_type)
        return finfo.min, finfo.max
    elif np.issubdtype(data_type, np.integer):
        iinfo = np.iinfo(data_type)
        return iinfo.min, iinfo.max
    else:
        raise ValueError(f"Unsupported data type: {data_type}")


def generate_random_data(data_type, size):
    """
    生成指定数据类型和大小的随机测试数据
    
    Args:
        data_type: NumPy dtype
        size: 元素数量
    
    Returns:
        numpy.ndarray: 随机数据数组
    """
    min_val, max_val = get_dtype_limits(data_type)
    
    if np.issubdtype(data_type, np.floating):
        # 浮点数：限制在合理范围内
        safe_min = max(min_val, -10000.0)
        safe_max = min(max_val, 10000.0)
        return np.random.uniform(safe_min, safe_max, size).astype(data_type)
    else:
        # 整数：限制在合理范围内
        safe_min = max(min_val, -10000)
        safe_max = min(max_val, 10000)
        return np.random.uniform(safe_min, safe_max, size).astype(data_type)


def save_bin(data, path):
    """将数据保存为二进制文件"""
    import os
    os.makedirs(os.path.dirname(path) if os.path.dirname(path) else "./", exist_ok=True)
    data.tofile(path)


def print_performance_header(api_name, type_name, size):
    """打印测试头部信息"""
    print(f"\n{'='*80}")
    print(f"Performance Test | API: {api_name} | DataType: {type_name} | Size: {size}")
    print(f"{'='*80}")
