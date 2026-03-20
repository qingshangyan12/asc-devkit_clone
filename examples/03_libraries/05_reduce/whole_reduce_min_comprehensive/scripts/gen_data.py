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
import numpy as np
import argparse


def get_range_by_dtype(input_type):
    try:
        if input_type == np.float16 or input_type == np.float32 or input_type == np.float64:
            return np.finfo(input_type).min, np.finfo(input_type).max
        else:
            return np.iinfo(input_type).min, np.iinfo(input_type).max
    except ValueError:
        print(f"Unsupported data type:{input_type}")

# 返回结果中索引index数据是按照dst的数据类型进行存储的，比如dst使用half类型时，
# index按照half类型进行存储，如果按照half格式进行读取，index的值是不对的，因此index的读取需要使用reinterpret_cast方法转换到整数类型。
# 若输入数据类型是half，需要使用reinterpret_cast<uint16_t*>，若输入是float，需要使用reinterpret_cast<uint32_t*>
def gen_golden_data_simple(scenarioNum, size):
    input_type = None
    min_index_type = None
    if scenarioNum in (1, 2):
        input_type = np.dtype("float16")
        min_index_type = np.uint16 #与half对应
    else:
        input_type = np.dtype("float32")
        min_index_type = np.uint32 #与half对应
    output_type = input_type
    
    # 一次迭代能处理256B数据
    one_repeat_items = (256 // input_type.itemsize) 
    repeat = size // one_repeat_items
    tail = size % one_repeat_items
    min_val, max_val = get_range_by_dtype(input_type)
    input_shape = [size]
    output_shape = [2 * repeat] if tail == 0 else [2 * (repeat + 1)]
    input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
    golden = np.zeros(output_shape).astype(output_type)
    for i in range(repeat):
        #获取最小值
        golden[0 + 2 * i] = np.min(input_x[i * one_repeat_items:(i + 1) * one_repeat_items])
        #获取最小值的下标，并且强制转换为half类型
        min_index = np.argmin(input_x[i * one_repeat_items:(i + 1) * one_repeat_items])
        #min_index的数据类型默认是int64_t，view方法只支持位宽不变
        min_index = min_index_type(min_index)
        if scenarioNum in (1, 2):
            golden[1 + 2 * i] = np.float16(min_index.view(np.float16))
        else:
            golden[1 + 2 * i] = np.float32(min_index.view(np.float32))

    if tail > 0:
        golden[repeat] = np.min(input_x[repeat * one_repeat_items:repeat * one_repeat_items + tail])
        min_index = np.argmin(input_x[repeat * one_repeat_items:repeat * one_repeat_items + tail])
        min_index = min_index_type(min_index)
        if scenarioNum in (1, 2):
            golden[1 + repeat] = np.float16(min_index.view(np.float16))
        else:
            golden[1 + repeat] = np.float32(min_index.view(np.float32))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 5))
    parser.add_argument('-size', type=int)
    args = parser.parse_args()
    gen_golden_data_simple(args.scenarioNum, args.size)
