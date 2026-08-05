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
import csv
import json
import statistics
from pathlib import Path


RAW_FIELDS = (
    "case",
    "dtype",
    "length",
    "repeat",
    "task_type",
    "block_num",
    "task_duration_us",
    "aiv_time_us",
    "aiv_total_cycles",
    "aicore_time_us",
    "aic_total_cycles",
    "cube_utilization_percent",
    "op_summary_csv",
)

SUMMARY_FIELDS = (
    "case",
    "dtype",
    "length",
    "repeats",
    "task_type",
    "block_num",
    "task_duration_median_us",
    "task_duration_min_us",
    "task_duration_max_us",
    "aiv_time_median_us",
    "aiv_time_min_us",
    "aiv_time_max_us",
    "aiv_total_cycles_median",
    "status",
)


def parse_number(row, key, csv_path):
    value = (row.get(key) or "").strip()
    if not value or value in ("-", "N/A"):
        raise ValueError(f"missing {key} in {csv_path}")
    return float(value)


def select_elu_row(csv_path):
    with csv_path.open(newline="", encoding="utf-8-sig") as stream:
        rows = list(csv.DictReader(stream))
    selected = [
        row
        for row in rows
        if "elu_custom" in (row.get("Op Name") or row.get("OP Name") or "").lower()
    ]
    if len(selected) != 1:
        raise ValueError(f"expected one ELU row in {csv_path}, found {len(selected)}")
    return selected[0]


def write_csv(path, fieldnames, rows):
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def format_number(value):
    return f"{value:.9g}"


def parse_args():
    parser = argparse.ArgumentParser(
        description="Validate and summarize ELU msprof op_summary CSV files."
    )
    parser.add_argument("--manifest", required=True)
    parser.add_argument("--results-root", required=True)
    parser.add_argument("--repeats", type=int, required=True)
    return parser.parse_args()


def main():
    args = parse_args()
    if args.repeats <= 0:
        raise ValueError("repeats must be positive")

    manifest = json.loads(Path(args.manifest).read_text(encoding="utf-8"))
    results_root = Path(args.results_root)
    raw_rows = []
    summary_rows = []

    for case in manifest:
        name = case["name"]
        correctness_log = results_root / "correctness" / f"{name}.log"
        correctness = correctness_log.read_text(encoding="utf-8", errors="replace")
        if "status=PASS" not in correctness:
            raise ValueError(f"correctness did not pass: {correctness_log}")

        case_rows = []
        for repeat in range(1, args.repeats + 1):
            repeat_root = results_root / "profiles" / name / f"repeat_{repeat:03d}"
            csv_paths = sorted(repeat_root.rglob("op_summary*.csv"))
            if len(csv_paths) != 1:
                raise ValueError(
                    f"expected one op_summary CSV under {repeat_root}, found {len(csv_paths)}"
                )
            csv_path = csv_paths[0]
            row = select_elu_row(csv_path)
            task_type = (row.get("Task Type") or "").strip()
            block_num = int(parse_number(row, "Block Num", csv_path))
            task_duration = parse_number(row, "Task Duration(us)", csv_path)
            aiv_time = parse_number(row, "aiv_time(us)", csv_path)
            aiv_cycles = parse_number(row, "aiv_total_cycles", csv_path)
            aicore_time = parse_number(row, "aicore_time(us)", csv_path)
            aic_cycles = parse_number(row, "aic_total_cycles", csv_path)
            cube_utilization = parse_number(row, "cube_utilization(%)", csv_path)

            if task_type != "AI_VECTOR_CORE":
                raise ValueError(f"unexpected Task Type {task_type!r} in {csv_path}")
            if block_num != case["expected_blocks"]:
                raise ValueError(
                    f"Block Num mismatch for {name}: expected {case['expected_blocks']}, got {block_num}"
                )
            if aicore_time != 0.0 or aic_cycles != 0.0 or cube_utilization != 0.0:
                raise ValueError(f"Cube/AIC activity detected in {csv_path}")

            raw_row = {
                "case": name,
                "dtype": case["dtype"],
                "length": case["length"],
                "repeat": repeat,
                "task_type": task_type,
                "block_num": block_num,
                "task_duration_us": format_number(task_duration),
                "aiv_time_us": format_number(aiv_time),
                "aiv_total_cycles": format_number(aiv_cycles),
                "aicore_time_us": format_number(aicore_time),
                "aic_total_cycles": format_number(aic_cycles),
                "cube_utilization_percent": format_number(cube_utilization),
                "op_summary_csv": str(csv_path.relative_to(results_root)),
            }
            raw_rows.append(raw_row)
            case_rows.append(
                {
                    "task_duration": task_duration,
                    "aiv_time": aiv_time,
                    "aiv_cycles": aiv_cycles,
                }
            )

        task_durations = [row["task_duration"] for row in case_rows]
        aiv_times = [row["aiv_time"] for row in case_rows]
        aiv_cycles_values = [row["aiv_cycles"] for row in case_rows]
        summary_rows.append(
            {
                "case": name,
                "dtype": case["dtype"],
                "length": case["length"],
                "repeats": args.repeats,
                "task_type": "AI_VECTOR_CORE",
                "block_num": case["expected_blocks"],
                "task_duration_median_us": format_number(
                    statistics.median(task_durations)
                ),
                "task_duration_min_us": format_number(min(task_durations)),
                "task_duration_max_us": format_number(max(task_durations)),
                "aiv_time_median_us": format_number(statistics.median(aiv_times)),
                "aiv_time_min_us": format_number(min(aiv_times)),
                "aiv_time_max_us": format_number(max(aiv_times)),
                "aiv_total_cycles_median": format_number(
                    statistics.median(aiv_cycles_values)
                ),
                "status": "PASS",
            }
        )

    write_csv(results_root / "performance_raw.csv", RAW_FIELDS, raw_rows)
    write_csv(results_root / "performance_summary.csv", SUMMARY_FIELDS, summary_rows)
    print(
        f"PERFORMANCE_SUMMARY cases={len(summary_rows)} repeats={args.repeats} "
        f"profiles={len(raw_rows)} status=PASS"
    )


if __name__ == "__main__":
    main()
