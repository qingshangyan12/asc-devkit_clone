#!/bin/bash
set -e
source /home/jinhu/Ascend/cann-9.0.0-beta.2/set_env.sh
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
echo "=== asc_mmad_sync 一键编译运行 ==="
mkdir -p "${BUILD_DIR}"
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"
make -j -C "${BUILD_DIR}"
echo ""
"${BUILD_DIR}/cube_asc_mmad_sync"
rm -rf ./*.toml
rm -rf ./*_log
rm -rf ./*.dump
