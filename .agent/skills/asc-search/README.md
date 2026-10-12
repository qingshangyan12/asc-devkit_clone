# OKF Search README

这是用户侧 skill `asc-search` 的全貌说明。看这一篇就能知道：它是干什么的、怎么让 AI 用它、
每个文件是干什么的、索引和配置里每个字段是什么意思。

## 快速开始

先克隆完整的 asc-devkit 仓库，在仓库根目录执行（Linux/macOS，Python 3.10+）：

```bash
python3 .agent/skills/asc-search/scripts/install.py
```

这一条命令将 `asc-search` 以软链安装到 Codex、Claude Code、OpenCode 的用户 skill 目录，
重复执行无需重新安装，已有同名且指向其他位置的 skill 会保留并报出冲突。

| 智能体 | 默认安装位置 |
| --- | --- |
| Codex | `~/.agents/skills/asc-search` |
| Claude Code | `~/.claude/skills/asc-search` |
| OpenCode | `~/.config/opencode/skills/asc-search`（设置 `XDG_CONFIG_HOME` 时使用对应配置目录） |

安装路径依据 [Codex](https://learn.chatgpt.com/docs/build-skills#where-codex-loads-local-skills)、
[Claude Code](https://code.claude.com/docs/en/skills#choose-where-skills-load) 和
[OpenCode](https://opencode.ai/docs/skills/#place-files) 的官方发现规则。

只安装到一个智能体或仅在本项目启用时，可选择：

```bash
python3 .agent/skills/asc-search/scripts/install.py --agent codex
python3 .agent/skills/asc-search/scripts/install.py --scope project
```

`--agent` 可选 `codex`、`claude`、`opencode` 或 `all`（默认）；`--scope project` 使用项目内的
`.agents/skills`、`.claude/skills`、`.opencode/skills`。加 `--dry-run` 可预览安装位置。

检索脚本还依赖 PyYAML；当前 Python 环境未安装时执行：

```bash
python3 -m pip install PyYAML
```

安装后重启或重新加载智能体，在对话中输入：

> 使用 asc-search，帮我找 A5 上将两个 uint32_t 向量与输入进位相加并输出进位的接口。

skill 会从安装软链定位原始 devkit 仓库，检查索引后返回候选接口及文档路径。请保留该仓库；
仓库移动后，需要移除旧安装软链并重新运行安装命令。仓库更新后，安装入口会同步使用更新后的 skill。

## 1. 这个 skill 是干什么的

### 1.1 背景

写 Ascend C 算子要用 C API 时，面对的是 800 多篇文档，凭记忆按目录翻很慢。`asc-search` 把这些
文档整理成一个能检索的库：你把需求说给 AI，它返回候选接口文档的路径和用途。

### 1.2 它靠什么工作

靠文档里预先写好的三类"头"（由建设侧 skill `asc-knowledge-build` 生产）：

| 头 | 在哪 | 给检索提供什么 |
| --- | --- | --- |
| 文档头 | 每篇 Markdown 顶部的 5 字段 YAML | 语义：做什么、什么操作、配套什么、哪个版本可用 |
| include 头 | `include/c_api/**/*.h` 声明上方的 Doxygen | 结构：重载编号、适用架构、参数方向 |
| examples 头 | 每源文件重点 API 共同范围首尾的 `//! [id1, id2, ...]` 成对标记 | 关联：同一展示范围对应哪些重载；单 ID 标记仍兼容 |

`asc-search` 把三类头推导成一份持久索引，查询时只查索引、按需打开候选文档，不需要现场全文搜索。

![asc-search：三类知识标注与检索工具链](references/asc-search三类知识标注与检索工具链.png)

### 1.3 两个工作流

1. **建立索引**：三类头 → 门控化持久索引（CANN 版本 × 架构 × 层级）。索引是纯派生数据，
   校验不过不出索引，同样输入两次构建结果字节一致。
2. **使用索引（检索）**：理解需求 → 保障索引就绪 → 多轮查询 → 给出分阶段候选文档。

### 1.4 边界

- 只帮找文档，不生成算子代码、不替你写 kernel。
- 当前只覆盖 C API（`docs/zh/api/SIMD-API/c_api/`）；其他层级会如实标注"范围外"。
- 不改文档、不改索引构建产物；写头、改词表是 `asc-knowledge-build` 的事，两个 skill 互不引用对方文件。

## 2. 怎么用（skill 使用方法）

### 2.1 接入方式

按前面的[快速开始](#快速开始)安装到智能体的 skill 目录。`asc-search` 的源码统一放在
`.agent/skills/asc-search/`，安装入口通过软链访问它。若工具不支持自动加载，在对话里补一句
"按 `.agent/skills/asc-search/SKILL.md` 的流程帮我找……"。

### 2.2 直接提问

把需求用自然语言说给 AI 即可，例如：

- "把 GM 上的数据搬到 UB，用哪个接口？"
- "想把 half 转成 fp8，有哪些接口？"
- "asc_xxx 这个接口要搭配什么接口一起用？"
- "9.1.0 起可用的拷贝接口有哪些？"

一句话里可以同时带上阶段、数据类型、版本、产品、行为约束（如非对齐、填充），说得越具体候选越准。

示例：你说"在 A5 上想把两个 uint32_t 向量和输入进位逐元素相加，并输出进位"，AI 会先按 A5 门控准备索引，
再按"操作=add、类型=uint32"查询，结合带进位语义返回
`docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_addc.md` 及一句话用途；如果需求有歧义，它会先反问你。

### 2.3 skill 会做什么

1. **索引前置保障**：按本机 CANN 版本 × 目标架构检查索引，缺失或不匹配就原地重建；
   换架构/版本等于换一套文档范围，AI 会以检查结果作为本次检索的边界。
2. **意图分析**：把需求拆成阶段、操作、类型、约束、版本、产品等可检索信号。
   如果需求有歧义（例如裸词"转换"分不清是类型转换还是位置搬运），AI 会先反问你：同一检索需求
   累计最多 3 轮，每轮最多 3 个问题并带选项。达到上限仍不明确时，说明信息不足，列出已确认条件与待确认项。
3. **多轮查询**：初查 → 用维度收窄；查不到时按降级链换关键词、换分类，最后用缺口登记确认是否范围外。
4. **配套展开**：对候选接口展开依赖、配置关系，反查参数用到的枚举/宏，家族变体一起保留。
5. **按需核对**：只读候选文档里支撑推荐所需的章节，区分"已确认适用 / 条件待确认 / 范围内未覆盖"。

### 2.4 skill 给什么结果

回答包含候选接口、文档路径和用途，以及适用条件、待确认事项或文档范围内未覆盖的能力。
信息不足时会先澄清；未检索到时会如实说明，不把空结果当成“仓库不支持”。正式检索的输出结构为：

```text
## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述；没有未覆盖项时写“无”。
```

下面四张问答卡片来自实际检索与候选文档核对，按 **CANN 9.2.0 × A5 × C API** 整理。
卡片将候选压成表格，点击接口名即可打开对应文档。换版本或架构后，应重新检索，结果可能不同。

<details>
<summary>问答 1：把 GM 上的数据搬到 UB，用哪个接口？</summary>

**答：** 先看 `asc_copy_gm2ub`；需要非对齐填充或多维搬运时，再看对应候选。

**候选**

| 接口与文档 | 用途与选择条件 |
| --- | --- |
| [asc_copy_gm2ub](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md) | GM→UB，支持连续搬运和高维切分搬运；目的 UB 地址须 32 字节对齐，长度非 32 字节整数倍时需要为 UB 侧补齐写入预留空间。 |
| [asc_copy_gm2ub_align](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md) | 非对齐 GM→UB 搬运，可按原型与参数选择填充方式；手动填充、循环搬运等场景有配套配置接口。 |
| [asc_ndim_copy_gm2ub](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_gm2ub.md) | 最多 5 维、各维独立步长的 GM→UB 搬运，支持随路 Padding。 |

以上接口仅在 AIV 上生效。具体选择还需确认数据类型、搬运维度、对齐和填充要求。
与其他指令存在 UB 地址重叠时，要配合
[asc_sync_notify](../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_notify.md)、
[asc_sync_wait](../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_wait.md) 同步。

**文档范围内未覆盖：** 无。

</details>

<details>
<summary>问答 2：想把 half 转成 fp8，有哪些接口？</summary>

**答：** 当前范围没有检出 half→`fp8_e4m3fn_t` / `fp8_e5m2_t` 的单步转换接口。
可以继续核对“half→float→fp8”的分步转换候选：

**候选**

| 接口与文档 | 用途 |
| --- | --- |
| [asc_half2float](../../../docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2float.md) | 将 half 矢量寄存器元素转换为 float，作为中间步骤。 |
| [asc_float2e4m3_rn / asc_float2e4m3_rn_sat](../../../docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2e4m3.md) | 将 float 转为 `fp8_e4m3fn_t`，分别对应非饱和与饱和形式。 |
| [asc_float2e5m2_rn / asc_float2e5m2_rn_sat](../../../docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2e5m2.md) | 将 float 转为 `fp8_e5m2_t`，分别对应非饱和与饱和形式。 |

待确认目标 fp8 格式、饱和策略和结果布局。这些接口按位置标签选取源或目的元素，
不能直接假定组合调用后得到连续打包的 fp8 数据；使用饱和或非饱和模式时，还需按文档配置
[asc_set_ctrl](../../../docs/zh/api/SIMD-API/c_api/spr/asc_set_ctrl.md)。

**文档范围内未覆盖：** 未检出上述两种 fp8 格式的 half 单步直转接口；分步候选的存在不等于已验证完整转换实现。

</details>

<details>
<summary>问答 3：asc_xxx 这个接口要搭配什么接口一起用？</summary>

**答：** 请把 `asc_xxx` 换成完整接口名，也可以提供接口文档路径。
不同接口的配置、同步与条件依赖不同，明确名称后才能检索配套关系。

**补充提问示例：** “asc_ndim_copy_gm2ub 要搭配什么接口一起用？”

**候选**

| 配套接口与文档 | 何时使用 |
| --- | --- |
| [asc_set_ndim_loop0_stride 至 asc_set_ndim_loop4_stride](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_loop_stride.md) | 搬运前配置各维源、目的步长，单位为元素个数。 |
| [asc_set_ndim_pad_count](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_pad_count.md) | 配置 loop1 至 loop4 的左右填充数；loop0 填充数由搬运接口参数设置。 |
| [asc_set_ndim_pad_value](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_pad_value.md) | `padding_mode=true` 时，搬运前配置与数据类型一致的常数填充值。 |
| [asc_ndim_copy_dci](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_dci.md) | 多核可能读取同一 GM 地址，且数据可能被其他核更新时，搬运前刷新 NDDMA DataCache。 |
| [asc_sync_notify](../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_notify.md)、[asc_sync_wait](../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_wait.md) | 与其他指令存在 UB 地址重叠时，配对同步以保证相关读写串行化。 |

配套关系有各自的触发条件，并非每次搬运都需要调用表中的全部接口。

**文档范围内未覆盖：** 对补充提问，无；原问题中的占位符需要先澄清。

</details>

<details>
<summary>问答 4：9.1.0 起可用的拷贝接口有哪些？</summary>

**答：** 这里需要先确认：你想找“文档标为从 9.1.0 开始支持”的接口，
还是“在 CANN 9.1.0 中可用的全部拷贝接口”？后者也包括更早版本引入的接口，需要按目标架构与 9.1.0 重新准备索引。

**补充提问示例：** “目标 A5、当前 CANN 9.2.0，找文档 version 标为 `>=9.1.0` 的拷贝接口。”

**候选**

| 接口与文档 | 用途 |
| --- | --- |
| [asc_copy_gm2ub](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md) | GM→UB。 |
| [asc_copy_ub2gm](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md) | UB→GM。 |
| [asc_copy_ub2l1](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2l1.md) | UB→L1。 |
| [asc_copy_ub2ub](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2ub.md) | UB 内部搬运。 |
| [asc_copy_gm2l1_align](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_align.md) | GM→L1，支持非对齐搬运与填充。 |
| [asc_copy_gm2l1_dn2nz](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_dn2nz.md) | GM→L1，并做 DN→Nz 格式转换。 |
| [asc_copy_gm2l1_nd2nz](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md) | GM→L1，并做 ND→Nz 格式转换。 |
| [asc_copy_l0c2gm](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md) | L0C→GM，可组合随路量化、激活与格式转换。 |
| [asc_copy_l0c2ub](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l0c2ub.md) | L0C→UB，可组合随路量化、激活与格式转换。 |
| [asc_copy_l12bt](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md) | L1→BiasTable Buffer。 |
| [asc_copy_l12fb](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_3510.md) | L1→Fixpipe Buffer，搬运量化或激活参数。 |
| [asc_copy_l12l0b_mx](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12l0b_mx.md) | L1→L0B_MX Buffer，搬运右矩阵量化系数。 |
| [asc_copy_l12l0b_trans](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md) | L1→L0B，并做二维分形转置。 |
| [asc_copy_l12ub](../../../docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12ub.md) | L1→UB。 |

此处按文档 `version` 区间精确匹配并筛去废弃接口；更早引入或标为 `>=9.2.0` 的接口不在这张表中。
表中结果反映当前门控下的文档标注，不代表已验证这些 A5 接口能在历史 CANN 9.1.0 环境运行。

**文档范围内未覆盖：** 对补充提问，无；“9.1.0 当时可用的全部接口”需要另行检索。

</details>
