#!/bin/bash

INPUT_SIZE=${INPUT_SIZE:-128}
INPUT_API_ID=${INPUT_API_ID:-1}
INPUT_API_TYPE=${INPUT_API_TYPE:-1}
INPUT_DATA_TYPE=${INPUT_DATA_TYPE:-0}

TYPE_NAMES=("unknown" "float16" "float32" "int16" "int32")

SUPPORTED_TYPES_MAP=(
    ""              # placeholder for index 0
    "1 2"           # 1: exp -> half, float
    "1 2 3 4"       # 2: mul -> half, float, int16, int32
    "1 2"           # 3: sqrt -> half, float
    "1 2"           # 4: rsqrt -> half, float
    "1 2"           # 5: relu -> half, float
    "1 2 3 4"       # 6: add -> half, float, int16, int32
    "1 2 3 4"       # 7: sub -> half, float, int16, int32
    "1 2"           # 8: div -> half, float
    "1 2 3 4"       # 9: max -> half, float, int16, int32
    "1 2 3 4"       # 10: min -> half, float, int16, int32
)

run_test() {
    local data_type=$1
    echo ""
    echo "========================================="
    echo "INPUT_SIZE:     $INPUT_SIZE"
    echo "INPUT_API_ID:   $INPUT_API_ID (1: exp, 2: mul, 3: sqrt, 4: rsqrt, 5: relu, 6: add, 7: sub, 8: div, 9: max, 10: min)"
    echo "INPUT_API_TYPE: $INPUT_API_TYPE (1: Unary单目, 2: Binary双目)"
    echo "INPUT_DATA_TYPE: $data_type (${TYPE_NAMES[$data_type]})"
    echo "========================================="

    mkdir -p build && cd build

    echo "====== start compile ======"
    cmake .. -DINPUT_SIZE=$INPUT_SIZE -DINPUT_API_ID=$INPUT_API_ID -DINPUT_API_TYPE=$INPUT_API_TYPE -DINPUT_DATA_TYPE=$data_type
    make -j

    echo "====== start gen golden ======"
    python3 ../scripts/gen_data.py -inputSize=$INPUT_SIZE -apiId=$INPUT_API_ID -inputType=$data_type

    echo "====== start run kernel ======"
    ./demo

    echo "====== start compare result ======"
    python3 ../scripts/verify_result.py -api_id=$INPUT_API_ID -output_type=$data_type
    
    cd ..
}

case $INPUT_API_ID in
    1|3|4|5) EXPECTED_TYPE=1 ;;
    2|6|7|8|9|10) EXPECTED_TYPE=2 ;;
    *) echo "Error: Unknown API_ID=$INPUT_API_ID"; exit 1 ;;
esac

if [ "$INPUT_API_TYPE" != "$EXPECTED_TYPE" ]; then
    echo "Error: API_ID=$INPUT_API_ID expects API_TYPE=$EXPECTED_TYPE, but got API_TYPE=$INPUT_API_TYPE"
    echo "  Exp (API_ID 1) should use API_TYPE=1 (Unary)"
    echo "  Sqrt (API_ID 3) should use API_TYPE=1 (Unary)"
    echo "  Rsqrt (API_ID 4) should use API_TYPE=1 (Unary)"
    echo "  Relu (API_ID 5) should use API_TYPE=1 (Unary)"
    echo "  Mul (API_ID 2) should use API_TYPE=2 (Binary)"
    echo "  Add (API_ID 6) should use API_TYPE=2 (Binary)"
    echo "  Sub (API_ID 7) should use API_TYPE=2 (Binary)"
    echo "  Div (API_ID 8) should use API_TYPE=2 (Binary)"
    echo "  Max (API_ID 9) should use API_TYPE=2 (Binary)"
    echo "  Min (API_ID 10) should use API_TYPE=2 (Binary)"
    exit 1
fi

if [ "$INPUT_DATA_TYPE" -eq 0 ]; then
    supported_types=${SUPPORTED_TYPES_MAP[$INPUT_API_ID]}
    if [ -z "$supported_types" ]; then
        echo "Error: No supported types defined for API_ID=$INPUT_API_ID"
        exit 1
    fi
    echo "========================================="
    echo "Running supported data types for API_ID=$INPUT_API_ID: $supported_types"
    echo "========================================="
    for dt in $supported_types; do
        run_test $dt
    done
else
    run_test $INPUT_DATA_TYPE
fi
