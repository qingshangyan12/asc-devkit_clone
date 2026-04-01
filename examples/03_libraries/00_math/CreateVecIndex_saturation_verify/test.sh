#!/bin/bash

INPUT_SIZE=${INPUT_SIZE:-128}
INPUT_DATA_TYPE=${INPUT_DATA_TYPE:-0}

TYPE_NAMES=("unknown" "float16" "float32" "int16" "int32")

run_test() {
    local data_type=$1
    echo ""
    echo "========================================="
    echo "INPUT_SIZE:     $INPUT_SIZE"
    echo "INPUT_DATA_TYPE: $data_type (${TYPE_NAMES[$data_type]})"
    echo "========================================="

    mkdir -p build && cd build

    echo "====== start compile ======"
    cmake .. -DINPUT_SIZE=$INPUT_SIZE -DINPUT_DATA_TYPE=$data_type
    make -j

    echo "====== start gen golden ======"
    python3 ../scripts/gen_data.py -inputSize=$INPUT_SIZE -inputType=$data_type

    echo "====== start run kernel ======"
    ./demo

    echo "====== start compare result ======"
    python3 ../scripts/verify_result.py -output_type=$data_type
    
    cd ..
}

if [ "$INPUT_DATA_TYPE" -eq 0 ]; then
    echo "========================================="
    echo "Running all data types for CreateVecIndex"
    echo "========================================="
    for dt in 1 2 3 4; do
        run_test $dt
    done
else
    run_test $INPUT_DATA_TYPE
fi