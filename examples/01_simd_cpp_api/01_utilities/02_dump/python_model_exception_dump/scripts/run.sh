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

# 用例执行入口：./run.sh <baseline|scene1|scene2|all|full>（full = scene1+verify+parse+all+baseline）
# 判定以日志与 dump 产物为准，注入场景退出码 2 属预期成功路径
set -u

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
TEST_DIR="${SAMPLE_DIR}/test"
MODE="${1:-scene1}"

run_one() {
    echo "===== test_public_dump.py $1 ====="
    python3 "${TEST_DIR}/test_public_dump.py" "$1"
    rc=$?
    if [ "$1" = "baseline" ]; then
        [ ${rc} -eq 0 ] && echo "[OK] baseline 正常结束（退出码 0）" || { echo "[FAIL] baseline 退出码 ${rc}"; return 1; }
    else
        { [ ${rc} -eq 0 ] || [ ${rc} -eq 2 ]; } && echo "[OK] 注入场景完成（退出码 ${rc}，2 为预期路径）" || { echo "[FAIL] 注入场景退出码 ${rc}"; return 1; }
    fi
}

case "${MODE}" in
    scene1|scene2|baseline|all)
        run_one "${MODE}"
        ;;
    full)
        # 场景分进程执行（D3 的 all 例外：同进程验证 per-binary 接管）
        run_one scene1   || exit 1
        echo "===== verify_dump.py ====="
        python3 "${SAMPLE_DIR}/python/verify_dump.py" || exit 1
        echo "===== parse_tiling.py ====="
        python3 "${SAMPLE_DIR}/python/parse_tiling.py" || exit 1
        run_one all      || exit 1
        run_one baseline || exit 1
        echo "[RESULT] 完整回归执行完成；产物与判定明细见上方日志"
        ;;
    *)
        echo "用法: $0 <baseline|scene1|scene2|all|full>" >&2
        exit 1
        ;;
esac
