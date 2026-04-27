#!/usr/bin/env python3
"""
生成三种reduce方案的输入数据和golden数据
case: 20 * (512 + 16) -> 20 * 1
"""

import struct
import os

# 配置参数
DIM_A = 20
DIM_R = 528  # 512 + 16
TOTAL_LENGTH = DIM_A * DIM_R
OUTPUT_COUNT = DIM_A

# 文件路径
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
INPUT_DIR = os.path.join(SCRIPT_DIR, "..", "input")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "..", "output")

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

def main():
    ensure_dirs()
    
    # 生成输入数据
    input_data = generate_input_data()
    
    # 生成golden数据
    golden_data = compute_golden(input_data)
    
    # 写入输入文件
    input_files = [
        ("reduce_accumulate_input_x.bin", input_data),
        ("reduce_binary_input_x.bin", input_data),
        ("reduce_iterator_input_x.bin", input_data),
    ]
    
    for filename, data in input_files:
        filepath = os.path.join(INPUT_DIR, filename)
        write_binary_file(filepath, data)
    
    # 写入golden文件
    golden_files = [
        ("reduce_accumulate_golden.bin", golden_data),
        ("reduce_binary_golden.bin", golden_data),
        ("reduce_iterator_golden.bin", golden_data),
    ]
    
    for filename, data in golden_files:
        filepath = os.path.join(OUTPUT_DIR, filename)
        write_binary_file(filepath, data)
    
    print(f"\nGenerated {len(input_files)} input files and {len(golden_files)} golden files")
    print(f"Input shape: ({DIM_A}, {DIM_R}), Output shape: ({OUTPUT_COUNT},)")
    print(f"Total input elements: {TOTAL_LENGTH}")

if __name__ == "__main__":
    main()
