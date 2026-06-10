#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -euo pipefail

REPO_URL="${REPO_URL:-https://gitcode.com/cann/asc-devkit}"
REPO_BRANCH="${REPO_BRANCH:-master}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORK_ROOT="${WORK_ROOT:-${SCRIPT_DIR}/build/prepared_master_src}"

log() {
    local current_time
    current_time="$(date +"%Y-%m-%d %H:%M:%S")"
    echo "[${current_time}] $1"
}

usage() {
    cat <<EOF
Usage: bash prepare_and_build.sh [build.sh args...]

This script will:
1. Clone ${REPO_URL} branch ${REPO_BRANCH}
2. Replace these directories in the cloned repo with the current workspace versions:
   - impl/tensor_api
   - include/tensor_api
   - tests/api/tensor_api
3. Run the cloned repo's build.sh with the provided arguments

Optional environment variables:
  REPO_URL     Clone source URL, default: ${REPO_URL}
  REPO_BRANCH  Branch to clone, default: ${REPO_BRANCH}
  WORK_ROOT    Temporary worktree path, default: ${WORK_ROOT}

Example:
  bash prepare_and_build.sh --basic_test_three --cov --cann_3rd_lib_path="/home/jenkins/opensource"
EOF
}

sync_dir() {
    local src_dir="$1"
    local dst_dir="$2"

    if [[ ! -d "${src_dir}" ]]; then
        log "Error: source directory not found: ${src_dir}"
        exit 1
    fi

    mkdir -p "${dst_dir}"
    if command -v rsync >/dev/null 2>&1; then
        rsync -a --delete "${src_dir}/" "${dst_dir}/"
    else
        rm -rf "${dst_dir}"
        mkdir -p "${dst_dir}"
        cp -a "${src_dir}/." "${dst_dir}/"
    fi
}

prepare_repo() {
    log "Preparing clean workspace at ${WORK_ROOT}"
    rm -rf "${WORK_ROOT}"
    git clone --branch "${REPO_BRANCH}" --depth 1 "${REPO_URL}" "${WORK_ROOT}"

    log "Overlaying current tensor_api sources"
    sync_dir "${SCRIPT_DIR}/impl/tensor_api" "${WORK_ROOT}/impl/tensor_api"
    sync_dir "${SCRIPT_DIR}/include/tensor_api" "${WORK_ROOT}/include/tensor_api"
    sync_dir "${SCRIPT_DIR}/tests/api/tensor_api" "${WORK_ROOT}/tests/api/tensor_api"
}

main() {
    if [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
        usage
        exit 0
    fi

    prepare_repo

    log "Running cloned build.sh $*"
    cd "${WORK_ROOT}"
    bash build.sh "$@"
}

main "$@"
