# 语义输入契约

语义输入只服务于需要语义取证的普通 API Profile。Index Profile 由 Markdown 链接直接生成
`navigation`，不需要 description、tag、dependencies 或语义输入工件。

## 普通 API annotation

每个 annotation 必须包含：

```json
{
  "document_path": "docs/zh/api/SIMD-API/c_api/foo.md",
  "document_sha256": "<prepare 后的 hash>",
  "description": {"value": "...", "evidence": [{"source_path": "...", "excerpt": "..."}]},
  "tag": {"roles": ["compute"]},
  "tag_evidence": {"roles:compute": [{"source_path": "...", "excerpt": "..."}]},
  "dependencies": []
}
```

annotation 必须覆盖 batch 中的每个普通 API 文档，路径和 hash 必须与 `prepare` 一致。description 必须来自
已验证的 description artifact；每个非空 tag value 必须有对应的逐字 evidence；dependencies 必须是合法数组。

## Index

Index 不进入普通语义 annotation 流程。worker 从正文 Markdown 链接生成：

```json
{"navigation": [{"path": "docs/zh/api/.../child.md", "title": "Child"}]}
```

链接必须是仓内 API Markdown 路径，保留正文出现顺序并去除重复项。

## 校验

普通 API 缺少语义输入、hash 不一致或 evidence 不合法时，`validate` 阻断。旧 front matter 不能作为语义证据继承。
