#!/usr/bin/env python3
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


ELEMENT_COUNT = 64
INITIAL_VALUE = np.float32(3.25)
EXPECTED_INCREMENT = np.float32(14.0)


def generate_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    np.asarray([INITIAL_VALUE], dtype=np.float32).tofile("input/input.bin")
    np.full(ELEMENT_COUNT, INITIAL_VALUE + EXPECTED_INCREMENT, dtype=np.float32).tofile("output/golden.bin")


if __name__ == "__main__":
    generate_data()
