---
name: okf-retrieve
description: 从用户算子需求出发，经 okf-index 持久化索引定位分阶段候选 C API 文档（范围：整个 c_api 目录，800 篇）。索引优先、按需核对、如实标注 c_api 范围外缺口；输出对齐评测口径的固定答案格式。
---

# OKF 知识检索

本 Skill 打通「算子需求 → 索引查询 → 分阶段候选文档」链路，验证 okf-index 索引可被日常检索直接复用。
检索入口是 `okf-index/scripts/query.py`，本 Skill 只定义检索流程与输出规范，不新建索引数据。

## 检索范围

整个 `docs/zh/api/SIMD-API/c_api`（800 篇）：vector/reg 双流水计算与搬运、cube 矩阵计算与搬运、
sync 同步、atomic 原子、spr、cache_ctrl、utils、scalar_compute、defs 类型/枚举/宏/常量/联合定义页、
deprecated_interface 废弃接口及顶层说明页。"文档范围内"= c_api 目录内。

## 检索流程（对齐流程与架构说明书第 5 节）

1. **理解需求**：判断算子类别（逐元素/归约/比较/转换/搬运/矩阵乘/同步/原子…）、计算意图、数据类型、
   对齐与掩码等约束；对会影响候选选择的缺失信息记为待确认事项。
2. **拆解阶段**：把需求映射为搬入（in）/计算（compute）/搬出（out），
   另识别配置类配套（掩码、填充、量化参数、矩阵模式）与同步关注点。
3. **查询索引**：先 `query.py` 取得分阶段候选，再沿依赖图展开配套 API；
   常用维度映射：

   | 需求信号 | 查询 |
   | --- | --- |
   | 搬入 / 搬出 | `--stage in[,out]`（GM→UB/L0/L1、UB↔矢量寄存器 load/store 均已覆盖） |
   | 计算意图 | `--operation add,divide,matrix_multiply_accumulate,synchronization,…` |
   | 类型转换 | `--convert <src> <dst[,dst2]>`（dtype 别名 f32/fp16/bf16/e4m3/hif8 可用） |
   | 能力关键词 | `--kw 非对齐,填充,矩阵,…`（title+description） |
   | 某 API 全貌 | `--api <name>`（主页+架构变体+家族成员，含 deprecated 散置变体） |
   | 配套配置 | `--deps-of <api> [--kind requires|configures] [--reverse]` |
   | 类型/宏/枚举 | `--kw <类型名>` 或 `--category defs`（对应的 reference 定义页） |
   | c_api 范围外能力 | `--gap <关键词>`（当前登记为空；命中即说明 c_api 内无文档） |

4. **按需核对**：只读候选文档中支撑推荐所需的章节（功能说明/约束说明），
   区分「已确认适用 / 条件待确认 / c_api 范围内未覆盖」；禁止全量精读。
5. **输出结果**：按固定格式给出候选（路径 + API 名 + 一句话用途）与未覆盖说明；
   用户主动询问后再展开接口细节或算子实现。

## 检索策略要点（运行实测提炼）

- **搬运家族保留**：需求出现"对齐搬运/对齐搬入搬出"字样时，`asc_copy_*` 与 `asc_copy_*_align`
  两个家族应同时保留为候选（除非需求明确排除）。
- **矩阵流水线**：矩阵乘需求按 搬入（`--stage in` 中 gm2l1/nd2nz/gm2l0a/l12l0a/l12l0b 家族，
  含 img2col 3D 展开）→ 计算（`--operation matrix_multiply_accumulate`，mmad 家族）→
  搬出（`--stage out` 中 l0c2gm 家族，随路量化/激活/格式转换配置经 `--deps-of asc_copy_l0c2gm` 展开）
  三段组织候选。
- **寄存器流水线**：reg 计算需求按 loadalign/loadunalign（`--operation load`）→ reg 计算 →
  storealign/storeunalign（`--operation store`）组织；Post Update 变体按 traits 区分。
- **同步需求**：`--operation synchronization` 覆盖核间/流水线同步；arrive/wait 跨核原语与
  asc_mem_bar/asc_lock 均在此视图；同步语义细节读 sync 概念页（system_sync_overview 等）。
- **deprecated 导航**：命中 deprecated 页时优先给出其正文声明的现行替代接口（依赖边 related 指向），
  deprecated 页本身作为兼容性候选保留。
- **缺口判断首选 `--gap`**：能力疑似 c_api 范围外时先查缺口登记；当前登记为空（原三条缺口
  ——流水线事件同步、寄存器 load/store、浮点 floor——已随范围扩展全部入索引），
  未命中时如实说明"c_api 范围内未覆盖"并给出源码或范围外目录线索（basic_api/adv_api/tensor_api/SIMT/AI-CPU）。
- **按需核对**：索引候选只读"功能说明/约束说明"章节核实；源码（include/impl）仅用于
  核对原型与重载；不做全量精读。

## 输出格式（与评测口径对齐，不得增删）

```
## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/... | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

## 检索子代理提示模板 v6.0-retrieve（冒烟与后续对照复用）

```text
你是 AscendC C API 检索助手。请完成以下算子需求的 API 检索任务。

【任务需求】
{task_prompt}

【检索资产】
1. 持久化索引（必须最先使用）：在仓库根目录执行
   python3 /mnt/workspace/asc-devkit/okf-skills/okf-index/scripts/query.py <查询>
   常用查询：--stage in,out,compute（分阶段候选）；--operation add,matrix_multiply_accumulate,synchronization,…（计算意图）；
   --kw 矩阵,非对齐,填充,…（能力关键词）；--convert bfloat16 float（类型转换对）；
   --api asc_copy_l12l0a（某 API 的主页/架构变体/家族成员）；--deps-of <API>（配套配置）；
   --gap <关键词>（c_api 范围外能力缺口，命中即"c_api 范围内无文档"）。
   加 --json 可得机器可读输出。索引数据：okf-skills/okf-index/index/{index.json,gaps.json}。
2. API 文档（检索与计分主目标）：/mnt/workspace/asc-devkit/docs/zh/api/SIMD-API/c_api 整目录及其子目录
   （含 vector/reg/cube 计算与搬运、sync、atomic、spr、cache_ctrl、utils、scalar_compute、defs、
   deprecated_interface）。
3. 源码（核实 API 声明、参数与重载）：/mnt/workspace/asc-devkit/include、/mnt/workspace/asc-devkit/impl
4. 仓库其余内容（如 examples/）可按需读取；但禁止访问 okf-skills/ 下除
   okf-index/scripts/query.py 与 okf-index/index/ 之外的任何内容（评测资产隔离）。

【要求】
1. 必须先用 query.py 查询索引获得分阶段候选，再按需读取候选文档的关键章节核实用途与
   约束；不做全量精读。
2. 从需求出发定位需要的 C API，候选可以是 API 文档，也可以是源码头文件/实现文件；
   优先给出最直接记载该 API 用法与约束的文档。
3. 若某项能力在 c_api 文档目录内找不到对应文档，必须如实说明"c_api 范围内未覆盖"，
   不要猜测或编造路径；索引缺口登记（--gap）命中时可引用其范围外线索。
4. 最终回复请严格使用以下固定格式，不要添加其他内容：

## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/c_api/xxx.h 或 impl/c_api/xxx.h | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

## 版本记录

| 版本 | 变更 |
| --- | --- |
| v5.0 | 双目录范围（140 篇）+ 索引资产 + 索引优先要求（stage5-v1 实测） |
| v6.0 | 范围扩展到整个 c_api（800 篇）：语料路径、矩阵/寄存器双流水线策略、同步/类型/deprecated 导航、缺口语义改为 c_api 范围外（登记为空） |
