#!/bin/bash
set -e

cd "$(dirname "$0")"

mkdir -p build && cd build
cmake .. && make -j
python3 ../scripts/gen_data.py
./demo
