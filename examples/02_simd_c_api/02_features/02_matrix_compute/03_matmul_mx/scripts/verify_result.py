#!/usr/bin/env python3
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
from pathlib import Path
import numpy as np


def verify_result(output_path, golden_path):
    expected_bytes = 8192 * 8192 * 2
    if any(Path(path).stat().st_size != expected_bytes for path in (output_path, golden_path)):
        raise ValueError("Incorrect file size")
    output = np.fromfile(output_path, dtype=np.float16)
    golden = np.fromfile(golden_path, dtype=np.float16)
    if output.size != 8192 * 8192 or golden.size != output.size:
        raise ValueError("Expected exactly 8192 * 8192 half elements in both files")
    errors = 0
    max_abs = 0.0
    for start in range(0, output.size, 1024 * 1024):
        actual = output[start : start + 1024 * 1024].astype(np.float32)
        expected = golden[start : start + 1024 * 1024].astype(np.float32)
        if not np.all(np.isfinite(actual)) or not np.all(np.isfinite(expected)):
            raise ValueError("Nonfinite output or golden")
        diff = np.abs(actual - expected)
        max_abs = max(max_abs, float(diff.max()))
        errors += np.count_nonzero(diff > 0.001 + 0.001 * np.abs(expected))
    print(f"Compared {output.size} elements; errors={errors}; max_abs={max_abs:g}")
    return errors == 0


if __name__ == "__main__":
    try:
        if len(sys.argv) != 3:
            raise ValueError("Usage: verify_result.py output.bin golden.bin")
        if not verify_result(*sys.argv[1:]):
            raise ValueError("Verification failed")
        print("test pass!")
    except (OSError, ValueError) as error:
        print(error, file=sys.stderr)
        sys.exit(1)
