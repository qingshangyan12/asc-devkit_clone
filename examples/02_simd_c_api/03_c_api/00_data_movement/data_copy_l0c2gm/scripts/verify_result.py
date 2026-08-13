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


RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-6
ERROR_TOL = 1e-3
# VQF322F32_PRE uses processor float32 (10 mantissa bits), precision ~2/1000.
F32_RELATIVE_TOL = 2e-3
F32_ABSOLUTE_TOL = 1e-6


# scenario_num -> output_type
#   'half'        -> np.float16
#   'int8'        -> np.int8
#   'int4b'       -> np.uint8 (packed, 2 values per byte)
#   'bfloat16'    -> np.uint16 (bit pattern)
#   'fp8_e4m3fn'  -> np.uint8
#   'hifloat8'    -> np.uint8
#   'float'       -> np.float32
SCENARIO_OUTPUT = {
    1: 'half',       2: 'half',       3: 'int8',       4: 'int8',       5: 'int8',
    6: 'int8',       7: 'int4b',      8: 'bfloat16',   9: 'half',       10: 'bfloat16',
    11: 'int4b',     12: 'fp8_e4m3fn', 13: 'hifloat8', 14: 'hifloat8',  15: 'float',
    16: 'half',      17: 'bfloat16',
}

NP_DTYPE = {
    'half': np.float16, 'int8': np.int8, 'int4b': np.uint8,
    'bfloat16': np.uint16, 'fp8_e4m3fn': np.uint8,
    'hifloat8': np.uint8, 'float': np.float32,
}


def unpack_int4b(packed):
    """Unpack packed bytes into int4 values (range -8..7)."""
    low = (packed & 0x0F).astype(np.int8)
    high = ((packed >> 4) & 0x0F).astype(np.int8)
    low[low >= 8] -= 16
    high[high >= 8] -= 16
    result = np.empty(packed.size * 2, dtype=np.int8)
    result[0::2] = low
    result[1::2] = high
    return result


def verify_result(scenario_num, output_path, golden_path):
    output_type = SCENARIO_OUTPUT[scenario_num]
    dtype = NP_DTYPE[output_type]

    output = np.fromfile(output_path, dtype=dtype).reshape(-1)
    golden = np.fromfile(golden_path, dtype=dtype).reshape(-1)

    if output_type == 'int4b':
        output = unpack_int4b(output)
        golden = unpack_int4b(golden)

    if output.size != golden.size:
        print(
            "element count mismatch: expected %d, actual %d"
            % (golden.size, output.size)
        )
        return False

    if output_type == 'float':
        match = np.isclose(
            output, golden, rtol=F32_RELATIVE_TOL, atol=F32_ABSOLUTE_TOL, equal_nan=True
        )
    elif output_type in ('half', 'bfloat16'):
        if output_type == 'bfloat16':
            output_cmp = (output.astype(np.uint32) << 16).view(np.float32)
            golden_cmp = (golden.astype(np.uint32) << 16).view(np.float32)
        else:
            output_cmp = output
            golden_cmp = golden
        match = np.isclose(
            output_cmp, golden_cmp, rtol=RELATIVE_TOL, atol=ABSOLUTE_TOL, equal_nan=True
        )
    elif output_type in ('fp8_e4m3fn', 'hifloat8'):
        match = np.isclose(
            output.astype(np.float64), golden.astype(np.float64),
            rtol=0, atol=1, equal_nan=True
        )
    else:
        match = (output == golden)

    mismatch_indices = np.where(~match)[0]
    for index in mismatch_indices[:101]:
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f"
            % (index, golden[index], output[index])
        )
    error_ratio = float(mismatch_indices.size) / golden.size
    return error_ratio <= ERROR_TOL


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
