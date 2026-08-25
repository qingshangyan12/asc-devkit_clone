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
import sys

import numpy as np


def verify_result(output_path, golden_path):
    output = np.fromfile(output_path, dtype=np.float32)
    golden = np.fromfile(golden_path, dtype=np.float32)
    if output.size != golden.size:
        print(f"[ERROR] element count mismatch: output={output.size}, golden={golden.size}")
        return False

    differences = np.flatnonzero(output.view(np.uint32) != golden.view(np.uint32))
    if differences.size != 0:
        index = int(differences[0])
        print(
            "[ERROR] first mismatch at index %d: expected=%g (0x%08x), actual=%g (0x%08x)"
            % (index, golden[index], golden.view(np.uint32)[index], output[index], output.view(np.uint32)[index])
        )
        return False
    print("test pass!")
    return True


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("output", type=str)
    parser.add_argument("golden", type=str)
    args = parser.parse_args()
    sys.exit(0 if verify_result(args.output, args.golden) else 1)
