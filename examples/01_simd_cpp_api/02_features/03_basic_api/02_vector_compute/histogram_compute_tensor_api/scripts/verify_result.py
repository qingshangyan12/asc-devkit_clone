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


def main() -> None:
    if len(sys.argv) != 3:
        raise ValueError("Usage: verify_result.py <output> <golden>")
    actual = np.fromfile(sys.argv[1], dtype=np.uint16)
    golden = np.fromfile(sys.argv[2], dtype=np.uint16)
    if actual.shape != golden.shape or not np.array_equal(actual, golden):
        index = int(np.flatnonzero(actual != golden)[0])
        raise ValueError(f"mismatch at index {index}: actual={actual[index]}, golden={golden[index]}")
    print("test pass!")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error)
        sys.exit(1)
