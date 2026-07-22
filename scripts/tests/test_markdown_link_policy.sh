#!/usr/bin/env bash
# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set -euo pipefail

REPO_ROOT=$(git rev-parse --show-toplevel)
CHECKER="$REPO_ROOT/scripts/markdown_link_check.sh"
TEST_ROOT=$(mktemp -d)
trap 'rm -rf "$TEST_ROOT"' EXIT

git -C "$TEST_ROOT" init -q -b master
git -C "$TEST_ROOT" config user.name test
git -C "$TEST_ROOT" config user.email test@example.com
mkdir -p "$TEST_ROOT/docs/zh/api" "$TEST_ROOT/docs/zh/guide" "$TEST_ROOT/examples/sample"
touch "$TEST_ROOT/markdown-link-check.toml"
printf '# example\n' > "$TEST_ROOT/examples/sample/README.md"
printf '# API\n' > "$TEST_ROOT/docs/zh/api/target.md"
printf '# Guide\n' > "$TEST_ROOT/docs/zh/guide/target.md"
cat > "$TEST_ROOT/docs/zh/api/relative.md" <<'EOF'
[file](../../../examples/sample/README.md)
[directory](../../../examples/sample)
[guide](../guide/target.md)
EOF
printf '%s\n' '[example](https://gitcode.com/cann/asc-devkit/tree/master/examples/sample)' \
    > "$TEST_ROOT/docs/examples_https.md"
printf '%s\n' '[guide](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/guide/target.md)' \
    > "$TEST_ROOT/docs/zh/api/guide_https.md"
printf '%s\n' '[api](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/target.md)' \
    > "$TEST_ROOT/docs/zh/guide/api_https.md"
printf '%s\n' '[external](https://example.com/reference)' > "$TEST_ROOT/docs/external.md"
cat > "$TEST_ROOT/lychee" <<'EOF'
#!/usr/bin/env bash
exit 0
EOF
chmod +x "$TEST_ROOT/lychee"
git -C "$TEST_ROOT" add .
git -C "$TEST_ROOT" commit -q -m fixtures

failures=0
run_check() {
    (cd "$TEST_ROOT" && MARKDOWN_LINK_CHECK_LYCHEE="$TEST_ROOT/lychee" bash "$CHECKER" "$1")
}

expect_pass() {
    local source=$1
    if ! run_check "$source" >/dev/null 2>&1; then
        echo "expected PASS: $source" >&2
        failures=$((failures + 1))
    fi
}

expect_fail() {
    local source=$1 expected=$2 output
    if output=$(run_check "$source" 2>&1); then
        echo "expected FAIL: $source" >&2
        failures=$((failures + 1))
    elif ! grep -Fq "$expected" <<<"$output"; then
        echo "missing diagnostic for $source: $expected" >&2
        failures=$((failures + 1))
    fi
}

expect_pass docs/zh/api/relative.md
expect_pass docs/external.md
expect_fail docs/examples_https.md "docs link to examples must use a relative path"
expect_fail docs/zh/api/guide_https.md "API and guide cross-reference must use a relative path"
expect_fail docs/zh/guide/api_https.md "API and guide cross-reference must use a relative path"
exit "$failures"
