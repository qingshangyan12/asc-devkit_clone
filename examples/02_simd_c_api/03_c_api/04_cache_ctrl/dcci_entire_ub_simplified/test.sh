#!/usr/bin/env bash

set -euo pipefail

sample_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)
repo_dir=$(cd "${sample_dir}/../../../../.." && pwd -P)
cann_path=${ASCEND_CANN_PACKAGE_PATH:-/usr/local/Ascend/cann-9.2.0-beta.1}
cann_asc="${cann_path}/x86_64-linux/asc"
expected_include="${repo_dir}/include/c_api"
expected_impl="${repo_dir}/impl/c_api"
build_dir="${sample_dir}/build"
run_log="${build_dir}/run.log"

check_link()
{
    local link_path=$1
    local expected_target=$2
    if [[ ! -L "${link_path}" ]]; then
        echo "error: ${link_path} is not a symbolic link" >&2
        return 1
    fi
    if [[ "$(readlink -f "${link_path}")" != "${expected_target}" ]]; then
        echo "error: ${link_path} does not point to ${expected_target}" >&2
        return 1
    fi
}

check_link "${cann_asc}/include/c_api" "${expected_include}"
check_link "${cann_asc}/impl/c_api" "${expected_impl}"

if [[ ! -f "${cann_path}/set_env.sh" ]]; then
    echo "error: ${cann_path}/set_env.sh does not exist" >&2
    exit 1
fi

set +u
source "${cann_path}/set_env.sh"
set -u

cmake -E remove_directory "${build_dir}"
cmake -S "${sample_dir}" -B "${build_dir}" \
    -DASCEND_CANN_PACKAGE_PATH="${cann_path}" \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    -DCMAKE_ASC_RUN_MODE=npu
cmake --build "${build_dir}" -j16

pushd "${build_dir}" >/dev/null
./demo | tee "${run_log}"
popd >/dev/null

if ! grep -qx "test pass!" "${run_log}"; then
    echo "error: expected an exact 'test pass!' line" >&2
    exit 1
fi
