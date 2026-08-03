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

import argparse
import os

import numpy as np


BLOCK_NUM = 8
TILE_LENGTH = 16 * 1024


def gen_golden_data(scenario_num):
    # DataCopyPad copies Vector2 first so an early MTE3 start is directly observable.
    vector2_value = 32 if scenario_num in (3, 4) else 2
    vector2 = np.full(TILE_LENGTH, vector2_value, dtype=np.float16)
    vector1 = np.full(TILE_LENGTH, 1, dtype=np.float16)
    golden = np.tile(np.concatenate((vector2, vector1)), BLOCK_NUM)
    os.makedirs("output", exist_ok=True)
    golden.tofile("output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, choices=range(5), required=True)
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
