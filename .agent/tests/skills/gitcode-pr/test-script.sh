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
SCRIPT_FILE="${REPO_ROOT}/.agent/skills/gitcode-pr/scripts/create_pr.py"
PR_API_SCRIPT="${REPO_ROOT}/.agent/skills/gitcode-pr/scripts/pr_api.py"
TMP_DIR="$(mktemp -d)"

cleanup() {
    rm -rf "$TMP_DIR"
}
trap cleanup EXIT

fail() {
    echo "[FAIL] $*" >&2
    exit 1
}

BODY_FILE="${TMP_DIR}/body.md"
OUTPUT_FILE="${TMP_DIR}/dry-run.json"
ERROR_FILE="${TMP_DIR}/error.txt"
cat > "$BODY_FILE" <<'EOF'
# Pull Request

dry-run body
EOF

python3 "$SCRIPT_FILE" \
    --target "https://gitcode.com/cann/asc-devkit.git" \
    --head "alice:fix/gitcode-pr" \
    --base "master" \
    --title "test: dry run" \
    --body-file "$BODY_FILE" \
    --dry-run > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["target"] == "cann/asc-devkit"
assert data["payload"]["head"] == "alice:fix/gitcode-pr"
assert data["payload"]["base"] == "master"
assert data["payload"]["title"] == "test: dry run"
assert "dry-run body" in data["payload"]["body"]
PY

set +e
env -u GITCODE_API_TOKEN -u GITCODE_TOKEN python3 "$SCRIPT_FILE" \
    --target "cann/asc-devkit" \
    --head "alice:fix/gitcode-pr" \
    --base "master" \
    --title "test: missing token" \
    --body "body" > /dev/null 2> "$ERROR_FILE"
status="$?"
set -e

[[ "$status" -eq 2 ]] || fail "missing token path must exit with status 2"
grep -Fq "missing token" "$ERROR_FILE" || fail "missing token path must explain the failure"

python3 "$SCRIPT_FILE" \
    --target "git@gitcode.com:cann/asc-devkit.git" \
    --head "bob:feature/a" \
    --title "test: ssh url" \
    --dry-run > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["target"] == "cann/asc-devkit"
assert data["payload"]["base"] == "master"
assert data["payload"]["head"] == "bob:feature/a"
PY

python3 "$SCRIPT_FILE" \
    --target "cann/asc-devkit" \
    --head "carol:fix/issue-123" \
    --base "master" \
    --title "fix: issue template" \
    --description "template description" \
    --issue "#123" \
    --dry-run > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

body = data["payload"]["body"]
assert "template description" in body
assert "Closes #123" in body
assert "## 关联的Issue" in body
assert "## 类型标签" in body
assert "{{description}}" not in body
assert "{{issue}}" not in body
PY

python3 "$PR_API_SCRIPT" \
    --target "cann/asc-devkit" \
    --dry-run \
    changes --pr 42 > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["method"] == "GET"
assert data["api_style"] == "gitlab-v4"
assert data["token"] == "<redacted>"
assert "/projects/cann%2Fasc-devkit/merge_requests/42/changes?per_page=100" in data["url"]
PY

python3 "$PR_API_SCRIPT" \
    changes \
    --target "cann/asc-devkit" \
    --dry-run \
    --pr 45 \
    --per-page 50 > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["method"] == "GET"
assert data["api_style"] == "gitlab-v4"
assert "/projects/cann%2Fasc-devkit/merge_requests/45/changes?per_page=50" in data["url"]
PY

python3 "$PR_API_SCRIPT" \
    --target "https://gitcode.com/cann/asc-devkit.git" \
    --dry-run \
    discussions --pr 43 > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["method"] == "GET"
assert data["api_style"] == "gitlab-v4"
assert "/projects/cann%2Fasc-devkit/merge_requests/43/discussions" in data["url"]
PY

python3 "$PR_API_SCRIPT" \
    --dry-run \
    comments --pr-url "https://gitcode.com/cann/asc-devkit/pull/44" > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["method"] == "GET"
assert data["api_style"] == "gitlab-v4"
assert "/projects/cann%2Fasc-devkit/merge_requests/44/notes" in data["url"]
PY

python3 "$PR_API_SCRIPT" \
    --target "git@gitcode.com:cann/asc-devkit.git" \
    --dry-run \
    delete-comment --comment-id 12345 > "$OUTPUT_FILE"

python3 - "$OUTPUT_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    data = json.load(f)

assert data["method"] == "DELETE"
assert data["api_style"] == "gitcode-v5"
assert "/repos/cann/asc-devkit/pulls/comments/12345" in data["url"]
assert "access_token=%3Credacted%3E" in data["url"]
PY

set +e
env -u GITCODE_API_TOKEN -u GITCODE_TOKEN python3 "$PR_API_SCRIPT" \
    --target "cann/asc-devkit" \
    comments --pr 42 > /dev/null 2> "$ERROR_FILE"
status="$?"
set -e

[[ "$status" -eq 2 ]] || fail "PR API missing token path must exit with status 2"
grep -Fq "missing token" "$ERROR_FILE" || fail "PR API missing token path must explain the failure"

echo "[PASS] gitcode-pr script validation passed"
