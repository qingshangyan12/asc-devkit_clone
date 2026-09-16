#!/usr/bin/env bash
set -euo pipefail
test_dir=$(cd "$(dirname "$0")" && pwd)
repo=$(cd "$test_dir/../../../.." && pwd)
out=${1:?pass a build/output directory}
mkdir -p "$out"
python3 "$repo/impl/adv_api/cmake/scripts/gen_kernel_tiling_data_def.py" "$repo/include/adv_api" "$out/generated/kernel_tiling/kernel_tiling.h"
for test_case in 1 2 3 4 5; do
    if bisheng -x cce -std=c++17 --cce-aicore-only --cce-aicore-arch=dav-c310-cube \
        -I"$repo/include" -I"$repo" -I"$repo/include/basic_api" -I"$repo/include/utils" \
        -I"$out/generated" \
        -DNEGATIVE_CASE="$test_case" -c "$test_dir/negative.asc" -o "$out/$test_case.o" > "$out/$test_case.log" 2>&1; then
        echo "FAIL: invalid operand case $test_case compiled"
        exit 1
    fi
    grep -Eq 'Invalid CUBE|Invalid LoadData|writable|const|no matching function|address space' "$out/$test_case.log"
    echo "PASS: invalid operand case $test_case rejected"
done
