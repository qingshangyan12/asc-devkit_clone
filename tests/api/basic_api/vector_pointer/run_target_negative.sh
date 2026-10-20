#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../../.." && pwd)
logs=${1:-"${root}/tests/api/basic_api/vector_pointer/negative_logs"}
mkdir -p "${logs}"
flags=(-x asc --npu-arch=dav-3510 -std=c++17 -O2 -c)
for dir in . include include/basic_api include/utils impl impl/basic_api; do
    flags+=(-iquote "${root}/${dir}")
done
flags+=(-I "${root}")
compiler=${ASC_COMPILER:-bisheng}
source_file="${root}/tests/api/basic_api/vector_pointer/negative_operands.cpp"
: > "${logs}/result.txt"
for test_case in 0 1 2 3 4 5; do
    if "${compiler}" "${flags[@]}" "-DVECTOR_NEGATIVE_CASE=${test_case}" "${source_file}" -o "${logs}/case${test_case}.o" > "${logs}/case${test_case}.log" 2>&1; then
        if (( test_case != 0 )); then
            echo "FAIL: invalid case ${test_case} compiled"; exit 1
        fi
    else
        if (( test_case == 0 )); then
            cat "${logs}/case0.log"; exit 1
        fi
        grep -Eq 'error: (no matching function|static assertion)' "${logs}/case${test_case}.log"
    fi
    echo "PASS operand constraint case=${test_case}" | tee -a "${logs}/result.txt"
done
