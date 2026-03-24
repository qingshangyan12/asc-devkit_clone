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
np.random.seed(9)


def gen_golden_data(scenarioNum=1):
    """
    根据场景编号生成输入数据和Golden数据
    场景1：[1, 512]数据搬运，mask连续模式
    场景2：从[18, 64]搬运[18, 8]数据，mask连续模式，源和目的空间不同
    场景3：[1, 1024]数据搬运，counter模式
    场景4：从[18, 64]搬运[18, 8]数据，counter模式，源和目的空间不同
    """
    if scenarioNum == 1:
        # 场景1：[1, 512]数据搬运，mask连续模式
        src_shape = [1, 512]
        dst_shape = [1, 512]
        src_length = 512
        dst_length = 512
    elif scenarioNum == 2:
        # 场景2：从[18, 64]搬运[18, 8]数据，mask连续模式
        src_shape = [18, 64]
        dst_shape = [18, 8]
        src_length = 18 * 64
        dst_length = 18 * 8
    elif scenarioNum == 3:
        # 场景3：[1, 1024]数据搬运，counter模式
        src_shape = [1, 1024]
        dst_shape = [1, 1024]
        src_length = 1024
        dst_length = 1024
    elif scenarioNum == 4:
        # 场景4：从[18, 64]搬运[18, 8]数据，counter模式
        src_shape = [18, 64]
        dst_shape = [18, 8]
        src_length = 18 * 64
        dst_length = 18 * 8
    else:
        # 默认场景1
        src_shape = [1, 512]
        dst_shape = [1, 512]
        src_length = 512
        dst_length = 512

    # 生成输入数据 (int32类型)
    input_data = np.random.randint(-1000, 1000, size=src_length).astype(np.int32)

    # 生成Golden数据
    if src_length != dst_length:
        # 源和目的空间不同：从每行取前dst_cols个元素
        src_rows = src_shape[0]
        src_cols = src_shape[1]
        dst_cols = dst_shape[1]
        input_2d = input_data.reshape(src_rows, src_cols)
        golden = input_2d[:, :dst_cols].flatten()
    else:
        # 源和目的空间相同：完整拷贝
        golden = input_data.copy()

    # 创建目录并保存数据
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_data.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
