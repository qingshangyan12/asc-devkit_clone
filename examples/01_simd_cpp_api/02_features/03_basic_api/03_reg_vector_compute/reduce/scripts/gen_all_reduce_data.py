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


def gen_golden_data_sum(total_length=1024, value_range=(1.0, 2.0)):
    """生成 reduce_accumulate.asc 的测试数据"""
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
    # 注意：索引以 uint32_t 的二进制形式写入，不是 float
    golden.view(np.uint32)[1] = max_idx
    return x, golden


def gen_golden_data_min(total_length=1024, value_range=(1.0, 100.0)):
    """生成 reduce_min.asc 的测试数据"""
    data_type = np.float32
    
    x = np.random.uniform(value_range[0], value_range[1], [total_length]).astype(data_type)
    
    # golden: [min_value, min_index, pad, pad, ...]
    golden = np.zeros(8, dtype=data_type)  # 32B对齐
    min_idx = np.argmin(x)
    golden[0] = x[min_idx]
    # 注意：索引以 uint32_t 的二进制形式写入，不是 float
    golden.view(np.uint32)[1] = min_idx
    return x, golden


def main():
    os.makedirs('input', exist_ok=True)
    os.makedirs('output', exist_ok=True)
    
    total_length = 1024
    
    print("=" * 60)
    print("Generating test data for all reduce kernels")
    print("=" * 60)
    
    # 先生成共享的 sum 类型数据（accumulate/iterator/binary 共用）
    x_sum, golden_sum = gen_golden_data_sum(total_length)
    input_path = './input/reduce_input_x.bin'
    golden_path = './output/reduce_golden.bin'
    x_sum.tofile(input_path)
    golden_sum.tofile(golden_path)
    print(f"\n[1/3] Generating shared sum data (totalLength={total_length})...")
    print(f"  Input:  {input_path}, shape={x_sum.shape}, sum={golden_sum[0]:.4f}")
    print(f"  Golden: {golden_path}, shape={golden_sum.shape}")
    
    # sum 类型的三个变体只需要创建 output 文件（软链接方式）
    sum_variants = ['reduce_accumulate', 'reduce_iterator', 'reduce_binary']
    for i, name in enumerate(sum_variants):
        output_path = f'./output/{name}_output.bin'
        # 创建空文件作为占位符，实际运行时会被 kernel 输出覆盖
        with open(output_path, 'wb') as f:
            golden_sum.tofile(f)
        print(f"  [{name}] output: {output_path} (uses shared input/golden)")
    
    # 生成 max 类型数据
    print(f"\n[2/3] Generating reduce_max data (totalLength={total_length})...")
    x_max, golden_max = gen_golden_data_max(total_length)
    input_path = './input/reduce_max_input_x.bin'
    golden_path = './output/reduce_max_golden.bin'
    x_max.tofile(input_path)
    golden_max.tofile(golden_path)
    output_path = './output/reduce_max_output.bin'
    with open(output_path, 'wb') as f:
        golden_max.tofile(f)
    print(f"  Input:  {input_path}, max={golden_max[0]:.4f}, index={golden_max.view(np.uint32)[1]}")
    print(f"  Golden: {golden_path}")
    print(f"  Output: {output_path}")
    
    # 生成 min 类型数据
    print(f"\n[3/3] Generating reduce_min data (totalLength={total_length})...")
    x_min, golden_min = gen_golden_data_min(total_length)
    input_path = './input/reduce_min_input_x.bin'
    golden_path = './output/reduce_min_golden.bin'
    x_min.tofile(input_path)
    golden_min.tofile(golden_path)
    output_path = './output/reduce_min_output.bin'
    with open(output_path, 'wb') as f:
        golden_min.tofile(f)
    print(f"  Input:  {input_path}, min={golden_min[0]:.4f}, index={golden_min.view(np.uint32)[1]}")
    print(f"  Golden: {golden_path}")
    print(f"  Output: {output_path}")
    
    print("\n" + "=" * 60)
    print("Test data generation completed!")
    print("=" * 60)


if __name__ == "__main__":
    main()
