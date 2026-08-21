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


INPUT_LENGTH = 256
INITIAL_VALUE = np.uint16(3)


def main() -> None:
    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    indexes = np.arange(INPUT_LENGTH, dtype=np.uint16)
    data = (indexes * np.uint16(37) + indexes % np.uint16(11)).astype(np.uint8)
    frequency = np.bincount(data, minlength=256).astype(np.uint16)
    golden = np.stack(
        [
            frequency[:128] + INITIAL_VALUE,
            frequency[128:] + INITIAL_VALUE,
            np.cumsum(frequency[:128], dtype=np.uint16) + INITIAL_VALUE,
            np.cumsum(frequency, dtype=np.uint16)[128:] + INITIAL_VALUE,
        ]
    ).astype(np.uint16)

    data.tofile(input_dir / "input.bin")
    golden.tofile(output_dir / "golden.bin")
    print("Generated frequency and cumulative histogram results for bin0 and bin1.")


if __name__ == "__main__":
    main()
