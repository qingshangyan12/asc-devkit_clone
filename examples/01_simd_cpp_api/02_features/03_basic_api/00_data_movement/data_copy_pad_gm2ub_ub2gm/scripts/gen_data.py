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
    场景1：输入[1, 20]，输出[1, 32]，使用SetPadValue填充
    场景2：输入[32, 59]，输出[32, 64]，rightPadding，不使用SetPadValue
    场景3：输入[3, 24]，输出[1, 80]，Compact模式
    场景4：输入[384]，输出[512]，SetLoopModePara loop mode（Compact模式）
    场景5：输入[384]，输出[576]，SetLoopModePara loop mode（Normal模式）
    """
    
    if scenarioNum == 1:
        src_rows = 1
        src_cols = 20
        dst_cols = 32
        use_setpadvalue = True
        data_type = np.float16
    elif scenarioNum == 2:
        src_rows = 32
        src_cols = 59
        dst_cols = 64
        use_setpadvalue = False
        data_type = np.float32
    elif scenarioNum == 3:
        src_rows = 3
        src_cols = 24
        dst_cols = 80
        use_setpadvalue = False
        data_type = np.float16
    elif scenarioNum == 4:
        src_rows = 1
        src_cols = 320
        dst_cols = 576  # Compact模式：320B数据 + 64B填充 + 192B随机值
        use_setpadvalue = False
        data_type = np.int8
    
    elif scenarioNum == 5:
        src_rows = 1
        src_cols = 320
        dst_cols = 576  # Normal模式：320B数据 + 192B填充 + 32B外层循环间隔 + 32B未使用
        use_setpadvalue = False
        data_type = np.int8
    elif scenarioNum == 5:
        src_rows = 1
        src_cols = 384
        dst_cols = 576  # 8个block，每个64B（48数据+16填充），外层循环间隔32B随机值
        use_setpadvalue = False
        data_type = np.int8
    
    input_x = np.random.uniform(-10, 10, [src_rows, src_cols]).astype(data_type)

    if scenarioNum == 1 or scenarioNum == 2:
        golden = np.zeros([src_rows, dst_cols], dtype=data_type)
        for i in range(src_rows):
            for j in range(src_cols):
                golden[i, j] = input_x[i, j]
        
        if use_setpadvalue:
            for i in range(src_rows):
                for j in range(src_cols, dst_cols):
                    golden[i, j] = 1
    
    elif scenarioNum == 3:
        golden = np.zeros([dst_cols], dtype=data_type)
        input_flatten = input_x.flatten()
        golden[:72] = input_flatten
        golden = golden.reshape(1, 80)
    
    elif scenarioNum == 4:
        # 场景4：Compact模式，每次内层循环搬运80B后填充16B使其96B对齐
        # 先清零UB buffer，确保间隔区域值为0，padding值设为-1
        golden = np.zeros([dst_cols], dtype=data_type)
        input_flatten = input_x.flatten()
        # 外层循环1（0-288）
        golden[0:80] = input_flatten[0:80]        # 内层循环1数据
        golden[80:96] = -1                         # 内层循环1填充
        golden[128:208] = input_flatten[80:160]   # 内层循环2数据
        golden[208:224] = -1                       # 内层循环2填充
        # 外层循环2（288-576）
        golden[288:368] = input_flatten[160:240]  # 内层循环1数据
        golden[368:384] = -1                       # 内层循环1填充
        golden[416:496] = input_flatten[240:320]  # 内层循环2数据
        golden[496:512] = -1                       # 内层循环2填充
        golden = golden.reshape(1, dst_cols)
    
    elif scenarioNum == 5:
        # 场景5：Normal模式，每个block搬运后填充24B使其64B对齐
        # 先清零UB buffer，确保间隔区域值为0，padding值设为-1
        golden = np.zeros([dst_cols], dtype=data_type)
        input_flatten = input_x.flatten()
        # 外层循环1（0-256）
        golden[0:40] = input_flatten[0:40]
        golden[40:64] = -1
        golden[64:104] = input_flatten[40:80]
        golden[104:128] = -1
        golden[128:168] = input_flatten[80:120]
        golden[168:192] = -1
        golden[192:232] = input_flatten[120:160]
        golden[232:256] = -1
        # 外层循环2（288-544）
        golden[288:328] = input_flatten[160:200]
        golden[328:352] = -1
        golden[352:392] = input_flatten[200:240]
        golden[392:416] = -1
        golden[416:456] = input_flatten[240:280]
        golden[456:480] = -1
        golden[480:520] = input_flatten[280:320]
        golden[520:544] = -1
        golden = golden.reshape(1, dst_cols)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=[1, 2, 3, 4, 5])
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
