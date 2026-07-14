#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# 一键执行 04_reg_base_add_compute 下所有样例，输出 pass/fail 日志
# 用法: bash run_all.sh
# ----------------------------------------------------------------------------------------------------------
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SCRIPT_DIR"

# ====================== CANN 环境 ======================
CANN_DIR="${CANN_DIR:-/usr/local/Ascend/cann}"
if [ -f "$CANN_DIR/set_env.sh" ]; then
    source "$CANN_DIR/set_env.sh"
else
    echo "WARN: CANN set_env.sh not found at $CANN_DIR, assuming env already set"
fi

# ====================== 配置 ======================
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOG_ROOT="$ROOT/logs/$TIMESTAMP"
SUMMARY_FILE="$LOG_ROOT/summary.txt"
mkdir -p "$LOG_ROOT"

PASS=0
FAIL=0
TOTAL=0

declare -a FAIL_LIST=()
declare -a PASS_LIST=()

echo "================================================"
echo " Run All 04_reg_base_add_compute Samples"
echo " Log Root   = $LOG_ROOT"
echo "================================================"

{
    echo "Timestamp: $TIMESTAMP"
    echo "================================================"
} > "$SUMMARY_FILE"

# 收集所有独立 run.sh（排除主脚本自身，只找子目录中的）
shopt -s nullglob
RUN_SCRIPTS=()
while IFS= read -r -d '' f; do
    RUN_SCRIPTS+=("$f")
done < <(find "$ROOT" -mindepth 2 -name run.sh -print0 | sort -z)

TOTAL=${#RUN_SCRIPTS[@]}
cnt=0

for run_sh in "${RUN_SCRIPTS[@]}"; do
    sample_dir="$(dirname "$run_sh")"
    REL_PATH="${sample_dir#$ROOT/}"
    ((cnt++))

    printf "[%3d/%3d] %-70s ... " "$cnt" "$TOTAL" "$REL_PATH"

    # ----- 创建日志目录 -----
    SAMPLE_LOG_DIR="$LOG_ROOT/$REL_PATH"
    mkdir -p "$SAMPLE_LOG_DIR"

    RUN_LOG="$SAMPLE_LOG_DIR/run.log"

    # ----- 执行独立 run.sh，捕获全部输出 -----
    cd "$sample_dir"
    if bash "$run_sh" > "$RUN_LOG" 2>&1; then
        echo "PASS"
        ((PASS++))
        PASS_LIST+=("$REL_PATH")
        echo "$REL_PATH : PASS" >> "$SUMMARY_FILE"
    else
        echo "FAIL"
        ((FAIL++))
        FAIL_LIST+=("$REL_PATH")
        echo "$REL_PATH : FAIL" >> "$SUMMARY_FILE"
    fi
done

# ====================== 汇总输出 ======================
{
    echo ""
    echo "================================================"
    echo " RESULT: $TOTAL total | $PASS PASS | $FAIL FAIL"
    echo "================================================"
    echo ""
    echo "--- PASS ---"
    for item in "${PASS_LIST[@]}"; do echo "  $item"; done
    echo ""
    echo "--- FAIL ---"
    for item in "${FAIL_LIST[@]}"; do echo "  $item"; done
} >> "$SUMMARY_FILE"

echo ""
echo "================================================"
echo " DONE: $TOTAL total | $PASS PASS | $FAIL FAIL"
echo " Summary: $SUMMARY_FILE"
echo "================================================"