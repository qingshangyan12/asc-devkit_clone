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
import argparse
import numpy as np
np.random.seed(9)


def gen_golden_data(scenarioNum=1):
    """
    根据场景编号生成输入数据和Golden数据
    场景1：输入[1, 20]，输出[1, 32]，使用SetPadValue填充
    场景2：输入[32, 59]，输出[32, 64]，rightPadding，不使用SetPadValue
    场景3：输入[3, 24]，输出[1, 80]，Compact模式
    """
    if scenarioNum == 1:
        src_rows = 1
        src_cols = 20
        dst_cols = 32
        use_setpadvalue = True
    elif scenarioNum == 2:
        src_rows = 32
        src_cols = 59
        dst_cols = 64
        use_setpadvalue = False
    elif scenarioNum == 3:
        src_rows = 3
        src_cols = 24
        dst_cols = 80
        use_setpadvalue = False
    else:
        src_rows = 1
        src_cols = 20
        dst_cols = 32
        use_setpadvalue = True
    
    input_x = np.random.uniform(-10, 10, [src_rows, src_cols]).astype(np.float16)
    golden = np.zeros([1, dst_cols], dtype=np.float16)
    
    if scenarioNum == 1 or scenarioNum == 2:
        for i in range(src_rows):
            for j in range(src_cols):
                golden[0, j] = input_x[i, j] + 0
        
        if not use_setpadvalue:
            for i in range(src_rows):
                for j in range(src_cols, dst_cols):
                    golden[i, j] = 0
    elif scenarioNum == 3:
        src_total = src_rows * src_cols
        for i in range(src_total):
            golden[0, i] = input_x.flatten()[i] + 0
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=[1, 2, 3])
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
