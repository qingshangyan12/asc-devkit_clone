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


def parse_dump(text, desc, value_type=int):
    match = re.search(rf"DumpTensor: desc={desc},[^\n]*\n\[(.*?)\]", text, re.S)
    if match is None:
        raise RuntimeError(f"missing DumpTensor desc={desc}")
    return [
        value_type(item.strip())
        for item in match.group(1).replace("\n", " ").split(",")
        if item.strip()
    ]


def parse_bias_cases(text):
    cases = []
    for match in re.finditer(r"BIAS_CASE\[\s*(\d+)\] input=(-?\d+(?:\.\d+)?)", text):
        cases.append((int(match.group(1)), float(match.group(2))))
    cases.sort(key=lambda item: item[0])
    if not cases:
        raise RuntimeError("missing BIAS_CASE lines")
    return [item[1] for item in cases]


def parse_fixpipe_cases(text):
    cases = []
    pattern = r"FIXPIPE_CASE\[\s*(\d+)\] input=0x([0-9a-fA-F]+) expected=0x([0-9a-fA-F]+)"
    for match in re.finditer(pattern, text):
        cases.append((int(match.group(1)), int(match.group(2), 16), int(match.group(3), 16)))
    cases.sort(key=lambda item: item[0])
    if not cases:
        raise RuntimeError("missing FIXPIPE_CASE lines")
    return cases


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
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <run.log>")
        return 1

    text = Path(sys.argv[1]).read_text(encoding="utf-8")
    bias_input = parse_bias_cases(text)
    fixpipe_cases = parse_fixpipe_cases(text)
    fixpipe_input = [item[1] for item in fixpipe_cases]
    fixpipe_expected = [item[2] for item in fixpipe_cases]

    checks = [
        check_equal("Bias L1 raw input check", parse_dump(text, 101, float), bias_input),
        check_equal("Bias Table raw data check", parse_dump(text, 201, float), bias_input),
        check_equal("Fixpipe L1 raw input check", parse_dump(text, 102), fixpipe_input),
        check_equal("Fixpipe quant visible-mask check", parse_dump(text, 301), fixpipe_expected),
        "QuantMatmulHifp8 output check: PASS" in text,
    ]
    print(f"QuantMatmulHifp8 result check: {'PASS' if checks[-1] else 'FAIL'}")
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
