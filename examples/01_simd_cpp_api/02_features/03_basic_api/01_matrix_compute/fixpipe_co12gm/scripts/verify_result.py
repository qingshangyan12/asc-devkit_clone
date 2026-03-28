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

import sys
import numpy as np
import tensorflow as tf
bfloat16 = tf.bfloat16.as_numpy_dtype

# 根据输出文件扩展名或约定确定数据类型？这里简单地从文件名推断或硬编码。
# 但为了灵活性，我们可以在运行时根据输出文件的二进制内容判断？但更简单的是，我们假定用户知道输出类型。
# 这里我们添加一个参数来指定数据类型，但为了简化，我们可以尝试根据文件大小推断，但不可靠。
# 实际上，验证脚本通常由用户根据实际配置调用，我们可以让用户传入数据类型作为参数。
# 但为了与之前样例兼容，我们可以在脚本中尝试自动判断。

def detect_dtype_from_file(file_path, size_bytes):
    # 根据文件大小和已知矩阵尺寸推断
    # 已知 M=128, N=256, 元素总数 = 32768
    # 若文件大小 = 32768 * 1B -> int8
    # = 32768 * 2B -> float16 / bfloat16
    # = 32768 * 4B -> float32
    # 但 bfloat16 和 float16 都是2字节，无法区分，所以需要额外信息。
    # 为简化，我们让调用者通过环境变量或参数传递 dtype。
    pass

def verify_result(output_file, golden_file, output_dtype='float32'):
    if output_dtype == 'bfloat16':
        dtype = bfloat16
    elif output_dtype == 'float16':
        dtype = np.float16
    elif output_dtype == 'float32':
        dtype = np.float32
    elif output_dtype == 'int8':
        dtype = np.int8
    else:
        raise ValueError("Unsupported output dtype")

    output = np.fromfile(output_file, dtype=dtype).reshape(-1)
    golden = np.fromfile(golden_file, dtype=dtype).reshape(-1)

    # 转换为 float32 进行比较
    output = output.astype(np.float32)
    golden = golden.astype(np.float32)

    # 根据输出类型调整误差容限
    if dtype == np.int8:
        rtol = 1e-6
        atol = 1e-9
        error_tol = 1e-4
    else:
        rtol = 1e-3
        atol = 1e-3
        error_tol = 1e-3

    different_element_results = np.isclose(output, golden, rtol=rtol, atol=atol, equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]
    for idx in range(len(different_element_indexes)):
        real_idx = different_element_indexes[idx]
        print("data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
              (real_idx, golden[real_idx], output[real_idx],
               abs(output[real_idx] - golden[real_idx]) / golden[real_idx]))
        if idx == 100:
            break
    error_ratio = float(different_element_indexes.size) / golden.size
    return error_ratio <= error_tol

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Usage: verify_result.py output.bin golden.bin output_dtype [float32|bfloat16|float16|int8]")
        sys.exit(1)
    output_file = sys.argv[1]
    golden_file = sys.argv[2]
    output_dtype = sys.argv[3] if len(sys.argv) > 3 else 'float32'
    res = verify_result(output_file, golden_file, output_dtype)
    if not res:
        raise ValueError("[ERROR] result error")
    else:
        print("test pass!")