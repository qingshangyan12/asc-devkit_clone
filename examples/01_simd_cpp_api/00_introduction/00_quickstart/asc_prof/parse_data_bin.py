#!/usr/bin/env python3
import argparse
import csv
import struct
from pathlib import Path


def iter_records(data):
    usable_size = len(data) - (len(data) % 4)
    invalid_records = 0
    index = 0

    for offset in range(0, usable_size, 4):
        (raw,) = struct.unpack_from("<I", data, offset)
        high = (raw >> 28) & 0xF
        if high > 6:
            invalid_records += 1
            continue

        yield {
            "index": index,
            "offset": offset,
            "raw": f"0x{raw:08x}",
            "high": high,
            "marker": (raw >> 16) & 0xFFF,
            "time": raw & 0xFFFF,
        }
        index += 1

    iter_records.invalid_records = invalid_records
    iter_records.trailing_bytes = len(data) - usable_size


def iter_raw_words(data):
    usable_size = len(data) - (len(data) % 4)
    for index, offset in enumerate(range(0, usable_size, 4)):
        (raw,) = struct.unpack_from("<I", data, offset)
        yield {
            "index": index,
            "offset": offset,
            "raw": f"0x{raw:08x}",
        }


iter_records.invalid_records = 0
iter_records.trailing_bytes = 0


def parse_args():
    parser = argparse.ArgumentParser(description="Parse asc_prof data.bin records.")
    parser.add_argument("input", nargs="?", default="data.bin", help="binary input file, default: data.bin")
    parser.add_argument("--output", "-o", default="data.csv", help="CSV output file, default: data.csv")
    parser.add_argument(
        "--raw-output",
        default=None,
        help="CSV file containing every raw 4-byte word as hex, default: <output>.raw.csv",
    )
    return parser.parse_args()


def main():
    args = parse_args()
    input_path = Path(args.input)
    output_path = Path(args.output)
    raw_output_path = Path(args.raw_output) if args.raw_output is not None else output_path.with_suffix(".raw.csv")
    data = input_path.read_bytes()
    raw_words = list(iter_raw_words(data))
    records = list(iter_records(data))

    with output_path.open("w", newline="") as fp:
        writer = csv.DictWriter(fp, fieldnames=["index", "offset", "raw", "high", "marker", "time"])
        writer.writeheader()
        writer.writerows(records)

    with raw_output_path.open("w", newline="") as fp:
        writer = csv.DictWriter(fp, fieldnames=["index", "offset", "raw"])
        writer.writeheader()
        writer.writerows(raw_words)

    print(
        f"input={input_path} output={output_path} raw_output={raw_output_path} bytes={len(data)} "
        f"raw_words={len(raw_words)} "
        f"records={len(records)} invalid_records={iter_records.invalid_records} "
        f"trailing_bytes={iter_records.trailing_bytes} skipped={iter_records.invalid_records}"
    )


if __name__ == "__main__":
    main()
