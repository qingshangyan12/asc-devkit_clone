#!/usr/bin/env bash
# Copyright (c) 2026 ddplys.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

# Source the matching CANN environment before running this script.
set -euo pipefail
repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../../.." && pwd)
log_dir=${1:-"${repo_root}/tests/api/basic_api/vector_pointer/target_logs"}
mkdir -p -- "${log_dir}"
log_dir=$(cd -- "${log_dir}" && pwd)
compiler=${ASC_COMPILER:-bisheng}
"${compiler}" --version > "${log_dir}/compiler.txt" 2>&1
flags=(-x asc --npu-arch=dav-3510 -std=c++17 -O2 -c -H)
for dir in "${repo_root}" "${repo_root}/include" "${repo_root}/include/basic_api" \
           "${repo_root}/include/utils" "${repo_root}/impl" "${repo_root}/impl/basic_api"; do
    flags+=(-iquote "${dir}")
done
flags+=(-I "${repo_root}")
: > "${log_dir}/command.txt"
for fixture in target_arithmetic target_conversion; do
    source_file="${repo_root}/tests/api/basic_api/vector_pointer/${fixture}.cpp"
    printf '%q ' "${compiler}" "${flags[@]}" "${source_file}" -o "${log_dir}/${fixture}.o" >> "${log_dir}/command.txt"
    printf '\n' >> "${log_dir}/command.txt"
    if ! "${compiler}" "${flags[@]}" "${source_file}" -o "${log_dir}/${fixture}.o" > "${log_dir}/${fixture}.log" 2>&1; then
        grep -n -m 20 'error:' "${log_dir}/${fixture}.log" || true
        exit 1
    fi
done
python3 - "${repo_root}" "${log_dir}" <<'PY'
import json
import re
import sys
from pathlib import Path
root, logdir = map(Path, sys.argv[1:])
root = root.resolve()
entries = json.loads((root / 'tests/api/basic_api/vector_pointer/scope.json').read_text())['entries']
expected = {e['header'] for e in entries}
seen = set()
for log in logdir.glob('target_*.log'):
    for raw in re.findall(r'^\.+\s+([^\n]+\.h)\s*$', log.read_text(errors='replace'), re.M):
        path = Path(raw.strip()).resolve()
        if path.name in expected or path.name.replace('_intf_impl.h', '_intf.h') in expected:
            if root not in path.parents:
                raise SystemExit(f'Wrong API header selected: {path}')
            seen.add(path.name)
missing = expected - seen
if missing:
    raise SystemExit(f'Missing public header provenance: {sorted(missing)}')
(logdir / 'result.txt').write_text('PASS: both fixtures generated dav-3510 objects; all 20 scope headers from edited tree.\n')
print((logdir / 'result.txt').read_text(), end='')
PY
