# 语义取证代理与 Resolver 协议

## 语义取证代理（Semantic Agent）

主控按 task 包为每批启动一个语义取证代理，产出批次级 `semantic-input.json`
（契约见 [`semantic-contract.md`](semantic-contract.md)）。语义代理按以下顺序执行：

1. 逐篇读取 task documents 的正文（标题、功能说明、原型、参数说明、约束、示例），使用 `rg` 定位加
   短范围读取；先运行 `okf-description-generate` 流程（prepare → 撰写 → validate），只把通过官方
   validate 的 description 与 evidence 带入语义输入，不得从标题、正文或路径自行撰写、拼接或猜测。
2. tag **按需生成**：只为有逐字证据的 namespace 提供值；无证据的 namespace 直接省略，不写空数组，
   也不得包含 `api_surfaces`（controller 按路径自动注入）。必填性由 `config/tag-rules.yaml` 声明：
   roles 恒必填；roles 含 compute/data_movement/data_initialization 时 operations 条件必填；含
   compute 时 execution_models 条件必填。条件必填 namespace 证据不足时省略并依赖 scaffold issue 交
   resolver 决议。词表顶层 `namespace_order` 是命名空间注册顺序，不是填写要求。
3. `tag_evidence` 覆盖每个 `namespace:value`，`source_path` 为当前文档路径，`excerpt` 逐字摘自正文。
4. `dependencies` 仅在正文明确说明与另一 API 文档的关系时提供（requires/configures/constrained_by/
   related），每项带内联 `evidence{source_path, excerpt}`；互为"配对使用"的双向关系都用 related。
5. 一次性写出 `semantic-input.json`；`annotations` 与 task documents 精确对账，`document_sha256`
   使用 prepare 记录的输入哈希。

语义代理不得触碰 signature、doxygen、example_markers 等结构字段，不得写 DevKit 任何文件。

## 主控派发要点

主控给语义代理的派发提示词必须转述以下规则，不得出现"保留 9 个 namespace"、"保持空数组"类措辞：

- `tag` 按需生成：只含有逐字证据的 namespace；不适用 namespace 省略，不写空数组。
- `api_surfaces` 由 source-context 按路径注入，语义代理不填。
- 必填性以 `config/tag-rules.yaml` 为准（机器可读规则源）；无证据 namespace 由 scaffold issue
  交 resolver 决议，条件必填 namespace 允许决议后省略。

## 结构合并（Controller Worker）

主控运行纯脚本合并，语义代理不参与：

```bash
python3 scripts/controller.py worker \
  --task tmp/<run-id>/batches/batch-01/task.json \
  --semantic-input tmp/<run-id>/batches/batch-01/semantic-input.json \
  --output tmp/<run-id>/batches/batch-01/worker-result.json
```

- scaffold 由本地 vendored 规则提取函数原型、函数宏、真实 header、dtype、样例调用和 Doxygen 声明
  身份；`c_api/defs` 实体页走独立定义解析，不能以缺函数原型处理；目录索引页不进入批次。
- signature 沿用旧生成器的 `identifier_field` 规则：保留文档占位符时写 `prototype_id`，具体重载写
  `override_id`；C++ 模板参数 `T`、`S`、`U` 不视为文档占位符，函数宏使用独立 `macro_id`。同目录同
  API 的 `_arch_<型号>` 文档出现相同 concrete override_id 时，scaffold 只给重叠项追加 `@型号`，并同步
  该 ID 到 marker。正文明确的 dtype 列表只能以与其 `prototype_id` 同级的单行 `dtype: [...]` 写入对应
  signature 条目。
- 缺 `--semantic-input` 时结果标记 `semantic_verified: false`，`validate` 阻断；构造 worker-result
  的控制器单元测试必须显式置 `semantic_verified: true`。
- worker-result 的 `doxygen` 是声明记录数组，不是文本补丁；`example_markers` 复用样例调用记录，
  controller 严格生成成对 `//! [override_id]` 标签。二者均由结构解析产出，语义代理不参与。

## Resolver

主控先执行：

```bash
python3 scripts/controller.py resolve-issues --run-dir <run-dir>
```

然后只把 `resolution-input.json` 的 JSON 内容交给一个 resolver agent，不把 run directory、DevKit 根目录或
worker-result 传给它。resolver 不重新读取完整 Markdown、头文件或样例，只根据问题中的
证据摘要给出一次性 decisions。只有证据互相冲突时，主代理才回读冲突位置并把补充摘要传给 resolver。

没有问题时跳过 resolver。resolver 完成后立即运行带 `--decisions` 的 `resolve-issues`，不要让 agent
再次复述文档或 write plan。
