#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(git -C "${SCRIPT_DIR}" rev-parse --show-toplevel)
CANN_ROOT=${ASCEND_HOME_PATH:-${ASCEND_TOOLKIT_HOME:-}}
RUN_TIMEOUT=${SYNC_TEST_TIMEOUT:-60}
BUILD_DIR="${SCRIPT_DIR}/build-dav-3510"
MODES=(block subblock inter intra)
FAILED_MODES=()

if [[ -z "${CANN_ROOT}" || ! -f "${CANN_ROOT}/set_env.sh" ]]; then
    echo "ASCEND_HOME_PATH or ASCEND_TOOLKIT_HOME must point to a valid CANN installation." >&2
    exit 2
fi

CANN_ASC_ROOT="${CANN_ROOT}/asc"
PACKAGE_PATHS=(
    "${CANN_ASC_ROOT}/include/c_api"
    "${CANN_ASC_ROOT}/impl/c_api"
    "${CANN_ASC_ROOT}/include/utils"
    "${CANN_ASC_ROOT}/impl/utils"
)
REPO_PATHS=(
    "${REPO_ROOT}/include/c_api"
    "${REPO_ROOT}/impl/c_api"
    "${REPO_ROOT}/include/utils"
    "${REPO_ROOT}/impl/utils"
)
BACKUP_SUFFIX=".sync_flag_id_backup"
SWAPPED_PATHS=()

restore_package()
{
    for index in "${SWAPPED_PATHS[@]}"; do
        target="${PACKAGE_PATHS[index]}"
        backup="${target}${BACKUP_SUFFIX}"
        if [[ -L "${target}" ]]; then
            unlink "${target}"
        fi
        if [[ -e "${backup}" || -L "${backup}" ]]; then
            mv "${backup}" "${target}"
        fi
    done
}

trap restore_package EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

for index in "${!PACKAGE_PATHS[@]}"; do
    target="${PACKAGE_PATHS[index]}"
    backup="${target}${BACKUP_SUFFIX}"
    if [[ -e "${backup}" || -L "${backup}" ]]; then
        echo "Stale backup exists: ${backup}. Restore or remove it before rerunning." >&2
        exit 2
    fi
    if [[ ! -e "${target}" && ! -L "${target}" ]]; then
        echo "CANN interface path does not exist: ${target}" >&2
        exit 2
    fi
    if [[ ! -e "${REPO_PATHS[index]}" ]]; then
        echo "Repository interface path does not exist: ${REPO_PATHS[index]}" >&2
        exit 2
    fi
done

for index in "${!PACKAGE_PATHS[@]}"; do
    target="${PACKAGE_PATHS[index]}"
    mv "${target}" "${target}${BACKUP_SUFFIX}"
    SWAPPED_PATHS+=("${index}")
    ln -s "${REPO_PATHS[index]}" "${target}"
done

source "${CANN_ROOT}/set_env.sh"

mkdir -p "${BUILD_DIR}"

for mode in "${MODES[@]}"; do
    echo "[ BUILD ] ${mode}"
    bisheng "${SCRIPT_DIR}/sync_${mode}.asc" -o "${BUILD_DIR}/sync_${mode}" --npu-arch=dav-3510
done

for mode in "${MODES[@]}"; do
    echo "[ RUN  ] ${mode}"
    if timeout --signal=TERM --kill-after=5 "${RUN_TIMEOUT}" "${BUILD_DIR}/sync_${mode}"; then
        echo "[ PASS ] ${mode}"
    else
        status=$?
        echo "[ FAIL ] ${mode} (exit=${status})" >&2
        FAILED_MODES+=("${mode}")
    fi
done

if [[ "${#FAILED_MODES[@]}" -ne 0 ]]; then
    echo "Failed modes: ${FAILED_MODES[*]}" >&2
    exit 1
fi

echo "All four sync modes passed on dav-3510."
