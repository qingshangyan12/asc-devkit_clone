# 问题处理规则

## 分类

- `document_defect`：Markdown 缺少或矛盾，导致某字段无法从文档确认。
- `source_defect`：头文件、样例或实现代码存在错误或与文档冲突。
- `tool_defect`：输入事实明确，但当前生成器无法处理合法结构。
- `unresolved`：现有证据不足，无法安全分类。

## 决定

- `blank_field`：把对应 YAML 字段或 namespace 在 worker 工件中置空；渲染输出省略该字段或 namespace，不写空数组或空值。
- `report_source_defect`：保留可确认 YAML，报告源问题，不改源文件。
- `report_unresolved`：保留可确认 YAML，记录待人工处理。
- `tool_change_required`：本轮不写入，报告需要修改生成器。

文档错误不能导致其他字段全部消失。source defect 不能通过修改头文件、样例或实现代码解决。
同一文档的多个 issue 在最终报告中合并展示。

## 严格决议边界

| issue_type | 允许的 resolution | 是否允许 validate 通过 |
| --- | --- | --- |
| `document_defect` | `blank_field`、`report_source_defect`、`report_unresolved` | 仅 `blank_field` 或 `report_source_defect` |
| `source_defect` | `report_source_defect`、`report_unresolved` | 仅 `report_source_defect` |
| `unresolved` | `report_unresolved` | 否 |
| `tool_defect` | `tool_change_required` | 否 |

`tool_defect` 不得被降级成 `blank_field`。语义输入缺失、hash 不一致、冻结 taxonomy 不匹配和解析器
无法处理合法声明都属于工具/协议阻断；必须先修复工件或代码，再重新运行 `prepare`、`worker` 和
`validate`。任何 `report_unresolved` 或 `tool_change_required` 都会生成 `validation.json` 的
`blocked` 状态，`write` 不得执行。
