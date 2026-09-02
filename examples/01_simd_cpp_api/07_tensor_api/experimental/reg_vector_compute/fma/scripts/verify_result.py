#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the license.

import sys
from pathlib import Path

import numpy as np


def verify_output(
    output_dir: Path, output_name: str, golden_name: str, rtol: float, atol: float
) -> None:
    actual = np.fromfile(output_dir / output_name, dtype=np.float32)
    golden = np.fromfile(output_dir / golden_name, dtype=np.float32)
    if actual.shape != golden.shape:
        raise ValueError(
            f"{output_name}: expected shape {golden.shape}, got {actual.shape}"
        )
    matches = np.isclose(actual, golden, rtol=rtol, atol=atol)
    if not np.all(matches):
        index = int(np.flatnonzero(~matches)[0])
        raise ValueError(
            f"{output_name}: mismatch at index {index}: actual={actual[index]}, golden={golden[index]}"
        )


def main() -> None:
    if len(sys.argv) > 2:
        raise ValueError("Usage: verify_result.py [output_dir]")
    output_dir = Path(sys.argv[1]) if len(sys.argv) == 2 else Path("output")
    verify_output(output_dir, "output.bin", "golden.bin", rtol=1e-5, atol=1e-5)
    print("test pass! verified float fma.")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error)
        sys.exit(1)
