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


def Mean(x, n, insum, scalar):
    for i, _ in enumerate(x):
        res = calculateSum(x[i, :n], x.dtype)
        insum[i] = res * scalar


def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=Mean, 2=Sum)
    """
    dtype = np.float32

    if scenarioNum == 1:
        outter = 1
        inner = 32
        n = 32
        cast = 0
        x = np.random.randint(-100, 100, [outter, inner]).astype(dtype)
        if dtype == np.float16:
            outter_pad = (outter * 2 + 31) // 32 * 32 // 2
        else:
            outter_pad = (outter * 4 + 31) // 32 * 32 // 4
        if cast == 1:
            scalar = np.float32(1 / n)
            x_cast = x.astype(np.float32)
            golden = np.zeros(outter_pad, dtype=np.float32)
            Mean(x_cast, n, golden, scalar)
            golden = golden.astype(np.float16)
        else:
            scalar = dtype(1 / n)
            golden = np.zeros(outter_pad, dtype=dtype)
            Mean(x, n, golden, scalar)
    else:
        input_shape = [7, 2024]
        x = np.random.uniform(-100, 100, input_shape).astype(dtype)
        golden = np.sum(x[:, :-1], axis=1).astype(dtype)

    os.makedirs("input", exist_ok=True)
    x.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 3))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
