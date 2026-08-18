#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the license.

import sys

import numpy as np


ROWS = 5
VECTOR_LENGTH = 64


def main() -> None:
    if len(sys.argv) != 3:
        raise ValueError("Usage: verify_result.py <output> <golden>")
    actual = np.fromfile(sys.argv[1], dtype=np.float32).reshape(ROWS, VECTOR_LENGTH)
    golden = np.fromfile(sys.argv[2], dtype=np.float32).reshape(ROWS, VECTOR_LENGTH)
    matches = np.isclose(actual, golden, rtol=1e-4, atol=1e-4)
    matches[2] = np.isclose(actual[2], golden[2], rtol=5e-3, atol=5e-3)
    if not np.all(matches):
        row, lane = np.argwhere(~matches)[0]
        raise ValueError(
            f"mismatch at row {row}, lane {lane}: actual={actual[row, lane]}, golden={golden[row, lane]}"
        )
    print(f"test pass! max absolute error: {np.max(np.abs(actual - golden)):.9g}")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error)
        sys.exit(1)
