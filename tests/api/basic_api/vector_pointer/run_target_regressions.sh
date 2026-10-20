#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../../.." && pwd)
logs=${1:-"${root}/tests/api/basic_api/vector_pointer/runtime_logs"}
mkdir -p "${logs}"
logs=$(cd "${logs}" && pwd)
compiler=${ASC_COMPILER:-bisheng}
: "${ASCEND_HOME_PATH:?Source the CANN environment first}"
flags=(-x asc --npu-arch=dav-3510 -std=c++17 -O2)
for dir in . include include/basic_api include/utils impl impl/basic_api; do
    flags+=(-iquote "${root}/${dir}")
done
flags+=(-I "${root}")
sources="${root}/examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic"
"${compiler}" --version > "${logs}/compiler.txt" 2>&1
npu-smi info > "${logs}/device.txt" 2>&1
: > "${logs}/commands.txt"
: > "${logs}/result.txt"
: > "${logs}/sources.sha256"
for suite in elementwise conversion layout compare index transpose sort special; do
    if [[ ${suite} == elementwise ]]; then
        source_file="${sources}/vector_pointer_regression.asc"
    else
        source_file="${sources}/vector_pointer_${suite}_regression.asc"
    fi
    binary="${logs}/${suite}_regression"
    command=("${compiler}" "${flags[@]}" "${source_file}" -L"${ASCEND_HOME_PATH}/lib64" -lascendcl -lm -o "${binary}")
    printf '%q ' "${command[@]}" >> "${logs}/commands.txt"
    printf '\n' >> "${logs}/commands.txt"
    sha256sum "${source_file}" >> "${logs}/sources.sha256"
    if ! "${command[@]}" > "${logs}/${suite}_build.log" 2>&1; then
        grep -n -m 20 'error:' "${logs}/${suite}_build.log" || true
        exit 1
    fi
    printf '%q ' stdbuf -oL -eL "${binary}" >> "${logs}/commands.txt"
    printf '\n' >> "${logs}/commands.txt"
    if ! stdbuf -oL -eL "${binary}" > "${logs}/${suite}_npu.log" 2>&1; then
        tail -20 "${logs}/${suite}_npu.log"
        exit 1
    fi
    echo "PASS suite=${suite}" | tee -a "${logs}/result.txt"
done
