---
name: okf-yaml-generate
description: 集中编排 Ascend DevKit API 的完整 OKF YAML 生成、并发取证、问题处理、Doxygen 注释和最终写入。用于指定 API 文件或目录；问题按文档聚合生成中文报告，先 dry-run 后写入，不依赖其他项目或旧 skill。
---

# OKF YAML 总控生成

本 Skill 是完整 OKF YAML 生成的唯一中控入口。所有脚本、词表、协议和报告格式都在本目录内，
不得在运行时导入 `api-yaml-metadata-guard`、旧 OKF skill、issue ledger、remedy library 或索引工具。原型 ID、
原型/宏解析、header 映射和样例 marker 的已验证实现已 vendoring 到本目录，必须以本地副本为准。
默认词表固定为冻结的 `taxonomy/releases/core-v9-frozen.json`；每次运行必须把实际解析的绝对路径写入
`manifest.json`，不得与旧版本混用。

## 处理范围

只接受以下 API Markdown 范围：

- `docs/zh/api/SIMD-API/c_api`
- `docs/zh/api/SIMD-API/basic_api`
- `docs/zh/api/SIMD-API/adv_api`
- `docs/zh/api/SIMD-API/tensor_api`

用户指定文件时只处理该文件；指定目录时递归处理该目录下的 Markdown。头文件和样例是只读证据，
include 只允许增加有证据的 Doxygen 注释，样例只允许增加 marker 注释。

函数原型和 header 必须按本 Skill 内的 legacy-compatible 解析器生成：它处理文档占位符、C++ 模板参数、
函数名占位符展开、函数宏、对象宏别名、头文件路径提示和聚合头 include 闭包。只有唯一且参数可兼容的真实
声明可以进入 `signature[].headers`；候选、多义或范围外结果只能进入问题报告。`c_api/defs` 的实体页走独立
定义解析：enum、struct/union、constant、`using` 类型别名、数据类型目录和宏定义分别产生受限的定义字段，
不因没有函数原型而降级。`defs.md` 及与目录同名的索引页不进入目录批处理。

同目录同 API 的 `_arch_<型号>` 文档会比较 concrete override_id；仅当不同型号文档确实包含相同具体原型时，
各冲突 ID 追加 `@型号`（如 `@2201`、`@3510`）。单文件运行也会只读检查该同组 sibling。该后缀不是字段，
并且必须在 YAML signature 和 example marker 中完全一致。多个架构页命中同一物理 header 声明时，include
Doxygen 仍按旧生成器合并为 canonical ID 和汇总的 `@archs`；不同物理声明才保留各自的 `@型号`。

## 总控流程

1. 执行 `prepare`，解析范围、计算输入 hash、建立批次和 worker 任务包。
2. 目录超过 25 个文档时按最多 25 个一批；每波最多 6 个 worker。worker 只读证据并输出结果，
   不得写 DevKit。
3. 主控汇总 worker 结果，执行一轮独立的问题处理 agent。问题处理 agent 只分析指定证据，
   输出分类和处理决定，不修改任何源文件。
4. 主控根据结果生成完整 write plan：基础 YAML、tag、dependencies、Doxygen 和 example marker。
5. 执行 `validate`。它必须校验所有路径、hash、证据、词表、字段、问题决定和写入边界，并完成
   所有阶段的 dry-run。
6. 只有 `validate` 成功后，使用相同 run directory 执行 `write`。写入前再次确认输入 hash 未变化。
7. 运行结束必须生成机器可读 `error-report.json` 和中文 `error-report.md`。同一文档的所有问题必须
   合并在同一个报告章节中。

## 临时目录和最终反馈

所有由 agent 执行的运行必须使用本 skill 的临时目录：`<skill-root>/tmp/<run-id>`，其中
`<skill-root>` 是 `SKILL.md` 所在目录。`<run-id>` 必须在当前运行中唯一；不得使用系统 `/tmp`
或 skill 目录外的其他目录作为 `--run-dir`。

错误报告固定输出为：

- `<skill-root>/tmp/<run-id>/error-report.json`
- `<skill-root>/tmp/<run-id>/error-report.md`

执行完 `validate` 后，agent 必须读取 `error-report.json`。若 `issues` 非空，最终答复必须说明问题数量，
并给出 `error-report.md` 的实际绝对路径；不得只笼统说明“存在问题”。

## 命令

从本 Skill 目录执行：

```bash
python3 scripts/controller.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_compute \
  --run-dir tmp/<run-id>

python3 scripts/controller.py worker \
  --task tmp/<run-id>/batches/batch-01/task.json \
  --output tmp/<run-id>/batches/batch-01/worker-result.json

python3 scripts/controller.py resolve-issues \
  --run-dir tmp/<run-id>

# 只把 resolution-input.json 的 JSON 内容交给一轮问题处理 agent 后，不能把 run-dir、DevKit 根目录或 worker-result 传给它：
python3 scripts/controller.py resolve-issues \
  --run-dir tmp/<run-id> \
  --decisions tmp/<run-id>/resolution-decisions.json

python3 scripts/controller.py validate \
  --run-dir tmp/<run-id>

python3 scripts/controller.py write \
  --run-dir tmp/<run-id>
```

主代理负责按 task 包启动 worker 和问题处理 agent；没有并发能力时按同一批次顺序执行。
`write` 不接受临时字段或新 artifact，只使用当前 run 的校验结果。

## 快速路径

- 单文档只启动一个 worker；不要为每个字段或每个问题单独启动 agent。
- worker 先运行 `controller.py worker` 生成 scaffold，再只补充缺失的语义字段和证据。
- 使用 `rg` 定位标题、功能说明、原型、约束和样例链接；只读取命中的局部区段，不要整篇重复读取。
- worker 完成后先运行 `resolve-issues` 生成紧凑的 `resolution-input.json`。问题处理 agent 只接收该文件内容，
  默认不给 resolver DevKit 根目录、run directory 或 worker-result 路径，不重新读取全文；只有证据冲突时才回读对应局部区段。
- 没有问题时跳过问题处理 agent，直接进入 validate。多个问题必须由同一轮 resolver 一次性处理。
- validate/write 是本地脚本操作，不需要再次让 agent 解释或复述整个文档。

## 字段和降级规则

基础 YAML 字段为 `okftype`、`title`、`resource`、`description`、`tag`、`signature`、
`dependencies`、`examples`、`timestamp`。`c_api/defs` 实体页还可有 source-derived 的 `headers`、
`declaration` 或 `type_catalog`；这些字段不由 worker 猜测，也不用于普通函数页。

问题只影响对应字段：

- description 无法确认：先运行 `okf-description-generate`；其 `unresolved` 结果才允许按决议写 `description: ""`，总控脚本不得自行从正文提取摘要。
- signature 无法确认：`signature: []`
- dtype 有正文明确类型列表时：仅写入对应 `prototype_id` 同级的单行数组，例如 `dtype: [half, float]`；
  不得写为根级字段，也不得在函数原型前的 YAML 代码块写 dtype。
- 非 callable 定义：只有正文能解析出声明且唯一 header 确认时才写 `declaration` 和 `headers`。
  `type_catalog` 不要求 header；函数宏继续使用独立 `signature[].macro_id`，对象宏使用 `headers` 和
  `declaration.macros` 表达。
- examples 无法确认：`examples: []`
- dependencies 无法确认：`dependencies: []`
- tag 某 namespace 无法确认：保留 namespace，值写空数组
- Doxygen 或 marker 无法确认：不写入该注释，并记录问题

Doxygen 记录不接受 `path/anchor/text` 自由补丁。主控按结构化声明身份写入或补齐
`@ingroup`、`@override_id`（宏为 `@macro_id`）和 `@archs`，再只追加缺失且带证据的
`@brief`、`@param`。已验证的具体函数即使参数说明表不完整，也必须写入可唯一确认的基础关联标签；
仅为具备方向、描述和表格 evidence 的参数追加 `@param`。缺失参数覆盖或方向无法映射时必须报告
`include_doxygen`，但不得抑制已验证基础标签或其他已取证参数。example marker 不接受自由 comment 文本，严格使用旧生成器的成对
`//! [override_id]` 格式包裹实际调用所在行。

文档错误不会导致整篇 YAML 跳过。头文件、样例或实现代码问题只能报告，不能修改源代码。
正常文档的必填 tag namespace 不允许为空；只有该 namespace 存在对应 issue、经过 resolver 记录并进入报告后，
才允许定向置空。每个非空 tag 值必须有 `namespace:value` 级逐字证据。

## 错误报告

每次运行输出：

- `manifest.json`：范围、批次和输入 hash
- `issues.json`：结构化问题
- `resolution-decisions.json`：问题处理 agent 决定
- `write-plan.json`：最终写入计划
- `error-report.json`：完整机器报告
- `error-report.md`：按文件聚合的中文报告

中文报告必须明确列出文件、具体问题、证据、处理决定、实际降级字段和是否写入。问题类型为
`document_defect`、`source_defect`、`tool_defect` 或 `unresolved`。

## 写入边界

主控是唯一写入者。用户请求生成或更新时，默认写入范围固定为 API front matter（含 tag、dependencies）、
include Doxygen 注释和 example marker；不得逐字段再次请求写入许可。只有用户明确要求 dry-run 或禁止写入时，
才不执行 `write`。它会移除先前生成的、仅含 `api_signature` 的 YAML 围栏。除该迁移外，禁止修改 Markdown
正文、声明、参数、类型、属性、实现代码、样例实现、索引和旧项目问题资产。
任何阶段 dry-run 失败都禁止写入。

详细 artifact 和问题处理约定见：

- [`references/artifact-contract.md`](references/artifact-contract.md)
- [`references/issue-resolution.md`](references/issue-resolution.md)
- [`references/field-rules.md`](references/field-rules.md)
- [`references/worker-protocol.md`](references/worker-protocol.md)
