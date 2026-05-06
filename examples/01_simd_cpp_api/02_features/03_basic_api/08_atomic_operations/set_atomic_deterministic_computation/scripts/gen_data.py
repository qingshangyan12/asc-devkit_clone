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
import argparse


def gen_golden_data_aic(): 
    input_type = np.dtype("float32")
    output_type = input_type
    sync_type = np.dtype("int32")
    M = 16
    N = 32
    K = 16

    os.system("mkdir -p input")
    os.system("mkdir -p output")

    x = 1.0
    y = 1e16
    z = -1e16

    # 第一对: golden = 0
    x1_gm = np.zeros([M, K], dtype=input_type)
    x2_gm = np.zeros([K, N], dtype=input_type)
    golden = np.matmul(x1_gm, x2_gm).astype(output_type)
    x1_gm.tofile("./input/x1_gm.bin")
    x2_gm.tofile("./input/x2_gm.bin")

    # 第二对: golden = 1.0
    x3_gm = np.ones([M, K], dtype=input_type)
    x4_gm = np.full([K, N], x / K, dtype=input_type)
    golden = np.matmul(x3_gm, x4_gm).astype(output_type)
    x3_gm.tofile("./input/x3_gm.bin")
    x4_gm.tofile("./input/x4_gm.bin")

    # 第三对: golden = 1e16
    x5_gm = np.ones([M, K], dtype=input_type)
    x6_gm = np.full([K, N], y / K, dtype=input_type)
    golden2 = np.matmul(x5_gm, x6_gm).astype(output_type)
    x5_gm.tofile("./input/x5_gm.bin")
    x6_gm.tofile("./input/x6_gm.bin")


    # 第四对: golden = -1e16
    x7_gm = np.ones([M, K], dtype=input_type)
    x8_gm = np.full([K, N], z / K, dtype=input_type)
    golden3 = np.matmul(x7_gm, x8_gm).astype(output_type)
    x7_gm.tofile("./input/x7_gm.bin")
    x8_gm.tofile("./input/x8_gm.bin")

    # 证明浮点数累加不具备交换律
    right = x + (y + z)

    # 预期输出数据
    golden = np.array([right] * M * N, dtype=output_type)
    golden.tofile("./output/golden.bin")

    # 用于核间同步，在多个核之间共享的gm，gm必须初始为0
    input_sync = np.zeros(256, dtype=sync_type)
    input_sync.tofile("./input/input_sync.bin")


def gen_golden_data_aiv():
    x = 1.0
    y = 1e16
    z = -1e16
    data_size = 8
    # 保存数据到文件
    input_type = np.dtype("float32")
    output_type = input_type
    sync_type = np.dtype("int32")

    # 用于存储输出数据的gm初始化的值为0
    input_gm = np.array([0.0] * data_size, dtype=input_type)

    # 三个累加的数据
    input_x = np.array([x] * data_size, dtype=input_type)
    input_y = np.array([y] * data_size, dtype=input_type)
    input_z = np.array([z] * data_size, dtype=input_type)

    # 证明浮点数累加不具备交换律
    left =  (input_x + input_y) + input_z
    right = input_x + (input_y + input_z)

    # 预期输出数据
    golden = np.array([right[0]+input_gm[0]] * data_size, dtype=output_type)

    # 用于核间同步，在多个核之间共享的gm，gm必须初始为0
    input_sync = np.zeros(256, dtype=sync_type)
    # 创建目录
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    # 保存数据
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    input_z.tofile("./input/input_z.bin")
    golden.tofile("./output/golden.bin")
    input_gm.tofile("./input/input_gm.bin")
    input_sync.tofile("./input/input_sync.bin")

def gen_golden_data(scenario_num):
    if scenario_num == '3':
        gen_golden_data_aic()
    else:
        gen_golden_data_aiv()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate golden data for atomic operations demo')
    parser.add_argument('--scenario_num', type=str, default='1')
    args = parser.parse_args()
    gen_golden_data(args.scenario_num)
