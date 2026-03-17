#!/bin/bash

INPUT_SIZE=${INPUT_SIZE:-128}
INPUT_API_ID=${INPUT_API_ID:-1}
INPUT_API_TYPE=${INPUT_API_TYPE:-1}

case $INPUT_API_ID in
    1) EXPECTED_TYPE=1 ;;
    2) EXPECTED_TYPE=2 ;;
    *) echo "Error: Unknown API_ID=$INPUT_API_ID"; exit 1 ;;
esac

if [ "$INPUT_API_TYPE" != "$EXPECTED_TYPE" ]; then
    echo "Error: API_ID=$INPUT_API_ID expects API_TYPE=$EXPECTED_TYPE, but got API_TYPE=$INPUT_API_TYPE"
    echo "  Exp/Sqrt/Ln/Abs (API_ID 1/3/4/5) should use API_TYPE=1 (Unary)"
    echo "  Mul/Add/Sub/Div (API_ID 2/6/7/8) should use API_TYPE=2 (Binary)"
    exit 1
fi

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
