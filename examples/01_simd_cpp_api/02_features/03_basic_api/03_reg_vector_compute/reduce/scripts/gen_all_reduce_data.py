#!/usr/bin/env python3
"""
生成三种reduce方案的输入数据和golden数据
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

def compute_golden_max_proper(input_data):
    """计算reduce_max的golden数据(正确方式): 每行的最大值和索引"""
    golden = []
    for row in range(DIM_A):
        row_max = float('-inf')
        row_max_idx = 0
        for col in range(DIM_R):
            idx = row * DIM_R + col
            if input_data[idx] > row_max:
                row_max = input_data[idx]
                row_max_idx = col
        golden.append(row_max)
        golden.append(row_max_idx)  # 索引作为float存储，后面会转换
    # 转换为 float32 数组后，将索引部分转换为 uint32 再转回 float32
    golden_f32 = np.array(golden, dtype=np.float32)
    # 索引在偶数位置(1, 3, 5...)，需要转换为uint32
    for i in range(1, len(golden_f32), 2):
        golden_f32[i] = float(int(golden_f32[i]))
    return golden_f32

def compute_golden_min(input_data):
    """计算reduce_min的golden数据(正确方式): 每行的最小值和索引"""
    golden = []
    for row in range(DIM_A):
        row_min = float('+inf')
        row_min_idx = 0
        for col in range(DIM_R):
            idx = row * DIM_R + col
            if input_data[idx] < row_min:
                row_min = input_data[idx]
                row_min_idx = col
        golden.append(row_min)
        golden.append(float(row_min_idx))
    golden_f32 = np.array(golden, dtype=np.float32)
    for i in range(1, len(golden_f32), 2):
        golden_f32[i] = float(int(golden_f32[i]))
    return golden_f32

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
    
    # ============================================
    # 生成 reduce_max 数据: 20行，每行528个float -> 每行[max_value, max_index]
    # ============================================
    print("\n" + "=" * 60)
    print("Generating reduce_max data...")
    print("=" * 60)
    
    golden_max = compute_golden_max_proper(input_data)
    golden_max_path = os.path.join(OUTPUT_DIR, "reduce_max_golden.bin")
    with open(golden_max_path, 'wb') as f:
        golden_max.tofile(f)
    print(f"  Golden: {golden_max_path}")
    print(f"  Shape: {golden_max.shape}, layout: [v0, idx0, v1, idx1, ...]")
    
    # 生成reduce_max的输入文件
    reduce_max_input_path = os.path.join(INPUT_DIR, "reduce_max_input_x.bin")
    with open(reduce_max_input_path, 'wb') as f:
        for value in input_data:
            f.write(struct.pack('f', value))
    print(f"  Input:  {reduce_max_input_path}")
    
    # 生成reduce_max的输出占位文件
    reduce_max_output_path = os.path.join(OUTPUT_DIR, "reduce_max_output.bin")
    with open(reduce_max_output_path, 'wb') as f:
        golden_max.tofile(f)
    print(f"  Output: {reduce_max_output_path}")
    
    # 打印前3行的max结果
    print("  First 3 rows results:")
    for i in range(3):
        max_val = golden_max[i * 2]
        max_idx = int(golden_max[i * 2 + 1])
        print(f"    Row {i}: max={max_val:.2f}, index={max_idx}")
    
    # ============================================
    # 生成 reduce_min 数据: 20行，每行528个float -> 每行[min_value, min_index]
    # ============================================
    print("\n" + "=" * 60)
    print("Generating reduce_min data...")
    print("=" * 60)
    
    golden_min = compute_golden_min(input_data)
    golden_min_path = os.path.join(OUTPUT_DIR, "reduce_min_golden.bin")
    with open(golden_min_path, 'wb') as f:
        golden_min.tofile(f)
    print(f"  Golden: {golden_min_path}")
    print(f"  Shape: {golden_min.shape}, layout: [v0, idx0, v1, idx1, ...]")
    
    # 生成reduce_min的输入文件
    reduce_min_input_path = os.path.join(INPUT_DIR, "reduce_min_input_x.bin")
    with open(reduce_min_input_path, 'wb') as f:
        for value in input_data:
            f.write(struct.pack('f', value))
    print(f"  Input:  {reduce_min_input_path}")
    
    # 生成reduce_min的输出占位文件
    reduce_min_output_path = os.path.join(OUTPUT_DIR, "reduce_min_output.bin")
    with open(reduce_min_output_path, 'wb') as f:
        golden_min.tofile(f)
    print(f"  Output: {reduce_min_output_path}")
    
    # 打印前3行的min结果
    print("  First 3 rows results:")
    for i in range(3):
        min_val = golden_min[i * 2]
        min_idx = int(golden_min[i * 2 + 1])
        print(f"    Row {i}: min={min_val:.2f}, index={min_idx}")
    
    print("\n" + "=" * 60)
    print("All reduce data generation completed!")
    print("=" * 60)

if __name__ == "__main__":
    main()
