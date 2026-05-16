#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# ----------------------------------------------------------------------------------------------------------

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"
make -j -C "${BUILD_DIR}"

python3 "${SCRIPT_DIR}/scripts/gen_data.py"
cp -r "${SCRIPT_DIR}/input" "${SCRIPT_DIR}/output" "${BUILD_DIR}/"

(cd "${BUILD_DIR}" && ./demo)

python3 "${SCRIPT_DIR}/scripts/verify_result.py" \
    "${BUILD_DIR}/output/output_int64min.bin" \
    "${BUILD_DIR}/output/golden_int64min.bin" \
    "${BUILD_DIR}/output/output_normal.bin" \
    "${BUILD_DIR}/output/golden_normal.bin"
