#!/usr/bin/env bash
# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set -euo pipefail

LYCHEE_VERSION="${LYCHEE_VERSION:-0.23.0}"
MARKDOWN_LINK_CHECK_FULL="${MARKDOWN_LINK_CHECK_FULL:-0}"
MARKDOWN_LINK_CHECK_DRY_RUN="${MARKDOWN_LINK_CHECK_DRY_RUN:-0}"

REPO_ROOT=$(git rev-parse --show-toplevel 2>/dev/null || pwd)
cd "$REPO_ROOT"

CONFIG_FILE="$REPO_ROOT/markdown-link-check.toml"
if [ ! -f "$CONFIG_FILE" ]; then
    echo "[Markdown check] Missing config: $CONFIG_FILE" >&2
    exit 1
fi

usage() {
    cat <<'EOF'
Usage: scripts/markdown_link_check.sh [--full|--all] [markdown-file...]

By default, checks changed Markdown files and Markdown files that link to them.
Set MARKDOWN_LINK_CHECK_FULL=1 or pass --full to scan docs/ and examples/.
Set MARKDOWN_LINK_CHECK_DRY_RUN=1 to print the computed scan set only.
Set MARKDOWN_LINK_CHECK_LYCHEE to use a preinstalled lychee binary.
Set MARKDOWN_LINK_CHECK_LYCHEE_ARCHIVE_URL, MARKDOWN_LINK_CHECK_CARGO_BINSTALL_INSTALLER_URL, or
MARKDOWN_LINK_CHECK_LYCHEE_PKG_URL to use mirrors.
EOF
}

download_lychee_archive() {
    local target archive_url tmp_dir

    case "$(uname -s):$(uname -m)" in
        Linux:x86_64|Linux:amd64)
            target="x86_64-unknown-linux-gnu"
            ;;
        Linux:aarch64|Linux:arm64)
            target="aarch64-unknown-linux-gnu"
            ;;
        *)
            return 1
            ;;
    esac

    archive_url="${MARKDOWN_LINK_CHECK_LYCHEE_ARCHIVE_URL:-https://gh.llkk.cc/https://github.com/lycheeverse/lychee/releases/download/lychee-v${LYCHEE_VERSION}/lychee-${target}.tar.gz}"
    tmp_dir=$(mktemp -d)
    echo "[Markdown check] Downloading lychee@$LYCHEE_VERSION from $archive_url..." >&2
    if ! curl -L --proto '=https' --tlsv1.2 -sSf "$archive_url" -o "$tmp_dir/lychee.tar.gz"; then
        rm -rf "$tmp_dir"
        return 1
    fi
    if ! tar -xzf "$tmp_dir/lychee.tar.gz" -C "$tmp_dir" lychee; then
        rm -rf "$tmp_dir"
        return 1
    fi
    install -m 0755 "$tmp_dir/lychee" "$LYCHEE_BIN"
    rm -rf "$tmp_dir"
    return 0
}

FULL_SCAN="$MARKDOWN_LINK_CHECK_FULL"
INPUT_FILES=()
while [ "$#" -gt 0 ]; do
    case "$1" in
        --all|--full)
            FULL_SCAN=1
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            INPUT_FILES+=("$1")
            ;;
    esac
    shift
done

find_lychee() {
    if [ -n "${MARKDOWN_LINK_CHECK_LYCHEE:-}" ] && [ -x "$MARKDOWN_LINK_CHECK_LYCHEE" ]; then
        printf '%s\n' "$MARKDOWN_LINK_CHECK_LYCHEE"
        return 0
    fi

    if command -v lychee >/dev/null 2>&1; then
        command -v lychee
        return 0
    fi

    GIT_COMMON_DIR=$(git rev-parse --git-common-dir 2>/dev/null || printf '.git')
    case "$GIT_COMMON_DIR" in
        /*) CACHE_ROOT="$GIT_COMMON_DIR/markdown-link-check" ;;
        *) CACHE_ROOT="$REPO_ROOT/$GIT_COMMON_DIR/markdown-link-check" ;;
    esac

    CARGO_HOME_DIR="$CACHE_ROOT/cargo"
    CARGO_BIN="$CARGO_HOME_DIR/bin"
    LYCHEE_BIN="$CARGO_BIN/lychee-$LYCHEE_VERSION"

    if [ -x "$LYCHEE_BIN" ]; then
        printf '%s\n' "$LYCHEE_BIN"
        return 0
    fi

    mkdir -p "$CARGO_BIN"
    export CARGO_HOME="$CARGO_HOME_DIR"
    export PATH="$CARGO_BIN:$PATH"

    if download_lychee_archive; then
        printf '%s\n' "$LYCHEE_BIN"
        return 0
    fi

    if command -v cargo-binstall >/dev/null 2>&1; then
        BINSTALL=(cargo-binstall)
    elif command -v cargo >/dev/null 2>&1 && cargo binstall -V >/dev/null 2>&1; then
        BINSTALL=(cargo binstall)
    else
        echo "[Markdown check] Installing cargo-binstall..." >&2
        CARGO_BINSTALL_INSTALLER_URL="${MARKDOWN_LINK_CHECK_CARGO_BINSTALL_INSTALLER_URL:-https://raw.githubusercontent.com/cargo-bins/cargo-binstall/main/install-from-binstall-release.sh}"
        curl -L --proto '=https' --tlsv1.2 -sSf \
            "$CARGO_BINSTALL_INSTALLER_URL" \
            | CARGO_HOME="$CARGO_HOME_DIR" BASH_XTRACEFD=7 bash 7>/dev/null >&2
        BINSTALL=("$CARGO_BIN/cargo-binstall")
    fi

    echo "[Markdown check] Installing lychee@$LYCHEE_VERSION..." >&2
    BINSTALL_ARGS=(-y --install-path "$CARGO_BIN")
    if [ -n "${MARKDOWN_LINK_CHECK_LYCHEE_PKG_URL:-}" ]; then
        BINSTALL_ARGS+=(--pkg-url "$MARKDOWN_LINK_CHECK_LYCHEE_PKG_URL")
    fi
    BINSTALL_ARGS+=("lychee@$LYCHEE_VERSION")
    "${BINSTALL[@]}" "${BINSTALL_ARGS[@]}" >&2
    if [ ! -x "$CARGO_BIN/lychee" ]; then
        echo "[Markdown check] lychee binary was not installed." >&2
        exit 1
    fi
    mv -f "$CARGO_BIN/lychee" "$LYCHEE_BIN"
    printf '%s\n' "$LYCHEE_BIN"
}

SCAN_SET_FILE=$(mktemp)
trap 'rm -f "$SCAN_SET_FILE"' EXIT

PY_ARGS=("$REPO_ROOT" "$SCAN_SET_FILE")
if [ "${#INPUT_FILES[@]}" -gt 0 ]; then
    PY_ARGS+=("${INPUT_FILES[@]}")
fi

FULL_SCAN="$FULL_SCAN" python3 - "${PY_ARGS[@]}" <<'PY'
import os
import posixpath
import re
import subprocess
import sys
from pathlib import Path, PurePosixPath
from urllib.parse import unquote

repo_root = Path(sys.argv[1])
output_file = Path(sys.argv[2])
input_files = sys.argv[3:]

scan_roots = ("docs", "examples")
markdown_exts = (".md", ".markdown", ".mdx")
full_scan = os.environ.get("FULL_SCAN") == "1"
full_reason = ""


def to_rel(path):
    p = Path(path)
    if p.is_absolute():
        try:
            return p.resolve().relative_to(repo_root.resolve()).as_posix()
        except ValueError:
            return None
    return PurePosixPath(path).as_posix()


def is_markdown_path(path):
    if path is None:
        return False
    return path.startswith(scan_roots) and path.lower().endswith(markdown_exts)


def existing_markdown_files():
    result = []
    for root in scan_roots:
        root_path = repo_root / root
        if not root_path.exists():
            continue
        for path in root_path.rglob("*"):
            if path.is_file() and path.name.lower().endswith(markdown_exts):
                result.append(path.relative_to(repo_root).as_posix())
    return sorted(result)


def staged_markdown_changes():
    try:
        result = subprocess.run(
            ["git", "diff", "--cached", "--name-status", "--diff-filter=ADMR"],
            cwd=repo_root,
            check=False,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
        )
    except OSError:
        return [], False

    changed = []
    needs_full = False
    for line in result.stdout.splitlines():
        parts = line.split("\t")
        if not parts:
            continue
        status = parts[0]
        paths = parts[1:]
        markdown_paths = [p for p in paths if is_markdown_path(p)]
        if not markdown_paths:
            continue
        if status.startswith(("D", "R")):
            needs_full = True
        for path in markdown_paths:
            if status.startswith("D"):
                continue
            changed.append(path)
    return changed, needs_full


def extract_link_targets(text):
    # Inline Markdown links/images: [text](target) or ![alt](target)
    for match in re.finditer(r"!?\[[^\]]*\]\(([^)\s]+)(?:\s+[^)]*)?\)", text):
        yield match.group(1)

    # Reference definitions: [label]: target
    for match in re.finditer(r"(?m)^\s*\[[^\]]+\]:\s*(\S+)", text):
        yield match.group(1)

    # Raw HTML links used by some generated docs.
    for match in re.finditer(r"""(?i)\bhref\s*=\s*["']([^"']+)["']""", text):
        yield match.group(1)


def normalize_target(source_file, target):
    target = target.strip().strip("<>").strip("\"'")
    if not target or target.startswith("#"):
        return None
    if re.match(r"^[a-zA-Z][a-zA-Z0-9+.-]*:", target):
        return None

    target = target.split("#", 1)[0].split("?", 1)[0]
    if not target:
        return None

    target = unquote(target)
    if target.startswith("/"):
        normalized = posixpath.normpath(target.lstrip("/"))
    else:
        normalized = posixpath.normpath(posixpath.join(posixpath.dirname(source_file), target))
    return normalized


if not full_scan:
    staged_changes, needs_full_for_staged = staged_markdown_changes()
    if needs_full_for_staged:
        full_scan = True
        full_reason = "staged Markdown rename/delete"

if full_scan:
    scan_set = [root for root in scan_roots if (repo_root / root).exists()]
else:
    changed = [to_rel(path) for path in input_files]
    changed = [path for path in changed if is_markdown_path(path)]
    if not changed:
        staged_changes, _ = staged_markdown_changes()
        changed = staged_changes

    changed_existing = {
        path for path in changed
        if path and (repo_root / path).is_file()
    }

    all_markdown = existing_markdown_files()
    scan_set = set(changed_existing)
    for source in all_markdown:
        source_path = repo_root / source
        try:
            text = source_path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            text = source_path.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue

        for target in extract_link_targets(text):
            normalized = normalize_target(source, target)
            if normalized in changed_existing:
                scan_set.add(source)
                break

    scan_set = sorted(scan_set)

with output_file.open("w", encoding="utf-8") as handle:
    if full_scan:
        full_reason = full_reason or "requested"
        handle.write(f"# full scan: {full_reason}\n")
    for item in scan_set:
        handle.write(item + "\n")
PY

SCAN_FILES=()
FULL_REASON=""
while IFS= read -r item; do
    case "$item" in
        "# full scan:"*)
            FULL_REASON="${item#"# full scan: "}"
            ;;
        "")
            ;;
        *)
            SCAN_FILES+=("$item")
            ;;
    esac
done < "$SCAN_SET_FILE"

if [ "${#SCAN_FILES[@]}" -eq 0 ]; then
    echo "[Markdown check] PASS: no Markdown files to check."
    exit 0
fi

if [ -n "$FULL_REASON" ]; then
    echo "[Markdown check] Checking all Markdown files under docs/ and examples/ ($FULL_REASON)."
else
    echo "[Markdown check] Checking ${#SCAN_FILES[@]} Markdown file(s) in the affected set."
fi

if [ "$MARKDOWN_LINK_CHECK_DRY_RUN" = "1" ]; then
    printf '%s\n' "${SCAN_FILES[@]}"
    exit 0
fi

LYCHEE_BIN=$(find_lychee)
exec "$LYCHEE_BIN" --no-progress --config "$CONFIG_FILE" "${SCAN_FILES[@]}"
