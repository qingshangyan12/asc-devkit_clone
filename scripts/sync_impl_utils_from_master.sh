#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -euo pipefail

REPO_URL="https://gitcode.com/cann/asc-devkit.git"
BRANCH="master"
SOURCE_PATH="impl/utils"
TARGET_PATH="impl/utils"
KEEP_BACKUP=0

usage() {
    cat <<EOF
Usage: bash $0 [options]

Options:
  --repo-url URL       Source repository. Default: ${REPO_URL}
  --branch BRANCH     Source branch. Default: ${BRANCH}
  --source PATH       Source path in repository. Default: ${SOURCE_PATH}
  --target PATH       Target path in current project. Default: ${TARGET_PATH}
  --keep-backup       Keep backup when target path already exists.
  -h, --help          Show this help message.
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --repo-url)
            REPO_URL="$2"
            shift 2
            ;;
        --branch)
            BRANCH="$2"
            shift 2
            ;;
        --source)
            SOURCE_PATH="$2"
            shift 2
            ;;
        --target)
            TARGET_PATH="$2"
            shift 2
            ;;
        --keep-backup)
            KEEP_BACKUP=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage >&2
            exit 1
            ;;
    esac
done

command -v git >/dev/null 2>&1 || {
    echo "git is not available in PATH" >&2
    exit 1
}

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PROJECT_ROOT=$(readlink -f "${SCRIPT_DIR}/..")
TARGET=$(readlink -m "${PROJECT_ROOT}/${TARGET_PATH}")
TARGET_PARENT=$(dirname "${TARGET}")
TARGET_NAME=$(basename "${TARGET}")

case "${TARGET}" in
    "${PROJECT_ROOT}"|"${PROJECT_ROOT}"/*) ;;
    *)
        echo "Refusing to write outside project root: ${TARGET}" >&2
        exit 1
        ;;
esac

mkdir -p "${TARGET_PARENT}"

TEMP_ROOT=$(mktemp -d)
TEMP_REPO="${TEMP_ROOT}/repo"
STAGED_TARGET="${TEMP_ROOT}/${TARGET_NAME}"
BACKUP=""

cleanup() {
    rm -rf "${TEMP_ROOT}"
}

rollback() {
    if [[ -n "${BACKUP}" && -d "${BACKUP}" && ! -e "${TARGET}" ]]; then
        mv "${BACKUP}" "${TARGET}"
    fi
}

trap cleanup EXIT
trap 'rollback' ERR

git clone --depth 1 --branch "${BRANCH}" --single-branch --sparse "${REPO_URL}" "${TEMP_REPO}"
git -C "${TEMP_REPO}" sparse-checkout set "${SOURCE_PATH}"

SOURCE="${TEMP_REPO}/${SOURCE_PATH}"
if [[ ! -d "${SOURCE}" ]]; then
    echo "Source path does not exist in ${REPO_URL}@${BRANCH}: ${SOURCE_PATH}" >&2
    exit 1
fi

cp -a "${SOURCE}" "${STAGED_TARGET}"

if [[ -e "${TARGET}" ]]; then
    BACKUP="${TARGET_PARENT}/${TARGET_NAME}.backup.$(date +%Y%m%d%H%M%S)"
    mv "${TARGET}" "${BACKUP}"
else
    echo "Target ${TARGET_PATH} does not exist; creating it."
fi

mv "${STAGED_TARGET}" "${TARGET}"

if [[ -n "${BACKUP}" && "${KEEP_BACKUP}" -eq 0 ]]; then
    rm -rf "${BACKUP}"
fi

echo "Synced ${SOURCE_PATH} from ${REPO_URL}@${BRANCH} to ${TARGET_PATH}"
