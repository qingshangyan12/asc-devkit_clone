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


OUTPUT_ELEMENT_COUNT = 16 * 16


def bfloat16_to_float(values):
    return (values.astype(np.uint32) << np.uint32(16)).view(np.float32)


def verify_result(output_path, golden_path):
    output = np.fromfile(output_path, dtype=np.uint16)
    golden = np.fromfile(golden_path, dtype=np.uint16)
    if output.size != OUTPUT_ELEMENT_COUNT or golden.size != OUTPUT_ELEMENT_COUNT:
        print(
            f"unexpected element count: output={output.size}, golden={golden.size}, "
            f"expected={OUTPUT_ELEMENT_COUNT}"
        )
        return False
    if output.size != golden.size:
        print(f"size mismatch: output={output.size}, golden={golden.size}")
        return False

    mismatch_indexes = np.flatnonzero(output != golden)
    output_float = bfloat16_to_float(output)
    golden_float = bfloat16_to_float(golden)
    for index in mismatch_indexes[:100]:
        print(
            f"data index: {index:06d}, expected: {golden_float[index]:-.9f} "
            f"(0x{golden[index]:04x}), actual: {output_float[index]:-.9f} (0x{output[index]:04x})"
        )
    return mismatch_indexes.size == 0


if __name__ == "__main__":
    try:
        if len(sys.argv) != 3:
            raise ValueError(f"Usage: {sys.argv[0]} <output.bin> <golden.bin>")
        if not verify_result(sys.argv[1], sys.argv[2]):
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except (OSError, ValueError) as error:
        print(error)
        sys.exit(1)
