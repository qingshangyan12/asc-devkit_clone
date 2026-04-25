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

"""
测试数据生成脚本
支持生成所有 reduce 系列核函数的测试数据：
- reduce_accumulate.asc: 累加器方式归约求和，输出 [float]
- reduce_iterator.asc: 迭代器方式归约求和，输出 [float]
- reduce_binary.asc: 二分累加求和，输出 [float]
- reduce_max.asc: 归约求最大值+索引，输出 [float, uint32]
- reduce_min.asc: 归约求最小值+索引，输出 [float, uint32]
"""

import os
import numpy as np


def gen_golden_data_accumulate(total_length=1024, value_range=(1.0, 2.0)):
    """生成 reduce_accumulate.asc 的测试数据"""
    data_type = np.float32
    x = np.random.uniform(value_range[0], value_range[1], [total_length]).astype(data_type)
    golden = np.zeros(8, dtype=data_type)  # 32B对齐
    golden[0] = np.sum(x)
    return x, golden


def gen_golden_data_iterator(total_length=1024, value_range=(1.0, 2.0)):
    """生成 reduce_iterator.asc 的测试数据"""
    data_type = np.float32
    x = np.random.uniform(value_range[0], value_range[1], [total_length]).astype(data_type)
    golden = np.zeros(8, dtype=data_type)  # 32B对齐
    golden[0] = np.sum(x)
    return x, golden


def gen_golden_data_binary(total_length=1024, value_range=(1.0, 2.0)):
    """生成 reduce_binary.asc 的测试数据"""
    data_type = np.float32
    x = np.random.uniform(value_range[0], value_range[1], [total_length]).astype(data_type)
    golden = np.zeros(8, dtype=data_type)  # 32B对齐
    golden[0] = np.sum(x)
    return x, golden


def gen_golden_data_max(total_length=1024, value_range=(1.0, 100.0)):
    """生成 reduce_max.asc 的测试数据"""
    data_type = np.float32
    
    x = np.random.uniform(value_range[0], value_range[1], [total_length]).astype(data_type)
    
    # golden: [max_value, max_index, pad, pad, ...]
    golden = np.zeros(8, dtype=data_type)  # 32B对齐
    max_idx = np.argmax(x)
    golden[0] = x[max_idx]
    # 注意：索引通过 float 的方式写入
    golden[1] = float(max_idx)
    return x, golden


def gen_golden_data_min(total_length=1024, value_range=(1.0, 100.0)):
    """生成 reduce_min.asc 的测试数据"""
    data_type = np.float32
    
    x = np.random.uniform(value_range[0], value_range[1], [total_length]).astype(data_type)
    
    # golden: [min_value, min_index, pad, pad, ...]
    golden = np.zeros(8, dtype=data_type)  # 32B对齐
    min_idx = np.argmin(x)
    golden[0] = x[min_idx]
    # 注意：索引通过 float 的方式写入
    golden[1] = float(min_idx)
    return x, golden


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    input_dir = os.path.join(script_dir, 'input')
    output_dir = os.path.join(script_dir, 'output')
    
    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)
    
    total_length = 1024
    
    print("=" * 60)
    print("Generating test data for all reduce kernels")
    print("=" * 60)
    
    configs = [
        ('reduce_accumulate',   gen_golden_data_accumulate,   'sum'),
        ('reduce_iterator',     gen_golden_data_iterator,     'sum'),
        ('reduce_binary',       gen_golden_data_binary,       'sum'),
        ('reduce_max',   gen_golden_data_max,   'max'),
        ('reduce_min',   gen_golden_data_min,   'min'),
    ]
    
    for i, (name, gen_fn, op_type) in enumerate(configs):
        print(f"\n[{i+1}/{len(configs)}] Generating {name}.asc data (totalLength={total_length})...")
        x, golden = gen_fn(total_length)
        
        # 使用带前缀的唯一文件名，避免覆盖
        input_path = f'./input/{name}_input_x.bin'
        golden_path = f'./output/{name}_golden.bin'
        
        x.tofile(input_path)
        golden.tofile(golden_path)
        
        if op_type == 'sum':
            print(f"  Input:  {input_path}, shape={x.shape}, sum={golden[0]:.4f}")
            print(f"  Golden: {golden_path}, shape={golden.shape}")
        elif op_type == 'max':
            print(f"  Input:  {input_path}, shape={x.shape}, max={golden[0]:.4f}, index={int(golden[1])}")
            print(f"  Golden: {golden_path}, shape={golden.shape}")
        elif op_type == 'min':
            print(f"  Input:  {input_path}, shape={x.shape}, min={golden[0]:.4f}, index={int(golden[1])}")
            print(f"  Golden: {golden_path}, shape={golden.shape}")
    
    print("\n" + "=" * 60)
    print("Test data generation completed!")
    print("=" * 60)


if __name__ == "__main__":
    main()
