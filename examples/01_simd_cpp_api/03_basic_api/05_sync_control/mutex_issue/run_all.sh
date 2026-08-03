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

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

for scenario_num in 0 1 2 3 4; do
    build_dir="$SCRIPT_DIR/build_scenario_${scenario_num}"
    cmake -S "$SCRIPT_DIR" -B "$build_dir" \
        -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
        -DSCENARIO_NUM="$scenario_num"
    cmake --build "$build_dir" -j
    (
        cd "$build_dir"
        python3 ../scripts/gen_data.py -scenarioNum "$scenario_num"
        ./demo
        python3 ../scripts/verify_result.py output/output.bin output/golden.bin
    )
done
