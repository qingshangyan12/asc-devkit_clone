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
UINT32_MODULUS = np.uint64(1 << 32)
UINT32_MASK = UINT32_MODULUS - np.uint64(1)
ADD_SCALAR = np.float32(0.5)
SUB_SCALAR = np.float32(0.75)
MUL_SCALAR = np.float32(1.25)
DIV_SCALAR = np.float32(2.0)
MAX_SCALAR = np.float32(0.25)
MIN_SCALAR = np.float32(-0.25)


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
            src_a + ADD_SCALAR,
            ADD_SCALAR + src_a,
            src_a - src_b,
            src_a - SUB_SCALAR,
            SUB_SCALAR - src_a,
            src_a * src_b,
            src_a * MUL_SCALAR,
            MUL_SCALAR * src_a,
            src_a / src_b,
            src_a / DIV_SCALAR,
            DIV_SCALAR / positive,
            np.maximum(src_a, src_b),
            np.maximum(src_a, MAX_SCALAR),
            np.maximum(MAX_SCALAR, src_a),
            np.minimum(src_a, src_b),
            np.minimum(src_a, MIN_SCALAR),
            np.minimum(MIN_SCALAR, src_a),
        ]
    ).astype(np.float32)

    lanes = np.arange(VECTOR_LENGTH, dtype=np.uint64)
    even_lane = lanes % 2 == 0
    uint32_src_a_u64 = np.where(
        even_lane,
        np.uint64(0xFFFFFFE0) + lanes % np.uint64(32),
        lanes * np.uint64(17) + np.uint64(3),
    )
    uint32_src_b_u64 = np.where(
        even_lane,
        np.uint64(0x40) + lanes % np.uint64(8),
        np.uint64(0x1000) + lanes * np.uint64(13),
    )
    uint32_src_a = uint32_src_a_u64.astype(np.uint32)
    uint32_src_b = uint32_src_b_u64.astype(np.uint32)

    add_wide = uint32_src_a_u64 + uint32_src_b_u64
    add_carry = add_wide >> np.uint64(32)
    add_c_wide = add_wide + add_carry
    sub_carry = (uint32_src_a_u64 >= uint32_src_b_u64).astype(np.uint64)
    sub_wide = uint32_src_a_u64 + UINT32_MODULUS - uint32_src_b_u64
    sub_c_wide = uint32_src_a_u64 + UINT32_MASK - uint32_src_b_u64 + sub_carry
    product_wide = uint32_src_a_u64 * uint32_src_b_u64
    uint32_golden = np.stack(
        [
            add_wide & UINT32_MASK,
            add_carry,
            add_c_wide & UINT32_MASK,
            add_c_wide >> np.uint64(32),
            sub_wide & UINT32_MASK,
            sub_carry,
            sub_c_wide & UINT32_MASK,
            sub_c_wide >> np.uint64(32),
            product_wide & UINT32_MASK,
            product_wide >> np.uint64(32),
        ]
    ).astype(np.uint32)

    src_a.tofile(input_dir / "src_a.bin")
    src_b.tofile(input_dir / "src_b.bin")
    slope.tofile(input_dir / "slope.bin")
    uint32_src_a.tofile(input_dir / "uint32_src_a.bin")
    uint32_src_b.tofile(input_dir / "uint32_src_b.bin")
    golden.tofile(output_dir / "golden.bin")
    uint32_golden.tofile(output_dir / "uint32_golden.bin")
    print(
        f"Generated {golden.shape[0]} float rows and "
        f"{uint32_golden.shape[0]} uint32 rows."
    )


if __name__ == "__main__":
    main()
