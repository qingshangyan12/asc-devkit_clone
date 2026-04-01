#!/bin/bash

INPUT_SIZE=${INPUT_SIZE:-128}
INPUT_DATA_TYPE=${INPUT_DATA_TYPE:-0}
INPUT_FIRST_VALUE_ID=${INPUT_FIRST_VALUE_ID:-0}

TYPE_NAMES=("unknown" "float16" "float32" "int16" "int32")
FIRST_VALUE_NAMES=("unknown" "min" "max" "+0.0" "-0.0" "+INF" "-INF" "NaN")

run_test() {
    local data_type=$1
    local first_value_id=$2
    echo ""
    echo "========================================="
    echo "INPUT_SIZE:          $INPUT_SIZE"
    echo "INPUT_DATA_TYPE:     $data_type (${TYPE_NAMES[$data_type]})"
    echo "INPUT_FIRST_VALUE_ID: $first_value_id (${FIRST_VALUE_NAMES[$first_value_id]})"
    echo "========================================="

    mkdir -p build && cd build

    echo "====== start compile ======"
    cmake .. -DINPUT_SIZE=$INPUT_SIZE -DINPUT_DATA_TYPE=$data_type -DINPUT_FIRST_VALUE_ID=$first_value_id
    make -j

    echo "====== start gen golden ======"
    python3 ../scripts/gen_data.py -inputSize=$INPUT_SIZE -inputType=$data_type -firstValueId=$first_value_id

    echo "====== start run kernel ======"
    ./demo

    echo "====== start compare result ======"
    python3 ../scripts/verify_result.py -output_type=$data_type
    
    cd ..
}

if [ "$INPUT_FIRST_VALUE_ID" -eq 0 ]; then
    echo "========================================="
    echo "Running all first value types for CreateVecIndex"
    echo "========================================="
    for fv in 1 2 3 4 5 6 7; do
        run_test $INPUT_DATA_TYPE $fv
    done
else
    run_test $INPUT_DATA_TYPE $INPUT_FIRST_VALUE_ID
fi