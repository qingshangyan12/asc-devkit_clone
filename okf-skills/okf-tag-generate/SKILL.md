---
name: okf-tag-generate
description: 从本 Skill 的冻结词表和 API Markdown 正文证据生成并写入 OKF YAML tag。用于已有 OKF front matter 的 API 文档或目录；先校验和 dry-run，再只修改 Markdown front matter 中的 tag 块，不参与词表发现、不创建新标签、不修改正文、头文件或样例。
---

# OKF Tag 生成

## 用途

本 Skill 负责冻结词表确定后的最后一个阶段：为指定 API Markdown 选择已有 tag 值，并把结果写入 YAML front matter。词表调研、候选标签发现、词表审批由 `okf-tag-research` 负责；本 Skill 只消费已批准的冻结词表，不得扩展或另建词表。

本 Skill 是冻结标签词表的唯一事实源：

- `okf-skills/okf-tag-generate/taxonomy/core-v8-frozen.json`

## 适用范围

- 用户明确要求给 API 文档生成、补充或更新 `tag`。
- 目标是单个 Markdown 文件或明确指定的 API Markdown 目录。
- 目标文件已经完成基础 OKF YAML；没有 front matter 时先走项目 OKF 基础阶段，不能由本 Skill 猜测或补齐其他字段。

不适用于词表调研、候选标签发现、检索评测、依赖关系生成或完整 OKF 的其他阶段。

## 工作流程

1. 确认精确的 DevKit 根目录和用户指定的文件或目录范围，不以近期修改文件代替目标。
2. 读取冻结 taxonomy；只使用其中已有的 namespace 和 value。
3. 阅读目标 Markdown 的标题、功能描述、原型、参数说明和约束，收集逐字证据。必要时对照至少两个相邻或同操作 API，但不能把对照文档的事实写成当前文档的证据。
4. 为每个强制 namespace 选择至少一个有证据的值。证据不足时阻断该文档，不能填空数组、临时值或猜测值。
5. 生成 annotation artifact，运行 `scripts/write_tags.py` 的默认 dry-run。dry-run 必须成功后，才能用 `--write` 执行实际写入。
6. 写入后重新解析目标 front matter，确认只改变 `tag`，并确认 9 个 namespace 仍全部存在且值属于冻结词表。

## 强制标签要求

当前 taxonomy 的以下 9 个 namespace 都是必填且非空：

`api_surfaces`、`roles`、`families`、`operations`、`objects`、`execution_models`、`source_locations`、`destination_locations`、`traits`。

YAML 使用主题加内容的数组形式，每个 namespace 只能出现一次：

```yaml
tag:
  - api_surfaces: [c_api]
  - roles: [compute]
  - families: [arithmetic]
  - operations: [add]
  - objects: [vector]
  - execution_models: [register_vector]
  - source_locations: [vector_register]
  - destination_locations: [vector_register]
  - traits: [elementwise]
```

输出顺序固定为 taxonomy 的 namespace 顺序，namespace 内的值按字典序排列。不得写成 `tags`、`semantic_tags`，也不得把主题和值压缩成无 namespace 的字符串数组。

冻结词表基线不能原地修改。经过调研和人工批准的新版本写入本 Skill 的 `taxonomy/releases/`，写入器只能使用已批准的冻结版本。

## 证据要求

- annotation 中每个选中的 `namespace:value` 都必须有 `evidence` 对应项。
- 除 `api_surfaces` 这类稳定导航标签外，证据必须是目标 Markdown 中逐字出现的短摘录。
- 不能仅凭目录、文件名、API 名称、dtype、架构、头文件声明、样例注释或相邻 API 推断语义标签。
- `traits:deprecated` 只有在目标 Markdown 明确声明废弃时才能选择。
- 不能为了满足必填 namespace 修改正文、声明、参数名、类型、实现代码、头文件或样例。

## Annotation artifact

将工件放在项目约定的临时目录（通常为 `tmp/okf-agent/<run-id>/tags.json`），格式如下。`tag` 必须包含且只包含 9 个 namespace；`proposed_tags` 必须为空。

```json
{
  "schema_version": 1,
  "annotations": [
    {
      "path": "docs/zh/api/SIMD-API/c_api/.../asc_example.md",
      "tag": {
        "api_surfaces": ["c_api"],
        "roles": ["compute"],
        "families": ["arithmetic"],
        "operations": ["add"],
        "objects": ["vector"],
        "execution_models": ["register_vector"],
        "source_locations": ["vector_register"],
        "destination_locations": ["vector_register"],
        "traits": ["elementwise"]
      },
      "evidence": {
        "operations:add": "目标 Markdown 中支持该操作的原文摘录"
      },
      "confidence": 0.9,
      "proposed_tags": []
    }
  ]
}
```

## 命令

默认只校验和预览，不写文件：

```bash
python3 okf-skills/okf-tag-generate/scripts/write_tags.py \
  --devkit-root /path/to/asc-devkit \
  --input tmp/okf-agent/<run-id>/tags.json
```

校验成功后才允许写入：

```bash
python3 okf-skills/okf-tag-generate/scripts/write_tags.py \
  --devkit-root /path/to/asc-devkit \
  --input tmp/okf-agent/<run-id>/tags.json \
  --write
```

写入器不生成 tag 索引，不改 headers、examples 或正文。每次目录运行都应使用一个范围精确的 aggregate artifact；不得把目录外的 API 混入工件。

## 失败处理

- taxonomy 不是 `frozen`、namespace 不一致或 value 不存在：停止，不写入。
- 缺少 namespace、值为空、证据缺失或证据不逐字匹配：停止，不写入。
- 目标路径不在支持的 API 文档目录、front matter 不存在或 YAML 无法解析：停止，交回基础 OKF 或人工补证流程。
- dry-run 未成功：禁止追加 `--write`。
