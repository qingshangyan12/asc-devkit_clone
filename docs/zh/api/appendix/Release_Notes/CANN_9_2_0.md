# C API

CANN 9.2.0面向Ascend 950PR/Ascend 950DT（3510架构）持续增强C API，新增数据搬运原子关闭、发射队列空闲槽位查询和矩阵搬运能力；补充Reg计算返回值重载及矩阵乘加Bias输入；同时调整部分接口签名并废弃不再推荐的旧接口或重载。

## 新增特性

### 原子操作与系统变量

- [asc_disable_dma_atomic](../../SIMD-API/c_api/atomic/datamove_atomic/asc_disable_dma_atomic.md)：关闭此前开启的数据搬运原子操作，避免影响后续搬运指令。
- [指令发射队列空闲槽位查询](../../SIMD-API/c_api/experimental/asc_get_pipe_idle_slot_count.md)：提供`asc_get_pipe_mte2_idle_slot_count`、`asc_get_pipe_mte3_idle_slot_count`、`asc_get_pipe_v_idle_slot_count`、`asc_get_pipe_m_idle_slot_count`、`asc_get_pipe_mte1_idle_slot_count`和`asc_get_pipe_fix_idle_slot_count`，用于查询指定流水线发射队列的空闲槽位数量。该组接口为试验特性，使用时需包含`c_api/experimental/asc_get_pipe_idle_slot_count.h`并定义`CANN_ASC_USE_EXPERIMENTAL`。
- `asc_get_system_cycle`：读取系统周期计数值。

### 数据搬运与标量操作

- `asc_copy_l0c2l1`、`asc_copy_l0c2l1_sync`：新增L0C Buffer到L1 Buffer的矩阵搬运能力。
- `asc_load_dev`：新增标量读取Global Memory数据的接口。
- `asc_duplicate_highest`、`asc_duplicate_highest_merge`：新增按最高有效元素复制的矢量寄存器操作。

## 增强

### Reg计算

- 为部分Reg算术、比较、融合和规约接口补充返回值形式的重载，调用方可直接接收矢量计算结果。涉及`asc_abs`、`asc_add_scalar`、`asc_ln`、`asc_log`、`asc_max_scalar`、`asc_min_scalar`、`asc_mul_scalar`、`asc_eq`、`asc_ge_scalar`、`asc_gt_scalar`、`asc_ne_scalar`、`asc_abs_sub`、`asc_axpy`、`asc_madd`、`asc_relu`及规约类接口。
- `asc_gather`按源操作数类型拆分为寄存器源收集和UB源收集两类资料与头文件归属；UB源收集接口继续使用`c_api/reg_compute/gather/ub_gather.h`。
- `asc_squeeze_and_storeunalign`、`asc_squeeze_and_storeunalign_init`和`asc_squeeze_and_storeunalign_finalize`的目的操作数改为输出引用参数，调用后可正确获取更新结果。

### 矩阵计算与数据搬运

- `asc_mmad`、`asc_mmad_mx`新增可传入Bias操作数的重载，支持在矩阵乘加计算中使用Bias数据。
- `asc_copy_gm2l1`及相关矩阵搬运、随路配置接口补充参数化重载，支持更多3510架构的搬运与配置场景。
- 3510架构的对齐GM/UB搬运接口将`burst_count`参数扩展为`uint32_t`，并统一连续搬运接口的`burst_count`、`burst_len`参数命名。
- HF32舍入模式枚举值名称已统一。使用`asc_enable_hf32_trans`或`asc_set_hf32_round_mode`配置HF32模式时，请使用当前`asc_hf32_round_mode`枚举定义。

## Bug修复

- 修复`asc_copy_gm2ub_align`、`asc_copy_ub2gm_align`等对齐搬运接口在3510架构下的`burst_count`参数位宽不足问题。
- 修复`asc_squeeze_and_storeunalign`系列接口目的操作数未以引用形式输出的问题。
- 完善3510架构的矩阵数据搬运和矩阵计算接口文档、约束及调用示例。

## 接口废弃

本版本标记以下接口或重载为`deprecated`。详情请参见[废弃接口](../deprecated_interface.md)。

| 废弃接口或重载 | 替代接口或迁移方式 | 说明 |
| :--- | :--- | :--- |
| `asc_set_atomic_none` | `asc_disable_dma_atomic` | 使用语义明确的原子操作关闭接口。 |
| `asc_set_atomic_add_int`、`asc_set_atomic_max_int`、`asc_set_atomic_min_int` | `asc_set_atomic_add_int32`、`asc_set_atomic_max_int32`、`asc_set_atomic_min_int32` | 使用带明确数据类型后缀的接口。 |
| `asc_mmad`、`asc_mmad_mx`、`asc_mmad_sparse`中`unit_flag`为`uint8_t`、带`feat_offset`参数及旧同步原型 | `unit_flag_mode`为`asc_unit_flag_mode`的当前同名接口；同步场景配合`asc_sync()` | 迁移到当前头文件中声明的新原型。 |
| 通过接口后缀控制源或目的元素位置的部分类型转换重载 | 显式传入位置参数的当前重载 | 涉及`asc_bfloat162float`、`asc_bfloat162int32`、`asc_float2bfloat16`、`asc_float2half`、`asc_float2int16`、`asc_float2int64`、`asc_half2float`、`asc_half2hif8`、`asc_half2int32`、`asc_half2int8`、`asc_half2uint8`等接口。 |
| B8目的地址且源操作数为`vector_bool`的`asc_storeunalign_postupdate`重载 | B16或B32重载 | 不再推荐使用该B8掩码非对齐搬出重载。 |
