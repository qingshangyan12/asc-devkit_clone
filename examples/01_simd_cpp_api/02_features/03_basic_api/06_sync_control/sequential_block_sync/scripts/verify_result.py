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
import sys
import numpy as np


LOSS = 1e-3
MINIMUM = 10e-10


def verify_result(output, golden):
    output_type = np.float32
    output = np.fromfile(output, dtype=output_type)
    golden = np.fromfile(golden, dtype=output_type)
    result = np.abs(output - golden)
    deno = np.maximum(np.abs(output), np.abs(golden))
    result_atol = np.less_equal(result, LOSS)
    result_rtol = np.less_equal(result / np.add(deno, MINIMUM), LOSS)
    if not result_rtol.all() and not result_atol.all():
        if np.sum(result_rtol == False) > output.size * LOSS \
            and np.sum(result_atol == False) > output.size * LOSS:
            print("[ERROR] result error")
            return False
    print("test pass")
    return True

if __name__ == '__main__':
    verify_result(sys.argv[1],sys.argv[2])
