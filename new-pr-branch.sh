#!/bin/bash
# new-pr-branch.sh
# 功能：基于主线仓 master 最新内容创建新分支并推送到 fork 仓
# 用法：./new-pr-branch.sh [PR标题]
# 要求：需要先配置 UPSTREAM_URL 为实际的主线仓地址

set -euo pipefail

# ===== 配置区：请根据实际情况修改以下变量 =====
UPSTREAM_URL="https://gitcode.com/cann/asc-devkit"   # 主线仓地址（替换为实际地址）
UPSTREAM_REMOTE="upstream"                                  # 主线仓的远程名称
FORK_REMOTE="origin"                                        # fork 仓的远程名称
# ===============================================
echo "===== 常用 Git 命令 ====="
echo "查看当前分支状态:         git status"
echo "查看分支列表:             git branch -a"
echo "删除本地分支:             git branch -d <branch_name>"
# 检查是否在 Git 仓库中
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "错误：当前目录不是一个 Git 仓库，请在仓库根目录下运行此脚本。" >&2
    exit 1
fi

# 检查并添加 upstream 远程仓库
if ! git remote get-url "$UPSTREAM_REMOTE" > /dev/null 2>&1; then
    echo "添加远程仓库: $UPSTREAM_REMOTE -> $UPSTREAM_URL"
    git remote add "$UPSTREAM_REMOTE" "$UPSTREAM_URL"
fi

# 获取 PR 标题
if [ $# -ge 1 ]; then
    pr_title="$1"
else
    read -r -p "请输入 PR 标题（英文，用下划线分隔，如 add_l2_cache_feature）: " pr_title
    if [ -z "$pr_title" ]; then
        echo "错误：PR 标题不能为空" >&2
        exit 1
    fi
fi

# 生成分支名称：格式为 202x_0x_0x_PR标题
date_str=$(date +%Y_%m_%d)
branch_name="${date_str}_${pr_title}"

# 检查分支是否已存在
if git show-ref --verify --quiet "refs/heads/$branch_name"; then
    echo "错误：本地分支 '$branch_name' 已存在，请检查或更换 PR 标题。" >&2
    exit 1
fi
if git show-ref --verify --quiet "refs/remotes/$FORK_REMOTE/$branch_name"; then
    echo "错误：远程分支 '$FORK_REMOTE/$branch_name' 已存在，请检查或更换 PR 标题。" >&2
    exit 1
fi

echo ""
echo "===== 步骤 1/2：从主线仓拉取 master 最新内容 ====="
git fetch "$UPSTREAM_REMOTE" master

echo ""
echo "===== 步骤 2/2：基于主线 master 创建新分支并推送到 fork 仓 ====="
git checkout -b "$branch_name" "$UPSTREAM_REMOTE/master"
git push -u "$FORK_REMOTE" "$branch_name"

echo ""
echo "===== 完成 ====="
echo "已创建并推送分支: $branch_name"
echo "该分支基于主线仓 master 最新 commit，与本地 master 无关。"
echo ""
