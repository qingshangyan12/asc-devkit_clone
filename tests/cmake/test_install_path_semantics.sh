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

REPO_ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
INSTALL_SCRIPT="${REPO_ROOT}/cmake/asc/fwk_modules/scripts/install.sh"
TEST_ROOT=$(mktemp -d)

cleanup()
{
    chmod -R u+w "${TEST_ROOT}" 2>/dev/null || true
    rm -rf "${TEST_ROOT}"
}
trap cleanup EXIT

PACKAGE_ROOT="${TEST_ROOT}/package"
DEFAULT_OPP_PATH="${TEST_ROOT}/default_opp"
PLAIN_DEFAULT_OPP_PATH="${TEST_ROOT}/plain_default_opp"
EXPLICIT_INSTALL_PATH="${TEST_ROOT}/explicit_install"
CUSTOM_OPP_PATH_A="${TEST_ROOT}/custom_a/vendors/customize"
CUSTOM_OPP_PATH_B="${TEST_ROOT}/custom_b/vendors/customize"
DEFAULT_INSTALL_LOG="${TEST_ROOT}/default_install.log"
PLAIN_INSTALL_LOG="${TEST_ROOT}/plain_install.log"
EXPLICIT_INSTALL_LOG="${TEST_ROOT}/explicit_install.log"
MISSING_DEFAULT_LOG="${TEST_ROOT}/missing_default.log"

mkdir -p \
    "${PACKAGE_ROOT}/packages/vendors/customize" \
    "${DEFAULT_OPP_PATH}" \
    "${PLAIN_DEFAULT_OPP_PATH}" \
    "${CUSTOM_OPP_PATH_A}" \
    "${CUSTOM_OPP_PATH_B}"
printf 'version=1.0\n' > "${PACKAGE_ROOT}/packages/vendors/customize/version.info"
printf '#!/usr/bin/env bash\n' > "${PACKAGE_ROOT}/uninstall.sh"

set +e
(
    cd "${PACKAGE_ROOT}"
    ASCEND_OPP_PATH="${DEFAULT_OPP_PATH}" \
    ASCEND_CUSTOM_OPP_PATH="${CUSTOM_OPP_PATH_A}:${CUSTOM_OPP_PATH_B}" \
        bash "${INSTALL_SCRIPT}" --quiet --force > "${DEFAULT_INSTALL_LOG}" 2>&1
)
INSTALL_STATUS=$?
set -e

if [ "${INSTALL_STATUS}" -ne 0 ]; then
    cat "${DEFAULT_INSTALL_LOG}"
    echo "FAIL: installer rejected a runtime-only ASCEND_CUSTOM_OPP_PATH"
    exit 1
fi

test -f "${DEFAULT_OPP_PATH}/vendors/customize/version.info"
test ! -e "${CUSTOM_OPP_PATH_A}/vendors"
test ! -e "${CUSTOM_OPP_PATH_B}/vendors"
grep -Fq "ASCEND_CUSTOM_OPP_PATH is a runtime search path and does not control the installation destination." \
    "${DEFAULT_INSTALL_LOG}"
grep -Fq "Operators in ASCEND_CUSTOM_OPP_PATH take precedence over operators installed in the default path." \
    "${DEFAULT_INSTALL_LOG}"

(
    cd "${PACKAGE_ROOT}"
    env -u ASCEND_CUSTOM_OPP_PATH \
        ASCEND_OPP_PATH="${PLAIN_DEFAULT_OPP_PATH}" \
        bash "${INSTALL_SCRIPT}" --quiet --force > "${PLAIN_INSTALL_LOG}" 2>&1
)

test -f "${PLAIN_DEFAULT_OPP_PATH}/vendors/customize/version.info"
if grep -Fq "ASCEND_CUSTOM_OPP_PATH is a runtime search path" "${PLAIN_INSTALL_LOG}"; then
    echo "FAIL: installer warned about ASCEND_CUSTOM_OPP_PATH when it was not set"
    exit 1
fi

(
    cd "${PACKAGE_ROOT}"
    env -u ASCEND_OPP_PATH \
        ASCEND_CUSTOM_OPP_PATH="${CUSTOM_OPP_PATH_A}:${CUSTOM_OPP_PATH_B}" \
        bash "${INSTALL_SCRIPT}" --quiet --force \
        --install-path="${EXPLICIT_INSTALL_PATH}" > "${EXPLICIT_INSTALL_LOG}" 2>&1
)

test -f "${EXPLICIT_INSTALL_PATH}/vendors/customize/version.info"
grep -Fq \
    "export ASCEND_CUSTOM_OPP_PATH=${EXPLICIT_INSTALL_PATH}/vendors/customize:\${ASCEND_CUSTOM_OPP_PATH}" \
    "${EXPLICIT_INSTALL_PATH}/vendors/customize/bin/set_env.bash"
if grep -Fq "does not control the installation destination" "${EXPLICIT_INSTALL_LOG}"; then
    echo "FAIL: installer emitted a default-path warning for --install-path"
    exit 1
fi

set +e
(
    cd "${PACKAGE_ROOT}"
    env -u ASCEND_OPP_PATH -u ASCEND_CUSTOM_OPP_PATH \
        bash "${INSTALL_SCRIPT}" --quiet --force > "${MISSING_DEFAULT_LOG}" 2>&1
)
MISSING_DEFAULT_STATUS=$?
set -e

if [ "${MISSING_DEFAULT_STATUS}" -eq 0 ]; then
    echo "FAIL: installer accepted a default install without ASCEND_OPP_PATH"
    exit 1
fi
grep -Fq "env ASCEND_OPP_PATH no exist" "${MISSING_DEFAULT_LOG}"

echo "PASS: custom-op installation path semantics"
