#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.

import argparse
import struct
from pathlib import Path


RESULT_LABELS = {
    0: "RAW negative: Scalar cache write, no DCCI, bypass reader",
    1: "RAW positive: Scalar cache write, DCCI+DSB, bypass reader",
    2: "stale-read negative: cached Scalar reader, no read-side DCCI",
    3: "stale-read positive: cached Scalar reader, read-side DCCI",
    4: "WAW negative A: Scalar dirty line flushed after bypass write",
    5: "WAW negative B: Scalar dirty line flushed after bypass write",
    6: "WAW positive A: Scalar dirty line flushed before bypass write",
    7: "WAW positive B: Scalar dirty line flushed before bypass write",
    8: "bypass positive: bypass write then bypass read",
    9: "atomic negative: asc_set_atomic_add_int after Scalar dirty init without DCCI",
    10: "atomic positive: asc_set_atomic_add_int after Scalar dirty init with DCCI",
    15: "progress marker: last completed verification step",
}

RESULT_STRIDE = 32

POSITIVE_EXPECTED = {
    1: 1,
    3: 1,
    6: 1,
    7: 2,
    8: 1,
    10: 15,
}

NEGATIVE_MANIFEST = {
    0: 0,
    2: 0,
    5: 0,
    9: 5,
}


def read_int32_file(path: Path):
    data = path.read_bytes()
    if len(data) % 4 != 0:
        raise ValueError(f"{path} size is not int32 aligned: {len(data)} bytes")
    return list(struct.unpack(f"<{len(data) // 4}i", data))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output", nargs="?", default="output/output.bin")
    parser.add_argument(
        "--strict-negative",
        action="store_true",
        help="fail if no negative case observes the expected stale/overwritten value",
    )
    args = parser.parse_args()

    values = read_int32_file(Path(args.output))
    observed = {index: values[index * RESULT_STRIDE] for index in RESULT_LABELS if index * RESULT_STRIDE < len(values)}
    print("observed results:")
    for index in sorted(RESULT_LABELS):
        if index in observed:
            print(f"  [{index:02d}] {RESULT_LABELS[index]} = {observed[index]}")

    failed = []
    for index, expected in POSITIVE_EXPECTED.items():
        actual = observed[index]
        if actual != expected:
            failed.append((index, expected, actual))

    if failed:
        print("\npositive checks failed:")
        for index, expected, actual in failed:
            print(f"  [{index}] expected {expected}, actual {actual}: {RESULT_LABELS[index]}")
        raise SystemExit(1)

    manifested = []
    for index, expected in NEGATIVE_MANIFEST.items():
        if observed[index] == expected:
            manifested.append(index)

    if manifested:
        print("\nnegative observations manifested:")
        for index in manifested:
            print(f"  [{index}] {RESULT_LABELS[index]} observed {observed[index]}")
    else:
        print("\nnegative observations did not manifest in this run.")
        if args.strict_negative:
            raise SystemExit(2)

    print("\npositive checks pass")


if __name__ == "__main__":
    main()
