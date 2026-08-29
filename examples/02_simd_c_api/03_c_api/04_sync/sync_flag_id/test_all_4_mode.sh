#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(git -C "${SCRIPT_DIR}" rev-parse --show-toplevel)
CANN_ROOT=${ASCEND_HOME_PATH:-${ASCEND_TOOLKIT_HOME:-}}
RUN_TIMEOUT=${SYNC_TEST_TIMEOUT:-60}
BUILD_DIR="${SCRIPT_DIR}/build-dav-3510"
MODES=(block subblock inter intra)
FAILED_MODES=()
CONSTEXPR_FLAG_ID_MODE=${1:-0}

if [[ "$#" -gt 1 || ( "${CONSTEXPR_FLAG_ID_MODE}" != 0 && "${CONSTEXPR_FLAG_ID_MODE}" != 1 ) ]]; then
    echo "Usage: $0 [0|1]" >&2
    exit 2
fi

BISHENG_OPTIONS=()
if [[ "${CONSTEXPR_FLAG_ID_MODE}" == 1 ]]; then
    BISHENG_OPTIONS+=("-DSYNC_FLAG_ID_CONSTEXPR=1")
fi

if [[ -z "${CANN_ROOT}" || ! -f "${CANN_ROOT}/set_env.sh" ]]; then
    echo "ASCEND_HOME_PATH or ASCEND_TOOLKIT_HOME must point to a valid CANN installation." >&2
    exit 2
fi

CANN_ASC_ROOT="${CANN_ROOT}/asc"
REPO_PATHS=(
    "${REPO_ROOT}/include/c_api"
    "${REPO_ROOT}/impl/c_api"
    "${REPO_ROOT}/include/utils"
    "${REPO_ROOT}/impl/utils"
)
OVERLAY_FILE=$(mktemp "${TMPDIR:-/tmp}/sync_flag_id.XXXXXX.vfsoverlay")

cleanup()
{
    rm -f "${OVERLAY_FILE}"
}

create_vfs_overlay()
{
    local first_entry=1
    local repo_path
    local file_path
    local relative_path

    {
        printf '{\n  "version": 0,\n  "roots": [\n'
        for repo_path in "${REPO_PATHS[@]}"; do
            while IFS= read -r file_path; do
                relative_path=${file_path#"${REPO_ROOT}/"}
                if [[ "${first_entry}" -eq 0 ]]; then
                    printf ',\n'
                fi
                printf '    {"type":"file","name":"%s","external-contents":"%s","use-external-name":"false"}' \
                    "${CANN_ASC_ROOT}/${relative_path}" "${file_path}"
                first_entry=0
            done < <(find "${repo_path}" -type f | sort)
        done
        printf '\n  ]\n}\n'
    } > "${OVERLAY_FILE}"
}

trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

for repo_path in "${REPO_PATHS[@]}"; do
    if [[ ! -d "${repo_path}" ]]; then
        echo "Repository interface path does not exist: ${repo_path}" >&2
        exit 2
    fi
done

source "${CANN_ROOT}/set_env.sh"
create_vfs_overlay

mkdir -p "${BUILD_DIR}"

for mode in "${MODES[@]}"; do
    echo "[ BUILD ] ${mode}"
    bisheng -ivfsoverlay "${OVERLAY_FILE}" "${SCRIPT_DIR}/sync_${mode}.asc" -o "${BUILD_DIR}/sync_${mode}" \
        --npu-arch=dav-3510 "${BISHENG_OPTIONS[@]}"
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
