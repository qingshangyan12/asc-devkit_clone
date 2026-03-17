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


import numpy as np
import os


def gen_golden_data_simple():
    scenarioNum = 0
    input_type = np.float32
    output_type = np.float32
    calCount = 4096
    bufferSize = 1024
    # aiv一次循环只处理bufferSize个数据，共计循环loopCount次
    loopCount = calCount // bufferSize
    alpha = 0.001
    scalar1 = 50
    scalar2 = 2
    scalar3 = 100

    a = np.ones(calCount//2).astype(input_type)
    b = -5 * np.ones(calCount//2).astype(input_type)
    input_src0 = np.concatenate([a,b])
    input_src1 = 10 *  np.ones(calCount).astype(input_type)

    # 对input_src0求leakRelu
    tmp2 = np.maximum(input_src0, 0)
    tmp1 = np.minimum(input_src0, 0)
    tmp1 = tmp1 * alpha
    dst0 = tmp1 + tmp2

    # 每一次循环内，input_src1搬入UB长度为bufferSize数据的前半部分，
    # 会被input_src0搬入UB长度为bufferSize数据的后半部分覆盖。

    dst1 = np.zeros(calCount).astype(input_type)


    if (scenarioNum == 0):
        for i in range(loopCount):
            # 对应GM-->UB
            tmp3 = input_src1[i * bufferSize:(i + 1) * bufferSize]
            tmp3[0:bufferSize//2] = input_src0[i * bufferSize:(i + 1) * bufferSize][bufferSize//2:]
            # UB上不做任何计算

            # 对应UB-->GM
            dst1[i * bufferSize:(i + 1) * bufferSize] = tmp3
 
    elif (scenarioNum == 1):
         for i in range(loopCount):
            # 对应GM-->UB
            tmp3 = input_src1[i * bufferSize:(i + 1) * bufferSize]
            tmp3[0:bufferSize//2] = input_src0[i * bufferSize:(i + 1) * bufferSize][bufferSize//2:]

            tmp3[0] = scalar3

            # UB上做Muls和SetValue
            tmp3 = tmp3 * scalar2
            tmp3[1] = scalar1

            # 对应UB-->GM
            dst1[i * bufferSize:(i + 1) * bufferSize] = tmp3

    # dst0的尾部长度为bufferSize的区间与dst1的头部长度为bufferSize的区间重叠，重叠部分保留dst0的值
    golden = np.zeros(calCount + bufferSize // 2).astype(output_type)
    golden[bufferSize // 2:] = dst1
    golden[0:calCount] = dst0
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_src0.tofile("./input/input_src0.bin")
    input_src1.tofile("./input/input_src1.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    gen_golden_data_simple()
