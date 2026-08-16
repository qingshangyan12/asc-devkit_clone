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

set -euo pipefail

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${TEST_DIR}/../../../.." && pwd)"
SKILL_FILE="${REPO_ROOT}/.agent/skills/asc-deprecated-api/SKILL.md"
REFERENCE_FILE="${REPO_ROOT}/.agent/skills/asc-deprecated-api/references/c-api-documentation-pattern.md"
AGENT_README="${REPO_ROOT}/.agent/README.md"

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

expect_fixed() {
    local file="$1"
    local text="$2"
    local label="$3"
    grep -Fq -- "$text" "$file" || fail "missing content: ${label}"
}

expect_fixed "$SKILL_FILE" "文档整理" "documentation-only stage"
expect_fixed "$SKILL_FILE" "标记废弃" "deprecation stage"
expect_fixed "$SKILL_FILE" "最终删除" "removal stage"
expect_fixed "$SKILL_FILE" "不执行删除" "default no-removal gate"
expect_fixed "$SKILL_FILE" '[[deprecated(' "compiler deprecation marker"
expect_fixed "$SKILL_FILE" "保留原公开签名和实现" "source compatibility"
expect_fixed "$SKILL_FILE" "Validate The Replacement Contract" "replacement validation"
expect_fixed "$SKILL_FILE" "deprecated_interface/deprecated_interface.md" "deprecated navigation"
expect_fixed "$SKILL_FILE" "docs/zh/api/appendix/deprecated_interface.md" "deprecated appendix"
expect_fixed "$SKILL_FILE" "从正常功能分类" "normal category removal"
expect_fixed "$SKILL_FILE" "产品与架构支持范围" "product support validation"
expect_fixed "$SKILL_FILE" "ABI/API 兼容性" "compatibility removal gate"
expect_fixed "$SKILL_FILE" "不要声称未执行" "verification honesty"
expect_fixed "$SKILL_FILE" "../asc-ar-dev/SKILL.md" "asc-ar-dev routing"
expect_fixed "$SKILL_FILE" "../asc-npu-arch/SKILL.md" "asc-npu-arch routing"

expect_fixed "$REFERENCE_FILE" "两种迁移起点" "migration entry patterns"
expect_fixed "$REFERENCE_FILE" "导航迁移矩阵" "navigation matrix"
expect_fixed "$REFERENCE_FILE" "组合替代" "composed replacement"
expect_fixed "$REFERENCE_FILE" "不自动产生编译告警" "documentation boundary"
expect_fixed "$AGENT_README" '`asc-deprecated-api`' "skill registry"
expect_fixed "$AGENT_README" "bash .agent/tests/skills/asc-deprecated-api/test.sh" "skill test command"

echo "[PASS] asc-deprecated-api skill content validation passed"

