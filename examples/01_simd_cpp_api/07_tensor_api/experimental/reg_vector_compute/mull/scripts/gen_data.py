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
uint32_mask = np.uint64(0xFFFFFFFF)


def main() -> None:
    Path("input").mkdir(exist_ok=True)
    Path("output").mkdir(exist_ok=True)

    indexes = np.arange(vector_length, dtype=np.uint64)
    src0 = (
        (np.uint64(0xF0000000) + indexes * np.uint64(0x01020304)) & uint32_mask
    ).astype(np.uint32)
    src1 = (
        (np.uint64(0x00010001) + indexes * np.uint64(0x00100011)) & uint32_mask
    ).astype(np.uint32)
    product = src0.astype(np.uint64) * src1.astype(np.uint64)

    src0.tofile(Path("input") / "src0_input.bin")
    src1.tofile(Path("input") / "src1_input.bin")
    (product & uint32_mask).astype(np.uint32).tofile(Path("output") / "low_golden.bin")
    (product >> np.uint64(32)).astype(np.uint32).tofile(
        Path("output") / "high_golden.bin"
    )
    print("Generated uint32 mull input and golden data.")


if __name__ == "__main__":
    main()
