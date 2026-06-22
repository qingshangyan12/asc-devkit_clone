#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "=== cast_count0 test ==="
echo ""

mkdir -p "${BUILD_DIR}"

echo "[1/2] cmake..."
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"

echo ""
echo "[2/2] make..."
make -j -C "${BUILD_DIR}"

echo ""
echo "=== run ==="
echo ""
"${BUILD_DIR}/demo"
