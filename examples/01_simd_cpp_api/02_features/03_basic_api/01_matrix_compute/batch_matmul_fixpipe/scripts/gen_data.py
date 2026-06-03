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


def gen_case_data(case_name, batch, m_size, k_size, n_size, golden_dtype):
    x1_gm = np.zeros((batch, m_size, k_size), dtype=np.float16)
    x2_gm = np.zeros((batch, k_size, n_size), dtype=np.float16)

    row = np.arange(m_size, dtype=np.float32).reshape(m_size, 1)
    col = np.arange(k_size, dtype=np.float32).reshape(1, k_size)
    for batch_index in range(batch):
        x1_gm[batch_index] = ((batch_index + 1) * 8.0 + row * 0.25 + col * 0.03125).astype(np.float16)
        x2_gm[batch_index] = np.eye(k_size, n_size, dtype=np.float16)

    golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(golden_dtype)

    input_dir = os.path.join("input", case_name)
    output_dir = os.path.join("output", case_name)
    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)
    x1_gm.tofile(os.path.join(input_dir, "x1_gm.bin"))
    x2_gm.tofile(os.path.join(input_dir, "x2_gm.bin"))
    golden.tofile(os.path.join(output_dir, "golden.bin"))


def gen_golden_data():
    gen_case_data("case1", 3, 56, 80, 64, np.float32)
    gen_case_data("case2", 3, 64, 64, 64, np.float16)


if __name__ == "__main__":
    gen_golden_data()
