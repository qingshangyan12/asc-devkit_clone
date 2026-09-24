# 语义输入契约

`worker` 只负责结构化解析（函数原型、header 映射、样例调用、Doxygen 声明身份）与写入计划；
`description`、`tag`、`dependencies` 三类语义字段必须由**语义取证 agent** 产出批次级
`semantic-input.json`，再由控制器按文档 hash 与冻结 taxonomy 校验后合并。语义代理不得触碰
signature、doxygen、example_markers 等结构字段。

- 缺少语义输入时，`worker` 产出的结果 `semantic_verified: false`，仅可用于调试，`validate` 必须阻断
  （`tool_defect` 级）；控制器单元测试直接构造 worker-result 时须显式置 `semantic_verified: true`。
- 语义输入是唯一语义来源：重新生成时旧 front matter 的语义值不被继承，语义代理必须从当前正文重新取证。
- `api_surfaces` 不属于语义输入：controller 按 `config/source-context-v1.yaml` 从稳定资源路径自动注入
  （证据即文档路径本身）。

## 文件结构

```json
{
  "schema_version": 1,
  "purpose": "okf_yaml_semantic_input",
  "run_id": "run-20260924-capi",
  "batch_id": "batch-01",
  "annotations": [
    {
      "document_path": "docs/zh/api/SIMD-API/c_api/foo.md",
      "document_sha256": "<prepare 记录的输入哈希>",
      "description": {
        "value": "按元素相加。",
        "evidence": [
          {"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "正文逐字摘录"}
        ]
      },
      "tag": [
        {"roles": ["compute"]},
        {"operations": ["add"]},
        {"execution_models": ["memory_vector"]}
      ],
      "tag_evidence": {
        "roles:compute": [
          {"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "正文逐字摘录"}
        ],
        "operations:add": [
          {"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "正文逐字摘录"}
        ],
        "execution_models:memory_vector": [
          {"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "正文逐字摘录"}
        ]
      },
      "dependencies": [
        {
          "path": "docs/zh/api/SIMD-API/c_api/bar.md",
          "kind": "related",
          "evidence": {"source_path": "docs/zh/api/SIMD-API/c_api/foo.md", "excerpt": "含该关系的正文行"}
        }
      ]
    }
  ]
}
```

## 字段规则

- `annotations` 必须与 task 的 documents 精确对账：不能多、不能少、不能重复；每项
  `document_sha256` 必须等于 `prepare` 时的输入哈希，输入变化即拒绝。
- `description.value` 必须来自同一文档、同一哈希且已通过 `okf-description-generate validate` 的
  `description-result.json`；正文不足以可靠摘要时 value 为空字符串且不带 evidence（unresolved）。
  非空 description 的每条 evidence 的 `excerpt` 必须逐字出现在该文档正文。
- `tag` **按需生成**：只包含有逐字证据的 namespace，每项为单键对象；无证据的 namespace 直接省略，
  不写空数组，也不得包含 `api_surfaces`。降级信号由 controller 生成的 scaffold issue 携带，交
  resolver 决议（`config/tag-rules.yaml` 声明 always/条件必填，条件必填 namespace 允许决议后省略）。
- `tag_evidence` 必须覆盖 tag 中的每个 `namespace:value`；`source_path` 必须是当前文档路径，
  `excerpt` 必须逐字出现在该文档正文。
- `dependencies` 为空是合法的明确结果。每项必须包含 `path`（存在的 .md）、`kind`
  （requires/configures/constrained_by/related）与内联 `evidence{source_path, excerpt}`；
  互为"配对使用"的双向关系都用 related。
- `tag` 值必须出自冻结 taxonomy；证据不足时宁可省略，不得猜测或伪造。

## 调用约束

```bash
python3 scripts/controller.py worker \
  --task tmp/<run-id>/batches/batch-01/task.json \
  --semantic-input tmp/<run-id>/batches/batch-01/semantic-input.json \
  --output tmp/<run-id>/batches/batch-01/worker-result.json
```

worker 将语义字段与 source-derived 结构字段合并，但不覆盖原型、声明、样例或 Doxygen 解析结果。
语义代理的工作顺序：先逐篇执行 `okf-description-generate` 流程取得验证过的 description，再从正文
取证 tag 与 dependencies，最后一次性写出 `semantic-input.json`；正文取证使用 `rg` 定位加短范围读取。
