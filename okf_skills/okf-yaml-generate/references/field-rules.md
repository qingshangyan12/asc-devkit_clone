# 字段规则

## 公共字段

所有 Profile 只共享 `okftype`、`title`、`resource`、`timestamp`。字段是否可用由
`config/document-profile-registry.yaml` 的 Profile `fields` 决定，不能从其他 Profile 或旧 front matter 继承。

`resource` 必须是 `asc-devkit/` 加仓库相对 Markdown 路径；`timestamp` 必须是 ISO 日期。

## 普通 API

- `description` 必须来自已验证的 description artifact，并附逐字 evidence。
- `tag` 只能使用 `okf-tag-generate/taxonomy/tag-taxonomy.json` 中的值，必填和证据规则读取 `tag-rules.yaml`。
- `signature`、`header`、`declaration`、`type_catalog`、`dependencies` 和 `examples` 只在 Profile 声明且有证据时生成。
- `signature` 中的 `prototype_id`、`override_id`、`macro_id`、`dtype` 和 header 映射遵守生成器校验规则。

## Index

`reference.index.cpp` 只生成：

```text
okftype, title, resource, navigation, timestamp
```

Index 不生成 `description`、`tag`、`dependencies`、`examples`、`signature` 或 `header`。
`navigation` 是按 Markdown 正文出现顺序提取的子页面列表，每项包含 `path` 和 `title`。

## 生成边界

普通 API 缺少语义工件时阻断 validate；Index 不需要语义工件。任何 Profile 都不得生成注册表未声明的字段，
也不得通过空数组伪造证据。
