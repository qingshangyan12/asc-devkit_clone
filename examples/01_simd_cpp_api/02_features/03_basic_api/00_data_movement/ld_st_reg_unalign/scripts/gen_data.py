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
import sys
import numpy as np

def gen_golden_data():
    total_length = 1024
    data_type = np.float32
    
    # 生成输入数据：[1, 1024] 向量
    # 索引1-128设置为1，其余为0
    input_x = np.zeros(total_length, dtype=data_type)
    input_x[1:129] = 1.0  # 索引1到128（包含1，不包含129）
    input_x = input_x.reshape(1, total_length)
    
    # 创建目录
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    # 保存输入数据
    input_x.tofile('./input/input_x.bin')
    golden = input_x.copy()
    
    # 保存golden数据
    golden.tofile('./output/golden.bin')

if __name__ == "__main__":
    gen_golden_data()
