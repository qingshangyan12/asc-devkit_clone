#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
"""

import os
import numpy as np

def gen_golden_data():
    total_length = 520
    data_type = np.float32
    # LoadAlignStoreAlign: 从UB搬入Reg再搬回UB，golden数据应与输入一致
    input_data = np.random.randn(1, total_length).astype(data_type)
    golden = input_data.copy()

    os.makedirs("./output", exist_ok=True)
    input_data.tofile("./output/input.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data()
