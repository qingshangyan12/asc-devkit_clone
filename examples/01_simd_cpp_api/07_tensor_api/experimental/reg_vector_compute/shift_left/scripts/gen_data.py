#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the license.

from pathlib import Path

import numpy as np


vector_length = 64


def main() -> None:
    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    indexes = np.arange(vector_length, dtype=np.uint64)
    data = (indexes * np.uint64(0x1020305) + np.uint64(0x12345678)).astype(np.uint32)
    shifts = (indexes % np.uint64(8)).astype(np.int32)
    golden = np.left_shift(data.astype(np.uint64), shifts.astype(np.uint64)).astype(
        np.uint32
    )

    data.tofile(input_dir / "input.bin")
    shifts.tofile(input_dir / "shifts.bin")
    golden.tofile(output_dir / "golden.bin")
    print("Generated uint32 register shift-left input and golden data.")


if __name__ == "__main__":
    main()
