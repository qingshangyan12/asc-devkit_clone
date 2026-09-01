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


def gen_golden_data():
    total_length = 1024
    # normal值除法，结果在float32中无法精确表示，需舍入
    # DIFF_COMPENSATION（0ulp）输出应与IEEE 754正确舍入结果完全一致
    x_small = np.array([1.0, 1.0, 2.0, 1.0, 3.0, 5.0, 1.0, 7.0, 1.0, 11.0], dtype=np.float64)
    y_small = np.array([3.0, 7.0, 3.0, 10.0, 7.0, 7.0, 13.0, 11.0, 17.0, 13.0], dtype=np.float64)
    reps = int(np.ceil(total_length / len(x_small)))
    x = (np.tile(x_small, reps)[:total_length]).astype(np.float32)
    y = (np.tile(y_small, reps)[:total_length]).astype(np.float32)

    golden = (x.astype(np.float64) / y.astype(np.float64)).astype(np.float32)
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile("./input/input_x.bin")
    y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()
