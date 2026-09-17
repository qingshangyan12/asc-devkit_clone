#!/usr/bin/env bash
set -euo pipefail
build_root=${1:?pass run.sh build directory}
objcopy=${ASCEND_HOME_PATH:?source CANN set_env.sh}/tools/bisheng_compiler/bin/llvm-objcopy
for dtype in fp16 bf16; do
    for side in 16 32 64; do
        for mode in 0 1 2; do
            directory="$build_root/${dtype}_side${side}_mode${mode}"
            "$objcopy" --dump-section ".aicore_binary=$directory/kernel.elf" "$directory/cube_pointer_validation"
            "$objcopy" --dump-section ".text=$directory/kernel.text" "$directory/kernel.elf"
        done
        cmp "$build_root/${dtype}_side${side}_mode0/kernel.text" "$build_root/${dtype}_side${side}_mode1/kernel.text"
        cmp "$build_root/${dtype}_side${side}_mode0/kernel.text" "$build_root/${dtype}_side${side}_mode2/kernel.text"
        echo "PASS: $dtype side $side Tensor/pointer/mixed machine code identical"
    done
done
