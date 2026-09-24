# Worker 与 Resolver 协议

## Worker

1. worker 先按结构分类器确定扁平 `okftype`，再按 Profile 生成允许字段。
2. 普通 API 必须接收批次级 `semantic-input.json`；description、tag 和 dependencies 只能来自经过 hash 校验的语义工件。
3. source-derived 字段由本地解析器生成，包括 signature、header、declaration、examples 和 Doxygen 记录。
4. `reference.index.cpp` 不需要语义输入；worker 从正文 Markdown 链接生成 `navigation`，不生成 description、tag、dependencies 或 examples。
5. worker-result 必须包含字段、问题和适用的 evidence。不得从旧 front matter 继承字段，也不得生成 Profile 未声明的字段。

普通 API 缺少语义工件时标记 `semantic_verified: false`，validate 阻断；Index 可以没有语义工件。

## Resolver

主控执行：

```bash
python3 scripts/controller.py resolve-issues --run-dir <run-dir>
```

resolver 只根据 `resolution-input.json` 处理当前运行的问题，不修改 Markdown、头文件、样例或实现代码。

## 决议矩阵

| 问题类型 | 允许的 resolution | 结果 |
| --- | --- | --- |
| `document_defect` | `blank_field`、`report_source_defect`、`report_unresolved` | 只影响对应字段 |
| `source_defect` | `report_source_defect`、`report_unresolved` | 保留其他已验证字段 |
| `unresolved` | `report_unresolved` | validate 阻断 |
| `tool_defect` | `tool_change_required` | validate 阻断 |

`report_unresolved` 和 `tool_change_required` 都会阻止 write；resolver 不能用空字段掩盖工具或协议错误。
