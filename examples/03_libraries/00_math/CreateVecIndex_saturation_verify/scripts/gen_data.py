#!/usr/bin python3
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

import os
import argparse
import numpy as np


DATA_TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}

TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}


def compute_createvecindex_golden(first_value, count, data_type):
    golden = np.arange(count, dtype=data_type)
    return golden


def gen_createvecindex_data(input_type, input_size):
    data_type = DATA_TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    first_value = 0
    
    golden = compute_createvecindex_golden(first_value, input_size, data_type)
    
    os.makedirs("../output", exist_ok=True)
    golden.tofile("../output/golden.bin")
    
    print(f"\n==================================================================================")
    print(f"API: CreateVecIndex | Data Type: {type_name} | Gen Golden")
    print(f"{'=================================================================================='}")
    print(f"First Value: {first_value}, Count: {input_size}")
    print(f"First 10 values: {golden[:10]}")
    print(f"{'=================================================================================='}\n")
    
    print("Data generation completed successfully!")


def main():
    parser = argparse.ArgumentParser(description="Generate golden data for CreateVecIndex saturation verify.")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input type: 1=float16, 2=float32, 3=int16, 4=int32")
    args = parser.parse_args()

    input_type = args.inputType
    type_name = str(TYPE_MAP[input_type].__name__)
    print(f"\nGenerating test data for: API=CreateVecIndex, Type={type_name}, Size={args.inputSize}")

    gen_createvecindex_data(input_type, args.inputSize)


if __name__ == "__main__":
    main()