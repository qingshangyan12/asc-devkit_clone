# Claude Code Hooks 配置

## 概述

本目录包含 Claude Code Agent 在执行 git-code-process 技能时的自动钩子脚本。这些钩子在关键的 Git 操作点自动触发，用于检测冲突、验证状态和提供操作指导。

## 钩子列表

| 钩子名称 | 触发时机 | 功能 | 是否暂停 |
|---------|---------|------|---------|
| **pre-commit** | git commit 前 | 检查暂存区和冲突状态 | 否 |
| **post-commit** | git commit 后 | 验证提交成功，显示提交信息 | 否 |
| **post-pull** | git pull 后 | **检测冲突** | **是** |
| **post-stash-pop** | git stash pop 后 | **检测冲突** | **是** |
| **pre-upload** | repo upload 前 | 检查待上传提交和工作目录状态 | 否 |
| **post-upload** | repo upload 后 | 验证上传成功，显示后续步骤 | 否 |

## 钩子详细说明

### 1. pre-commit

**文件**: `pre-commit.bash`

**功能**:
- 检查暂存区是否为空
- 检测工作目录是否存在冲突
- 防止提交带有冲突的文件

**检查项**:
```bash
✓ 暂存区不为空
✓ 工作目录无冲突
```

**退出码**:
- `0`: 检查通过，可以提交
- `1`: 检查失败，需要先处理问题

---

### 2. post-commit

**文件**: `post-commit.bash`

**功能**:
- 验证提交成功
- 显示提交信息（Commit ID 和提交消息）
- 提示未推送的提交数量

**输出示例**:
```
============================================================
[HOOK] post-commit 触发
============================================================

[√] 提交成功

提交信息：
  Commit ID: a1b2c3d
  DTS: AR20250113161172
  Description: 添加新的编译任务
  Team: HISI_EE
  Feature or Bugfix: Feature

提示：有 1 个提交未上传到远程仓
下一步：执行 repo upload . 上传到 CodeHub
```

---

### 3. post-pull ⚠️

**文件**: `post-pull.bash`

**功能**: **检测 git pull 后是否产生冲突**

**检查项**:
```bash
✓ 调用 check_conflict.py 检测冲突
✓ 如有冲突，显示冲突解决指导
```

**冲突检测**:
- 解析 `git status --porcelain` 输出
- 识别冲突状态码：UU, AA, DD, DU, UD
- 列出所有冲突文件

**冲突处理流程**:
1. 检测到冲突时，显示警告和冲突文件列表
2. 提供三种冲突解决方式：
   - 接受远程代码：`git checkout --theirs <文件>`
   - 接受本地代码：`git checkout --ours <文件>`
   - 手动合并：编辑文件删除冲突标记
3. 返回退出码 1，提示 Agent 暂停流程
4. 等待用户手动解决冲突
5. 用户确认后，Agent 恢复流程

**输出示例（有冲突）**:
```
============================================================
[警告] 检测到代码冲突！
============================================================

冲突文件数：2

【类型1】已删除冲突（DD/DU/UD）：
  - ci_config/custom/config.yaml

【类型2】内容冲突（UU/AA/U）：
  - ci_config/rule.yaml

请按照以下步骤解决冲突：
1. 打开冲突文件，查找冲突标记（<<<<<<<, =======, >>>>>>>）
2. 根据需要保留远程代码、本地代码或手动合并
3. 删除所有冲突标记
4. 保存文件
5. 执行：git add <冲突文件>
```

---

### 4. post-stash-pop ⚠️

**文件**: `post-stash-pop.bash`

**功能**: **检测 git stash pop 后是否产生冲突**

**检查项**: 与 post-pull 相同

**特殊说明**:
- 恢复本地修改时产生的冲突通常更复杂
- 需要仔细对比本地修改和远程代码
- 建议优先保留重要的本地修改

**输出示例（有冲突）**:
```
============================================================
[警告] 恢复本地修改时检测到冲突！
============================================================

这表示本地修改与远程代码产生了冲突。

请按照以下步骤解决冲突：
...（同 post-pull）
```

---

### 5. pre-upload

**文件**: `pre-upload.bash`

**功能**:
- 检查是否有待上传的提交
- 检查工作目录状态
- 显示待上传的提交列表

**检查项**:
```bash
✓ 有新的提交需要上传
✓ 工作目录状态提示
```

**退出码**:
- `0`: 检查通过，可以上传
- `1`: 没有提交需要上传，或用户取消上传

**输出示例**:
```
============================================================
[HOOK] pre-upload 触发
============================================================

[√] 发现 2 个提交待上传

待上传的提交：
  a1b2c3d DTS: AR20250113161172
  d4e5f6g DTS: AR20250113161173

[√] 检查通过，准备上传
```

---

### 6. post-upload

**文件**: `post-upload.bash`

**功能**:
- 验证上传成功
- 提示后续步骤（CodeReview 审核）
- 提供查看状态的命令

**输出示例**:
```
============================================================
[HOOK] post-upload 触发
============================================================

[√] 上传流程完成

后续步骤：
1. 等待 CodeReview 审核
2. 根据审核意见进行修改
3. 审核通过后，代码将被合并到远程仓库

查看上传状态：
  repo info .

查看审核进度：
  访问 CodeHub 平台
```

## 使用方法

### 自动执行

这些钩子由 Claude Code Agent 在执行 git-code-process 技能时自动调用，无需手动执行。

### 手动测试

如需单独测试某个钩子：

```bash
# 测试 post-pull 钩子
bash .claude/hooks/post-pull.bash

# 测试 pre-commit 钩子
bash .claude/hooks/pre-commit.bash

# 测试 pre-upload 钩子
bash .claude/hooks/pre-upload.bash
```

## 钩子工作流程

### 标准上库流程（含钩子）

```
【步骤1：环境检查】
    git status
    git branch -v

【步骤2：暂存本地修改】
    git stash

【步骤3：同步远程代码】
    git pull
    ↓
【Hook: post-pull 触发】
    ↓
    检测冲突？
    ├─ 无冲突 → 继续
    └─ 有冲突 → 【暂停流程，等待用户处理】

【步骤4：恢复本地修改】
    git stash pop
    ↓
【Hook: post-stash-pop 触发】
    ↓
    检测冲突？
    ├─ 无冲突 → 继续
    └─ 有冲突 → 【暂停流程，等待用户处理】

【步骤5：添加到暂存区】
    git add .

【步骤6：本地提交】
    ↓
【Hook: pre-commit 触发】
    ↓
    检查暂存区和冲突状态
    ├─ 通过 → 继续提交
    └─ 失败 → 【停止提交】
    ↓
    git commit
    ↓
【Hook: post-commit 触发】
    ↓
    显示提交信息

【步骤7：上传到 CodeHub】
    ↓
【Hook: pre-upload 触发】
    ↓
    检查待上传提交
    ├─ 通过 → 继续上传
    └─ 失败 → 【停止上传】
    ↓
    repo upload .
    ↓
【Hook: post-upload 触发】
    ↓
    显示后续步骤
```

## 冲突检测机制

### 检测原理

钩子通过调用 `.claude/skills/git-code-process/scripts/check_conflict.py` 脚本检测冲突：

```bash
python .claude/skills/git-code-process/scripts/check_conflict.py
```

该脚本解析 `git status --porcelain` 的输出，识别以下状态码：

| 状态码 | 含义 | 冲突类型 |
|-------|------|---------|
| UU | 双方都修改 | 内容冲突 |
| AA | 双方都添加 | 内容冲突 |
| DD | 双方都删除 | 删除冲突 |
| DU | 远程删除，本地修改 | 删除冲突 |
| UD | 本地删除，远程修改 | 删除冲突 |

### 退出码约定

所有钩子脚本遵循以下退出码约定：

- **0**: 操作成功，可以继续执行下一步
- **1**: 操作失败或检测到冲突，需要暂停流程或停止操作

### 暂停流程机制

当 **post-pull** 或 **post-stash-pop** 钩子检测到冲突时：

1. 钩子脚本返回退出码 1
2. Claude Code Agent 捕获到非零退出码
3. Agent 暂停自动化流程
4. 向用户展示冲突信息和建议
5. 等待用户手动解决冲突
6. 用户确认冲突已解决后
7. Agent 恢复流程，继续执行后续步骤

## 自定义和扩展

### 修改钩子行为

如需修改钩子行为，编辑对应的 `.bash` 文件：

```bash
# 示例：修改 post-pull 钩子
vim .claude/hooks/post-pull.bash
```

### 添加新的钩子

1. 创建新的 `.bash` 文件：
   ```bash
   touch .claude/hooks/pre-pull.bash
   ```

2. 编写钩子逻辑：
   ```bash
   #!/bin/bash
   echo "Hook 触发"
   # 执行检查
   exit 0  # 或 exit 1
   ```

3. 在 git-code-process/SKILL.md 中更新钩子文档

### 禁用某个钩子

临时禁用钩子（不推荐）：
```bash
# 重命名钩子文件，移除 .bash 后缀
mv .claude/hooks/post-pull.bash .claude/hooks/post-pull.bash.disabled
```

## 故障排查

### Q1：钩子没有执行？

**检查**:
```bash
# 确认钩子文件存在
ls -la .claude/hooks/

# 确认钩子有执行权限
chmod +x .claude/hooks/*.bash
```

### Q2：钩子报告错误但实际没有冲突？

**检查**:
```bash
# 手动运行冲突检测脚本
python .claude/skills/git-code-process/scripts/check_conflict.py --verbose

# 查看 git status
git status --porcelain
```

### Q3：如何查看钩子输出？

钩子输出会在 Claude Code 的终端中显示。如需调试：
```bash
# 手动执行钩子脚本
bash .claude/hooks/post-pull.bash
```

## 相关文档

- `git-code-process/SKILL.md` - Git 代码处理技能完整文档
- `git-code-process/scripts/README.md` - Hooks 机制详细说明
- `git-code-process/scripts/check_conflict.py` - 冲突检测脚本

## 维护信息

- 钩子位置：`.claude/hooks/`
- 冲突检测脚本：`.claude/skills/git-code-process/scripts/check_conflict.py`
- 更新日期：2026-02-06
