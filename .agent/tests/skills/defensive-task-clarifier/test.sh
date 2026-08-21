#!/usr/bin/env bash
set -euo pipefail

test_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
bash "$test_dir/test-structure.sh"
bash "$test_dir/test-content.sh"
