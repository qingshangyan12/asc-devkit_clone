#!/usr/bin/env bash

set -euo pipefail

repo_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)
cann_path=${ASCEND_CANN_PACKAGE_PATH:-/usr/local/Ascend/cann-9.2.0-beta.1}
build_dir=${ASCENDC_BUILD_DIR:-${repo_dir}/build}
target_name=ascendc_ut_c_api_ascend950pr_9599_AIV
target_dir="${build_dir}/tests/api/c_api/CMakeFiles/${target_name}.dir"
main_object="${target_dir}/__/__/main.cpp.o"
dcci_object="${target_dir}/npu_arch_3510/cache_ctrl/test_dcci.cpp.o"
test_binary="${build_dir}/probes/asc_dcci_entire_ub_ut"

require_file()
{
    if [[ ! -f "$1" ]]; then
        echo "error: required file does not exist: $1" >&2
        exit 1
    fi
}

first_existing_file()
{
    local candidate
    for candidate in "$@"; do
        if [[ -f "${candidate}" ]]; then
            printf '%s\n' "${candidate}"
            return 0
        fi
    done
    return 1
}

require_file "${cann_path}/set_env.sh"
require_file "${build_dir}/tests/api/c_api/Makefile"
require_file "${target_dir}/build.make"

set +u
source "${cann_path}/set_env.sh"
set -u

cmake --build "${build_dir}/tests/api/c_api" --target __/__/main.cpp.o -j16
cmake -E rm -f "${dcci_object}" "${dcci_object}.d"
cmake --build "${build_dir}/tests/api/c_api" \
    --target npu_arch_3510/cache_ctrl/test_dcci.cpp.o -j16

require_file "${main_object}"
require_file "${dcci_object}"

gtest_library=$(first_existing_file \
    "${repo_dir}/third_party/lib_cache/gtest/lib/libgtest.a" \
    "${build_dir}/tests/third_party_gtest-prefix/src/third_party_gtest-build/lib/libgtest.a") || {
    echo "error: libgtest.a was not found under the configured repository build" >&2
    exit 1
}
mockcpp_library=$(first_existing_file \
    "${build_dir}/tests/mockcpp-prefix/src/mockcpp-build/libmockcpp.a" \
    "${build_dir}/_CPack_Packages/makeself_staging/mockcpp/lib/libmockcpp.a") || {
    echo "error: libmockcpp.a was not found under the configured repository build" >&2
    exit 1
}

cpudebug_lib="${cann_path}/tools/cpudebug/lib"
cpudebug_soc_lib="${cpudebug_lib}/Ascend950PR_9599"
simulator_lib="${cann_path}/x86_64-linux/simulator/dav_3510/lib"
cann_lib="${cann_path}/lib64"

require_file "${cpudebug_soc_lib}/libcpudebug.so"
require_file "${cpudebug_lib}/libcpudebug_cceprint.so"
require_file "${cpudebug_lib}/libcpudebug_npuchk.so"
require_file "${cpudebug_lib}/libcpudebug_stubreg.so"
require_file "${simulator_lib}/libpem_davinci.so"
require_file "${cann_lib}/liberror_manager.so"
require_file "${cann_lib}/libmmpa.so"
require_file "${cann_lib}/libc_sec.so"

cmake -E make_directory "$(dirname "${test_binary}")"
/usr/bin/c++ "${main_object}" "${dcci_object}" -o "${test_binary}" \
    -L"${cpudebug_lib}" \
    -L"${cpudebug_soc_lib}" \
    -L"${simulator_lib}" \
    -L"${cann_lib}" \
    -Wl,-rpath,"${cpudebug_lib}:${cpudebug_soc_lib}:${simulator_lib}:${cann_lib}" \
    -Wl,--no-as-needed \
    "${cann_lib}/liberror_manager.so" \
    "${cann_lib}/libmmpa.so" \
    "${gtest_library}" \
    "${mockcpp_library}" \
    -lpthread \
    -lpem_davinci \
    -lcpudebug \
    "${cpudebug_lib}/libcpudebug_cceprint.so" \
    "${cpudebug_lib}/libcpudebug_npuchk.so" \
    "${cpudebug_lib}/libcpudebug_stubreg.so" \
    "${cann_lib}/libc_sec.so"

"${test_binary}" --gtest_filter='TestAscCApiSimdDcci.*'
