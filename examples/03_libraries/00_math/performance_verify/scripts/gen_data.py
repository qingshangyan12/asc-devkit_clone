#!/usr/bin/env python3
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

from gen_data_utils import get_dtype_limits, generate_random_data, save_bin, print_performance_header
from gen_data_exp import compute_exp_golden, gen_exp_data
from gen_data_mul import compute_mul_golden, gen_mul_data
from gen_data_adds import compute_adds_golden, gen_adds_data
from gen_data_leakyrelu import compute_leakyrelu_golden, gen_leakyrelu_data

API_ID_EXP = 1
API_ID_MUL = 2
API_ID_ADDS = 3
API_ID_LEAKYRELU = 4

TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "adds",
    4: "leakyrelu"
}


def main():
    parser = argparse.ArgumentParser(description="Generate performance test data for Ascend C APIs.")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    parser.add_argument('-apiId', type=int, default=1, choices=[1, 2, 3, 4],
                        help="API ID: 1=exp, 2=mul, 3=adds, 4=leakyrelu")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-apiType', type=int, default=1, choices=[1, 2, 3],
                        help="API type: 1=Unary, 2=Binary, 3=BinaryScalar")
    args = parser.parse_args()

    api_name = API_NAME_MAP.get(args.apiId, "unknown")
    input_type = args.inputType
    api_type = args.apiType
    type_name = str(TYPE_MAP[input_type].__name__)
    
    print_performance_header(api_name, type_name, args.inputSize)
    print(f"Generating test data for: API={api_name}, API_Type={api_type}, Type={type_name}, Size={args.inputSize}")

    if args.apiId == API_ID_EXP:
        gen_exp_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MUL:
        gen_mul_data(input_type, args.inputSize)
    elif args.apiId == API_ID_ADDS:
        gen_adds_data(input_type, args.inputSize)
    elif args.apiId == API_ID_LEAKYRELU:
        gen_leakyrelu_data(input_type, args.inputSize)
    else:
        raise ValueError(f"Unsupported API ID: {args.apiId}")

    print("\nData generation completed successfully!")


if __name__ == "__main__":
    main()
