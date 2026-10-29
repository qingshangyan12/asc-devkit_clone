#!/usr/bin/env python3
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
import ml_dtypes


def gen_data(seed):
    m = n = k = 8192
    rng = np.random.default_rng(seed)
    Path("input").mkdir(exist_ok=True)
    Path("output").mkdir(exist_ok=True)
    a = rng.uniform(-1, 1, (m, k)).astype(ml_dtypes.float8_e4m3fn)
    b = rng.uniform(-1, 1, (n, k)).astype(ml_dtypes.float8_e4m3fn)
    a_scale = rng.integers(125, 130, (m, k // 32), dtype=np.uint8)
    b_scale = rng.integers(125, 130, (n, k // 32), dtype=np.uint8)
    a.tofile("input/x1_gm.bin")
    b.tofile("input/x2_gm.bin")
    a_scale.tofile("input/x1_scale_gm.bin")
    b_scale.tofile("input/x2_scale_gm.bin")
    a_full = a.astype(np.float64).reshape(m, k // 32, 32)
    b_full = b.astype(np.float64).reshape(n, k // 32, 32)
    a_full *= np.exp2(a_scale.astype(np.float64) - 127)[..., None]
    b_full *= np.exp2(b_scale.astype(np.float64) - 127)[..., None]
    a_full = a_full.reshape(m, k)
    b_full = b_full.reshape(n, k)
    with open("output/golden.bin", "wb") as golden:
        for row in range(0, m, 256):
            (a_full[row : row + 256] @ b_full.T).astype(np.float16).tofile(golden)
    print(f"Generated 8192 x 8192 x 8192, seed={seed}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--seed", type=int, default=2026)
    gen_data(parser.parse_args().seed)
