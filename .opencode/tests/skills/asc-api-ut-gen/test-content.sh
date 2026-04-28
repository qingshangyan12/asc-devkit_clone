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
SKILL_DIR="${SKILL_DIR:-${REPO_ROOT}/.opencode/skills/asc-api-ut-gen}"
SKILL_FILE="${SKILL_DIR}/SKILL.md"
REFERENCES_DIR="${SKILL_DIR}/references"
GENERATOR="${SKILL_DIR}/scripts/ut_generator.py"
CLI="${SKILL_DIR}/scripts/ut_generator_cli.py"

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

expect_pattern() {
    local file="$1"
    local pattern="$2"
    local label="$3"

    grep -Eq "$pattern" "$file" || fail "missing content: ${label}"
}

expect_fixed() {
    local file="$1"
    local text="$2"
    local label="$3"

    grep -Fq -- "$text" "$file" || fail "missing content: ${label}"
}

expect_pattern "$SKILL_FILE" '^description:.*(当用户需要|触发).*' "description trigger condition"
expect_pattern "$SKILL_FILE" '^description:.*(UT|单元测试|覆盖率).*' "description keywords"
expect_fixed "$SKILL_FILE" "本技能使用 Python 脚本自动生成 UT 代码" "Python generator statement"
expect_fixed "$SKILL_FILE" "首次使用环境配置" "first-use environment gate"
expect_fixed "$SKILL_FILE" "CANN 包安装路径" "CANN package path requirement"
expect_fixed "$SKILL_FILE" "asc-devkit 代码仓路径" "asc-devkit repository path requirement"
expect_fixed "$SKILL_FILE" "禁止跳过此步骤" "no-skip environment gate"

expect_fixed "$SKILL_FILE" "Git 扫描模式" "git scan mode"
expect_fixed "$SKILL_FILE" "精确交互模式" "precise interaction mode"
expect_fixed "$SKILL_FILE" "覆盖率扫描模式" "coverage scan mode"
expect_fixed "$SKILL_FILE" "覆盖率报告补齐模式" "coverage report backfill mode"
expect_fixed "$SKILL_FILE" "/asc-api-ut-gen <芯片版本> <API类型> <API名称> [核心类型]" "exact command format"
expect_fixed "$SKILL_FILE" "build/cov_report" "coverage report path"

expect_fixed "$SKILL_FILE" "高阶API" "advanced API coverage"
expect_fixed "$SKILL_FILE" "membase基础API" "membase API coverage"
expect_fixed "$SKILL_FILE" "regbase基础API" "regbase API coverage"
expect_fixed "$SKILL_FILE" "C API" "C API coverage"
expect_fixed "$SKILL_FILE" "SIMT API" "SIMT API coverage"
expect_fixed "$SKILL_FILE" "工具类API" "utils API coverage"
expect_fixed "$SKILL_FILE" "../asc-npu-arch/SKILL.md" "npu arch source link"
expect_fixed "$SKILL_FILE" "统一事实来源" "single source statement"
expect_fixed "$SKILL_FILE" "本 skill 不再单独维护完整芯片类型表" "chip table centralization"
expect_fixed "$SKILL_FILE" "统一数据类型视图" "dtype source statement"

expect_pattern "$SKILL_FILE" 'regbase.*ascend950pr_9599.*mc62cm12aa' "regbase architecture restriction"
expect_pattern "$SKILL_FILE" 'SIMT API.*ascend950pr_9599' "SIMT architecture restriction"

if grep -Fq '**芯片架构：** `ascend910`, `ascend910b1`, `ascend310p`, `ascend610`, `ascend310b1`, `ascend950pr_9599`, `mc62cm12aa`' "$SKILL_FILE"; then
    fail "asc-api-ut-gen skill must not keep a duplicated full chip list"
fi

if grep -Fq '**数据类型：** `half`, `float`, `int8_t`, `int16_t`, `int32_t`, `uint8_t`, `uint16_t`, `uint32_t`, `bfloat16_t`' "$SKILL_FILE"; then
    fail "asc-api-ut-gen skill must not keep a duplicated full dtype list"
fi

if grep -REq '^\|[[:space:]]*数据类型[[:space:]]*\|[[:space:]]*sizeof[[:space:]]*\|[[:space:]]*(32B 对齐元素数|元素对齐要求)' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen references must not keep duplicated dtype size tables"
fi

if grep -REq '^\|[[:space:]]*数据类型[[:space:]]*\|[[:space:]]*(说明|sizeof[[:space:]]*\|[[:space:]]*对齐要求)[[:space:]]*\|' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen references must not keep duplicated generic dtype tables"
fi

if grep -REq '^\|[[:space:]]*参数类型[[:space:]]*\|[[:space:]]*常见组合[[:space:]]*\|' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen references must not keep duplicated common dtype combination tables"
fi

if grep -REq '支持类型:[[:space:]]*half,[[:space:]]*float|[,{][[:space:]]*[0-9]+[[:space:]]*,[[:space:]]*kernel_func_(half|float)' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen references must not hard-code generic dtype support or dtype sizes in templates"
fi

if grep -REq --exclude='test-templates.md' '^## [0-9]+\. 测试模板示例|^### [0-9]+\.[0-9]+ (基础|参数化)测试模板' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen scenario references must link test-templates.md instead of duplicating template sections"
fi

if grep -REq --exclude='automation-guide.md' 'source \{CANN_PATH\}/set_env\.sh|bash build\.sh|--gtest_filter' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen scenario references must link automation-guide.md instead of duplicating build/test commands"
fi

if grep -REq '数据类型大小来源|float \(4 bytes\)|half \(2 bytes\)|count %[[:space:]]*[0-9]+[[:space:]]*==' "$REFERENCES_DIR"; then
    fail "asc-api-ut-gen references must not duplicate dtype size or alignment facts"
fi

expect_fixed "$SKILL_FILE" "scripts/ut_generator_cli.py" "CLI generator reference"
expect_fixed "$SKILL_FILE" "scripts/ut_generator.py" "core generator reference"
expect_fixed "$SKILL_FILE" "分支覆盖分析指南" "branch coverage guide"
expect_fixed "$SKILL_FILE" "覆盖率报告补齐指南" "coverage report backfill guide"
expect_fixed "$SKILL_FILE" "常见问题汇总" "FAQ guide"
expect_fixed "${REFERENCES_DIR}/README.md" "目录划分" "reference directory navigation"
expect_fixed "${REFERENCES_DIR}/README.md" "api-guides/" "API guide reference folder"
expect_fixed "${REFERENCES_DIR}/README.md" "workflows/" "workflow reference folder"
expect_fixed "${REFERENCES_DIR}/README.md" "foundations/" "foundation reference folder"
expect_fixed "${REFERENCES_DIR}/README.md" "troubleshooting/" "troubleshooting reference folder"
expect_fixed "${REFERENCES_DIR}/README.md" "api-directory-map.md" "API directory map reference"
expect_fixed "${REFERENCES_DIR}/README.md" "generation-constraints.json" "generation constraints reference"
expect_fixed "${REFERENCES_DIR}/foundations/api-directory-map.md" "API 目录映射表" "API directory map title"
expect_fixed "${REFERENCES_DIR}/foundations/api-directory-map.md" "tests/api/reg_compute_api/" "reg compute test mapping"
expect_fixed "${REFERENCES_DIR}/foundations/api-directory-map.md" "impl/c_api/instr_impl/npu_arch_3510/" "C API implementation mapping"
expect_fixed "${REFERENCES_DIR}/foundations/api-directory-map.md" "ascendc_case_ascend950pr_9599_aiv_basic" "950 AIV basic test mapping"
if grep -Eq 'Tensor API|tensor_api|experimental/tensor_api|tests/api/tensor_api' "${REFERENCES_DIR}/foundations/api-directory-map.md"; then
    fail "current asc-api-ut-gen skill must ignore Tensor API mappings"
fi
expect_fixed "${REFERENCES_DIR}/foundations/test-templates.md" "只维护跨 API 可复用的 UT 骨架" "template responsibility boundary"
expect_fixed "${REFERENCES_DIR}/foundations/test-templates.md" "模板职责边界" "template ownership table"
expect_fixed "${REFERENCES_DIR}/foundations/generation-constraints.json" "\"api_restrictions\"" "structured API restrictions"

if grep -Eq 'class Kernel\{ApiName\}|class Test\{ApiName\}CAPI|KERNEL_FIXPIPE' "${REFERENCES_DIR}/foundations/test-templates.md"; then
    fail "test-templates.md must not duplicate API-specific AIC/C API code blocks"
fi

for token in \
    "class ApiType" \
    "ChipArch = Enum" \
    "class TestCase" \
    "class UTConfig" \
    "ARCH_DIR_MAP" \
    "NPU_ARCH_MAP" \
    "DTYPE_MAP" \
    "GENERATOR_DTYPE_MAP" \
    "npu-arch-facts.json" \
    "generation-constraints.json" \
    "def create_generator"; do
    expect_fixed "$GENERATOR" "$token" "generator token ${token}"
done

if grep -Eq 'DTYPE_MAP[[:space:]]*=[[:space:]]*\{|ARCH_DIR_MAP[[:space:]]*=[[:space:]]*\{|NPU_ARCH_MAP[[:space:]]*=[[:space:]]*\{' "$GENERATOR"; then
    fail "generator must load chip and dtype facts from structured references"
fi

for token in \
    "def validate_config" \
    "def parse_config" \
    "def get_output_path" \
    "def get_token_usage" \
    "def parse_coverage_report" \
    "def print_generation_report" \
    "def main" \
    "argparse.ArgumentParser" \
    "GENERATOR_DTYPE_MAP" \
    "--config" \
    "--type" \
    "--api" \
    "--chip" \
    "--coverage-report" \
    "--report-json" \
    "--list-supported"; do
    expect_fixed "$CLI" "$token" "CLI token ${token}"
done

expect_fixed "$SKILL_FILE" "token 消耗、总耗时、当前覆盖率" "post generation metrics"
expect_fixed "${REFERENCES_DIR}/workflows/automation-guide.md" "Token 消耗" "automation token report"
expect_fixed "${REFERENCES_DIR}/workflows/automation-guide.md" "当前覆盖率" "automation coverage report"

echo "[PASS] asc-api-ut-gen skill content validation passed"
