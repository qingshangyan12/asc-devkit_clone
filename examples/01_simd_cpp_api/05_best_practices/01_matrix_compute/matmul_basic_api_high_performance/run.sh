#!/usr/bin/env bash
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
#
# 编译/运行方式：
#   ./run.sh <case_num|all> [dav-2201|dav-3510] [build|run]
#
# 示例：
#   ./run.sh 0 dav-2201 build   # 仅编译Case 0，不运行
#   ./run.sh 7 dav-3510 run     # 编译、运行并校验Case 7
#   ./run.sh all dav-2201 build # 依次编译dav-2201下的全部Case，不运行
#
# Case说明：
#   0: 单核基础版本，base=[64, 64, 64]
#   1: 单核base块优化
#   2: 多核基础切分（dav-2201为2x12，dav-3510为2x16）
#   3: 多核切分优化（dav-2201为4x6，dav-3510为4x8）
#   4: Double Buffer优化
#   5: 大包搬运优化
#   6: L2Cache优化
#   7: UnitFlag优化
#
# 可通过CANN_INSTALL_PATH覆盖默认CANN安装目录：
#   CANN_INSTALL_PATH=/path/to/cann ./run.sh 0 dav-2201 build

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CANN_INSTALL_PATH="${CANN_INSTALL_PATH:-/mnt/workspace/z00940346/cann530/cann-9.1.0}"
CASE_ARG="${1:-}"
ASC_ARCH="${2:-dav-2201}"
RUN_MODE="${3:-build}"

usage()
{
    sed -n '3,23{s/^# \{0,1\}//;p}' "$0"
}

if [[ "$CASE_ARG" == "-h" || "$CASE_ARG" == "--help" ]]; then
    usage
    exit 0
fi

if [[ -z "$CASE_ARG" ]]; then
    usage
    exit 2
fi

if [[ "$CASE_ARG" != "all" && ! "$CASE_ARG" =~ ^[0-7]$ ]]; then
    echo "错误：case_num必须是0-7或all，当前值为：${CASE_ARG}" >&2
    exit 2
fi

case "$ASC_ARCH" in
    dav-2201 | dav-3510) ;;
    *)
        echo "错误：架构必须是dav-2201或dav-3510，当前值为：${ASC_ARCH}" >&2
        exit 2
        ;;
esac

case "$RUN_MODE" in
    build | run) ;;
    *)
        echo "错误：模式必须是build或run，当前值为：${RUN_MODE}" >&2
        exit 2
        ;;
esac

if [[ ! -f "${CANN_INSTALL_PATH}/set_env.sh" ]]; then
    echo "错误：未找到${CANN_INSTALL_PATH}/set_env.sh，请通过CANN_INSTALL_PATH指定CANN安装目录。" >&2
    exit 2
fi

# CANN环境脚本可能读取尚未定义的变量，因此在source完成后再开启nounset。
source "${CANN_INSTALL_PATH}/set_env.sh"
set -u

CASE_NAMES=(
    "单核基础版本"
    "单核base块优化"
    "多核基础切分"
    "多核切分优化"
    "Double Buffer优化"
    "大包搬运优化"
    "L2Cache优化"
    "UnitFlag优化"
)

DATA_READY=false

build_case()
{
    local case_num="$1"
    local build_dir="${SCRIPT_DIR}/build/case${case_num}_${ASC_ARCH}"

    echo "[INFO] Case ${case_num}: ${CASE_NAMES[case_num]}"
    echo "[INFO] Architecture: ${ASC_ARCH}"
    echo "[INFO] Build directory: ${build_dir}"

    cmake \
        -S "$SCRIPT_DIR" \
        -B "$build_dir" \
        -DCMAKE_ASC_ARCHITECTURES="$ASC_ARCH" \
        -DSCENARIO_NUM="$case_num"
    cmake --build "$build_dir" --parallel

    if [[ "$RUN_MODE" == "run" ]]; then
        if [[ "$DATA_READY" == "false" ]]; then
            echo "[INFO] Generating input and golden data"
            (
                cd "$SCRIPT_DIR"
                python3 scripts/gen_data.py
            )
            DATA_READY=true
        fi
        echo "[INFO] Running Case ${case_num}"
        (
            cd "$SCRIPT_DIR"
            "$build_dir/demo"
            python3 scripts/verify_result.py output/output.bin output/golden.bin
        )
    fi
}

if [[ "$CASE_ARG" == "all" ]]; then
    for case_num in {0..7}; do
        build_case "$case_num"
    done
else
    build_case "$CASE_ARG"
fi
