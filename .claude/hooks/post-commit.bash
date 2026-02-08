#!/bin/bash
# post-commit Hook
# 触发时机：git commit 执行后
# 功能：验证提交成功，显示提交信息

set -e

# 颜色定义
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "============================================================"
echo -e "${YELLOW}[HOOK] post-commit 触发${NC}"
echo "============================================================"
echo ""

# 获取最新的提交信息
COMMIT_HASH=$(git rev-parse --short HEAD)
COMMIT_MSG=$(git log -1 --pretty=%B)

echo -e "${GREEN}[√] 提交成功${NC}"
echo ""
echo "提交信息："
echo "  Commit ID: $COMMIT_HASH"
git log -1 --pretty=format:"  %s%n" | sed 's/^/  /'
echo ""

# 检查是否有未推送的提交
UNPUSHED=$(git log origin/master..HEAD --oneline 2>/dev/null | wc -l)
if [ $UNPUSHED -gt 0 ]; then
    echo "提示：有 $UNPUSHED 个提交未上传到远程仓"
    echo "下一步：执行 repo upload . 上传到 CodeHub"
else
    echo "提示：所有提交已同步到远程仓"
fi
echo ""

exit 0
