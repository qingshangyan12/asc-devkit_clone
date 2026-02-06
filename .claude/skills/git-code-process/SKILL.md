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
- **冲突处理**：通过 hooks 机制实现冲突检测、流程暂停、人工干预、流程恢复
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
- **冲突处理**：通过 hooks 机制实现冲突检测、流程暂停、人工干预、流程恢复

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

### 4.3 Hooks 机制与冲突处理

本技能通过 **hooks 机制** 实现代码冲突的人工干预流程，确保在遇到冲突时能够安全地暂停、处理和恢复。

#### 4.3.1 Hooks 触发点

在以下关键操作点会触发 hooks：

| Hook 点 | 触发时机 | 功能 |
|---------|---------|------|
| **pre-stash** | git stash 执行前 | 检查是否有未提交的修改 |
| **post-stash** | git stash 执行后 | 验证暂存是否成功 |
| **pre-pull** | git pull 执行前 | 检查网络连接和远程仓库状态 |
| **post-pull** | git pull 执行后 | **检测是否有冲突**，如有冲突则暂停流程 |
| **pre-stash-pop** | git stash pop 执行前 | 检查工作目录状态 |
| **post-stash-pop** | git stash pop 执行后 | **检测是否有冲突**，如有冲突则暂停流程 |
| **pre-commit** | git commit 执行前 | 检查暂存区状态和提交信息格式 |
| **post-commit** | git commit 执行后 | 验证提交是否成功 |
| **pre-upload** | repo upload 执行前 | 检查本地是否有未推送的提交 |
| **post-upload** | repo upload 执行后 | 验证上传是否成功 |

#### 4.3.2 冲突检测流程

**自动冲突检测：**

```bash
# post-pull 和 post-stash-pop hook 会自动执行：
git status | grep "Unmerged files"  # 检测未合并的文件
git status | grep "both modified"   # 检测冲突文件
```

**检测到冲突时的处理流程：**

```
【检测到冲突】
    ↓
【Hook 暂停】自动化流程暂停
    ↓
【通知用户】显示冲突文件列表和冲突标记示例
    ↓
【等待人工处理】提供详细的冲突解决指导
    ↓
【用户解决冲突】手动编辑文件，解决冲突
    ↓
【用户确认】告知系统冲突已解决
    ↓
【Hook 恢复】验证冲突已解决，继续后续流程
```

#### 4.3.3 冲突处理完整流程

**步骤1：冲突检测与暂停**

```bash
# 系统检测到冲突
============================================================
⚠️  检测到代码冲突！
============================================================

冲突文件列表：
  - ci_config/rule.yaml
  - ci_config/custom/config.yaml

自动化流程已暂停，请按照以下步骤解决冲突。
```

**步骤2：人工解决冲突**

打开冲突文件，查找冲突标记：

```yaml
# <<<<<<< Updated upstream
# 远程代码
version: cann_new
enabled: true
# =======
# 本地代码
version: cann
enabled: false
# >>>>>>> Stashed changes
```

**解决冲突的三种方式：**

**方式1：接受远程代码**

放弃本地修改，完全使用远程版本：

```bash
# 1. 接受远程版本
git checkout --theirs <冲突文件>

# 2. 添加到暂存区
git add <冲突文件>

# 示例：
git checkout --theirs ci_config/rule.yaml
git add ci_config/rule.yaml
```

**适用场景：**
- 远程修改是正确的，本地修改已过时
- 不确定哪个版本正确，选择信任团队代码
- 快速解决冲突，接受权威版本

---

**方式2：接受本地代码**

保留本地修改，覆盖远程修改：

```bash
# 1. 接受本地版本
git checkout --ours <冲突文件>

# 2. 添加到暂存区
git add <冲突文件>

# 示例：
git checkout --ours ci_config/rule.yaml
git add ci_config/rule.yaml
```

**适用场景：**
- 本地修改是正确的，远程修改是错误的
- 本地修改是紧急修复，必须保留
- 确认远程修改可以被覆盖

---

**方式3：手动合并（推荐）**

编辑文件，选择需要保留的内容：

```bash
# 1. 打开冲突文件
vi ci_config/rule.yaml

# 2. 查找冲突标记
<<<<<<< Updated upstream
# 远程代码
version: cann_new
enabled: true
=======
# 本地代码
version: cann
enabled: false
>>>>>>> Stashed changes

# 3. 删除冲突标记，保留需要的内容
# 例如：
version: cann_new
enabled: true

# 4. 保存文件

# 5. 添加到暂存区
git add ci_config/rule.yaml
```

**适用场景：**
- 需要保留远程和本地两部分的有用修改
- 需要仔细审查每一处冲突
- 复杂的合并场景

**手动合并注意事项：**
- 完全删除所有冲突标记（<<<<<<<、=======、>>>>>>>）
- 确保合并后的代码语法正确
- 测试验证功能正常
- 可以与团队成员沟通确认

**步骤3：验证冲突已解决**

```bash
# 检查冲突文件状态
git status

# 应该看到：
# both modified: <冲突文件>
# 或
# modified: <冲突文件>
# 不应该再看到 "Unmerged files"
```

**步骤4：标记冲突已解决**

```bash
# 将解决后的文件添加到暂存区
git add <冲突文件>

# 确认所有冲突已解决
git status

# 应该看不到任何冲突标记
```

**步骤5：继续流程**

```bash
# 告诉系统继续执行
# 输入："继续" 或 "resume"
```

系统会自动验证冲突已解决，并继续执行剩余的上库流程。

#### 4.3.4 Hook 辅助脚本：check_conflict.py

**冲突检测脚本位置**：`.claude/skills/git-code-process/scripts/check_conflict.py`

该脚本用于检测工作目录中的 Git 冲突，提供详细的冲突信息和处理建议。

##### 基本用法

```bash
# 检测是否存在冲突
python .claude/skills/git-code-process/scripts/check_conflict.py

# 检测并显示详细信息（包括原始冲突标记）
python .claude/skills/git-code-process/scripts/check_conflict.py --verbose

# 安静模式（仅返回退出码，不输出信息）
python .claude/skills/git-code-process/scripts/check_conflict.py --quiet
```

##### 命令行参数

| 参数 | 说明 |
|------|------|
| 无参数 | 默认模式，显示冲突检测报告 |
| `--verbose, -v` | 详细模式，显示原始冲突标记内容 |
| `--quiet, -q` | 安静模式，仅返回退出码（0=无冲突，1=有冲突） |
| `--list, -l` | 列出原始冲突标记（同 --verbose） |

##### 退出码

- `0`：无冲突，可以继续操作
- `1`：存在冲突，需要先解决

##### 输出示例

**无冲突时：**

```bash
$ python .claude/skills/git-code-process/scripts/check_conflict.py

============================================================
Git 冲突检测报告
============================================================

✓ 未检测到冲突
工作目录干净，可以继续操作。
```

**有冲突时（默认模式）：**

```bash
$ python .claude/skills/git-code-process/scripts/check_conflict.py

============================================================
Git 冲突检测报告
============================================================

✗ 检测到冲突！

冲突文件数：2

【类型1】已删除冲突（DD/DU/UD）：
  - ci_config/custom/config.yaml

  建议：
  - 查看远程和本地差异
  - 与团队成员沟通确认

【类型2】内容冲突（UU/AA/U）：
  - ci_config/rule.yaml

  建议：
  - 手动编辑文件解决冲突
  - 删除冲突标记（<<<<<<<, =======, >>>>>>>）

============================================================
冲突解决指导：
============================================================

1. 打开冲突文件，查找冲突标记
2. 根据需要保留远程代码、本地代码或手动合并
3. 删除所有冲突标记（<<<<<<<, =======, >>>>>>>）
4. 保存文件
5. 执行：git add <冲突文件>
6. 再次运行本脚本验证冲突已解决
```

**有冲突时（详细模式）：**

```bash
$ python .claude/skills/git-code-process/scripts/check_conflict.py --verbose

============================================================
Git 冲突检测报告
============================================================

✗ 检测到冲突！

...（省略基本信息）...

============================================================
冲突标记详情：
============================================================

冲突文件：ci_config/rule.yaml
------------------------------------------------------------
<<<<<<< Updated upstream
version: cann_new
enabled: true
=======
version: cann
enabled: false
>>>>>>> Stashed changes
------------------------------------------------------------
```

##### 在脚本中使用

```bash
#!/bin/bash

# 拉取远程代码
git pull

# 检测冲突
python .claude/skills/git-code-process/scripts/check_conflict.py
if [ $? -ne 0 ]; then
    echo "请先解决冲突后再继续"
    exit 1
fi

# 继续后续流程
git stash pop
```

#### 4.3.5 冲突处理最佳实践

**1. 备份优先**
```bash
# 在解决冲突前先备份
cp <冲突文件> <冲突文件>.conflict_backup
```

**2. 逐个文件处理**
- 不要一次性解决所有冲突
- 逐个文件处理，验证后继续下一个

**3. 理解冲突原因**
- 查看冲突内容，理解远程和本地的差异
- 询问团队成员相关修改的背景

**4. 测试验证**
- 冲突解决后，先在本地测试
- 确保功能正常后再提交

**5. 保留记录**
- 记录冲突原因和解决方案
- 便于后续查阅和团队分享

#### 4.3.6 冲突预防建议

虽然本技能提供了完整的冲突处理机制，但预防冲突更为重要：

**1. 及时同步**
```bash
# 定期执行 git pull
# 不要长时间不更新代码
git pull
```

**2. 小步快跑**
```bash
# 频繁提交小改动
# 避免积累大量本地修改
git add .
git commit -m "xxx"
```

**3. 沟通协调**
- 修改公共文件前先沟通
- 使用分支隔离开发
- 定期同步代码

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
- 脚本位置：`.claude/skills/git-code-process/scripts/`
- 文档位置：`.claude/skills/git-code-process/SKILL.md`
- 更新日期：2025-02-06

### 脚本文件

| 脚本名称 | 功能说明 | 用途 |
|---------|---------|------|
| **check_conflict.py** | Git 冲突检测工具 | 检测工作目录中的冲突文件，提供处理建议 |

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

# 3. 检测冲突（hooks 自动触发）
python .claude/skills/git-code-process/scripts/check_conflict.py

# 4. 恢复本地修改
git stash pop

# 5. 如果有冲突，按照提示解决
# 6. 再次检测冲突
python .claude/skills/git-code-process/scripts/check_conflict.py
```

### Q8：如何使用 hooks 机制处理冲突？

**回答：**

hooks 机制是本技能的核心特性，用于在冲突发生时提供人工干预流程。

**完整流程：**
```bash
# 1. 检测冲突
python .claude/skills/git-code-process/scripts/check_conflict.py

# 2. 如果有冲突，会显示冲突文件列表
# ✗ 检测到冲突文件：
#   - ci_config/rule.yaml

# 3. 查看冲突内容
python .claude/skills/git-code-process/scripts/check_conflict.py --verbose

# 4. 手动解决冲突
# - 打开冲突文件
# - 删除冲突标记
# - 保存文件

# 5. 标记冲突已解决
git add ci_config/rule.yaml

# 6. 再次检测确认
python .claude/skills/git-code-process/scripts/check_conflict.py

# 7. 确认无冲突后，继续流程
```

**三种解决冲突的方式：**

```bash
# 方式1：接受远程代码（放弃本地修改）
git checkout --theirs ci_config/rule.yaml

# 方式2：接受本地代码（保留本地修改）
git checkout --ours ci_config/rule.yaml

# 方式3：手动合并（推荐）
# - 手动编辑文件，选择需要保留的内容
# - 删除冲突标记：<<<<<<<, =======, >>>>>>>
# - 保存文件
```

### Q9：如何知道冲突已经完全解决？

**验证方法：**
```bash
# 方法1：使用冲突检测脚本
python .claude/skills/git-code-process/scripts/check_conflict.py
# 如果输出 "未检测到冲突"，说明冲突已解决

# 方法2：检查 git status
git status
# 应该看不到 "Unmerged files" 或 "both modified"

# 方法3：尝试添加到暂存区
git add .
# 如果没有错误提示，说明所有冲突已解决
```

### Q10：冲突解决后如何继续上库流程？

**回答：**

冲突解决并验证后，可以继续执行上库流程：

```bash
# 1. 确认冲突已解决
python .claude/skills/git-code-process/scripts/check_conflict.py

# 2. 继续添加到暂存区
git add .

# 3. 继续本地提交
git commit -m "DTS: AR20250113161172" -m "Description: 解决合并冲突" -m "Team: HISI_EE" -m "Feature or Bugfix: Feature"

# 4. 继续上传
repo upload .
```

**注意：**
- 提交信息中应注明"解决合并冲突"
- 如果冲突复杂，建议先在本地测试验证
- 确保功能正常后再上传

### Q11：如何清理未跟踪的文件？

```bash
# 查看未跟踪的文件（不删除，仅预览）
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
