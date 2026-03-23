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

import sys
import argparse
import numpy as np

C0_SIZE = 32  # 一个小分形每行的字节数
relative_tol = 1e-6
absolute_tol = 1e-9
error_tol = 1e-4


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


def get_valid_indices(params, sizeof_T, output_size):
    """根据ND2NZ算法计算有效数据位置"""
    ndNum = params['ndNum']
    nValue = params['nValue']
    dValue = params['dValue']
    dstNzC0Stride = params['dstNzC0Stride']
    dstNzNStride = params['dstNzNStride']
    dstNzMatrixStride = params['dstNzMatrixStride']
    
    elements_per_C0_SIZE = C0_SIZE // sizeof_T
    
    valid_indices = []
    for i in range(ndNum):
        dst_nd_addr = dstNzMatrixStride * i * sizeof_T
        for j in range(nValue):
            dst_n_addr = dst_nd_addr + j * dstNzNStride * C0_SIZE
            for k in range((dValue * sizeof_T + C0_SIZE - 1) // C0_SIZE):
                dst_block_addr = dst_n_addr + k * dstNzC0Stride * C0_SIZE
                dst_elem_start = dst_block_addr // sizeof_T
                for m in range(elements_per_C0_SIZE):
                    dst_idx = dst_elem_start + m
                    if dst_idx < output_size:
                        valid_indices.append(dst_idx)
    
    return np.array(valid_indices)


def verify_result(scenario_num, output_file, golden_file):
    # 根据scenario_num选择数据类型
    if scenario_num in [1, 4, 7]:
        output_type = np.float16
    elif scenario_num in [2, 5]:
        output_type = np.float32
    elif scenario_num in [3, 6]:
        output_type = np.int8
    
    # 读取数据
    output = np.fromfile(output_file, dtype=output_type).reshape(-1)
    golden = np.fromfile(golden_file, dtype=output_type).reshape(-1)
    
    # 获取ND2NZ参数
    params = get_nd2nz_params(scenario_num)
    sizeof_T = np.dtype(output_type).itemsize
    
    # 计算有效数据位置
    valid_indices = get_valid_indices(params, sizeof_T, len(golden))
    
    # 提取有效位置的数据
    output_valid = output[:len(valid_indices)] if len(output) < len(valid_indices) else output[valid_indices]
    golden_valid = golden[valid_indices]
    
    # 对比有效数据
    different_element_results = np.isclose(
        output_valid,
        golden_valid,
        rtol=relative_tol,
        atol=absolute_tol,
        equal_nan=True
    )
    different_element_indexes = np.where(different_element_results == False)[0]
    
    # 打印错误详情
    for index in range(len(different_element_indexes)):
        valid_idx = different_element_indexes[index]
        real_index = valid_indices[valid_idx]
        golden_data = golden_valid[valid_idx]
        output_data = output_valid[valid_idx]
        rdiff = abs(output_data - golden_data) / golden_data if golden_data != 0 else abs(output_data - golden_data)
        print(f"data index: {real_index:06d}, expected: {golden_data:.9f}, actual: {output_data:.9f}, rdiff: {rdiff:.6f}")
        if index == 100:
            break
    
    error_ratio = float(len(different_element_indexes)) / len(valid_indices)
    return error_ratio <= error_tol


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenario_num', type=int, default=1, choices=range(1, 8))
    parser.add_argument('output', type=str)
    parser.add_argument('golden', type=str)
    args = parser.parse_args()
    
    try:
        res = verify_result(args.scenario_num, args.output, args.golden)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)