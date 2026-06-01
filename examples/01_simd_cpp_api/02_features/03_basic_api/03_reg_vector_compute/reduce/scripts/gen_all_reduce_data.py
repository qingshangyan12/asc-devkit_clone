#!/usr/bin/env python3
"""
生成三种reduce求和方案的输入数据和golden数据
case: 20 * (512 + 16) -> 20 * 1
"""

import struct
import os
import numpy as np

# 配置参数
DIM_A = 20
DIM_R = 528  # 512 + 16
TOTAL_LENGTH = DIM_A * DIM_R
OUTPUT_COUNT = DIM_A

# reduce_max/min 使用 1024 个输入值
REDUCE_MAX_MIN_LENGTH = 1024

# 文件路径: 使用当前工作目录，而不是脚本所在目录
CWD = os.getcwd()
INPUT_DIR = os.path.join(CWD, "input")
OUTPUT_DIR = os.path.join(CWD, "output")

def ensure_dirs():
    """确保输入输出目录存在"""
    os.makedirs(INPUT_DIR, exist_ok=True)
    os.makedirs(OUTPUT_DIR, exist_ok=True)

def generate_input_data():
    """生成输入数据: 20行，每行528个float"""
    data = []
    for i in range(TOTAL_LENGTH):
        # 使用简单的模式填充
        value = float(i % 256)
        data.append(value)
    return data

def generate_input_data_max_min():
    """生成 reduce_max/min 的输入数据: 1024个float"""
    data = []
    for i in range(REDUCE_MAX_MIN_LENGTH):
        value = float(i % 256)
        data.append(value)
    return data

def compute_golden(input_data):
    """计算golden数据: 每行512+16个float的和"""
    golden = []
    for row in range(DIM_A):
        row_sum = 0.0
        for col in range(DIM_R):
            idx = row * DIM_R + col
            row_sum += input_data[idx]
        golden.append(row_sum)
    return golden

def write_binary_file(filepath, data):
    """写入二进制文件"""
    with open(filepath, 'wb') as f:
        for value in data:
            f.write(struct.pack('f', value))
    print(f"Written: {filepath}")

def compute_golden_max(input_data):
    """计算reduce_max的golden数据: [max_value, max_index]"""
    data_array = np.array(input_data, dtype=np.float32)
    max_idx = np.argmax(data_array)
    max_val = data_array[max_idx]
    
    # golden: [max_value, max_index, pad, pad, ...] (8个float对齐)
    golden = np.zeros(8, dtype=np.float32)
    golden[0] = max_val
    golden.view(np.uint32)[1] = max_idx
    return golden

def compute_golden_min(input_data):
    """计算reduce_min的golden数据: [min_value, min_index]"""
    data_array = np.array(input_data, dtype=np.float32)
    min_idx = np.argmin(data_array)
    min_val = data_array[min_idx]
    
    # golden: [min_value, min_index, pad, pad, ...] (8个float对齐)
    golden = np.zeros(8, dtype=np.float32)
    golden[0] = min_val
    golden.view(np.uint32)[1] = min_idx
    return golden

def main():
    ensure_dirs()
    
    # 生成输入数据
    input_data = generate_input_data()
    
    # 生成golden数据
    golden_data = compute_golden(input_data)
    
    # 生成reduce_sum的输入和golden文件
    input_path = os.path.join(INPUT_DIR, "reduce_sum_input_x.bin")
    write_binary_file(input_path, input_data)
    
    golden_path = os.path.join(OUTPUT_DIR, "reduce_sum_golden.bin")
    write_binary_file(golden_path, golden_data)
    
    print(f"  Input: {input_path}")
    print(f"  Golden: {golden_path}")
    
    print(f"\nGenerated reduce_sum input and golden files")
    print(f"Input shape: ({DIM_A}, {DIM_R}), Output shape: ({OUTPUT_COUNT},)")
    print(f"Total input elements: {TOTAL_LENGTH}")
    
    # ============================================
    # 生成 reduce_max 数据: 1024个float -> [max_value, max_index]
    # ============================================
    print("\n" + "=" * 60)
    print("Generating reduce_max data (1024 input elements)...")
    print("=" * 60)
    
    max_input = generate_input_data_max_min()
    golden_max = compute_golden_max(max_input)
    
    # 生成reduce_max的输入文件
    reduce_max_input_path = os.path.join(INPUT_DIR, "reduce_max_input_x.bin")
    write_binary_file(reduce_max_input_path, max_input)
    
    # 生成reduce_max的golden和output文件
    reduce_max_golden_path = os.path.join(OUTPUT_DIR, "reduce_max_golden.bin")
    with open(reduce_max_golden_path, 'wb') as f:
        golden_max.tofile(f)
    print(f"  Golden: {reduce_max_golden_path}")
    
    reduce_max_output_path = os.path.join(OUTPUT_DIR, "reduce_max_output.bin")
    with open(reduce_max_output_path, 'wb') as f:
        golden_max.tofile(f)
    print(f"  Output: {reduce_max_output_path}")
    
    print(f"  Max value: {golden_max[0]:.2f}, Max index: {golden_max.view(np.uint32)[1]}")
    
    # ============================================
    # 生成 reduce_min 数据: 1024个float -> [min_value, min_index]
    # ============================================
    print("\n" + "=" * 60)
    print("Generating reduce_min data (1024 input elements)...")
    print("=" * 60)
    
    min_input = generate_input_data_max_min()
    golden_min = compute_golden_min(min_input)
    
    # 生成reduce_min的输入文件
    reduce_min_input_path = os.path.join(INPUT_DIR, "reduce_min_input_x.bin")
    write_binary_file(reduce_min_input_path, min_input)
    
    # 生成reduce_min的golden和output文件
    reduce_min_golden_path = os.path.join(OUTPUT_DIR, "reduce_min_golden.bin")
    with open(reduce_min_golden_path, 'wb') as f:
        golden_min.tofile(f)
    print(f"  Golden: {reduce_min_golden_path}")
    
    reduce_min_output_path = os.path.join(OUTPUT_DIR, "reduce_min_output.bin")
    with open(reduce_min_output_path, 'wb') as f:
        golden_min.tofile(f)
    print(f"  Output: {reduce_min_output_path}")
    
    print(f"  Min value: {golden_min[0]:.2f}, Min index: {golden_min.view(np.uint32)[1]}")
    
    print("\n" + "=" * 60)
    print("All reduce data generation completed!")
    print("=" * 60)

if __name__ == "__main__":
    main()
