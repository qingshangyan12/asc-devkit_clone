#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "=== asc_abs 一键编译运行 ==="
echo ""

mkdir -p "${BUILD_DIR}"

echo "[1/2] cmake 配置..."
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"

echo ""
echo "[2/2] make 编译..."
make -j -C "${BUILD_DIR}"

echo ""
echo "=== 运行 ==="
echo ""
"${BUILD_DIR}/c_api_abs_example"
