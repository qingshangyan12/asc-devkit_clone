---
name: okf-tag-research
description: 自动完成 Ascend DevKit API tag 词表调研，包括两个独立发现子代理、独立检索金标准、确定性校验、候选 taxonomy/profile 生成和最终人工审批。用户要求确定、扩展、审计、冻结或发布 API tag 词表，或要求自动化 tag 确定流程时使用；仅为既有冻结词表给文档写 tag 时不使用。
---

# OKF Tag 调研

本 Skill 用于确定或扩展受控 tag 词表，不向 DevKit Markdown 写入 tag。词表获得批准后，才可使用
`annotate-api-tags` 或项目 OKF 工作流完成逐篇标注和写入。

## 唯一词表来源

`taxonomy/core-v8-frozen.json` 与 `taxonomy/retrieval-profile-v7.json` 是本 Skill 当前唯一维护的
taxonomy/profile 基线。所有运行默认读取这两个文件；不得在项目仓库或其他目录另建一份并行维护的
词表。批准后的新版本只能写入本目录的 `taxonomy/releases/`，基线文件不得原地覆盖。

taxonomy 中标记为 `required: true` 的 namespace 是强制概念维度。每一个最终 API 标注必须为每个
强制 namespace 至少选择一个值，并为每个值提供目标 Markdown 正文中的逐字证据；缺少任一强制
概念时，标注校验和写入都必须失败。

## 人工关口

仅要求人工参与两次：

1. 执行 `prepare` 前，确认精确 DevKit 范围、检索目标、基础 taxonomy/profile 和候选发布名称。
2. 生成发布包后，明确批准或拒绝候选 taxonomy/profile。批准前不得更新 release registry、写入 DevKit Markdown 或生成 DevKit tag 索引。

不得以近期修改的文档代替已指定范围。不得混用不同发布版本的 taxonomy 和 retrieval profile。

## 执行流程

每次运行都在 `okf-skills/okf-tag-research/runs/` 下使用新的空目录。该目录是审计记录，禁止复用其他运行的工件。

1. 以审批的范围、唯一词表基线和明确的 `--run-id` 执行 `scripts/tag_research.py prepare`。脚本固化文档哈希并生成三份任务包。
2. 用 `fork_turns="none"` 启动三个全新子代理：`discovery-a`、`discovery-b` 和 `retrieval-gold`。每个子代理只能得到自己的任务包、输出位置和 DevKit 原文根目录；不得透露其他子代理输出、候选列表或历史 tag。提示词见 `references/worker-protocol.md`。
3. 分别执行 `validate-discovery` 和 `validate-gold`。任何校验失败即停止本轮；源文档缺失或哈希变化同样使本轮无效。
4. 执行 `summarize-discovery`。此时主代理才可读取两份已校验发现结果、汇总、原文和已校验检索金标准，并按 `references/artifact-contract.md` 写入 `main-agent-review.json`。主代理只能批准两个发现子代理均支持的候选；定义、选择规则、原文证据或检索价值不明确时必须 `defer`。
5. 执行 `validate-review`、`freeze-provisional` 和 `build-release-bundle`。产出的 provisional taxonomy/profile 对不能被 tag 写入器使用。
6. 仅向人工审批者展示发布包和 proposed taxonomy/profile diff。收到 approval record 后运行 `approve`，默认将新的冻结 taxonomy/profile 对写入本 Skill 的 `taxonomy/releases/`。结果仍是候选发布工件，绝不自动切换生产 release。

## 子代理隔离

独立性依赖上下文隔离和工件隔离。运行环境支持时，为每个子代理使用独立文件系统 sandbox。共享工作区环境中，`fork_turns="none"` 与分开的输入/输出目录只构成逻辑隔离，无法阻止子代理主动读取其他文件；必须在 run manifest 中记录 `isolation_level: logical_shared_workspace`，并且不得在发布说明中将它描述为严格盲审。

主代理必须等待三个子代理全部结束且源文件哈希已校验后，再读取任何输出。主代理是规则化裁决者，不是第三个独立发现者。

## 命令

从本 Skill 目录执行，或为每个路径提供绝对路径。

```bash
python3 scripts/tag_research.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_compute \
  --guard-repo /path/to/api-yaml-metadata-guard \
  --run-id vector-compute-v1 \
  --output-dir runs/vector-compute-v1

python3 scripts/tag_research.py validate-discovery \
  --run-dir runs/vector-compute-v1 --worker discovery-a
python3 scripts/tag_research.py validate-discovery \
  --run-dir runs/vector-compute-v1 --worker discovery-b
python3 scripts/tag_research.py validate-gold \
  --run-dir runs/vector-compute-v1
python3 scripts/tag_research.py summarize-discovery \
  --run-dir runs/vector-compute-v1
python3 scripts/tag_research.py validate-review \
  --run-dir runs/vector-compute-v1
python3 scripts/tag_research.py validate-annotation \
  --run-dir runs/vector-compute-v1 \
  --annotations /path/to/annotations.json
python3 scripts/tag_research.py freeze-provisional \
  --run-dir runs/vector-compute-v1
python3 scripts/tag_research.py build-release-bundle \
  --run-dir runs/vector-compute-v1
```

人工根据契约创建 `approval.json` 后，仅冻结已批准的 proposal：

```bash
python3 scripts/tag_research.py approve \
  --run-dir runs/vector-compute-v1 \
  --approval runs/vector-compute-v1/approval.json
```

如需审计或迁移，可显式传入 `--taxonomy` 和 `--profile`；正常运行不要替换 Skill 内置基线。
`approve` 省略输出路径时，会写入 `taxonomy/releases/`，并拒绝覆盖已有文件。

主代理创建 review 或 approval 文件前，必须阅读 `references/artifact-contract.md`；启动子代理前，必须阅读 `references/worker-protocol.md`。

## 强制约束

- Markdown 正文是证据源；路径只能支持 API surface 或导航事实。
- 只能在基础 taxonomy 已有 namespace 下提出新 value。
- 不得把生命周期步骤、参数绑定、模板、实现细节、架构细节、dtype 或仅 API 名称提升为检索 tag。
- 调用顺序、条件前置和同步关系属于 dependency/index 资产，不得伪装为无条件文档 tag。
- `provisional` 不等于 `frozen`，不得传给 tag 写入器。
- 任一检查失败都必须重新产出相应工件并校验；不得编辑校验报告或人工标记门禁通过。
