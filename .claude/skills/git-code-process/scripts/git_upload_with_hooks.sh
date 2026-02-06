#!/bin/bash
# Git 上库流程脚本（集成 Hooks 机制）
# 本脚本演示了完整的 hooks 机制，包括冲突检测、流程暂停、人工处理、流程恢复

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 日志函数
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_separator() {
    echo "============================================================"
}

# 检测冲突的函数
check_conflict() {
    python .claude/skills/git-code-process/scripts/check_conflict.py
    return $?
}

# 暂停流程，等待用户处理
pause_for_user() {
    print_separator
    log_warn "自动化流程已暂停"
    print_separator
    echo ""
    echo "请按照以下步骤解决冲突："
    echo "1. 打开冲突文件，查找冲突标记"
    echo "   冲突标记：<<<<<<<, =======, >>>>>>>"
    echo "2. 根据需要保留远程代码、本地代码或手动合并"
    echo "3. 删除所有冲突标记"
    echo "4. 保存文件"
    echo "5. 执行：git add <冲突文件>"
    echo ""
    read -p "冲突解决后，按回车键继续..."
    echo ""
}

# 主流程
main() {
    print_separator
    echo "Git 代码上库流程（集成 Hooks 机制）"
    print_separator
    echo ""

    # === 步骤1：环境检查 ===
    log_info "步骤1：环境检查"
    git status
    git branch -v
    echo ""

    # === 步骤2：暂存本地修改 ===
    log_info "步骤2：暂存本地修改"
    git stash
    git status
    echo ""

    # === 步骤3：同步远程代码 ===
    log_info "步骤3：同步远程代码"
    git pull
    echo ""

    # === Hook：post-pull 冲突检测 ===
    log_info "Hook 检测：检查是否产生冲突"
    if check_conflict; then
        pause_for_user
    else
        log_info "未检测到冲突，继续流程"
    fi
    echo ""

    # === 步骤4：恢复本地修改 ===
    log_info "步骤4：恢复本地修改"
    git stash pop
    echo ""

    # === Hook：post-stash-pop 冲突检测 ===
    log_info "Hook 检测：检查是否产生冲突"
    if check_conflict; then
        pause_for_user
    else
        log_info "未检测到冲突，继续流程"
    fi
    echo ""

    # === 步骤5：添加到暂存区 ===
    log_info "步骤5：添加到暂存区"
    git add .
    git status
    echo ""

    # === 步骤6：本地提交 ===
    log_info "步骤6：本地提交"
    echo "请输入提交信息（按照规范格式）："
    echo "格式：DTS: <需求单号>"
    echo "      Description: <修改描述>"
    echo "      Team: <团队>"
    echo "      Feature or Bugfix: <类型>"
    echo ""
    read -p "DTS: " dts_number
    read -p "Description: " description
    read -p "Team: " team
    read -p "Feature or Bugfix: " feature_type

    git commit -m "DTS: ${dts_number}" -m "Description: ${description}" -m "Team: ${team}" -m "Feature or Bugfix: ${feature_type}"
    git log --oneline -3
    echo ""

    # === 步骤7：上传到 CodeHub ===
    log_info "步骤7：上传到 CodeHub"
    repo upload .

    print_separator
    log_info "上库流程完成！"
    print_separator
}

# 执行主流程
main "$@"
