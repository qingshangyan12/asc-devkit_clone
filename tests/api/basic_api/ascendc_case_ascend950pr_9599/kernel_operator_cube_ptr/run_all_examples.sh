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
# 端到端跑 14 个 CUBE 样例：造数(gen_data.py) -> 编译(cmake+make) -> 运行(./demo) -> 校验(verify_result.py)。
# 这些样例的 .asc 已经是指针化版本（LoadData/Mmad/Fixpipe 用裸硬件指针调用），因此本脚本同时充当
# 「指针路径端到端自测」。样例源码即 examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/<name>/。
#
# 用法:
#   source /usr/local/Ascend/cann/set_env.sh
#   bash run_all_examples.sh [all|dav-3510|dav-2201]
#
# 说明:
#   - dav-3510 样例在 Ascend 950PR/950DT 上以 NPU 模式运行；
#   - dav-2201 样例需要 A2/A3，950PR 机器上请用 RUN_MODE_2201=cpu（或 sim）；
#   - 需要 CANN >= 9.2.0（样例与仓库头文件使用 fp8_e4m3_t / float4_e2m1_t / __nop 等）。
#
set -u

HERE=$(cd "$(dirname "$0")" && pwd)
REPO="${REPO:-$(cd "$HERE/../../../../.." && pwd)}"
CM="$REPO/examples/01_simd_cpp_api/03_basic_api/03_matrix_compute"
FILTER="${1:-all}"
RUN_MODE_2201="${RUN_MODE_2201:-cpu}"
LOGD="${LOGD:-$PWD/cube_examples_logs}"
mkdir -p "$LOGD"

[ -d "$CM" ] || { echo "找不到样例目录 $CM"; exit 2; }

SAMPLES=$(cat <<'EOF'
batch_matmul dav-2201 1
fixpipe_l0c2gm dav-2201 1
fixpipe_l0c2l1 dav-2201 1
fixpipe_l0c2ub dav-3510 1
load_data_2dmx_l12l0 dav-3510 1
load_data_2dv2_l12l0 dav-3510 1
load_data_l12l0 dav-2201 1
load_data_with_stride dav-3510 1
mmad dav-2201 1
mmad_gemv dav-2201 1
mmad_load3dv2 dav-2201 4
mmad_mx dav-3510 1
mmad_unitflag dav-2201 1
mmad_with_sparse dav-2201 1
EOF
)

printf '%-24s %-9s %-5s %-6s %-6s %-6s %-6s\n' sample arch scen build run verify note > "$LOGD/summary.txt"
npass=0; nfail=0
while read -r n a s; do
    [ -n "${n:-}" ] || continue
    if [ "$FILTER" != "all" ] && [ "$FILTER" != "$a" ]; then continue; fi
    d="$CM/$n"
    [ -d "$d" ] || { echo "SKIP $n（目录不存在）"; continue; }
    runmode=npu
    [ "$a" = "dav-2201" ] && runmode="$RUN_MODE_2201"

    rm -rf "$d/build"
    mkdir -p "$d/build"
    ( cd "$d/build" && cmake -DCMAKE_ASC_ARCHITECTURES="$a" -DCMAKE_ASC_RUN_MODE="$runmode" \
        -DSCENARIO_NUM="$s" .. ) > "$LOGD/$n.cmake.log" 2>&1
    rc_cm=$?
    ( cd "$d/build" && make -j ) > "$LOGD/$n.make.log" 2>&1
    rc_mk=$?
    ( cd "$d" && python3 scripts/gen_data.py -scenarioNum="$s" ) > "$LOGD/$n.gen.log" 2>&1
    rc_gen=$?
    ( cd "$d/build" && ./demo ) > "$LOGD/$n.run.log" 2>&1
    rc_run=$?
    ( cd "$d/build" && python3 ../scripts/verify_result.py -scenarioNum="$s" \
        output/output.bin output/golden.bin ) > "$LOGD/$n.verify.log" 2>&1
    rc_vf=$?

    note=""
    [ "$rc_cm" != 0 ] && note="cmake失败"
    [ "$rc_cm" = 0 ] && [ "$rc_mk" != 0 ] && note="编译失败"
    [ "$rc_mk" = 0 ] && [ "$rc_run" != 0 ] && note="运行失败"
    [ "$rc_run" = 0 ] && [ "$rc_vf" != 0 ] && note="结果不一致"
    if [ "$rc_cm" = 0 ] && [ "$rc_mk" = 0 ] && [ "$rc_run" = 0 ] && [ "$rc_vf" = 0 ]; then
        note="PASS"; npass=$((npass+1))
    else
        nfail=$((nfail+1))
    fi
    printf '%-24s %-9s %-5s %-6s %-6s %-6s %-6s\n' "$n" "$a" "$s" \
        "$rc_cm/$rc_mk" "$rc_gen" "$rc_run" "$rc_vf" "$note" | tee -a "$LOGD/summary.txt"
done <<EOF2
$SAMPLES
EOF2

echo ""
echo "PASS=${npass} FAIL=${nfail}  日志目录: ${LOGD}"
[ "$nfail" = 0 ]