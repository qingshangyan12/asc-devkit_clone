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

    src_a = np.linspace(-2.0, 2.0, VECTOR_LENGTH, dtype=np.float32)
    src_b = (0.75 + (np.arange(VECTOR_LENGTH) % 7) * 0.125).astype(np.float32)
    slope = np.linspace(0.05, 0.35, VECTOR_LENGTH, dtype=np.float32)
    positive = np.abs(src_a) + np.float32(0.5)
    golden = np.stack(
        [
            np.abs(src_a),
            np.exp(src_a),
            np.sqrt(positive),
            np.log(positive),
            np.log2(positive),
            np.log10(positive),
            -src_a,
            np.maximum(src_a, np.float32(0.0)),
            np.where(src_a >= 0.0, src_a, src_a * slope),
            np.where(src_a >= 0.0, src_a, src_a * np.float32(0.1)),
            src_a + src_b,
            src_a - src_b,
            src_a * src_b,
            src_a / src_b,
            np.maximum(src_a, src_b),
            np.minimum(src_a, src_b),
        ]
    ).astype(np.float32)

    src_a.tofile(input_dir / "src_a.bin")
    src_b.tofile(input_dir / "src_b.bin")
    slope.tofile(input_dir / "slope.bin")
    golden.tofile(output_dir / "golden.bin")
    print(f"Generated {golden.shape[0]} basic arithmetic result rows.")


if __name__ == "__main__":
    main()
