#!/usr/bin/env bash

set -uo pipefail

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
cd "${script_dir}"

cann_root=${CANN_PATH:-${ASCEND_HOME_PATH:-${ASCEND_TOOLKIT_HOME:-}}}
if [[ -n "${cann_root}" && ! -f "${cann_root}/set_env.sh" && -f "$(dirname "${cann_root}")/set_env.sh" ]]; then
    cann_root=$(dirname "${cann_root}")
fi
if [[ -z "${cann_root}" && -f /usr/local/Ascend/cann/set_env.sh ]]; then
    cann_root=/usr/local/Ascend/cann
fi
if [[ -z "${cann_root}" || ! -f "${cann_root}/set_env.sh" ]]; then
    echo "CANN environment not found. Set CANN_PATH, ASCEND_HOME_PATH, or ASCEND_TOOLKIT_HOME." >&2
    exit 2
fi

source "${cann_root}/set_env.sh" >/dev/null 2>&1

build_dir=${script_dir}/build
log_dir=${script_dir}/logs
summary=${log_dir}/summary.log
jobs=${JOBS:-8}
arch=${ASC_ARCH:-dav-3510}
case_count=19
if (( $# > 1 )); then
    echo "Usage: bash run.sh [case_id]" >&2
    exit 2
fi
if (( $# == 1 )); then
    if [[ ! "$1" =~ ^(0|[1-9][0-9]*)$ ]] || (( $1 >= case_count )); then
        echo "case_id must be an integer from 0 to $((case_count - 1)), got: $1" >&2
        exit 2
    fi
    case_ids=("$1")
else
    case_ids=()
    for ((case_id = 0; case_id < case_count; ++case_id)); do
        case_ids+=("${case_id}")
    done
fi
rm -rf "${build_dir}" "${log_dir}"
mkdir -p "${build_dir}" "${log_dir}"

if ! cmake -S "${script_dir}" -B "${build_dir}" -DCMAKE_ASC_ARCHITECTURES="${arch}" \
    >"${log_dir}/configure.log" 2>&1; then
    echo "ASSERT_CASES FAIL: configure failed; see ${log_dir}/configure.log" >&2
    exit 1
fi

: >"${summary}"
failures=0
for case_id in "${case_ids[@]}"; do
    target=assert_case_${case_id}
    build_log=${log_dir}/case_${case_id}_build.log
    run_log=${log_dir}/case_${case_id}_run.log
    verify_log=${log_dir}/case_${case_id}_verify.log

    if ! cmake --build "${build_dir}" --target "${target}" -j "${jobs}" >"${build_log}" 2>&1; then
        diagnostic=$(sed -n '/stack smashing detected\|fatal error:\|error:/p' "${build_log}" | head -n 1)
        printf 'case=%u BUILD_FAIL diagnostic=%s log=%s\n' \
            "${case_id}" "${diagnostic:--}" "${build_log}" >>"${summary}"
        failures=$((failures + 1))
        continue
    fi

    "${build_dir}/bin/${target}" >"${run_log}" 2>&1
    run_status=$?
    python3 "${script_dir}/verify_log.py" "${case_id}" "${run_log}" >"${verify_log}" 2>&1
    verify_status=$?
    if (( run_status == 0 && verify_status == 0 )); then
        printf 'case=%u PASS log=%s\n' "${case_id}" "${run_log}" >>"${summary}"
    else
        printf 'case=%u FAIL run_status=%u verify_status=%u log=%s verify=%s\n' \
            "${case_id}" "${run_status}" "${verify_status}" "${run_log}" "${verify_log}" >>"${summary}"
        failures=$((failures + 1))
    fi
done

cat "${summary}"
echo "ASSERT_CASES logs: ${log_dir}"
if (( failures != 0 )); then
    echo "ASSERT_CASES FAIL failures=${failures}" >&2
    exit 1
fi
echo "ASSERT_CASES PASS cases=${#case_ids[@]} selected=$(IFS=,; echo "${case_ids[*]}")"
