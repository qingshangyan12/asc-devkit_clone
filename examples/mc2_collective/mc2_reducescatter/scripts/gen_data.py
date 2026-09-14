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


DEFAULT_RECV_COUNT = 128
DEFAULT_RANK_SIZE = 2
DEFAULT_DTYPE = "fp16"

# 与main.asc中DTYPE_TABLE保持一致的数据类型集合。
# ReduceScatter的SUM归约仅支持int8/int16/int32/fp16/fp32/bf16，不支持fp8等类型。
DTYPE_MAP = {
    "int8": np.int8,
    "int16": np.int16,
    "int32": np.int32,
    "fp16": np.float16,
    "fp32": np.float32,
    "bf16": np.uint16,
}

# bf16无numpy原生类型，按原始bit(uint16)存储，需要专门的编解码。
INT_DTYPES = ("int8", "int16", "int32")


def to_bf16_bits(values):
    # float数组按就近偶数舍入转为bf16位模式
    bits = np.asarray(values, dtype=np.float32).view(np.uint32)
    rounded = (bits.astype(np.uint64) + 0x7FFF + ((bits >> 16) & 1)) & 0xFFFF0000
    return (rounded >> 16).astype(np.uint16)


def from_bf16_bits(bits):
    return (bits.astype(np.uint32) << 16).view(np.float32)


def make_rank_input(rank, recv_count, rank_size, dtype_name):
    # 输入为recv_count * rank_size个元素。
    # int8/bf16取值范围小，使用小值模式：第rank卡第k个元素为((rank + k) mod 2) * ((k mod 15) + 1)，
    # 保证任意归约顺序下的部分和均不超过int8表示范围/bf16精确表示范围(256)；
    # 其余类型沿用大值模式：第rank卡第k个元素为(rank + 1) + (k mod 32)
    k = np.arange(recv_count * rank_size, dtype=np.int64)
    if dtype_name in ("int8", "bf16"):
        values = ((rank + k) % 2) * ((k % 15) + 1)
    else:
        values = (rank + 1) + (k % 32)
    if dtype_name == "bf16":
        return to_bf16_bits(values)
    return values.astype(DTYPE_MAP[dtype_name])


def decode_input(data, dtype_name):
    # 统一解码为float64用于真值计算：小整数在各类型下均精确
    if dtype_name == "bf16":
        return from_bf16_bits(data).astype(np.float64)
    return data.astype(np.float64)


def encode_golden(sums, dtype_name):
    if dtype_name == "bf16":
        return to_bf16_bits(sums)
    if dtype_name in INT_DTYPES:
        # 整数SUM按补码回绕语义编码
        np_dtype = DTYPE_MAP[dtype_name]
        dtype_bits = np.dtype(np_dtype).itemsize * 8
        bits = sums.astype(np.int64) % (1 << dtype_bits)
        bits = np.where(bits >= (1 << (dtype_bits - 1)), bits - (1 << dtype_bits), bits)
        return bits.astype(np_dtype)
    return sums.astype(DTYPE_MAP[dtype_name])


def make_golden(rank, recv_count, rank_size, inputs, dtype_name):
    # ReduceScatter: y[i] = sum_rank(x_rank[rank * recv_count + i])，
    # 即每个rank收到归约结果中第rank块
    blocks = [decode_input(data[rank * recv_count:(rank + 1) * recv_count], dtype_name) for data in inputs]
    return encode_golden(np.sum(np.stack(blocks), axis=0), dtype_name)


def format_value(value):
    if isinstance(value, (np.floating, float)):
        return "%g" % value
    return str(int(value))


def decode_for_print(data, dtype_name):
    values = from_bf16_bits(data) if dtype_name == "bf16" else data
    return values


def print_data(name, data, dtype_name):
    values = decode_for_print(data.reshape(-1)[:32], dtype_name)
    print("%s: %s" % (name, " ".join(format_value(value) for value in values)))


def write_bin(path, data, dtype_name):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    data.astype(DTYPE_MAP[dtype_name]).tofile(path)


def get_input_path(input_dir, rank):
    return os.path.join(input_dir, "%d" % rank, "x1.bin")


def get_cpu_output_path(output_dir, rank):
    return os.path.join(output_dir, "%d" % rank, "output_cpu_low.bin")


def get_npu_output_path(output_dir, rank):
    return os.path.join(output_dir, "%d" % rank, "output_npu.bin")


def read_bin(path, expect_size, dtype_name):
    data = np.fromfile(path, dtype=DTYPE_MAP[dtype_name])
    if data.size != expect_size:
        raise ValueError("%s size invalid, expect %d elements, got %d" % (path, expect_size, data.size))
    return data


def generate_data(args):
    inputs = [
        make_rank_input(rank, args.recv_count, args.rank_size, args.dtype)
        for rank in range(args.rank_size)
    ]

    for rank, data in enumerate(inputs):
        write_bin(get_input_path(args.input_dir, rank), data, args.dtype)
        write_bin(
            get_cpu_output_path(args.output_dir, rank),
            make_golden(rank, args.recv_count, args.rank_size, inputs, args.dtype),
            args.dtype,
        )

    print_data("card0 input", inputs[0], args.dtype)
    if args.rank_size > 1:
        print_data("card1 input", inputs[1], args.dtype)
    print_data("card0 CPU output", make_golden(0, args.recv_count, args.rank_size, inputs, args.dtype), args.dtype)


def verify_outputs(args):
    expected_size = args.recv_count
    passed = True
    for rank in range(args.rank_size):
        npu_output_path = get_npu_output_path(args.output_dir, rank)
        if not os.path.isfile(npu_output_path):
            raise ValueError("[ERROR] output file not found: %s, run ./demo before verify" % npu_output_path)
        golden = read_bin(get_cpu_output_path(args.output_dir, rank), expected_size, args.dtype)
        output = read_bin(npu_output_path, expected_size, args.dtype)
        if rank < 2:
            print_data("card%d CPU output" % rank, golden, args.dtype)
            print_data("card%d NPU output" % rank, output, args.dtype)
        diff_indexes = np.where(output != golden)[0]
        for diff_index in diff_indexes[:100]:
            print(
                "card%d data index: %06d, expected: %s, actual: %s"
                % (rank, diff_index, format_value(decode_for_print(golden[diff_index:diff_index + 1], args.dtype)[0]),
                   format_value(decode_for_print(output[diff_index:diff_index + 1], args.dtype)[0]))
            )
        error_ratio = float(diff_indexes.size) / float(golden.size)
        print("card%d error ratio: %.4f" % (rank, error_ratio))
        passed = passed and diff_indexes.size == 0
    if not passed:
        raise ValueError("[ERROR] result error")
    print("test pass!")


def parse_args():
    parser = argparse.ArgumentParser(description="Generate and verify MC2 ReduceScatter data with configurable dtype.")
    parser.add_argument("--recv-count", type=int, default=DEFAULT_RECV_COUNT)
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
        if parsed_args.recv_count < 1:
            raise ValueError("recv-count must be at least 1")
        if parsed_args.verify:
            verify_outputs(parsed_args)
        else:
            generate_data(parsed_args)
    except Exception as err:
        print(err)
        sys.exit(1)
