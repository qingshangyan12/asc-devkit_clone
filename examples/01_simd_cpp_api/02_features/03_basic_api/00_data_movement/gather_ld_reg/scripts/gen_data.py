#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import os
import argparse
import numpy as np


def gen_golden_data_gather_scenario1():
    """场景1：高维Gather（源为LocalTensor/UB地址）- 按元素收集"""
    total_length = 128
    data_type = np.float16
    x = np.random.uniform(0, 1, [1, total_length]).astype(data_type)
    offset = np.arange(total_length - 1, -1, -1).astype(np.uint32) * 2
    golden = x.flatten()[::-1]
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile('./input/input_x.bin')
    offset.tofile('./input/input_y.bin')
    golden.tofile('./output/golden.bin')


def gen_golden_data_gatherb_scenario2():
    """场景2：Reg::GatherB - 按32字节DataBlock收集"""
    total_length = 128
    data_type = np.float16
    elements_per_block = 32 // np.dtype(data_type).itemsize  # 16个half per DataBlock
    num_blocks = total_length // elements_per_block  # 8个DataBlock
    # 源数据：128个half元素（8个DataBlock）
    x = np.random.uniform(0, 1, [1, total_length]).astype(data_type)
    # index: 128个uint32_t元素，前num_blocks个为DataBlock的byte偏移（32B对齐）
    # GatherB的index RegTensor<uint32_t>有64个元素，
    # index[i]对应dstReg中第i个DataBlock在baseAddr处的byte偏移
    index = np.zeros(total_length, dtype=np.uint32)
    for i in range(num_blocks):
        index[i] = i * 32  # byte偏移，32B对齐
    # golden: 恒等映射（index[i]=i*32，即第i个DataBlock取自baseAddr+i*32处）
    golden = x.copy()
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    x.tofile('./input/input_x.bin')
    index.tofile('./input/input_y.bin')
    golden.tofile('./output/golden.bin')


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1)
    args = parser.parse_args()
    if args.scenarioNum == 1:
        gen_golden_data_gather_scenario1()
    elif args.scenarioNum == 2:
        gen_golden_data_gatherb_scenario2()