#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(git -C "${SCRIPT_DIR}" rev-parse --show-toplevel)
CANN_ROOT=${ASCEND_HOME_PATH:-${ASCEND_TOOLKIT_HOME:-}}
RUN_TIMEOUT=${SYNC_UT_TIMEOUT:-1800}

if [[ -z "${CANN_ROOT}" || ! -f "${CANN_ROOT}/set_env.sh" ]]; then
    echo "ASCEND_HOME_PATH or ASCEND_TOOLKIT_HOME must point to a valid CANN installation." >&2
    exit 2
fi

source "${CANN_ROOT}/set_env.sh"

set +e
timeout --signal=TERM --kill-after=10 "${RUN_TIMEOUT}" bash "${REPO_ROOT}/build.sh" --basic_test_five
run_status=$?
set -e

if [[ "${run_status}" -eq 124 || "${run_status}" -eq 137 ]]; then
    echo "C API UT timed out after ${RUN_TIMEOUT}s." >&2
fi
exit "${run_status}"
