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
from pathlib import Path

import numpy as np


def verify_result(output_path, golden_path):
    expected_size = 128 * np.dtype(np.uint16).itemsize
    for filename in [output_path, golden_path]:
        if filename.stat().st_size != expected_size:
            raise ValueError(f"{filename}: expected {expected_size} bytes")
    actual = np.fromfile(output_path, dtype=np.uint16)
    expected = np.fromfile(golden_path, dtype=np.uint16)
    mismatches = np.flatnonzero(actual != expected)
    if mismatches.size:
        print(f"FAIL: {mismatches.size}/128 mismatches; first index {mismatches[0]}")
        return False
    print("PASS: 128/128 elements match golden bit-for-bit")
    return True


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True, help="Actual result input path")
    parser.add_argument("--golden", type=Path, required=True, help="Golden result input path")
    args = parser.parse_args()
    raise SystemExit(0 if verify_result(args.output, args.golden) else 1)
