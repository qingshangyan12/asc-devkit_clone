#!/usr/bin/env bash
set -euo pipefail

case_name="${1:-urma}"
rank_size="${RANK_SIZE:-2}"
npu_arch="${CMAKE_ASC_ARCHITECTURES:-dav-3510}"

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
build_dir="${script_dir}/build"

if [[ "${case_name}" != "urma" && "${case_name}" != "urma_timeout" && "${case_name}" != "cqe" && "${case_name}" != "cqe_error" ]]; then
  echo "usage: bash ${0} [urma|cqe]"
  exit 1
fi

if [[ -z "${ASCEND_HOME_PATH:-}" ]]; then
  echo "ASCEND_HOME_PATH is not set. Source the CANN set_env.sh first."
  exit 1
fi

mkdir -p "${build_dir}"
cmake_args=(
  -S "${script_dir}"
  -B "${build_dir}"
  -DCMAKE_ASC_ARCHITECTURES="${npu_arch}"
)
if [[ -n "${CMAKE_ASC_COMPILER:-}" ]]; then
  cmake_args+=("-DCMAKE_ASC_COMPILER=${CMAKE_ASC_COMPILER}")
fi
if [[ -n "${ASC_DIR:-}" ]]; then
  cmake_args+=("-DASC_DIR=${ASC_DIR}")
fi
if [[ -n "${CMAKE_PREFIX_PATH:-}" ]]; then
  cmake_args+=("-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
fi
cmake "${cmake_args[@]}"
cmake --build "${build_dir}" -j

export LD_LIBRARY_PATH="${build_dir}:${ASCEND_HOME_PATH}/lib64:/usr/local/Ascend/driver/lib64/driver:${ASCEND_HOME_PATH}/devlib:${LD_LIBRARY_PATH:-}"

if [[ -z "${HCOMM_CHANNEL_DUMP_SO:-}" ]]; then
  export HCOMM_CHANNEL_DUMP_SO="libhcomm_channel_dump.so"
fi

"${build_dir}/hcomm_exception_fault_runner" --case "${case_name}" --rank-size "${rank_size}"
