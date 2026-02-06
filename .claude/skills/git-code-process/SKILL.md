---
name: git-code-process
description: 采用git进行代码管理，完成添加文件到暂存区、提交修改、repo upload提交到codehub平台等一系列操作。
---

# Git代码处理技能

## 一、技能概述

本技能（git-code-process）为华为 CANN CI/CD 体系下 **ci_config** 仓库的工程需求开发提供**标准化代码上库流程**，核心支撑该仓库内修改后的 YAML 配置文件的**提交、上库**等全流程操作。

**⚠️ 重要提示：本技能是代码提交上库的唯一标准化途径，所有代码提交操作必须通过本技能执行，禁止绕过本技能直接使用 git 命令。**

本技能的核心价值在于：
- **标准化流程**：提供统一的 Git 操作规范，确保代码上库流程的一致性
- **冲突预防**：通过 git stash → git pull → git stash pop 机制，有效避免代码冲突
- **规范提交**：强制使用符合项目要求的提交信息格式（DTS、Description、Team、Feature or Bugfix）
- **自动化执行**：自动化完成 add、commit、repo upload 等操作
- **安全可靠**：内置安全检查机制，避免误操作导致的数据丢失
- **可追溯性**：完整的操作记录和日志，便于问题追踪和审计

## 二、重要操作原则

### 2.1 ⚠️ 强制要求

**关键原则：所有代码上库操作必须优先使用本技能提供的标准化流程**

**为什么必须使用 git-code-process 技能？**
1. **避免代码冲突**：通过 git stash → git pull → git stash pop 机制，确保代码基于最新基线
2. **标准化流程**：统一的操作规范，减少人为错误
3. **规范提交信息**：强制使用符合项目要求的提交格式（DTS、Description、Team、Feature or Bugfix）
4. **可追溯性**：完整的操作记录，便于问题追踪
5. **安全性**：内置安全检查，避免危险操作

**核心原则：**
- **强制要求**：任何涉及代码提交、上库的操作，**必须**使用本技能提供的标准化流程；
- **禁止绕过**：禁止绕过本技能直接使用 Bash 工具执行 git 命令；
- **前置检查**：执行上库操作前，必须确认 YAML 配置文件修改已完成并通过验证；
- **分支管理**：必须基于正确的分支进行操作，避免分支混乱；
- **提交信息规范**：提交信息必须清晰、准确地描述修改内容；
- **禁止强制操作**：禁止使用 `--force` 等危险参数，除非经过明确授权；
- **工具优先级**：Skill工具(git-code-process技能) > Bash工具(git命令) > 手动操作。

### 2.2 核心操作流程

| 操作步骤 | 功能说明 | 典型使用场景 |
|---------|---------|-------------|
| **状态检查** | 使用 git status 查看当前工作目录状态、分支信息、修改内容 | 确认修改范围和影响 |
| **暂存修改** | 使用 git stash 保存本地修改 | 上库前保护本地修改 |
| **同步代码** | 使用 git pull 拉取远程最新代码 | 避免基于过期代码修改 |
| **恢复修改** | 使用 git stash pop 恢复暂存的修改 | 在最新代码基础上应用修改 |
| **添加到暂存区** | 使用 git add 将修改的文件添加到 Git 暂存区 | 准备提交的文件 |
| **提交修改** | 使用 git commit 创建本地提交，附带规范的提交信息 | 记录修改历史 |
| **repo upload** | 使用 repo upload 通过 repo 工具上传到 CodeHub 平台 | 华为内部代码审查流程 |

**技术特性：**
- **自动备份**：操作前自动检查并提示备份
- **分支验证**：确认当前分支是否为目标分支
- **提交信息规范**：支持标准化提交信息格式
- **日志记录**：完整记录所有操作步骤和结果
- **CodeHub集成**：直接对接华为 CodeHub 平台
- **冲突预防**：通过 stash + pull 机制避免代码冲突

## 三、详细操作指南

### 3.1 状态检查：git_status

查看当前工作目录状态，确认修改内容。

**命令格式：**
```bash
# 查看工作目录状态
git status

# 查看分支信息
git branch -v

# 查看修改内容
git diff

# 查看已暂存的修改
git diff --staged

```

**检查要点：**
- 当前分支是否正确
- 修改的文件是否与预期一致
- 是否有未跟踪的文件需要添加
- 是否有冲突需要解决

### 3.2 添加到暂存区：git_add

将修改的文件添加到 Git 暂存区。

**命令格式：**
```bash
# 添加所有修改的文件 （推荐）
git add .

# 添加指定文件
git add ci_config/rule.yaml

# 添加多个指定文件
git add ci_config/rule.yaml ci_config/custom/rule_custom.yaml

```

**参数说明：**
- `.`：添加当前目录下所有修改的文件
- `<file_path>`：添加指定路径的文件
- `-i`：交互式添加，可逐个确认要添加的文件
- `-u`：添加已跟踪文件的修改（不包含新文件）
- `-A`：添加所有修改（包括删除和新增）

**注意事项：**
- 添加前确认文件内容正确无误
- 避免将敏感文件（如 .env、临时文件）添加到暂存区
- 建议使用 `git status` 确认暂存的文件列表

### 3.3 提交修改：git_commit

创建本地提交，记录修改历史。

**命令格式：**
```bash
# 提交并附带详细说明（首次提交或上传后的首次提交推荐）
git commit -m "关联的需求或问题单" -m "描述修改内容" -m "所属团队"  -m "新功能还是修复bug"

# 再次提交（未上传前）
git commit --amend

# 查看提交历史
git log --oneline -5
```

**提交信息规范：**
```bash
# 格式要求：<type>: <subject>

# 必须包含以下四个类型（type）：
# - DTS: 关联的需求或问题单
# - Description: 描述修改内容
# - Team: 所属团队
# - Feature or Bugfix: 新功能还是修复bug

# 示例： 
git commit -m "DTS: AR20250113161172" -m "Description: vcast任务超时，改为非拦截" -m "Team: HISI_EE" -m "Feature or Bugfix: Feature"

```

**参数说明：**
- `-m, --message`：提交信息
- `--amend`：修改最后一次提交（⚠️ 仅在未 repo upload 前使用）
- `--no-verify`：跳过 pre-commit 钩子（谨慎使用）
- `-v, --verbose`：显示提交的详细信息

**注意事项：**
- 提交的**Description**信息应清晰、准确地描述修改内容
- 单次提交应聚焦于单一功能或修复
- 提交前确认暂存区的文件正确
- **重要**：已 repo upload 的提交不要使用 `--amend`

### 3.4 repo upload：上传到 CodeHub

通过 repo 工具上传代码到华为 CodeHub 平台，进行代码审查。

**命令格式：**
```bash
# 上传当前项目的修改（推荐）
repo upload .

# 查看上传状态
repo status
```

**参数说明：**
- `.`：当前项目

**典型使用流程：**
```bash
# 上传修改到 CodeHub
repo upload .
```

**交互确认：**
执行 `repo upload .` 后，会提示：
```
Upload project ... to remote branch ... (y/N)?
```
输入 `y` 并回车确认上传。

**注意事项：**
- 上传前确认 repo 工具已正确配置
- 上传前确认所有提交已完成本地验证
- 如需修改已上传的代码，可在本地修改后重新 upload

## 四、标准化上库流程

### 4.1 完整流程图

```
【步骤1】环境检查
    ├─ 检查当前分支
    ├─ 检查工作目录状态
    └─ 确认修改文件列表

【步骤2】暂存本地修改
    ├─ git stash 暂存本地修改
    └─ 确认工作目录干净

【步骤3】同步远程代码
    ├─ git pull 拉取远程最新代码
    └─ 确认无冲突

【步骤4】恢复本地修改
    ├─ git stash pop 恢复暂存的修改
    └─ 解决可能的冲突

【步骤5】添加到暂存区
    ├─ git add 添加修改文件
    └─ git status 确认暂存内容

【步骤6】本地提交
    ├─ 撰写规范的提交信息
    ├─ git commit 创建提交
    └─ git log 确认提交记录

【步骤7】上传到 CodeHub
    └─ repo upload 上传到 CodeHub
```

### 4.2 操作示例

**场景：完成 YAML 配置修改后的上库流程**

```bash
# === 步骤1：环境检查 ===
git status              # 查看工作目录状态
git branch -v           # 查看当前分支

# === 步骤2：暂存本地修改 ===
git stash               # 暂存本地修改
git status              # 确认工作目录干净

# === 步骤3：同步远程代码 ===
git pull  # 拉取远程最新代码

# === 步骤4：恢复本地修改 ===
git stash pop           # 恢复暂存的修改
# 如有冲突，手动解决后继续

# === 步骤5：添加到暂存区 ===
git add .
git status              # 确认暂存内容

# === 步骤6：本地提交 ===
git commit -m "DTS: AR20250113161172" -m "Description: vcast任务超时，改为非拦截" -m "Team: HISI_EE" -m "Feature or Bugfix: Feature"
git log --oneline -3    # 确认提交记录

# === 步骤7：上传到 CodeHub ===
repo upload .
# 看到提示 "Upload project ... to remote branch ... (y/N)?" 时，输入 y 确认
```

## 五、异常处理

### 5.1 git stash pop 冲突

**现象：**
```bash
git stash pop
# CONFLICT (content): Merge conflict in ci_config/rule.yaml
```

**处理步骤：**
1. 查看冲突文件：`git status`
2. 打开冲突文件，查找冲突标记（<<<<<<<、=======、>>>>>>>）
3. 手动编辑文件，保留需要的内容，删除冲突标记
4. 标记冲突已解决：`git add <冲突文件>`
5. 确认所有冲突已解决：`git status`
6. 继续提交和上传流程

**冲突标记示例：**
```yaml
# <<<<<<< Updated upstream
# 远程代码
version: cann_new
# =======
# 本地代码
version: cann
# >>>>>>> Stashed changes
```

### 5.2 提交信息错误

**场景：提交后发现信息写错了（未 repo upload 前）**

```bash
# 修改最后一次提交信息
git commit --amend -m "正确的提交信息"

# ⚠️ 注意：如果已经 repo upload，不要使用 --amend
# 应创建新的提交进行修正
```

### 5.4 repo upload 失败

**常见原因：**
- 网络问题
- 权限不足
- repo 配置错误
- 远程有新的提交未同步

**解决方案：**
```bash
# 1. 先同步远程最新代码
git pull

# 2. 检查网络连接
ping repo.example.com

# 3. 检查 repo 配置
repo info

# 4. 检查权限
git config --list | grep user

# 5. 重新上传
repo upload .
```

### 5.5 误操作恢复

**场景：不慎执行了危险操作**

```bash
# 恢复到指定的提交
git reflog                       # 查看操作历史
git reset --hard <commit_id>     # 恢复到指定提交

# ⚠️ 注意：
# - 如果已 repo upload，恢复后需重新上传
# - 强制恢复会丢失后续的提交，谨慎使用
```

## 六、最佳实践

### 6.1 提交频率

- **小步快跑**：频繁提交小改动，而不是一次性提交大量修改
- **逻辑完整**：每次提交应是一个逻辑完整的改动
- **及时提交**：完成一个功能点后立即提交，避免积累过多修改

### 6.2 提交信息

- **清晰明确**：提交信息应准确描述改动内容
- **规范格式**：必须使用 `<type>: <subject>` 格式
- **适当详细**：复杂改动可添加详细说明

**良好示例：**
```bash
git commit -m "DTS: AR20250113161172" -m "Description: vcast任务超时，改为非拦截" -m "Team: HISI_EE" -m "Feature or Bugfix: Feature"
```

**避免示例：**
```bash
git commit -m "fix"          # ❌ 太简单
git commit -m "update"       # ❌ 不明确
git commit -m "修改文件"      # ❌ 缺少细节
```

### 6.3 分支管理

- **功能分支**：新功能开发应使用独立的分支
- **定期同步**：定期执行 `git pull` 同步最新代码

### 6.3.1 避免冲突的关键步骤

**上库前必须执行以下步骤：**
1. **暂存本地修改**：`git stash` 保存本地修改
2. **同步远程代码**：`git pull` 拉取最新代码
3. **恢复本地修改**：`git stash pop` 恢复暂存的修改
4. **解决冲突**：如有冲突，手动解决后提交

**为什么必须这样做：**
- 避免基于过期代码进行修改
- 减少合并冲突的概率
- 确保提交基于最新的代码基线

### 6.4 repo upload 建议

- **上传前验证**：确保代码通过测试和自检
- **小批量上传**：避免单次上传过多改动

### 6.5 安全原则

- **必须使用 git-code-process 技能**：所有代码提交操作必须通过本技能执行，禁止绕过直接使用 git 命令
- **备份重要数据**：执行危险操作前先备份
- **审查代码**：上传前确认代码质量
- **测试验证**：确保修改通过测试后再上传
- **权限管理**：妥善保管账号和密钥

**违反安全原则的后果：**
- 绕过 git-code-process 技能直接提交可能导致代码冲突
- 不使用 stash + pull 机制可能导致基于过期代码提交
- 不规范的提交信息可能导致代码被驳回
- 危险操作可能导致数据丢失且无法恢复

## 七、维护信息

- 技能名称：git-code-process
- 技能路径：`.claude/skills/git-code-process/`
- 文档位置：`.claude/skills/git-code-process/SKILL.md`
- 更新日期：2025-02-05

## 八、常见问题

### Q1：为什么必须使用 git-code-process 技能提交代码？

**回答：**
使用 git-code-process 技能是强制要求，原因如下：

1. **避免代码冲突**
   - 技能内置 git stash → git pull → git stash pop 机制
   - 确保每次提交都基于最新的远程代码
   - 大幅减少合并冲突的概率

2. **标准化流程**
   - 统一的 Git 操作规范，减少人为错误
   - 强制使用符合项目要求的提交信息格式
   - 确保所有提交信息完整且规范

3. **安全性**
   - 内置安全检查，避免危险操作
   - 防止误用 `--force` 等危险参数
   - 完整的操作记录，便于问题追踪

4. **可追溯性**
   - 记录所有操作步骤
   - 便于审查和回溯
   - 符合 CI/CD 审计要求

**错误做法：**
```bash
# ❌ 错误：直接使用 git 命令
git add .
git commit -m "fix bug"
git push
```

**正确做法：**
```bash
# ✅ 正确：使用 git-code-process 技能
Skill(skill="git-code-process", args="提交代码")
```

### Q2：如何查看当前所在的分支？
```bash
git branch                  # 显示所有分支，当前分支前有 * 号
git branch --show-current   # 仅显示当前分支名
```

### Q3：如何撤销未提交的修改？
```bash
# 撤销工作目录的修改
git checkout -- <file_path>

# 撤销暂存区的修改
git reset HEAD <file_path>
```

### Q4：如何查看提交历史？
```bash
git log                    # 详细提交历史
git log --oneline          # 简洁提交历史
git log --graph            # 图形化展示分支历史
```

### Q5：repo upload 后如何修改代码？

如需修改已上传的代码：
```bash
# 1. 在本地修改代码
# 2. 提交修改
git add .
git commit -m "DTS: AR20250113161172" -m "Description: vcast任务超时，改为非拦截" -m "Team: HISI_EE" -m "Feature or Bugfix: Feature"

# 3. 重新上传
repo upload .
```

### Q6：如何使用 git stash 保存临时修改？
```bash
# 保存当前修改（包括暂存区和工作目录）
git stash

# 保存时添加说明
git stash save "修改rule.yaml配置"

# 查看暂存列表
git stash list

# 恢复最近的暂存并删除
git stash pop

# 恢复指定的暂存
git stash pop stash@{n}

# 仅恢复暂存，不删除
git stash apply

# 删除指定暂存
git stash drop stash@{n}

# 清空所有暂存
git stash clear
```

### Q7：为什么上库前要先 git stash 再 git pull？

**原因：**
1. **避免基于过期代码修改**：确保本地修改基于最新的远程代码
2. **减少冲突概率**：先拉取最新代码，再应用本地修改，更容易发现和解决冲突
3. **保护本地修改**：stash 保存本地修改，避免 pull 时被覆盖或产生复杂冲突

**正确流程：**
```bash
# 1. 保存本地修改
git stash

# 2. 拉取远程最新代码
git pull 

# 3. 恢复本地修改
git stash pop

# 4. 如有冲突，手动解决
# 5. 提交并上传
```

### Q8：如何清理未跟踪的文件？
```bash
# 查看未跟踪的文件
git clean -n

# 删除未跟踪的文件（不删除目录）
git clean -f

# 删除未跟踪的文件和目录
git clean -fd

# ⚠️ 注意：清理前请确认，避免误删重要文件
```

## 十、相关技能

- **edit-yaml**：YAML 配置文件修改技能
- **user_require_analysis**：用户需求分析技能

本技能应在 YAML 配置文件修改完成后使用，完成代码的标准化上库流程。

**典型工作流：**
```
用户需求
  → user_require_analysis（分析需求）
  → edit-yaml（修改配置）
  → git-code-process（上库）✅ 本技能
```
