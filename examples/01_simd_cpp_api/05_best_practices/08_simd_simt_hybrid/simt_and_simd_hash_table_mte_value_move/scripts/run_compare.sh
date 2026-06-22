#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
SAMPLE_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
BUILD_ROOT="${SAMPLE_DIR}/build"
DIMS=(16 32 64 128 256)

run_case() {
    local scenario=$1
    local name=$2
    local build_dir="${BUILD_ROOT}/scenario_${scenario}"
    local launch_count=1
    if [[ "${scenario}" == "1" ]]; then
        launch_count=2
    fi

    rm -rf "${build_dir}"
    cmake -S "${SAMPLE_DIR}" -B "${build_dir}" -DSCENARIO_NUM="${scenario}" -DPROFILE_REPEAT_TIMES=1
    cmake --build "${build_dir}" -j

    for dim in "${DIMS[@]}"; do
        local run_dir="${build_dir}/profile_dim_${dim}"
        local verify_log="${run_dir}/${name}_dim_${dim}_verify.log"
        local log_file="${run_dir}/${name}_dim_${dim}.log"
        rm -rf "${run_dir}"
        mkdir -p "${run_dir}"

        "${build_dir}/hash_table_mte_value_move" "${dim}" 2>&1 | tee "${verify_log}"
        grep -q "Verification PASSED" "${verify_log}"
        grep -q "| ${scenario} | ${dim} |" "${verify_log}"

        (
            cd "${run_dir}"
            msprof op --warm-up=0 --kill=on --launch-count="${launch_count}" \
                "${build_dir}/hash_table_mte_value_move" "${dim}" 2>&1 |
                tee "${log_file}"
        )
        if [[ "${scenario}" == "1" ]]; then
            grep -q "write_values_mte_queue_kernel" "${log_file}"
        elif [[ "${scenario}" == "2" ]]; then
            grep -q "insert_or_assign_mte_task_queue_kernel" "${log_file}"
        fi
        find "${run_dir}" -maxdepth 1 -type d -name "OPPROF_*" | grep -q .
    done
}

run_case 0 "simt_group_insert_or_assign"
run_case 1 "mte_two_stage_insert_or_assign"
run_case 2 "mte_task_queue_insert_or_assign"
