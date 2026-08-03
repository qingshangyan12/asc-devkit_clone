#!/usr/bin/env bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(cd "$SCRIPT_DIR/../../../../.." && pwd)

ARCH=${ARCH:-dav-3510}
ASC_RUN_MODE=${ASC_RUN_MODE:-npu}
BUILD_DIR=${BUILD_DIR:-$SCRIPT_DIR/build}
RUN_DIR=${RUN_DIR:-$SCRIPT_DIR}
JOBS=${JOBS:-2}
SEED=${SEED:-20260803}
USE_REPO_HEADERS=${USE_REPO_HEADERS:-1}

if [[ -n "${CANN_ENV:-}" ]]; then
    if [[ ! -f "$CANN_ENV" ]]; then
        echo "[ERROR] CANN environment script not found: $CANN_ENV" >&2
        exit 1
    fi
    # shellcheck disable=SC1090
    source "$CANN_ENV"
fi

for command in cmake python3; do
    if ! command -v "$command" >/dev/null 2>&1; then
        echo "[ERROR] required command not found: $command" >&2
        exit 1
    fi
done

if [[ ! "$JOBS" =~ ^[1-9][0-9]*$ ]]; then
    echo "[ERROR] JOBS must be a positive integer, but got: $JOBS" >&2
    exit 1
fi

if [[ ! "$SEED" =~ ^-?[0-9]+$ ]]; then
    echo "[ERROR] SEED must be an integer, but got: $SEED" >&2
    exit 1
fi

mkdir -p "$BUILD_DIR" "$RUN_DIR"
BUILD_DIR=$(cd "$BUILD_DIR" && pwd)
RUN_DIR=$(cd "$RUN_DIR" && pwd)

cmake_args=(
    -S "$SCRIPT_DIR"
    -B "$BUILD_DIR"
    -DCMAKE_ASC_ARCHITECTURES="$ARCH"
    -DCMAKE_ASC_RUN_MODE="$ASC_RUN_MODE"
)

if [[ "$USE_REPO_HEADERS" == "1" ]]; then
    cmake_args+=(
        "-DCMAKE_ASC_FLAGS=-iquote $REPO_ROOT/include -iquote $REPO_ROOT"
    )
elif [[ "$USE_REPO_HEADERS" != "0" ]]; then
    echo "[ERROR] USE_REPO_HEADERS must be 0 or 1, but got: $USE_REPO_HEADERS" >&2
    exit 1
else
    cmake_args+=("-DCMAKE_ASC_FLAGS=")
fi

echo "[INFO] Configure MatMul sample"
echo "[INFO] Architecture: $ARCH"
echo "[INFO] ASC run mode: $ASC_RUN_MODE"
echo "[INFO] Build directory: $BUILD_DIR"
echo "[INFO] Run directory: $RUN_DIR"
echo "[INFO] Use asc-devkit repository headers: $USE_REPO_HEADERS"

cmake "${cmake_args[@]}"
cmake --build "$BUILD_DIR" -j "$JOBS"

python3 "$SCRIPT_DIR/scripts/run_cases.py" \
    --demo "$BUILD_DIR/demo" \
    --root "$RUN_DIR" \
    --seed "$SEED"

python3 - "$RUN_DIR" "$ARCH" "$ASC_RUN_MODE" "$USE_REPO_HEADERS" "$BUILD_DIR" <<'PY'
import json
import sys
from pathlib import Path


run_dir = Path(sys.argv[1])
architecture = sys.argv[2]
run_mode = sys.argv[3]
use_repo_headers = sys.argv[4]
build_dir = sys.argv[5]
report_dir = run_dir / "output" / "cases"
case_names = (
    "uniform",
    "normal",
    "boundary",
    "bias_only",
    "non_aligned_tail",
    "zeros",
)

rows = []
for case_name in case_names:
    report_path = report_dir / f"{case_name}_report.json"
    report = json.loads(report_path.read_text(encoding="utf-8"))
    rows.append(
        (
            case_name,
            report["matched_ratio"],
            report["max_abs_error"],
            report["effective_max_abs_error_limit"],
            report["passed"],
        )
    )

passed_count = sum(1 for row in rows if row[4])
separator = "=" * 88
print()
print(separator)
print("MatMul validation summary")
print(separator)
print(f"{'Case':<20} {'Matched ratio':>15} {'Max abs error':>18} {'Effective limit':>18} {'Result':>10}")
print("-" * 88)
for case_name, matched_ratio, max_abs_error, effective_limit, passed in rows:
    max_error_text = "n/a" if max_abs_error is None else f"{max_abs_error:.10g}"
    result_text = "PASS" if passed else "FAIL"
    print(
        f"{case_name:<20} {matched_ratio:>15.6f} {max_error_text:>18} "
        f"{effective_limit:>18.10g} {result_text:>10}"
    )
print("-" * 88)
print(f"Cases: {passed_count}/{len(rows)} passed")
print(f"Architecture: {architecture}")
print(f"ASC run mode: {run_mode}")
print(f"Header source: {'asc-devkit repository' if use_repo_headers == '1' else 'CANN installation'}")
print(f"Build directory: {build_dir}")
print(f"Report directory: {report_dir}")
print("Overall result: PASS")
print(separator)
PY
