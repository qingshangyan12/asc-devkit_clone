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

import os
import sys

import numpy as np


TOTAL_LENGTH = 1024
ABSOLUTE_TOLERANCE = 1e-3
SAMPLE_COUNT = 8


def print_file_sample(label, path):
    if not os.path.isfile(path):
        print("%s: unavailable (%s)" % (label, path))
        return
    data = np.fromfile(path, dtype=np.float32, count=SAMPLE_COUNT)
    print("%s (float32, first %d): %s" % (label, data.size, data))


def print_samples(output_path, golden_path):
    input_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(golden_path))), "input")
    print("===== data samples =====")
    print_file_sample("input/x.bin", os.path.join(input_dir, "x.bin"))
    print_file_sample("input/y.bin", os.path.join(input_dir, "y.bin"))
    print_file_sample("output", output_path)
    print_file_sample("golden", golden_path)
    print("========================")


def verify_result(output_path, golden_path):
    print_samples(output_path, golden_path)
    if os.path.getsize(output_path) != TOTAL_LENGTH * np.dtype(np.float32).itemsize:
        print("output file size is invalid")
        return False
    if os.path.getsize(golden_path) != TOTAL_LENGTH * np.dtype(np.float32).itemsize:
        print("golden file size is invalid")
        return False

    output = np.fromfile(output_path, dtype=np.float32)
    golden = np.fromfile(golden_path, dtype=np.float32)
    differences = np.abs(output - golden)
    failed_indexes = np.where(differences > ABSOLUTE_TOLERANCE)[0]
    for index in failed_indexes[:101]:
        print(
            "data index: %04d, expected: %-.9f, actual: %-.9f, absdiff: %-.9f"
            % (index, golden[index], output[index], differences[index])
        )
    return failed_indexes.size == 0


if __name__ == "__main__":
    try:
        if not verify_result(sys.argv[1], sys.argv[2]):
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as error:
        print(error)
        sys.exit(1)
