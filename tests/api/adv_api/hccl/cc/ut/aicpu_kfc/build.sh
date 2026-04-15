#!/bin/bash
# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set -euo pipefail
trap 'echo "Error: build.sh failed at line $LINENO" >&2; exit 1' ERR

CURRENT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="${CURRENT_DIR}/build"
THIRD_PARTY_DIR="${BUILD_DIR}/third_party"
THREAD_NUM=$(nproc)
BUILD_TYPE="Debug"
CANN_PATH=""
ENABLE_ASAN="false"
BUILD_ONLY="false"
CLEAN_ONLY="false"

usage() {
    cat <<EOF
Usage: bash build.sh [options]

Options:
  -j, --jobs <N>          Build with N parallel jobs
  -p, --cann_path <PATH>  Ascend CANN installation path
      --build-type <TYPE> CMake build type, default: Debug
      --asan              Enable ASAN
      --build-only        Configure and build only, do not run ctest
      --clean             Remove build directory and exit
  -h, --help              Show this help
EOF
}

resolve_cann_path() {
    if [[ -n "${CANN_PATH}" ]]; then
        return
    fi

    if [[ -n "${ASCEND_HOME_PATH:-}" ]]; then
        CANN_PATH="${ASCEND_HOME_PATH}"
    elif [[ -n "${ASCEND_OPP_PATH:-}" ]]; then
        CANN_PATH=$(dirname "${ASCEND_OPP_PATH}")
    else
        local candidates=(
            "/usr/local/Ascend/ascend-toolkit/latest"
            "/usr/local/Ascend/cann/latest"
            "/usr/local/Ascend/cann"
            "${HOME}/Ascend/ascend-toolkit/latest"
            "${HOME}/Ascend/cann/latest"
            "${HOME}/Ascend/cann"
        )
        local candidate
        for candidate in "${candidates[@]}"; do
            if [[ -d "${candidate}" ]]; then
                CANN_PATH="${candidate}"
                break
            fi
        done
    fi
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        -j|--jobs)
            THREAD_NUM="$2"
            shift 2
            ;;
        -p|--cann_path)
            CANN_PATH="$2"
            shift 2
            ;;
        --build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        --asan)
            ENABLE_ASAN="true"
            shift
            ;;
        --build-only)
            BUILD_ONLY="true"
            shift
            ;;
        --clean)
            CLEAN_ONLY="true"
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Error: unsupported option $1" >&2
            usage
            exit 1
            ;;
    esac
done

if [[ "${CLEAN_ONLY}" == "true" ]]; then
    rm -rf "${BUILD_DIR}"
    exit 0
fi

resolve_cann_path

if [[ -n "${CANN_PATH}" && -f "${CANN_PATH}/set_env.sh" ]]; then
    # shellcheck disable=SC1090
    source "${CANN_PATH}/set_env.sh" || true
fi

rm -rf "${BUILD_DIR}"
mkdir -p "${THIRD_PARTY_DIR}"

cmake -S "${CURRENT_DIR}" -B "${BUILD_DIR}" \
    -DENABLE_TEST=ON \
    -DBUILD_OPEN_PROJECT=ON \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCANN_3RD_LIB_PATH="${THIRD_PARTY_DIR}" \
    ${CANN_PATH:+-DCUSTOM_ASCEND_CANN_PACKAGE_PATH="${CANN_PATH}"} \
    -DENABLE_ASAN="${ENABLE_ASAN}"

cmake --build "${BUILD_DIR}" --target aicpu_kfc_utest_aicpu -j "${THREAD_NUM}"

if [[ "${BUILD_ONLY}" == "true" ]]; then
    exit 0
fi

export LD_LIBRARY_PATH="${BUILD_DIR}/stub:${LD_LIBRARY_PATH:-}"
ctest --test-dir "${BUILD_DIR}" --output-on-failure -R '^aicpu_kfc_utest_aicpu$'
