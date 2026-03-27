mkdir -p build && cd build && cmake .. && make -j

python3 ../scripts/gen_data.py

DTYPES=("int8_t" "int16_t" "half" "int32_t" "float")
DTYPE_INDICES=(0 1 2 4 5)

for i in "${!DTYPES[@]}"; do
    dtype="${DTYPES[$i]}"
    idx="${DTYPE_INDICES[$i]}"
    echo "========================================"
    echo "Testing dtype: $dtype (index: $idx)"
    echo "========================================"
    ./demo $idx
    python3 ../scripts/verify_result.py "output/output_$dtype.bin" "output/golden_$dtype.bin" "$dtype"
    if [ $? -ne 0 ]; then
        echo "FAILED: $dtype"
        exit 1
    fi
    echo "PASSED: $dtype"
done

echo "All tests passed!"