#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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
    output = np.fromfile(output_path, dtype=np.uint8)
    golden = np.fromfile(golden_path, dtype=np.uint8)
    if output.size != golden.size:
        print("byte count mismatch: expected %d, actual %d" % (golden.size, output.size))
        return False

    mismatch_indices = np.flatnonzero(output != golden)
    for index in mismatch_indices[:20]:
        print(
            "byte index: %04d, expected: 0x%02x, actual: 0x%02x"
            % (index, golden[index], output[index])
        )
    return mismatch_indices.size == 0


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage: verify_result.py <output> <golden>")
        sys.exit(1)
    if not verify_result(sys.argv[1], sys.argv[2]):
        print("[ERROR] result mismatch")
        sys.exit(1)
    print("test pass!")
