#!/bin/bash

mkdir -p build && cd build && cmake .. && make -j

python3 ../scripts/gen_data.py

DTYPES=("int8_t" "int16_t" "half" "bfloat16_t" "int32_t" "float")
DTYPE_INDICES=(0 1 2 3 4 5)

all_passed=true

for i in "${!DTYPES[@]}"; do
    dtype="${DTYPES[$i]}"
    idx="${DTYPE_INDICES[$i]}"
    echo "========================================"
    echo "Testing dtype: $dtype (index: $idx)"
    echo "========================================"
    ./demo $idx
    if [ $? -ne 0 ]; then
        echo "FAILED: ./demo returned non-zero"
        all_passed=false
        continue
    fi
    python3 ../scripts/verify_result.py "output/output_$dtype.bin" "output/golden_$dtype.bin" "$dtype"
    if [ $? -ne 0 ]; then
        echo "FAILED: $dtype verification failed"
        all_passed=false
    else
        echo "PASSED: $dtype"
    fi
done

if [ "$all_passed" = true ]; then
    echo ""
    echo "========================================"
    echo "All tests passed!"
    echo "========================================"
else
    echo ""
    echo "========================================"
    echo "Some tests failed!"
    echo "========================================"
    exit 1
fi
