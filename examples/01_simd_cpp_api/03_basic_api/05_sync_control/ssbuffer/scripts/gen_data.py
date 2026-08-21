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
import argparse
import numpy as np

EXPECTED_VALUE = np.array(
    [
        0x1122334455667788,
        0x8877665544332211,
        0x0123456789ABCDEF,
        0xFEDCBA9876543210,
    ],
    dtype=np.uint64,
)


def gen_golden_data_scenario0():
    """
    SCENARIO_NUM=0: 基址验证场景。
    该场景无输入/输出数据文件，仅需检查运行日志中GetSsbufBaseAddr的输出。
    """
    os.makedirs("output", exist_ok=True)


def gen_golden_data_scenario1():
    """
    SCENARIO_NUM=1: AIV->AIC消息验证场景。
    output共8个uint64_t：
      output[0..3]为AIC从SSBuffer读取的消息（AIV写入的值）；
      output[4..7]为AIV侧回写GM的消息。
    两段均等于EXPECTED_VALUE。
    """
    golden = np.concatenate([EXPECTED_VALUE, EXPECTED_VALUE])
    os.makedirs("output", exist_ok=True)
    golden.astype(np.uint64).tofile("./output/golden.bin")


def gen_golden_data(scenario_num):
    if scenario_num == 0:
        gen_golden_data_scenario0()
    elif scenario_num == 1:
        gen_golden_data_scenario1()
    else:
        raise ValueError(f"Invalid scenario_num: {scenario_num}, must be 0 or 1")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-scenarioNum",
        type=int,
        default=1,
        choices=[0, 1],
        help="Scenario number: 0 or 1",
    )
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)