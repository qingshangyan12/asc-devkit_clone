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
INSTALL_SCRIPT="${REPO_ROOT}/.opencode/install-skills.sh"
TMP_CODEX_HOME="/tmp/asc-devkit-codex-skill-install-test"
TMP_REPO_ROOT="/tmp/asc-devkit-claude-skill-install-test"

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

expect_symlink_target() {
    local path="$1"
    local expected="$2"

    [[ -L "$path" ]] || fail "missing symlink: $path"
    actual="$(readlink "$path")"
    [[ "$actual" == "$expected" ]] || fail "unexpected target for $path: $actual"
}

expect_file() {
    local path="$1"
    [[ -f "$path" ]] || fail "missing file: $path"
    [[ -s "$path" ]] || fail "empty file: $path"
}

expect_absent() {
    local path="$1"
    [[ ! -e "$path" && ! -L "$path" ]] || fail "path should be absent: $path"
}

expect_file "$INSTALL_SCRIPT"

bash -n "$INSTALL_SCRIPT"

rm -rf "$TMP_CODEX_HOME"
CODEX_HOME="$TMP_CODEX_HOME" bash "$INSTALL_SCRIPT" --codex

expect_symlink_target \
    "${TMP_CODEX_HOME}/skills/asc-api-ut-gen" \
    "${REPO_ROOT}/.opencode/skills/asc-api-ut-gen"
expect_symlink_target \
    "${TMP_CODEX_HOME}/skills/asc-npu-arch" \
    "${REPO_ROOT}/.opencode/skills/asc-npu-arch"

CODEX_HOME="$TMP_CODEX_HOME" bash "$INSTALL_SCRIPT" --codex >/dev/null

CODEX_HOME="$TMP_CODEX_HOME" bash "$INSTALL_SCRIPT" --uninstall --codex
expect_absent "${TMP_CODEX_HOME}/skills/asc-api-ut-gen"
expect_absent "${TMP_CODEX_HOME}/skills/asc-npu-arch"
CODEX_HOME="$TMP_CODEX_HOME" bash "$INSTALL_SCRIPT" --uninstall --codex >/dev/null

rm -rf "$TMP_CODEX_HOME"

rm -rf "$TMP_REPO_ROOT"
mkdir -p "${TMP_REPO_ROOT}/.opencode"
cp -R "${REPO_ROOT}/.opencode/skills" "${TMP_REPO_ROOT}/.opencode/skills"
cp "${INSTALL_SCRIPT}" "${TMP_REPO_ROOT}/.opencode/install-skills.sh"

CODEX_HOME="${TMP_REPO_ROOT}/codex-home" bash "${TMP_REPO_ROOT}/.opencode/install-skills.sh" --all

expect_symlink_target \
    "${TMP_REPO_ROOT}/codex-home/skills/asc-api-ut-gen" \
    "${TMP_REPO_ROOT}/.opencode/skills/asc-api-ut-gen"
expect_symlink_target \
    "${TMP_REPO_ROOT}/codex-home/skills/asc-npu-arch" \
    "${TMP_REPO_ROOT}/.opencode/skills/asc-npu-arch"
expect_symlink_target \
    "${TMP_REPO_ROOT}/.claude/skills/asc-api-ut-gen" \
    "${TMP_REPO_ROOT}/.opencode/skills/asc-api-ut-gen"
expect_symlink_target \
    "${TMP_REPO_ROOT}/.claude/skills/asc-npu-arch" \
    "${TMP_REPO_ROOT}/.opencode/skills/asc-npu-arch"

CODEX_HOME="${TMP_REPO_ROOT}/codex-home" bash "${TMP_REPO_ROOT}/.opencode/install-skills.sh" --uninstall --all

expect_absent "${TMP_REPO_ROOT}/codex-home/skills/asc-api-ut-gen"
expect_absent "${TMP_REPO_ROOT}/codex-home/skills/asc-npu-arch"
expect_absent "${TMP_REPO_ROOT}/.claude/skills/asc-api-ut-gen"
expect_absent "${TMP_REPO_ROOT}/.claude/skills/asc-npu-arch"

mkdir -p "${TMP_REPO_ROOT}/codex-home/skills"
ln -s "/tmp/not-asc-devkit-skill" "${TMP_REPO_ROOT}/codex-home/skills/asc-api-ut-gen"
if CODEX_HOME="${TMP_REPO_ROOT}/codex-home" bash "${TMP_REPO_ROOT}/.opencode/install-skills.sh" --uninstall --codex >/dev/null 2>&1; then
    fail "uninstall must reject mismatched symlink without --force"
fi
CODEX_HOME="${TMP_REPO_ROOT}/codex-home" bash "${TMP_REPO_ROOT}/.opencode/install-skills.sh" --uninstall --codex --force >/dev/null
expect_absent "${TMP_REPO_ROOT}/codex-home/skills/asc-api-ut-gen"

rm -rf "$TMP_REPO_ROOT"

echo "[PASS] skill install script validation passed"
