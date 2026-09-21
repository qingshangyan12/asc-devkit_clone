#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PROJECT_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
BUILD_DIR=${BUILD_DIR:-"${PROJECT_DIR}/build_issue_1692"}

if [[ -z "${ASCEND_HOME_PATH:-}" && -z "${ASCEND_TOOLKIT_HOME:-}" ]]; then
    echo "ASCEND_HOME_PATH or ASCEND_TOOLKIT_HOME must be set" >&2
    exit 1
fi

if [[ -z "${ASCEND_CANN_PACKAGE_PATH:-}" ]]; then
    export ASCEND_CANN_PACKAGE_PATH=${ASCEND_HOME_PATH:-${ASCEND_TOOLKIT_HOME}}
fi

cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" \
    -DENABLE_TEST=ON \
    -DBUILD_OPEN_PROJECT=ON \
    -DBUILD_WITH_INSTALLED_DEPENDENCY_CANN_PKG=ON \
    -DASCEND_CANN_PACKAGE_PATH="${ASCEND_CANN_PACKAGE_PATH}"

cmake --build "${BUILD_DIR}" --target ascendc_run_all_header_checks_npu -- -j"${JOBS:-$(nproc)}"

echo "Issue 1692 UT passed: DataCacheCleanAndInvalid(LocalTensor) header checks completed."
