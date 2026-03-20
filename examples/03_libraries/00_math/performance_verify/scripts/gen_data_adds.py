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

import numpy as np
from gen_data_utils import DATA_TYPE_MAP, generate_random_data, save_bin


def compute_adds_golden(a, scalar):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.add(a, scalar)
    return result.astype(a.dtype)


def gen_adds_data(input_type, input_size):
    data_type = DATA_TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    input_x = generate_random_data(data_type, input_size)
    if np.issubdtype(data_type, np.floating):
        scalar = np.array(0.5, dtype=data_type)
    else:
        scalar = np.array(5, dtype=data_type)
    input_y = np.full(input_size, scalar, dtype=data_type)
    golden = compute_adds_golden(input_x, scalar)
    
    save_bin(input_x, f"./input/input_x.bin")
    save_bin(input_y, f"./input/input_y.bin")
    save_bin(golden, f"./output/golden.bin")
    
    print(f"[DataGen] input_x shape: {input_x.shape}, dtype: {type_name}")
    print(f"[DataGen] input_y (scalar={scalar}) shape: {input_y.shape}, dtype: {type_name}")
    print(f"[DataGen] Golden data range: [{golden.min()}, {golden.max()}]")
