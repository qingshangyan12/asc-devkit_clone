# 字段规则

- `resource` 使用 `asc-devkit/` 加仓库相对 Markdown 路径。
- `description` 只能来自 Markdown 功能说明或等价正文段落。
- `signature` 只记录正文、声明和证据能确认的集合；无法确认时为 `[]`。
- `tag` 使用本 Skill 内冻结 taxonomy，主题每个只出现一次；正常文档的必填 namespace 必须非空，
  只有经过 issue/resolution 记录的字段才允许写 `[]`。每个非空值必须有 `namespace:value` 级逐字证据。
- `dependencies` 是唯一依赖字段；无明确关系写 `[]`，删除旧 `usage_dependencies`。
- `examples` 只记录真实存在且能关联到文档的样例；无法确认时为 `[]`。
- include 只允许新增 Doxygen 注释；任何声明不一致都作为 source defect 报告。
- example marker 只允许插入注释标记，不修改调用或实现代码。
