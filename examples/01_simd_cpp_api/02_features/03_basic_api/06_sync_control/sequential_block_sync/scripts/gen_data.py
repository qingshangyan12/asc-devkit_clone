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
import numpy as np

def gen_golden_data_simple():
    input_type = np.float32
    output_type = np.float32
    work_type = np.int32
    use_core_num = 8
    block_length = 256
    gm_workspace_bytes = use_core_num * 32  # gmWorkspace最少需要blockNum * 32Bytes
    gm_workspace_elems = gm_workspace_bytes // 4  # 转换为int32_t元素个数
    
    total_input_size = use_core_num * block_length
    input_x = np.random.uniform(1.0, 10.0, total_input_size).astype(input_type)
    
    work_space = np.zeros(gm_workspace_elems).astype(work_type)
    
    golden = np.zeros(block_length, dtype=output_type)
    for i in range(block_length):
        for core_id in range(use_core_num):
            golden[i] += input_x[core_id * block_length + i]
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    work_space.tofile("./input/input_work.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()
