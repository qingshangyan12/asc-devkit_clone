# 检索评测工作流（阶段1初版）

> **独立功能**：本功能不参与建设/接入流程编排（词表调研、打标写入、接入编排均不引用本功能），
> 按需单独执行。测试集与基线为历史存档，复测按当前诉求自行组织。

## 职责

建立代表性测试集并测量检索路径的效率与质量：阶段1 在完全无 OKF、无索引的原始文档上建立基线；阶段7 使用同一测试集与口径完成增强方案对照。核心效率指标为**任务耗时**与 **token 消耗**，质量指标（查全、误报、缺口正确性）作为"质量不降低"门槛。

## 资产

| 文件 | 说明 |
| --- | --- |
| `评测口径.md` | 指标定义、隔离约束、测量流程、对照判据（必须先读） |
| `testset/tasks.yaml` | 20 个任务 + gold + 样例反查依据（file:line） |
| `scripts/collect_metrics.py` | 从 opencode 会话存储采集单任务 token/耗时/审计数据 |
| `scripts/score.py` | 对照 gold 自动评分（E 类缺口正确性需人工终审） |
| `baseline/` | 基线原始结果、逐任务指标、基线报告 |
| `reports/` | 增强检索评测报告；原始运行产物保留在对应检索 skill 的 `runs/` 目录 |

## 运行流程（基线条件）

1. **前置检查**：确认范围内文档无 YAML 头（`head -1` 抽查 + 全量扫描 0/140）；记录 `git rev-parse --short HEAD`、模型名、日期。
2. **逐任务执行**（并行度 = 1，顺序 A1→E3）：
   1. 记录起始时间戳：`date +%s%3N`。
   2. 派发 explore subagent，提示词使用下方模板 v2，`{task_prompt}` 取自 tasks.yaml 对应任务。
   3. 子代理返回后记录结束时间戳：`date +%s%3N`。
   4. 保存子代理最终回复到 `baseline/results/<ID>.answer.md`，并追加一行到 `baseline/results/answers.jsonl`：`{"id": "<ID>", "answer": "<全文>"}`。
   5. 采集指标：`python3 scripts/collect_metrics.py --start-ms <起> --end-ms <止> --task-id <ID> --out baseline/results/<ID>.metrics.json`。
3. **合规审计**：检查各任务 metrics.json 的 `isolation_ok`（白名单：两个文档目录 ∪ impl ∪ include ∪ /tmp）；出现范围外访问的任务结果单列，复测一次并标注。
4. **评分**：`python3 scripts/score.py --tasks testset/tasks.yaml --answers baseline/results/answers.jsonl --out baseline/results/score.json`；对 `gap_pending_manual` 与误报项人工终审。
5. **报告**：汇总生成 `baseline/基线报告.md`（逐任务表 + 分组统计 + 审计结论 + 异常标注）。

## 子代理提示模板 v2（不得改动，阶段7对照复用）

```text
你是一个 AscendC C API 检索助手。请在指定语料范围内完成以下算子需求的 API 检索任务。

【任务需求】
{task_prompt}

【检索语料】
允许访问以下位置的内容：
- API 文档（检索与计分主目标）：以下两个目录及其子目录中的 Markdown 文档
  - /mnt/workspace/asc-devkit/docs/zh/api/SIMD-API/c_api/vector_datamove
  - /mnt/workspace/asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute
- 源码（用于核实 API 声明、参数与重载）：
  - /mnt/workspace/asc-devkit/include 及其子目录
  - /mnt/workspace/asc-devkit/impl 及其子目录

【要求】
1. 只使用检索与阅读工具（glob/grep/read），不修改任何文件，也不访问检索语料之外的任何文件（包括 examples/、okf-skills/、docs 下其他文档目录）。注意：glob/grep/read 以及 bash 中 ls/rg/find 等命令的路径参数，必须以上述四个语料目录之一（两个文档目录、include、impl）或其子路径为前缀；禁止使用仓库根目录（/mnt/workspace/asc-devkit）、docs/zh/api/SIMD-API/c_api 等任何上层目录或范围外目录作为搜索或访问路径。
2. 从需求出发定位需要的 C API，候选可以是 API 文档，也可以是源码头文件/实现文件；优先给出最直接记载该 API 用法与约束的文档。
3. 若某项能力在两个文档目录内找不到对应文档，必须如实说明"文档范围内未覆盖"，不要猜测或编造路径；若源码中存在该 API 的声明，可以指出源码位置作为线索。
4. 最终回复请严格使用以下固定格式，不要添加其他内容：

## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/c_api/xxx.h 或 impl/c_api/xxx.h | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

### 模板版本记录

| 版本 | 变更 | 原因 |
| --- | --- | --- |
| v1 | 语料仅两个文档目录 | 阶段1初版（试运行 A1/A2/E1，已归档 baseline/results/v1-archive/） |
| v2 | 语料扩展 impl/、include/ 整树；候选允许源码路径；计分语料仍为两个文档目录 | 用户决策：检索知识时允许访问 impl 与 include 内容 |
| v2.1 | 要求 1 追加：grep/glob/bash 的搜索路径必须限定在语料目录内，禁止以仓库根目录为搜索起点 | B1 两次运行均出现仓库根目录 grep（run2 输出泄漏 examples/、tests/ 路径） |
| v2.2 | 要求 1 强化为四目录白名单前缀规则，明确禁止 c_api 等上层目录 | B2、B3 出现以 docs/zh/api/SIMD-API/c_api 上层目录为 grep 路径的系统性违规 |

模板版本执行记录：A1–A8 使用 v2；B1 使用 v2（run1 扫描越界、run2 泄漏，均归档）后以 v2.1 复测；B2 使用 v2.1（run1 泄漏归档）后复测合规；B3 使用 v2.1（run1 泄漏归档）后以 v2.2 复测；B4–E3 及阶段7对照统一使用 v2.2。版本间差异仅为隔离约束的执行性澄清，不改变任务、语料与评分口径；阶段7对照时如复测历史任务须用 v2.2 重跑。

## 阶段7对照约定（预告）

- 增强组使用同一 tasks.yaml、同一模板 v2（检索语料表述不变）、同一模型与运行条件；唯一差异是知识资产（OKF YAML 头 + 持久化索引 + asc-search 检索流程）可用。
- 对照结果按任务、按类别分组报告耗时差值与 token 差值；质量门槛（文档必选查全率、缺口正确性）不达标项先回流对应建设 skill（词表、YAML 头或索引）修正，再判断效率收益；能力覆盖（文档或源码命中）作为辅助指标单列。
