#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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
import sys
import numpy as np

M = 128
N = 128

# scenario_num -> output_type
#   'half'        -> golden np.float16,  output np.float32 (half Mmad output)
#   'int8'        -> golden np.int8,     output np.int32  (int8 Mmad output)
#   'bfloat16'    -> golden np.uint16,   output np.float32 (bf16 Mmad output)
#   'int4b'       -> golden np.uint8(packed), output np.int32 (int4 Mmad output)
#   'fp8_e4m3fn'  -> golden np.uint8,    output np.float32 (fp8 Mmad output)
#   'hifloat8'    -> golden np.uint8,    output np.float32 (hif8 Mmad output)
#   'float'       -> golden np.float32,  output np.float32 (float Mmad output)
SCENARIO_OUTPUT = {
    1: 'int8',       2: 'int8',       3: 'int8',       4: 'int8',
    5: 'half',       6: 'half',       7: 'half',       8: 'half',
    9: 'bfloat16',   10: 'bfloat16',
    11: 'int4b',     12: 'bfloat16',  13: 'int4b',
    14: 'fp8_e4m3fn', 15: 'hifloat8', 16: 'hifloat8',
    17: 'float',
}

GOLDEN_DTYPE = {
    'half': np.float16, 'int8': np.int8, 'int4b': np.uint8,
    'bfloat16': np.uint16, 'fp8_e4m3fn': np.uint8,
    'hifloat8': np.uint8, 'float': np.float32,
}

OUTPUT_DTYPE = {
    'half': np.float32, 'int8': np.int32, 'int4b': np.int32,
    'bfloat16': np.float32, 'fp8_e4m3fn': np.float32,
    'hifloat8': np.float32, 'float': np.float32,
}


def unpack_int4b(packed):
    low = (packed & 0x0F).astype(np.int8)
    high = ((packed >> 4) & 0x0F).astype(np.int8)
    low[low >= 8] -= 16
    high[high >= 8] -= 16
    result = np.empty(packed.size * 2, dtype=np.int8)
    result[0::2] = low
    result[1::2] = high
    return result


def bf16_to_float32(bf16_arr):
    return (bf16_arr.astype(np.uint32) << 16).view(np.float32).astype(np.float64)


def verify_result(scenario_num, output_path, golden_path):
    output_type = SCENARIO_OUTPUT[scenario_num]

    golden = np.fromfile(golden_path, dtype=GOLDEN_DTYPE[output_type]).reshape(-1)
    output = np.fromfile(output_path, dtype=OUTPUT_DTYPE[output_type]).reshape(-1)

    if output_type == 'int4b':
        golden = unpack_int4b(golden)

    if output.size != golden.size:
        print("element count mismatch: expected %d, actual %d" % (golden.size, output.size))
        return False

    if output_type == 'bfloat16':
        golden_f64 = bf16_to_float32(golden)
    else:
        golden_f64 = golden.astype(np.float64)
    output_f64 = output.astype(np.float64)

    if output_type in ('float', 'half', 'bfloat16'):
        relative_tol = 1e-3
        absolute_tol = 1e-3
        error_tol = 1e-3
    elif output_type in ('int8', 'int4b', 'fp8_e4m3fn', 'hifloat8'):
        relative_tol = 0
        absolute_tol = 1
        error_tol = 1e-3
    else:
        relative_tol = 1e-6
        absolute_tol = 1e-9
        error_tol = 1e-3

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
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 18))
    parser.add_argument("output", type=str)
    parser.add_argument("golden", type=str)
    args = parser.parse_args()
    if not verify_result(args.scenarioNum, args.output, args.golden):
        print("[ERROR] result error")
        sys.exit(1)
    print("test pass!")
