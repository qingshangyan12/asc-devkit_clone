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

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${TEST_DIR}/../../../.." && pwd)"
SKILL_DIR="${SKILL_DIR:-${REPO_ROOT}/.agent/skills/asc-api-ut-gen}"
CLI="${SKILL_DIR}/scripts/ut_generator_cli.py"
FIXTURE_DIR="${TEST_DIR}/fixtures"
GOLDEN_DIR="${TEST_DIR}/golden"
ARCH_FACTS="${REPO_ROOT}/.agent/skills/asc-npu-arch/references/npu-arch-facts.json"
GENERATION_CONSTRAINTS="${SKILL_DIR}/references/foundations/generation-constraints.json"
WORK_DIR="$(mktemp -d)"
seen_api_types=""
seen_chips=""

cleanup() {
    rm -rf "$WORK_DIR"
}
trap cleanup EXIT

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

json_field() {
    local file="$1"
    local field="$2"

    grep -m1 "\"${field}\"" "$file" | sed -E "s/.*\"${field}\"[[:space:]]*:[[:space:]]*\"([^\"]+)\".*/\\1/"
}

mark_seen() {
    local current="$1"
    local value="$2"

    if [[ " ${current} " == *" ${value} "* ]]; then
        printf '%s' "$current"
    else
        printf '%s %s' "$current" "$value"
    fi
}

assert_covered() {
    local label="$1"
    local seen="$2"
    shift 2

    for expected in "$@"; do
        [[ " ${seen} " == *" ${expected} "* ]] || fail "missing ${label} fixture coverage: ${expected}"
    done
}

read_json_object_keys() {
    local -n target="$1"
    local file="$2"
    local key="$3"
    local label="$4"
    local output

    if ! output="$(python3 - "$file" "$key" "$label" <<'PY'
import json
import sys

path, key, label = sys.argv[1:4]
try:
    with open(path, encoding="utf-8") as f:
        data = json.load(f)
except Exception as exc:
    raise SystemExit(f"{label}: failed to read {path}: {exc}")

if key not in data:
    raise SystemExit(f"{label}: missing top-level object {key!r} in {path}")

value = data[key]
if not isinstance(value, dict):
    raise SystemExit(
        f"{label}: expected top-level {key!r} to be an object in {path}, "
        f"got {type(value).__name__}"
    )
if not value:
    raise SystemExit(f"{label}: top-level object {key!r} is empty in {path}")

print("\n".join(value.keys()))
PY
    )"; then
        fail "failed to read ${label} from ${file}:${key}"
    fi

    mapfile -t target <<< "$output"
}

join_words() {
    local IFS=" "
    printf '%s' "$*"
}

self_test_reference_key_reader() {
    local generation_ref="${WORK_DIR}/self-generation-constraints.json"
    local arch_ref="${WORK_DIR}/self-npu-arch-facts.json"
    local -a self_api_types
    local -a self_chips

    cat > "$generation_ref" <<'JSON'
{"api_types":{"aiv":{},"c_api":{}},"chips":"wrong-source"}
JSON
    cat > "$arch_ref" <<'JSON'
{"chips":{"ascend910":{},"mc62cm12aa":{}},"api_types":"wrong-source"}
JSON

    read_json_object_keys self_api_types "$generation_ref" "api_types" "self-test API types"
    read_json_object_keys self_chips "$arch_ref" "chips" "self-test chips"

    [[ "$(join_words "${self_api_types[@]}")" == "aiv c_api" ]] || \
        fail "self-test must read API types from generation-constraints api_types"
    [[ "$(join_words "${self_chips[@]}")" == "ascend910 mc62cm12aa" ]] || \
        fail "self-test must read chips from npu-arch-facts chips"
}

self_test_reference_key_reader
read_json_object_keys expected_api_types "$GENERATION_CONSTRAINTS" "api_types" "generation constraints API types"
read_json_object_keys expected_chips "$ARCH_FACTS" "chips" "npu arch chip facts"

self_test_generator_helpers() {
    python3 - "$SKILL_DIR" "$GENERATION_CONSTRAINTS" <<'PY'
import json
import sys

skill_dir, constraints_path = sys.argv[1:3]
sys.path.insert(0, f"{skill_dir}/scripts")

from ut_generator import (  # noqa: E402
    API_TYPE_CHOICES,
    ApiType,
    ChipArch,
    TEMPLATES,
    UTConfig,
    UTGenerator,
)
from ut_generator_cli import default_kernel_params  # noqa: E402

with open(constraints_path, encoding="utf-8") as f:
    expected_api_types = list(json.load(f)["api_types"].keys())

assert API_TYPE_CHOICES == expected_api_types
assert default_kernel_params(ApiType.AIV.value) == {}
assert default_kernel_params(ApiType.AIC.value) == {"m": 16, "k": 64, "n": 16}

config = UTConfig(api_type=ApiType.AIV, api_name="Add", chip=ChipArch.ASCEND910B1)
generator = UTGenerator(config)
TEMPLATES["_self_template"] = "${A} ${B}"
try:
    rendered = generator.render("_self_template", {"A": "${B}", "B": "ok"})
finally:
    del TEMPLATES["_self_template"]

assert rendered == "${B} ok"
PY
}

self_test_generator_helpers

run_case() {
    local name="$1"
    local fixture="${FIXTURE_DIR}/${name}.json"
    local golden="${GOLDEN_DIR}/${name}.cpp"
    local actual="${WORK_DIR}/${name}.cpp"
    local api_type
    local chip

    [[ -f "$fixture" ]] || fail "missing fixture: $fixture"
    [[ -f "$golden" ]] || fail "missing golden: $golden"

    api_type="$(json_field "$fixture" "api_type")"
    chip="$(json_field "$fixture" "chip")"
    seen_api_types="$(mark_seen "$seen_api_types" "$api_type")"
    seen_chips="$(mark_seen "$seen_chips" "$chip")"

    python3 "$CLI" --config "$fixture" --output "$actual" >/dev/null
    if grep -Eq 'template <[^>]*,[[:space:]]*Src1T([,>])' "$actual"; then
        fail "generated UT contains naked Src1T template parameter: $name"
    fi
    if [[ "$api_type" == "aiv" ]] && grep -Eq 'uint8_t (dstGm|src0Gm|src1Gm)\[[^]]*param\.data_size' "$actual"; then
        fail "generated AIV UT must not allocate runtime-sized GM arrays on stack: $name"
    fi
    if [[ "$api_type" == "aiv" ]] && ! grep -Fq "param.init_func(src0Gm.data(), src1Gm.data(), param.data_size);" "$actual"; then
        fail "generated AIV UT must initialize inputs through dtype-aware init function: $name"
    fi
    if [[ "$api_type" == "reg" ]] && ! grep -Fq "dstGlobal.SetValue(i, src0Global.GetValue(i));" "$actual"; then
        fail "generated reg UT must write initialized data back to dst GM: $name"
    fi
    diff -u "$golden" "$actual" || fail "generated UT differs from golden: $name"
}

supported_output="${WORK_DIR}/list-supported.txt"
python3 "$CLI" --list-supported > "$supported_output"
grep -Fq "npu-arch-facts.json" "$supported_output" || fail "CLI must report npu-arch structured reference"
grep -Fq "generation-constraints.json" "$supported_output" || fail "CLI must report generation constraints reference"
grep -Fq "fp8_e8m0_t" "$supported_output" || fail "CLI must list documented builtin dtypes from npu-arch facts"
grep -Fq "api-specific-init" "$supported_output" || fail "CLI must distinguish dtype initialization constraints"
grep -Fq "通用 UT 生成可直接初始化的数据类型" "$supported_output" || fail "CLI must list generic generator dtype subset"

invalid_dtype_output="${WORK_DIR}/invalid-dtype.txt"
if python3 "$CLI" --type aiv --api Add --chip ascend910b1 --dtype fp8_e5m2_t --output - >"$invalid_dtype_output" 2>&1; then
    fail "generic generation must reject dtypes that require API-specific initialization"
fi
grep -Fq "invalid choice" "$invalid_dtype_output" || \
    fail "unsupported dtype rejection must come from structured generator dtype constraints"

invalid_output="${WORK_DIR}/invalid-simt.txt"
if python3 "$CLI" --type simt --api vector_add --chip ascend910b1 --output - >"$invalid_output" 2>&1; then
    fail "SIMT generation must reject chips disallowed by generation-constraints.json"
fi
grep -Fq "SIMT API only supports ascend950pr_9599" "$invalid_output" || \
    fail "SIMT rejection must use generation-constraints.json message"

coverage_report="${WORK_DIR}/coverage.html"
cat > "$coverage_report" <<'EOF'
<html><body>
<div>Lines: 1.0%</div>
<table>
<tr><td class="headerItem">Lines:</td><td class="headerCovTableEntryHi">93.6 %</td></tr>
<tr><td class="headerItem">Functions:</td><td class="headerCovTableEntryHi">91.2 %</td></tr>
</table>
</body></html>
EOF

report_json="${WORK_DIR}/generation-report.json"
actual_report_case="${WORK_DIR}/report_case.cpp"
ASC_API_UT_PROMPT_TOKENS=11 \
ASC_API_UT_COMPLETION_TOKENS=22 \
python3 "$CLI" \
    --config "${FIXTURE_DIR}/add_aiv_ascend910b1.json" \
    --output "$actual_report_case" \
    --coverage-report "$coverage_report" \
    --report-json "$report_json" >/dev/null

python3 - "$report_json" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    report = json.load(f)

assert report["token_usage"]["prompt_tokens"] == 11
assert report["token_usage"]["completion_tokens"] == 22
assert report["token_usage"]["total_tokens"] == 33
assert report["elapsed_seconds"] >= 0
assert report["current_coverage"]["status"] == "available"
assert report["current_coverage"]["lines"] == 93.6
assert report["current_coverage"]["functions"] == 91.2
PY

invalid_token_report_json="${WORK_DIR}/invalid-token-report.json"
invalid_token_case="${WORK_DIR}/invalid_token_case.cpp"
ASC_API_UT_PROMPT_TOKENS=abc \
ASC_API_UT_COMPLETION_TOKENS=" 7 " \
ASC_API_UT_TOTAL_TOKENS=bad \
python3 "$CLI" \
    --config "${FIXTURE_DIR}/add_aiv_ascend910b1.json" \
    --output "$invalid_token_case" \
    --report-json "$invalid_token_report_json" >/dev/null

python3 - "$invalid_token_report_json" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    report = json.load(f)

token_usage = report["token_usage"]
assert token_usage["source"] == "env"
assert token_usage["prompt_tokens"] is None
assert token_usage["completion_tokens"] == 7
assert token_usage["total_tokens"] is None
assert "ASC_API_UT_PROMPT_TOKENS" in token_usage["reason"]
assert "ASC_API_UT_TOTAL_TOKENS" in token_usage["reason"]
PY

fixture_count=0
for fixture in "${FIXTURE_DIR}"/*.json; do
    [[ -f "$fixture" ]] || fail "no fixture found under ${FIXTURE_DIR}"
    run_case "$(basename "$fixture" .json)"
    fixture_count=$((fixture_count + 1))
done

assert_covered "API type" "$seen_api_types" "${expected_api_types[@]}"
assert_covered "chip" "$seen_chips" "${expected_chips[@]}"

echo "[PASS] asc-api-ut-gen generation golden validation passed (${fixture_count} fixtures)"
