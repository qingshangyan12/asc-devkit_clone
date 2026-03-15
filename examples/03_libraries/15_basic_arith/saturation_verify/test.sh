#!/bin/bash

INPUT_TYPE=${INPUT_TYPE:-1}
INPUT_SIZE=${INPUT_SIZE:-224}
INPUT_API_ID=${INPUT_API_ID:-1}

echo "========================================="
echo "INPUT_TYPE:  $INPUT_TYPE (1: half, 2: float, 3: int16, 4: int32)"
echo "INPUT_SIZE:  $INPUT_SIZE"
echo "INPUT_API_ID: $INPUT_API_ID (1: exp, 2: mul)"
echo "========================================="

mkdir -p build && cd build

echo "====== start compile ======"
cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE -DINPUT_API_ID=$INPUT_API_ID
make -j

echo "====== start gen golden ======"
python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE -apiId=$INPUT_API_ID

echo "====== start run kernel ======"
./demo

echo "====== start compare result ======"
python3 ../scripts/verify_result.py -output_type=$INPUT_TYPE -output=output/output.bin -golden=output/golden.bin -api_id=$INPUT_API_ID
