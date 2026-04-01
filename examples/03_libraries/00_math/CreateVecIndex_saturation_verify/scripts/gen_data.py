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

FIRST_VALUE_MAP = {
    1: "min",
    2: "max",
    3: "+0.0",
    4: "-0.0",
    5: "+INF",
    6: "-INF",
    7: "NaN",
}


def get_first_value(first_value_id, data_type):
    if np.issubdtype(data_type, np.floating):
        finfo = np.finfo(data_type)
        if first_value_id == 1:
            return np.array(finfo.min, dtype=data_type)
        elif first_value_id == 2:
            return np.array(finfo.max, dtype=data_type)
        elif first_value_id == 3:
            return np.array(+0.0, dtype=data_type)
        elif first_value_id == 4:
            return np.array(-0.0, dtype=data_type)
        elif first_value_id == 5:
            return np.array(np.inf, dtype=data_type)
        elif first_value_id == 6:
            return np.array(-np.inf, dtype=data_type)
        elif first_value_id == 7:
            return np.array(np.nan, dtype=data_type)
    elif np.issubdtype(data_type, np.integer):
        iinfo = np.iinfo(data_type)
        if first_value_id == 1:
            return np.array(iinfo.min, dtype=data_type)
        elif first_value_id == 2:
            return np.array(iinfo.max, dtype=data_type)
        elif first_value_id == 3:
            return np.array(0, dtype=data_type)
        elif first_value_id == 4:
            return np.array(0, dtype=data_type)
    return np.array(0, dtype=data_type)


def compute_createvecindex_golden(first_value, count, data_type):
    indices = np.arange(count, dtype=data_type)
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        golden = first_value + indices
    golden = golden.astype(data_type)
    return golden


def gen_createvecindex_data(input_type, input_size, first_value_id):
    data_type = DATA_TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    first_value = get_first_value(first_value_id, data_type)
    first_value_name = FIRST_VALUE_MAP.get(first_value_id, "unknown")
    
    golden = compute_createvecindex_golden(first_value, input_size, data_type)
    
    os.makedirs("../input", exist_ok=True)
    os.makedirs("../output", exist_ok=True)
    
    first_value_arr = np.array([first_value], dtype=data_type)
    first_value_arr.tofile("../input/input_x.bin")
    golden.tofile("../output/golden.bin")
    
    print(f"\n==================================================================================")
    print(f"API: CreateVecIndex | Data Type: {type_name} | First Value: {first_value_name}")
    print(f"{'=================================================================================='}")
    print(f"First Value: {first_value}, Count: {input_size}")
    print(f"golden values: {golden}")
    print(f"{'=================================================================================='}\n")
    
    print("Data generation completed successfully!")


def main():
    parser = argparse.ArgumentParser(description="Generate golden data for CreateVecIndex saturation verify.")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-firstValueId', type=int, default=1, choices=[1, 2, 3, 4, 5, 6, 7],
                        help="First value ID: 1=min, 2=max, 3=+0.0, 4=-0.0, 5=+INF, 6=-INF, 7=NaN")
    args = parser.parse_args()

    input_type = args.inputType
    type_name = str(TYPE_MAP[input_type].__name__)
    first_value_name = FIRST_VALUE_MAP.get(args.firstValueId, "unknown")
    print(f"\nGenerating test data for: API=CreateVecIndex, Type={type_name}, FirstValue={first_value_name}, Size={args.inputSize}")

    gen_createvecindex_data(input_type, args.inputSize, args.firstValueId)


if __name__ == "__main__":
    main()