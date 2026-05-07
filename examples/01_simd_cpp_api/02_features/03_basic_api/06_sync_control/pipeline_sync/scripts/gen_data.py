#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You can not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import argparse
import numpy as np

TILE_LENGTH = 256
TILE_NUM = 3


def gen_golden_data_vector():
    """
    场景1：Vector模式
    输入：src0、src1 各768个float
    输出：output = (src0 + src1) * src0
    """
    total_length = TILE_LENGTH * TILE_NUM
    input_type = np.dtype("float32")
    input_src0 = np.random.uniform(1.0, 10.0, total_length).astype(input_type)
    input_src1 = np.random.uniform(1.0, 10.0, total_length).astype(input_type)

    golden = (input_src0 + input_src1) * input_src0

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    input_src0.tofile("./input/input_src0.bin")
    input_src1.tofile("./input/input_src1.bin")
    golden.tofile("./output/golden.bin")


def gen_golden_data_cube():
    """
    场景2：Cube模式
    输入：A矩阵(128x512 half)，B矩阵(512x128 half)
    输出：C矩阵(128x128 float)
    """
    M = 128
    K = 512
    N = 128

    input_src0 = np.random.uniform(1.0, 5.0, (M, K)).astype(np.float16)
    input_src1 = np.random.uniform(1.0, 5.0, (K, N)).astype(np.float16)

    golden = np.matmul(input_src0.astype(np.float32), input_src1.astype(np.float32))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    input_src0.tofile("./input/input_src0.bin")
    input_src1.tofile("./input/input_src1.bin")
    golden.tofile("./output/golden.bin")


def gen_golden_data(scenario_num=1):
    if scenario_num == 1:
        gen_golden_data_vector()
    else:
        gen_golden_data_cube()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=[1, 2])
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
