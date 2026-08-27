#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../../.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$SCRIPT_DIR/build}"
CANN_ROOT="${CANN_ROOT:-/home/developer/Ascend/cann-9.2.0}"
EXECUTABLE="$BUILD_DIR/asc_disable_dma_atomic_example"

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

echo "[Build] 配置并编译asc_disable_dma_atomic用例"
cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DASCEND_CANN_PACKAGE_PATH="$CANN_ROOT"
cmake --build "$BUILD_DIR" -j

echo "[Run] 执行asc_disable_dma_atomic用例"
"$EXECUTABLE"
