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

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="${SCRIPT_DIR}/build"
REPEAT_COUNT=10
SCENARIOS=(0 1 2)

usage() {
    echo "Usage: bash $0 <dav-2201|dav-3510>"
}

if [ "$#" -ne 1 ]; then
    usage
    exit 1
fi

ASC_ARCH="$1"
if [ "${ASC_ARCH}" != "dav-2201" ] && [ "${ASC_ARCH}" != "dav-3510" ]; then
    echo "[ERROR] Unsupported NPU architecture: ${ASC_ARCH}"
    usage
    exit 1
fi

total_count=$((REPEAT_COUNT * ${#SCENARIOS[@]}))
current_count=0

for ((round = 1; round <= REPEAT_COUNT; round++)); do
    for scenario_num in "${SCENARIOS[@]}"; do
        current_count=$((current_count + 1))
        echo "========== [${current_count}/${total_count}] round=${round}, SCENARIO_NUM=${scenario_num}, arch=${ASC_ARCH} =========="

        # Each combination uses a clean build directory.
        rm -rf -- "${BUILD_DIR}"

        cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
            -DCMAKE_ASC_RUN_MODE=npu \
            -DCMAKE_ASC_ARCHITECTURES="${ASC_ARCH}" \
            -DSCENARIO_NUM="${scenario_num}"
        cmake --build "${BUILD_DIR}" --parallel

        (
            cd "${BUILD_DIR}"
            python3 "${SCRIPT_DIR}/scripts/gen_data.py" -scenarioNum "${scenario_num}"
            ./demo
            python3 "${SCRIPT_DIR}/scripts/verify_result.py" ./output/output.bin ./output/golden.bin
        )
    done
done

echo "[PASS] All ${total_count} combinations passed."
