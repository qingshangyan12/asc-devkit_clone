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


def gen_golden_data_simple():
    elementCount = 128
    dtype = np.float32
    srcValue = np.arange(0, elementCount, 1, np.float32)
    np.random.shuffle(srcValue)

    srcIndex = np.arange(0, elementCount, 1, np.uint32)

    sortedIndexGolden = np.argsort(srcValue)[::-1].astype(np.uint32)
    sortedValueGolden = srcValue[sortedIndexGolden]
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    srcValue.tofile("./input/srcValue.bin")
    srcIndex.tofile("./input/srcIndex.bin")
    
    sortedValueGolden.tofile("./output/golden_output_dstValue.bin")
    sortedIndexGolden.tofile("./output/golden_output_dstIndex.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
