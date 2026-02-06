#!/bin/bash
# pre-upload Hook
# 触发时机：repo upload 执行前
# 功能：检查是否有未推送的提交，验证工作目录状态

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "============================================================"
echo -e "${YELLOW}[HOOK] pre-upload 触发${NC}"
echo "============================================================"
echo ""

# 检查是否有待上传的提交
UNPUSHED=$(git log origin/master..HEAD --oneline 2>/dev/null | wc -l)
if [ $UNPUSHED -eq 0 ]; then
    echo -e "${YELLOW}[警告] 没有新的提交需要上传${NC}"
    echo ""
    echo "当前分支已经是最新的。"
    echo "如果需要上传新提交，请先执行："
    echo "  git add ."
    echo "  git commit -m \"<提交信息>\""
    echo ""
    exit 1
fi

echo -e "${GREEN}[√] 发现 $UNPUSHED 个提交待上传${NC}"
echo ""

# 显示待上传的提交
echo "待上传的提交："
git log origin/master..HEAD --oneline | sed 's/^/  /'
echo ""

# 检查工作目录是否干净
WORKING_DIR_CLEAN=$(git status --porcelain | wc -l)
if [ $WORKING_DIR_CLEAN -gt 0 ]; then
    echo -e "${YELLOW}[提示] 工作目录中有未提交的修改${NC}"
    echo ""
    echo "未提交的修改："
    git status --short | sed 's/^/  /'
    echo ""
    echo "建议：先提交当前修改，再执行 repo upload"
    echo ""
    read -p "是否继续上传？(y/N): " confirm
    if [ "$confirm" != "y" ] && [ "$confirm" != "Y" ]; then
        echo "已取消上传"
        exit 1
    fi
fi

echo -e "${GREEN}[√] 检查通过，准备上传${NC}"
echo ""
exit 0
