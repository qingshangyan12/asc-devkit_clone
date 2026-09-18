# 工件契约

所有 JSON 使用 UTF-8 和 `schema_version: 1`。路径相对于 DevKit 根目录。运行脚本写入任务包和校验报告；代理仅可写入下列输出工件。

## 发现输出

写入 `outputs/discovery-a.json` 或 `outputs/discovery-b.json`。

```json
{
  "schema_version": 1,
  "purpose": "independent_open_discovery",
  "run_id": "example-v1",
  "worker": "discovery-a",
  "reviewed_documents": ["docs/.../api.md"],
  "documents": [
    {
      "path": "docs/.../api.md",
      "title": "API title",
      "peer_paths": ["docs/.../peer-a.md", "docs/.../peer-b.md"],
      "proposed_tags": ["operations:example_operation"],
      "evidence": {
        "operations:example_operation": "从此 API Markdown 正文逐字截取的证据。"
      }
    }
  ]
}
```

`reviewed_documents` 与 `documents.path` 必须恰好等于任务包分配的路径。某篇文档可以没有 proposal：使用 `proposed_tags: []` 和 `evidence: {}`。每个 proposal 恰有一个非空、逐字匹配正文的 evidence。若范围至少有三篇文档，`peer_paths` 必须包含两条不同、存在且不等于 `path` 的 API Markdown 路径。

## 检索金标准输出

写入 `outputs/retrieval-gold.json`。

```json
{
  "schema_version": 1,
  "purpose": "independent_retrieval_gold",
  "run_id": "example-v1",
  "worker": "retrieval-gold",
  "reviewed_documents": ["docs/.../api.md"],
  "queries": [
    {
      "id": "q-001",
      "query": "用户自然语言请求",
      "relevant_paths": ["docs/.../api-a.md", "docs/.../api-b.md"],
      "confusing_paths": ["docs/.../api-c.md"]
    }
  ]
}
```

查询数量至少达到任务包声明的最小值，且共同引用每篇被分配文档。relevant 和 confusing 路径必须在任务包内且互不重叠。

## 主代理评选

仅在所有 worker 报告有效后写入 `main-agent-review.json`。

```json
{
  "schema_version": 1,
  "purpose": "main_agent_taxonomy_review",
  "run_id": "example-v1",
  "reviewer": "main_agent_rule_based_adjudication",
  "reviewed_documents": ["docs/.../api.md"],
  "normalizations": [
    {"from": "operations:alternate_spelling", "to": "operations:example_operation", "reason": "同一已文档化操作的不同拼写。"}
  ],
  "decisions": [
    {
      "tag": "operations:example_operation",
      "decision": "approve",
      "reason": "两个发现子代理均支持，且金标准查询需要该区分。",
      "definition": "稳定、用户可见的操作定义。",
      "selection_rule": "当 API 自身文档说明执行该操作时使用。",
      "evidence": [
        {"path": "docs/.../api-a.md", "excerpt": "逐字源文证据。"},
        {"path": "docs/.../api-b.md", "excerpt": "逐字源文证据。"}
      ],
      "retrieval_cases": [
        {"query_id": "q-001", "effect": "candidate_filter"},
        {"query_id": "q-002", "effect": "rank_boost"}
      ]
    }
  ]
}
```

汇总中每个 `requires_main_agent_review` 候选都必须恰有一个 `approve`、`defer` 或 `reject` 决定。`approve` 至少需要两篇不同文档的 evidence，以及 profile 规定数量的不同金标准 `query_id`。记录 normalizations 后，两个发现子代理必须都支持该值。

## 人工审批

审阅 `approval/release-bundle.json` 与 provisional taxonomy/profile diff 后，才可创建 `approval.json`。

```json
{
  "schema_version": 1,
  "purpose": "human_taxonomy_approval",
  "run_id": "example-v1",
  "decision": "approve",
  "approver": "name-or-team",
  "approved_at": "2026-09-17T10:00:00+08:00",
  "comment": "批准作为候选发布。"
}
```

只有 `decision: "approve"` 才能生成冻结 taxonomy/profile。拒绝或延后必须保留运行工件，不得修改既有 release。

## 最终 API 标注的必填概念

词表中的 namespace 定义是强制覆盖策略的事实来源。对每一篇最终要写入的 API Markdown，标注
工件必须：

- `tag` 恰好包含 taxonomy 中的所有 namespace；
- 对每个 `required: true` 的 namespace，值列表至少包含一个冻结值；
- 每个 `namespace:value` 都有一条非空 `evidence`；除 `api_surfaces` 这类允许由稳定路径证明的导航
  值外，其他证据必须逐字出现在目标 Markdown 正文中；
- 不允许用空数组、`unknown`、`other` 或临时值绕过必填概念；无法从正文确定概念时，该 API 必须
  阻断写入并进入人工补证/词表修订队列。

开放发现工件可以暂时没有候选值，因为它的任务是发现词表；但在 taxonomy 批准后的逐篇标注阶段，
必须运行覆盖校验，覆盖不完整时不得调用 tag 写入器。
