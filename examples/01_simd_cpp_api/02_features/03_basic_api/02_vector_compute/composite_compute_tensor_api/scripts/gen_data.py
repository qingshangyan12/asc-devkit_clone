#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.

from pathlib import Path

import numpy as np


VECTOR_LENGTH = 64


def main() -> None:
    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    src_a = np.linspace(-1.5, 1.5, VECTOR_LENGTH, dtype=np.float32)
    src_b = (0.5 + (np.arange(VECTOR_LENGTH) % 8) * 0.125).astype(np.float32)
    golden = np.stack(
        [
            src_b + np.float32(0.25) * src_a,
            np.abs(src_a - src_b),
            np.exp(src_a - src_b),
            src_a * src_b + src_b,
            (src_a * np.float32(1.5)).astype(np.float16).astype(np.float32),
        ]
    ).astype(np.float32)

    src_a.tofile(input_dir / "src_a.bin")
    src_b.tofile(input_dir / "src_b.bin")
    golden.tofile(output_dir / "golden.bin")
    print(f"Generated {golden.shape[0]} composite compute result rows.")


if __name__ == "__main__":
    main()
