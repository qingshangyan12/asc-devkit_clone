#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Step 1: Build ==="
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..
make -j

echo "=== Step 2: Generate test data ==="
python3 ../scripts/gen_data.py

echo "=== Step 3: Run kernel ==="
./demo

echo "=== Step 4: Verify result ==="5
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
