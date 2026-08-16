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
SKILL_DIR="${REPO_ROOT}/.agent/skills/asc-deprecated-api"
SKILL_FILE="${SKILL_DIR}/SKILL.md"
REFERENCE_FILE="${SKILL_DIR}/references/c-api-documentation-pattern.md"
AGENT_FILE="${SKILL_DIR}/agents/openai.yaml"

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

for file in "$SKILL_FILE" "$REFERENCE_FILE" "$AGENT_FILE"; do
    [[ -s "$file" ]] || fail "missing or empty file: $file"
done

[[ ! -e "${SKILL_DIR}/README.md" ]] || fail "skill directory must not contain README.md"
[[ "$(head -n 1 "$SKILL_FILE")" == "---" ]] || fail "SKILL.md must start with YAML frontmatter"
grep -Eq '^name:[[:space:]]*asc-deprecated-api[[:space:]]*$' "$SKILL_FILE" || fail "invalid skill name"
grep -Eq '^description:[[:space:]]*.+(废弃|弃用).*$' "$SKILL_FILE" || fail "description must include trigger terms"
grep -Fq 'references/c-api-documentation-pattern.md' "$SKILL_FILE" || fail "reference must be linked"
grep -Fq '$asc-deprecated-api' "$AGENT_FILE" || fail "default prompt must name the skill"

echo "[PASS] asc-deprecated-api skill structure validation passed"
