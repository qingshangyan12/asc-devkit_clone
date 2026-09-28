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

# 用例执行入口：./run.sh <baseline|scene1|scene2|all|full|clean>
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
        [ ${rc} -eq 2 ] && echo "[OK] 注入场景按预期捕获异常（退出码 2）" || { echo "[FAIL] 注入场景未按预期捕获异常（退出码 ${rc}）"; return 1; }
    fi
}

clean_outputs() {
    local d
    for d in "${TEST_DIR}/task_fail_dump" "${SAMPLE_DIR}/python/task_fail_dump"; do
        if [ -d "$d" ]; then
            rm -rf "$d" || return 1
            echo "[OK] 已删除 $d"
        fi
    done
    for d in "${TEST_DIR}/extra-info" "${SAMPLE_DIR}/python/extra-info"; do
        if [ -d "$d" ]; then
            rm -rf "$d" 2>/dev/null && echo "[OK] 已删除 $d" || \
                echo "[WARN] $d 删除失败（可能需在容器内删除）"
        fi
    done
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
    clean)
        clean_outputs
        ;;
    *)
        echo "用法: $0 <baseline|scene1|scene2|all|full|clean>" >&2
        exit 2
        ;;
esac
