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


VECTOR_LENGTH = 64


def main() -> None:
    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    src_a = np.arange(VECTOR_LENGTH, dtype=np.int32) - np.int32(24)
    src_b = (3 * np.arange(VECTOR_LENGTH, dtype=np.int32) + np.int32(1))
    shifts = (np.arange(VECTOR_LENGTH, dtype=np.int32) % np.int32(4))
    golden = np.stack(
        [
            (src_a == 0).astype(np.int32),
            src_a & src_b,
            src_a | np.int32(16),
            np.int32(7) ^ src_a,
            src_a << shifts,
            src_a << np.int32(2),
            src_a >> shifts,
            src_a >> np.int32(2),
        ]
    ).astype(np.int32)

    src_a.tofile(input_dir / "src_a.bin")
    src_b.tofile(input_dir / "src_b.bin")
    shifts.tofile(input_dir / "shifts.bin")
    golden.tofile(output_dir / "golden.bin")
    print(f"Generated {golden.shape[0]} logical compute result rows.")


if __name__ == "__main__":
    main()
