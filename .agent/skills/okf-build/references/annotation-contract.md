# 语义标注工件契约

标注工件是 `prepare` 之后、`validate` 之前由标注 Agent 产出的唯一语义输入，
放在 run 目录 `annotations/*.json`（一个或多个文件，不得重复覆盖同一文档）。
脚本负责全部确定性校验（哈希、词表、逐字证据、依赖图）；Agent 只产出语义判断。

## 文件结构

```json
{
  "schema_version": 1,
  "purpose": "okf_semantic_annotations",
  "annotations": [
    {
      "document_path": "docs/zh/api/SIMD-API/c_api/.../asc_xxx.md",
      "document_sha256": "<manifest 中该文档的 sha256>",
      "description": {
        "value": "从Unified Buffer读取数据搬入矢量数据寄存器，支持对齐与非对齐模式。",
        "evidence": [
          {"source_path": "docs/zh/api/SIMD-API/c_api/.../asc_xxx.md", "excerpt": "正文逐字摘录"}
        ]
      },
      "tag": {
        "roles": ["data_movement"],
        "operations": ["load"],
        "traits": ["aligned_transfer"]
      },
      "tag_evidence": {
        "roles:data_movement": "正文逐字摘录",
        "operations:load": "正文逐字摘录",
        "traits:aligned_transfer": "正文逐字摘录"
      },
      "dependencies": [
        {
          "path": "docs/zh/api/SIMD-API/c_api/.../asc_yyy.md",
          "kind": "requires",
          "condition": "地址寄存器偏移模式",
          "evidence": {
            "source_path": "docs/zh/api/SIMD-API/c_api/.../asc_xxx.md",
            "excerpt": "正文逐字摘录"
          }
        }
      ]
    }
  ]
}
```

## 字段规则

- 覆盖范围：`annotation-task.json` 列出的每个文档恰好一条标注（`reference.index`
  页无需标注）。
- `description`：中文单段 8–180 字，陈述核心动作、处理对象与关键约束；
  不是标题或首句复制；1–3 条正文逐字摘录作证据；概念页仅需本字段。
- `tag`：只允许 `roles/operations/traits` 三个 namespace，值必须出自冻结词表。
  实体页必填 `roles`；roles 含 compute/data_movement/data_initialization 时必填
  `operations`；`traits` 仅在有正文证据且影响检索时填写。不适用 namespace 省略。
- `tag_evidence`：每个选中的 `namespace:value` 一条正文逐字摘录；键集合与 tag
  展开完全一致。可推导维度（执行模型、数据位置、dtype、架构、掩码、废弃等）
  不得作为标签，见 [头字段与标注规范.md](头字段与标注规范.md) 第 1 节。
- `dependencies`：必填（可为 `[]`）。kind 取
  `requires/configures/constrained_by/related/replaced_by`；仅凭正文明确表达的关系
  生成，示例调用顺序不能单独证明 `requires`；条件保留在 `condition`，不得扩大为
  全局关系；无法唯一确定目标时不生成。废弃实体指向替代目标用 `replaced_by`。
- 证据一律取自目标文档正文（front matter 之后），逐字子串匹配；旧 front matter
  是待替换遗留数据，禁止作为证据或依据。

## 失败与重跑

- 任一校验失败：修正对应标注后重跑 `validate`；文档正文变化必须重跑 `prepare`。
- `version` 由脚本从 Release Notes 推导（`prepare` 已写入任务），标注不需要提供；
  `validate` 会拒绝与推导值冲突的标注 version。
