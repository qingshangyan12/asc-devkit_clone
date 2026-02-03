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
import argparse
import copy
import struct
import numpy as np


def extract_quant_params(quant_gm):
    """
    从uint64类型的quant_gm中提取M1、offset、sign参数
    param:
        quant_gm:uint64类型的整数
    return:
        quant_alpha:自定义格式(1,8,10)的浮点数
        offset:9位整数
        sign:1位布尔值(0或1)
    """
    quant_gm = int(quant_gm)
    quant_alpha_bits = (quant_gm >> 13) & 0xFFFFF  # 提取M1的20位[31:13]，0xFFFFF是20位掩码
    mode_control_bit = (quant_gm >> 36) & 0x1  # 提取mode_ctrl bit的一位[36]，0x1是1位掩码
    offset = (quant_gm >> 37) & 0x1FF # 提取offset的9位[45:37]，0x1FF是9位掩码
    sign = (quant_gm >> 46) & 0x1  # 提取sign的一位[46]，0x1是1位掩码
    n = (quant_gm >> 32) & 0xF
    # 解析M1为(1,8,10)格式的浮点数
    sign_bit = (quant_alpha_bits >> 18) & 0x1
    exponent = (quant_alpha_bits >> 10) & 0xFF
    mantissa = quant_alpha_bits & 0x3FF
    exponent_bias = 127 # 假设指数偏倚量为127，与float32一致
    quant_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return quant_alpha, offset, sign, n, mode_control_bit

if __name__ == "__main__":
    a, b, c, d, e = extract_quant_params(1.0)
    print(a, b, c, d, e)
