# PR 与分支映射关系

## 文

本文件用于管理当前 fork 仓库的分支与 PR（Pull Request）的对应关系，解决以下痛点：

- **痛点**：每次提新的 PR 都需要从主线仓重新 fork 一个干净的目录，然后 clone 到本地，流程繁琐且浪费时间。
- **解决方案**：**只 fork 一次**。每次新建 PR 时，基于主线仓 `master` 的最新内容直接创建新功能分支，提 PR 时基于该新分支。
- **本文件的作用**：
  1. 提供自动化的脚本，一键完成「拉取主线 master 最新内容 → 基于主线 master 创建新分支 → 推送到 fork 仓」的流程。
  2. 以表格形式记录所有已创建的分支与对应的 PR 标题、PR 号的映射关系，方便后续回溯查找。

---

## 使用流程

1. **首次使用**：确保本地仓库的 `upstream` 远程仓库指向主线仓（主线仓地址见下方脚本）。
2. **新建 PR 时**：运行下方脚本，输入 PR 标题，脚本自动基于主线仓 master 最新内容创建以日期命名的分支并推送到 fork 仓。
3. **在新建分支上开发**，完成后提交并推送，然后到 GitHub/GitLab 上提 PR。
4. **将分支与 PR 信息**记录到下方的映射表格中。
5. **PR 合入后**，可将对应分支在本地和远程删除，保持仓库整洁。

---

## 自动化脚本

以下脚本适用于 **Linux** 环境。将脚本保存为 `new-pr-branch.sh` 并按以下方式执行：

```bash
# 添加可执行权限
chmod +x new-pr-branch.sh

# 执行（交互式输入 PR 标题）
./new-pr-branch.sh

# 或直接传入 PR 标题（非交互）
./new-pr-branch.sh add_l2_cache_feature
```

```bash
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
echo "请将分支和 PR 信息记录到映射表格中。"
```

### 脚本说明

| 步骤 | 操作 | 说明 |
|:---|:---|:---|
| 1 | `git fetch upstream master` | 从主线仓拉取最新 master 内容 |
| 2 | `git checkout -b 新分支 upstream/master` + `git push -u origin 新分支` | 直接基于主线 master 最新 commit 创建新分支并推送到 fork 仓 |

> **注意**：运行脚本前，请将 `UPSTREAM_URL` 变量修改为实际的主线仓地址。

---

## PR 与分支映射表

| 创建日期 | 分支名称 | PR 标题 | PR 号 | 状态 | 备注 |
|:---:|:---|:---|---:|:---:|:---|
| 2025_06_13 | `2025_06_13_add_l2_cache_example` | Add L2 Cache bypass example | #1 | ✅ 已合入 | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |

### 表格字段说明

- **创建日期**：分支创建的日期，格式 `YYYY_MM_DD`
- **分支名称**：实际的分支名，格式 `YYYY_MM_DD_PR标题`
- **PR 标题**：提交 PR 时使用的标题
- **PR 号**：PR 的编号（如 `#42`），PR 提交后补充
- **状态**：`⏳ 进行中` / `✅ 已合入` / `❌ 已关闭`
- **备注**：其他需要记录的信息

---

## 分支清理建议

PR 合入主线后，可以清理已合并的分支以保持仓库整洁：

```bash
# 删除本地分支
git branch -d 2025_06_13_add_l2_cache_example

# 删除远程分支
git push origin --delete 2025_06_13_add_l2_cache_example
```
