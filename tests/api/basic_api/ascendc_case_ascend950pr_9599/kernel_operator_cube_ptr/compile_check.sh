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
# 校验 CUBE Basic API 指针重载的签名（21 种指针调用形态）。
#
# ASC 驱动会把「已安装的 asc-devkit」硬编码进头文件搜索路径且优先级最高，因此直接 -isystem/-iquote 指向
# 源码树会编译到另一棵树（-isystem 无效；-iquote 会两棵树混用）。本脚本用「符号链接影子根 +
# ASCEND_HOME_PATH」让驱动去读本仓库的 include/impl，且不修改共享的 CANN 安装。
#
# 用法:
#   source /usr/local/Ascend/cann/set_env.sh      # 提供 ccec 与 ASCEND_HOME_PATH
#   bash compile_check.sh [dav-3510|dav-2201|all] [repo_root]
#
set -u

HERE=$(cd "$(dirname "$0")" && pwd)
ARCH="${1:-all}"
REPO="${2:-$(cd "$HERE/../../../../.." && pwd)}"

CCEC="${CCEC:-$(command -v ccec || true)}"
[ -n "$CCEC" ] || CCEC=/usr/local/Ascend/cann-9.1.0/bin/ccec
[ -x "$CCEC" ] || { echo "找不到 ccec，请先 source <cann>/set_env.sh"; exit 2; }
: "${ASCEND_HOME_PATH:?请先 source <cann>/set_env.sh 以设置 ASCEND_HOME_PATH}"
CANN_ROOT="${ASCEND_HOME_PATH%/}"
ASC="${CANN_ROOT}/asc"
[ -d "$ASC" ] || { echo "找不到 ${ASC}"; exit 2; }
[ -d "$REPO/include" ] && [ -d "$REPO/impl" ] || { echo "repo_root 不对: $REPO"; exit 2; }

case "$ARCH" in
    all) ARCH_LIST="dav-3510 dav-2201" ;;
    *)   ARCH_LIST="$ARCH" ;;
esac

WORK=$(mktemp -d)
cleanup(){ rm -rf "$WORK"; }
trap cleanup EXIT

SH="$WORK/shadow/cann"
mkdir -p "$SH"
for d in "$CANN_ROOT"/*; do ln -sfn "$d" "$SH/$(basename "$d")"; done
rm -f "$SH/asc"
mkdir -p "$SH/asc"
for d in "$ASC"/*; do ln -sfn "$d" "$SH/asc/$(basename "$d")"; done

# 安全检查：asc/include、asc/impl 必须是指向真实安装的符号链接，否则下面的 rm -f 有可能删到真实目录。
if [ ! -L "$SH/asc/include" ] || [ ! -L "$SH/asc/impl" ]; then
    echo "shadow 构建异常（asc/include 或 asc/impl 不是符号链接），中止以免误删真实安装"
    exit 3
fi
rm -f "$SH/asc/include" "$SH/asc/impl"
cp -r "$REPO/include" "$SH/asc/include"
cp -r "$REPO/impl" "$SH/asc/impl"

cp "$HERE/test_operator_cube_ptr.cpp" "$WORK/t.asc"

rc=0
for a in $ARCH_LIST; do
    echo "== ${a} =="
    if ASCEND_HOME_PATH="$SH" "$CCEC" --asc-aicore-lang -fsyntax-only --npu-arch="$a" \
            -ferror-limit=0 "$WORK/t.asc"; then
        echo "PASS ${a}: 21 种指针调用形态全部解析"
    else
        echo "FAIL ${a}"
        rc=1
    fi
done
exit $rc