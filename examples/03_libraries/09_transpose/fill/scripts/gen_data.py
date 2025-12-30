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


SIZE = 256

def gen_golden_data_simple():
    x = np.random.uniform(-60000, 60000, [SIZE]).astype(np.float32)
    y = np.random.uniform(-60000, 60000, [SIZE]).astype(np.float32)
    z = x + y

    os.system("mkdir -p ./input")
    x.tofile("./input/input_x.bin")
    y.tofile("./input/input_y.bin")

    os.system("mkdir -p ./output")
    z.tofile("./output/golden_z.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
