# issues_auto 脚本使用说明

本文档说明当前目录下 3 个脚本的功能、使用方式、使用约束和执行结果。

最终流程：

1. 长期保存一份账号库：`gitcode_accounts.md`。
2. 每天拿当天 issue Excel 执行 `merge_gitcode_accounts.py`，根据 Excel 中出现的新账号补充并更新 `gitcode_accounts.md`。
3. 再执行 `update_issue_excel_from_md.py`，基于更新后的 `gitcode_accounts.md` 生成一份新的 `_updated.xlsx`。
4. 最后用 `_updated.xlsx` 执行 `send_resource_group_emails.py` 发送邮件。

默认不保留中间文件。需要保留接口原始 JSON、邮件附件和发送报告时，执行前设置环境变量：

```bash
export KEEP_INTERMEDIATE_FILES=1
```

可识别的开启值包括：`1`、`true`、`yes`、`y`、`on`。

## 1. merge_gitcode_accounts.py

### 功能

从 issue Excel 的 `提出者`、`责任人` 两列，以及脚本同级的 `gitcode_accounts.md` 中提取 gitcode 账号，合并去重后按 20 个一组调用固定接口，获取账号详细信息。

脚本会基于接口返回结果就地更新账号信息 Markdown：

```text
gitcode_accounts.md
```

如果设置 `KEEP_INTERMEDIATE_FILES=1`，会额外保留接口原始结果：

```text
merged_gitcode_accounts.json
```

### 使用方式

先在脚本中替换固定接口地址：

```python
FETCH_URL_TEMPLATE = "https://xxxx.com/gitcode={str}&size={size}&page=1"
```

然后执行：

```bash
python3 merge_gitcode_accounts.py issue_20260813_085422.xlsx
```

### 使用约束

- 只接收 1 个命令行参数：issue Excel 文件路径。
- `gitcode_accounts.md` 必须和脚本在同一目录。
- Excel 中只读取 `提出者`、`责任人` 两列。
- Excel 中的中文账号会被忽略；中文账号需要依赖 `gitcode_accounts.md` 中已有的英文 gitcode。
- `gitcode_accounts.md` 必须是新格式账号表，需要包含 `gitcode` 列。
- 接口返回必须是字典，并且包含 `results` 字段，`results` 必须是列表。
- 每次请求最多拼接 `GROUP_SIZE = 20` 个账号。
- 当前 `SSL_VERIFY = False`，用于跳过证书校验。
- 接口字段取值规则：
  - `gitcode`：`gitcode_account`
  - `姓名`：`chinese_name`
  - `邮箱`：`email`
  - `资源组`：优先 `user_ship.projec`，再取 `user_ship.project`，最后取 `unaffiliated_detail.lowest_department`
  - `是否为算子组`：`user_ship.department == "昇腾解决方案开发部"` 时为 `是`，否则为 `否`
  - `PL`：优先 `user_ship.manager_chinese_name`，再取 `user_ship.manager`

### 执行结果

执行成功后会在脚本同级目录更新：

```text
gitcode_accounts.md
```

如果设置 `KEEP_INTERMEDIATE_FILES=1`，还会生成：

```text
merged_gitcode_accounts.json
```

终端会打印：

```text
已更新 gitcode_accounts.md，共 M 个账号
未保留中间文件 merged_gitcode_accounts.json
字段缺失样例: {...}
```

保留中间文件时会额外打印：

```text
已写入 merged_gitcode_accounts.json，共 N 条账号信息
```

`gitcode_accounts.md` 的列为：

```text
gitcode | 姓名 | 邮箱 | 资源组 | 是否为算子组 | PL
```

更新后的 Markdown 会按 `资源组` 排序；无资源组的账号排在最后。同一个 gitcode 多次出现时会合并，有效的新字段覆盖旧字段。

## 2. update_issue_excel_from_md.py

### 功能

根据脚本同级的 `gitcode_accounts.md` 更新 issue Excel。

更新内容：

- `提出者`、`责任人`：改成 `中文名(gitcode)` 格式。
- `资源组`：根据 `责任人` 从 `gitcode_accounts.md` 中查找并更新。
- `PL`：根据 `责任人` 从 `gitcode_accounts.md` 中查找并更新。
- `算子内测`：根据 `提出者` 的 `是否为算子组` 更新。

### 使用方式

```bash
python3 update_issue_excel_from_md.py issue_20260813_085422.xlsx
```

### 使用约束

- 只接收 1 个命令行参数：issue Excel 文件路径。
- `gitcode_accounts.md` 必须和脚本在同一目录。
- `gitcode_accounts.md` 必须包含以下列：
  - `gitcode`
  - `姓名`
  - `资源组`
  - `PL`
  - `是否为算子组`
- Excel 中如果存在多个 sheet，脚本会遍历所有 sheet。
- 只有 sheet 中存在 `提出者` 或 `责任人` 列时才会处理该 sheet。
- 找不到账号映射时，原单元格保持不变。
- 如果 Excel 中缺少 `资源组`、`PL`、`算子内测` 中的某些列，对应字段不会更新。

### 执行结果

不会覆盖原 Excel，会生成一个新文件：

```text
原文件名_updated.xlsx
```

例如：

```text
issue_20260813_085422_updated.xlsx
```

终端会打印：

```text
已写入 issue_20260813_085422_updated.xlsx
处理 sheet 数: N
更新提出者/责任人单元格: N
更新资源组单元格: N
更新 PL 单元格: N
更新算子内测单元格: N
```

## 3. send_resource_group_emails.py

### 功能

读取更新后的 issue Excel，按 `资源组` 分组发送 issue 提醒邮件。

每个资源组发送一封邮件，邮件包含：

- 邮件主题：`{资源组} asc-devkit仓 Issue 提醒`
- 邮件正文顶部统计：
  - `生成时间`
  - 当前资源组 open issue 总数
  - 当前资源组超期未关闭数量
  - 当前资源组未回复数量
- 责任人 issue 数量统计表
- 邮件正文中的 Excel 表格预览
- 当前资源组对应的 Excel 附件

发送过程中会临时生成附件目录：

```text
resource_group_email_attachments/
```

如果设置 `KEEP_INTERMEDIATE_FILES=1`，发送报告会写入：

```text
resource_group_email_send_report.json
```

### 使用方式

先确认脚本顶部的 SMTP 配置：

```python
SMTP_HOST = "smtp.huawei.com"
SMTP_USER = "xxxx"
SMTP_PASSWORD = "XXXXX"
```

调试发送时，保留测试收件人：

```python
TEST_TO = ["wangxu243@h-partners.com"]
```

正式发送给资源组责任人和 PL 时，改为：

```python
TEST_TO = None
```

也可以使用空列表：

```python
TEST_TO = []
```

正式发送时，如果所有资源组都需要额外发送给固定人员，可以配置：

```python
EXTRA_TO = [
    "gaoyue6@huawei.com",
    "wangxu243@h-partners.com",
]
```

然后执行：

```bash
python3 send_resource_group_emails.py issue_20260813_085422_updated.xlsx
```

### 使用约束

- 只接收 1 个命令行参数：已更新后的 issue Excel 文件路径。
- `gitcode_accounts.md` 必须和脚本在同一目录，用于查找责任人和 PL 的邮箱。
- 当前只读取 Excel 的第一个 sheet 生成收件人、附件和邮件正文预览。
- 只发送以下资源组：
  - `向量编程一组`
  - `向量编程二组`
  - `向量编程三组`
  - `矩阵编程一组`
  - `矩阵编程二组`
  - `矩阵编程三组`
  - `编程框架一组`
  - `编程框架二组`
  - `Pass开发组`
  - `工具组`
  - `通信编程组`
- 只处理 `状态 == open` 的 issue；非 open issue 不参与统计、不进入邮件正文、不进入附件。
- 责任人 issue 数量统计表只统计当前资源组的 open issue，并按 issue 数量从多到少排序。
- 附件和邮件正文会删除 `状态`、`资源组`、`PL` 三列。
- 附件保留第一列的超链接。
- `标题` 列宽设置为 30，并开启自动换行。
- 超期规则固定为 `OVERDUE_UNCLOSED_DAYS = 14`，即 `开启时长 >= 14天`。
- 未回复规则：
  - `最后回复时间` 为空；
  - 且 `提出者` 和 `责任人` 不是同一个人。
- 超期或未回复的 issue 行会在邮件正文和附件中使用橙色底色。
- 如果 `TEST_TO` 列表有值，邮件实际只发给测试邮箱；报告中仍会记录真实收件人。
- 如果 `TEST_TO = None` 或 `TEST_TO = []`，邮件会发给当前资源组的责任人邮箱、PL 邮箱，并在最后追加 `EXTRA_TO` 中的邮箱。
- 如果某资源组没有有效收件人，脚本会跳过该资源组发送。

### 执行结果

执行成功后会：

1. 临时生成各资源组附件。
2. 通过 SMTP 发送邮件。
3. 默认清理附件目录和发送报告。

如果设置 `KEEP_INTERMEDIATE_FILES=1`，会保留附件目录和发送报告：

```text
resource_group_email_attachments/
resource_group_email_send_report.json
```

终端会打印：

```text
Email send success
发送资源组邮件数: N
未保留邮件附件和发送报告
```

保留中间文件时会额外打印：

```text
已生成附件目录 resource_group_email_attachments
已写入 resource_group_email_send_report.json
```

保留下来的发送报告中包含：

- `resource_group`：资源组
- `real_to_addrs`：真实责任人和 PL 邮箱
- `actual_to_addrs`：实际发送邮箱
- `extra_to_addrs`：正式发送时额外添加的公共收件人邮箱
- `owner_accounts`：责任人 gitcode
- `owner_emails`：责任人邮箱
- `pl_emails`：PL 邮箱
- `pl_values`：PL 字段原始值
- `attachment`：附件路径
- `sent`：是否发送

## 每日执行流程

每天拿到新的 issue Excel 后，按下面顺序执行：

```bash
python3 merge_gitcode_accounts.py issue_20260813_085422.xlsx
python3 update_issue_excel_from_md.py issue_20260813_085422.xlsx
python3 send_resource_group_emails.py issue_20260813_085422_updated.xlsx
```

执行后的核心结果：

1. `gitcode_accounts.md`：长期保存并持续更新的账号库。
2. `issue_20260813_085422_updated.xlsx`：当天用于发邮件的新 Excel。
3. 邮件发送结果：默认不保留附件和发送报告；设置 `KEEP_INTERMEDIATE_FILES=1` 时保留。

调试邮件效果时：

```python
TEST_TO = ["wangxu243@h-partners.com"]
```

正式发送时：

```python
TEST_TO = None
```

或：

```python
TEST_TO = []
```

正式发送时需要每封邮件都额外发送给固定人员：

```python
EXTRA_TO = [
    "gaoyue6@huawei.com",
    "wangxu243@h-partners.com",
]
```
