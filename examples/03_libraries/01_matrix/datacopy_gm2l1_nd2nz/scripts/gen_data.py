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

MATRIX_ROW = 32
MATRIX_COL = 32
C0_SIZE = 32  # 一个小分形每行的字节数
FRACTAL_ROWS = 16 # 一个大小分形的行数

def gen_golden_data(scenarioNum=1):
    # 根据scenarioNum选择数据类型
    if scenarioNum == 1 or scenarioNum == 4:
        input_type = np.dtype("float16")
    elif scenarioNum == 2 or scenarioNum == 5:
        input_type = np.dtype("float32")
    elif scenarioNum == 3 or scenarioNum == 6:
        input_type = np.dtype("int8")
    
    output_type = input_type

    # 根据scenarioNum生成输入数据
    input_x = np.arange(1, MATRIX_ROW * MATRIX_COL + 1).reshape(MATRIX_ROW, MATRIX_COL)
    input_x = input_x.astype(input_type)
    if scenarioNum == 4 or scenarioNum == 5 or scenarioNum == 6:
        # 每行最后两列设为0
        input_x[:, -2:] = 0

    # 计算ND2NZ转换参数
    sizeof_T = input_type.itemsize
    elements_per_C0_SIZE = C0_SIZE // sizeof_T
    fractal_row_count = MATRIX_ROW // FRACTAL_ROWS
    fractal_col_count = MATRIX_COL // elements_per_C0_SIZE

    print(f"Data type: {input_type}, sizeof(T): {sizeof_T}")
    print(f"fractal_row_count: {fractal_row_count}, fractal_col_count: {fractal_col_count}")

    # 按照ND2NZ布局生成golden数据
    golden = []
    for col in range(fractal_col_count):
        for row in range(fractal_row_count):
            col_start = col * elements_per_C0_SIZE
            col_end = (col + 1) * elements_per_C0_SIZE
            row_start = row * FRACTAL_ROWS
            row_end = (row + 1) * FRACTAL_ROWS
            tmp_mat = input_x[row_start:row_end, col_start:col_end]
            golden += tmp_mat.flatten().tolist()

    golden = np.array(golden, dtype=output_type)
    
    print(input_x)
    print(golden)
    # 创建目录
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    # 保存数据
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 7))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
