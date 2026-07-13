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

import re
import sys
from pathlib import Path

import numpy as np


BIAS_ELEMENT_COUNT = 16
QUANT_ELEMENT_COUNT = 16
FIXPIPE_QUANT_VISIBLE_MASK = 0x00007FE0FFFFE0FF


def parse_dump(text, desc, value_type=int):
    matches = re.findall(rf"DumpTensor: desc={desc},[^\n]*\n\[(.*?)\]", text, re.S)
    if len(matches) != 1:
        raise RuntimeError(f"expected one DumpTensor desc={desc}, found {len(matches)}")
    values = [
        value_type(item.strip())
        for item in matches[0].replace("\n", " ").split(",")
        if item.strip()
    ]
    if len(values) != BIAS_ELEMENT_COUNT:
        raise RuntimeError(f"unexpected DumpTensor desc={desc} element count: {len(values)}")
    return values


def read_input(bias_path, scale_path):
    bias = np.fromfile(bias_path, dtype=np.float32)
    quant = np.fromfile(scale_path, dtype=np.uint64)
    if bias.size != BIAS_ELEMENT_COUNT:
        raise RuntimeError(f"unexpected bias element count: {bias.size}")
    if quant.size != QUANT_ELEMENT_COUNT:
        raise RuntimeError(f"unexpected scale element count: {quant.size}")
    return bias.tolist(), [int(value) for value in quant]


def check_equal(name, actual, expected):
    if actual == expected:
        print(f"{name}: PASS")
        return True
    print(f"{name}: FAIL")
    if len(actual) != len(expected):
        print(f"  length mismatch: actual={len(actual)}, expected={len(expected)}")
    for index, (actual_value, expected_value) in enumerate(zip(actual, expected)):
        if actual_value != expected_value:
            if isinstance(actual_value, int) and isinstance(expected_value, int):
                print(f"  [{index:02d}] actual=0x{actual_value:016x}, expected=0x{expected_value:016x}")
            else:
                print(f"  [{index:02d}] actual={actual_value}, expected={expected_value}")
    return False


def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <run.log> <bias.bin> <scale.bin>")
        return 1

    text = Path(sys.argv[1]).read_text(encoding="utf-8")
    bias_input, fixpipe_input = read_input(Path(sys.argv[2]), Path(sys.argv[3]))
    fixpipe_expected = [value & FIXPIPE_QUANT_VISIBLE_MASK for value in fixpipe_input]

    checks = [
        check_equal("Bias L1 raw input check", parse_dump(text, 101, float), bias_input),
        check_equal("Bias Table raw data check", parse_dump(text, 201, float), bias_input),
        check_equal("Fixpipe L1 raw input check", parse_dump(text, 102), fixpipe_input),
        check_equal("Fixpipe quant visible-mask check", parse_dump(text, 301), fixpipe_expected),
    ]
    if all(checks):
        print("test pass!")
        return 0
    return 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError, ValueError) as error:
        print(error)
        sys.exit(1)
