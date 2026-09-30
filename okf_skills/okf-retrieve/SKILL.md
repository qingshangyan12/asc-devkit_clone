---
name: okf-retrieve
description: 从用户算子需求出发，经 okf-index 门控化索引（CANN 版本×架构×层级，当前 c_api 试点实例：vector_compute+vector_datamove × A5 × CANN 9.2.0）定位分阶段候选 C API 文档。索引优先、按需核对、如实标注试点范围外缺口；输出固定答案格式。
---

# OKF 知识检索

本 Skill 打通「算子需求 → 索引查询 → 分阶段候选文档」链路。检索入口是
`okf-index/scripts/query.py`，本 Skill 只定义检索流程与输出规范，不新建索引数据。
字段与推导契约见 [`../字段与推导规范.md`](../字段与推导规范.md)。

## 检索范围与门控

索引实例构建时已按 **CANN 版本 × 架构 × 层级** 门控（当前：CANN 9.2.0 自动检测 × A5 × c_api
试点范围 vector_compute+vector_datamove）。检索结果天然只含当前门控下可用的 API；
换架构门控需重建索引（`build_index.py --arch A3`），不要把门控外缺失误写为「仓库不支持」。
"文档范围内" = 当前索引实例覆盖的试点目录。

## 检索流程

1. **理解需求**：判断算子类别（逐元素/归约/比较/转换/搬运/矩阵乘/同步/原子…）、计算意图、
   数据类型、对齐与掩码等约束；对影响候选选择的缺失信息记为待确认事项。
2. **拆解阶段**：搬入（in）/计算（compute）/搬出（out）+ 配置类配套 + 同步关注点。
3. **查询索引**：先 `query.py` 取得分阶段候选，再沿依赖图展开配套 API。维度映射：

   | 需求信号 | 查询 |
   | --- | --- |
   | 搬入 / 搬出 | `--stage in[,out]` |
   | 计算意图 | `--operation add,copy,matrix_multiply_accumulate,…` |
   | API 分类（include 目录语义） | `--api-class vector_datamove,reg_compute,…` |
   | 类型转换 | `--convert <src> <dst[,dst2]>`（dtype 别名 f32/e4m3/hif8 可用） |
   | 特性（低精度等） | `--feature low_precision,low_precision_matrix,int4` |
   | 参数/枚举反查 | `--param round_mode`、`--enum asc_cast_round_mode`、`--enum ASC_CAST_RN` |
   | CANN 版本 / 产品 | `--version 9.1.0-`、`--product 950,A3` |
   | 能力关键词 | `--kw 非对齐,填充,…`（title+description） |
   | 某 API 全貌 | `--api <name>`（主页+家族成员+分类页） |
   | 配套配置 | `--deps-of <api> [--kind requires|configures] [--reverse]` |
   | 范围外能力 | `--gap <关键词>`（命中即说明试点范围内无文档） |

4. **按需核对**：只读候选文档中支撑推荐所需的章节（功能说明/约束说明）；
   区分「已确认适用 / 条件待确认 / 试点范围内未覆盖」；禁止全量精读。
5. **输出结果**：固定格式（见下）；用户主动询问后再展开接口细节或算子实现。

## 输出格式（不落盘、不增删章节）

```text
## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/... | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

必要时保留待确认事项。

## 检索策略要点

- **搬运家族保留**：需求出现"对齐搬运"时 `asc_copy_*` 与 `asc_copy_*_align` 同时保留。
- **配置展开**：掩码、填充、量化、循环搬运等配置经 `--deps-of <执行接口> --reverse` 或
  `--kind configures` 展开；条件依赖注意 `condition` 提示。
- **同步需求**：`--operation synchronization` 覆盖核间/流水线同步；语义细节读 sync 概念页
  （全量轮次接入；试点范围内 UB 地址重叠约束经 `--deps-of <搬运接口> --kind requires` 展开）。
- **枚举/宏适配**：外部枚举与宏经 `--enum`/`--param` 反查定位（含 defs 头文件目录与使用方）。
- **缺口判断**：首选 `--gap`；未命中时如实说明「试点范围内未覆盖」并给出范围外目录线索
  （reg_compute/cube_*/basic_api/SIMT/AI-CPU）。
- 索引缺失或失效时说明限制并回流 okf-index，不能把范围内未检索到写成仓库不支持。
