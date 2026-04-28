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

usage() {
    cat <<'EOF'
Usage:
  bash .opencode/install-skills.sh [--codex] [--claude] [--all] [--force]
  bash .opencode/install-skills.sh --uninstall [--codex] [--claude] [--all] [--force]

Installs or uninstalls asc-devkit skills by managing symlinks from:
  .opencode/skills

Targets:
  --codex   Manage Codex target:
  ${CODEX_HOME:-$HOME/.codex}/skills
  --claude  Manage Claude Code target:
  <repo>/.claude/skills
  --all     Manage both targets
  --uninstall
            Remove installed symlinks instead of creating them
  --force   Replace mismatched symlinks during install, or remove mismatched symlinks during uninstall

Installed skills:
  - asc-api-ut-gen
  - asc-npu-arch

No target option defaults to --codex for both install and uninstall.
Restart the target tool after installation so $asc-api-ut-gen and $asc-npu-arch are reloaded.
EOF
}

force=0
uninstall=0
install_codex=0
install_claude=0
for arg in "$@"; do
    case "$arg" in
        --codex)
            install_codex=1
            ;;
        --claude)
            install_claude=1
            ;;
        --all)
            install_codex=1
            install_claude=1
            ;;
        --force)
            force=1
            ;;
        --uninstall)
            uninstall=1
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "[FAIL] unknown argument: $arg" >&2
            usage >&2
            exit 1
            ;;
    esac
done

if [[ "$install_codex" -eq 0 && "$install_claude" -eq 0 ]]; then
    install_codex=1
fi

OPENCODE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${OPENCODE_DIR}/.." && pwd)"
SOURCE_ROOT="${OPENCODE_DIR}/skills"
CODEX_HOME="${CODEX_HOME:-${HOME}/.codex}"
SKILLS=(asc-api-ut-gen asc-npu-arch)

install_one_skill() {
    local skill_name="$1"
    local dest_root="$2"
    local target_label="$3"
    local source_dir="${SOURCE_ROOT}/${skill_name}"
    local dest_dir="${dest_root}/${skill_name}"
    local current_target

    [[ -f "${source_dir}/SKILL.md" ]] || {
        echo "[FAIL] missing skill source: ${source_dir}/SKILL.md" >&2
        exit 1
    }

    if [[ -L "$dest_dir" ]]; then
        current_target="$(readlink "$dest_dir")"
        if [[ "$current_target" == "$source_dir" ]]; then
            echo "[OK] ${skill_name} already installed -> ${source_dir}"
            return 0
        fi
        if [[ "$force" -ne 1 ]]; then
            echo "[FAIL] ${dest_dir} already points to ${current_target}; rerun with --force to replace it" >&2
            exit 1
        fi
        rm "$dest_dir"
    elif [[ -e "$dest_dir" ]]; then
        echo "[FAIL] ${dest_dir} already exists and is not a symlink; move it aside before installing" >&2
        exit 1
    fi

    mkdir -p "$dest_root"
    ln -s "$source_dir" "$dest_dir"
    echo "[OK] installed ${skill_name} for ${target_label} -> ${source_dir}"
}

uninstall_one_skill() {
    local skill_name="$1"
    local dest_root="$2"
    local target_label="$3"
    local source_dir="${SOURCE_ROOT}/${skill_name}"
    local dest_dir="${dest_root}/${skill_name}"
    local current_target

    if [[ -L "$dest_dir" ]]; then
        current_target="$(readlink "$dest_dir")"
        if [[ "$current_target" == "$source_dir" ]]; then
            rm "$dest_dir"
            echo "[OK] uninstalled ${skill_name} for ${target_label}"
            return 0
        fi
        if [[ "$force" -ne 1 ]]; then
            echo "[FAIL] ${dest_dir} points to ${current_target}; rerun with --force to remove the mismatched symlink" >&2
            exit 1
        fi
        rm "$dest_dir"
        echo "[OK] removed mismatched ${skill_name} symlink for ${target_label} -> ${current_target}"
    elif [[ -e "$dest_dir" ]]; then
        echo "[FAIL] ${dest_dir} exists and is not a symlink; refusing to remove it" >&2
        exit 1
    else
        echo "[OK] ${skill_name} not installed for ${target_label}"
    fi
}

if [[ "$install_codex" -eq 1 ]]; then
    codex_dest_root="${CODEX_HOME}/skills"
    mkdir -p "$codex_dest_root"
    for skill_name in "${SKILLS[@]}"; do
        if [[ "$uninstall" -eq 1 ]]; then
            uninstall_one_skill "$skill_name" "$codex_dest_root" "codex"
        else
            install_one_skill "$skill_name" "$codex_dest_root" "codex"
        fi
    done
fi

if [[ "$install_claude" -eq 1 ]]; then
    claude_dest_root="${REPO_ROOT}/.claude/skills"
    mkdir -p "$claude_dest_root"
    for skill_name in "${SKILLS[@]}"; do
        if [[ "$uninstall" -eq 1 ]]; then
            uninstall_one_skill "$skill_name" "$claude_dest_root" "claude"
        else
            install_one_skill "$skill_name" "$claude_dest_root" "claude"
        fi
    done
fi

if [[ "$uninstall" -eq 1 ]]; then
    echo "[PASS] restart the target tool so removed skills are no longer loaded"
else
    echo "[PASS] restart the target tool to use \$asc-api-ut-gen and \$asc-npu-arch"
fi
