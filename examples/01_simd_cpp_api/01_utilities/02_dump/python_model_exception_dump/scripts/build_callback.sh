#!/usr/bin/env bash
# build_callback.sh — 编译 Exception Dump 回调库并校验 C ABI 符号
#
# 仅依赖 CANN 公开库/头（-lacl_rt -lascendcl），无 experiment / runtime 内部依赖。
# 产物 runtime/exception_dump_public.so 不入仓，新环境（或 CANN 升级后）必须重编译。
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
