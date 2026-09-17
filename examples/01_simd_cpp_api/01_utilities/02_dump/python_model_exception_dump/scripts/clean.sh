#!/usr/bin/env bash
# clean.sh — 清理本轮运行产物
#
# 只清理本样例创建的 task_fail_dump 与 CANN 崩溃日志 extra-info/，不删除外部仓
# 已有文件，也不删除已编译的回调库（如需重编执行 scripts/build_callback.sh）。
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
