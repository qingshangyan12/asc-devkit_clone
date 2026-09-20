# 字段规则

- `resource` 使用 `asc-devkit/` 加仓库相对 Markdown 路径。
- `description` 必须由 `okf-description-generate` 中的 Agent 基于 Markdown 正文生成，并附带逐字 evidence；总控脚本不得从正文、标题或路径提取、拼接或改写描述。
- `signature` 只记录正文、声明和证据能确认的集合；无法确认时为 `[]`。沿用旧生成器的 `identifier_field` 规则：仍保留文档占位符的标识符使用 `prototype_id`；没有文档占位符的具体重载使用 `override_id`。C++ 模板参数 `T`、`S`、`U` 不视为文档占位符；已证实的函数宏使用唯一字段 `macro_id`，且不得附加 archs 或 headers。
- `dtype` 只在正文明确列出对应 `<dtype>` 可取类型时写入该 `signature` 条目，并与 `prototype_id` 同级；值为不重复的单行字符串数组，例如 `dtype: [half, float]`。不同原型的类型集合不得合并为根级 dtype。不得在函数原型前新增 `api_signature` YAML 围栏。
- `signature.headers` 只写唯一、参数兼容且范围内的真实声明 header。文档提示头文件时先查提示头，再查其递归 include 闭包；没有提示时按 API family 查找，最后才报告全量 include 的范围外候选。参数类型/数量不一致、同名多候选和范围外候选均不写入 YAML。
- `tag` 使用本 Skill 内冻结 taxonomy，主题每个只出现一次；正常文档的必填 namespace 必须非空，
  只有经过 issue/resolution 记录的字段才允许写 `[]`。每个非空值必须有 `namespace:value` 级逐字证据。
- `dependencies` 是唯一依赖字段；无明确关系写 `[]`，删除旧 `usage_dependencies`。
- `examples` 只记录真实存在且能关联到文档的样例；无法确认时为 `[]`。
- `api_kind`、`headers`、`declaration`、`type_catalog` 只用于 `c_api/defs` 实体页，且必须完全来自本地解析器。
  `api_kind` 只能是 `constant`、`enum`、`struct`、`union`、`type_alias`、`type_catalog`、`function_macro` 或 `macro`。
  非 callable 定义的 `headers` 是唯一验证过的 `/include/...` 声明文件；`type_catalog` 没有 header 要求。
  `declaration` 必须保留正文中可确认的 `symbol`、`cpp` 和成员/字段/值/别名目标；对象宏使用 `macros` 条目。
  函数宏只有 `signature[].macro_id`，不得附加 top-level `headers` 或 `declaration`。
- include 只允许新增或补齐 Doxygen 注释；记录必须以 header、函数/宏、带名称的参数原型和证据字段定位，不能提供 anchor 或渲染后的 comment。任何声明不一致都作为 source defect 报告。
- example marker 只允许用同一 concrete `override_id` 的成对 `//! [override_id]` 包裹实际调用所在行；仍有占位符、非样例源码、范围重叠或歧义调用不写 marker，不修改调用或实现代码。
- 一次正常生成默认处理所有写入目标：front matter（含 tag、dependencies）、include Doxygen 和 example marker。不得为这些字段分别要求用户确认；只有明确 dry-run 才不写入。
