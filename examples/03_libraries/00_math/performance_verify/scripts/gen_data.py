#!/usr/bin/env python3
# coding=utf-8
"""
Ascend C API 测试数据生成入口模块
功能：根据 API 类型和参数，调用对应的数据生成函数
"""

import argparse
from gen_data_utils import (
    API_ID_EXP, API_ID_MUL, API_ID_ADDS, API_ID_LEAKYRELU,
    API_NAME_MAP, DATA_TYPE_NP_MAP, print_performance_header
)
from gen_data_exp import gen_exp_data
from gen_data_mul import gen_mul_data
from gen_data_adds import gen_adds_data
from gen_data_leakyrelu import gen_leakyrelu_data


def main():
    parser = argparse.ArgumentParser(description="Generate test data for Ascend C APIs")
    parser.add_argument('-inputSize', type=int, required=True, help="Number of elements")
    parser.add_argument('-apiId', type=int, default=1, choices=[1, 2, 3, 4],
                       help="API ID: 1=exp, 2=mul, 3=adds, 4=leakyrelu")
    parser.add_argument('-inputType', type=int, default=1, choices=[1, 2, 3, 4],
                       help="Data type: 1=half, 2=float, 3=int16, 4=int32")
    parser.add_argument('-apiType', type=int, default=1, choices=[1, 2, 3],
                       help="API type: 1=Unary, 2=Binary, 3=BinaryScalar")
    args = parser.parse_args()

    api_name = API_NAME_MAP.get(args.apiId, "unknown")
    type_name = str(DATA_TYPE_NP_MAP[args.inputType].__name__)
    
    print_performance_header(api_name, type_name, args.inputSize)

    # 根据 API 类型调用对应的数据生成函数
    if args.apiId == API_ID_EXP:
        gen_exp_data(args.inputType, args.inputSize)
    elif args.apiId == API_ID_MUL:
        gen_mul_data(args.inputType, args.inputSize)
    elif args.apiId == API_ID_ADDS:
        gen_adds_data(args.inputType, args.inputSize)
    elif args.apiId == API_ID_LEAKYRELU:
        gen_leakyrelu_data(args.inputType, args.inputSize)
    else:
        raise ValueError(f"Unsupported API ID: {args.apiId}")


if __name__ == "__main__":
    main()
