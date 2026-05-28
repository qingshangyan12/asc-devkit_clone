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
SOURCE_TEMPLATE="${REPO_ROOT}/.gitcode/PULL_REQUEST_TEMPLATE.zh-CN.md"
REFERENCE_FILE="${SKILL_DIR}/references/gitcode_api.md"
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

reject_fixed() {
    local file="$1"
    local text="$2"
    local label="$3"
    if grep -Fq -- "$text" "$file"; then
        fail "forbidden content: ${label}"
    fi
}

expect_fixed "$SKILL_FILE" "cann/asc-devkit" "target upstream repository"
expect_fixed "$SKILL_FILE" "不要硬编码个人仓库地址或用户名" "no hardcoded personal remote rule"
expect_fixed "$SKILL_FILE" "git remote -v" "remote inspection command"
expect_fixed "$SKILL_FILE" "git status --short" "worktree inspection command"
expect_fixed "$SKILL_FILE" "GITCODE_API_TOKEN" "primary token variable"
expect_fixed "$SKILL_FILE" "GITCODE_TOKEN" "compatible token variable"
expect_fixed "$SKILL_FILE" "git push -u <personal-remote> HEAD:<branch-name>" "personal remote push command"
expect_fixed "$SKILL_FILE" "--dry-run" "dry run guidance"
expect_fixed "$SKILL_FILE" "不要使用 \`git reset --hard\` 或强推" "destructive command guard"
expect_fixed "$SKILL_FILE" "如果个人远程或目标主仓无法唯一确定，停止并向用户确认" "ambiguous remote guard"
expect_fixed "$SKILL_FILE" "创建PR、提个PR、发PR、做个PR、帮我PR、生成PR、需要PR、pull request、merge request" "create PR triggers"
expect_fixed "$SKILL_FILE" "push代码、推代码、把代码推上去、提交到远程、推送到gitcode、提交代码到GitCode" "push triggers"
expect_fixed "$SKILL_FILE" "PR模板、PR描述、PR格式" "PR template triggers"
expect_fixed "$SKILL_FILE" "关issue的PR、关联issue创建PR" "issue PR triggers"
expect_fixed "$SKILL_FILE" "查看PR变更、PR文件列表、PR改了什么、看PRdiff、获取PR文件" "PR diff triggers"
expect_fixed "$SKILL_FILE" "查看PR评论、PR评论、获取评论、read comments" "comment triggers"
expect_fixed "$SKILL_FILE" "PR discussions、查看讨论、discussions" "discussion triggers"
expect_fixed "$SKILL_FILE" "删除评论、删除PR评论、移除评论、delete comment、移除这条评论" "delete comment triggers"
expect_fixed "$SKILL_FILE" "scripts/pr_api.py changes" "changes command"
expect_fixed "$SKILL_FILE" "scripts/pr_api.py comments" "comments command"
expect_fixed "$SKILL_FILE" "scripts/pr_api.py discussions" "discussions command"
expect_fixed "$SKILL_FILE" "scripts/pr_api.py delete-comment" "delete comment command"
expect_fixed "$SKILL_FILE" "https://gitcode.com/<owner>/<repo>/pull/<PR_NUMBER>" "PR URL format"
expect_fixed "$SKILL_FILE" "--pr-url" "PR URL argument"

expect_fixed "$SCRIPT_FILE" "DEFAULT_TARGET = \"cann/asc-devkit\"" "script target default"
expect_fixed "$SCRIPT_FILE" "GITCODE_API_TOKEN" "script token env"
expect_fixed "$SCRIPT_FILE" "GITCODE_TOKEN" "script token compatibility"
expect_fixed "$SCRIPT_FILE" "dry-run" "script dry-run option"
expect_fixed "$SCRIPT_FILE" "parse_repo_path" "repo parser"
expect_fixed "$SCRIPT_FILE" "render_template" "template renderer"
expect_fixed "$SCRIPT_FILE" "--issue" "issue argument"
expect_fixed "$SCRIPT_FILE" "--description" "description argument"

expect_fixed "$PR_API_SCRIPT" "DEFAULT_TARGET = \"cann/asc-devkit\"" "PR API target default"
expect_fixed "$PR_API_SCRIPT" "DEFAULT_API_V4_BASE" "PR API v4 base"
expect_fixed "$PR_API_SCRIPT" "DEFAULT_API_V5_BASE" "PR API v5 base"
expect_fixed "$PR_API_SCRIPT" "delete-comment" "PR API delete comment subcommand"
expect_fixed "$PR_API_SCRIPT" "discussions" "PR API discussions subcommand"
expect_fixed "$PR_API_SCRIPT" "changes" "PR API changes subcommand"
expect_fixed "$PR_API_SCRIPT" "parse_pr_url" "PR URL parser"
expect_fixed "$PR_API_SCRIPT" "resolve_target_and_pr" "PR target resolver"
expect_fixed "$PR_API_SCRIPT" "add_common_options" "common option parser"
expect_fixed "$PR_API_SCRIPT" "PRIVATE-TOKEN" "PR API v4 token header"
expect_fixed "$PR_API_SCRIPT" "access_token" "PR API delete token query"

[[ -f "$SOURCE_TEMPLATE" ]] || fail "missing source PR template: ${SOURCE_TEMPLATE}"
cmp -s "$TEMPLATE_FILE" "$SOURCE_TEMPLATE" || fail "skill PR template must match .gitcode/PULL_REQUEST_TEMPLATE.zh-CN.md"
expect_fixed "$TEMPLATE_FILE" "## 描述" "template description section"
expect_fixed "$TEMPLATE_FILE" "## 关联的Issue" "template issue section"
expect_fixed "$TEMPLATE_FILE" "## 测试" "template test section"
expect_fixed "$TEMPLATE_FILE" "## 文档更新" "template docs section"
expect_fixed "$TEMPLATE_FILE" "## 类型标签" "template type label section"
expect_fixed "$TEMPLATE_FILE" "🐛 fix: Bug 修复" "template fix type label"
reject_fixed "$TEMPLATE_FILE" "{{description}}" "template description placeholder"
reject_fixed "$TEMPLATE_FILE" "{{issue}}" "template issue placeholder"
reject_fixed "$TEMPLATE_FILE" "## 如何测试" "old template test section"
reject_fixed "$TEMPLATE_FILE" "## 核对清单" "old template checklist"
expect_fixed "$REFERENCE_FILE" "head" "api head field"
expect_fixed "$REFERENCE_FILE" "cann/asc-devkit" "api target repository"
expect_fixed "$REFERENCE_FILE" "cann%2Fasc-devkit" "encoded target repository"
expect_fixed "$REFERENCE_FILE" "/merge_requests/<PR_NUMBER>/changes" "changes API"
expect_fixed "$REFERENCE_FILE" "/merge_requests/<PR_NUMBER>/notes" "comments API"
expect_fixed "$REFERENCE_FILE" "/merge_requests/<PR_NUMBER>/discussions" "discussions API"
expect_fixed "$REFERENCE_FILE" "pulls/comments/<COMMENT_ID>" "delete comment API"
expect_fixed "$REFERENCE_FILE" "https://gitcode.com/<owner>/<repo>/pull/<PR_NUMBER>" "reference PR URL format"
expect_fixed "$REFERENCE_FILE" "git@gitcode.com:<owner>/<repo>.git" "ssh remote format"
expect_fixed "$REFERENCE_FILE" "个人远程 owner 不应硬编码" "reference no hardcode rule"
expect_fixed "$AGENT_README" "\`gitcode-pr\`" "README skill listing"

reject_fixed "$SKILL_FILE" "maohaodi/asc-devkit" "personal fork hardcoding in skill"
reject_fixed "$SCRIPT_FILE" "maohaodi/asc-devkit" "personal fork hardcoding in script"
reject_fixed "$PR_API_SCRIPT" "maohaodi/asc-devkit" "personal fork hardcoding in PR API script"
reject_fixed "$REFERENCE_FILE" "maohaodi/asc-devkit" "personal fork hardcoding in reference"

echo "[PASS] gitcode-pr skill content validation passed"
