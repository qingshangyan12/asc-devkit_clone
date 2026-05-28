---
name: gitcode-pr
description: |
  使用 GitCode API 创建 Pull Request、推送代码、维护 PR 描述、关联 issue、获取 PR 改动、读取 PR 评论/讨论和删除 PR 评论的 asc-devkit 技能。必须在用户提到以下场景时使用：创建PR、提个PR、发PR、做个PR、帮我PR、生成PR、需要PR、pull request、merge request；push代码、推代码、把代码推上去、提交到远程、推送到gitcode、提交代码到GitCode；合并请求、代码合入请求、请求合并；PR模板、PR描述、PR格式；关issue的PR、关联issue创建PR；查看PR变更、PR文件列表、PR改了什么、看PRdiff、获取PR文件；查看PR评论、PR评论、获取评论、read comments；PR discussions、查看讨论、discussions；删除评论、删除PR评论、移除评论、delete comment、移除这条评论。
---

# GitCode PR Skill

## 概述

本 skill 用于在 `asc-devkit` 仓内处理 GitCode PR 全流程：提交并推送代码到当前开发者自己的远程仓库、向主仓 `cann/asc-devkit` 创建 Pull Request、生成 PR 描述、关联 issue、查询 PR 改动、读取评论/讨论以及删除评论。

核心规则：不要硬编码个人仓库地址或用户名。个人远程仓库必须从 `git remote -v`、fork parent 信息或用户明确输入动态确认。

## 前置条件

- 访问令牌优先读取 `GITCODE_API_TOKEN`，兼容读取 `GITCODE_TOKEN`。
- 如果令牌不存在，向用户索要 GitCode Access Token；不要把令牌写入仓库文件或输出到日志。
- 令牌通常需要 `read_repository`、`write_repository`、`read_api` 权限。
- 创建 PR 前先完成与本次变更匹配的验证；未验证时在 PR 描述和最终报告中明确说明原因。
- 所有查询类命令默认目标仓为 `cann/asc-devkit`，除非用户给出其他 GitCode PR 链接或仓库路径。

## 远程仓库识别

先执行：

```bash
git remote -v
git status --short
git branch --show-current
```

识别规则：

- 目标主仓默认为 `cann/asc-devkit`。优先使用名为 `upstream` 且指向 `gitcode.com/cann/asc-devkit` 的远程。
- 个人远程通常是 `origin`，但不能假设。选择指向 GitCode、仓库名为 `asc-devkit`、owner 不是 `cann` 的可写远程。
- 如果只有一个 GitCode 远程且它是 fork，可用 GitCode API 查询 `.parent.full_name`，将 parent 作为目标主仓。
- 如果个人远程或目标主仓无法唯一确定，停止并向用户确认，不要猜测。
- PR `head` 使用 `个人owner:分支名`，不是远程名。例如远程 `git@gitcode.com:alice/asc-devkit.git` 的分支 `fix/a` 对应 `head=alice:fix/a`。

支持解析的远程 URL 格式见 [references/gitcode_api.md](references/gitcode_api.md)。

## 任务路由

| 用户意图 | 执行入口 |
|---------|----------|
| 创建/提交 PR、提个 PR、发 PR、做个 PR、生成合并请求 | 走“创建 PR 流程” |
| push 代码、推代码、提交到远程、推送到 GitCode | 只执行到推送步骤；除非用户也要求创建 PR |
| PR 模板、PR 描述、PR 格式 | 使用 [assets/pr_template.md](assets/pr_template.md) 生成或修改描述 |
| 关联 issue 创建 PR、关 issue 的 PR | 在 PR 描述中填写 `Closes #<issue>`，必要时标题保留 issue 编号 |
| 查看 PR 变更、PR 文件列表、PR diff、获取 PR 文件 | 使用 `scripts/pr_api.py changes` |
| 查看 PR 评论、获取评论、read comments | 使用 `scripts/pr_api.py comments` 和必要时 `scripts/pr_api.py discussions` |
| 查看 PR 讨论、PR discussions、discussions | 使用 `scripts/pr_api.py discussions` |
| 删除评论、删除 PR 评论、delete comment | 先确认评论 ID，再使用 `scripts/pr_api.py delete-comment` |

如果用户粘贴 PR 链接，先解析出目标仓和 PR 编号。GitCode PR 链接通常是：

```text
https://gitcode.com/<owner>/<repo>/pull/<PR_NUMBER>
```

## 创建 PR 流程

1. 检查工作树。只纳入本次任务相关文件；遇到不相关未提交变更时先说明并保持不动。
2. 确认目标分支。优先查询主仓默认分支：

```bash
git ls-remote --symref <target-remote> HEAD
```

若无法查询，`asc-devkit` 主仓默认使用 `master`，除非用户指定其他 base。

3. 确保当前分支适合作为 PR 源分支。不要在 detached HEAD 上推送；不要使用 `git reset --hard` 或强推。
4. 如需提交，按最小范围暂存并提交：

```bash
git add <intended-files>
git commit -m "<type>: <summary>"
```

如果用户已有合适提交，不要 amend，除非用户明确要求。

5. 推送到个人远程仓库：

```bash
git push -u <personal-remote> HEAD:<branch-name>
```

6. 根据 [assets/pr_template.md](assets/pr_template.md) 生成 PR 描述，填写变更说明、测试结果、Issue 关联和风险。
7. 使用脚本创建 PR：

```bash
python3 .agent/skills/gitcode-pr/scripts/create_pr.py \
  --target cann/asc-devkit \
  --head "<personal-owner>:<branch-name>" \
  --base "<base-branch>" \
  --title "<type>: <summary>" \
  --body-file "<filled-pr-body.md>"
```

脚本会读取 `GITCODE_API_TOKEN` 或 `GITCODE_TOKEN`。需要预览请求但不调用 API 时，加 `--dry-run`。

如果用户给出 issue，可直接渲染模板：

```bash
python3 .agent/skills/gitcode-pr/scripts/create_pr.py \
  --target cann/asc-devkit \
  --head "<personal-owner>:<branch-name>" \
  --base "<base-branch>" \
  --title "<type>: <summary>" \
  --description "<summary for template>" \
  --issue "#<issue-number>"
```

## 查询 PR 改动

使用 GitLab API v4 风格读取 merge request 变更，项目路径需要 URL 编码为 `cann%2Fasc-devkit`：

```bash
python3 .agent/skills/gitcode-pr/scripts/pr_api.py changes \
  --target cann/asc-devkit \
  --pr <PR_NUMBER>
```

如果用户提供 PR 链接：

```bash
python3 .agent/skills/gitcode-pr/scripts/pr_api.py changes \
  --pr-url "https://gitcode.com/cann/asc-devkit/pull/<PR_NUMBER>"
```

输出中重点关注：

| 字段 | 说明 |
|------|------|
| `changes[].new_path` | 新文件路径 |
| `changes[].old_path` | 旧文件路径 |
| `changes[].diff` | 文件 diff |
| `diff_refs` | 行内评论需要的 base/head SHA 信息 |

若用户只要文件列表，读取 `changes[].new_path` 和 `changes[].old_path` 后汇总即可；不要把完整大 diff 全量贴给用户，优先摘要。

## 获取评论和讨论

普通评论：

```bash
python3 .agent/skills/gitcode-pr/scripts/pr_api.py comments \
  --target cann/asc-devkit \
  --pr <PR_NUMBER>
```

讨论和行内评论：

```bash
python3 .agent/skills/gitcode-pr/scripts/pr_api.py discussions \
  --target cann/asc-devkit \
  --pr <PR_NUMBER>
```

评论摘要输出格式：

```markdown
# PR 评论摘要

## 总体统计
- 讨论数量: X
- 行内评论: Y
- 已解决评论: Z

## 详情
- 文件: path/to/file.cpp
- 行号: N
- 评论者: @username
- 状态: 未解决/已解决
- 内容: ...
```

行内评论常见字段：

| 字段 | 说明 |
|------|------|
| `notes[].type` | `DiffNote` 表示行内评论，`DiscussionNote` 表示普通讨论 |
| `notes[].body` | 评论内容 |
| `notes[].author.username` | 评论作者 |
| `notes[].position.new_path` | 评论文件 |
| `notes[].position.new_line` | 新代码行号 |
| `notes[].resolved` | 是否已解决 |

## 删除 PR 评论

删除前必须先确认目标评论 ID 和评论内容，避免误删。只能删除自己创建的评论，或在有仓库管理权限时删除他人评论。

查询评论 ID：

```bash
python3 .agent/skills/gitcode-pr/scripts/pr_api.py discussions \
  --target cann/asc-devkit \
  --pr <PR_NUMBER>

python3 .agent/skills/gitcode-pr/scripts/pr_api.py comments \
  --target cann/asc-devkit \
  --pr <PR_NUMBER>
```

删除评论：

```bash
python3 .agent/skills/gitcode-pr/scripts/pr_api.py delete-comment \
  --target cann/asc-devkit \
  --comment-id <COMMENT_ID>
```

删除操作不可恢复；如果用户没有明确评论 ID，应先列出候选评论并要求用户确认。

## PR 标题

标题优先使用 Conventional Commits 风格：

```text
<type>: <summary>
```

常用类型：

| 类型 | 说明 |
|------|------|
| `feat` | 新功能或新 API |
| `fix` | Bug 修复 |
| `docs` | 文档更新 |
| `test` | 测试新增或修复 |
| `refactor` | 无行为变化的重构 |
| `build` | 构建系统或依赖变更 |
| `chore` | 维护性变更 |

如果关联 issue，可在描述中使用 `Closes #<issue>`；标题是否带 issue 编号遵循当前 PR 习惯或用户要求。

## 错误处理

- API 返回认证失败时，只提示令牌缺失或权限不足，不回显令牌。
- 远程分支已存在且内容冲突时，先 `git fetch <personal-remote>` 和比较差异；不要自动强推。
- PR 创建失败时，输出状态码、GitCode 返回的错误摘要、目标仓、head、base 和本地分支，方便用户修正。
- 查询 PR 失败时，输出目标仓、PR 编号、API 风格和状态码；不要把失败伪装为空评论或空变更。
- 删除评论失败时，输出状态码和错误摘要；不要重复删除或尝试其他评论 ID。
- 如果 GitCode API 不可用，可报告已完成 push，并给出手工创建 PR 所需字段：目标仓、head、base、标题和描述文件路径。

## 资源

- [scripts/create_pr.py](scripts/create_pr.py): 使用 GitCode API 创建 PR 的参数化脚本。
- [scripts/pr_api.py](scripts/pr_api.py): 查询 PR 信息、变更、评论、讨论和删除评论的参数化脚本。
- [assets/pr_template.md](assets/pr_template.md): asc-devkit PR 描述模板。
- [references/gitcode_api.md](references/gitcode_api.md): GitCode PR API 和远程 URL 解析参考。
