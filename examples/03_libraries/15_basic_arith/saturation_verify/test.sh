#!/bin/bash

INPUT_SIZE=128
INPUT_API_ID=${INPUT_API_ID:-1}
INPUT_API_TYPE=${INPUT_API_TYPE:-1}

echo "========================================="
echo "INPUT_SIZE:     $INPUT_SIZE"
echo "INPUT_API_ID:   $INPUT_API_ID (1: exp, 2: mul)"
echo "INPUT_API_TYPE: $INPUT_API_TYPE (1: Unary单目, 2: Binary双目)"
echo "========================================="

mkdir -p build && cd build

echo "====== start compile ======"
cmake .. -DINPUT_SIZE=$INPUT_SIZE -DINPUT_API_ID=$INPUT_API_ID -DINPUT_API_TYPE=$INPUT_API_TYPE
make -j

echo "====== start gen golden ======"
python3 ../scripts/gen_data.py -inputSize=$INPUT_SIZE -apiId=$INPUT_API_ID

echo "====== start run kernel ======"
./demo

echo "====== start compare result ======"
python3 ../scripts/verify_result.py -api_id=$INPUT_API_ID
