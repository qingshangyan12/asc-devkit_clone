#!/usr/bin/env bash

set -u

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly BUILD_DIR="${SCRIPT_DIR}/build"
readonly LOG_FILE="${SCRIPT_DIR}/stress_test_log.txt"
readonly TOTAL_RUNS=50

cleanup()
{
    rm -rf "${BUILD_DIR}"
}

trap cleanup EXIT

: > "${LOG_FILE}"
reproduced_count=0
not_reproduced_count=0

for ((run = 1; run <= TOTAL_RUNS; ++run)); do
    rm -rf "${BUILD_DIR}"
    if run_output="$(
        {
            cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_ASC_ARCHITECTURES=dav-3510 &&
            cmake --build "${BUILD_DIR}" --parallel &&
            (cd "${BUILD_DIR}" && ./demo --preload)
        } 2>&1
    )"; then
        run_status=0
    else
        run_status=$?
    fi

    {
        printf '%s\n' "===== Run ${run}/${TOTAL_RUNS} (exit=${run_status}) ====="
        printf '%s\n' "${run_output}"
    } | tee -a "${LOG_FILE}"

    if [[ "${run_output}" == *$'FIRST_SYNC_OK\nPROBE_FAIL acl_error=507035'* ]]; then
        ((++reproduced_count))
        printf '%s\n' "Result: REPRODUCED" | tee -a "${LOG_FILE}"
    else
        ((++not_reproduced_count))
        printf '%s\n' "Result: NOT_REPRODUCED" | tee -a "${LOG_FILE}"
    fi
done

{
    printf '%s\n' '===== Summary ====='
    printf '%s\n' "Total runs: ${TOTAL_RUNS}"
    printf '%s\n' "Reproduced (FIRST_SYNC_OK + PROBE_FAIL acl_error=507035): ${reproduced_count}"
    printf '%s\n' "Not reproduced: ${not_reproduced_count}"
} | tee -a "${LOG_FILE}"
