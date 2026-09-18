# 问题处理规则

## 分类

- `document_defect`：Markdown 缺少或矛盾，导致某字段无法从文档确认。
- `source_defect`：头文件、样例或实现代码存在错误或与文档冲突。
- `tool_defect`：输入事实明确，但当前生成器无法处理合法结构。
- `unresolved`：现有证据不足，无法安全分类。

## 决定

- `blank_field`：只把对应 YAML 字段或 namespace 置空。
- `report_source_defect`：保留可确认 YAML，报告源问题，不改源文件。
- `report_unresolved`：保留可确认 YAML，记录待人工处理。
- `tool_change_required`：本轮不写入，报告需要修改生成器。

文档错误不能导致其他字段全部消失。source defect 不能通过修改头文件、样例或实现代码解决。
同一文档的多个 issue 在最终报告中合并展示。
