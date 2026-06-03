#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import sys
import numpy as np


RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-3


def get_dtype(dtype_name):
    dtype_alias = {
        "int8": np.int8,
        "uint8": np.uint8,
        "half": np.float16,
        "float16": np.float16,
        "float": np.float32,
        "float32": np.float32,
    }
    if dtype_name not in dtype_alias:
        raise ValueError(f"unsupported dtype: {dtype_name}")
    return dtype_alias[dtype_name]


def verify_result(output_path, golden_path, dtype_name="float32"):
    dtype = get_dtype(dtype_name)
    output = np.fromfile(output_path, dtype=dtype).reshape(-1)
    golden = np.fromfile(golden_path, dtype=dtype).reshape(-1)
    if output.size != golden.size:
        raise ValueError(f"size mismatch: output={output.size}, golden={golden.size}")

    if np.issubdtype(dtype, np.integer):
        compare = output == golden
    else:
        compare = np.isclose(output, golden, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL, equal_nan=True)

    mismatch = np.where(compare == False)[0]
    for index, real_index in enumerate(mismatch[:100]):
        golden_data = golden[real_index]
        output_data = output[real_index]
        if np.issubdtype(dtype, np.integer):
            print("data index: %06d, expected: %d, actual: %d" % (real_index, golden_data, output_data))
        else:
            denom = golden_data if golden_data != 0 else 1.0
            print(
                "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
                (real_index, golden_data, output_data, abs(output_data - golden_data) / abs(denom))
            )
    return mismatch.size == 0


if __name__ == "__main__":
    try:
        dtype_arg = sys.argv[3] if len(sys.argv) > 3 else "float32"
        if not verify_result(sys.argv[1], sys.argv[2], dtype_arg):
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as err:
        print(err)
        sys.exit(1)
