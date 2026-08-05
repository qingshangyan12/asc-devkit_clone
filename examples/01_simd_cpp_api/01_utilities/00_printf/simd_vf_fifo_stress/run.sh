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
jobs=${JOBS:-8}
arch=${ASC_ARCH:-dav-3510}
iterations=${ITERATIONS:-3}
if [[ ! "${iterations}" =~ ^[1-9][0-9]*$ ]]; then
    echo "ITERATIONS must be a positive integer, got: ${iterations}" >&2
    exit 2
fi
rm -rf "${build_dir}" "${log_dir}"
mkdir -p "${build_dir}" "${log_dir}"

if ! cmake -S "${script_dir}" -B "${build_dir}" -DCMAKE_ASC_ARCHITECTURES="${arch}" \
    >"${log_dir}/configure.log" 2>&1; then
    echo "FIFO_STRESS FAIL: configure failed; see ${log_dir}/configure.log" >&2
    exit 1
fi
if ! cmake --build "${build_dir}" -j "${jobs}" >"${log_dir}/build.log" 2>&1; then
    echo "FIFO_STRESS FAIL: build failed; see ${log_dir}/build.log" >&2
    exit 1
fi

summary=${log_dir}/summary.log
: >"${summary}"
failures=0
total_bytes=0
for ((iteration = 1; iteration <= iterations; ++iteration)); do
    run_log=${log_dir}/run_${iteration}.log
    verify_log=${log_dir}/verify_${iteration}.log
    set +e
    "${build_dir}/demo" "${script_dir}/acl.json" >"${run_log}" 2>&1
    run_status=$?
    python3 "${script_dir}/verify_log.py" "${run_log}" >"${verify_log}" 2>&1
    verify_status=$?
    set -e
    log_bytes=$(wc -c <"${run_log}")
    total_bytes=$((total_bytes + log_bytes))
    if (( run_status == 0 && verify_status == 0 )); then
        printf 'iteration=%u PASS bytes=%u log=%s\n' "${iteration}" "${log_bytes}" "${run_log}" >>"${summary}"
    else
        printf 'iteration=%u FAIL run_status=%u verify_status=%u bytes=%u log=%s verify=%s\n' \
            "${iteration}" "${run_status}" "${verify_status}" "${log_bytes}" "${run_log}" "${verify_log}" \
            >>"${summary}"
        failures=$((failures + 1))
    fi
done

cat "${summary}"
echo "FIFO_STRESS logs: ${log_dir}"
if (( failures != 0 )); then
    echo "FIFO_STRESS FAIL: failures=${failures}/${iterations}" >&2
    exit 1
fi

echo "FIFO_STRESS PASS iterations=${iterations} total_bytes=${total_bytes}"
