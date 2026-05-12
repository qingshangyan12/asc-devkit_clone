#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
src_file="${script_dir}/erfcx_cpu.cpp"
exe_file="/tmp/erfcx_test"

echo "=== Compiling erfcx_cpu.cpp ==="
g++ -std=c++17 -O2 -o "${exe_file}" "${src_file}" -lm
echo "Compilation succeeded."

echo ""
echo "=== Running tests ==="
"${exe_file}"
