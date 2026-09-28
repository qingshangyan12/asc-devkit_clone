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

set -u

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

usage() {
    echo "用法: $0 <check|callback|ifa>" >&2
    exit 2
}

check_env() {
    local fail=0 header
    if [ -z "${ASCEND_HOME_PATH:-}" ]; then
        echo "[MISS] ASCEND_HOME_PATH 未设置" >&2
        fail=1
    else
        header="${ASCEND_HOME_PATH}/include/acl/acl_rt.h"
        if [ ! -f "$header" ]; then
            header="${ASCEND_HOME_PATH}/aarch64-linux/include/acl/acl_rt.h"
        fi
        if [ ! -f "$header" ] || ! grep -q aclrtExceptionInfoCallbackRegister "$header"; then
            echo "[MISS] CANN 头文件无 aclrtExceptionInfoCallbackRegister: $header" >&2
            fail=1
        fi
    fi
    if ! command -v npu-smi >/dev/null 2>&1 || ! npu-smi info 2>/dev/null | grep -q 910; then
        echo "[MISS] 未检测到 Ascend 910 NPU" >&2
        fail=1
    fi
    if ! python3 -c 'import torch, torch_npu, ascend_ops' 2>/dev/null; then
        echo "[MISS] 缺少 torch、torch_npu 或 ascend_ops" >&2
        fail=1
    fi
    if [ ! -f "${SAMPLE_DIR}/runtime/exception_dump_public.so" ]; then
        echo "[TODO] 回调库未编译；运行 $0 callback"
    fi
    [ "$fail" -eq 0 ] && echo "[OK] 环境检查通过"
    return "$fail"
}

build_callback() {
    if [ -z "${ASCEND_HOME_PATH:-}" ]; then
        echo "[FATAL] 请先设置 ASCEND_HOME_PATH" >&2
        return 1
    fi
    local out="${SAMPLE_DIR}/runtime/exception_dump_public.so"
    g++ -g -std=c++11 -fPIC -shared "${SAMPLE_DIR}/runtime/exception_dump_public.cpp" -o "$out" \
        -lacl_rt -lascendcl -L"${ASCEND_HOME_PATH}/lib64" -I"${ASCEND_HOME_PATH}/include" || return
    local count
    count=$(nm -D "$out" | grep -cE ' T (AddInputInfo|ClearInputInfo|RegisterPublicExceptionDump)$')
    if [ "$count" -ne 3 ] || nm -D "$out" | grep -qE ' T RegisterBySymbol$'; then
        echo "[FATAL] 回调库 C ABI 符号校验失败" >&2
        return 1
    fi
    echo "[OK] 回调库已编译并通过符号校验: $out"
}

build_ifa() (
    set -eu
    if [ -z "${OPS_TRANSFORMER_ROOT:-}" ] || [ ! -d "$OPS_TRANSFORMER_ROOT" ]; then
        echo "[FATAL] 请设置 OPS_TRANSFORMER_ROOT 为 ops-transformer 仓根目录" >&2
        exit 1
    fi
    : "${NPU_SOC_VERSION:?请设置 NPU_SOC_VERSION（如 ascend910_93）}"
    : "${ASCEND_HOME_PATH:?请先设置 ASCEND_HOME_PATH}"

    cd "$OPS_TRANSFORMER_ROOT"
    local baseline="${IFA_BASELINE_COMMIT:-ecfc11431fc6e78eaa4b40632845498813121afd}"
    if [ "${SKIP_COMMIT_CHECK:-0}" != 1 ] && [ "$(git rev-parse HEAD)" != "$baseline" ]; then
        echo "[FATAL] ops-transformer commit 与验证基线 $baseline 不一致；如需覆盖请设置 SKIP_COMMIT_CHECK=1" >&2
        exit 1
    fi

    local worktree
    worktree=$(mktemp -d "${OPS_TRANSFORMER_ROOT}/../ifa_build_XXXXXX")
    trap 'cd "$OPS_TRANSFORMER_ROOT"; git worktree remove --force "$worktree" 2>/dev/null || rmdir "$worktree" 2>/dev/null || true' EXIT
    git worktree add --detach "$worktree" HEAD >/dev/null
    cd "$worktree/examples/fast_kernel_launch_example"

    local patch="${SAMPLE_DIR}/test/patches/ops_transform_build_adapt.patch"
    if git apply --check "$patch" 2>/dev/null; then
        git apply "$patch"
    else
        echo "[WARN] 构建适配补丁不可应用，继续尝试构建" >&2
    fi
    export OPS_BUILD_FILTER="incre_flash_attention,incre_flash_attention_meta"
    python3 setup.py clean
    python3 -m build --wheel --no-isolation
    pip install dist/ascend_ops-*.whl --force-reinstall --no-deps
    cd "$SAMPLE_DIR"
    python3 -c "import torch, ascend_ops; print('[OK] ascend_ops 导入正常:', torch.ops.ascend_ops.npu_fused_infer_attention_score is not None)"
)

case "${1:-}" in
    check) check_env ;;
    callback) build_callback ;;
    ifa) build_ifa ;;
    *) usage ;;
esac
