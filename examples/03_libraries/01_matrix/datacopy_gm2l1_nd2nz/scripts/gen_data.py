#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np
import argparse

C0_SIZE = 32  # 一个小分形每行的字节数

def get_nd2nz_params(scenario_num):
    """获取ND2NZ参数"""
    if scenario_num in [1, 2, 3]:
        # scenario 1-3: 单个16x32矩阵，对齐情况
        return {
            'ndNum': 1,
            'nValue': 16,
            'dValue': 32,
            'srcNdMatrixStride': 0,
            'srcDValue': 32,
            'dstNzC0Stride': 16,
            'dstNzNStride': 1,
            'dstNzMatrixStride': 0,
        }
    elif scenario_num in [4, 5, 6]:
        # scenario 4-6: 单个16x23矩阵，非对齐情况
        return {
            'ndNum': 1,
            'nValue': 16,
            'dValue': 23,
            'srcNdMatrixStride': 0,
            'srcDValue': 32,
            'dstNzC0Stride': 16,
            'dstNzNStride': 1,
            'dstNzMatrixStride': 0,
        }
    elif scenario_num == 7:
        # scenario 7: 两个8x23矩阵，非连续搬运
        return {
            'ndNum': 2,
            'nValue': 8,
            'dValue': 23,
            'srcNdMatrixStride': 384,
            'srcDValue': 32,
            'dstNzC0Stride': 37,
            'dstNzNStride': 2,
            'dstNzMatrixStride': 320,
        }

def gen_input_data(scenario_num, input_type):
    """生成输入数据"""
    params = get_nd2nz_params(scenario_num)
    nValue = params['nValue']
    dValue = params['dValue']
    
    if scenario_num == 7:
        # scenario 7: 输入大小为 20 * 32 = 640 个元素
        # 第一个矩阵：第0-255个元素（8行×32列）
        # 间隔区域：第256-383个元素（不关心）
        # 第二个矩阵：第384-639个元素（8行×32列）
        input_x = np.zeros(20 * 32, dtype=input_type)
        
        # 第一个矩阵：每行前dValue个为1
        for row in range(8):
            start = row * 32
            input_x[start:start + dValue] = 1
        
        # 第二个矩阵：每行前dValue个为1
        for row in range(8):
            start = 384 + row * 32
            input_x[start:start + dValue] = 1
    else:
        # scenario 1-6: 输入大小为 16 * 32 = 512 个元素
        input_x = np.zeros(16 * 32, dtype=input_type)
        
        # 每行前dValue个为1
        for row in range(nValue):
            start = row * 32
            input_x[start:start + dValue] = 1
    
    return input_x

def gen_golden_data(input_x, params, sizeof_T):
    """使用ND2NZ算法生成golden数据"""
    # 提取参数
    ndNum = params['ndNum']
    nValue = params['nValue']
    dValue = params['dValue']
    srcNdMatrixStride = params['srcNdMatrixStride']
    srcDValue = params['srcDValue']
    dstNzC0Stride = params['dstNzC0Stride']
    dstNzNStride = params['dstNzNStride']
    dstNzMatrixStride = params['dstNzMatrixStride']
    
    # 计算输出大小
    # alignedD = ceil(dValue / 16) * 16
    elements_per_C0_SIZE = C0_SIZE // sizeof_T
    alignedD = (dValue + elements_per_C0_SIZE - 1) // elements_per_C0_SIZE * elements_per_C0_SIZE
    output_size = dstNzC0Stride * alignedD
    golden = np.zeros(output_size, dtype=input_x.dtype)
    
    # 按照ND2NZ伪代码算法生成golden数据
    for i in range(ndNum):
        src_nd_addr = srcNdMatrixStride * i * sizeof_T
        dst_nd_addr = dstNzMatrixStride * i * sizeof_T
        
        for j in range(nValue):
            src_n_addr = src_nd_addr + j * srcDValue * sizeof_T
            dst_n_addr = dst_nd_addr + j * dstNzNStride * C0_SIZE
            
            # ceil(dValue * sizeof(T)) / 32
            for k in range((dValue * sizeof_T + C0_SIZE - 1) // C0_SIZE):
                src_block_addr = src_n_addr + k * C0_SIZE
                dst_block_addr = dst_n_addr + k * dstNzC0Stride * C0_SIZE
                
                # 转换成元素索引
                src_elem_start = src_block_addr // sizeof_T
                dst_elem_start = dst_block_addr // sizeof_T
                
                for m in range(elements_per_C0_SIZE):
                    src_idx = src_elem_start + m
                    dst_idx = dst_elem_start + m
                    if src_idx < len(input_x) and dst_idx < len(golden):
                        golden[dst_idx] = input_x[src_idx]
    
    return golden

def main(scenario_num=1):
    # 根据scenario_num选择数据类型
    if scenario_num in [1, 4, 7]:
        input_type = np.dtype("float16")
    elif scenario_num in [2, 5]:
        input_type = np.dtype("float32")
    elif scenario_num in [3, 6]:
        input_type = np.dtype("int8")
    
    # 获取ND2NZ参数
    params = get_nd2nz_params(scenario_num)
    sizeof_T = input_type.itemsize
    
    # 生成输入数据
    input_x = gen_input_data(scenario_num, input_type)
    
    # 生成golden数据
    golden = gen_golden_data(input_x, params, sizeof_T)
    
    # 创建目录
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    # 保存数据
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenario_num', type=int, default=1, choices=range(1, 8))
    args = parser.parse_args()
    main(args.scenario_num)