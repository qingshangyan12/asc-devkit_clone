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

import sys

import numpy as np


RELATIVE_TOLERANCE = 1e-6
ABSOLUTE_TOLERANCE = 1e-6


def verify_result(output_path, golden_path):
    output = np.fromfile(output_path, dtype=np.float32)
    golden = np.fromfile(golden_path, dtype=np.float32)

    if output.shape != golden.shape:
        print(f"shape mismatch: output {output.shape}, golden {golden.shape}")
        return False

    matches = np.isclose(
        output,
        golden,
        rtol=RELATIVE_TOLERANCE,
        atol=ABSOLUTE_TOLERANCE,
        equal_nan=True,
    )
    mismatch_indexes = np.flatnonzero(~matches)
    for index in mismatch_indexes[:10]:
        print(
            f"index {index:06d}: expected {golden[index]:.9f}, "
            f"actual {output[index]:.9f}"
        )

    if mismatch_indexes.size != 0:
        print(f"test failed: {mismatch_indexes.size}/{golden.size} elements differ")
        return False

    print("test pass!")
    return True


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} OUTPUT GOLDEN")
        sys.exit(2)
    sys.exit(0 if verify_result(sys.argv[1], sys.argv[2]) else 1)
