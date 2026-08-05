#!/usr/bin/env bash

set -euo pipefail

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

cmake -S "${script_dir}" -B "${script_dir}/build"
cmake --build "${script_dir}/build" -j

cd "${script_dir}/build"
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
