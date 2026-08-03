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
import shutil
import subprocess
import sys
from pathlib import Path


CASES = ("uniform", "normal", "boundary", "bias_only", "non_aligned_tail", "zeros")


def run(command, cwd: Path):
    print("+", " ".join(str(item) for item in command), flush=True)
    subprocess.run(command, cwd=cwd, check=True)


def main():
    parser = argparse.ArgumentParser(
        description="Run all MatMul precision cases on an Ascend 950 environment."
    )
    parser.add_argument("--demo", type=Path, default=Path("build/demo"))
    parser.add_argument("--root", type=Path, default=Path("."))
    parser.add_argument("--seed", type=int, default=20260803)
    args = parser.parse_args()

    root = args.root.resolve()
    demo = args.demo if args.demo.is_absolute() else (root / args.demo)
    if not demo.is_file():
        raise FileNotFoundError(f"demo executable not found: {demo}")

    scripts = Path(__file__).resolve().parent
    archive_dir = root / "output" / "cases"
    archive_dir.mkdir(parents=True, exist_ok=True)

    for offset, case_name in enumerate(CASES):
        run(
            [
                sys.executable,
                str(scripts / "gen_data.py"),
                "--case",
                case_name,
                "--seed",
                str(args.seed + offset),
                "--root",
                str(root),
            ],
            root,
        )
        run([str(demo)], root)
        report_path = archive_dir / f"{case_name}_report.json"
        run(
            [
                sys.executable,
                str(scripts / "verify_result.py"),
                str(root / "output" / "output.bin"),
                str(root / "output" / "golden.bin"),
                "--report",
                str(report_path),
            ],
            root,
        )
        shutil.copy2(
            root / "output" / "output.bin", archive_dir / f"{case_name}_output.bin"
        )
        shutil.copy2(
            root / "output" / "golden.bin", archive_dir / f"{case_name}_golden.bin"
        )
        shutil.copy2(
            root / "output" / "case_manifest.json",
            archive_dir / f"{case_name}_manifest.json",
        )

    print(f"all {len(CASES)} cases passed; reports are under {archive_dir}")


if __name__ == "__main__":
    main()
