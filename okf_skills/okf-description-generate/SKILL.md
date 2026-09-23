---
name: okf-description-generate
description: 由 Agent 基于 Ascend DevKit API Markdown 正文生成、复核并写入可追溯的 OKF description。用户要求为单个 API 生成、补充、重写、校验或写入 description 时使用；不用于脚本自动摘要、tag、signature、依赖、示例或完整 YAML 生成。
---

# OKF API 描述生成

本 Skill 只处理一个 API Markdown 的 `description`。Agent 负责理解 API 语义并撰写描述；脚本只固定输入、
校验证据与文档哈希、生成写入计划和执行受控写入。脚本不得从标题、文件名、正则匹配的段落或既有 YAML
自行提取、拼接或改写 description。

## 输出要求

description 使用中文单段摘要，准确说明 API 的核心动作、处理对象和结果或关键约束。它必须：

- 只陈述目标 Markdown 正文直接支持的事实，不添加未说明的能力、dtype、架构、调用顺序或性能结论；
- 不是标题、文件名、函数名或正文的机械首行复制；
- 对每一项语义结论提供目标 Markdown 正文中的一至三条逐字 evidence；
- 无法可靠概括时输出 `unresolved`，不猜测。默认不能将 unresolved 结果写入文档。

允许阅读标题、功能说明、原型、参数说明、约束和示例章节来理解语义。相邻文档或头文件只可帮助消除歧义，
不能作为 description 的证据来源。

## 流程

1. 对用户给定的单个 API 执行 `prepare`。脚本创建带输入哈希的任务包，未修改 Markdown。
2. Agent 只读取任务包指定的 Markdown 正文，撰写 `description-result.json`。创建前必须阅读
   `references/artifact-contract.md`。
3. 执行 `validate`。它验证 JSON 结构、输入哈希和 evidence 逐字匹配；校验器不评价或生成摘要语义。
4. 执行不带 `--write` 的 `write`，检查 `write-plan.json`。通过后才执行带 `--write` 的相同命令。
   写入只修改现有 front matter 的顶层 `description`；若字段不存在则插入到 `resource` 后。它不会创建
   front matter，也不会改动正文、tag、signature、dependencies、examples 或 timestamp。

## 命令

```bash
python3 scripts/description.py prepare \
  --devkit-root /path/to/asc-devkit \
  --document docs/zh/api/SIMD-API/c_api/.../asc_example.md \
  --run-id asc-example-description \
  --run-dir tmp/okf-agent/asc-example-description

# Agent 写入 tmp/okf-agent/asc-example-description/description-result.json 后：
python3 scripts/description.py validate \
  --run-dir tmp/okf-agent/asc-example-description \
  --result tmp/okf-agent/asc-example-description/description-result.json

python3 scripts/description.py write \
  --run-dir tmp/okf-agent/asc-example-description \
  --result tmp/okf-agent/asc-example-description/description-result.json

python3 scripts/description.py write \
  --run-dir tmp/okf-agent/asc-example-description \
  --result tmp/okf-agent/asc-example-description/description-result.json \
  --write
```

`unresolved` 结果只保留问题工件。只有用户明确要求写空值时，才可在最后一条命令附加
`--allow-unresolved --write`。

## 与 YAML 总控的关系

`okf-yaml-generate` 的 scaffold 不再从正文自动生成 description。完整 YAML 流程需要 description 时，先运行
本 Skill，将经验证的 `description`、`evidence` 和文档哈希带入 worker artifact；缺少该工件时应报告
`description` 未决，而不是让总控脚本补写摘要。

## 强制约束

- 只接受用户指定的单个 API Markdown，路径必须在 `docs/zh/api/SIMD-API/` 下。
- `prepare` 之后目标文档发生任何改变，必须重新开始运行；不得复用旧 result。
- 不得用 API 名称、目录、产品支持表、头文件、样例注释或相邻文档作为 evidence。
- dry-run 未通过时，禁止使用 `--write`。默认拒绝写入空 description。
