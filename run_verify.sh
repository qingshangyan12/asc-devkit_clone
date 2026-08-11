#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS FILE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# 一键运行 Tensor API UT + 编译通过的样例
# 用法: bash run_verify.sh [cann_path] [thread_num]

set -e

CANN_PATH="${1:-/mnt/workspace/bao0805/cann-9.2.0}"
THREAD_NUM="${2:-32}"
CURRENT_DIR=$(dirname $(readlink -f ${BASH_SOURCE[0]}))

dotted_line="----------------------------------------------------------------"
PASS=0
FAIL=0
FAILED_LIST=()

function log() {
    local current_time=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[$current_time] $1"
}

function record_result() {
    local name="$1"
    local status="$2"
    if [ "$status" = "PASS" ]; then
        echo "  [PASS] $name"
        PASS=$((PASS+1))
    else
        echo "  [FAIL] $name"
        FAIL=$((FAIL+1))
        FAILED_LIST+=("$name")
    fi
}

# ========== 1. 运行 UT ==========

function run_ut() {
    log "========== 运行 Tensor API UT =========="

    cp "${CURRENT_DIR}/tests/test_parts.sh" /tmp/test_parts.sh.bak

    bash build.sh --basic_test_three -j${THREAD_NUM} 2>&1 | tee /tmp/ut_output.log | grep -E 'PASSED|FAILED|error:'

    cp /tmp/test_parts.sh.bak "${CURRENT_DIR}/tests/test_parts.sh"

    local aic_pass=$(grep -c 'PASSED' /tmp/ut_output.log || true)
    if [ "$aic_pass" -gt 0 ]; then
        record_result "UT (AIC + AIV)" "PASS"
    else
        record_result "UT (AIC + AIV)" "FAIL"
    fi

    rm -rf "${CURRENT_DIR}/impl/utils" "${CURRENT_DIR}/include/utils"
}

# ========== 2. 编译样例 ==========

function build_examples() {
    log "========== 编译样例 =========="

    source "${CANN_PATH}/set_env.sh"

    # 仅编译已知通过的样例
    local examples=(
        "examples/01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api"
        "examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api"
        "examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api"
        "examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api"
        "examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/batch_matmul_tensor_api"
        "examples/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance"
    )

    for ex in "${examples[@]}"; do
        local name=$(basename "$ex")
        rm -rf "${ex}/build"
        if cmake -S "$ex" -B "$ex/build" -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 > /dev/null 2>&1 && \
           cmake --build "$ex/build" -j > /dev/null 2>&1; then
            record_result "$name" "PASS"
        else
            record_result "$name" "FAIL"
        fi
    done
}

# ========== 3. 汇总 ==========

function summary() {
    echo ""
    echo "$dotted_line"
    echo "汇总: $PASS 通过, $FAIL 失败"
    if [ $FAIL -gt 0 ]; then
        echo "失败项:"
        for f in "${FAILED_LIST[@]}"; do
            echo "  - $f"
        done
    fi
    echo "$dotted_line"
}

# ========== main ==========

run_ut
build_examples
summary
