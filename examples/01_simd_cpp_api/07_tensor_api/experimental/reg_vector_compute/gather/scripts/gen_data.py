# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
from pathlib import Path

import numpy as np


def gen_golden_data_gather(case, source_path, index_path, golden_path):
    rng = np.random.default_rng(2026)
    total_length = 128
    source = rng.uniform(0, 1, total_length).astype(np.float16)
    if case == "random":
        indexes = rng.integers(0, total_length, total_length, dtype=np.uint16)
        indexes[:2] = [0, total_length - 1]
    elif case == "identity":
        indexes = np.arange(total_length, dtype=np.uint16)
    elif case == "reverse":
        indexes = np.arange(total_length - 1, -1, -1, dtype=np.uint16)
    else:
        indexes = np.full(total_length, total_length - 1, dtype=np.uint16)
    for path in (source_path, index_path, golden_path):
        path.parent.mkdir(parents=True, exist_ok=True)
    source.tofile(source_path)
    indexes.tofile(index_path)
    source[indexes].tofile(golden_path)
    print(f"Generated {case}: half/uint16, {total_length} elements")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--case", choices=["random", "identity", "reverse", "repeated"], default="random")
    parser.add_argument("--source", type=Path, required=True, help="Source data output path")
    parser.add_argument("--indexes", type=Path, required=True, help="Index data output path")
    parser.add_argument("--golden", type=Path, required=True, help="Golden data output path")
    args = parser.parse_args()
    gen_golden_data_gather(args.case, args.source, args.indexes, args.golden)
