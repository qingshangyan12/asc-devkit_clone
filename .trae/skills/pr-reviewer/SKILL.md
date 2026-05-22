---
name: "PR信息获取"
description: "获取 GitCode PR 的变更文件清单、Diff 内容和检视意见，并能将检视意见映射到对应的文件内容上。当用户给出 GitCode PR 链接或 PR 编号时调用此技能。"
---

# PR 信息获取技能

## 技能用途

获取 GitCode 平台上 Pull Request 的详细信息，包括：
1. PR 基本信息（标题、作者、状态、合并信息等）
2. 变更文件清单（区分修改/新增/删除，输出**完整路径**）
3. 每个文件的 Diff 内容（代码变更详情）
4. 检视意见（评论）及其**对应的文件内容和代码片段**

## 使用方式

用户提供 GitCode PR 链接或 PR 编号，例如：
- `https://gitcode.com/cann/asc-devkit/pull/2099`
- 或直接说 `PR #2099`

默认仓库为 `cann/asc-devkit`，如用户未指定仓库则使用此默认值。

## API 调用方法

使用 PowerShell 的 `Invoke-RestMethod` 通过 GitCode REST API 获取数据。

### 公共参数

```powershell
$headers = @{Authorization = "Bearer mHJpvzbD8x4RYxM9uT7S-icz"}
$baseUrl = "https://api.gitcode.com/api/v5/repos/cann/asc-devkit/pulls"
```

### API 端点

| 用途 | 端点 | 说明 |
|------|------|------|
| PR 基本信息 | `GET /pulls/{number}` | 标题、作者、状态、增减行数等 |
| 变更文件清单 + Diff | `GET /pulls/{number}/files?per_page=100` | 每个文件的 `filename`、`new_file`、`deleted_file`、`patch.diff` 等 |
| 检视意见 | `GET /pulls/{number}/comments?per_page=100` | 评论内容、作者、评论的文件路径、起始行号等 |

> **注意：** 必须加 `?per_page=100` 参数，否则 API 默认只返回前 20 条，可能导致数据不完整。

### 关键字段说明

**PR 信息 API 返回的关键字段：**

| 字段 | 说明 |
|------|------|
| `title` | PR 标题（Latin-1 编码，需要转 UTF-8） |
| `user.login` | PR 作者 |
| `state` | 状态（open/merged/closed） |
| `created_at` | 创建时间 |
| `merged_at` | 合并时间（未合并则为 null） |
| `additions` | 总新增行数 |
| `deletions` | 总删除行数 |
| `description` | PR 描述 |

**Files API 返回的关键字段：**

| 字段 | 说明 |
|------|------|
| `filename` | 文件路径（完整路径） |
| `new_file` | 是否为新增文件（true/false） |
| `deleted_file` | 是否为删除文件（true/false） |
| `renamed_file` | 是否为重命名文件（true/false） |
| `additions` | 新增行数 |
| `deletions` | 删除行数 |
| `patch.diff` | 该文件的 Diff 内容（含 @@ 行号信息，Latin-1 编码） |

**Comments API 返回的关键字段：**

| 字段 | 说明 |
|------|------|
| `body` | 评论正文（Latin-1 编码） |
| `user.login` | 评论作者 |
| `path` | 评论对应的文件路径（为空则为全局评论） |
| `line` | 评论针对的行号（为空则为全局评论） |
| `position` | 评论在 diff 中的位置 |
| `created_at` | 创建时间 |
| `commit_id` | 评论对应的 commit SHA |

## 输出格式

### 1. PR 基本信息

```
PR #{number} - {title}

| 项目 | 内容 |
|------|------|
| 作者 | {user} |
| 状态 | {state}（合并于 {merged_at} / 未合并） |
| 创建时间 | {created_at} |
| 总变更 | +{additions}/-{deletions} |
```

### 2. 变更文件清单

按类型列出（先列修改、再列新增、再列删除），**必须输出完整路径**，格式：

```
### 📝 修改的文件（共 N 个）

| # | 完整文件路径 |
|---|-------------|
| 1 | `examples/.../.../file1.h` |
| 2 | `examples/.../.../file2.h` |

### ➕ 新增的文件（共 N 个）

| # | 完整文件路径 |
|---|-------------|
| 1 | `examples/.../file_new.h` |

### 🗑️ 删除的文件（共 N 个）

| # | 完整文件路径 |
|---|-------------|
| 1 | `examples/.../file_deleted.h` |
```

### 3. 检视意见（含代码映射）

列出每条人工检视意见，**必须有对应的代码片段**。格式：

```
---

### 检视意见 #{n}

- **作者**：{user}
- **文件**：`{path}`（如为全局评论则推断对应文件）
- **行号**：第 {line} 行（如为全局评论则标注"全局评论"）
- **意见内容**：{body}

**对应的 Diff 代码片段：**

```diff
{与意见相关的代码变更片段}
```
```

### 4. 检视意见与代码内容的映射方法

通过 comments 中的 `path` 和 `line` 字段，与 files 中的 `patch.diff` 内容做匹配：

**情况 A：评论有 path 和 line 字段（行内评论）**
1. 从 comments 获取 `path`（文件路径）和 `line`（行号）
2. 在对应文件的 `patch.diff` 中解析 `@@` 行号标记，定位到评论行附近的代码
3. 提取该代码片段作为"对应的代码片段"

**情况 B：评论的 path 和 line 为空（全局评论）**
1. 根据评论内容的关键词，**推断**对应的文件
2. 从该文件的 `patch.diff` 中提取相关的代码片段
3. 在输出中标注"全局评论"，并附上推断的文件和代码片段

### 5. 编码处理

API 返回的中文内容使用 Latin-1（ISO-8859-1）编码，需要在 PowerShell 中做转码：

```powershell
$bytes = [System.Text.Encoding]::GetEncoding("ISO-8859-1").GetBytes($text)
$decoded = [System.Text.Encoding]::UTF8.GetString($bytes)
```

需要转码的字段包括：
- PR 信息的 `title`、`description`
- 评论的 `body`（正文）
- Diff 中的 `patch.diff`（注释等中文内容）

## 执行步骤

1. **获取 PR 编号**：从用户输入中提取 PR 编号（支持 URL 或纯数字）
2. **调用 PR 信息 API**：获取基本信息和增减行数（含编码转换）
3. **调用 Files API**：获取变更文件清单和每个文件的 Diff 内容（含编码转换）
4. **调用 Comments API**：获取所有检视意见（含编码转换），**必须加 `?per_page=100`**
5. **过滤评论**：
   - 过滤掉 `cann-robot` 的 CI 自动检查评论
   - 过滤掉作者的 `compile` 触发流水线命令
   - 过滤掉 `/lgtm`、`/approve` 等操作命令
   - 只保留人工评审意见
6. **映射关联**：将每条人工评审意见的 `path` + `line` 与对应文件的 Diff 内容做匹配，提取相关的代码片段。如为全局评论则根据内容推断对应文件
7. **输出报告**：按上述格式输出完整的 PR 分析报告

## PowerShell 调用示例

```powershell
$headers = @{Authorization = "Bearer mHJpvzbD8x4RYxM9uT7S-icz"}
$prNumber = 2099

# 获取 PR 信息（必须加 per_page=100）
$prInfo = Invoke-RestMethod -Uri "https://api.gitcode.com/api/v5/repos/cann/asc-devkit/pulls/$prNumber" -Headers $headers -TimeoutSec 60

# 获取变更文件
$files = Invoke-RestMethod -Uri "https://api.gitcode.com/api/v5/repos/cann/asc-devkit/pulls/$prNumber/files?per_page=100" -Headers $headers -TimeoutSec 60

# 获取检视意见
$comments = Invoke-RestMethod -Uri "https://api.gitcode.com/api/v5/repos/cann/asc-devkit/pulls/$prNumber/comments?per_page=100" -Headers $headers -TimeoutSec 60

# 中文解码函数
function Decode-CN($text) {
    $bytes = [System.Text.Encoding]::GetEncoding("ISO-8859-1").GetBytes($text)
    return [System.Text.Encoding]::UTF8.GetString($bytes)
}
```

## 已知限制

- Comments API 返回的评论可能包含 CI 机器人（如 cann-robot）的自动检查结果，需要过滤掉非人工检视意见
- 评论的 `line` 字段可能为空，此时评论为 PR 全局评论，需要根据内容推断对应文件
- Diff 内容中的行号是通过 `@@` 标记解析的，可能有多段不连续的代码行
- API Token 已嵌入脚本中，无需用户额外提供
- API 返回的中文内容为 Latin-1 编码，需要额外的解码步骤
- GitCode 不支持 `/pulls/:number/reviews` 端点（返回 404）
- GitCode 不支持 `/issues/:number/comments` 端点获取 PR 评论（返回 400）
- PR 合并后无法再提交评论（返回 400）
