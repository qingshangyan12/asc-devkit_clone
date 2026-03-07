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
import numpy as np
import sys
import argparse

def gen_golden_data(scenario_num=1):
    # 根据scenario_num选择数据类型
    if scenario_num == 1 or scenario_num == 4:
        input_type = np.dtype("float16")
    elif scenario_num == 2 or scenario_num == 5:
        input_type = np.dtype("float32")
    elif scenario_num == 3 or scenario_num == 6:
        input_type = np.dtype("int8")
    
    output_type = input_type

    # 生成golden数据（按照ND2NZ布局）
    golden = []
    
    if scenario_num == 4:
        # 生成输入数据，shape为32*32
        input_x = np.zeros((32, 32), dtype=input_type)
        # 前30列为全1数据
        input_x[:, :30] = 1
        # 后2列保持为0（不需要定义，可以是随机，但这里设为0）
        
        # 常规情况：根据数据类型调整分形方式，只操作列，每列32B
        # 每个小分形的列是32B，计算每列的元素数
        sizeof_T = input_type.itemsize
        elements_per_col = 32 // sizeof_T  # 32B / 每个元素的大小
        
        # 计算小分型的shape（高度为整个矩阵的高度，宽度为32B对应的元素数）
        fractal_shape = [32, elements_per_col]
        
        # 计算行方向的分形数量
        row_num = 32 // 16
        # 计算列方向的分形数量
        col_num = 32 * sizeof_T // 32  # 总字节数 / 每个分形的列字节数
        
        print(f"Data type: {input_type}, sizeof(T): {sizeof_T}, fractal shape: {fractal_shape}")
        print(f"row_num: {row_num}, col_num: {col_num}")
        
        # 按照ND2NZ布局生成golden数据
        block_size = 16  # 每个分形的行数固定为16
        for col in range(col_num):
            for row in range(row_num):
                # 计算当前块的列范围
                col_start = col * elements_per_col
                col_end = (col + 1) * elements_per_col
                # 计算当前块的行范围
                row_start = row * block_size
                row_end = (row + 1) * block_size
                # 处理当前块的数据
                tmp_mat = input_x[row_start:row_end, col_start:col_end]
                # 后两列设为0
                if col_end > 30:
                    # 找到后两列的位置并设为0
                    tmp_mat[:, max(0, 30 - col_start):] = 0
                golden += tmp_mat.flatten().tolist()
    else:
        # 生成输入数据
        # 其他情况使用32x32的shape
        input_x = np.arange(1, 32 * 32 + 1).reshape(32, 32)
        input_x = input_x.astype(input_type)
        
        # 常规情况：根据数据类型调整分形方式，只操作列，每列32B
        # 每个小分形的列是32B，计算每列的元素数
        sizeof_T = input_type.itemsize
        elements_per_col = 32 // sizeof_T  # 32B / 每个元素的大小
        
        # 计算小分型的shape（高度为整个矩阵的高度，宽度为32B对应的元素数）
        fractal_shape = [32, elements_per_col]
        
        # 计算行方向的分形数量
        row_num = 32 // 16
        # 计算列方向的分形数量
        col_num = 32 * sizeof_T // 32  # 总字节数 / 每个分形的列字节数
        
        print(f"Data type: {input_type}, sizeof(T): {sizeof_T}, fractal shape: {fractal_shape}")
        print(f"row_num: {row_num}, col_num: {col_num}")
        
        # 按照ND2NZ布局生成golden数据
        block_size = 16  # 每个分形的行数固定为16
        for col in range(col_num):
            for row in range(row_num):
                # 计算当前块的列范围
                col_start = col * elements_per_col
                col_end = (col + 1) * elements_per_col
                # 计算当前块的行范围
                row_start = row * block_size
                row_end = (row + 1) * block_size
                # 处理当前块的数据
                tmp_mat = input_x[row_start:row_end, col_start:col_end]
                golden += tmp_mat.flatten().tolist()
    
    golden = np.array(golden, dtype=output_type)
    
    # 创建目录
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    # 保存数据
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 6))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)


