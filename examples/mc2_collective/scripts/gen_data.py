#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# the CANN Open Software License Agreement Version 2.0.
# Please refer to the License for details.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO, NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import os
import sys

import numpy as np


DATA_TYPES = {
    "int8": np.int8,
    "int16": np.int16,
    "int32": np.int32,
    "fp16": np.float16,
    "fp32": np.float32,
    "int64": np.int64,
    "uint64": np.uint64,
    "uint8": np.uint8,
    "uint16": np.uint16,
    "uint32": np.uint32,
    "fp64": np.float64,
    "bf16": np.uint16,
    "hif8": np.uint8,
    "fp8e4m3": np.uint8,
    "fp8e5m2": np.uint8,
    "fp8e8m0": np.uint8,
}

REDUCE_TYPES = {"int8", "int16", "int32", "fp16", "fp32", "bf16"}


def get_input_path(input_dir, rank):
    return os.path.join(input_dir, str(rank), "x1.bin")


def get_metadata_path(input_dir, rank, name):
    return os.path.join(input_dir, str(rank), "%s.bin" % name)


def get_cpu_output_path(output_dir, rank):
    return os.path.join(output_dir, str(rank), "output_cpu_low.bin")


def get_npu_output_path(output_dir, rank):
    return os.path.join(output_dir, str(rank), "output_npu.bin")


def write_bin(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    np.asarray(data).tofile(path)


def write_u64_array(path, values):
    write_bin(path, np.asarray(values, dtype=np.uint64))


def bf16_from_float32(values):
    bits = np.asarray(values, dtype=np.float32).view(np.uint32)
    return (bits >> 16).astype(np.uint16)


def bf16_to_float32(values):
    bits = np.asarray(values, dtype=np.uint16).astype(np.uint32) << 16
    return bits.view(np.float32)


def make_values(rank, count, data_type):
    base = rank + 1
    if data_type == "fp16":
        return (base + np.arange(count, dtype=np.float32) / 10).astype(np.float16)
    if data_type == "fp32":
        return (base + np.arange(count, dtype=np.float32) / 10).astype(np.float32)
    if data_type == "fp64":
        return (base + np.arange(count, dtype=np.float64) / 10).astype(np.float64)
    if data_type == "bf16":
        return bf16_from_float32(base + np.arange(count, dtype=np.float32) / 10)
    if data_type.startswith("fp8") or data_type == "hif8":
        return ((np.arange(count, dtype=np.uint16) + base * 7) & 0xFF).astype(np.uint8)
    dtype = DATA_TYPES[data_type]
    return (np.arange(count, dtype=np.int64) + base * 100).astype(dtype)


def reduce_arrays(arrays, data_type, reduce_op):
    if data_type == "bf16":
        result = bf16_to_float32(arrays[0])
        for array in arrays[1:]:
            value = bf16_to_float32(array)
            if reduce_op == "sum":
                result = bf16_to_float32(bf16_from_float32(result + value))
            elif reduce_op == "max":
                result = bf16_to_float32(bf16_from_float32(np.maximum(result, value)))
            else:
                result = bf16_to_float32(bf16_from_float32(np.minimum(result, value)))
        return bf16_from_float32(result)

    result = arrays[0].copy()
    for array in arrays[1:]:
        if reduce_op == "sum":
            result = (result + array).astype(DATA_TYPES[data_type])
        elif reduce_op == "max":
            result = np.maximum(result, array).astype(DATA_TYPES[data_type])
        else:
            result = np.minimum(result, array).astype(DATA_TYPES[data_type])
    return result


def make_alltoallv_metadata(rank_size):
    send_counts = []
    for source in range(rank_size):
        send_counts.append([1 + ((source + 2 * destination) % 4) for destination in range(rank_size)])
    recv_counts = [[send_counts[source][destination] for source in range(rank_size)] for destination in range(rank_size)]

    send_displs = []
    for source in range(rank_size):
        offsets = []
        offset = 0
        for count in send_counts[source]:
            offsets.append(offset)
            offset += count
        send_displs.append(offsets)

    recv_displs = []
    for destination in range(rank_size):
        offsets = []
        offset = 0
        for count in recv_counts[destination]:
            offsets.append(offset)
            offset += count
        recv_displs.append(offsets)
    return send_counts, send_displs, recv_counts, recv_displs


def make_expected(args, inputs, rank, metadata):
    rank_size = args.rank_size
    if args.ops == "allgather":
        return np.concatenate(inputs)
    if args.ops == "allreduce":
        return reduce_arrays(inputs, args.data_type, args.reduce_op)
    if args.ops == "reducescatter":
        reduced = reduce_arrays(inputs, args.data_type, args.reduce_op)
        recv_count = reduced.size // rank_size
        return reduced[rank * recv_count : (rank + 1) * recv_count]
    if args.ops == "alltoall":
        data_count = inputs[0].size // rank_size
        return np.concatenate([inputs[source][rank * data_count : (rank + 1) * data_count]
                               for source in range(rank_size)])

    send_counts, send_displs, recv_counts, recv_displs = metadata
    output_elements = max(
        recv_displs[rank][source] + recv_counts[rank][source] for source in range(rank_size)
    )
    output = np.zeros(output_elements, dtype=DATA_TYPES[args.data_type])
    for source in range(rank_size):
        count = send_counts[source][rank]
        source_start = send_displs[source][rank]
        destination_start = recv_displs[rank][source]
        output[destination_start : destination_start + count] = inputs[source][source_start : source_start + count]
    return output


def generate_data(args):
    if args.rank_size < 1:
        raise ValueError("rank-size must be positive")
    if args.ops in ("alltoall", "reducescatter") and args.count <= 0:
        raise ValueError("count must be positive")
    if args.ops in ("allreduce", "allgather") and args.count <= 0:
        raise ValueError("count must be positive")
    if args.ops in ("allreduce", "reducescatter") and args.data_type not in REDUCE_TYPES:
        raise ValueError("%s does not support %s" % (args.data_type, args.ops))

    metadata = None
    if args.ops == "alltoallv":
        metadata = make_alltoallv_metadata(args.rank_size)
        inputs = []
        for rank in range(args.rank_size):
            input_elements = max(
                metadata[1][rank][destination] + metadata[0][rank][destination]
                for destination in range(args.rank_size)
            )
            inputs.append(make_values(rank, input_elements, args.data_type))
    else:
        elements_per_rank = args.count
        if args.ops in ("alltoall", "reducescatter"):
            elements_per_rank *= args.rank_size
        inputs = [make_values(rank, elements_per_rank, args.data_type) for rank in range(args.rank_size)]

    for rank, data in enumerate(inputs):
        write_bin(get_input_path(args.input_dir, rank), data)
        if metadata is not None:
            write_u64_array(get_metadata_path(args.input_dir, rank, "send_counts"), metadata[0][rank])
            write_u64_array(get_metadata_path(args.input_dir, rank, "send_displs"), metadata[1][rank])
            write_u64_array(get_metadata_path(args.input_dir, rank, "recv_counts"), metadata[2][rank])
            write_u64_array(get_metadata_path(args.input_dir, rank, "recv_displs"), metadata[3][rank])
        write_bin(get_cpu_output_path(args.output_dir, rank),
                  make_expected(args, inputs, rank, metadata))


def verify_outputs(args):
    passed = True
    for rank in range(args.rank_size):
        expected = np.fromfile(get_cpu_output_path(args.output_dir, rank), dtype=np.uint8)
        actual = np.fromfile(get_npu_output_path(args.output_dir, rank), dtype=np.uint8)
        diff_indexes = np.where(actual != expected)[0] if actual.size == expected.size else np.array([0])
        print("%d error ratio: %.4f" % (rank, float(diff_indexes.size) / max(float(expected.size), 1.0)))
        for diff_index in diff_indexes[:100]:
            print(
                "%d byte index: %06d, expected: %u, actual: %u"
                % (rank, diff_index, expected[diff_index], actual[diff_index])
            )
        passed = passed and diff_indexes.size == 0 and actual.size == expected.size
    if not passed:
        raise ValueError("[ERROR] result error")
    print("test pass!")


def parse_args():
    parser = argparse.ArgumentParser(description="Generate and verify MC2 collective data.")
    parser.add_argument("--ops", choices=["allgather", "allreduce", "alltoall", "alltoallv", "reducescatter"],
                        default="allgather")
    parser.add_argument("--count", type=int, default=128,
                        help="per-rank count for allgather/allreduce/reducescatter, or per-peer count for alltoall")
    parser.add_argument("--rank-size", type=int, default=2)
    parser.add_argument("--data-type", choices=sorted(DATA_TYPES), default="fp16")
    parser.add_argument("--reduce-op", choices=["sum", "max", "min"], default="sum")
    parser.add_argument("--input-dir", default="./input")
    parser.add_argument("--output-dir", default="./output")
    parser.add_argument("--verify", action="store_true")
    return parser.parse_args()


if __name__ == "__main__":
    try:
        parsed_args = parse_args()
        if parsed_args.verify:
            verify_outputs(parsed_args)
        else:
            generate_data(parsed_args)
    except Exception as err:
        print(err)
        sys.exit(1)
