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
SKILL_DIR="${SKILL_DIR:-${REPO_ROOT}/.agent/skills/gitcode-pr}"
SKILL_FILE="${SKILL_DIR}/SKILL.md"
SCRIPT_FILE="${SKILL_DIR}/scripts/create_pr.py"
PR_API_SCRIPT="${SKILL_DIR}/scripts/pr_api.py"
TEMPLATE_FILE="${SKILL_DIR}/assets/pr_template.md"
REFERENCE_FILE="${SKILL_DIR}/references/gitcode_api.md"

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

expect_file() {
    local path="$1"
    [[ -f "$path" ]] || fail "missing file: $path"
    [[ -s "$path" ]] || fail "empty file: $path"
}

expect_dir() {
    local path="$1"
    [[ -d "$path" ]] || fail "missing directory: $path"
}

expect_dir "$SKILL_DIR"
expect_dir "${SKILL_DIR}/scripts"
expect_dir "${SKILL_DIR}/assets"
expect_dir "${SKILL_DIR}/references"
expect_file "$SKILL_FILE"
expect_file "$SCRIPT_FILE"
expect_file "$PR_API_SCRIPT"
expect_file "$TEMPLATE_FILE"
expect_file "$REFERENCE_FILE"

[[ "$(basename "$SKILL_DIR")" == "gitcode-pr" ]] || fail "skill directory must be gitcode-pr"
[[ ! -e "${SKILL_DIR}/README.md" ]] || fail "skill directory must not contain README.md"
[[ "$(head -n 1 "$SKILL_FILE")" == "---" ]] || fail "SKILL.md must start with YAML frontmatter"
grep -Eq '^name:[[:space:]]*gitcode-pr[[:space:]]*$' "$SKILL_FILE" || fail "frontmatter name must be gitcode-pr"
grep -Eq '^description:[[:space:]]*.+' "$SKILL_FILE" || fail "frontmatter description is required"

python3 - "$SCRIPT_FILE" "$PR_API_SCRIPT" <<'PY' || fail "python scripts must parse"
import ast
import sys

for path in sys.argv[1:]:
    with open(path, encoding="utf-8") as f:
        ast.parse(f.read(), filename=path)
PY

grep -Fq "scripts/create_pr.py" "$SKILL_FILE" || fail "SKILL.md must link create_pr.py"
grep -Fq "scripts/pr_api.py" "$SKILL_FILE" || fail "SKILL.md must link pr_api.py"
grep -Fq "assets/pr_template.md" "$SKILL_FILE" || fail "SKILL.md must link pr_template.md"
grep -Fq "references/gitcode_api.md" "$SKILL_FILE" || fail "SKILL.md must link gitcode_api.md"

echo "[PASS] gitcode-pr skill structure validation passed"
