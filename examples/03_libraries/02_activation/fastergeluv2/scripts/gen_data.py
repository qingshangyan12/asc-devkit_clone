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
import sys
import numpy as np


def gen_golden_data_simple():
    dtype = np.float32
    highPrecision = 0
    np.set_printoptions(threshold=sys.maxsize, suppress=True)
    src = np.random.uniform(-10, 10, [64]).astype(dtype)

    if (dtype == np.float16 and highPrecision == 1):
        golden = np.zeros([64]).astype(np.float32)
        absSrc = np.abs(0.7071 * src.astype(np.float32))
        srcMin = np.ones([64]).astype(np.float32) * 1.769
        srcMin = np.minimum(absSrc, srcMin)
        golden = (src.astype(np.float32) * (np.sign(src.astype(np.float32)) * (-0.1444 * np.power(srcMin - 1.769, 2) + 0.5) + 0.5)).astype(np.half)
    else:
        golden = np.zeros([64]).astype(dtype)
        absSrc = np.abs(0.7071 * src)
        srcMin = np.ones([64]).astype(dtype) * 1.769
        srcMin = np.minimum(absSrc, srcMin)
        golden = src * (np.sign(src) * (-0.1444 * np.power(srcMin - 1.769, 2) + 0.5) + 0.5)

    os.makedirs("input", exist_ok=True)
    src.tofile("./input/input_src.bin")
    os.makedirs("output", exist_ok=True)
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
