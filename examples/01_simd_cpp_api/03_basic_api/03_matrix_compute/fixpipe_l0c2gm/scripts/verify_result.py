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
import argparse
import numpy as np

# Scenario table: (input_type, l0c_type, output_type, quant_mode, format, is_tensor)
SCENARIO_TABLE = {
    1:  ('half', 'float32', 'float32',    'NoQuant',              'NZ', False),
    2:  ('half', 'float32', 'float32',    'NoQuant',              'ND', False),
    3:  ('half', 'float32', 'float32',    'NoQuant',              'DN', False),
    4:  ('half', 'float32', 'int8',       'QF322B8_PRE',          'ND', False),
    5:  ('half', 'float32', 'int8',       'VQF322B8_PRE',         'ND', True),
    6:  ('half', 'float32', 'float32',    'NoQuant+ReLU',         'ND', False),
    7:  ('half', 'float32', 'float32',    'NoQuant+ChannelSplit', 'NZ', False),
    8:  ('half', 'float32', 'int8',       'QF322B8_PRE',          'NZ', False),
    9:  ('half', 'float32', 'int8',       'VQF322B8_PRE',         'NZ', True),
    10: ('half', 'float32', 'uint8',      'QF322B8_PRE',          'ND', False),
    11: ('half', 'float32', 'uint8',      'VQF322B8_PRE',         'ND', True),
    12: ('half', 'float32', 'uint8',      'QF322B8_PRE',          'NZ', False),
    13: ('half', 'float32', 'uint8',      'VQF322B8_PRE',         'NZ', True),
    14: ('half', 'float32', 'float16',    'F322F16',              'ND', False),
    15: ('half', 'float32', 'float16',    'F322F16',              'NZ', False),
    16: ('half', 'float32', 'float16',    'QF322F16_PRE',         'ND', False),
    17: ('half', 'float32', 'float16',    'VQF322F16_PRE',        'ND', True),
    18: ('half', 'float32', 'float16',    'QF322F16_PRE',         'NZ', False),
    19: ('half', 'float32', 'float16',    'VQF322F16_PRE',        'NZ', True),
    20: ('half', 'float32', 'bfloat16',   'F322BF16',             'ND', False),
    21: ('half', 'float32', 'bfloat16',   'F322BF16',             'NZ', False),
    22: ('half', 'float32', 'bfloat16',   'QF322BF16_PRE',        'ND', False),
    23: ('half', 'float32', 'bfloat16',   'VQF322BF16_PRE',       'ND', True),
    24: ('half', 'float32', 'bfloat16',   'QF322BF16_PRE',        'NZ', False),
    25: ('half', 'float32', 'bfloat16',   'VQF322BF16_PRE',       'NZ', True),
    26: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE',         'ND', False),
    27: ('half', 'float32', 'fp8_e4m3fn', 'VQF322FP8_PRE',        'ND', True),
    28: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE',         'NZ', False),
    29: ('half', 'float32', 'fp8_e4m3fn', 'VQF322FP8_PRE',        'NZ', True),
    30: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE',        'ND', False),
    31: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE',       'ND', True),
    32: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE_HYBRID', 'ND', False),
    33: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE_HYBRID','ND', True),
    34: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE',        'NZ', False),
    35: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE',       'NZ', True),
    36: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE_HYBRID', 'NZ', False),
    37: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE_HYBRID','NZ', True),
    38: ('half', 'float32', 'float32',    'QF322F32_PRE',         'ND', False),
    39: ('half', 'float32', 'float32',    'VQF322F32_PRE',        'ND', True),
    40: ('half', 'float32', 'float32',    'QF322F32_PRE',         'NZ', False),
    41: ('half', 'float32', 'float32',    'VQF322F32_PRE',        'NZ', True),
    42: ('int8', 'int32',   'int32',      'NoQuant',              'ND', False),
    43: ('int8', 'int32',   'int32',      'NoQuant',              'NZ', False),
    44: ('int8', 'int32',   'int8',       'REQ8',                 'ND', False),
    45: ('int8', 'int32',   'int8',       'VREQ8',                'ND', True),
    46: ('int8', 'int32',   'int8',       'REQ8',                 'NZ', False),
    47: ('int8', 'int32',   'int8',       'VREQ8',                'NZ', True),
    48: ('int8', 'int32',   'uint8',      'REQ8',                 'ND', False),
    49: ('int8', 'int32',   'uint8',      'VREQ8',                'ND', True),
    50: ('int8', 'int32',   'uint8',      'REQ8',                 'NZ', False),
    51: ('int8', 'int32',   'uint8',      'VREQ8',                'NZ', True),
    52: ('int8', 'int32',   'float16',    'DEQF16',               'ND', False),
    53: ('int8', 'int32',   'float16',    'VDEQF16',              'ND', True),
    54: ('int8', 'int32',   'float16',    'DEQF16',               'NZ', False),
    55: ('int8', 'int32',   'float16',    'VDEQF16',              'NZ', True),
    56: ('int8', 'int32',   'bfloat16',   'QS322BF16_PRE',        'ND', False),
    57: ('int8', 'int32',   'bfloat16',   'VQS322BF16_PRE',       'ND', True),
    58: ('int8', 'int32',   'bfloat16',   'QS322BF16_PRE',        'NZ', False),
    59: ('int8', 'int32',   'bfloat16',   'VQS322BF16_PRE',       'NZ', True),
    # --- Boundary test scenarios (60-64) ---
    60: ('float', 'float32', 'float32',    'NoQuant+Saturate',     'ND', False),
    61: ('float', 'float32', 'float32',    'NoQuant+NonSaturate',  'ND', False),
    62: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE+Saturate','ND', False),
    63: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE+SaturateNan','ND', False),
    64: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE+NonSaturate','ND', False),
}

NP_DTYPE = {
    'half': np.float16, 'int8': np.int8, 'float32': np.float32, 'int32': np.int32,
    'uint8': np.uint8, 'float16': np.float16, 'bfloat16': np.uint16,
    'fp8_e4m3fn': np.uint8, 'hifloat8': np.uint8,
}
def verify_result(output, golden, scenarioNum=1):
    _, _, output_type, _, _, _ = SCENARIO_TABLE[scenarioNum]
    output_dtype = NP_DTYPE[output_type]

    output_data = np.fromfile(output, dtype=output_dtype).reshape(-1)
    golden_data = np.fromfile(golden, dtype=output_dtype).reshape(-1)

    if output_type in ('float32', 'float16', 'bfloat16'):
        relative_tol = 1e-3
        absolute_tol = 1e-6
        error_tol = 1e-3
    elif output_type in ('int8', 'uint8', 'fp8_e4m3fn', 'hifloat8'):
        relative_tol = 0
        absolute_tol = 1
        error_tol = 1e-4
    else:
        relative_tol = 1e-6
        absolute_tol = 1e-9
        error_tol = 1e-4

    different = ~np.isclose(output_data.astype(np.float64), golden_data.astype(np.float64),
                            rtol=relative_tol, atol=absolute_tol, equal_nan=True)
    diff_indexes = np.where(different)[0]
    for idx in range(min(len(diff_indexes), 100)):
        real_idx = diff_indexes[idx]
        print("data index: %06d, expected: %s, actual: %s" % (
            real_idx, golden_data[real_idx], output_data[real_idx]))

    error_ratio = float(diff_indexes.size) / golden_data.size
    return error_ratio <= error_tol


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("output", help="output file path")
    parser.add_argument("golden", help="golden file path")
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 65))
    args = parser.parse_args()
    try:
        res = verify_result(args.output, args.golden, args.scenarioNum)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
