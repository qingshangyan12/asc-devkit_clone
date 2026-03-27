#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import numpy as np
import tensorflow as tf

bfloat16 = tf.bfloat16.as_numpy_dtype

DTYPE_MAP = {
    "int8_t": (np.int8, 1),
    "int16_t": (np.int16, 2),
    "half": (np.float16, 2),
    "bfloat16_t": (bfloat16, 2),
    "int32_t": (np.int32, 4),
    "float": (np.float32, 4)
}

NUM_CORES = 3
BLOCK_LENGTH = 256

def gen_data_for_dtype(dtype_name):
    np_type, _ = DTYPE_MAP[dtype_name]
    input_shape = [BLOCK_LENGTH]
    
    if dtype_name == "int8_t":
        input_data = np.ones(input_shape, dtype=np.int8)
    elif dtype_name == "int16_t":
        input_data = np.ones(input_shape, dtype=np.int16)
    elif dtype_name == "half":
        input_data = np.ones(input_shape, dtype=np.float16)
    elif dtype_name == "bfloat16_t":
        input_data = np.ones(input_shape, dtype=bfloat16)
    elif dtype_name == "int32_t":
        input_data = np.ones(input_shape, dtype=np.int32)
    elif dtype_name == "float":
        input_data = np.ones(input_shape, dtype=np.float32)
    else:
        raise ValueError(f"Unsupported dtype: {dtype_name}")
    
    golden = NUM_CORES * input_data
    
    return input_data, golden


def gen_all_data():
    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    
    for dtype_name in DTYPE_MAP.keys():
        print(f"Generating data for {dtype_name}...")
        input_data, golden = gen_data_for_dtype(dtype_name)
        
        input_path = f"./input/input_x_{dtype_name}.bin"
        golden_path = f"./output/golden_{dtype_name}.bin"
        
        input_data.tofile(input_path)
        golden.tofile(golden_path)
        
        print(f"  Input: {input_path} ({input_data.nbytes} bytes)")
        print(f"  Golden: {golden_path} ({golden.nbytes} bytes)")


if __name__ == "__main__":
    gen_all_data()
    print("All data generated successfully!")