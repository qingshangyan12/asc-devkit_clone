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
import numpy as np


DATA_TYPE_MAP = {
    1: np.float16,
    2: np.float32,
    3: np.int16,
    4: np.int32,
}


def get_dtype_limits(data_type):
    if np.issubdtype(data_type, np.floating):
        finfo = np.finfo(data_type)
        return finfo.min, finfo.max
    elif np.issubdtype(data_type, np.integer):
        iinfo = np.iinfo(data_type)
        return iinfo.min, iinfo.max
    else:
        raise ValueError(f"Unsupported data type: {data_type}")


def generate_random_data(data_type, size):
    min_val, max_val = get_dtype_limits(data_type)
    if np.issubdtype(data_type, np.floating):
        safe_min = max(min_val, -10000.0)
        safe_max = min(max_val, 10000.0)
        return np.random.uniform(safe_min, safe_max, size).astype(data_type)
    else:
        safe_min = max(min_val, -10000)
        safe_max = min(max_val, 10000)
        return np.random.uniform(safe_min, safe_max, size).astype(data_type)


def save_bin(data, path):
    os.makedirs(os.path.dirname(path) if os.path.dirname(path) else "./", exist_ok=True)
    data.tofile(path)


def print_performance_header(api_name, type_name, size):
    print(f"\n{'='*80}")
    print(f"Performance Test | API: {api_name} | DataType: {type_name} | Size: {size}")
    print(f"{'='*80}")
