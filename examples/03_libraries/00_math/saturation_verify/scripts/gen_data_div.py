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
from gen_data_utils import DATA_TYPE_MAP, cartesian_product_inputs, save_bin, get_special_names, print_binary_api_table


def compute_div_golden(a, b):
    with np.errstate(divide='ignore', over='ignore', under='ignore', invalid='ignore'):
        result = np.divide(a, b)
    return result.astype(a.dtype)


def gen_div_data(input_type, input_size):
    data_type = DATA_TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    inputs_a, inputs_b = cartesian_product_inputs(data_type)
    golden = compute_div_golden(inputs_a, inputs_b)
    
    save_bin(inputs_a, f"./input/input_x.bin")
    save_bin(inputs_b, f"./input/input_y.bin")
    save_bin(golden, f"./output/golden.bin")
    
    special_names = get_special_names(data_type)
    print_binary_api_table("div", type_name, special_names, inputs_a, inputs_b, golden)
