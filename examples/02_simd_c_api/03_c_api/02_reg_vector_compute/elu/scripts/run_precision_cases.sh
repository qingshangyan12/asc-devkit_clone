#!/usr/bin/env bash

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -euo pipefail

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
example_dir=$(cd -- "${script_dir}/.." && pwd)
demo_path=${1:-"${example_dir}/build/demo"}
cases_root=${2:-"${example_dir}/cases/precision"}
results_root=${3:-"${example_dir}/precision-results"}
manifest_path=${cases_root}/manifest.json

if [[ ! -x "${demo_path}" ]]; then
    echo "[ERROR] demo is not executable: ${demo_path}" >&2
    exit 2
fi
if [[ ! -f "${manifest_path}" ]]; then
    echo "[ERROR] precision manifest not found: ${manifest_path}" >&2
    echo "[ERROR] generate it with: python3 ${script_dir}/gen_cases.py --output-root <cases>" >&2
    exit 2
fi
if [[ -e "${results_root}" ]]; then
    echo "[ERROR] results root already exists: ${results_root}" >&2
    exit 2
fi

mkdir -p "${results_root}/cases"
summary_csv=${results_root}/precision_summary.csv
runner_log=${results_root}/runner.log
printf 'case,dtype,length,matched_ratio,max_abs_error,status,log\n' > "${summary_csv}"
exec > >(tee "${runner_log}") 2>&1

passed=0
failed=0
total=0
while IFS=$'\t' read -r name dtype length alpha scale input_scale; do
    total=$((total + 1))
    case_root=${cases_root}/${name}
    case_log=${results_root}/cases/${name}.log
    echo "[CASE] ${name}"

    exit_code=0
    "${demo_path}" \
        "${dtype}" "${length}" "${alpha}" "${scale}" "${input_scale}" \
        "${case_root}/input/input_x.bin" "${case_root}/output/golden.bin" \
        > "${case_log}" 2>&1 || exit_code=$?
    result_line=$(grep '^RESULT ' "${case_log}" | tail -n 1 || true)
    matched_ratio=$(sed -n 's/.*matched_ratio=\([^ ]*\).*/\1/p' <<< "${result_line}")
    max_abs_error=$(sed -n 's/.*max_abs_error=\([^ ]*\).*/\1/p' <<< "${result_line}")
    status=$(sed -n 's/.*status=\([^ ]*\).*/\1/p' <<< "${result_line}")

    if [[ ${exit_code} -eq 0 && "${status}" == "PASS" ]]; then
        passed=$((passed + 1))
    else
        status=FAIL
        failed=$((failed + 1))
    fi
    printf '%s,%s,%s,%s,%s,%s,cases/%s.log\n' \
        "${name}" "${dtype}" "${length}" "${matched_ratio}" "${max_abs_error}" "${status}" "${name}" \
        >> "${summary_csv}"
    echo "${result_line:-RESULT status=FAIL reason=missing_result_line}"
done < <(
    python3 - "${manifest_path}" <<'PY'
import json
import sys

for case in json.load(open(sys.argv[1], encoding="utf-8")):
    print(
        case["name"],
        case["dtype"],
        case["length"],
        case["alpha"],
        case["scale"],
        case["input_scale"],
        sep="\t",
    )
PY
)

echo "PRECISION_SUMMARY passed=${passed} failed=${failed} total=${total}"
[[ ${total} -eq 54 && ${passed} -eq 54 && ${failed} -eq 0 ]]
