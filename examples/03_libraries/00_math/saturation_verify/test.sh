#!/bin/bash

INPUT_SIZE=${INPUT_SIZE:-128}
INPUT_API_ID=${INPUT_API_ID:-1}
INPUT_API_TYPE=${INPUT_API_TYPE:-1}
INPUT_DATA_TYPE=${INPUT_DATA_TYPE:-0}

TYPE_NAMES=("unknown" "float16" "float32" "int16" "int32")
API_TYPE_NAMES=("unknown" "Unary" "Binary" "BinaryScalar")

run_test() {
    local data_type=$1
    echo ""
    echo "========================================="
    echo "INPUT_SIZE:     $INPUT_SIZE"
    echo "INPUT_API_ID:   $INPUT_API_ID (1: exp, 2: mul, 3: adds)"
    echo "INPUT_API_TYPE: $INPUT_API_TYPE (${API_TYPE_NAMES[$INPUT_API_TYPE]})"
    echo "INPUT_DATA_TYPE: $data_type (${TYPE_NAMES[$data_type]})"
    echo "========================================="

    mkdir -p build && cd build

    echo "====== start compile ======"
    cmake .. -DINPUT_SIZE=$INPUT_SIZE -DINPUT_API_ID=$INPUT_API_ID -DINPUT_API_TYPE=$INPUT_API_TYPE -DINPUT_DATA_TYPE=$data_type
    make -j

    echo "====== start gen golden ======"
    python3 ../scripts/gen_data.py -inputSize=$INPUT_SIZE -apiId=$INPUT_API_ID -inputType=$data_type -apiType=$INPUT_API_TYPE

    echo "====== start run kernel ======"
    ./demo

    echo "====== start compare result ======"
    python3 ../scripts/verify_result.py -api_id=$INPUT_API_ID -api_type=$INPUT_API_TYPE -output_type=$data_type
    
    cd ..
}

case $INPUT_API_ID in
    1) EXPECTED_TYPE=1 ;; # 单目API
    2) EXPECTED_TYPE=2 ;; # 双目API
    3) EXPECTED_TYPE=3 ;; # 双目矢量API
    *) echo "Error: Unknown API_ID=$INPUT_API_ID"; exit 1 ;;
esac

if [ "$INPUT_API_TYPE" != "$EXPECTED_TYPE" ]; then
    echo "Error: API_ID=$INPUT_API_ID expects API_TYPE=$EXPECTED_TYPE, but got API_TYPE=$INPUT_API_TYPE"
    echo "  Exp (API_ID 1) should use API_TYPE=1 (Unary)"
    echo "  Mul (API_ID 2) should use API_TYPE=2 (Binary)"
    echo "  Adds (API_ID 3) should use API_TYPE=3 (BinaryScalar)"
    exit 1
fi

if [ "$INPUT_DATA_TYPE" -eq 0 ]; then
    echo "========================================="
    echo "Running all data types for API_ID=$INPUT_API_ID"
    echo "========================================="
    for dt in 1 2 3 4; do
        run_test $dt
    done
else
    run_test $INPUT_DATA_TYPE
fi
