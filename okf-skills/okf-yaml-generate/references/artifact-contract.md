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
字段规则写空。`dtype` 是含 `<dtype>` 的 `signature` 条目中的字符串数组，写入时与 `prototype_id` 同级渲染为
单行数组，绝不位于根级或 Markdown 原型前的 YAML 围栏。证据必须带 source path 和逐字 excerpt。

`fields.description` 不由 worker 或总控脚本生成。它必须来自同一文档、同一哈希且已通过
`okf-description-generate validate` 的 `description-result.json`；worker 将其中的 evidence 补充当前
`source_path` 后写入本工件。没有有效结果时，description 保持空并记录问题。

`doxygen` 不是 `{path, anchor, text}` 补丁。每项以 `header`、canonical `function`、带名称的
`parameters`（宏另含 `macro_id`）定位真实声明；控制器负责写入关联块并只增加有 evidence 的
`level`、`brief` 或 `params`。`example_markers` 不是自由 comment；它复用样例调用的文件和偏移，控制器
严格按旧规则生成或更新成对 `//! [override_id]` 标签。

`c_api/defs` 实体页的 `fields` 可附加 source-derived 字段：`api_kind`、`headers`、`declaration`、
`type_catalog`。`headers` 仅在唯一的真实声明 header 已验证时写入；`declaration` 记录 enum/record/constant/
type alias 或对象宏定义，`type_catalog` 记录数据类型分组。函数宏仍只通过 `signature[].macro_id` 表达，
不写 top-level `headers` 或 `declaration`。定义页没有可调用签名时，不能产生“缺函数原型”问题。

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
