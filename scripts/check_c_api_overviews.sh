#!/usr/bin/env bash

set -euo pipefail

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
repo_root=$(cd "${script_dir}/.." && pwd)
c_api_dir="${repo_root}/docs/zh/api/SIMD-API/C-API"

mapfile -d '' overview_files < <(find "${c_api_dir}" -type f -name '*.md' \( \
    -name '*概述*.md' -o \
    -name '*概览*.md' -o \
    -path '*/数据搬运导览/*' -o \
    -path '*/矩阵计算的搬入/*' -o \
    -path '*/矩阵计算的搬出/*' -o \
    -path '*/sync/核*同步/*' -o \
    -path '*/simd_atomic/关键特性说明.md' -o \
    -path '*/reg/关键特性说明/关键特性说明.md' -o \
    -path '*/cube_datamove/矩阵数据搬运.md' \
\) -print0)

failed=0

check_pattern() {
    local description=$1
    local pattern=$2
    local matches

    if matches=$(rg -n "${pattern}" "${overview_files[@]}"); then
        echo "[C API overview check] ${description}:"
        echo "${matches}"
        failed=1
    fi
}

check_pattern "found Basic API-only parameter or type" \
    'repeatParams|SliceInfo|NdDmaParams|NdDmaLoopInfo|NdDmaConfig|loadDataParams|loadMxDataParams|intriParams|cbufWorkspace|CopyRepeatParams|MmadWithSparse|dataCopyParams|MmadMx'
check_pattern "found an architecture document suffix displayed as an API name" \
    '\[asc_[^]]*_arch_(2201|3510)'
check_pattern "found a template-style call for a C API" \
    'asc_[a-z0-9_]+<'
check_pattern "found multiple products in one npu tag" \
    '<!-- npu="[^"]*,[^"]*"'

duplicate_anchors=$(perl -ne '
    if ($. == 1) { %seen = (); }
    if (/<a (?:name|id)="([^"]+)"/) {
        print "$ARGV:$.:$1\n" if $seen{$1}++;
    }
    close ARGV if eof;
' "${overview_files[@]}")

if [[ -n "${duplicate_anchors}" ]]; then
    echo "[C API overview check] found duplicate anchors:"
    echo "${duplicate_anchors}"
    failed=1
fi

if [[ ${failed} -ne 0 ]]; then
    exit 1
fi

echo "[C API overview check] PASS"
