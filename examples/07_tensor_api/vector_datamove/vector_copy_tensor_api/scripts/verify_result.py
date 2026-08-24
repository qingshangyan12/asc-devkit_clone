#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import sys

import numpy as np


def verify_file(output_path, golden_path):
    dtype = np.float16
    output = np.fromfile(output_path, dtype=dtype)
    golden = np.fromfile(golden_path, dtype=dtype)
    if output.shape != golden.shape:
        print(f"[ERROR] shape mismatch for {output_path}: {output.shape} != {golden.shape}")
        return False

    matched = np.isclose(output, golden, rtol=0.0, atol=0.0, equal_nan=True)
    if np.all(matched):
        return True

    indexes = np.flatnonzero(~matched)
    for index in indexes[:10]:
        print(f"[ERROR] {output_path}[{index}]: expected {golden[index]}, actual {output[index]}")
    return False


def main():
    if verify_file("./output/output.bin", "./output/golden.bin"):
        print("test pass!")
        return 0
    return 1


if __name__ == "__main__":
    sys.exit(main())
