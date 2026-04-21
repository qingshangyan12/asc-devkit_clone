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
    """
    生成DataSyncBarrier样例的输入数据和Golden数据
    
    样例场景：双核同步场景
    - 核0：先写入GM[1]=7，DataSyncBarrier同步，再写入GM[0]=6
    - 核1：轮询GM[0]直到=6，读取GM[1]=7，输出GM[0]=2*7=14
    
    输入src: [8]个int32，初始全为1
    输出dst: [8]个int32，golden[0]=14，其余为0
    """
    data_type = np.int32
    cal_count = 8
    value = 6
    
    input_src = np.ones(cal_count, dtype=data_type)
    golden = np.zeros(cal_count, dtype=data_type)
    golden[0] = 2 * (value + 1)
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_src.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data()