#!/usr/bin/python3
# coding=utf-8

import os
import argparse
import numpy as np
np.random.seed(9)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_dtype', type=str, default='half', choices=['half', 'int8'])
    parser.add_argument('--output_dtype', type=str, default='int8', choices=['int8', 'half'])
    parser.add_argument('--pre_quant_mode', type=int, default=0, choices=[0,1,2], help='0:none, 1:scalar, 2:vector')
    parser.add_argument('--pre_relu_mode', action='store_true', help='enable norm relu')
    args = parser.parse_args()

    # 根据参数选择生成逻辑
    if args.pre_quant_mode == 0:
        # 无量化，直接输出（仅当输入输出类型匹配时有效，这里不实现）
        print("No quantization mode not supported in this sample, use pre_quant_mode=1 or 2.")
        return
    elif args.pre_quant_mode != 0 and args.output_dtype == 'int8':
        # 量化（浮点 -> int8）
        from gen_data_quant import gen_quant
        gen_quant(args.input_dtype, args.output_dtype, args.pre_quant_mode, args.pre_relu_mode)
    elif args.pre_quant_mode != 0 and args.output_dtype == 'half':
        # 反量化（int8 -> 浮点）
        from gen_data_dequant import gen_dequant
        gen_dequant(args.input_dtype, args.output_dtype, args.pre_quant_mode, args.pre_relu_mode)
    else:
        raise ValueError("Unsupported combination")

if __name__ == "__main__":
    main()