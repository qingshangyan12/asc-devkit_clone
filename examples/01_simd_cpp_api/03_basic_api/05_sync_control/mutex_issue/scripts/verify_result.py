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

import sys

import numpy as np


def verify_result(output_path, golden_path):
    output = np.fromfile(output_path, dtype=np.float16)
    golden = np.fromfile(golden_path, dtype=np.float16)
    if output.size != golden.size:
        print(f"element count mismatch: expected {golden.size}, actual {output.size}")
        return False

    different_indexes = np.flatnonzero(output != golden)
    for index in different_indexes[:20]:
        print(f"data index: {index:06d}, expected: {golden[index]:.1f}, actual: {output[index]:.1f}")
    print(f"different elements: {different_indexes.size}/{golden.size}")
    return different_indexes.size == 0


if __name__ == "__main__":
    try:
        if not verify_result(sys.argv[1], sys.argv[2]):
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as error:
        print(error)
        sys.exit(1)
