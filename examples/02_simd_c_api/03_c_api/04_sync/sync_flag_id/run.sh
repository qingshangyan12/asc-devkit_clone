#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(git -C "${SCRIPT_DIR}" rev-parse --show-toplevel)
NPU_ARCH=${1:-dav-2201}
CANN_ROOT=${ASCEND_HOME_PATH:-${ASCEND_TOOLKIT_HOME:-}}
RUN_TIMEOUT=${SYNC_TEST_TIMEOUT:-60}

if [[ "${NPU_ARCH}" != "dav-2201" && "${NPU_ARCH}" != "dav-3510" ]]; then
    echo "Usage: $0 [dav-2201|dav-3510]" >&2
    exit 2
fi

if [[ -z "${CANN_ROOT}" || ! -f "${CANN_ROOT}/set_env.sh" ]]; then
    echo "ASCEND_HOME_PATH or ASCEND_TOOLKIT_HOME must point to a valid CANN installation." >&2
    exit 2
fi

CANN_ASC_ROOT="${CANN_ROOT}/asc"
PACKAGE_INCLUDE="${CANN_ASC_ROOT}/include/c_api"
PACKAGE_IMPL="${CANN_ASC_ROOT}/impl/c_api"
REPO_INCLUDE="${REPO_ROOT}/include/c_api"
REPO_IMPL="${REPO_ROOT}/impl/c_api"
BACKUP_SUFFIX=".sync_flag_id_backup"
LINKS_CREATED=0

restore_package()
{
    if [[ "${LINKS_CREATED}" -ne 1 ]]; then
        return
    fi
    for target in "${PACKAGE_INCLUDE}" "${PACKAGE_IMPL}"; do
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

for target in "${PACKAGE_INCLUDE}" "${PACKAGE_IMPL}"; do
    backup="${target}${BACKUP_SUFFIX}"
    if [[ -e "${backup}" || -L "${backup}" ]]; then
        echo "Stale backup exists: ${backup}. Restore or remove it before rerunning." >&2
        exit 2
    fi
    if [[ ! -e "${target}" && ! -L "${target}" ]]; then
        echo "CANN interface path does not exist: ${target}" >&2
        exit 2
    fi
done

mv "${PACKAGE_INCLUDE}" "${PACKAGE_INCLUDE}${BACKUP_SUFFIX}"
mv "${PACKAGE_IMPL}" "${PACKAGE_IMPL}${BACKUP_SUFFIX}"
LINKS_CREATED=1
ln -s "${REPO_INCLUDE}" "${PACKAGE_INCLUDE}"
ln -s "${REPO_IMPL}" "${PACKAGE_IMPL}"

source "${CANN_ROOT}/set_env.sh"

BUILD_DIR="${SCRIPT_DIR}/build-${NPU_ARCH}"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_ASC_ARCHITECTURES="${NPU_ARCH}" \
    -DCMAKE_ASC_RUN_MODE=npu
cmake --build "${BUILD_DIR}" --parallel

if [[ "${SYNC_BUILD_ONLY:-0}" == "1" ]]; then
    exit 0
fi

modes=(block subblock inter)
if [[ "${NPU_ARCH}" == "dav-3510" ]]; then
    modes+=(intra)
fi

for mode in "${modes[@]}"; do
    set +e
    timeout --signal=TERM --kill-after=5 "${RUN_TIMEOUT}" "${BUILD_DIR}/demo" "${mode}"
    run_status=$?
    set -e

    if [[ "${run_status}" -eq 124 || "${run_status}" -eq 137 ]]; then
        echo "${mode} example timed out after ${RUN_TIMEOUT}s. Check the paired flag_id values." >&2
        exit "${run_status}"
    fi
    if [[ "${run_status}" -ne 0 ]]; then
        exit "${run_status}"
    fi
done
