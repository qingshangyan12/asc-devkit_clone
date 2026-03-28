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

import os
import argparse
import numpy as np
np.random.seed(9)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_dtype', type=str, default='half', choices=['half', 'int8'])
    parser.add_argument('--output_dtype', type=str, default='float32', choices=['float32', 'bfloat16', 'float16', 'int8'])
    parser.add_argument('--output_format', type=int, default=1, choices=[1,2])
    parser.add_argument('--channel_split', action='store_true', help='enable channel split (only for float32 NZ output)')
    parser.add_argument('--pre_quant_mode', type=int, default=0, choices=[0,1,2], help='0:none, 1:scalar, 2:vector')
    parser.add_argument('--pre_relu_mode', action='store_true', help='enable norm relu')
    args = parser.parse_args()

    # 根据参数选择生成逻辑
    # 基础矩阵乘真值生成（无量化）
    from gen_data_base import gen_base
    if args.pre_quant_mode == 0 and args.output_dtype in ['float32', 'bfloat16', 'float16']:
        gen_base(args.input_dtype, args.output_dtype, args.output_format, args.channel_split)
    elif args.pre_quant_mode != 0 and args.output_dtype == 'int8':
        # 量化（浮点 -> int8）
        from gen_data_quant import gen_quant
        gen_quant(args.input_dtype, args.output_dtype, args.output_format, args.pre_quant_mode, args.pre_relu_mode)
    elif args.pre_quant_mode != 0 and args.output_dtype in ['float32', 'bfloat16', 'float16']:
        # 反量化（int8 -> 浮点）
        from gen_data_dequant import gen_dequant
        gen_dequant(args.input_dtype, args.output_dtype, args.output_format, args.pre_quant_mode, args.pre_relu_mode)
    else:
        raise ValueError("Unsupported combination")

if __name__ == "__main__":
    main()