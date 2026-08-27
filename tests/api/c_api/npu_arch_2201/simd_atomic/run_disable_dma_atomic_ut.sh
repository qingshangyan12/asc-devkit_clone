#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../../../" && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build-ut-2201}"
CANN_ROOT="${CANN_ROOT:-/home/developer/Ascend/cann-9.2.0}"

ASC_LINK="$CANN_ROOT/asc"
ASC_LINK_BACKUP="$CANN_ROOT/asc.asc-devkit-backup.$$"
ASC_LINK_LOCK="$CANN_ROOT/.asc-devkit-link.lock"

restore_asc_link()
{
    if [[ -L "$ASC_LINK_BACKUP" ]]; then
        rm -f "$ASC_LINK"
        mv "$ASC_LINK_BACKUP" "$ASC_LINK"
    fi
    rmdir "$ASC_LINK_LOCK" 2>/dev/null || true
}

if ! mkdir "$ASC_LINK_LOCK"; then
    echo "CANN包正在被另一个任务使用，无法安全切换$ASC_LINK" >&2
    exit 1
fi
trap restore_asc_link EXIT INT TERM

if [[ ! -L "$ASC_LINK" ]]; then
    echo "$ASC_LINK不是符号链接，拒绝修改CANN包" >&2
    exit 1
fi
mv "$ASC_LINK" "$ASC_LINK_BACKUP"
ln -s "$PROJECT_ROOT" "$ASC_LINK"

source "$CANN_ROOT/set_env.sh"
cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" \
    -DENABLE_TEST=ON \
    -DBUILD_OPEN_PROJECT=ON \
    -DASCEND_CANN_PACKAGE_PATH="$CANN_ROOT" \
    -DTEST_MOD=ascendc_ut_c_api_ascend910B1_AIV
cmake --build "$BUILD_DIR" --target ascendc_ut_c_api_ascend910B1_AIV -j
"$BUILD_DIR/tests/api/c_api/ascendc_ut_c_api_ascend910B1_AIV" \
    --gtest_filter='TestDisableDmaAtomicCApi.*'
