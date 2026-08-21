#!/usr/bin/env bash
set -euo pipefail

skill_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../skills/defensive-task-clarifier" && pwd)"
skill_file="$skill_dir/SKILL.md"

for heading in \
    '## Trigger Early' \
    '## Workflow' \
    '## Output Contract' \
    '## Boundaries' \
    '## Example'; do
    rg -qF "$heading" "$skill_file"
done

for phrase in \
    '用户声明' \
    '新会话' \
    '原始 prompt' \
    '不绕过' \
    '不扩大授权范围' \
    '不提供凭证'; do
    rg -qF "$phrase" "$skill_file"
done

echo "defensive-task-clarifier content: PASS"
