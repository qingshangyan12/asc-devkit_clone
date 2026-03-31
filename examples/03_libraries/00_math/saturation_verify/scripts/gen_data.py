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

from gen_data_utils import get_dtype_limits, construct_special_values, cartesian_product_inputs, save_bin
from gen_data_utils import print_unary_api_table, print_binary_api_table
from gen_data_exp import compute_exp_golden, gen_exp_data
from gen_data_mul import compute_mul_golden, gen_mul_data
from gen_data_adds import compute_adds_golden, gen_adds_data
from gen_data_leakyrelu import compute_leakyrelu_golden, gen_leakyrelu_data
from gen_data_ln import compute_ln_golden, gen_ln_data
from gen_data_abs import compute_abs_golden, gen_abs_data
from gen_data_reciprocal import compute_reciprocal_golden, gen_reciprocal_data
from gen_data_sqrt import compute_sqrt_golden, gen_sqrt_data
from gen_data_rsqrt import compute_rsqrt_golden, gen_rsqrt_data
from gen_data_relu import compute_relu_golden, gen_relu_data
from gen_data_add import compute_add_golden, gen_add_data
from gen_data_sub import compute_sub_golden, gen_sub_data
from gen_data_div import compute_div_golden, gen_div_data
from gen_data_max import compute_max_golden, gen_max_data
from gen_data_min import compute_min_golden, gen_min_data
from gen_data_muls import compute_muls_golden, gen_muls_data
from gen_data_maxs import compute_maxs_golden, gen_maxs_data
from gen_data_mins import compute_mins_golden, gen_mins_data

API_ID_EXP = 1
API_ID_MUL = 2
API_ID_ADDS = 3
API_ID_LEAKYRELU = 4
API_ID_LN = 5
API_ID_ABS = 6
API_ID_RECIPROCAL = 7
API_ID_SQRT = 8
API_ID_RSQRT = 9
API_ID_RELU = 10
API_ID_ADD = 11
API_ID_SUB = 12
API_ID_DIV = 13
API_ID_MAX = 14
API_ID_MIN = 15
API_ID_MULS = 16
API_ID_MAXS = 17
API_ID_MINS = 18

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
    4: "leakyrelu",
    5: "ln",
    6: "abs",
    7: "reciprocal",
    8: "sqrt",
    9: "rsqrt",
    10: "relu",
    11: "add",
    12: "sub",
    13: "div",
    14: "max",
    15: "min",
    16: "muls",
    17: "maxs",
    18: "mins"
}


def main():
    parser = argparse.ArgumentParser(description="Generate golden data for saturation verify.")
    parser.add_argument('-inputSize', type=int, required=True,
                        help="Total number of elements in the tensor")
    parser.add_argument('-apiId', type=int, default=1, choices=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18],
                        help="API ID: 1=exp, 2=mul, 3=adds, 4=leakyrelu, 5=ln, 6=abs, 7=reciprocal, 8=sqrt, 9=rsqrt, 10=relu, 11=add, 12=sub, 13=div, 14=max, 15=min, 16=muls, 17=maxs, 18=mins")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                        help="Input type: 1=float16, 2=float32, 3=int16, 4=int32")
    parser.add_argument('-apiType', type=int, default=1, choices=[1, 2, 3],
                        help="API type: 1=Unary, 2=Binary, 3=BinaryScalar")
    args = parser.parse_args()

    api_name = API_NAME_MAP.get(args.apiId, "unknown")
    input_type = args.inputType
    api_type = args.apiType
    type_name = str(TYPE_MAP[input_type].__name__)
    print(f"\nGenerating test data for: API={api_name}, API_Type={api_type}, Type={type_name}, Size={args.inputSize}")

    if args.apiId == API_ID_EXP:
        gen_exp_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MUL:
        gen_mul_data(input_type, args.inputSize)
    elif args.apiId == API_ID_ADDS:
        gen_adds_data(input_type, args.inputSize)
    elif args.apiId == API_ID_LEAKYRELU:
        gen_leakyrelu_data(input_type, args.inputSize)
    elif args.apiId == API_ID_LN:
        gen_ln_data(input_type, args.inputSize)
    elif args.apiId == API_ID_ABS:
        gen_abs_data(input_type, args.inputSize)
    elif args.apiId == API_ID_RECIPROCAL:
        gen_reciprocal_data(input_type, args.inputSize)
    elif args.apiId == API_ID_SQRT:
        gen_sqrt_data(input_type, args.inputSize)
    elif args.apiId == API_ID_RSQRT:
        gen_rsqrt_data(input_type, args.inputSize)
    elif args.apiId == API_ID_RELU:
        gen_relu_data(input_type, args.inputSize)
    elif args.apiId == API_ID_ADD:
        gen_add_data(input_type, args.inputSize)
    elif args.apiId == API_ID_SUB:
        gen_sub_data(input_type, args.inputSize)
    elif args.apiId == API_ID_DIV:
        gen_div_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MAX:
        gen_max_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MIN:
        gen_min_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MULS:
        gen_muls_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MAXS:
        gen_maxs_data(input_type, args.inputSize)
    elif args.apiId == API_ID_MINS:
        gen_mins_data(input_type, args.inputSize)
    else:
        raise ValueError(f"Unsupported API ID: {args.apiId}")

    print("\nData generation completed successfully!")


if __name__ == "__main__":
    main()
