# Tag 调研工件说明

本目录不保存冻结 tag 词表。冻结 taxonomy 的唯一事实源是
`../../okf-tag-generate/taxonomy/tag-taxonomy.json`；调研、标注和写入流程都必须从那里读取 namespace
和值。标签检索策略由其他专门 Skill 负责，本目录不维护相关配置。

## 版本规则

- 词表和规则直接维护在 `../../okf-tag-generate/taxonomy/` 的唯一事实源文件中。
- provisional 文件只存在于对应的 `runs/<run-id>/`，不能传给 tag 写入器。
- 发布 registry 不在本目录自动切换；切换生产版本是单独的人工批准动作。

仓库中其他 taxonomy 文件只作历史兼容或迁移输入，不得继续作为新的修改来源。
