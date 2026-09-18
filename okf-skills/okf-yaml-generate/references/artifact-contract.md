# Artifact 契约

## Worker 结果

```json
{
  "schema_version": 1,
  "run_id": "run-01",
  "batch_id": "batch-01",
  "worker_id": "worker-a",
  "documents": [
    {
      "path": "docs/zh/api/SIMD-API/c_api/foo.md",
      "sha256": "...",
      "fields": {
        "okftype": "reference.api.cpp",
        "title": "foo",
        "resource": "asc-devkit/docs/zh/api/SIMD-API/c_api/foo.md",
        "description": "...",
        "tag": [{"api_surfaces": ["c_api"]}],
        "signature": [],
        "dependencies": [],
        "examples": [],
        "timestamp": "2026-09-18"
      },
      "evidence": {
        "description": [{"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "正文逐字摘录"}],
        "api_surfaces:c_api": [{"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "docs/zh/api/SIMD-API/c_api/foo.md"}]
      },
      "issues": [],
      "doxygen": [],
      "example_markers": []
    }
  ]
}
```

每个文档只能由一个 worker 提交。`fields` 必须包含所有顶层字段；不能以缺键表示未知，未知值按
字段规则写空。证据必须带 source path 和逐字 excerpt。

`resolve-issues --run-dir` 会先生成精简的 `issues.json` 和 `resolution-input.json`。问题处理 agent
只读取 `resolution-input.json`，一次性处理该运行的全部问题；不应重新读取完整 API 文档。

## 问题处理决定

```json
{
  "schema_version": 1,
  "run_id": "run-01",
  "decisions": [
    {
      "issue_id": "foo-description-01",
      "classification": "document_defect",
      "resolution": "blank_field",
      "field": "description",
      "note": "正文没有可确认的功能说明。"
    }
  ]
}
```

问题处理 agent 不得以决定为理由修改源文件。主控只接受与问题中的文件、字段和证据一致的决定。
