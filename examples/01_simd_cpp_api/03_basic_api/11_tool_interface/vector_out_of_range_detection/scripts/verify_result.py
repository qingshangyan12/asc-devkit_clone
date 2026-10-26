# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Check every result and both packed comparison masks."""

import sys
from pathlib import Path
import numpy as np


def read_result(file_path):
    data = Path(file_path).read_bytes()
    data_bytes = 256 * np.dtype(np.float32).itemsize
    if len(data) != data_bytes + 64:
        raise ValueError(f"Unexpected file size: {file_path}, expected {data_bytes + 64} bytes")
    z = np.frombuffer(data, dtype=np.float32, count=256)
    masks = np.frombuffer(data, dtype=np.uint8, count=64, offset=data_bytes)
    return z, masks


def verify_result(output_path, golden_path):
    z, masks = read_result(output_path)
    golden, golden_masks = read_result(golden_path)
    np.testing.assert_array_equal(np.isposinf(z), np.isposinf(golden))
    np.testing.assert_array_equal(np.isneginf(z), np.isneginf(golden))
    finite = np.isfinite(golden)
    np.testing.assert_array_equal(z[finite], golden[finite])
    np.testing.assert_array_equal(masks, golden_masks)
    print("test pass!")


if __name__ == "__main__":
    try:
        if len(sys.argv) != 3:
            raise ValueError("Usage: verify_result.py output_file golden_file")
        verify_result(sys.argv[1], sys.argv[2])
    except (OSError, ValueError, AssertionError) as error:
        print(f"test failed: {error}", file=sys.stderr)
        sys.exit(1)
