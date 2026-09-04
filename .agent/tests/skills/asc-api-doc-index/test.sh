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
SKILL_FILE="${REPO_ROOT}/.agent/skills/asc-api-doc-index/SKILL.md"
AGENT_README="${REPO_ROOT}/.agent/README.md"

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

expect() {
    grep -Fq -- "$2" "$1" || fail "missing content: $3"
}

[[ -s "$SKILL_FILE" ]] || fail "missing skill file"
[[ "$(head -n 1 "$SKILL_FILE")" == "---" ]] || fail "SKILL.md must start with YAML frontmatter"
grep -Eq '^name:[[:space:]]*asc-api-doc-index[[:space:]]*$' "$SKILL_FILE" || fail "invalid skill name"
grep -Eq '^description:[[:space:]]*.+' "$SKILL_FILE" || fail "missing skill description"

expect "$SKILL_FILE" 'docs/zh/api/README.md' "global navigation"
expect "$SKILL_FILE" 'docs/zh/api/figures/' "PNG figure directory"
expect "$SKILL_FILE" 'docs/zh/api/figures/svg/' "SVG source directory"
expect "$SKILL_FILE" 'same basename' "PNG SVG pairing"
expect "$SKILL_FILE" 'Markdown pages must reference the relative `.png` path' "PNG Markdown reference rule"
expect "$SKILL_FILE" 'Use FZLanTingHeiS for Chinese text and English text' "unified figure font"
if grep -Fq 'Huawei Sans for English text' "$SKILL_FILE"; then
    fail "skill must require FZLanTingHeiS for English text"
fi
expect "$SKILL_FILE" 'MARKDOWN_LINK_CHECK_ONLINE=0 bash scripts/markdown_link_check.sh' "Markdown validation"
expect "$SKILL_FILE" 'git diff --check' "diff validation"
expect "$AGENT_README" '`asc-api-doc-index`' "README skill entry"
expect "$AGENT_README" 'bash .agent/tests/skills/asc-api-doc-index/test.sh' "README test command"

echo "[PASS] asc-api-doc-index skill test passed"
