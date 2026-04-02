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
import tensorflow as tf
bfloat16 = tf.bfloat16.as_numpy_dtype

DTYPE_CONFIG = {
    'cast_fp16': {'dtype': np.float16, 'rtol': 1e-3, 'atol': 1e-3, 'error_tol': 1e-3},
    'cast_bf16': {'dtype': bfloat16, 'rtol': 1e-3, 'atol': 1e-3, 'error_tol': 1e-3},
    'int8': {'dtype': np.int8, 'rtol': 1e-6, 'atol': 1e-9, 'error_tol': 1e-4},
    'uint8': {'dtype': np.uint8, 'rtol': 1e-6, 'atol': 1e-9, 'error_tol': 1e-4},
    'fp16': {'dtype': np.float16, 'rtol': 1e-6, 'atol': 1e-9, 'error_tol': 1e-4},
    'fp32': {'dtype': np.float32, 'rtol': 1e-6, 'atol': 1e-9, 'error_tol': 1e-4},
}

def verify_result(output, golden, quant_mode):
    if quant_mode == 'channelsplit':
        config = DTYPE_CONFIG['fp32']
    elif quant_mode == 'f322bf16':
        config = DTYPE_CONFIG['cast_bf16']
    elif quant_mode == 'f322f16':
        config = DTYPE_CONFIG['cast_fp16']
    elif quant_mode == 'f322s8':
        config = DTYPE_CONFIG['int8']
    elif quant_mode == 's322f16':
        config = DTYPE_CONFIG['fp16']
    elif quant_mode == 's322s8':
        config = DTYPE_CONFIG['int8']
    else:
        raise ValueError(f"Unknown quant_mode: {quant_mode}")
    
    dtype = config['dtype']
    relative_tol = config['rtol']
    absolute_tol = config['atol']
    error_tol = config['error_tol']
    
    output = np.fromfile(output, dtype=dtype).reshape(-1)
    golden = np.fromfile(golden, dtype=dtype).reshape(-1)
    if quant_mode == 'f322bf16':
        output = output.astype(np.float32)
        golden = golden.astype(np.float32)
    
    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=relative_tol,
                                           atol=absolute_tol,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
            (real_index, golden_data, output_data,
             abs(output_data - golden_data) / golden_data))
        if index == 100:
            break
    error_ratio = float(different_element_indexes.size) / golden.size
    return error_ratio <= error_tol

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('output', type=str, help='output file path')
    parser.add_argument('golden', type=str, help='golden file path')
    parser.add_argument('-quant_mode', type=str, default='s322s8', 
                        choices=['channelsplit', 'f322bf16', 'f322f16', 'f322s8', 's322f16', 's322s8'],
                        help='quantization mode')
    args = parser.parse_args()
    
    try:
        res = verify_result(args.output, args.golden, args.quant_mode)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
