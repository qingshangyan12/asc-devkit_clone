# Worker 与 Resolver 快速协议

## Worker

worker 收到 task 后按以下顺序执行：

1. 先运行 `controller.py worker` 生成 scaffold，不手工重写整份 YAML。
2. description 只能读取通过 `okf-description-generate validate` 的 `description-result.json`；确认 document hash 一致后，
   将其中的 description 与 evidence 带入 worker-result。不得从标题、正文或路径重新撰写、提取或猜测 description。
3. scaffold 已按本地 vendored 规则提取函数原型、函数宏、真实 header、dtype、样例调用和 Doxygen 声明身份。`c_api/defs` 实体页改为提取 enum、record、constant、type alias、type catalog 或宏定义，不能以缺函数原型处理；目录索引页不进入批次。不要以标题或文件名重建 signature 或 declaration；仅补充脚本不能确认的语义字段和证据。signature 沿用旧生成器的 `identifier_field` 规则：保留文档占位符时写 `prototype_id`，具体重载写 `override_id`；C++ 模板参数 `T`、`S`、`U` 不视为文档占位符，函数宏使用独立 `macro_id`。正文明确的 dtype 列表只能以与其 `prototype_id` 同级的单行 `dtype: [...]` 写入对应 signature 条目，不能写为根级字段，也不得在原型前插入 `api_signature` YAML 围栏。
4. 使用 `rg` 定位，再用短范围读取；没有命中证据时不要读取全文或遍历整个 examples 目录。
5. 只有目标文档明确关联样例或依赖时，才读取对应的一个 README/源码片段。
6. 一次性提交一个 worker-result；不要为不同字段拆分 agent，也不要在结果之外写解释性长文本。

worker-result 必须包含字段、证据和问题；缺少有效 description 工件时，description 保持空值并记录问题。
tag、dependencies、doxygen 和 example_markers 都是默认处理字段；不能等待每个字段的额外写入许可。

`doxygen` 是声明记录数组，而不是文本补丁：每项必须包含 `source_document`、`header`、`function`、
`parameters`、`archs`、`template_parameters` 和可选 `macro_id`、`doxygen`。`doxygen` 只可提供带 evidence 的
`level`、`brief`、`params`，不得提交 `anchor`、`text` 或整段注释。

`example_markers` 是旧样例发现器输出的 example/call 记录数组。每个 call 使用 `file`、`call_start_offset`、
`call_end_offset`、`canonical_id` 或 `call_template` 及可选 `placeholder_bindings`；不得提交 marker ID、anchor 或
自由 comment。总控计算 concrete `override_id` 后生成成对 `//! [override_id]` 标签。

## Resolver

主控先执行：

```bash
python3 scripts/controller.py resolve-issues --run-dir <run-dir>
```

然后只把 `resolution-input.json` 的 JSON 内容交给一个 resolver agent，不把 run directory、DevKit 根目录、
worker-result 或其他源文件路径传给它。resolver 不重新读取完整 Markdown、头文件或样例，只根据问题中的
证据摘要给出一次性 decisions。只有证据互相冲突时，主代理才回读冲突位置并把补充摘要传给 resolver。

没有问题时不启动 resolver。resolver 完成后立即运行带 `--decisions` 的 `resolve-issues`，不要让 agent
再次复述文档或 write plan。
