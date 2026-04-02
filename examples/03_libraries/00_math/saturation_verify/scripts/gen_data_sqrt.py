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

import numpy as np
from gen_data_utils import DATA_TYPE_MAP, construct_multi_block_data, save_bin, get_special_names, print_unary_api_table


def compute_sqrt_golden(input_array):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.sqrt(input_array)
    return result.astype(input_array.dtype)


def gen_sqrt_data(input_type, input_size):
    data_type = DATA_TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    inputs = construct_multi_block_data(data_type)
    inputs = construct_multi_block_data(data_type)
    golden = compute_sqrt_golden(inputs)
    
    save_bin(inputs, f"./input/input_x.bin")
    save_bin(golden, f"./output/golden.bin")
    
    special_names = get_special_names(data_type)
    print_unary_api_table("sqrt", type_name, special_names, inputs, golden)
