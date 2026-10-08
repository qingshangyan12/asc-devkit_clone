# Tag 词表调研工作流

对 API 文档、include 头文件、样例三类源开展双盲发现与确定性校验，产出并冻结纯语义词表
（roles/operations/traits）。

本工作流确定或扩展受控 tag 词表，不向 DevKit Markdown 写入 tag。词表获得批准后，由
本 skill 打标写入工作流（[yaml-generate.md](yaml-generate.md)）从冻结词表读取值、基于正文证据逐篇标注并写入 YAML 头。

字段与标注边界以 [头字段与标注规范.md](头字段与标注规范.md) 为唯一事实源：

- 文档头 tag 仅保留 `roles/operations/traits` 三个语义 namespace；
- **不得为可推导事实建标签**——[头字段与标注规范.md](头字段与标注规范.md) 第 1 节与
  `taxonomy/tag-rules.yaml` 的 `derivation_boundary` 列出的推导维度
  （api 层级、执行模型、数据位置、dtype、架构、签名、掩码、废弃、样例、导航、版本等）
  一律不得进入词表；
- 词表中的值必须是脚本无法从三类源（API 文档、include、examples）推导的语义概念。

## 词表来源

冻结词表的生产文件是本 skill 配置 `config/tag-taxonomy.json`，本 Skill 是词表唯一事实源与
唯一写入方（调研 `approve` 后人工切换生产版本）。`taxonomy/tag-rules.yaml` 配套维护必填与
证据规则，`taxonomy/releases/` 保存冻结发布审计。标注管线从本 skill 配置读取，
不得另建并行词表。

## 人工关口

1. 执行 `prepare` 前，确认精确 DevKit 范围、调研目标、基础 taxonomy 和候选发布名称。
2. 生成发布包后，明确批准或拒绝候选 taxonomy。批准前不得更新 release registry、
   写入 DevKit Markdown 或生成 tag 索引。

不得以近期修改的文档代替已指定范围，不得混用不同发布版本的 taxonomy。

## 执行流程

每次运行在 `runs/<run-id>/` 下使用新的空目录，禁止复用其他运行的工件。

1. 以审批的范围、词表基线和 `--run-id` 执行 `scripts/tag_research.py prepare`。
   脚本固化文档哈希并生成两份任务包；任务包的 `sources` 指明三类源目录
   （docs、include/c_api、examples/02_simd_c_api/03_c_api）。
2. 用 `fork_turns="none"` 启动两个全新子代理 `discovery-a`、`discovery-b`。提示词见
   [子代理协议](references/worker-protocol.md)；子代理按三源调研并产出候选值。
3. 执行两次 `validate-discovery`。任何校验失败即停止本轮；源文档缺失或哈希变化同样使本轮无效。
4. 执行 `summarize-discovery`。主代理读取两份已校验发现，按
   [工件契约](references/artifact-contract.md) 写入 `main-agent-review.json`；
   只能批准两个子代理均支持且不属可推导维度的候选。
5. 执行 `validate-review`、`freeze-provisional`、`build-release-bundle`。provisional taxonomy
   不能被 tag 写入器使用。
6. 仅向人工审批者展示发布包与 taxonomy diff。收到 approval record 后按批准结果冻结发布
   （产物写入 `taxonomy/releases/`），生产词表 `../config/tag-taxonomy.json` 的切换是
   单独的人工批准动作。

## 子代理隔离

独立性依赖上下文隔离与工件隔离。共享工作区环境中 `fork_turns="none"` 与分开的输入/输出目录
只构成逻辑隔离，run manifest 记录 `isolation_level: logical_shared_workspace`，不得描述为严格盲审。
主代理必须等待两个子代理全部结束且源文件哈希校验后再读取任何输出；主代理是规则化裁决者。

## 命令

从本 Skill 目录执行：

```bash
python3 scripts/tag_research.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_datamove \
  --run-id <run-id> --output-dir runs/<run-id>

python3 scripts/tag_research.py validate-discovery --run-dir runs/<run-id> --worker discovery-a
python3 scripts/tag_research.py validate-discovery --run-dir runs/<run-id> --worker discovery-b
python3 scripts/tag_research.py summarize-discovery --run-dir runs/<run-id>
python3 scripts/tag_research.py validate-review --run-dir runs/<run-id>
python3 scripts/tag_research.py freeze-provisional --run-dir runs/<run-id>
python3 scripts/tag_research.py build-release-bundle --run-dir runs/<run-id>
python3 scripts/tag_research.py approve --run-dir runs/<run-id> --approval runs/<run-id>/approval.json
```

如需审计或迁移，可显式传入 `--taxonomy`；正常运行不要替换 Skill 内置基线。
标注覆盖校验使用 `validate-annotation --run-dir ... --annotations <annotations.json>`。

主代理创建 review 或 approval 文件前，必须阅读 `references/artifact-contract.md`；
启动子代理前，必须阅读 `references/worker-protocol.md`。

## 强制约束

- Markdown 正文是证据源；路径只能支持 API 层级或导航事实。
- 只能在基础 taxonomy 已有 namespace 下提出新 value；namespace 集合固定为
  `roles/operations/traits`，新增 namespace 需先修订[头字段与标注规范.md](头字段与标注规范.md)。
- 可推导维度、生命周期步骤、参数绑定、模板、实现细节、架构细节、dtype 或仅 API 名称
  不得提升为检索 tag。
- 调用顺序、条件前置和同步关系属于 dependency/index 资产，不得伪装为无条件文档 tag。
- `provisional` 不等于 `frozen`，不得传给 tag 写入器。
- 任一检查失败都必须重新产出相应工件并校验；不得编辑校验报告或人工标记门禁通过。
