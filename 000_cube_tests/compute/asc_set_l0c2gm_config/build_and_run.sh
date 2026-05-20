#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source ~/Ascend/ascend-toolkit/set_env.sh 2>/dev/null
BUILD_DIR="${SCRIPT_DIR}/build"
echo "=== asc_set_l0c2gm_config 一键编译运行 ==="
mkdir -p "${BUILD_DIR}"
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"
make -j -C "${BUILD_DIR}"
echo ""
LD_LIBRARY_PATH="/home/jinhu/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav-3510/lib:${LD_LIBRARY_PATH}" "${BUILD_DIR}/cube_asc_set_l0c2gm_config"
