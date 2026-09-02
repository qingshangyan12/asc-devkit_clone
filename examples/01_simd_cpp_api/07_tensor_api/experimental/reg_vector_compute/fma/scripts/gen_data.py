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

    input_a = np.linspace(-1.0, 1.0, vector_length, dtype=np.float32)
    input_b = np.linspace(0.25, 1.25, vector_length, dtype=np.float32)
    input_c = np.linspace(-0.5, 0.5, vector_length, dtype=np.float32)
    golden = (
        input_a.astype(np.float64) * input_b.astype(np.float64)
        + input_c.astype(np.float64)
    ).astype(np.float32)
    input_a.tofile(input_dir / "input_a.bin")
    input_b.tofile(input_dir / "input_b.bin")
    input_c.tofile(input_dir / "input_c.bin")
    golden.tofile(output_dir / "golden.bin")
    print("Generated float fma input and golden data.")


if __name__ == "__main__":
    main()
