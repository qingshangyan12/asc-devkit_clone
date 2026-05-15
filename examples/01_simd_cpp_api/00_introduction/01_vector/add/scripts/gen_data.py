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
        scenarioNum: 场景编号(1=PipeBarrier同步,2=SetFlag/WaitFlag同步,3=Tile切分单缓冲,4=Tile切分双缓冲)
    """
    if scenarioNum == 1 or scenarioNum == 2:
        # 场景1和场景2：shape=[8, 2048]
        input_x = np.random.uniform(-10, 10, [8, 2048]).astype(np.float32)
        input_y = np.random.uniform(-10, 10, [8, 2048]).astype(np.float32)
        golden = input_x + input_y
    elif scenarioNum == 3 or scenarioNum == 4:
        # 场景3和场景4：shape=[2048, 2048]
        input_x = np.random.uniform(-10, 10, [2048, 2048]).astype(np.float32)
        input_y = np.random.uniform(-10, 10, [2048, 2048]).astype(np.float32)
        golden = input_x + input_y
    
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5),
                        help='scenario number (1-4)')
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)