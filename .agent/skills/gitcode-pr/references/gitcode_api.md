# GitCode PR API 参考

## 认证方式

GitCode PR 相关接口同时使用两种 API 风格：

| API 风格 | 认证头 | 端点格式 | 用途 |
|---------|--------|----------|------|
| GitCode API v5 | `Authorization: Bearer <token>` | `/api/v5/repos/<owner>/<repo>/...` | 创建 PR、查询 fork、删除评论 |
| GitLab API v4 | `PRIVATE-TOKEN: <token>` | `/api/v4/projects/<encoded_path>/...` | 查询 PR 信息、变更、评论、讨论 |

令牌从 `GITCODE_API_TOKEN` 读取，兼容 `GITCODE_TOKEN`。不要在命令输出、PR 描述或仓库文件中写入令牌。

项目路径编码示例：`cann/asc-devkit` -> `cann%2Fasc-devkit`。

## 创建 PR

创建 PR 使用 API v5：

```bash
POST https://gitcode.com/api/v5/repos/<owner>/<repo>/pulls
Authorization: Bearer <token>
Content-Type: application/json
```

请求体字段：

| 字段 | 必需 | 说明 |
|------|------|------|
| `title` | 是 | PR 标题 |
| `head` | 是 | 源分支，格式为 `<personal-owner>:<branch>` |
| `base` | 是 | 目标分支，通常是主仓默认分支 |
| `body` | 否 | Markdown PR 描述 |

示例：

```json
{
  "title": "fix: correct vector example",
  "head": "alice:fix/vector-example",
  "base": "master",
  "body": "PR description"
}
```

成功响应通常包含 `number`、`html_url`、`web_url` 或 `state`。PR 网页地址格式通常为：

```text
https://gitcode.com/<owner>/<repo>/pull/<number>
```

## 获取 PR 信息

```bash
GET https://api.gitcode.com/api/v4/projects/cann%2Fasc-devkit/merge_requests/<PR_NUMBER>
PRIVATE-TOKEN: <token>
```

常用字段：

| 字段 | 说明 |
|------|------|
| `iid` | PR 编号 |
| `title` | PR 标题 |
| `state` | PR 状态 |
| `source_branch` | 源分支 |
| `target_branch` | 目标分支 |
| `diff_refs` | diff 基准 SHA，用于行内评论定位 |

## PR URL 解析

支持从以下链接解析仓库和 PR 编号：

```text
https://gitcode.com/<owner>/<repo>/pull/<PR_NUMBER>
https://gitcode.com/<owner>/<repo>/pulls/<PR_NUMBER>
```

解析后：

- `<owner>/<repo>` 作为 API 目标项目路径。
- `<PR_NUMBER>` 作为 GitLab API v4 的 `merge_requests/<PR_NUMBER>`。

## 获取 PR 文件变更

```bash
GET https://api.gitcode.com/api/v4/projects/cann%2Fasc-devkit/merge_requests/<PR_NUMBER>/changes?per_page=100
PRIVATE-TOKEN: <token>
```

常用字段：

| 字段 | 说明 |
|------|------|
| `changes[].old_path` | 旧文件路径 |
| `changes[].new_path` | 新文件路径 |
| `changes[].new_file` | 是否新增文件 |
| `changes[].renamed_file` | 是否重命名 |
| `changes[].deleted_file` | 是否删除 |
| `changes[].diff` | 文件 diff |

## 获取 PR 评论

普通评论：

```bash
GET https://api.gitcode.com/api/v4/projects/cann%2Fasc-devkit/merge_requests/<PR_NUMBER>/notes
PRIVATE-TOKEN: <token>
```

讨论和行内评论：

```bash
GET https://api.gitcode.com/api/v4/projects/cann%2Fasc-devkit/merge_requests/<PR_NUMBER>/discussions
PRIVATE-TOKEN: <token>
```

讨论返回字段：

| 字段 | 说明 |
|------|------|
| `notes[].id` | 评论 ID，删除评论时使用 |
| `notes[].type` | `DiffNote` 表示行内评论 |
| `notes[].body` | 评论内容 |
| `notes[].author.username` | 评论作者 |
| `notes[].position.new_path` | 评论文件 |
| `notes[].position.new_line` | 新代码行号 |
| `notes[].content` | 被评论的代码行内容 |
| `notes[].resolved` | 是否已解决 |
| `notes[].created_at` | 评论创建时间 |

## 删除 PR 评论

删除评论使用 API v5：

```bash
DELETE https://api.gitcode.com/api/v5/repos/cann/asc-devkit/pulls/comments/<COMMENT_ID>?access_token=<token>
```

响应码：

| 状态码 | 说明 |
|--------|------|
| `200` | 删除成功，通常返回 `{}` |
| `401` | 未授权 |
| `403` | 无权限删除此评论 |
| `404` | 评论不存在 |

权限限制：

- 只能删除自己创建的评论。
- 仓库管理员可以删除其他评论。
- 删除操作不可恢复。

## 查询 Fork Parent

当只有个人 fork 远程而没有明确主仓远程时，可查询仓库信息：

```bash
curl -s -H "Authorization: Bearer $GITCODE_API_TOKEN" \
  "https://api.gitcode.com/api/v5/repos/<owner>/<repo>"
```

关键字段：

| 字段 | 说明 |
|------|------|
| `fork` | 是否为 fork |
| `parent.full_name` | fork 的上游仓库，如 `cann/asc-devkit` |
| `parent.default_branch` | 上游默认分支 |
| `default_branch` | 当前仓默认分支 |

仅当 `.fork == true` 且 `.parent.full_name` 存在时，才能把 parent 作为 PR 目标仓。

## 远程 URL 解析

支持这些 GitCode URL 形式：

```text
https://gitcode.com/<owner>/<repo>
https://gitcode.com/<owner>/<repo>.git
git@gitcode.com:<owner>/<repo>.git
ssh://git@gitcode.com/<owner>/<repo>.git
```

解析规则：

- `<owner>` 是 PR `head` 中冒号前的值。
- `<repo>` 应为 `asc-devkit`，除非用户明确在其他 fork 名下工作且 GitCode 支持跨仓 PR。
- 主仓 owner 为 `cann`，个人远程 owner 不应硬编码，应从实际 URL 提取。

## 常见失败

| 现象 | 处理 |
|------|------|
| 401 或 403 | 检查令牌是否存在、权限是否包含仓库读写和 API 访问 |
| head not found | 确认已经 push 到个人远程，且 `head` 使用 `<owner>:<branch>` |
| base not found | 确认目标主仓分支名，优先使用主仓默认分支 |
| no commits between base and head | 确认源分支包含相对于 base 的新提交 |
| 404 comment not found | 删除评论前重新查询评论 ID |
| empty changes unexpectedly | 先查询 PR 信息确认编号和目标仓是否正确，不要直接报告无变更 |
