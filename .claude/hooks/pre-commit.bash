#!/bin/bash
# pre-commit Hook
# 触发时机：git commit 执行前
# 功能：检查暂存区状态，验证没有冲突文件

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "============================================================"
echo -e "${YELLOW}[HOOK] pre-commit 触发${NC}"
echo "============================================================"
echo ""

# 检查暂存区状态
STAGED_FILES=$(git diff --cached --name-only)
if [ -z "$STAGED_FILES" ]; then
    echo -e "${RED}[错误] 暂存区为空，没有要提交的文件${NC}"
    echo "请先执行：git add <文件>"
    echo ""
    exit 1
fi

echo -e "${GREEN}[√] 暂存区检查通过${NC}"
echo "待提交文件数：$(echo "$STAGED_FILES" | wc -l)"
echo ""

# 再次检测冲突（防止提交冲突文件）
python .claude/skills/git-code-process/scripts/check_conflict.py --quiet
CONFLICT_STATUS=$?

if [ $CONFLICT_STATUS -ne 0 ]; then
    echo -e "${RED}[错误] 工作目录中存在冲突，不能提交${NC}"
    echo ""
    echo "请先解决冲突后再提交："
    echo "1. 查看冲突文件：python .claude/skills/git-code-process/scripts/check_conflict.py"
    echo "2. 解决冲突"
    echo "3. 标记冲突已解决：git add <冲突文件>"
    echo ""
    exit 1
fi

echo -e "${GREEN}[√] 冲突检查通过，可以提交${NC}"
echo ""
exit 0
