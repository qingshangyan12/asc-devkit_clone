---
name: simd-api-mapping
description: Analyze and compare Basic SIMD register-vector APIs with corresponding Tensor SIMD APIs from Markdown documentation, extracting interface names, exact supported data types, template mode ranges, missing types, and capability differences into a validated Markdown table. Use when a repository contains parallel basic_api and tensor_api documentation and the user requests API parity, migration, coverage, or type/mode gap analysis.
---

# SIMD API 对标分析

## 目标

以 `basic_api/reg_vector_compute` 为基线，分析 `tensor_api/reg_vector_compute` 的对标接口，生成根目录 Markdown 报告。必须以基础 API 的接口文件和数据类型表为准，不能仅按文件名或接口名称猜测对应关系。

## 工作流

1. **盘点目录**
   - 使用 `rg --files` 分别列出 Basic 和 Tensor 目录。
   - 以 Basic API 子文件夹作为“基础 API 类别”；排除 `overview.md`、类别索引页、`register_data_types`、`aux_data_types` 等说明性/类型定义页，除非用户明确要求纳入。
   - 每个剩余接口文件必须在最终表中出现，或明确记录为无直接对标。

2. **提取接口信息**
   - 读取标题、函数原型、`## 数据类型`及参数说明。
   - 数据类型必须保留源类型→目的类型、寄存器/标量、模式条件和位宽关系；不要把条件类型压扁成不准确的并集。
   - 对 Tensor API 同样提取模式限定的类型表，例如 `load_sideband_mode`、`store_sideband_mode`、`cast` 的 layout/round/sat 组合。

3. **建立语义映射**
   - 先按数学/搬运语义匹配，再按名称匹配。例如 `Add`/`Adds` 可能共同对应 `operator+`，`ShiftRight`/`ShiftRights` 对应同一运算符的寄存器和标量重载，`LoadAlign` 的多个变体可能只部分对应 `load` 的 sideband 模式。
   - 对每行给出“完整对标”“部分对标”或“无直接对标”。
   - “完整对标”只用于主要语义、数据类型和关键模板能力均覆盖的情况；存在类型、模式、对齐、mask、返回对象或调用形式差异时使用“部分对标”。

4. **核对模板参数和模式**
   - 不要把 `T/U/S/V` 这类由实参推导的类型参数当作额外能力；要关注会改变行为或合法范围的参数。
   - 重点提取并核对：
     - `MaskMergeMode`：`ZEROING`、`MERGING`，以及文档声明的“仅支持”限制。
     - 算法模式：`DivAlgo`、`ExpAlgo`、`LnAlgo`、`LogAlgo`、`Log2Algo`、`Log10Algo`、`SqrtAlgo`，包括精度、FTZ 和 Subnormal 约束。
     - 比较/归约/索引/直方图：`CMPMODE`、`ReduceType`、`PairReduce`、`IndexOrder`、`HistogramsBinType`、`HistogramsType`。
     - 类型转换：`RegLayout`、`RoundMode`、`SatMode`、`CastTrait` 及源→目的类型组合。
     - 数据搬运：`LoadDist`、`StoreDist`、`MaskDist`、`DataCopyMode`、`PostLiteral`、`HighLowPart`，包括对齐、步长和地址更新限制。
     - 特殊参数：`GatherMaskMode`、`MaskGenWithRegTensor` 的 `offset`、`SpecialPurposeReg`、`MemType` 合法组合。
   - 详细参数范围可放在主表之后的独立章节；主表保持可读。

5. **编写主表**

主表列顺序固定为：

| 基础 API 类别 | 基础 API 接口名称 | 基础 API 支持的数据类型 | Tensor API 接口名称 | Tensor API 支持的数据类型 | Tensor API 对标能力 |
| --- | --- | --- | --- | --- | --- |

“Tensor API 对标能力”必须具体：

- 有类型差异时使用格式：`数据类型缺少：int64_t、uint64_t、complex64；……`。
- 没有类型缺失时不要写“数据类型缺少：无”；直接描述模式、对齐、返回值或语义差异。
- 不使用“类型集合不同”“同上”“部分支持”等无法核验的泛化表述；必须列出具体类型或明确引用模式范围。
- 低精度类型必须写出完整名称，如 `hifloat8_t`、`fp8_e4m3fn_t`、`fp4x2_e2m1_t`、`int4x2_t`。

6. **总结数据类型差异**

在主表后增加表格，至少覆盖：8/16/32/64 位整数、`half`/`float`、`bfloat16_t`、`complex32/complex64`、`bool`/Mask、`b8/b16/b32/b64` 抽象、FP8/HIFLOAT8、FP4/INT4，以及“类型受模式约束”的组合差异。总结必须同时写明 Basic 优势、Tensor 新增类型和迁移影响。

7. **验证**
   - 统计主表行数，确保等于纳入的 Basic API 接口文件数。
   - 检查每行 Markdown 分隔符数量一致。
   - 检查“完整/部分/无直接对标”汇总数量与逐行数量一致。
   - 搜索主表中的 `同上`、`类型集合不同`、`数据类型缺少：无` 等模糊或不应出现的表述并清理。

## 输出要求

- 默认保存为当前仓库根目录下的 `.md` 文件。
- 使用中文说明，API 名称、枚举值、C++ 类型和模式值保留代码格式。
- 不修改源 API 文档；只生成或更新对标报告。
