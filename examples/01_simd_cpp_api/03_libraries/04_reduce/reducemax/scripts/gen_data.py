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
import argparse
import numpy as np


def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=ReduceMax,2=ReduceMin,3=ReduceMean,4=ReduceSum)
    """
    np.random.seed(0)

    # 所有场景统一使用 [136, 32] 数据，Pattern=RA
    # 输入shape: [32, 136] (Pattern=RA: rLength=32, aLength=136)
    dtype = np.float32
    x_gm = np.random.uniform(-10, 10, [32, 136]).astype(dtype)

    # 根据场景选择不同的reduce操作 (axis=0, 在32维度上reduce，输出136)
    if scenarioNum == 1:
        # ReduceMax
        y_gm = np.max(x_gm, axis=0).astype(dtype)
    elif scenarioNum == 2:
        # ReduceMin
        y_gm = np.min(x_gm, axis=0).astype(dtype)
    elif scenarioNum == 3:
        # ReduceMean
        y_gm = np.mean(x_gm, axis=0).astype(dtype)
    elif scenarioNum == 4:
        # ReduceSum
        y_gm = np.sum(x_gm, axis=0).astype(dtype)
    else:
        raise ValueError(f"Unsupported scenarioNum: {scenarioNum}")

    # 保存数据
    os.makedirs("input", exist_ok=True)
    x_gm.tofile("./input/input_x.bin")
    os.makedirs("output", exist_ok=True)
    y_gm.tofile("./output/golden.bin")

if __name__ == "__main__":
    
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5),
                        help='场景编号：1=ReduceMax, 2=ReduceMin, 3=ReduceMean, 4=ReduceSum')
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
