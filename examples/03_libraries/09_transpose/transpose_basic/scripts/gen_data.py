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


def gen_golden_data_2d():
    """场景1: 简单2D矩阵转置 (16x16)"""
    dtype = np.float16
    shape = (16, 16)

    input_data = np.random.uniform(-100.0, 100.0, shape).astype(dtype)
    golden = input_data.T  # 矩阵转置

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated 2D transpose data: input shape {shape}, output shape {golden.shape}")


def gen_golden_data_nchw2nhwc():
    """场景2: 4D转置 NCHW -> NHWC"""
    dtype = np.float16
    # [N, C, H, W] = [3, 3, 2, 8]
    n, c, h, w = 3, 3, 2, 8
    shape = (n, c, h, w)

    input_data = np.random.uniform(-100.0, 100.0, shape).astype(dtype)
    # NCHW -> NHWC: [N, C, H, W] -> [N, H, W, C]
    golden = np.transpose(input_data, (0, 2, 3, 1))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated NCHW2NHWC data: input shape {shape}, output shape {golden.shape}")


def gen_golden_data_nhwc2nchw():
    """场景3: 4D转置 NHWC -> NCHW"""
    dtype = np.float16
    # [N, H, W, C] = [3, 2, 8, 3]
    n, h, w, c = 3, 2, 8, 3
    shape = (n, h, w, c)

    input_data = np.random.uniform(-100.0, 100.0, shape).astype(dtype)
    # NHWC -> NCHW: [N, H, W, C] -> [N, C, H, W]
    golden = np.transpose(input_data, (0, 3, 1, 2))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")
    print(f"Generated NHWC2NCHW data: input shape {shape}, output shape {golden.shape}")


def gen_golden_data(scenarioNum, size):
    """根据场景生成测试数据"""
    if scenarioNum == 1:
        gen_golden_data_2d()
    elif scenarioNum == 2:
        gen_golden_data_nchw2nhwc()
    elif scenarioNum == 3:
        gen_golden_data_nhwc2nchw()
    else:
        raise ValueError(f"Unsupported scenario: {scenarioNum}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 4),
                        help='Scenario number: 1=2D, 2=NCHW2NHWC, 3=NHWC2NCHW')
    parser.add_argument('-size', type=int, default=256, help='Data size (not used for 4D scenarios)')
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, args.size)
