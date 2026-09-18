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

# 从 ops-transformer 外部仓（固定 commit）构建 IFA(+meta) wheel 并安装，算子零改动；
# 构建适配补丁仅弥合本地构建环境差异，与 dump 能力无关
# 环境变量：OPS_TRANSFORMER_ROOT 必填（仓根目录）；NPU_SOC_VERSION 必填（如 ascend910_93）；
#           IFA_BASELINE_COMMIT / SKIP_COMMIT_CHECK=1 可选（基线 commit 校验）
set -eu

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
PATCH="${SAMPLE_DIR}/test/patches/ops_transform_build_adapt.patch"
IFA_BASELINE_COMMIT_DEFAULT="ecfc11431fc6e78eaa4b40632845498813121afd"

if [ -z "${OPS_TRANSFORMER_ROOT:-}" ] || [ ! -d "${OPS_TRANSFORMER_ROOT}" ]; then
    echo "[FATAL] 请设置 OPS_TRANSFORMER_ROOT 为 ops-transformer 仓根目录" >&2
    exit 1
fi
if [ -z "${NPU_SOC_VERSION:-}" ]; then
    echo "[FATAL] 请设置 NPU_SOC_VERSION（如 ascend910_93）" >&2
    exit 1
fi
if [ -z "${ASCEND_HOME_PATH:-}" ]; then
    echo "[FATAL] ASCEND_HOME_PATH 未设置，请先 source CANN 环境脚本" >&2
    exit 1
fi

# ---- 1. 基线 commit 校验（外部仓版本改变必须重新执行完整回归）----
cd "${OPS_TRANSFORMER_ROOT}"
if [ "${SKIP_COMMIT_CHECK:-0}" != "1" ]; then
    BASELINE="${IFA_BASELINE_COMMIT:-${IFA_BASELINE_COMMIT_DEFAULT}}"
    CURRENT="$(git rev-parse HEAD)"
    if [ "${CURRENT}" != "${BASELINE}" ]; then
        echo "[FATAL] 外部仓 commit 不在验证基线上: 当前 ${CURRENT}，基线 ${BASELINE}" >&2
        echo "        如确需在其他 commit 上构建，设 SKIP_COMMIT_CHECK=1，且完成后必须重新执行完整回归" >&2
        exit 1
    fi
fi

# ---- 2. 临时 worktree 中构建（不动外部仓工作区）----
WORKTREE="$(mktemp -d "${OPS_TRANSFORMER_ROOT}/../ifa_build_XXXXXX")"
trap 'rm -rf "${WORKTREE}"' EXIT
git worktree add --detach "${WORKTREE}" HEAD >/dev/null
cd "${WORKTREE}/examples/fast_kernel_launch_example"

# ---- 3. 按需应用构建适配补丁（仅构建环境差异；与 dump 能力无关，算子逻辑零改动）----
if ! git apply --check "${PATCH}" 2>/dev/null; then
    if git diff --quiet && git log --oneline -1 >/dev/null 2>&1; then
        echo "[WARN] 构建适配补丁不可应用（可能已包含或环境不匹配），跳过；构建失败时人工核查"
    fi
else
    git apply "${PATCH}"
    echo "[OK] 已应用构建适配补丁"
fi

# ---- 4. 白名单只编 IFA 两个算子并安装 wheel ----
export OPS_BUILD_FILTER="incre_flash_attention,incre_flash_attention_meta"
python3 setup.py clean
python3 -m build --wheel --no-isolation
pip install dist/ascend_ops-*.whl --force-reinstall --no-deps

# ---- 5. 导入核查（从样例目录外导入，确认不被本地源码遮蔽）----
cd "${SAMPLE_DIR}"
python3 -c "import torch, ascend_ops; print('[OK] ascend_ops 导入正常:', torch.ops.ascend_ops.npu_fused_infer_attention_score is not None)"
echo "[OK] IFA wheel 构建安装完成"
