#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.

import argparse
import struct
import sys
from pathlib import Path


RESULT_STRIDE = 32

CHECKS = [
    (
        "host_stream_output.bin",
        [
            (0, "same Stream write then read", 11),
            (1, "cross Stream Event wait then read", 22),
            (2, "aclrtSynchronizeEvent then Host read", 33),
            (3, "aclrtSynchronizeDevice then Host read", 44),
            (4, "same Stream async H2D then kernel read", 55),
        ],
    ),
    (
        "pipeline_output.bin",
        [
            (0, "MTE2 -> MTE3 notify/wait result", 42),
            (1, "same MTE2 pipeline sync_pipe result", 66),
        ],
    ),
    (
        "atomic_output.bin",
        [
            (0, "two-core DMA atomic add result", 2),
        ],
    ),
]


def read_int32_file(path: Path):
    data = path.read_bytes()
    if len(data) % 4 != 0:
        raise ValueError(f"{path} size {len(data)} is not aligned to int32")
    return list(struct.unpack(f"<{len(data) // 4}i", data))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output_dir", nargs="?", default="output")
    args = parser.parse_args()

    output_dir = Path(args.output_dir)
    failed = []

    for file_name, checks in CHECKS:
        path = output_dir / file_name
        if not path.exists():
            failed.append((file_name, "-", "file exists", "missing"))
            continue

        values = read_int32_file(path)
        print(f"{file_name}:")
        for index, desc, expected in checks:
            offset = index * RESULT_STRIDE
            actual = values[offset] if offset < len(values) else None
            print(f"  [{index:02d}] {desc} = {actual}")
            if actual != expected:
                failed.append((file_name, index, expected, actual, desc))

    if failed:
        print("\nchecks failed:")
        for item in failed:
            if len(item) == 4:
                file_name, index, expected, actual = item
                print(f"  {file_name} [{index}] expected {expected}, actual {actual}")
            else:
                file_name, index, expected, actual, desc = item
                print(f"  {file_name} [{index}] expected {expected}, actual {actual}: {desc}")
        return 1

    print("\nall checks pass")
    return 0


if __name__ == "__main__":
    sys.exit(main())
