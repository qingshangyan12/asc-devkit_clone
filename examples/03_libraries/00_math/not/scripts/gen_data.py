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
import sys
import numpy as np
import argparse


def gen_golden_data_simple(scenarioNum, size):
    dtype = np.int16
    src_shape = [size]
    dst_shape = [size]

    np.set_printoptions(threshold=sys.maxsize, suppress=False)

    src = np.random.uniform(-1000, 1000, src_shape).astype(dtype)
    golden = np.zeros(dst_shape).astype(dtype)

    if scenarioNum == 0:
        golden = ~src
    elif scenarioNum == 1:
        for i in range(size):
            if i % 2 == 0:
                golden[i] = ~src[i]
    elif scenarioNum == 2:
        for i in range(size):
            if i % 128 < 64:
                golden[i] = ~src[i]

    os.makedirs("input", exist_ok=True)
    src.tofile("./input/input_src.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int)
    parser.add_argument('-size', type=int)
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum, args.size)
