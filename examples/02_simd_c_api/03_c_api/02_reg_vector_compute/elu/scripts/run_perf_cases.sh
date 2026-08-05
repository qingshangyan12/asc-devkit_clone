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
cases_root=${2:-"${example_dir}/cases/performance"}
results_root=${3:-"${example_dir}/performance-results"}
repeat_count=${ELU_PERF_REPEATS:-1}
manifest_path=${cases_root}/manifest.json

if [[ ! -x "${demo_path}" ]]; then
    echo "[ERROR] demo is not executable: ${demo_path}" >&2
    exit 2
fi
if [[ ! -f "${manifest_path}" ]]; then
    echo "[ERROR] performance manifest not found: ${manifest_path}" >&2
    echo "[ERROR] generate it with: python3 ${script_dir}/gen_cases.py --output-root <cases>" >&2
    exit 2
fi
if [[ ! ${repeat_count} =~ ^[1-9][0-9]*$ ]]; then
    echo "[ERROR] ELU_PERF_REPEATS must be a positive integer" >&2
    exit 2
fi
if ! command -v msprof >/dev/null 2>&1; then
    echo "[ERROR] msprof was not found in PATH" >&2
    exit 2
fi
if [[ -e "${results_root}" ]]; then
    echo "[ERROR] results root already exists: ${results_root}" >&2
    exit 2
fi

demo_path=$(realpath "${demo_path}")
cases_root=$(realpath "${cases_root}")
results_root=$(realpath -m "${results_root}")
manifest_path=${cases_root}/manifest.json
mkdir -p "${results_root}/correctness" "${results_root}/profiles"
exec > >(tee "${results_root}/runner.log") 2>&1

date --iso-8601=seconds > "${results_root}/started_at.txt"
printf 'demo=%s\ncases=%s\nrepeats=%s\n' \
    "${demo_path}" "${cases_root}" "${repeat_count}" > "${results_root}/invocation.txt"
if command -v npu-smi >/dev/null 2>&1; then
    npu-smi info > "${results_root}/npu_before.txt"
fi

failures=0
while IFS=$'\t' read -r name dtype length alpha scale input_scale; do
    case_root=${cases_root}/${name}
    correctness_log=${results_root}/correctness/${name}.log
    echo "[CORRECTNESS] ${name}"
    if ! "${demo_path}" \
        "${dtype}" "${length}" "${alpha}" "${scale}" "${input_scale}" \
        "${case_root}/input/input_x.bin" "${case_root}/output/golden.bin" \
        > "${correctness_log}" 2>&1; then
        echo "[ERROR] correctness failed: ${name}"
        failures=$((failures + 1))
        continue
    fi
    if ! grep -q 'status=PASS' "${correctness_log}"; then
        echo "[ERROR] correctness result missing PASS: ${name}"
        failures=$((failures + 1))
        continue
    fi

    application="${demo_path} ${dtype} ${length} ${alpha} ${scale} ${input_scale} ${case_root}/input/input_x.bin ${case_root}/output/golden.bin"
    mkdir -p "${results_root}/profiles/${name}"
    for repeat in $(seq 1 "${repeat_count}"); do
        repeat_label=$(printf 'repeat_%03d' "${repeat}")
        profile_root=${results_root}/profiles/${name}/${repeat_label}
        profile_log=${profile_root}.log
        echo "[PROFILE] ${name} ${repeat_label}"
        if ! msprof \
            --output="${profile_root}" \
            --aic-metrics=PipeUtilization \
            --task-block=off \
            --application="${application}" \
            > "${profile_log}" 2>&1; then
            echo "[ERROR] profiling failed: ${name} ${repeat_label}"
            failures=$((failures + 1))
            continue
        fi
        if ! grep -q 'status=PASS' "${profile_log}"; then
            echo "[ERROR] profiled application did not report PASS: ${name} ${repeat_label}"
            failures=$((failures + 1))
        fi
    done
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

if command -v npu-smi >/dev/null 2>&1; then
    npu-smi info > "${results_root}/npu_after.txt"
fi
date --iso-8601=seconds > "${results_root}/finished_at.txt"

if ! python3 "${script_dir}/summarize_perf.py" \
    --manifest "${manifest_path}" \
    --results-root "${results_root}" \
    --repeats "${repeat_count}"; then
    failures=$((failures + 1))
fi

echo "PERFORMANCE_RUNNER failures=${failures}"
[[ ${failures} -eq 0 ]]
