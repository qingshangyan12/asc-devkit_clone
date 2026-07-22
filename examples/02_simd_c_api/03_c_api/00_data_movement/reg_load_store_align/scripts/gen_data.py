# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

import argparse
from pathlib import Path

import numpy as np


INPUT_LENGTH = 1024
VECTOR_LENGTH = 128


def generate_golden(scenario_num, src0, src1):
    if scenario_num == 1:
        output = np.zeros(INPUT_LENGTH, dtype=np.float16)
        output[: INPUT_LENGTH - 3] = src0[: INPUT_LENGTH - 3] + src1[: INPUT_LENGTH - 3]
        return output
    if scenario_num in (2, 3):
        return src0 + src1
    if scenario_num == 4:
        output = np.empty(INPUT_LENGTH // 2, dtype=np.float16)
        for repeat_index in range(INPUT_LENGTH // VECTOR_LENGTH // 2):
            src_offset = repeat_index * VECTOR_LENGTH * 2
            dst_offset = repeat_index * VECTOR_LENGTH
            for block_index in range(8):
                dst_start = dst_offset + block_index * 16
                src_start = src_offset + block_index * 32
                output[dst_start : dst_start + 16] = (
                    src0[src_start : src_start + 16] + src1[src_start : src_start + 16]
                )
        return output
    if scenario_num == 5:
        return np.repeat(
            src0[::VECTOR_LENGTH] + src1[::VECTOR_LENGTH], VECTOR_LENGTH
        ).astype(np.float16)
    return np.repeat(src0 + src1, 2).astype(np.float16)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, choices=range(1, 7), required=True)
    args = parser.parse_args()

    sample_dir = Path(__file__).resolve().parent.parent
    input_dir = sample_dir / "input"
    output_dir = sample_dir / "output"
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)

    src0 = np.arange(INPUT_LENGTH, dtype=np.float16)
    src1 = np.arange(INPUT_LENGTH, dtype=np.float16)
    golden = generate_golden(args.scenarioNum, src0, src1)
    src0.tofile(input_dir / "input_x.bin")
    src1.tofile(input_dir / "input_y.bin")
    golden.tofile(output_dir / "golden.bin")


if __name__ == "__main__":
    main()
