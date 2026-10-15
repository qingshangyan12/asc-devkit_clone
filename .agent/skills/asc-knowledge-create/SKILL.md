---
name: asc-knowledge-create
description: OKF 建设侧入口：为 Ascend C API 文档建设知识资产。用户需要确定或扩展 tag 词表（双盲调研与人工冻结）、为文档写入 5 字段 slim OKF 头、为 include/examples 写入源码标注、把新 API 文档范围接入三段流水线，或按需使用独立检索评测功能时使用；检索查文档请用 asc-search。
---

# OKF 建设（asc-knowledge-create）

本 Skill 是建设侧唯一入口：三个流水线工作流（词表调研、打标写入、接入编排）串成建头全流程，
另有一个独立功能（检索评测，不参与流程编排）。字段契约唯一事实源是
[references/头字段与标注规范.md](references/头字段与标注规范.md)；各工作流细节见对应 reference，
本文件只做路由与全局约束。

## 工作流与独立功能

| 项 | 类型 | 何时使用 | 入口 |
| --- | --- | --- | --- |
| 词表调研 | 流水线 | 确定、扩展、审计或冻结 tag 词表 | [references/tag-research.md](references/tag-research.md) |
| 打标写入 | 流水线 | 为指定文档/目录生成、校验并写入 slim 头；为 include/样例补源码标注 | [references/yaml-generate.md](references/yaml-generate.md) |
| 接入编排 | 流水线 | 新 API 文档范围接入（调研→重标注→索引） | [references/maintain.md](references/maintain.md) |
| 检索评测 | 独立功能 | 建测试集、测检索效率与质量、基线与对照 | [references/evaluate.md](references/evaluate.md) |

索引构建与检索属于 asc-search，不在本 skill 内；存量头的漂移体检与修复属于
asc-knowledge-update；接入编排只负责分派与复核；检索评测按需单独使用，不被建设流程或接入编排引用。

## 全局约束

- **人工关口**：词表发布前必须人工批准；production 词表切换是单独的人工动作。
- **写入门禁**：validate 未通过不得写入；`provisional` 词表不得传给标注管线。
- **写入面**：仅选中文档的 front matter、include 声明上方 Doxygen、样例 marker；
  `docs/` 正文、样例代码主体不在写入面。
- **已有样例约束**：只为仓库已有样例加 marker 注释，不新建或扩写样例源码；
  API 没有原有样例时如实报告缺失，不把文档中的调用片段生成 `.asc` 文件。
- **include 功能摘要**：每个具体重载包含一句英文 `@brief`，依据正文功能与该重载的输出方式撰写，
  术语参考 `docs/en/`；摘要记录保留正文逐字证据。已有摘要保留，缺失摘要时不得写入该声明。
  Agent 记录格式和写入命令见 [references/source-annotation-contract.md](references/source-annotation-contract.md)。
- **唯一事实源**：字段与顺序以 [references/头字段与标注规范.md](references/头字段与标注规范.md) 为准；
  词表以 `config/tag-taxonomy.json` 为准；版本裁决以 `config/version-overrides.yaml` 为准。
- 运行目录每次新建，禁止复用他人工件；词表问题回流调研新轮次，不得临时创造未定义值。

## 快速命令

从本 skill 目录执行：

```bash
# 词表调研（prepare → 子代理发现 → 校验 → 裁决 → 冻结 → 人工批准）
python3 scripts/tag_research.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_datamove \
  --run-id <run-id> --output-dir runs/<run-id>

# 文档标注（prepare → 标注 → validate → write）
python3 scripts/controller.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_datamove \
  --run-dir tmp/<run-id>
python3 scripts/controller.py validate --run-dir tmp/<run-id>
python3 scripts/controller.py write --run-dir tmp/<run-id> --write
python3 scripts/controller.py check

# 源码标注（默认 dry-run）
python3 scripts/annotate_sources.py include --run-dir tmp/<run-id> [--write]
# include 执行前由 Agent 在该运行目录生成 include-briefs.json（逐重载英文摘要与证据）。
python3 scripts/annotate_sources.py examples [--write]
```

独立评测功能（不参与上述流水线，按需使用）：

```bash
python3 scripts/score.py --tasks testset/tasks.yaml \
  --answers baseline/results/answers.jsonl --out baseline/results/score.json
```
