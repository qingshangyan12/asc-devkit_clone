#!/bin/bash
# post-stash-pop Hook
# 触发时机：git stash pop 执行后
# 功能：检测是否产生冲突，如果有冲突则暂停流程等待人工处理

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "============================================================"
echo -e "${YELLOW}[HOOK] post-stash-pop 触发${NC}"
echo "============================================================"
echo ""

# 检测冲突
python .claude/skills/git-code-process/scripts/check_conflict.py
CONFLICT_STATUS=$?

if [ $CONFLICT_STATUS -ne 0 ]; then
    echo ""
    echo "============================================================"
    echo -e "${RED}[警告] 恢复本地修改时检测到冲突！${NC}"
    echo "============================================================"
    echo ""
    echo "这表示本地修改与远程代码产生了冲突。"
    echo ""
    echo "请按照以下步骤解决冲突："
    echo "1. 打开冲突文件，查找冲突标记（<<<<<<<, =======, >>>>>>>）"
    echo "2. 根据需要保留远程代码、本地代码或手动合并"
    echo "3. 删除所有冲突标记"
    echo "4. 保存文件"
    echo "5. 执行：git add <冲突文件>"
    echo ""
    echo "冲突解决方式："
    echo "  - 接受远程代码：git checkout --theirs <文件>"
    echo "  - 接受本地代码：git checkout --ours <文件>"
    echo "  - 手动合并：编辑文件删除冲突标记"
    echo ""
    echo "============================================================"

    # 返回非0状态码，提示需要暂停流程
    exit 1
else
    echo -e "${GREEN}[√] 未检测到冲突，继续流程${NC}"
    echo ""
    exit 0
fi
