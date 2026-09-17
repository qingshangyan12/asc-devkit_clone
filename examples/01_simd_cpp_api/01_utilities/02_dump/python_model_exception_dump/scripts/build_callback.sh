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

# 编译 Exception Dump 回调库并校验 C ABI 符号（产物 runtime/*.so 不入仓，新环境必须重编译）
set -eu

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC="${SAMPLE_DIR}/runtime/exception_dump_public.cpp"
OUT="${SAMPLE_DIR}/runtime/exception_dump_public.so"

if [ -z "${ASCEND_HOME_PATH:-}" ]; then
    echo "[FATAL] ASCEND_HOME_PATH 未设置，请先 source <CANN>/bin/setenv.bash 或 ascend-toolkit/set_env.sh" >&2
    exit 1
fi

echo "[1/2] 编译回调库 -> ${OUT}"
g++ -g -std=c++11 -fPIC -shared "${SRC}" -o "${OUT}" \
    -lacl_rt -lascendcl \
    -L"${ASCEND_HOME_PATH}/lib64" -I"${ASCEND_HOME_PATH}/include"

echo "[2/2] 校验 C ABI 符号（预期恰好 3 个）"
nm -D "${OUT}" | grep -E ' T '
count=$(nm -D "${OUT}" | grep -cE ' T (AddInputInfo|ClearInputInfo|RegisterPublicExceptionDump)$')
if [ "${count}" -ne 3 ]; then
    echo "[FATAL] C ABI 符号数量异常: ${count}（预期 3）" >&2
    exit 1
fi
if nm -D "${OUT}" | grep -qE ' T RegisterBySymbol'; then
    echo "[FATAL] 不应存在的符号 RegisterBySymbol" >&2
    exit 1
fi
echo "[OK] 编译与符号校验通过"
