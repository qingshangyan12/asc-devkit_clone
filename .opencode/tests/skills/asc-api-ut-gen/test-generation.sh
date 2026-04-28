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
SKILL_DIR="${SKILL_DIR:-${REPO_ROOT}/.opencode/skills/asc-api-ut-gen}"
CLI="${SKILL_DIR}/scripts/ut_generator_cli.py"
FIXTURE_DIR="${TEST_DIR}/fixtures"
GOLDEN_DIR="${TEST_DIR}/golden"
ARCH_FACTS="${REPO_ROOT}/.opencode/skills/asc-npu-arch/references/npu-arch-facts.json"
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

mapfile -t expected_api_types < <(python3 -c 'import json,sys; print("\n".join(json.load(open(sys.argv[1], encoding="utf-8"))["api_types"].keys()))' "$GENERATION_CONSTRAINTS")
mapfile -t expected_chips < <(python3 -c 'import json,sys; print("\n".join(json.load(open(sys.argv[1], encoding="utf-8"))["chips"].keys()))' "$ARCH_FACTS")

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
<table>
<tr><td>Lines:</td><td>93.6%</td></tr>
<tr><td>Functions:</td><td>91.2%</td></tr>
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

fixture_count=0
for fixture in "${FIXTURE_DIR}"/*.json; do
    [[ -f "$fixture" ]] || fail "no fixture found under ${FIXTURE_DIR}"
    run_case "$(basename "$fixture" .json)"
    fixture_count=$((fixture_count + 1))
done

assert_covered "API type" "$seen_api_types" "${expected_api_types[@]}"
assert_covered "chip" "$seen_chips" "${expected_chips[@]}"

echo "[PASS] asc-api-ut-gen generation golden validation passed (${fixture_count} fixtures)"
