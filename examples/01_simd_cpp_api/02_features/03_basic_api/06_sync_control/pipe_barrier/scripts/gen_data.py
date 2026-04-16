#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You can not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import numpy as np


def gen_golden_data_simple():
    """
    生成输入数据和真值数据
    
    输入数据：
    - src0: [1.0, 1.0, ...] 256个元素
    - src1: [1.0, 1.0, ...] 256个元素  
    - src2: [-3.0, -3.0, ...] 256个元素（用于地址重叠测试，不参与计算）
    
    真值数据（合并为一个输出文件）：
    - golden[0:256]: dst = src0 + src1 = [2.0, 2.0, ...]
    - golden[256:512]: dst2 = dst * src0 = (src0 + src1) * src0 = [2.0, 2.0, ...]
    """
    input_type = np.dtype("float32")
    output_type = input_type
    total_length = 256
    
    # 生成输入数据
    input_src0 = np.ones(total_length, dtype=input_type)  # [1.0, 1.0, ...]
    input_src1 = np.ones(total_length, dtype=input_type)  # [1.0, 1.0, ...]
    input_src2 = np.full(total_length, -3.0, dtype=input_type)  # [-3.0, -3.0, ...]
    
    # 计算真值
    # dst = src0 + src1
    golden_dst = input_src0 + input_src1  # [2.0, 2.0, ...]
    
    # dst2 = dst * src0 = (src0 + src1) * src0
    golden_dst2 = golden_dst * input_src0  # [2.0, 2.0, ...]
    
    # 合并输出：dst + dst2
    golden = np.concatenate([golden_dst, golden_dst2])
    
    # 创建目录并保存数据
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    input_src0.tofile("./input/input_src0.bin")
    input_src1.tofile("./input/input_src1.bin")
    input_src2.tofile("./input/input_src2.bin")
    
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()