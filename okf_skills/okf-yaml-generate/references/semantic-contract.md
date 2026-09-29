# 语义输入契约

语义输入服务于所有非 Index Profile。Index Profile 由 Markdown 链接直接生成
`navigation`，不需要 description、tag、dependencies 或语义输入工件。

## 非 Index 页 annotation

每个 annotation 必须包含：

```json
{
  "document_path": "docs/zh/api/SIMD-API/c_api/foo.md",
  "document_sha256": "<prepare 后的 hash>",
  "description": {"value": "...", "evidence": [{"source_path": "...", "excerpt": "..."}]},
  "tag": {"roles": ["compute"]},
  "tag_evidence": {"roles:compute": [{"source_path": "...", "excerpt": "..."}]},
  "dependencies": [],
  "version": "9.0.0-"
}
```

annotation 必须覆盖 batch 中的每个非 Index 文档，路径和 hash 必须与 `prepare` 一致。description 必须来自
已验证的 description artifact；每个非空 tag value 必须有对应的逐字 evidence；dependencies 必须是合法数组。
`traits:deprecated` 的证据须表明当前页面描述的实体已废弃；描述某个符号可标注其他废弃接口不算。
当前实体已废弃且正文明确指向替代目标时，在 `dependencies` 使用 `kind: replaced_by`，方向为当前实体到替代文档；
组合替代可列多个目标，并用 `condition` 说明作用。没有明确目标时不推断。

## Index

Index 不进入普通语义 annotation 流程。worker 从正文 Markdown 链接生成：

```json
{"navigation": [{"path": "docs/zh/api/.../child.md"}]}
```

链接必须是仓内 API Markdown 路径，保留正文出现顺序并去除重复项。

## 校验

非 Index 页缺少语义输入、hash 不一致或 evidence 不合法时，`validate` 阻断。旧 front matter 不能作为语义证据继承。
