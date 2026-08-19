#!/usr/bin/env bash

set -euo pipefail

example_root=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <path relative to ${example_root}, without .asc>" >&2
    exit 2
fi

case_rel=${1%.asc}
if [[ ${case_rel} == /* || ${case_rel} == *..* || ! ${case_rel} =~ ^[A-Za-z0-9_/-]+$ ]]; then
    echo "Invalid example path: ${case_rel}" >&2
    exit 2
fi
case_source="${example_root}/${case_rel}.asc"
if [[ ! -f ${case_source} ]]; then
    echo "Example source does not exist: ${case_source}" >&2
    exit 2
fi

case_arch=$(sed -n 's#^// Architecture: ##p' "${case_source}" | head -n 1)
if [[ -z ${case_arch} ]]; then
    echo "Missing '// Architecture:' metadata in ${case_source}" >&2
    exit 2
fi

case_name=$(printf '%s' "${case_rel}" | tr '/-' '__')
build_root=${TMPDIR:-/tmp}/asc-devkit-c-api-call-examples
build_dir="${build_root}/${case_rel}"
rm -rf "${build_dir}"

cmake -S "${example_root}" -B "${build_dir}" \
    -DCASE_SOURCE="${case_source}" \
    -DCASE_NAME="${case_name}" \
    -DCMAKE_ASC_ARCHITECTURES="${case_arch}"
cmake --build "${build_dir}" --parallel
"${build_dir}/${case_name}"
