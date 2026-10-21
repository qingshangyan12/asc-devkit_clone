# C API Reg精度与低精度支持方案设计

## 目标与原则

- 面向Ascend 950PR&950DT系列产品，基于现有Basic API表1、表2，分别生成Reg C API精度标准表3和低精度支持表4。
- Basic API与C API是对同一套编译器指令的不同风格封装，但Basic API可能增加软仿分支，因此不能直接复制结果，必须核对公开原型、数据类型和最终指令。

## 生成方案

1. 按计算语义匹配表1与C API，如`Exp/asc_exp`、`MulDstAdd/asc_madd`，避免只按名称对应。
2. 对比两套实现的最终指令。数据类型、模式和指令一致时，表3沿用表1的硬件精度。
3. 检查算法配置及补偿分支。表3中的C API均直接调用编译器指令，软仿列填`-`；`Log2/Log10`无对应接口，不列入。
4. 按表2的数据类型核对C API文档和声明，只将实际存在的Reg重载写入表4；转换接口按方向列出，Int8和UInt8按功能分类。
5. 检查低精度接口实现。FP8广播直接调用`vdup`，没有软件组合，因此不沿用表2中`Duplicate`的软仿标记。

## 测试用例

- 接口测试：核对表4各接口的文档、声明和数据类型。预期：所有接口及重载存在，转换方向与表格一致。
- FP8测试：检查六类广播接口实现。预期：FP8重载直接调用`vdup`，不存在软件组合分支。
- 指令测试：逐项比较Basic API和C API硬件路径。预期：表3各组最终指令一致，可复用表1硬件精度。
- 语义测试：重点核对`asc_madd`、`asc_mula`及归约接口。预期：计算公式、操作数关系和映射名称正确。
- 软仿测试：搜索算法配置、补偿分支及Basic API转调。预期：表3对应C API无软仿，软仿列均为`-`。
- 文档测试：执行`git diff --check`并检查表格列数、标题和锚点。预期：无格式错误、缺行或重复锚点。

## 历史测试结果（错误结论，已撤回）

以下原始记录仅作误判存档，不代表验收通过。更正结果见[C_API_Reg精度与低精度支持测试结果.md](C_API_Reg精度与低精度支持测试结果.md)，原因见[C_API_Reg检查误判存档.md](C_API_Reg检查误判存档.md)。

- 接口测试：通过。表4中的转换、广播、算术和比较接口均能在`include/c_api`声明、`impl/c_api`实现或对应 C API 文档中找到；FP4、FP8、HiFloat8、Int4、Int8和UInt8的数据类型与接口方向一致。证据：`include/c_api/reg_compute`、`impl/c_api/reg_base_impl`。
- FP8测试：通过。`asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`和`asc_duplicate_scalar_merge`均存在 FP8 重载，并直接调用`vdup`，未发现 Basic API 转调或多指令软件组合。证据：`impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h`。
- 指令测试：通过。表3中`asc_exp`、`asc_sqrt`、`asc_add`、`asc_div`、`asc_ln`、`asc_mul`、`asc_mul_scalar`等接口直接调用对应的`vexp`、`vsqrt`、`vadd`、`vdiv`、`vln`、`vmul`/`vmuls`；`asc_madd`、`asc_mula`和归约接口分别调用`vmadd`、`vmula`和`vcadd`。因此相同数据类型和模式下可复用表1的硬件指令精度标准。证据：`impl/c_api/reg_base_impl/reg_arith_intf_impl.h`、`reg_fused_intf_impl.h`、`reg_reduce_intf_impl.h`。
- 语义测试：通过。`MulDstAdd`对应`asc_madd`，`MulAddDst`对应`asc_mula`，`Reduce`、`ReduceDataBlock`和`PairReduceElem`分别对应`asc_reduce_sum`、`asc_reduce_sum_datablock`和`asc_pair_reduce_sum`，操作数顺序和归约语义与表1一致。
- 软仿测试：通过。表3所列 C API 实现均为直接编译器指令调用，没有算法配置、数据范围判断、补偿计算或 Basic API 转调，因此软仿精度标准统一为`-`。`asc_ln`和`asc_log`实现均直接调用`vln`；表1的`Log2/Log10`没有对应 C API，未错误添加到表3。
- 文档测试：通过。已执行`git diff --check`；表3、表4列数一致，标题和锚点唯一，表1跳转链接有效。

## 历史结果结论（已撤回）

表3、表4当前内容通过上述核对，可以作为表1、表2的 C API 对应版本使用。复核时发现的关键约束已保留：C API 只记录公开 Reg 原型，Basic API 的软仿标记不直接继承，硬件精度仅在底层编译器指令一致时复用。
