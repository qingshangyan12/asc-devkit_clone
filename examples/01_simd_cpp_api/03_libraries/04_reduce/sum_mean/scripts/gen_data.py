#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np
import argparse


def calculateSum(arr, dtype):
    """递归二分求和，模拟硬件归约过程"""
    n = len(arr)
    if n == 1:
        return arr
    if n % 2 != 0:
        newInput = np.zeros(n // 2 + 1).astype(dtype)
        for i in range(n // 2):
            newInput[i] = arr[i * 2] + arr[i * 2 + 1]
        newInput[n // 2] = arr[-1]
    else:
        newInput = np.zeros(n // 2).astype(dtype)
        for i in range(n // 2):
            newInput[i] = arr[i * 2] + arr[i * 2 + 1]
    return calculateSum(newInput, dtype)


def Sum(x, n, insum):
    """计算每行的和"""
    for i, _ in enumerate(x):
        res = calculateSum(x[i, :n], x.dtype)
        insum[i] = res


def Mean(x, n, insum, scalar):
    """计算每行的均值"""
    for i, _ in enumerate(x):
        res = calculateSum(x[i, :n], x.dtype)
        insum[i] = res * scalar


def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=Sum求和, 2=Mean求均值)
    """
    dtype = np.float32
    outter = 3      # 行数
    inner = 32      # 列数
    n = 32          # 实际元素个数
    
    # 生成随机输入数据
    x = np.random.randint(-100, 100, [outter, inner]).astype(dtype)
    
    # 计算输出对齐后的大小
    if dtype == np.float16:
        outter_pad = (outter * 2 + 31) // 32 * 32 // 2
    else:
        outter_pad = (outter * 4 + 31) // 32 * 32 // 4
    
    golden = np.zeros(outter_pad, dtype=dtype)
    
    if scenarioNum == 1:
        # 场景1：Sum - 求和
        Sum(x, n, golden)
        print("Generating golden data for Sum scenario (scenarioNum=1)")
    else:
        # 场景2：Mean - 求均值
        scalar = dtype(1 / n)
        Mean(x, n, golden, scalar)
        print("Generating golden data for Mean scenario (scenarioNum=2)")
    
    # 保存二进制文件
    os.makedirs("input", exist_ok=True)
    x.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")
    
    print(f"Input shape: {x.shape}")
    print(f"Output shape: {golden.shape}")
    print(f"Golden data: {golden[:outter]}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="生成 Sum/Mean 样例的测试数据")
    parser.add_argument("-scenarioNum", type=int, default=1, choices=[1, 2],
                        help="场景编号: 1=Sum求和, 2=Mean求均值 (默认: 1)")
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
