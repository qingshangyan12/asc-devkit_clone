# 唯一 Tag 词表

本目录是 `okf-tag-research` 的唯一词表事实源。调研、标注和检索流程都必须读取这里的
taxonomy/profile；不得在其他目录另建一份正在维护的词表。

## 当前基线

- `core-v8-frozen.json`：冻结的 tag taxonomy。
- `retrieval-profile-v7.json`：与该 taxonomy 成对使用的检索 profile。

两者必须作为一个不可拆分的版本对使用。taxonomy 中 `required: true` 的 namespace 是每个
API 标注都必须填写的概念维度；每个值还必须有目标 Markdown 正文中的逐字证据。

## 版本规则

- 这两个基线文件不原地修改。
- 人工批准后的新版本写入 `taxonomy/releases/`，文件名必须包含唯一 `run_id` 或显式版本号。
- provisional 文件只存在于对应的 `runs/<run-id>/`，不能传给 tag 写入器。
- 发布 registry 不在本目录自动切换；切换生产版本是单独的人工批准动作。

仓库中其他 taxonomy 文件只作历史兼容或迁移输入，不得继续作为新的修改来源。
