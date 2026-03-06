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


def gen_golden_data_simple(scenarioNum):
    if scenarioNum >= 1 and scenarioNum <= 3:
        count = 128
        input_type = np.dtype("float16")
        input_x = np.arange(0, count, 1, np.float16)
        input_y = np.arange(0, count * input_type.itemsize, input_type.itemsize, np.uint32)
        input_y = np.flipud(input_y)
        golden = np.flipud(input_x)
    elif scenarioNum == 4:
        count = 1024 * 16
        src_base_addr = 0
        input_type = np.dtype("float16")
        input_x = np.arange(0, count, 1, np.float16)
        input_y = np.arange(0, count * input_type.itemsize, input_type.itemsize, np.uint32)
        input_y = np.flipud(input_y)
        golden = np.zeros(count, dtype=input_type)
        for i in range(count):
            golden[i] = input_x[(src_base_addr + input_y[i]) // input_type.itemsize]

    input_x = np.arange(128).astype(np.uint16)
    input_y = np.arange(8)[::-1] * 32
    input_y = input_y.astype(np.uint32)
    data_size = 2
    golden = np.zeros(128).astype(np.uint16)
    count = 0
    for i in range(8):
        for j in range(int(32/data_size)):
            golden[count] = input_x[int(input_y[i] / data_size +j)]
            count += 1
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()
