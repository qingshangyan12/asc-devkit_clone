#!/usr/bin/env bash

set -euo pipefail

readonly REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SAMPLE_DIR="${REPO_ROOT}/examples/01_simd_cpp_api/06_compatibility_guide/fill"
readonly CANN_INSTALL_PATH="${CANN_PATH:-/home/developer/Ascend/cann-9.2.0}"
readonly CHECK_MODE="${1:-build-only}"
readonly RESULT_DIR="${2:-$(mktemp -d /tmp/issue1676-mssanitizer.XXXXXX)}"
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
        mssanitizer --tool="${tool_name}" --log-level=error --log-file="${log_file}" ./demo
    ) 2>&1 | tee "${stdout_file}"
}

[[ "${CHECK_MODE}" == "build-only" || "${CHECK_MODE}" == "racecheck" || "${CHECK_MODE}" == "all" ]] || \
    die "Usage: $0 [build-only|racecheck|all] [result_dir]"
[[ -f "${CANN_INSTALL_PATH}/set_env.sh" ]] || \
    die "CANN set_env.sh not found: ${CANN_INSTALL_PATH}/set_env.sh"
[[ -d "${SAMPLE_DIR}" ]] || die "Sample directory not found: ${SAMPLE_DIR}"

# shellcheck disable=SC1090
source "${CANN_INSTALL_PATH}/set_env.sh"

command -v cmake >/dev/null || die "cmake is not available"
command -v mssanitizer >/dev/null || die "mssanitizer is not available"
command -v rg >/dev/null || die "rg is not available"

mkdir -p "${RESULT_DIR}"

echo "Result directory: ${RESULT_DIR}"
echo "CANN path: ${CANN_INSTALL_PATH}"
echo "Target architecture: dav-3510"
mssanitizer --version > "${RESULT_DIR}/mssanitizer-version.log" 2>&1

echo "Building the normal dav-3510 binary..."
cmake \
    -S "${SAMPLE_DIR}" \
    -B "${NORMAL_BUILD_DIR}" \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    2>&1 | tee "${RESULT_DIR}/normal-cmake.log"
cmake --build "${NORMAL_BUILD_DIR}" -j8 \
    2>&1 | tee "${RESULT_DIR}/normal-build.log"

echo "Building the sanitizer-instrumented dav-3510 binary..."
cmake \
    -S "${SAMPLE_DIR}" \
    -B "${SANITIZER_BUILD_DIR}" \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    '-DCMAKE_ASC_FLAGS=-g --sanitizer' \
    '-DCMAKE_ASC_LINK_FLAGS=--cce-enable-sanitizer' \
    2>&1 | tee "${RESULT_DIR}/sanitizer-cmake.log"
cmake --build "${SANITIZER_BUILD_DIR}" -j8 \
    2>&1 | tee "${RESULT_DIR}/sanitizer-build.log"

if [[ "${CHECK_MODE}" == "build-only" ]]; then
    echo "PASS: normal and sanitizer dav-3510 cross-builds completed."
    echo "Dynamic verification was skipped; run racecheck on an Ascend950 device."
    echo "Logs: ${RESULT_DIR}"
    exit 0
fi

command -v npu-smi >/dev/null || die "npu-smi is not available"
npu_info=$(npu-smi info 2>&1)
printf '%s\n' "${npu_info}" > "${RESULT_DIR}/npu-smi.log"

if ! rg -q '950' "${RESULT_DIR}/npu-smi.log" && [[ "${ISSUE1676_SKIP_DEVICE_CHECK:-0}" != "1" ]]; then
    die "Ascend950 device not detected. Issue1676 cannot be dynamically verified on DAV 2201."
fi

readonly PHYSICAL_NPU_ID="${ISSUE1676_NPU_ID:-0}"
export ASCEND_VISIBLE_DEVICES="${PHYSICAL_NPU_ID}"
export ASCEND_RT_VISIBLE_DEVICES="${PHYSICAL_NPU_ID}"

for build_dir in "${NORMAL_BUILD_DIR}" "${SANITIZER_BUILD_DIR}"; do
    (
        cd "${build_dir}"
        python3 "${SAMPLE_DIR}/scripts/gen_data.py"
    )
done

echo "Running the normal binary and precision verification..."
(
    cd "${NORMAL_BUILD_DIR}"
    ./demo
    python3 "${SAMPLE_DIR}/scripts/verify_result.py" output/output.bin output/golden.bin
) 2>&1 | tee "${RESULT_DIR}/normal-run.log"

run_sanitizer racecheck
(
    cd "${SANITIZER_BUILD_DIR}"
    python3 "${SAMPLE_DIR}/scripts/verify_result.py" output/output.bin output/golden.bin
) 2>&1 | tee "${RESULT_DIR}/racecheck-verify.log"

if rg -q "without .*cce-enable-sanitizer" "${RESULT_DIR}/racecheck.log" \
    "${RESULT_DIR}/racecheck.stdout.log" 2>/dev/null; then
    die "racecheck ignored the kernel because sanitizer instrumentation is missing"
fi

waw_count=$(count_hazard WAW "${RESULT_DIR}/racecheck.log")
raw_count=$(count_hazard RAW "${RESULT_DIR}/racecheck.log")
war_count=$(count_hazard WAR "${RESULT_DIR}/racecheck.log")

{
    echo "Issue1676 mssanitizer summary"
    echo "target_arch=dav-3510"
    echo "WAW=${waw_count}"
    echo "RAW=${raw_count}"
    echo "WAR=${war_count}"
} | tee "${RESULT_DIR}/summary.txt"

[[ "${waw_count}" -eq 0 ]] || die "Issue1676 check failed: WAW=${waw_count}, expected 0"
[[ "${raw_count}" -eq 0 ]] || die "Issue1676 check failed: RAW=${raw_count}, expected 0"
[[ "${war_count}" -eq 0 ]] || die "Issue1676 check failed: WAR=${war_count}, expected 0"

if [[ "${CHECK_MODE}" == "all" ]]; then
    run_sanitizer memcheck
    run_sanitizer initcheck
    run_sanitizer synccheck
fi

echo "PASS: precision verification succeeded and all L1 hazard counts are zero."
echo "Logs: ${RESULT_DIR}"
