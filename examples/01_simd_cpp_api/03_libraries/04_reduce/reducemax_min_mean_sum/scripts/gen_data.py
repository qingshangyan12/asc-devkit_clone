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
import sys
import argparse
import torch
import numpy as np
from ml_dtypes import bfloat16


def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=ReduceMax,2=ReduceMin,3=ReduceMean,4=ReduceSum)
    """
    dtype = np.float32
    x_gm_type = dtype
    y_gm_type = dtype

    if x_gm_type == np.int8:
        range_min = -128
        range_max = 127
    elif x_gm_type == np.uint8:
        range_min = 0
        range_max = 255
    elif x_gm_type == np.int16:
        range_min = -32768
        range_max = 32767
    elif x_gm_type == np.uint16:
        range_min = 0
        range_max = 65535
    elif x_gm_type == np.int32:
        range_min = -2147483648
        range_max = 2147483647
    elif x_gm_type == np.uint32:
        range_min = 0
        range_max = 4294967295
    elif x_gm_type == np.int64:
        range_min = -9223372036854775808
        range_max = 9223372036854775807
    elif x_gm_type == np.uint64:
        range_min = 0
        range_max = 18446744073709551615
    else:
        range_min = -10
        range_max = 10

    if scenarioNum == 1:
        np.random.seed(0)
        a_length = 136
        r_length = 32
        pattern = 1
        x_gm_shape = [a_length, -1]
        if pattern == 1:
            x_gm_shape = [r_length, -1]

        x_gm = (
            np.random.uniform(range_min, range_max, [4352])
            .reshape(x_gm_shape)
            .astype(x_gm_type)
        )

        if x_gm_type == bfloat16:
            tensor_x = torch.from_numpy(x_gm.astype(np.float32))
            tensor_y, top_indices = torch.max(tensor_x, 1)
            if pattern == 1:
                tensor_y, top_indices = torch.max(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)
        elif x_gm_type == np.uint32 or x_gm_type == np.uint16:
            tensor_x = torch.from_numpy(x_gm.astype(np.int64))
            tensor_x = tensor_x.to(torch.int64)
            tensor_y, top_indices = torch.max(tensor_x, 1)
            if pattern == 1:
                tensor_y, top_indices = torch.max(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)
        elif x_gm_type == np.uint64:
            y_gm = np.max(x_gm, axis=1)
            if pattern == 1:
                y_gm = np.max(x_gm, axis=0)
        else:
            tensor_x = torch.from_numpy(x_gm)
            tensor_y, top_indices = torch.max(tensor_x, 1)
            if pattern == 1:
                tensor_y, top_indices = torch.max(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)

        os.makedirs("input", exist_ok=True)
        x_gm.tofile("./input/input_x.bin")
        os.makedirs("output", exist_ok=True)
        y_gm.tofile("./output/golden.bin")

    elif scenarioNum == 2:
        np.random.seed(0)
        a_length = 2
        r_length = 32
        pattern = 0
        x_gm_shape = [a_length, -1]
        if pattern == 1:
            x_gm_shape = [r_length, -1]

        x_gm = (
            np.random.uniform(range_min, range_max, [64])
            .reshape(x_gm_shape)
            .astype(x_gm_type)
        )

        if x_gm_type == bfloat16:
            tensor_x = torch.from_numpy(x_gm.astype(np.float32))
            tensor_y, top_indices = torch.max(tensor_x, 1)
            if pattern == 1:
                tensor_y, top_indices = torch.min(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)
        elif x_gm_type == np.uint32 or x_gm_type == np.uint16:
            tensor_x = torch.from_numpy(x_gm.astype(np.int64))
            tensor_x = tensor_x.to(torch.int64)
            tensor_y, top_indices = torch.min(tensor_x, 1)
            if pattern == 1:
                tensor_y, top_indices = torch.min(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)
        elif x_gm_type == np.uint64:
            y_gm = np.min(x_gm, axis=1)
            if pattern == 1:
                y_gm = np.min(x_gm, axis=0)
        else:
            tensor_x = torch.from_numpy(x_gm)
            tensor_y, top_indices = torch.min(tensor_x, 1)
            if pattern == 1:
                tensor_y, top_indices = torch.min(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)

        os.makedirs("input", exist_ok=True)
        x_gm.tofile("./input/input_x.bin")
        os.makedirs("output", exist_ok=True)
        y_gm.tofile("./output/golden.bin")

    elif scenarioNum == 3:
        np.random.seed(0)
        a_length = 136
        r_length = 32
        pattern = 1  # RA
        total_size = 4352
        x_gm_shape = [a_length, -1]
        if pattern == 1:
            x_gm_shape = [r_length, -1]

        x_gm = np.random.uniform(range_min, range_max, [total_size]).reshape(x_gm_shape).astype(x_gm_type)

        # ReduceMean场景
        if x_gm_type == bfloat16:
            tensor_x = torch.from_numpy(x_gm.astype(np.float32))
            tensor_y = torch.mean(tensor_x, 1)
            if pattern == 1:
                tensor_y = torch.mean(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)
        elif x_gm_type == np.uint32 or x_gm_type == np.uint16:
            tensor_x = torch.from_numpy(x_gm.astype(np.int64))
            tensor_x = tensor_x.to(torch.int64)
            tensor_y = torch.mean(tensor_x.float(), 1)
            if pattern == 1:
                tensor_y = torch.mean(tensor_x.float(), 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)
        elif x_gm_type == np.uint64:
            y_gm = np.mean(x_gm, axis=1)
            if pattern == 1:
                y_gm = np.mean(x_gm, axis=0)
        else:
            tensor_x = torch.from_numpy(x_gm)
            tensor_y = torch.mean(tensor_x, 1)
            if pattern == 1:
                tensor_y = torch.mean(tensor_x, 0)
            y_gm = tensor_y.cpu().numpy().astype(y_gm_type)

        os.makedirs("input", exist_ok=True)
        x_gm.tofile("./input/input_x.bin")
        os.makedirs("output", exist_ok=True)
        y_gm.tofile("./output/golden.bin")

    elif scenarioNum == 4:
        np.random.seed(0)
        input_type = dtype
        min_val, max_val = -10, 10
        input_shape = [2, 32]
        input_x = np.random.uniform(min_val, max_val, input_shape).astype(input_type)
        reduce_axis = 1
        golden = np.sum(input_x, axis=reduce_axis)
        os.makedirs("input", exist_ok=True)
        input_x.tofile("./input/input_x.bin")
        os.makedirs("output", exist_ok=True)
        golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 5))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
