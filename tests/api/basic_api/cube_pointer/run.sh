#!/usr/bin/env bash
set -euo pipefail
test_dir=$(cd "$(dirname "$0")" && pwd)
build_root=${1:?pass a build/output directory}
mkdir -p "$build_root"
: > "$build_root/results.jsonl"
for dtype in fp16 bf16; do
for side in 16 32 64; do
    for mode in 0 1 2; do
        build_dir="$build_root/${dtype}_side${side}_mode${mode}"
        bf16=OFF
        if [[ "$dtype" == bf16 ]]; then bf16=ON; fi
        cmake -S "$test_dir" -B "$build_dir" -DCUBE_SIDE="$side" -DCUBE_POINTER_MODE="$mode" -DCUBE_BFLOAT16="$bf16" > "$build_root/configure_${dtype}_${side}_${mode}.log"
        cmake --build "$build_dir" -j2 > "$build_root/build_${dtype}_${side}_${mode}.log" 2>&1
        repo=$(cd "$test_dir/../../../.." && pwd)
        grep -Fq "$repo/include/basic_api/kernel_cube_intf.h" "$build_dir/CMakeFiles/cube_pointer_validation.dir/runner.asc.o.d"
        "$build_dir/cube_pointer_validation" | tee -a "$build_root/results.jsonl"
    done
done
done
