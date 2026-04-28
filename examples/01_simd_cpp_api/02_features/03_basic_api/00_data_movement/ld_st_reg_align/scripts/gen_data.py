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

def gen_golden_data(scenario_num):
    total_length = 1024
    data_type = np.float32
    
    # 生成输入数据：[1, 1024] 向量
    input_x = np.arange(total_length, dtype=data_type).reshape(1, total_length)
    
    # 创建目录
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    # 保存输入数据
    input_x.tofile('./input/input_x.bin')
    
    # 根据场景计算golden数据
    if scenario_num == 1:
        # 场景1：连续搬运，输入数据直接输出
        golden = input_x.copy()
    elif scenario_num == 2:
        # 场景2：连续搬运(postUpdate模式)，输入数据直接输出
        golden = input_x.copy()
    elif scenario_num == 3:
        # 场景3：非连续搬运，输入数据直接输出
        golden = input_x.copy()
    elif scenario_num == 4:
        # 场景4：非连续搬运(postUpdate模式)，输入数据直接输出
        golden = input_x.copy()
    elif scenario_num == 5:
        # 场景5：连续搬运(解压缩模式UNPACK搬入和压缩模式PACK搬出)
        # TODO: 需要实现DIST_UNPACK_B32和DIST_PACK_B64的数据变换逻辑
        print("场景5的golden数据生成逻辑尚未实现，请后续补充")
        golden = np.zeros_like(input_x)
    elif scenario_num == 6:
        # 场景6：连续搬运(双搬入和双搬出模式)
        # TODO: 需要实现DIST_DINTLV_B32和DIST_INTLV_B32的数据变换逻辑
        print("场景6的golden数据生成逻辑尚未实现，请后续补充")
        golden = np.zeros_like(input_x)
    else:
        print(f"不支持的场景编号: {scenario_num}")
        sys.exit(1)
    
    # 保存golden数据
    golden.tofile('./output/golden.bin')

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("使用方法: python3 gen_data.py <scenario_num>")
        print("scenario_num: 1-6")
        sys.exit(1)
    
    scenario_num = int(sys.argv[1])
    gen_golden_data(scenario_num)