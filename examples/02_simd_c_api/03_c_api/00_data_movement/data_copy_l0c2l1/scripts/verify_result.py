#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import sys
import numpy as np

M = 128
N = 128

# scenario_num -> output_type (quantization target type, not second Mmad output type)
SCENARIO_OUTPUT = {
    1: 'half',       2: 'int8',       3: 'int8',       4: 'half',
    5: 'int8',       6: 'int8',       7: 'half',       8: 'half',
    9: 'half',       10: 'bfloat16',  11: 'bfloat16',  12: 'int4b',
    13: 'bfloat16',  14: 'int4b',     15: 'fp8_e4m3fn', 16: 'hifloat8',
    17: 'hifloat8',  18: 'float',
}

# Golden and output are both saved in the second Mmad output type:
#   int8/int4b output -> int32 (int8/int4b × identity int8/int4b → int32)
#   half/bf16/fp8/hif8/float output -> float32 (× identity → float)
GOLDEN_DTYPE = {
    'half': np.float32, 'int8': np.int32, 'int4b': np.int32,
    'bfloat16': np.float32, 'fp8_e4m3fn': np.float32,
    'hifloat8': np.float32, 'float': np.float32,
}

OUTPUT_DTYPE = GOLDEN_DTYPE  # same as golden


def verify_result(scenario_num, output_path, golden_path):
    output_type = SCENARIO_OUTPUT[scenario_num]
    dtype = GOLDEN_DTYPE[output_type]

    golden = np.fromfile(golden_path, dtype=dtype).reshape(-1)
    output = np.fromfile(output_path, dtype=dtype).reshape(-1)

    if output.size != golden.size:
        print("element count mismatch: expected %d, actual %d" % (golden.size, output.size))
        return False

    golden_f64 = golden.astype(np.float64)
    output_f64 = output.astype(np.float64)

    if output_type in ('float', 'half', 'bfloat16'):
        relative_tol = 1e-3
        absolute_tol = 1e-3
        error_tol = 1e-3
    elif output_type in ('int8', 'int4b', 'fp8_e4m3fn', 'hifloat8'):
        relative_tol = 0
        absolute_tol = 1
        error_tol = 1e-4
    else:
        relative_tol = 1e-6
        absolute_tol = 1e-9
        error_tol = 1e-4

    match = np.isclose(
        output_f64, golden_f64, rtol=relative_tol, atol=absolute_tol, equal_nan=True
    )

    mismatch_indices = np.where(~match)[0]
    for index in mismatch_indices[:101]:
        print("data index: %06d, expected: %-.9f, actual: %-.9f"
              % (index, golden_f64[index], output_f64[index]))
    error_ratio = float(mismatch_indices.size) / golden.size
    return error_ratio <= error_tol


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 19))
    parser.add_argument("output", type=str)
    parser.add_argument("golden", type=str)
    args = parser.parse_args()
    if not verify_result(args.scenarioNum, args.output, args.golden):
        print("[ERROR] result error")
        sys.exit(1)
    print("test pass!")
