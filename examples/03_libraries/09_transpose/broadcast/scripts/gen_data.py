#!/usr/bin/python3
# coding=utf-8

# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

import os
import numpy as np
import argparse


def gen_golden_data_simple(testcase):
    x_shape = (1, 48)
    y_shape = (96, 48)
    if testcase == 1:
        x_shape = (96, 1)
        y_shape = (96, 96)
    x = np.random.uniform(-10, 10, x_shape).astype(np.float32)
    y = np.broadcast_to(x, y_shape).astype(np.float32)
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    x.tofile("./input/input.bin")
    y.tofile("./output/golden.bin")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--testcase', type=int, help='testcase,目前只有case 0 与 1')
    args = parser.parse_args()
    testcase = 0
    if args.testcase:
        testcase = args.testcase
    gen_golden_data_simple(testcase)
