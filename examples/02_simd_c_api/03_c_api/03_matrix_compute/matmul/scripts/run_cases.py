#!/usr/bin/env python3

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
import subprocess
import sys
from pathlib import Path


CASES = tuple(f"P{index:02d}" for index in range(1, 9))
SCRIPT_DIR = Path(__file__).resolve().parent
GEN_DATA = SCRIPT_DIR / "gen_data.py"
VERIFY_RESULT = SCRIPT_DIR / "verify_result.py"


def run(command: list[str], cwd: Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(command, cwd=cwd, capture_output=True, text=True, check=False)


def execute(binary: Path, work_dir: Path, cases: list[str], repeat: int) -> int:
    input_dir = work_dir / "input"
    output_path = work_dir / "output" / "output.bin"
    total = len(cases) * repeat
    passed = 0

    for case_name in cases:
        generated = run(
            [sys.executable, str(GEN_DATA), "--case", case_name, "--output-dir", str(input_dir)],
            work_dir,
        )
        if generated.returncode != 0:
            print(generated.stdout, end="")
            print(generated.stderr, end="", file=sys.stderr)
            print(f"summary={passed}/{total} passed")
            return 1

        for iteration in range(1, repeat + 1):
            output_path.unlink(missing_ok=True)
            print(f"case={case_name} iteration={iteration}/{repeat}")

            executed = run([str(binary)], work_dir)
            if executed.returncode != 0:
                print(executed.stdout, end="")
                print(executed.stderr, end="", file=sys.stderr)
                print(f"summary={passed}/{total} passed")
                return 1

            verified = run(
                [sys.executable, str(VERIFY_RESULT), str(output_path), str(input_dir / "golden.bin")],
                work_dir,
            )
            print(verified.stdout, end="")
            if verified.stderr:
                print(verified.stderr, end="", file=sys.stderr)
            if verified.returncode != 0:
                print(f"summary={passed}/{total} passed")
                return 1
            passed += 1

    print(f"summary={passed}/{total} passed")
    return 0


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate, execute, and verify fixed MatMul cases.")
    parser.add_argument("--binary", type=Path, required=True, help="Path to the compiled demo executable.")
    parser.add_argument("--work-dir", type=Path, default=Path.cwd(), help="Project run directory.")
    parser.add_argument("--repeat", type=int, default=10, help="Executions per case.")
    parser.add_argument("--cases", nargs="+", choices=CASES, default=list(CASES), help="Cases to execute.")
    args = parser.parse_args()
    if args.repeat < 1:
        parser.error("--repeat must be at least 1")
    args.binary = args.binary.expanduser().resolve()
    args.work_dir = args.work_dir.expanduser().resolve()
    if not args.binary.is_file():
        parser.error(f"binary does not exist: {args.binary}")
    return args


def main() -> int:
    args = parse_args()
    args.work_dir.mkdir(parents=True, exist_ok=True)
    return execute(args.binary, args.work_dir, args.cases, args.repeat)


if __name__ == "__main__":
    raise SystemExit(main())
