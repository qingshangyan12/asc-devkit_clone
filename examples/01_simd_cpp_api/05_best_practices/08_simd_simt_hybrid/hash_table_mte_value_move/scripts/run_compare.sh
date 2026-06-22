#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
SAMPLE_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
BUILD_ROOT="${SAMPLE_DIR}/build"

run_case() {
    local scenario=$1
    local name=$2
    local build_dir="${BUILD_ROOT}/scenario_${scenario}"

    rm -rf "${build_dir}"
    cmake -S "${SAMPLE_DIR}" -B "${build_dir}" -DSCENARIO_NUM="${scenario}"
    cmake --build "${build_dir}" -j

    "${build_dir}/hash_table_mte_value_move" | tee "${build_dir}/${name}.log"
    grep -q "Verification PASSED" "${build_dir}/${name}.log"
    grep -q "### HashTable insert_or_assign benchmark" "${build_dir}/${name}.log"
    grep -q "Throughput(Mops/s)" "${build_dir}/${name}.log"
    grep -q "Effective Value BW(GB/s)" "${build_dir}/${name}.log"
}

run_case 0 "simt_group_insert_or_assign"
run_case 1 "mte_queue_insert_or_assign"
