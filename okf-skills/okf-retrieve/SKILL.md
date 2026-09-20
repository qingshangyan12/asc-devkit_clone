---
name: okf-retrieve
description: 从用户算子需求出发，经 okf-index 持久化索引定位分阶段候选 C API 文档。索引优先、按需核对、如实标注范围外缺口；输出对齐评测口径的固定答案格式。
---

# OKF 知识检索

本 Skill 是阶段 5 交付物：打通「算子需求 → 索引查询 → 分阶段候选文档」链路，
验证阶段 4 索引可被日常检索直接复用。检索入口是 `okf-index/scripts/query.py`，
本 Skill 只定义检索流程与输出规范，不新建索引数据。

## 检索流程（对齐流程与架构说明书第 5 节）

1. **理解需求**：判断算子类别（逐元素/归约/比较/转换/搬运…）、计算意图、数据类型、
   对齐与掩码等约束；对会影响候选选择的缺失信息记为待确认事项。
2. **拆解阶段**：把需求映射为搬入（in）/计算（compute）/搬出（out），
   另识别配置类配套（掩码、填充、量化参数）与同步关注点。
3. **查询索引**：先 `query.py` 取得分阶段候选，再沿依赖图展开配套 API；
   常用维度映射：

   | 需求信号 | 查询 |
   | --- | --- |
   | 搬入 / 搬出 | `--stage in[,out]` |
   | 计算意图 | `--operation add,divide,leaky_relu,reduction,…` |
   | 类型转换 | `--convert <src> <dst[,dst2]>`（dtype 别名 f32/fp16/bf16 可用） |
   | 能力关键词 | `--kw 非对齐,填充,…`（title+description） |
   | 某 API 全貌 | `--api <name>`（主页+架构变体+分类页） |
   | 配套配置 | `--deps-of <api> [--kind requires|configures] [--reverse]` |
   | 同步等范围外能力 | `--gap 同步,寄存器,…`（命中即范围内无文档+范围外线索） |

4. **按需核对**：只读候选文档中支撑推荐所需的章节（功能说明/约束说明），
   区分「已确认适用 / 条件待确认 / 范围内未覆盖」；禁止全量精读。
5. **输出结果**：按固定格式给出候选（路径 + API 名 + 一句话用途）与未覆盖说明；
   用户主动询问后再展开接口细节或算子实现。

## 检索策略要点（阶段5运行实测提炼）

- **搬运家族保留**：需求出现"对齐搬运/对齐搬入搬出"字样时，`asc_copy_*` 与 `asc_copy_*_align`
  两个家族应同时保留为候选（除非需求明确排除）：`_align` 家族承载"对齐搬运"语义（支持非
  32B 对齐长度并自动补齐），官方对齐类示例普遍采用；只报标准家族会漏 gold（stage5-v1 的
  A2/A3 唯一质量缺口即此因）。两家族在 `--stage in,out` 视图中本就同时返回。
- **缺口判断首选 `--gap`**：能力疑似范围外时先查缺口登记（支持逗号多关键词），命中即可
  直接给出"范围内无文档"+范围外线索（E3 单题 28.7s/-83% 耗时）。
- **按需核对**：索引候选只读"功能说明/约束说明"章节核实；源码（include/impl）仅用于
  核对原型与重载；不做全量精读（stage5-v1 平均读文档 4.3 篇/题，基线 5.6 篇/题）。

## 输出格式（与评测口径对齐，不得增删）

```
## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/... | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

## 检索子代理提示模板 v5.0-retrieve（阶段5测试与阶段7对照复用）

基线模板 v2.2 的任务、语料与输出格式不变，仅追加索引资产与「索引优先」要求：

```text
你是 AscendC C API 检索助手。请完成以下算子需求的 API 检索任务。

【任务需求】
{task_prompt}

【检索资产】
1. 持久化索引（必须最先使用）：在仓库根目录执行
   python3 /mnt/workspace/asc-devkit/okf-skills/okf-index/scripts/query.py <查询>
   常用查询：--stage in,out,compute（分阶段候选）；--operation add,divide,…（计算意图）；
   --kw 非对齐,填充,…（能力关键词）；--convert half float（类型转换对）；
   --api asc_copy_gm2ub（某 API 的主页/架构变体/分类页）；--deps-of <API>（配套配置）；
   --gap 同步,寄存器,取整（范围外能力缺口，命中即“范围内无文档”+范围外线索）。
   加 --json 可得机器可读输出。索引数据：okf-skills/okf-index/index/{index.json,gaps.json}。
2. API 文档（检索与计分主目标）：
   - /mnt/workspace/asc-devkit/docs/zh/api/SIMD-API/c_api/vector_datamove
   - /mnt/workspace/asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute
3. 源码（核实 API 声明、参数与重载）：/mnt/workspace/asc-devkit/include、/mnt/workspace/asc-devkit/impl
4. 仓库其余内容（如 examples/）可按需读取；但禁止访问 okf-skills/ 下除
   okf-index/scripts/query.py 与 okf-index/index/ 之外的任何内容（评测资产隔离）。

【要求】
1. 必须先用 query.py 查询索引获得分阶段候选，再按需读取候选文档的关键章节核实用途与
   约束；不做全量精读。
2. 从需求出发定位需要的 C API，候选可以是 API 文档，也可以是源码头文件/实现文件；
   优先给出最直接记载该 API 用法与约束的文档。
3. 若某项能力在两个文档目录内找不到对应文档，必须如实说明“文档范围内未覆盖”，
   不要猜测或编造路径；索引缺口登记（--gap）命中时可引用其范围外线索。
4. 最终回复请严格使用以下固定格式，不要添加其他内容：

## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/c_api/xxx.h 或 impl/c_api/xxx.h | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

## 阶段5测试运行记录

- 原始运行目录：`runs/stage5-v1/`（answers/、metrics、score、timestamps.tsv）；评测报告归档在
  `../okf-evaluate/reports/stage5-v1.md`。执行方式对齐基线：
  explore 子代理逐任务派发、并行度 = 1、同一 tasks.yaml 与输出格式。
- 与基线的差异仅检索资产可用（索引 + front matter + 本 skill 模板）；
  读取范围按用户决策放开（可读 examples/ 等），但 okf-evaluate/ 与 okf-index/tests/
  （含 dryrun 查询清单）对检索代理隔离，防止 gold 泄漏。
- 指标采集复用 okf-evaluate/scripts/collect_metrics.py；其隔离白名单审计按基线口径
  仍会标记 examples/、okf-skills/ 访问，增强组属预期行为，报告中单列不判违规。
