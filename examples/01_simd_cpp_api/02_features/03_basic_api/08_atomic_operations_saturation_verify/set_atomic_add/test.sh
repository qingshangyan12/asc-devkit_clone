#!/bin/bash

set -e

echo "Building..."
mkdir -p build && cd build && cmake .. && make -j
cd ..

echo ""
echo "========================================"
echo "Step 1: Generate input & golden data"
echo "========================================"
python3 scripts/gen_data.py

echo ""
echo "========================================"
echo "Step 2: Run demo (saturation mode)"
echo "========================================"

DTYPES=("int8_t" "int16_t" "half" "bfloat16_t" "int32_t" "float")
INDICES=(0 1 2 3 4 5)

for i in "${!DTYPES[@]}"; do
    dtype="${DTYPES[$i]}"
    idx="${INDICES[$i]}"
    echo "Running sat mode for $dtype..."
    ./build/demo $idx 1 || true
done

echo ""
echo "========================================"
echo "Step 3: Run demo (non-saturation mode)"
echo "========================================"

for i in "${!DTYPES[@]}"; do
    dtype="${DTYPES[$i]}"
    idx="${INDICES[$i]}"
    echo "Running non-sat mode for $dtype..."
    ./build/demo $idx 0 || true
done

echo ""
echo "========================================"
echo "Step 4: Verify results"
echo "========================================"
python3 scripts/verify_result.py
