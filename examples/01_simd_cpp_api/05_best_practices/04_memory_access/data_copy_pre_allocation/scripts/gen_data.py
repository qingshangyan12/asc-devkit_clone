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


TOTAL_LENGTH = 8192


def gen_golden_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # 样例只做GM->UB->GM的往返搬运，不修改数据内容，因此golden与输入一致。
    rng = np.random.default_rng(0)
    input_data = rng.integers(-2, 3, size=TOTAL_LENGTH, dtype=np.int16).astype(np.float16)
    input_data.tofile("./input/input.bin")
    input_data.tofile("./output/golden.bin")
    print(f"[INFO] Generated sample with length {TOTAL_LENGTH} (half)")


if __name__ == "__main__":
    gen_golden_data()
