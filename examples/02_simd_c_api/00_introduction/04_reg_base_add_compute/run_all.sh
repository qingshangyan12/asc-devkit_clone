#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# 一键执行 04_reg_base_add_compute 下所有样例，输出 pass/fail 日志
# 用法: bash run_all.sh
# ----------------------------------------------------------------------------------------------------------
set -euo pipefail

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
ARCH="${CMAKE_ASC_ARCHITECTURES:-dav-3510}"
SOC_VERSION="${SOC_VERSION:-Ascend910B2}"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOG_ROOT="$ROOT/logs/$TIMESTAMP"
SUMMARY_FILE="$LOG_ROOT/summary.txt"
mkdir -p "$LOG_ROOT"

PASS=0
FAIL_BUILD=0
FAIL_RUN=0
TOTAL=0

declare -a FAIL_BUILD_LIST=()
declare -a FAIL_RUN_LIST=()
declare -a PASS_LIST=()

echo "================================================"
echo " Run All 04_reg_base_add_compute Samples"
echo " ARCH       = $ARCH"
echo " SOC_VERSION= $SOC_VERSION"
echo " Log Root   = $LOG_ROOT"
echo "================================================"

{
    echo "Timestamp: $TIMESTAMP"
    echo "ARCH=$ARCH  SOC_VERSION=$SOC_VERSION"
    echo "================================================"
} >> "$SUMMARY_FILE"

# 收集所有含 CMakeLists.txt 的样例目录
shopt -s nullglob
SAMPLE_DIRS=()
while IFS= read -r -d '' dir; do
    SAMPLE_DIRS+=("$dir")
done < <(find "$ROOT" -name CMakeLists.txt -printf '%h\0' | sort -z)

TOTAL=${#SAMPLE_DIRS[@]}

for dir in "${SAMPLE_DIRS[@]}"; do
    REL_PATH="${dir#$ROOT/}"
    SAMPLE_NAME=$(basename "$dir")
    ((cnt=cnt+1))

    printf "[%3d/%3d] %-70s ... " "$cnt" "$TOTAL" "$REL_PATH"

    # ----- 创建日志目录 -----
    SAMPLE_LOG_DIR="$LOG_ROOT/$REL_PATH"
    mkdir -p "$SAMPLE_LOG_DIR"

    CMAKE_LOG="$SAMPLE_LOG_DIR/cmake.log"
    MAKE_LOG="$SAMPLE_LOG_DIR/make.log"
    RUN_LOG="$SAMPLE_LOG_DIR/run.log"

    # ----- 编译 -----
    rm -rf "$dir/build"
    mkdir -p "$dir/build"

    BUILD_OK=true
    cd "$dir/build"
    if ! cmake .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" > "$CMAKE_LOG" 2>&1; then
        BUILD_OK=false
    fi
    if $BUILD_OK && ! make -j > "$MAKE_LOG" 2>&1; then
        BUILD_OK=false
    fi

    if ! $BUILD_OK; then
        echo "BUILD FAIL"
        ((FAIL_BUILD++))
        FAIL_BUILD_LIST+=("$REL_PATH")
        echo "$REL_PATH : BUILD FAIL" >> "$SUMMARY_FILE"
        continue
    fi

    # ----- 提取可执行文件名 -----
    EXE=$(grep -oP 'add_executable\(\s*\K\S+' "$dir/CMakeLists.txt" | head -1)
    if [ -z "$EXE" ]; then
        echo "SKIP (no executable)"
        echo "$REL_PATH : SKIP - no add_executable" >> "$SUMMARY_FILE"
        continue
    fi

    # ----- 运行 -----
    RUN_OK=true
    if ! soc_version="$SOC_VERSION" ./"$EXE" > "$RUN_LOG" 2>&1; then
        RUN_OK=false
    fi

    if ! $RUN_OK; then
        echo "RUN FAIL"
        ((FAIL_RUN++))
        FAIL_RUN_LIST+=("$REL_PATH")
        echo "$REL_PATH : RUN FAIL" >> "$SUMMARY_FILE"
    else
        echo "PASS"
        ((PASS++))
        PASS_LIST+=("$REL_PATH")
        echo "$REL_PATH : PASS" >> "$SUMMARY_FILE"
    fi

    cd "$ROOT"
done

# ====================== 汇总输出 ======================
{
    echo ""
    echo "================================================"
    echo " RESULT: $TOTAL total | $PASS PASS | $FAIL_BUILD BUILD_FAIL | $FAIL_RUN RUN_FAIL"
    echo "================================================"
    echo ""
    echo "--- PASS ---"
    for item in "${PASS_LIST[@]}"; do echo "  $item"; done
    echo ""
    echo "--- BUILD FAIL ---"
    for item in "${FAIL_BUILD_LIST[@]}"; do echo "  $item"; done
    echo ""
    echo "--- RUN FAIL ---"
    for item in "${FAIL_RUN_LIST[@]}"; do echo "  $item"; done
} >> "$SUMMARY_FILE"

echo ""
echo "================================================"
echo " DONE: $TOTAL total | $PASS PASS | $FAIL_BUILD BUILD_FAIL | $FAIL_RUN RUN_FAIL"
echo " Logs  : $LOG_ROOT"
echo " Summary: $SUMMARY_FILE"
echo "================================================"