# Tag 词表工件说明

本目录保存冻结 tag 词表的唯一事实源：

- `tag-taxonomy.json`：冻结的 namespace、标签值、定义和选择规则。
- `tag-rules.yaml`：必填与证据规则，与 `tag-taxonomy.json` 配套。
- `core-v8-frozen.json`：旧基线词表，保留用于回退与对照。
- `releases/`：经人工批准发布的冻结版本（`c-api-full-v2-frozen.json`、`core-v9-frozen.json`）。

调研、标注和写入流程都必须从 `tag-taxonomy.json` 读取 namespace 和值；`okf-yaml-generate`
（tags 模式）与生产写入器同样读取本目录，不得另建并行词表。

## 版本规则

- 词表和规则直接维护在本目录的唯一事实源文件中；调研 `approve` 的冻结产物写入 `releases/`。
- provisional 文件只存在于对应的 `runs/<run-id>/`，不能传给 tag 写入器。
- 发布 registry 不在本目录自动切换；切换生产版本是单独的人工批准动作。

仓库中其他 taxonomy 文件只作历史兼容或迁移输入，不得继续作为新的修改来源。
