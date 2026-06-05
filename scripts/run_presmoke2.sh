#!/bin/bash
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

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
ASCEND_HOME_DIR="${ASCEND_HOME_DIR:-/usr/local/Ascend/ascend-toolkit/latest}"

set +u
if [ -f "${ASCEND_HOME_DIR}/set_env.sh" ]; then
    # shellcheck disable=SC1090
    source "${ASCEND_HOME_DIR}/set_env.sh"
elif [ -f "/usr/local/Ascend/cann/set_env.sh" ]; then
    # shellcheck disable=SC1091
    source "/usr/local/Ascend/cann/set_env.sh"
else
    echo "Error: Ascend environment file not found. Set ASCEND_HOME_DIR or install CANN under /usr/local/Ascend/cann." >&2
    exit 2
fi
set -u

export PYTHONPATH="${SCRIPT_DIR}${PYTHONPATH:+:${PYTHONPATH}}"
export PRESMOKE_PROJECT_ROOT="${PROJECT_ROOT}"

exec python3 -m presmoke "$@"
