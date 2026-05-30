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
SKILLS_DIR="${REPO_ROOT}/.agent/skills"
DEFAULT_SKILL_FILE="${SKILLS_DIR}/default-skills/SKILL.md"
INSTALL_SCRIPT="${SKILLS_DIR}/default-skills/scripts/install-default-skills.sh"
REMOTE_SKILL_FILE="${SKILLS_DIR}/_remote/gitcode-pr/SKILL.md"
AGENT_README="${REPO_ROOT}/.agent/README.md"
GITIGNORE="${REPO_ROOT}/.gitignore"

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

reject_tree_pattern() {
    local path="$1"
    local pattern="$2"
    local label="$3"

    if grep -REnI \
        --include='*.md' \
        --include='*.py' \
        --include='*.sh' \
        --include='*.yaml' \
        -- "$pattern" "$path"; then
        fail "forbidden content: ${label}"
    fi
}

if [[ ! -f "$REMOTE_SKILL_FILE" ]]; then
    bash "$INSTALL_SCRIPT"
fi

expect_fixed "$DEFAULT_SKILL_FILE" "安装默认skills步骤" "default skill install workflow"
expect_fixed "$DEFAULT_SKILL_FILE" "install-default-skills.sh" "default skill install script reference"
expect_fixed "$DEFAULT_SKILL_FILE" "DEFAULT_SKILLS" "default skill list reference"
expect_fixed "$DEFAULT_SKILL_FILE" "https://gitcode.com/cann-agent/skills.git" "remote skills repository"
expect_fixed "$DEFAULT_SKILL_FILE" ".agent/skills/_remote/" "asc-devkit remote skill directory"
expect_fixed "$DEFAULT_SKILL_FILE" "gitcode-pr" "gitcode-pr default skill"
expect_fixed "$DEFAULT_SKILL_FILE" "gitcode-issue" "gitcode-issue default skill"
expect_fixed "$DEFAULT_SKILL_FILE" "api-doc-generator" "api-doc-generator default skill"
expect_fixed "$DEFAULT_SKILL_FILE" "gitcode-pipeline" "gitcode-pipeline default skill"

expect_fixed "$INSTALL_SCRIPT" 'DEFAULT_SKILLS=("gitcode-pr" "gitcode-issue" "api-doc-generator" "gitcode-pipeline")' \
    "script default skill list"
expect_fixed "$INSTALL_SCRIPT" 'REPO_URL="https://gitcode.com/cann-agent/skills.git"' "script remote repository"
expect_fixed "$INSTALL_SCRIPT" 'git clone --depth 1 "$REPO_URL"' "script shallow clone"
expect_fixed "$INSTALL_SCRIPT" 'REMOTE_DIR="$SKILLS_DIR/_remote"' "script remote directory"
expect_fixed "$INSTALL_SCRIPT" 'ln -sfn "_remote/$skill" "$SKILLS_DIR/$skill"' "script symlink creation"
expect_fixed "$INSTALL_SCRIPT" 'rm -rf "$SKILLS_DIR/$skill"' "script replace local skill with symlink"
expect_fixed "$INSTALL_SCRIPT" 'ENTRY=".agent/skills/$skill"' "script gitignore symlink entry"

expect_fixed "$REMOTE_SKILL_FILE" "创建/提交 PR" "remote gitcode-pr create PR triggers"
expect_fixed "$REMOTE_SKILL_FILE" "获取PR改动" "remote gitcode-pr changes triggers"
expect_fixed "$REMOTE_SKILL_FILE" "查看 PR 讨论" "remote gitcode-pr discussion triggers"
expect_fixed "$REMOTE_SKILL_FILE" "删除 PR 评论" "remote gitcode-pr delete comment triggers"

expect_fixed "$AGENT_README" "\`default-skills\`" "README default-skills listing"
expect_fixed "$AGENT_README" "\`gitcode-pr\`" "README gitcode-pr listing"
expect_fixed "$AGENT_README" "_remote/gitcode-pr" "README remote skill implementation"
expect_fixed "$GITIGNORE" ".agent/skills/_remote/" "gitignore remote skill cache"
expect_fixed "$GITIGNORE" ".agent/skills/gitcode-pr" "gitignore gitcode-pr symlink"

reject_tree_pattern "${SKILLS_DIR}/default-skills" '\.claude/skills' "GE .claude path in asc-devkit default-skills"

echo "[PASS] gitcode-pr skill content validation passed"
