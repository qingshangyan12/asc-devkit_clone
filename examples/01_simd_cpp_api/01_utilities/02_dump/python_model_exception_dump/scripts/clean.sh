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

# 清理本轮运行产物（task_fail_dump/ 与 extra-info/；不动外部仓文件与已编译的回调库）
set -u

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

for d in "${SAMPLE_DIR}/test/task_fail_dump" "${SAMPLE_DIR}/python/task_fail_dump"; do
    if [ -d "${d}" ]; then
        rm -rf "${d}"
        echo "[OK] 已删除 ${d}"
    fi
done

# AICore error 时 CANN 在 CWD 自动生成的崩溃日志目录，属正常副产物而非 dump 交付物
for d in "${SAMPLE_DIR}/test/extra-info" "${SAMPLE_DIR}/python/extra-info"; do
    if [ -d "${d}" ]; then
        rm -rf "${d}" 2>/dev/null && echo "[OK] 已删除 ${d}" || \
            echo "[WARN] ${d} 删除失败（docker 内 root 生成时需在容器内删除）"
    fi
done

echo "[OK] 清理完成"
