# 逻辑位置与物理位置资料语义复核报告

## 结论

本报告不是按关键词直接判错，而是对候选行结合参数语义、表格列含义、章节目标、前后文映射说明和历史整改提交逐项复核。

- 扫描当前工作树`docs`下3132个已跟踪Markdown资料文件（不含本报告）；按要求不纳入本地未跟踪的DFX设计文档。
- 原扫描中`R/M`共903条疑似遗漏：确认应修改并已完成685条，确认不应修改218条。
- 对原`K`（已合规）339条重新检查后，确认其中11条物理/逻辑通路分层表达也无需修改。
- 最终确认应修改并已完成685条；所有候选均已有明确结论，不存在`REVIEW`或“待确认”项。
- 代码块和代码注释仍按原整改范围排除，不修改示例代码中的`TPosition::...`标识符。

## 数量汇总

| 结论 | 类型 | 行数 | 文件数 |
| --- | --- | ---: | ---: |
| 已修改 | `FIX_OPERAND` API操作数或约束仅展示TPosition | 610 | 212 |
| 已修改 | `FIX_ROUTE` Release Notes只展示逻辑通路 | 2 | 1 |
| 已修改 | `FIX_PROSE` 正文存取、Queue、缓存或结果位置使用裸逻辑位置 | 73 | 34 |
| 不修改 | `KEEP_ROUTE_DETAIL` 物理通路下补充逻辑TPosition通路 | 112 | 22 |
| 不修改 | `KEEP_META` TPosition概念、模板参数或显式逻辑通路定义 | 51 | 7 |
| 不修改 | `KEEP_NAME` 接口、资源管理类或特性名称 | 28 | 14 |
| 不修改 | `KEEP_COMPLIANT` 已经采用物理位置（逻辑位置） | 11 | 6 |
| 不修改 | `KEEP_PRODUCT` 产品相关CO2逻辑位置，正文已给出映射说明 | 7 | 1 |
| 不修改 | `KEEP_DATA` 矩阵/数据块编号或产品代际名称 | 7 | 5 |
| 不修改 | `KEEP_PHYSICAL` 物理Buffer简称，不是逻辑位置 | 2 | 1 |
| 合计 | 已完成修改 | 685 | - |
| 合计 | 原R/M中确认不修改 | 218 | - |

## 与上次整改的关系

以`cafca01f52c55bdefad5017132d0702f6b812df4`（`docs: 整改文档物理位置和逻辑位置混用问题`）为基准，并忽略目录迁移、文件重命名、链接替换和产品过滤标签改写对blame的干扰：

- 681条确认问题在该整改提交之前已经存在，属于当时没有覆盖到的存量遗漏。
- 2条由该整改提交本身留下，位于`SetTensorA.md:64`和`SetTensorB.md:70`：同一长行后半段已补物理位置，但开头的“支持的TPosition为TSCM/VECOUT”仍是裸逻辑位置。
- 2条在整改提交之后新增，均位于`Cann_9_0_0.md:13、229`，是新增Release Notes中的`CO1->GM/CO1->A1`通路。

因此，不能说报告中的内容“之前都没整改过”：大量相邻内容已经在该提交中正确整改；本次整改项是同一文件中的漏行、未覆盖文件和后续新增内容。原候选中另有218条经上下文确认本来就不应修改。

## 典型判定

| 类型 | 当前形式 | 判定 | 建议/理由 |
| --- | --- | --- | --- |
| API操作数 | `支持的TPosition为VECIN/VECCALC/VECOUT` | 应改 | 面向用户展示实际存储，应为`UB（VECIN/VECCALC/VECOUT）`。 |
| 逻辑通路子项 | 物理主项`Global Memory -> Unified Buffer`下的`GM -> VECIN` | 不改 | 子项明确在补充TPosition通路。 |
| C API Release Notes | 仅写`CO1->GM、CO1->A1` | 应改 | 没有物理通路主项，应改为`L0C Buffer->GM、L0C Buffer->L1 Buffer`。 |
| Queue正文 | `从VECIN的Queue中取出LocalTensor` | 应改 | 应为`从UB（VECIN）的Queue中...`。 |
| 通路括注 | `L0C Buffer->UB（CO1->CO2）` | 不改 | 已同时说明物理通路和逻辑TPosition通路。 |
| 映射定义 | `A1：逻辑内存，物理对应L1 Buffer` | 不改 | 本段正在定义逻辑位置与物理存储的映射。 |
| 显式逻辑模型 | `逻辑位置的数据流如下：GM->A1->A2` | 不改 | 章节明确声明使用逻辑位置表达数据流。 |
| 产品相关CO2 | `逻辑位置CO2`并链接产品映射 | 不改 | CO2不能跨产品硬编码成单一物理位置。 |
| 特性/类名 | `TSCM输入的矩阵乘`、`TSCM::AllocTensor` | 不改 | 这里是特性名或资源管理类型名。 |
| 数据编号 | `矩阵A1 * B1 + A2 * B2` | 不改 | A1/A2/B1/B2是矩阵分块名。 |

## 最近资料复核

- 整改提交之后真正新增且需要整改的文本只有`Cann_9_0_0.md:13、229`两行。
- 2026-07-31的目录/文件名同步提交主要改变路径和链接；对其忽略后，相关裸位置文本均可追溯到整改提交之前，不应误算为最近新增。
- `DataCopy_L0CToUB.md`虽然早于整改提交创建，但原整改遗漏了第86-89行；第6行同时给出物理通路和逻辑通路，按本次确认口径保留。

## 本次已修改

### `FIX_OPERAND`：API操作数或约束仅展示TPosition

这些行面向用户说明LocalTensor、矩阵或临时空间的实际可用位置，仅写`支持的TPosition为...`不够，应补成对应物理位置（逻辑位置）。

共610行，212个文件。

- [docs/en/asc_adv_api_contributing.md](<en/asc_adv_api_contributing.md>)：52。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/data_rearrange_ISASI/DeInterleave.md](<zh/api/SIMD-API/基础API/Memory矢量计算/data_rearrange_ISASI/DeInterleave.md>)：63、64。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/data_rearrange_ISASI/Interleave.md](<zh/api/SIMD-API/基础API/Memory矢量计算/data_rearrange_ISASI/Interleave.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/MrgSort.md](<zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/MrgSort.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/MrgSort4.md](<zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/MrgSort4.md>)：76、100。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/ProposalConcat.md](<zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/ProposalConcat.md>)：98、107。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/ProposalExtract.md](<zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/ProposalExtract.md>)：76、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/RpSort16.md](<zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/RpSort16.md>)：76、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/Sort32.md](<zh/api/SIMD-API/基础API/Memory矢量计算/sort_and_merge_ISASI/Sort32.md>)：63、64、65。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Abs.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Abs.md>)：89、90。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Add.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Add.md>)：84、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Adds.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Adds.md>)：103、104。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Adds_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Adds_flexible_scalar.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/BilinearInterpolation_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/BilinearInterpolation_ISASI.md>)：173、174、175。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Div.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Div.md>)：143、144。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Divs_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Divs_flexible_scalar.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Exp.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Exp.md>)：137、138。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md>)：113、114。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Ln.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Ln.md>)：139、140。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Max.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Max.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Maxs.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Maxs.md>)：103、104。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Maxs_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Maxs_flexible_scalar.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Min.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Min.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mins.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mins.md>)：103、104。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mins_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mins_flexible_scalar.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mul.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mul.md>)：84、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mull_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Mull_ISASI.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Muls.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Muls.md>)：103、104。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Muls_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Muls_flexible_scalar.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Neg_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Neg_ISASI.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Prelu_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Prelu_ISASI.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Reciprocal.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Reciprocal.md>)：137、138。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Relu.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Relu.md>)：82、83。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Rsqrt.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Rsqrt.md>)：137、138。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Sqrt.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Sqrt.md>)：137、138。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Sub.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Sub.md>)：84、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Subs_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/基础算术/Subs_flexible_scalar.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AbsSub_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AbsSub_ISASI.md>)：56、57。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AddDeqRelu.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AddDeqRelu.md>)：110、111。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AddRelu.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AddRelu.md>)：80、81。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AddReluCast.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/AddReluCast.md>)：81、82。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/Axpy.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/Axpy.md>)：81、82。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/CastDequant.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/CastDequant.md>)：97、98。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/ExpSub_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/ExpSub_ISASI.md>)：61、62。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/FusedMulAdd.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/FusedMulAdd.md>)：80、81。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulAddDst.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulAddDst.md>)：81、82。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulAddRelu.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulAddRelu.md>)：80、81。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulCast.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulCast.md>)：75、76。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulsCast_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/MulsCast_ISASI.md>)：67、68。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/SubRelu.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/SubRelu.md>)：80、81。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/SubReluCast.md](<zh/api/SIMD-API/基础API/Memory矢量计算/复合计算/SubReluCast.md>)：81、82。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据填充/Duplicate.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据填充/Duplicate.md>)：100。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据填充/VectorPadding_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据填充/VectorPadding_ISASI.md>)：109、118。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据排布转换/TransDataTo5HD.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据排布转换/TransDataTo5HD.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/Copy_UBToUB_continuous.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/Copy_UBToUB_continuous.md>)：69、70。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compare.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compare.md>)：108、109。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compare_store_to_register.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compare_store_to_register.md>)：76。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compares.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compares.md>)：93、94。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compares_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Compares_flexible_scalar.md>)：91、92。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/GatherMask.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/GatherMask.md>)：143、144。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/GetCmpMask_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/GetCmpMask_ISASI.md>)：54。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Select.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Select.md>)：168、169、170、171。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Select_flexible_scalar.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/Select_flexible_scalar.md>)：85、86、87。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/SetCmpMask_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/比较与选择/SetCmpMask_ISASI.md>)：64。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/类型转换/Cast.md](<zh/api/SIMD-API/基础API/Memory矢量计算/类型转换/Cast.md>)：77、78。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/类型转换/Truncate_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/类型转换/Truncate_ISASI.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/类型转换辅助配置接口/SetDeqScale.md](<zh/api/SIMD-API/基础API/Memory矢量计算/类型转换辅助配置接口/SetDeqScale.md>)：83。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/And.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/And.md>)：84、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Ands.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Ands.md>)：80、81。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Not.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Not.md>)：78、79。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Or.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Or.md>)：84、85。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Ors.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/Ors.md>)：80、81。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftLeft.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftLeft.md>)：104、105。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftLeft_shift_amount_tensor.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftLeft_shift_amount_tensor.md>)：54、55。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftRight.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftRight.md>)：104、105。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftRight_shift_amount_tensor.md](<zh/api/SIMD-API/基础API/Memory矢量计算/逻辑计算/ShiftRight_shift_amount_tensor.md>)：63、64。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/Conv2D_deprecated.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/Conv2D_deprecated.md>)：53、54、55、71。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/Gemm_deprecated.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/Gemm_deprecated.md>)：55、56、57、62。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵搬入辅助配置接口/LoadDataUnzip.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵搬入辅助配置接口/LoadDataUnzip.md>)：44。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_2D_BitMode.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_2D_BitMode.md>)：53、54。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_3D.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_3D.md>)：178、209、210、215、216、221、222、231、232。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_3D_BitMode.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_3D_BitMode.md>)：113。
- [docs/zh/api/SIMD-API/基础API/同步控制/核间同步/IBSet.md](<zh/api/SIMD-API/基础API/同步控制/核间同步/IBSet.md>)：55。
- [docs/zh/api/SIMD-API/基础API/同步控制/核间同步/IBWait.md](<zh/api/SIMD-API/基础API/同步控制/核间同步/IBWait.md>)：55。
- [docs/zh/api/SIMD-API/基础API/同步控制/核间同步/SyncAll.md](<zh/api/SIMD-API/基础API/同步控制/核间同步/SyncAll.md>)：149。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/DeNormMin.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/DeNormMin.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Infinity.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Infinity.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Lowest.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Lowest.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Max.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Max.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Min.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/Min.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/NegativeInfinity.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/NegativeInfinity.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/QuietNaN.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/QuietNaN.md>)：53。
- [docs/zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/SignalingNaN.md](<zh/api/SIMD-API/基础API/工具接口/NumericLimits工具类/SignalingNaN.md>)：53。
- [docs/zh/api/SIMD-API/基础API/数据结构/LocalTensor/GetValue.md](<zh/api/SIMD-API/基础API/数据结构/LocalTensor/GetValue.md>)：32。
- [docs/zh/api/SIMD-API/基础API/数据结构/LocalTensor/SetValue.md](<zh/api/SIMD-API/基础API/数据结构/LocalTensor/SetValue.md>)：32。
- [docs/zh/api/SIMD-API/高阶API/卷积计算/Conv3D-Tiling侧接口/SetOutputType.md](<zh/api/SIMD-API/高阶API/卷积计算/Conv3D-Tiling侧接口/SetOutputType.md>)：27。
- [docs/zh/api/SIMD-API/高阶API/张量变换/Broadcast.md](<zh/api/SIMD-API/高阶API/张量变换/Broadcast.md>)：131、132、135、141、142。
- [docs/zh/api/SIMD-API/高阶API/张量变换/Pad.md](<zh/api/SIMD-API/高阶API/张量变换/Pad.md>)：73、74、76。
- [docs/zh/api/SIMD-API/高阶API/张量变换/TransData.md](<zh/api/SIMD-API/高阶API/张量变换/TransData.md>)：119、120、121。
- [docs/zh/api/SIMD-API/高阶API/张量变换/Transpose-96.md](<zh/api/SIMD-API/高阶API/张量变换/Transpose-96.md>)：245、246、247。
- [docs/zh/api/SIMD-API/高阶API/张量变换/UnPad.md](<zh/api/SIMD-API/高阶API/张量变换/UnPad.md>)：65、66、68。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/BatchNorm.md](<zh/api/SIMD-API/高阶API/归一化操作/BatchNorm.md>)：71、72、73、74、75、76、77。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/DeepNorm.md](<zh/api/SIMD-API/高阶API/归一化操作/DeepNorm.md>)：67、68、69、70、71、72、73、74。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/GroupNorm.md](<zh/api/SIMD-API/高阶API/归一化操作/GroupNorm.md>)：74、75、76、77、78、79、80。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/LayerNorm.md](<zh/api/SIMD-API/高阶API/归一化操作/LayerNorm.md>)：140、141、142、143、144、145、146、174、175、176、177、178、179、181。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/LayerNormGrad.md](<zh/api/SIMD-API/高阶API/归一化操作/LayerNormGrad.md>)：91、92、93、94、95、96、97、98。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/LayerNormGradBeta.md](<zh/api/SIMD-API/高阶API/归一化操作/LayerNormGradBeta.md>)：72、73、74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/Normalize.md](<zh/api/SIMD-API/高阶API/归一化操作/Normalize.md>)：85、86、87、88、89、90、91、92。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/RmsNorm.md](<zh/api/SIMD-API/高阶API/归一化操作/RmsNorm.md>)：76、77、78、79。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/WelfordFinalize.md](<zh/api/SIMD-API/高阶API/归一化操作/WelfordFinalize.md>)：134、135、136、137、138、139。
- [docs/zh/api/SIMD-API/高阶API/归一化操作/WelfordUpdate.md](<zh/api/SIMD-API/高阶API/归一化操作/WelfordUpdate.md>)：100、101、102、103、104、105。
- [docs/zh/api/SIMD-API/高阶API/归约操作/Mean接口/Mean.md](<zh/api/SIMD-API/高阶API/归约操作/Mean接口/Mean.md>)：78、79、80。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceAll接口/ReduceAll.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceAll接口/ReduceAll.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceAny接口/ReduceAny.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceAny接口/ReduceAny.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceMax接口/ReduceMax-91.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceMax接口/ReduceMax-91.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceMean接口/ReduceMean.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceMean接口/ReduceMean.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceMin接口/ReduceMin-92.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceMin接口/ReduceMin-92.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceProd接口/ReduceProd.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceProd接口/ReduceProd.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceSum接口/ReduceSum-90.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceSum接口/ReduceSum-90.md>)：74、75、76。
- [docs/zh/api/SIMD-API/高阶API/归约操作/ReduceXorSum接口/ReduceXorSum.md](<zh/api/SIMD-API/高阶API/归约操作/ReduceXorSum接口/ReduceXorSum.md>)：68、69、70、71。
- [docs/zh/api/SIMD-API/高阶API/归约操作/Sum接口/Sum.md](<zh/api/SIMD-API/高阶API/归约操作/Sum接口/Sum.md>)：83、84、85。
- [docs/zh/api/SIMD-API/高阶API/排序操作/Concat.md](<zh/api/SIMD-API/高阶API/排序操作/Concat.md>)：48、49、50。
- [docs/zh/api/SIMD-API/高阶API/排序操作/Extract.md](<zh/api/SIMD-API/高阶API/排序操作/Extract.md>)：48、49、50。
- [docs/zh/api/SIMD-API/高阶API/排序操作/MrgSort-93.md](<zh/api/SIMD-API/高阶API/排序操作/MrgSort-93.md>)：81、102、103、104、105。
- [docs/zh/api/SIMD-API/高阶API/排序操作/Sort.md](<zh/api/SIMD-API/高阶API/排序操作/Sort.md>)：190、191、192、193、222、223、224、225、226。
- [docs/zh/api/SIMD-API/高阶API/排序操作/TopK.md](<zh/api/SIMD-API/高阶API/排序操作/TopK.md>)：267、268、269、270、271。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Acosh接口/Acosh.md](<zh/api/SIMD-API/高阶API/数学计算/Acosh接口/Acosh.md>)：85、86、87。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Acos接口/Acos.md](<zh/api/SIMD-API/高阶API/数学计算/Acos接口/Acos.md>)：91、92、93。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Asinh接口/Asinh.md](<zh/api/SIMD-API/高阶API/数学计算/Asinh接口/Asinh.md>)：85、86、87。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Asin接口/Asin.md](<zh/api/SIMD-API/高阶API/数学计算/Asin接口/Asin.md>)：91、92、93。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Atanh接口/Atanh.md](<zh/api/SIMD-API/高阶API/数学计算/Atanh接口/Atanh.md>)：85、86、87。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Atan接口/Atan.md](<zh/api/SIMD-API/高阶API/数学计算/Atan接口/Atan.md>)：103、104、105。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Axpy接口/Axpy-80.md](<zh/api/SIMD-API/高阶API/数学计算/Axpy接口/Axpy-80.md>)：56、57、59。
- [docs/zh/api/SIMD-API/高阶API/数学计算/BitwiseAnd接口/BitwiseAnd.md](<zh/api/SIMD-API/高阶API/数学计算/BitwiseAnd接口/BitwiseAnd.md>)：57、58。
- [docs/zh/api/SIMD-API/高阶API/数学计算/BitwiseNot接口/BitwiseNot.md](<zh/api/SIMD-API/高阶API/数学计算/BitwiseNot接口/BitwiseNot.md>)：57、58。
- [docs/zh/api/SIMD-API/高阶API/数学计算/BitwiseOr接口/BitwiseOr.md](<zh/api/SIMD-API/高阶API/数学计算/BitwiseOr接口/BitwiseOr.md>)：57、58。
- [docs/zh/api/SIMD-API/高阶API/数学计算/BitwiseXor接口/BitwiseXor.md](<zh/api/SIMD-API/高阶API/数学计算/BitwiseXor接口/BitwiseXor.md>)：57、58。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Ceil接口/Ceil.md](<zh/api/SIMD-API/高阶API/数学计算/Ceil接口/Ceil.md>)：73、74、75。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Clamp接口/Clamp.md](<zh/api/SIMD-API/高阶API/数学计算/Clamp接口/Clamp.md>)：61、62、63、64。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Clamp接口/ClampMax.md](<zh/api/SIMD-API/高阶API/数学计算/Clamp接口/ClampMax.md>)：69、70、71。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Clamp接口/ClampMin.md](<zh/api/SIMD-API/高阶API/数学计算/Clamp接口/ClampMin.md>)：69、70、71。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Cosh接口/Cosh.md](<zh/api/SIMD-API/高阶API/数学计算/Cosh接口/Cosh.md>)：92、93、94。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Cos接口/Cos.md](<zh/api/SIMD-API/高阶API/数学计算/Cos接口/Cos.md>)：104、105、106。
- [docs/zh/api/SIMD-API/高阶API/数学计算/CumSum接口/CumSum.md](<zh/api/SIMD-API/高阶API/数学计算/CumSum接口/CumSum.md>)：105、106、107、108。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Digamma接口/Digamma.md](<zh/api/SIMD-API/高阶API/数学计算/Digamma接口/Digamma.md>)：69、70、71。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Erfc接口/Erfc.md](<zh/api/SIMD-API/高阶API/数学计算/Erfc接口/Erfc.md>)：101、102、103。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Erf接口/Erf.md](<zh/api/SIMD-API/高阶API/数学计算/Erf接口/Erf.md>)：103、104、105。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Exp接口/Exp-81.md](<zh/api/SIMD-API/高阶API/数学计算/Exp接口/Exp-81.md>)：82、83、84。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Floor接口/Floor.md](<zh/api/SIMD-API/高阶API/数学计算/Floor接口/Floor.md>)：79、80、81。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Fma接口/Fma.md](<zh/api/SIMD-API/高阶API/数学计算/Fma接口/Fma.md>)：75、76、77。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Fmod接口/Fmod.md](<zh/api/SIMD-API/高阶API/数学计算/Fmod接口/Fmod.md>)：105、106、107。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Frac接口/Frac.md](<zh/api/SIMD-API/高阶API/数学计算/Frac接口/Frac.md>)：92、93、94。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Hypot接口/Hypot.md](<zh/api/SIMD-API/高阶API/数学计算/Hypot接口/Hypot.md>)：89、90、91。
- [docs/zh/api/SIMD-API/高阶API/数学计算/IsFinite接口/IsFinite.md](<zh/api/SIMD-API/高阶API/数学计算/IsFinite接口/IsFinite.md>)：61、62。
- [docs/zh/api/SIMD-API/高阶API/数学计算/IsInf接口/IsInf.md](<zh/api/SIMD-API/高阶API/数学计算/IsInf接口/IsInf.md>)：84、85、86。
- [docs/zh/api/SIMD-API/高阶API/数学计算/IsNan接口/IsNan.md](<zh/api/SIMD-API/高阶API/数学计算/IsNan接口/IsNan.md>)：84、85、86。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Lgamma接口/Lgamma.md](<zh/api/SIMD-API/高阶API/数学计算/Lgamma接口/Lgamma.md>)：71、72、73。
- [docs/zh/api/SIMD-API/高阶API/数学计算/LogicalAnds接口/LogicalAnds.md](<zh/api/SIMD-API/高阶API/数学计算/LogicalAnds接口/LogicalAnds.md>)：62、63。
- [docs/zh/api/SIMD-API/高阶API/数学计算/LogicalAnd接口/LogicalAnd.md](<zh/api/SIMD-API/高阶API/数学计算/LogicalAnd接口/LogicalAnd.md>)：58、59。
- [docs/zh/api/SIMD-API/高阶API/数学计算/LogicalNot接口/LogicalNot.md](<zh/api/SIMD-API/高阶API/数学计算/LogicalNot接口/LogicalNot.md>)：60、61。
- [docs/zh/api/SIMD-API/高阶API/数学计算/LogicalOrs接口/LogicalOrs.md](<zh/api/SIMD-API/高阶API/数学计算/LogicalOrs接口/LogicalOrs.md>)：62、63。
- [docs/zh/api/SIMD-API/高阶API/数学计算/LogicalOr接口/LogicalOr.md](<zh/api/SIMD-API/高阶API/数学计算/LogicalOr接口/LogicalOr.md>)：58、59。
- [docs/zh/api/SIMD-API/高阶API/数学计算/LogicalXor接口/LogicalXor.md](<zh/api/SIMD-API/高阶API/数学计算/LogicalXor接口/LogicalXor.md>)：58、59。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Log接口/Log-79.md](<zh/api/SIMD-API/高阶API/数学计算/Log接口/Log-79.md>)：131、132、133。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Power接口/Power.md](<zh/api/SIMD-API/高阶API/数学计算/Power接口/Power.md>)：161、162、163、165。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Rint接口/Rint.md](<zh/api/SIMD-API/高阶API/数学计算/Rint接口/Rint.md>)：81、82、83。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Round接口/Round.md](<zh/api/SIMD-API/高阶API/数学计算/Round接口/Round.md>)：73、74、75。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Sign接口/Sign.md](<zh/api/SIMD-API/高阶API/数学计算/Sign接口/Sign.md>)：91、92、93。
- [docs/zh/api/SIMD-API/高阶API/数学计算/SinCos接口/SinCos.md](<zh/api/SIMD-API/高阶API/数学计算/SinCos接口/SinCos.md>)：75、76、77。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Sinh接口/Sinh.md](<zh/api/SIMD-API/高阶API/数学计算/Sinh接口/Sinh.md>)：92、93、94。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Sin接口/Sin.md](<zh/api/SIMD-API/高阶API/数学计算/Sin接口/Sin.md>)：104、105、106。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Tanh接口/Tanh.md](<zh/api/SIMD-API/高阶API/数学计算/Tanh接口/Tanh.md>)：103、104、105。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Tan接口/Tan.md](<zh/api/SIMD-API/高阶API/数学计算/Tan接口/Tan.md>)：96、97、98。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Trunc接口/Trunc.md](<zh/api/SIMD-API/高阶API/数学计算/Trunc接口/Trunc.md>)：90、91、92。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Where接口/Where.md](<zh/api/SIMD-API/高阶API/数学计算/Where接口/Where.md>)：53、54、55。
- [docs/zh/api/SIMD-API/高阶API/数学计算/Xor接口/Xor-82.md](<zh/api/SIMD-API/高阶API/数学计算/Xor接口/Xor-82.md>)：94、95、96、97。
- [docs/zh/api/SIMD-API/高阶API/数据过滤/DropOut.md](<zh/api/SIMD-API/高阶API/数据过滤/DropOut.md>)：120、121、122、123。
- [docs/zh/api/SIMD-API/高阶API/数据过滤/Select-95.md](<zh/api/SIMD-API/高阶API/数据过滤/Select-95.md>)：92、93。
- [docs/zh/api/SIMD-API/高阶API/激活函数/GeGLU接口/GeGLU.md](<zh/api/SIMD-API/高阶API/激活函数/GeGLU接口/GeGLU.md>)：103、104、105。
- [docs/zh/api/SIMD-API/高阶API/激活函数/Gelu接口/FasterGelu.md](<zh/api/SIMD-API/高阶API/激活函数/Gelu接口/FasterGelu.md>)：76、77、78。
- [docs/zh/api/SIMD-API/高阶API/激活函数/Gelu接口/FasterGeluV2.md](<zh/api/SIMD-API/高阶API/激活函数/Gelu接口/FasterGeluV2.md>)：72、73、74。
- [docs/zh/api/SIMD-API/高阶API/激活函数/Gelu接口/Gelu.md](<zh/api/SIMD-API/高阶API/激活函数/Gelu接口/Gelu.md>)：69、70、71。
- [docs/zh/api/SIMD-API/高阶API/激活函数/LogSoftMax接口/LogSoftMax.md](<zh/api/SIMD-API/高阶API/激活函数/LogSoftMax接口/LogSoftMax.md>)：89、90、91、92、93。
- [docs/zh/api/SIMD-API/高阶API/激活函数/ReGlu接口/ReGlu.md](<zh/api/SIMD-API/高阶API/激活函数/ReGlu接口/ReGlu.md>)：77、78、79、80。
- [docs/zh/api/SIMD-API/高阶API/激活函数/Sigmoid接口/Sigmoid.md](<zh/api/SIMD-API/高阶API/激活函数/Sigmoid接口/Sigmoid.md>)：92、93、94。
- [docs/zh/api/SIMD-API/高阶API/激活函数/Silu接口/Silu.md](<zh/api/SIMD-API/高阶API/激活函数/Silu接口/Silu.md>)：59、60。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/AdjustSoftMaxRes.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/AdjustSoftMaxRes.md>)：68、69。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SimpleSoftMax.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SimpleSoftMax.md>)：124、125、126、127、128。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftMax.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftMax.md>)：165、166、167、168、169。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxFlash.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxFlash.md>)：113、114、115、116、117、118、119、120。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxFlashV2.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxFlashV2.md>)：204、205、206、207、208、209、210、211、212。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxFlashV3.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxFlashV3.md>)：126、127、128、129、130、131、132、133、134、135。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxGrad.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxGrad.md>)：100、101、102、103。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxGradFront.md](<zh/api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftmaxGradFront.md>)：97、98、99、100。
- [docs/zh/api/SIMD-API/高阶API/激活函数/SwiGLU接口/SwiGLU.md](<zh/api/SIMD-API/高阶API/激活函数/SwiGLU接口/SwiGLU.md>)：93、94、96。
- [docs/zh/api/SIMD-API/高阶API/激活函数/Swish接口/Swish.md](<zh/api/SIMD-API/高阶API/激活函数/Swish接口/Swish.md>)：53、54。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/IterateAll.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/IterateAll.md>)：55。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetBias.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetBias.md>)：49。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetLocalWorkspace.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetLocalWorkspace.md>)：54。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorA.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorA.md>)：64。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorB.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorB.md>)：70。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorScaleA.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorScaleA.md>)：52。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorScaleB.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetTensorScaleB.md>)：52。
- [docs/zh/api/SIMD-API/高阶API/索引计算/Arange-94.md](<zh/api/SIMD-API/高阶API/索引计算/Arange-94.md>)：61。
- [docs/zh/api/SIMD-API/高阶API/量化操作/AntiQuantize.md](<zh/api/SIMD-API/高阶API/量化操作/AntiQuantize.md>)：116、117、120。
- [docs/zh/api/SIMD-API/高阶API/量化操作/AscendAntiQuant.md](<zh/api/SIMD-API/高阶API/量化操作/AscendAntiQuant.md>)：282、283、284、285、286、303、304、305、306、307。
- [docs/zh/api/SIMD-API/高阶API/量化操作/AscendDequant.md](<zh/api/SIMD-API/高阶API/量化操作/AscendDequant.md>)：214、215、216、217、232、233、234、235、236。
- [docs/zh/api/SIMD-API/高阶API/量化操作/AscendQuant.md](<zh/api/SIMD-API/高阶API/量化操作/AscendQuant.md>)：389、390、391、400、401、402、403、404、413、414、415、416、417。
- [docs/zh/api/SIMD-API/高阶API/量化操作/Dequantize.md](<zh/api/SIMD-API/高阶API/量化操作/Dequantize.md>)：103、104、107。
- [docs/zh/api/SIMD-API/高阶API/量化操作/Quantize.md](<zh/api/SIMD-API/高阶API/量化操作/Quantize.md>)：115、116、119。
- [docs/zh/api/SIMD-API/高阶API/随机函数/PhiloxRandom.md](<zh/api/SIMD-API/高阶API/随机函数/PhiloxRandom.md>)：60。
- [docs/zh/asc_adv_api_contributing.md](<zh/asc_adv_api_contributing.md>)：52。

### `FIX_ROUTE`：Release Notes只展示逻辑通路

这两行位于C API Release Notes，只给出CO1/A1逻辑通路且没有物理通路主项；按C API口径应改为纯物理位置。

共2行，1个文件。

- [docs/zh/api/附录/Release_Notes/Cann_9_0_0.md](<zh/api/附录/Release_Notes/Cann_9_0_0.md>)：13、229。

### `FIX_PROSE`：正文存取、Queue、缓存或结果位置使用裸逻辑位置

这些行描述真实的存放、输出、缓存、Queue入出队或位置约束，逻辑位置直接承担了物理存储含义，应整改。

共73行，34个文件。

- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_GMToUB.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_GMToUB.md>)：149、189、195。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_UBToGM.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_UBToGM.md>)：114、122。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵搬入辅助配置接口/LoadImageToLocal.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵搬入辅助配置接口/LoadImageToLocal.md>)：82。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵搬入辅助配置接口/SetLoadDataBoundary.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵搬入辅助配置接口/SetLoadDataBoundary.md>)：55、56。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_2D_BitMode.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_2D_BitMode.md>)：60。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_2D_MX.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_2D_MX.md>)：73。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_3D_BitMode.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/LoadData_3D_BitMode.md>)：60。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬出/DataCopy_L0CToUB.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬出/DataCopy_L0CToUB.md>)：86、87、88、89。
- [docs/zh/api/SIMD-API/基础API/工具接口/系统初始化/InitSocState.md](<zh/api/SIMD-API/基础API/工具接口/系统初始化/InitSocState.md>)：43、72、84。
- [docs/zh/api/SIMD-API/基础API/资源管理/TQue/DeQue.md](<zh/api/SIMD-API/基础API/资源管理/TQue/DeQue.md>)：58。
- [docs/zh/api/SIMD-API/基础API/资源管理/TQue/EnQue.md](<zh/api/SIMD-API/基础API/资源管理/TQue/EnQue.md>)：50。
- [docs/zh/api/SIMD-API/基础API/资源管理/TQueBind/TQueBind简介.md](<zh/api/SIMD-API/基础API/资源管理/TQueBind/TQueBind简介.md>)：121、125。
- [docs/zh/api/SIMD-API/高阶API/卷积计算/Conv3D-Tiling侧接口/SetOutputType.md](<zh/api/SIMD-API/高阶API/卷积计算/Conv3D-Tiling侧接口/SetOutputType.md>)：17。
- [docs/zh/api/SIMD-API/高阶API/排序操作/TopK.md](<zh/api/SIMD-API/高阶API/排序操作/TopK.md>)：272。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/GetMMConfig.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/GetMMConfig.md>)：115。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/GetTensorC.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/GetTensorC.md>)：65、76、81、83、92、212。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/Iterate.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/Iterate.md>)：42、48、55、58、70、76、92。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/IterateBatch.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/IterateBatch.md>)：119、141、145、148。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/MatmulConfig.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/MatmulConfig.md>)：70、72、73。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/Matmul使用说明.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/Matmul使用说明.md>)：68、69、74、147。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetBias.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetBias.md>)：60。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetLocalWorkspace.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/SetLocalWorkspace.md>)：33、38。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetAType.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetAType.md>)：19。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetBType.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetBType.md>)：19。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetBiasType.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetBiasType.md>)：19。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetCType.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetCType.md>)：19。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetScaleAType.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetScaleAType.md>)：19。
- [docs/zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetScaleBType.md](<zh/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/SetScaleBType.md>)：19。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矢量编程/TBuf的使用.md](<zh/guide/算子实践参考/SIMD算子实现/矢量编程/TBuf的使用.md>)：98、103。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矢量编程/基础矢量算子.md](<zh/guide/算子实践参考/SIMD算子实现/矢量编程/基础矢量算子.md>)：143、149、222、238、240、260。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/Matmul特性介绍.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/Matmul特性介绍.md>)：61。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/MxMatmul场景.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/MxMatmul场景.md>)：212、213、217、220。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/矩阵乘输出的Channel拆分.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/矩阵乘输出的Channel拆分.md>)：23。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/矩阵乘输出的量化-反量化.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/矩阵乘输出的量化-反量化.md>)：55。

## 确认不应修改

以下218条均来自原`R/M`疑似遗漏集，逐项结合上下文后确认不属于本次整改。

### `KEEP_ROUTE_DETAIL`：物理通路下补充逻辑TPosition通路

上文或父级条目已经说明物理通路，本行明确用于补充对应的逻辑TPosition通路；这是两层信息，不属于裸逻辑位置遗漏。

共112行，22个文件。

- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/Copy_UBToUB_continuous.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/Copy_UBToUB_continuous.md>)：42、43、44、45、46、47。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/Copy_UBToUB_mask_highdim_split.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/Copy_UBToUB_mask_highdim_split.md>)：42、43、44、45、46、47。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_GMToUB.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_GMToUB.md>)：68、69、71、139、141。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_UBToGM.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopyPad_UBToGM.md>)：62、63。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMAndUB_continuous.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMAndUB_continuous.md>)：42、44、46。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMAndUB_highdim_split.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMAndUB_highdim_split.md>)：44、46、48。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMAndUB_slice.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMAndUB_slice.md>)：36、38、40。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMToUB_ND2NZ.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMToUB_ND2NZ.md>)：42。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMToUB_NDDMA.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_GMToUB_NDDMA.md>)：36。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_UBToGM_NZ2ND.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_UBToGM_NZ2ND.md>)：36、38。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_UBToUB_continuous.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_UBToUB_continuous.md>)：42、43。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_UBToUB_highdim_split.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运/DataCopy_UBToUB_highdim_split.md>)：42、43。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运辅助配置接口/ResetLoopModePara.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运辅助配置接口/ResetLoopModePara.md>)：36、38。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运辅助配置接口/SetLoopModePara.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运辅助配置接口/SetLoopModePara.md>)：36、38。
- [docs/zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运辅助配置接口/SetPadValue_ISASI.md](<zh/api/SIMD-API/基础API/Memory矢量计算/数据搬运辅助配置接口/SetPadValue_ISASI.md>)：36、37。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/GM与L1或L0数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/GM与L1或L0数据搬运.md>)：30、32、61、63、90、120、142、163、182、203、221、239、257。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/GM与UB数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/GM与UB数据搬运.md>)：32、34、36、66、68、70、98、100、102、130、159、161、180、210、211、213、245、246。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/L1与BiasTable-Buffer数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/L1与BiasTable-Buffer数据搬运.md>)：10、30。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/L1与Fixpipe-Buffer数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/L1与Fixpipe-Buffer数据搬运.md>)：10、30。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/L1与L0数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/L1与L0数据搬运.md>)：34、53、75、103、130、167、188、207、229、257、284、321、343、370、388。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/L1或L0C与UB数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/L1或L0C与UB数据搬运.md>)：102、132、162、186。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/UB与UB数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/UB与UB数据搬运.md>)：25、26、46、47、65、66、67、68、69、70、88、89、90、91、92、93。

### `KEEP_META`：TPosition概念、模板参数或显式逻辑通路定义

上下文明确在定义TPosition、模板逻辑参数或声明“以逻辑位置表达数据流”；此时逻辑位置是被解释对象，改成物理优先会破坏章节语义。

共51行，7个文件。

- [docs/zh/api/SIMD-API/基础API/基础API列表.md](<zh/api/SIMD-API/基础API/基础API列表.md>)：7。
- [docs/zh/api/SIMD-API/基础API/资源管理/TBuf/TBuf构造函数.md](<zh/api/SIMD-API/基础API/资源管理/TBuf/TBuf构造函数.md>)：59。
- [docs/zh/api/SIMD-API/基础API/资源管理/TQue/TQue简介.md](<zh/api/SIMD-API/基础API/资源管理/TQue/TQue简介.md>)：51。
- [docs/zh/api/SIMD-API/基础API/资源管理/TQueBind/TQueBind简介.md](<zh/api/SIMD-API/基础API/资源管理/TQueBind/TQueBind简介.md>)：20、27、34、41、48、55、62、69、76、83、90、97、106、146、151、162。
- [docs/zh/guide/技术附录/概念原理和术语/术语表.md](<zh/guide/技术附录/概念原理和术语/术语表.md>)：90。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/基础知识.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/基础知识.md>)：21、22、23、24、25、26、27、28、29、33、35、37、39、41、42、43。
- [docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于TPipe-TQue框架编程/TPipe-TQue框架编程范式.md](<zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于TPipe-TQue框架编程/TPipe-TQue框架编程范式.md>)：32、34、100、101、102、103、104、105、106、107、108、109、110、147、148。

### `KEEP_NAME`：接口、资源管理类或特性名称

TSCM、SPM Buffer等是类/接口/特性名，或是既有主题名称，不是遗漏的存储位置展示。

共28行，14个文件。

- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopyPad_UBToL1.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopyPad_UBToL1.md>)：103。
- [docs/zh/api/SIMD-API/基础API/基础API列表.md](<zh/api/SIMD-API/基础API/基础API列表.md>)：499、500、501。
- [docs/zh/api/SIMD-API/基础API/资源管理/TPipe/InitBuffer.md](<zh/api/SIMD-API/基础API/资源管理/TPipe/InitBuffer.md>)：72。
- [docs/zh/api/SIMD-API/基础API/资源管理/TPipe/InitSpmBuffer.md](<zh/api/SIMD-API/基础API/资源管理/TPipe/InitSpmBuffer.md>)：37、94、102。
- [docs/zh/api/SIMD-API/基础API/资源管理/TPipe/ReadSpmBuffer.md](<zh/api/SIMD-API/基础API/资源管理/TPipe/ReadSpmBuffer.md>)：37、99。
- [docs/zh/api/SIMD-API/基础API/资源管理/TPipe/WriteSpmBuffer.md](<zh/api/SIMD-API/基础API/资源管理/TPipe/WriteSpmBuffer.md>)：37、110、151。
- [docs/zh/api/SIMD-API/基础API/资源管理/TSCM/AllocTensor-60.md](<zh/api/SIMD-API/基础API/资源管理/TSCM/AllocTensor-60.md>)：31。
- [docs/zh/api/SIMD-API/基础API/资源管理/TSCM/FreeTensor-61.md](<zh/api/SIMD-API/基础API/资源管理/TSCM/FreeTensor-61.md>)：31。
- [docs/zh/api/SIMD-API/基础API/资源管理/TSCM/TSCM简介.md](<zh/api/SIMD-API/基础API/资源管理/TSCM/TSCM简介.md>)：3、67、69、70、72。
- [docs/zh/guide/index.md](<zh/guide/index.md>)：289。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/Matmul特性介绍.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/Matmul特性介绍.md>)：59。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/TSCM输入的矩阵乘.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/特性场景/TSCM输入的矩阵乘.md>)：5、9、13、17。
- [docs/zh/guide/算子实践参考/SIMD算子性能优化/优化建议总览表.md](<zh/guide/算子实践参考/SIMD算子性能优化/优化建议总览表.md>)：73。
- [docs/zh/guide/算子实践参考/SIMD算子性能优化/内存访问/纯搬运类算子VECIN和VECOUT建议复用.md](<zh/guide/算子实践参考/SIMD算子性能优化/内存访问/纯搬运类算子VECIN和VECOUT建议复用.md>)：1。

### `KEEP_COMPLIANT`：已经采用物理位置（逻辑位置）

同一行已经给出物理位置和对应逻辑位置，不需要重复整改。

共11行，6个文件。

- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/Mmad.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/Mmad.md>)：160、171。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/MmadBitMode.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/MmadBitMode.md>)：116、135、177。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/MmadMx.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/MmadMx.md>)：152、180。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/MmadWithSparse.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/Mmad计算/MmadWithSparse.md>)：120。
- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md>)：21、22。
- [docs/zh/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过BT-Buffer实现高效的bias计算.md](<zh/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过BT-Buffer实现高效的bias计算.md>)：5。

### `KEEP_PRODUCT`：产品相关CO2逻辑位置，正文已给出映射说明

CO2的物理映射随产品变化；上下文已明确使用逻辑位置并链接产品映射，不能硬写成单一物理位置。

共7行，1个文件。

- [docs/zh/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md](<zh/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md>)：32、35、140、141、159、597、627。

### `KEEP_DATA`：矩阵/数据块编号或产品代际名称

A1/A2/B1/B2等在这些行中是矩阵分块、图中数据块编号或A2/A3产品代际，不是TPosition。

共7行，5个文件。

- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopyPad_UBToL1.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopyPad_UBToL1.md>)：88、89。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopy_UBToL1_ND2NZ.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopy_UBToL1_ND2NZ.md>)：126。
- [docs/zh/api/SIMD-API/基础API/cube_compute_TensorAPI/矩阵计算的搬入/Copy_GMToL1.md](<zh/api/SIMD-API/基础API/cube_compute_TensorAPI/矩阵计算的搬入/Copy_GMToL1.md>)：133、168。
- [docs/zh/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过L0C-Buffer数据暂存实现高效的矩阵乘结果累加.md](<zh/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过L0C-Buffer数据暂存实现高效的矩阵乘结果累加.md>)：5。
- [docs/zh/guide/算子实践参考/典型算子实践案例/SIMD算子实现/MatmulLeakyRelu融合算子实践/高阶极限打磨.md](<zh/guide/算子实践参考/典型算子实践案例/SIMD算子实现/MatmulLeakyRelu融合算子实践/高阶极限打磨.md>)：26。

### `KEEP_PHYSICAL`：物理Buffer简称，不是逻辑位置

`BT Buffer`本身是物理存储简称；是否统一为`BiasTable Buffer`属于术语一致性问题，不属于本次物理/逻辑位置整改。

共2行，1个文件。

- [docs/zh/guide/编程指南/高级编程/硬件实现/基本架构.md](<zh/guide/编程指南/高级编程/硬件实现/基本架构.md>)：155、180。

## 原K组中的通路表达复核

以下11条已经给出物理通路，并以分层或括注方式补充逻辑TPosition通路，确认不修改。

### `KEEP_ROUTE_DETAIL_K`：物理通路与逻辑通路在同一处分别说明

这些行已经同时提供物理通路和逻辑TPosition通路，只是采用分层或整段括注表达；按本次确认口径保留。

共11行，5个文件。

- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/BroadCastVecToMM_ISASI.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/BroadCastVecToMM_ISASI.md>)：29。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopyPad_UBToL1.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬入/DataCopyPad_UBToL1.md>)：62、78、83、101。
- [docs/zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬出/DataCopy_L0CToUB.md](<zh/api/SIMD-API/基础API/cube_compute_ISASI/矩阵计算的搬出/DataCopy_L0CToUB.md>)：6。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/L1或L0C与UB数据搬运.md](<zh/api/SIMD-API/基础API/数据搬运导览/L1或L0C与UB数据搬运.md>)：27、47、65、83。
- [docs/zh/api/SIMD-API/基础API/数据搬运导览/概览/数据搬运概念.md](<zh/api/SIMD-API/基础API/数据搬运导览/概览/数据搬运概念.md>)：68。

## 完整性检查

- 原`R/M`候选数：903；语义复核结果：685条已修改 + 218条不修改 = 903。
- 原`K`候选复核：339条仍保持合规，其中11条物理/逻辑通路分层表达已单独列明。
- 最终完成整改：685条。
- 未决项：0。
