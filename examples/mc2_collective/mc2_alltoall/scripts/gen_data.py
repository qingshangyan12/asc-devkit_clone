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

import argparse
import os
import sys

import numpy as np


DEFAULT_DATA_COUNT = 32
DEFAULT_RANK_SIZE = 2
DEFAULT_DTYPE = "fp16"

# 与main.asc中DTYPE_TABLE保持一致的数据类型集合。
# bf16与fp8无numpy原生类型，AlltoAll为纯数据搬运，按原始bit(uint16/uint8)生成和校验。
DTYPE_MAP = {
    "int8": np.int8,
    "uint8": np.uint8,
    "int16": np.int16,
    "uint16": np.uint16,
    "fp16": np.float16,
    "bf16": np.uint16,
    "int32": np.int32,
    "uint32": np.uint32,
    "fp32": np.float32,
    "int64": np.int64,
    "uint64": np.uint64,
    "fp64": np.float64,
    "fp8_e8m0": np.uint8,
    "fp8_e5m2": np.uint8,
    "fp8_e4m3": np.uint8,
}


def make_rank_input(rank, data_count, rank_size, np_dtype):
    # 输入为data_count * rank_size个元素，第rank卡第k个元素为(rank + 1) * 32 + (k mod 32)
    count = data_count * rank_size
    values = np.arange(count, dtype=np.int64) % 32 + (rank + 1) * 32
    if np.issubdtype(np_dtype, np.integer):
        # 整数类型取模，避免超出表示范围后溢出
        info = np.iinfo(np_dtype)
        values = values % (info.max - info.min + 1) + info.min
    return values.astype(np_dtype)


def make_golden(rank, data_count, rank_size, inputs, np_dtype):
    # AlltoAll: y[j * dataCount + m] = x_j[rank * dataCount + m]，
    # 即本卡输出的第j块来自rank j输入的第rank块
    blocks = [data[rank * data_count:(rank + 1) * data_count] for data in inputs]
    return np.concatenate(blocks).astype(np_dtype)


def format_value(value):
    if isinstance(value, (np.floating, float)):
        return "%g" % value
    return str(int(value))


def print_data(name, data):
    print("%s: %s" % (name, " ".join(format_value(value) for value in data.reshape(-1)[:32])))


def write_bin(path, data, np_dtype):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.astype(np_dtype).tofile(path)


def get_input_path(input_dir, rank):
    return os.path.join(input_dir, "%d" % rank, "x1.bin")


def get_cpu_output_path(output_dir, rank):
    return os.path.join(output_dir, "%d" % rank, "output_cpu_low.bin")


def get_npu_output_path(output_dir, rank):
    return os.path.join(output_dir, "%d" % rank, "output_npu.bin")


def read_bin(path, expect_size, np_dtype):
    data = np.fromfile(path, dtype=np_dtype)
    if data.size != expect_size:
        raise ValueError("%s size invalid, expect %d elements, got %d" % (path, expect_size, data.size))
    return data


def generate_data(args):
    np_dtype = DTYPE_MAP[args.dtype]
    inputs = [
        make_rank_input(rank, args.data_count, args.rank_size, np_dtype)
        for rank in range(args.rank_size)
    ]

    for rank, data in enumerate(inputs):
        write_bin(get_input_path(args.input_dir, rank), data, np_dtype)
        write_bin(
            get_cpu_output_path(args.output_dir, rank),
            make_golden(rank, args.data_count, args.rank_size, inputs, np_dtype),
            np_dtype,
        )

    print_data("card0 input", inputs[0])
    if args.rank_size > 1:
        print_data("card1 input", inputs[1])
    print_data("card0 CPU output", make_golden(0, args.data_count, args.rank_size, inputs, np_dtype))


def verify_outputs(args):
    np_dtype = DTYPE_MAP[args.dtype]
    expected_size = args.data_count * args.rank_size
    passed = True
    for rank in range(args.rank_size):
        npu_output_path = get_npu_output_path(args.output_dir, rank)
        if not os.path.isfile(npu_output_path):
            raise ValueError("[ERROR] output file not found: %s, run ./demo before verify" % npu_output_path)
        golden = read_bin(get_cpu_output_path(args.output_dir, rank), expected_size, np_dtype)
        output = read_bin(npu_output_path, expected_size, np_dtype)
        if rank < 2:
            print_data("card%d CPU output" % rank, golden)
            print_data("card%d NPU output" % rank, output)
        diff_indexes = np.where(output != golden)[0]
        for diff_index in diff_indexes[:100]:
            print(
                "card%d data index: %06d, expected: %s, actual: %s"
                % (rank, diff_index, format_value(golden[diff_index]), format_value(output[diff_index]))
            )
        error_ratio = float(diff_indexes.size) / float(golden.size)
        print("card%d error ratio: %.4f" % (rank, error_ratio))
        passed = passed and diff_indexes.size == 0
    if not passed:
        raise ValueError("[ERROR] result error")
    print("test pass!")


def parse_args():
    parser = argparse.ArgumentParser(description="Generate and verify MC2 AlltoAll data with configurable dtype.")
    parser.add_argument("--data-count", type=int, default=DEFAULT_DATA_COUNT)
    parser.add_argument("--rank-size", type=int, default=DEFAULT_RANK_SIZE)
    parser.add_argument("--dtype", default=DEFAULT_DTYPE, choices=sorted(DTYPE_MAP.keys()))
    parser.add_argument("--input-dir", default="./input")
    parser.add_argument("--output-dir", default="./output")
    parser.add_argument(
        "--verify",
        action="store_true",
        help="Compare output/cardN/output_npu.bin with output/cardN/output_cpu_low.bin.",
    )
    return parser.parse_args()


if __name__ == "__main__":
    try:
        parsed_args = parse_args()
        if parsed_args.rank_size < 1:
            raise ValueError("rank-size must be at least 1")
        if parsed_args.data_count < 1:
            raise ValueError("data-count must be at least 1")
        if parsed_args.verify:
            verify_outputs(parsed_args)
        else:
            generate_data(parsed_args)
    except Exception as err:
        print(err)
        sys.exit(1)
