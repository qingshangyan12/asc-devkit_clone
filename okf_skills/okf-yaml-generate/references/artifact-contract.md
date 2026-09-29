# Artifact 契约

## Worker 结果

Index 文档使用独立字段契约：

### Index 示例

```json
{
  "fields": {
    "okftype": "reference.index",
    "title": "Vector Compute",
    "resource": "asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute/vector_compute.md",
    "navigation": [
      {"path": "docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/vector_arith.md"}
    ],
    "version": "9.0.0-"
  }
}
```

Index 结果不得包含 `description`、`tag`、`dependencies`、`examples`、`signature` 或 `header`。

### 非 Index 页示例

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
      "plan": {
        "okftype": "reference.api",
        "rule": "callable_structure",
        "fields": ["okftype", "title", "resource", "description", "tag", "signature", "dependencies", "examples", "version"],
        "outputs": ["front_matter", "doxygen", "example_markers"]
      },
      "fields": {
        "okftype": "reference.api",
        "title": "foo",
        "resource": "asc-devkit/docs/zh/api/SIMD-API/c_api/foo.md",
        "description": "...",
        "tag": [{"api_surfaces": ["c_api"]}],
        "signature": [],
        "dependencies": [],
        "examples": [],
        "version": "9.0.0-"
      },
      "evidence": {
        "description": [{"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "正文逐字摘录"}],
        "api_surfaces:c_api": [{"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "docs/zh/api/SIMD-API/c_api/foo.md"}]
      },
      "issues": [],
      "semantic_verified": true,
      "doxygen": [],
      "example_markers": []
    }
  ]
}
```

每个文档只能由一个 worker 提交。`plan` 是扁平 Profile 计算出的字段和输出契约；`fields` 只允许包含
公共字段和该 Profile 声明的字段，无法确认的字段按字段规则写空或省略。Index 只允许写入 navigation。`dtype` 是含 `<dtype>` 的
`signature` 条目中的字符串数组，写入时与 `prototype_id` 同级渲染为
单行数组，绝不位于根级或 Markdown 原型前的 YAML 围栏。证据必须带 source path 和逐字 excerpt。

非 Index 页的 `fields.description`、`fields.tag` 和 `fields.dependencies` 不由 worker 或总控脚本猜测。它们必须来自同一
批次 `semantic-input.json`，并且每项 annotation 的 document hash 与当前输入一致。description 必须来自同一
文档、同一哈希且已通过 `okf-description-generate validate` 的 `description-result.json`；tag 的每个非空值
必须有 namespace:value 级 evidence。没有有效语义结果时，非 Index 页 worker 只能标记 `semantic_verified: false`，
`validate` 必须阻断；Index 不需要语义结果。

语义输入契约见 [`semantic-contract.md`](semantic-contract.md)。它要求 annotation 覆盖整个 batch，且不允许
从旧 front matter 继承字段。

`doxygen` 不是 `{path, anchor, text}` 补丁。每项以 `header`、canonical `function`、带名称的
`parameters`（宏另含 `macro_id`）定位真实声明；控制器负责写入关联块并只增加有 evidence 的
`level`、`brief` 或 `params`。`example_markers` 不是自由 comment；它复用样例调用的文件和偏移，控制器
严格按旧规则生成或更新成对 `//! [override_id]` 标签。发生跨型号 concrete ID 冲突时，控制器可在对应
Doxygen record 和 call record 中携带派生的 `override_id: <canonical>@<型号>`；它必须由 canonical function
校验。不同物理 header 声明保留该 Doxygen ID；多个记录命中同一物理声明时，Doxygen 改用 canonical ID 并合并
archs，marker 始终使用与 signature 相同的派生 ID。

已验证 concrete 函数有命名参数时，worker 从本页参数表生成 `doxygen.params`；每项含 `name`、
`direction`、`description` 和逐字表格 `evidence`。表缺失、未覆盖全部参数或方向不明确时，
保留可确认的基础 Doxygen 关联并附加 `include_doxygen` 问题，不伪造缺失参数。

`c_api/defs` 实体页的 `fields` 可附加 source-derived 字段：`header`、`declaration`、`type_catalog`。
`header` 仅在唯一的真实声明 header 已验证时写入；`declaration` 记录 enum/record/constant/
type alias 或对象宏定义，`type_catalog` 记录数据类型分组。函数宏通过 `signature[].macro_id`
和顶层 `header` 表达，不写 `declaration`。定义页没有可调用签名时，不能产生“缺函数原型”问题。

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
