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
#
# Build and verify every MX scale pathway scene under both A/B transpose modes in isolation.
# Each combination is a separate compile (TRANS_MODE=0/1 x SCENE_ID=0/1/2).
#   TRANS_MODE 0: A non-transposed (GM ND->L1 NZ->L0 NZ), B transposed (GM DN->L1 ZN->L0 ZN)
#   TRANS_MODE 1: A transposed     (GM DN->L1 ZN->L0 NZ), B non-transposed (GM ND->L1 NZ->L0 ZN)
#   scene 0: ScaleAND + ScaleBND   scene 1: ScaleADN + ScaleBDN   scene 2: ZZ + NN
#
# Usage: bash run.sh [run_mode] [arch]
#   run_mode: sim (default) | npu
#   arch:     dav-3510 (default)
# CANN env: source <cann>/set_env.sh before running, or set ASCEND_HOME_PATH.

set -e

RUN_MODE="${1:-sim}"
ARCH="${2:-dav-3510}"
SAMPLE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCENE_NAMES=("ScaleAND+ScaleBND" "ScaleADN+ScaleBDN" "ZZ+NN")
TRANS_NAMES=("A-nonT+B-T" "A-T+B-nonT")

cd "${SAMPLE_DIR}"

declare -A RESULTS
ALL_PASS=1
for TRANS_MODE in 0 1; do
    for SCENE_ID in 0 1 2; do
        TAG="trans${TRANS_MODE}_scene${SCENE_ID}"
        echo "==================== ${TRANS_NAMES[$TRANS_MODE]} | Scene ${SCENE_ID} (${SCENE_NAMES[$SCENE_ID]}) ===================="
        BUILD_DIR="build_${TAG}"
        rm -rf "${BUILD_DIR}"
        cmake -S . -B "${BUILD_DIR}" \
            -DCMAKE_ASC_RUN_MODE="${RUN_MODE}" \
            -DCMAKE_ASC_ARCHITECTURES="${ARCH}" \
            -DSCENE_ID="${SCENE_ID}" \
            -DTRANS_MODE="${TRANS_MODE}"
        cmake --build "${BUILD_DIR}" -j

        cd "${BUILD_DIR}"
        python3 ../scripts/gen_data.py "${TRANS_MODE}"
        ./demo
        if python3 ../scripts/verify_result.py output/output.bin output/golden.bin; then
            RESULTS[$TAG]="PASS"
        else
            RESULTS[$TAG]="FAIL"
            ALL_PASS=0
        fi
        cd "${SAMPLE_DIR}"
    done
done

echo "==================== Summary ===================="
for TRANS_MODE in 0 1; do
    for SCENE_ID in 0 1 2; do
        TAG="trans${TRANS_MODE}_scene${SCENE_ID}"
        echo "${TRANS_NAMES[$TRANS_MODE]} scene ${SCENE_ID} (${SCENE_NAMES[$SCENE_ID]}): ${RESULTS[$TAG]}"
    done
done

# Clean up per-combination build artifacts after results have been summarized.
echo "Cleaning up build artifacts..."
for TRANS_MODE in 0 1; do
    for SCENE_ID in 0 1 2; do
        rm -rf "${SAMPLE_DIR}/build_trans${TRANS_MODE}_scene${SCENE_ID}"
    done
done

if [ "${ALL_PASS}" -eq 1 ]; then
    echo "all scenes pass!"
    exit 0
else
    echo "some scenes failed."
    exit 1
fi
