#!/bin/bash
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

CASE_REL=01_simd_cpp_api/03_basic_api/04_memory_management/tbufpool_management
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"


get_scenario_list() {
    echo "1 2 3"
}

get_build_subdir() {
    local scenario=$1
    echo "${BUILD_DIR}_scenario${scenario}"
}

case_build() {
    local scenario_failed=0
    for scenario in $(get_scenario_list); do
        local scenario_build_dir
        scenario_build_dir="$(get_build_subdir "$scenario")"
        mkdir -p "$scenario_build_dir"
        (
            set -e
            (cd "${scenario_build_dir}" && SCENARIO_NUM=$scenario soc_version=$SOC_VERSION bash -lc 'cmake -DSCENARIO_NUM=$SCENARIO_NUM .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
            (cd "${scenario_build_dir}" && SCENARIO_NUM=$scenario soc_version=$SOC_VERSION bash -lc 'make -j')
            (cd "${scenario_build_dir}" && SCENARIO_NUM=$scenario soc_version=$SOC_VERSION bash -lc 'python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM')
        ) && echo "场景${scenario}编译成功" || { echo "场景${scenario}编译失败" >&2; scenario_failed=1; }
    done
    return $scenario_failed
}

case_run() {
    local scenario_failed=0
    for scenario in $(get_scenario_list); do
        local scenario_build_dir
        scenario_build_dir="$(get_build_subdir "$scenario")"
        mkdir -p "$scenario_build_dir"
        (cd "${scenario_build_dir}" && SCENARIO_NUM=$scenario soc_version=$SOC_VERSION bash -lc ./demo) || scenario_failed=1
    done
    return $scenario_failed
}

case_verify() {
    local scenario_failed=0
    for scenario in $(get_scenario_list); do
        local scenario_build_dir
        scenario_build_dir="$(get_build_subdir "$scenario")"
        mkdir -p "$scenario_build_dir"
        (cd "${scenario_build_dir}" && SCENARIO_NUM=$scenario soc_version=$SOC_VERSION bash -lc 'python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin') && echo "场景${scenario}精度比对成功" || { echo "场景${scenario}精度比对失败" >&2; scenario_failed=1; }
    done
    return $scenario_failed
}

case_clean() {
    for scenario in $(get_scenario_list); do
        rm -rf "$(get_build_subdir "$scenario")"
    done
    presmoke_default_clean
}

presmoke_case_main "$@"
