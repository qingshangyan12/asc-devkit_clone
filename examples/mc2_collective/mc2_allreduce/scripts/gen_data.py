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


DEFAULT_SEND_COUNT = 128
DEFAULT_RANK_SIZE = 2


def make_rank_input(rank, send_count):
    # 与main.asc中MakeFallbackInputValue的兜底数据保持一致
    base = rank + 1
    return (base + (np.arange(send_count, dtype=np.int64) % 32)).astype(np.float16)


def make_golden(inputs):
    # AllReduce: y[i] = sum_rank(x_rank[i])，FP32累加后转FP16，整数结果无精度损失
    stacked = np.stack([data.astype(np.float32) for data in inputs])
    return np.sum(stacked, axis=0).astype(np.float16)


def print_data(name, data):
    print("%s: %s" % (name, " ".join(str(int(value)) for value in data.reshape(-1)[:32])))


def write_bin(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.astype(np.float16).tofile(path)


def get_input_path(input_dir, rank):
    return os.path.join(input_dir, "%d" % rank, "x1.bin")


def get_cpu_output_path(output_dir, rank):
    return os.path.join(output_dir, "%d" % rank, "output_cpu_low.bin")


def get_npu_output_path(output_dir, rank):
    return os.path.join(output_dir, "%d" % rank, "output_npu.bin")


def read_bin(path, expect_size):
    data = np.fromfile(path, dtype=np.float16)
    if data.size != expect_size:
        raise ValueError("%s size invalid, expect %d elements, got %d" % (path, expect_size, data.size))
    return data


def generate_data(args):
    inputs = [make_rank_input(rank, args.send_count) for rank in range(args.rank_size)]
    golden = make_golden(inputs)

    for rank, data in enumerate(inputs):
        write_bin(get_input_path(args.input_dir, rank), data)
        write_bin(get_cpu_output_path(args.output_dir, rank), golden)

    print_data("card0 input", inputs[0])
    print_data("card1 input", inputs[1])
    print_data("card0 CPU output", golden)
    print_data("card1 CPU output", golden)


def verify_outputs(args):
    expected_size = args.send_count
    passed = True
    for rank in range(args.rank_size):
        golden = read_bin(get_cpu_output_path(args.output_dir, rank), expected_size)
        output = read_bin(get_npu_output_path(args.output_dir, rank), expected_size)
        if rank < 2:
            print_data("card%d CPU output" % rank, golden)
            print_data("card%d NPU output" % rank, output)
        diff_indexes = np.where(output != golden)[0]
        for diff_index in diff_indexes[:100]:
            print(
                "card%d data index: %06d, expected: %u, actual: %u"
                % (rank, diff_index, golden[diff_index], output[diff_index])
            )
        error_ratio = float(diff_indexes.size) / float(golden.size)
        print("card%d error ratio: %.4f" % (rank, error_ratio))
        passed = passed and diff_indexes.size == 0
    if not passed:
        raise ValueError("[ERROR] result error")
    print("test pass!")


def parse_args():
    parser = argparse.ArgumentParser(description="Generate and verify MC2 AllReduce card0/card1 data.")
    parser.add_argument("--send-count", type=int, default=DEFAULT_SEND_COUNT)
    parser.add_argument("--rank-size", type=int, default=DEFAULT_RANK_SIZE)
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
        if parsed_args.rank_size < 2:
            raise ValueError("rank-size must be at least 2")
        if parsed_args.verify:
            verify_outputs(parsed_args)
        else:
            generate_data(parsed_args)
    except Exception as err:
        print(err)
        sys.exit(1)
