# Dependencies 契约

## YAML 字段

Skill 只生成 API Markdown front matter 中的 `dependencies` 字段。每个条目只允许：

```yaml
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/.../configure.md
    kind: requires
    condition: 量化模式下
```

`path` 是 DevKit 根目录相对的、已存在的 Markdown 路径，不使用当前文档相对路径，也不使用绝对路径。
`kind` 只能是 `requires`、`configures`、`constrained_by` 或 `related`。`condition` 只保留正文明确写出的适用条件。

没有明确关系时必须写 `dependencies: []`。不生成 `status`、`example_before`、`all_of`、`any_of` 或其他旧字段。

## 关系含义

- `requires`：当前 API 使用前必须满足的 API 前置关系。
- `configures`：当前 API 为另一个 API 或执行模式提供配置关系。
- `constrained_by`：当前 API 受另一个 API、模式或约束文档限制。
- `related`：有明确关联但不表示固定调用顺序。

带条件的前置关系仍使用 `requires`，并填写原文条件；不能把条件关系改写成无条件依赖。

## 旧字段替换

写入时，如果目标 front matter 存在 `usage_dependencies`，Skill 会删除整个旧字段，再写入本次生成的
`dependencies`。最终文档不能同时存在两个字段。旧字段不会被机械映射；新字段必须经过当前正文证据重新确认。

## 关系约束

- 依赖目标必须存在，且不能是执行 API 自身。
- `requires` 关系不能形成环；发现环时整个写入失败。
- `any_of` 关系无法由当前字段表达时保持待审核，不得拆成多个无条件 `requires`。
- 反向索引不是本 Skill 的输出，也不在 DevKit 根目录生成。
