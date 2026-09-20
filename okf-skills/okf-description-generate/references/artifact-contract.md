# Description 工件契约

所有 JSON 采用 UTF-8，`schema_version` 固定为 `1`。`prepare` 创建 `task.json` 与 `run-manifest.json`；
Agent 只写 `description-result.json`。

```json
{
  "schema_version": 1,
  "purpose": "okf_api_description_result",
  "run_id": "asc-example-description",
  "document_path": "docs/zh/api/SIMD-API/c_api/.../asc_example.md",
  "document_sha256": "prepare 记录的输入哈希",
  "status": "ready",
  "description": "对 API 核心动作和对象的中文摘要。",
  "evidence": [
    {
      "excerpt": "目标 Markdown 正文中的逐字摘录。"
    }
  ],
  "confidence": 0.9
}
```

## `ready` 结果

- `description` 必须是 8 至 180 个字符的单段文本，不能等于 API 名称或标题，不能包含 Markdown 标题、列表、
  链接或代码块。
- `evidence` 必须有一至三项。每项只含非空 `excerpt`；它必须逐字出现在目标 Markdown 去掉 front matter 后的正文中。
- `confidence` 是 0 到 1 的数值。它只记录 Agent 的确信程度，不是事实依据。

## `unresolved` 结果

正文不足以支持可靠摘要时，使用下列形式：

```json
{
  "schema_version": 1,
  "purpose": "okf_api_description_result",
  "run_id": "asc-example-description",
  "document_path": "docs/zh/api/SIMD-API/c_api/.../asc_example.md",
  "document_sha256": "prepare 记录的输入哈希",
  "status": "unresolved",
  "description": "",
  "evidence": [],
  "reason": "正文没有能确认核心行为的功能说明或等价证据。",
  "confidence": 0.0
}
```

`unresolved` 不能有 description 或 evidence。默认只生成验证报告，不写入 Markdown；用户明确要求字段置空时才允许
使用 `--allow-unresolved`。
