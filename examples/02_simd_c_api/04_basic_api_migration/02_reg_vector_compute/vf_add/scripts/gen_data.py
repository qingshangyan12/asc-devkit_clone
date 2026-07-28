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


TOTAL_LENGTH = 1024


def gen_data():
    """Keep the same float32 input formula and golden logic as Basic API scenario 4."""
    indices = np.arange(TOTAL_LENGTH, dtype=np.uint32)
    x = (indices % 100).astype(np.float32) * np.float32(0.1)
    y = ((indices + 50) % 100).astype(np.float32) * np.float32(0.2)
    golden = (x + y).astype(np.float32)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile("input/x.bin")
    y.tofile("input/y.bin")
    golden.tofile("output/golden.bin")


if __name__ == "__main__":
    gen_data()
