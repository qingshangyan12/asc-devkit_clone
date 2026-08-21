#!/usr/bin/env bash
set -euo pipefail

skill_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../skills/defensive-task-clarifier" && pwd)"

test -f "$skill_dir/SKILL.md"
test -f "$skill_dir/agents/openai.yaml"

rg -q '^name: defensive-task-clarifier$' "$skill_dir/SKILL.md"
rg -q '^description: Use when ' "$skill_dir/SKILL.md"
! rg -q '\[TODO|Structuring This Skill|^## Resources' "$skill_dir/SKILL.md"

echo "defensive-task-clarifier structure: PASS"
