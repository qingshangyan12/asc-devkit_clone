# 字段规则

- `resource` 使用 `asc-devkit/` 加仓库相对 Markdown 路径。
- `description` 必须由 `okf-description-generate` 中的 Agent 基于 Markdown 正文生成，并附带逐字 evidence；总控脚本不得从正文、标题或路径提取、拼接或改写描述。
- `signature` 只记录正文、声明和证据能确认的集合；无法确认时为 `[]`。沿用旧生成器的 `identifier_field` 规则：仍保留文档占位符的标识符使用 `prototype_id`；没有文档占位符的具体重载使用 `override_id`。C++ 模板参数 `T`、`S`、`U` 不视为文档占位符；已证实的函数宏使用唯一字段 `macro_id`，且不得附加 archs 或 headers。同目录同 API 的 `_arch_<型号>` 文档若证实存在相同 concrete override_id，只给这些冲突项追加 `@型号`，例如 `...@2201`；该后缀不是新字段，且必须同步到 signature 和 marker。若记录命中同一物理 header 声明，Doxygen 继续按旧规则合并 canonical ID 与 archs。
- `dtype` 只在正文明确列出对应 `<dtype>` 可取类型时写入该 `signature` 条目，并与 `prototype_id` 同级；值为不重复的单行字符串数组，例如 `dtype: [half, float]`。不同原型的类型集合不得合并为根级 dtype。不得在函数原型前新增 `api_signature` YAML 围栏。
- `signature.headers` 只写唯一、参数兼容且范围内的真实声明 header。文档提示头文件时先查提示头，再查其递归 include 闭包；没有提示时按 API family 查找，最后才报告全量 include 的范围外候选。参数类型/数量不一致、同名多候选和范围外候选均不写入 YAML。
- `tag` 使用本 Skill 内冻结 taxonomy，主题每个只出现一次。`roles` 全局必填；compute、data_movement、
  data_initialization 角色要求 `operations`，compute 角色要求 `execution_models`。其他 namespace 按证据和检索用途选填，
  不适用字段省略，不用空数组伪造信息。每个非空值必须有 `namespace:value` 级逐字证据。
  reference.type.cpp 页仅 api_surfaces 必填，其余 namespace 有证据即填、无证据留空数组。
- `dependencies` 是唯一依赖字段；无明确关系写 `[]`，删除旧 `usage_dependencies`。
- `examples` 只记录真实存在且能关联到文档的样例；无法确认时为 `[]`。
- `headers`、`declaration`、`type_catalog` 只用于 `c_api/defs` 实体页，且必须完全来自本地解析器。
  非 callable 定义的 `headers` 是唯一验证过的 `/include/...` 声明文件；`type_catalog` 没有 header 要求。
  `declaration` 必须保留正文中可确认的 `symbol`、`cpp` 和成员/字段/值/别名目标；对象宏使用 `macros` 条目。
  函数宏只有 `signature[].macro_id`，不得附加 top-level `headers` 或 `declaration`。
- include 只允许新增或补齐 Doxygen 注释；记录必须以 header、函数/宏、带名称的参数原型和证据字段定位，不能提供 anchor 或渲染后的 comment。已验证的具体函数若有命名参数，必须从本页“参数说明”表为全部参数提取方向、描述和逐字表格 evidence；没有完整覆盖表或方向无法映射时必须报告 `include_doxygen` 问题，不能静默留下空 `doxygen`。不同物理声明保留架构消歧后的 `@型号`；同一物理声明的多架构记录合并为 canonical override_id 和汇总 archs。任何声明不一致都作为 source defect 报告。
- example marker 只允许用同一 concrete `override_id` 的成对 `//! [override_id]` 包裹实际调用所在行；架构消歧的 `@型号` 必须与 signature 一致。共享物理 include 声明仍按 canonical ID 合并。仍有占位符、非样例源码、范围重叠或歧义调用不写 marker，不修改调用或实现代码。
- 一次正常生成默认处理所有写入目标：front matter（含 tag、dependencies）、include Doxygen 和 example marker。不得为这些字段分别要求用户确认；只有明确 dry-run 才不写入。
