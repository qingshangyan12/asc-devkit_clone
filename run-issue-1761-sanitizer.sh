#!/usr/bin/env bash

set -euo pipefail

readonly REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SAMPLE_DIR="${REPO_ROOT}/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier"
readonly CANN_INSTALL_PATH="${CANN_PATH:-/home/developer/Ascend/cann-9.2.0}"
readonly PHYSICAL_NPU_ID="${ISSUE1761_NPU_ID:-7}"
readonly CHECK_MODE="${1:-racecheck}"
readonly RESULT_DIR="${2:-$(mktemp -d /tmp/issue1761-sanitizer.XXXXXX)}"
readonly NORMAL_BUILD_DIR="${RESULT_DIR}/normal-build"
readonly SANITIZER_BUILD_DIR="${RESULT_DIR}/sanitizer-build"

die()
{
    echo "ERROR: $*" >&2
    exit 1
}

count_hazard()
{
    local hazard_type="$1"
    local log_file="$2"
    local count
    count=$(rg -c "Potential ${hazard_type} hazard" "${log_file}" 2>/dev/null || true)
    echo "${count:-0}"
}

run_sanitizer()
{
    local tool_name="$1"
    local log_file="${RESULT_DIR}/${tool_name}.log"
    local stdout_file="${RESULT_DIR}/${tool_name}.stdout.log"

    echo "Running ${tool_name}..."
    (
        cd "${SANITIZER_BUILD_DIR}"
        mssanitizer --tool="${tool_name}" --log-file="${log_file}" ./demo
    ) 2>&1 | tee "${stdout_file}"
}

[[ "${CHECK_MODE}" == "racecheck" || "${CHECK_MODE}" == "all" ]] || \
    die "Usage: $0 [racecheck|all] [result_dir]"
[[ -f "${CANN_INSTALL_PATH}/set_env.sh" ]] || \
    die "CANN set_env.sh not found: ${CANN_INSTALL_PATH}/set_env.sh"
[[ -d "${SAMPLE_DIR}" ]] || die "Sample directory not found: ${SAMPLE_DIR}"

# shellcheck disable=SC1090
source "${CANN_INSTALL_PATH}/set_env.sh"
export ASCEND_VISIBLE_DEVICES="${PHYSICAL_NPU_ID}"

command -v cmake >/dev/null || die "cmake is not available"
command -v mssanitizer >/dev/null || die "mssanitizer is not available"
command -v npu-smi >/dev/null || die "npu-smi is not available"
command -v rg >/dev/null || die "rg is not available"

mkdir -p "${RESULT_DIR}"

echo "Result directory: ${RESULT_DIR}"
echo "CANN path: ${CANN_INSTALL_PATH}"
echo "NPU architecture: dav-2201"
echo "Physical NPU: ${PHYSICAL_NPU_ID}"
npu-smi info > "${RESULT_DIR}/npu-smi.log"
mssanitizer --version > "${RESULT_DIR}/mssanitizer-version.log" 2>&1

echo "Building and running the normal binary..."
cmake \
    -S "${SAMPLE_DIR}" \
    -B "${NORMAL_BUILD_DIR}" \
    -DCMAKE_ASC_ARCHITECTURES=dav-2201 \
    2>&1 | tee "${RESULT_DIR}/normal-cmake.log"
cmake --build "${NORMAL_BUILD_DIR}" -j8 \
    2>&1 | tee "${RESULT_DIR}/normal-build.log"
(
    cd "${NORMAL_BUILD_DIR}"
    ./demo
) 2>&1 | tee "${RESULT_DIR}/normal-run.log"

normal_output_count=$(rg -c 'OUTPUT = 24' "${RESULT_DIR}/normal-run.log" || true)
[[ "${normal_output_count:-0}" -eq 6 ]] || \
    die "Functional check failed: expected 6 lines containing 'OUTPUT = 24'"

echo "Building the sanitizer-instrumented binary..."
cmake \
    -S "${SAMPLE_DIR}" \
    -B "${SANITIZER_BUILD_DIR}" \
    -DCMAKE_ASC_ARCHITECTURES=dav-2201 \
    '-DCMAKE_ASC_FLAGS=-g --sanitizer' \
    2>&1 | tee "${RESULT_DIR}/sanitizer-cmake.log"
cmake --build "${SANITIZER_BUILD_DIR}" -j8 \
    2>&1 | tee "${RESULT_DIR}/sanitizer-build.log"

run_sanitizer racecheck

if rg -q "without .*cce-enable-sanitizer" "${RESULT_DIR}/racecheck.log" \
    "${RESULT_DIR}/racecheck.stdout.log" 2>/dev/null; then
    die "racecheck ignored the kernel because sanitizer instrumentation is missing"
fi

race_output_count=$(rg -c 'OUTPUT = 24' "${RESULT_DIR}/racecheck.stdout.log" || true)
[[ "${race_output_count:-0}" -eq 6 ]] || \
    die "Sanitizer functional check failed: expected 6 lines containing 'OUTPUT = 24'"

waw_count=$(count_hazard WAW "${RESULT_DIR}/racecheck.log")
raw_count=$(count_hazard RAW "${RESULT_DIR}/racecheck.log")
war_count=$(count_hazard WAR "${RESULT_DIR}/racecheck.log")

{
    echo "Issue1761 sanitizer summary"
    echo "normal_output_24_count=${normal_output_count}"
    echo "racecheck_output_24_count=${race_output_count}"
    echo "WAW=${waw_count}"
    echo "RAW=${raw_count}"
    echo "WAR=${war_count}"
} | tee "${RESULT_DIR}/summary.txt"

[[ "${waw_count}" -eq 0 ]] || die "Issue1761 check failed: WAW=${waw_count}, expected 0"

if [[ "${CHECK_MODE}" == "all" ]]; then
    run_sanitizer memcheck
    run_sanitizer initcheck
    run_sanitizer synccheck
fi

echo "PASS: functional output is correct and WAW=0"
echo "Review RAW/WAR details before classifying them as synchronized accesses."
echo "Logs: ${RESULT_DIR}"
