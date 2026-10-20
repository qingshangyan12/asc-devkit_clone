# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Check every result and all three packed comparison masks."""
import sys
from pathlib import Path
import numpy as np


def verify_result(directory):
    directory = Path(directory)
    z = np.fromfile(directory / "output_z.bin", dtype=np.float32)
    golden = np.fromfile(directory / "golden_z.bin", dtype=np.float32)
    masks = np.fromfile(directory / "output_masks.bin", dtype=np.uint8)
    golden_masks = np.fromfile(directory / "golden_masks.bin", dtype=np.uint8)
    if z.size != 256 or golden.size != 256 or masks.size != 96 or golden_masks.size != 96:
        raise ValueError("Unexpected output or reference file size")
    np.testing.assert_array_equal(np.isnan(z), np.isnan(golden))
    np.testing.assert_array_equal(np.isposinf(z), np.isposinf(golden))
    np.testing.assert_array_equal(np.isneginf(z), np.isneginf(golden))
    finite = np.isfinite(golden)
    np.testing.assert_array_equal(z[finite], golden[finite])
    np.testing.assert_array_equal(masks, golden_masks)
    counts = np.unpackbits(masks.reshape(3, 32), axis=1, bitorder="little").sum(axis=1)
    print(f"NaN: {counts[0]}, +Inf: {counts[1]}, -Inf: {counts[2]}")
    print("test pass!")


if __name__ == "__main__":
    try:
        verify_result(sys.argv[1] if len(sys.argv) > 1 else "output")
    except (OSError, ValueError, AssertionError) as error:
        print(f"test failed: {error}", file=sys.stderr)
        sys.exit(1)
