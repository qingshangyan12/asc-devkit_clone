# C API

CANN 9.2.0在CANN 9.1.0的基础上继续增强C API。本版本新增矩阵与数据搬运配置、Reg计算与搬运、缓存和特殊寄存器控制等能力，为大量Reg接口补充函数返回值形式的重载，并统一公共头文件、接口命名和参数类型。同时，部分旧命名、隐式位置重载和同步复合接口进入废弃期。

## 新增特性

### 系统、缓存与控制

- [asc_disable_dma_atomic](../../SIMD-API/c_api/atomic/datamove_atomic/asc_disable_dma_atomic.md)：清空数据搬运原子操作状态，避免此前开启的原子操作影响后续搬运指令。
- [指令发射队列空闲槽位查询](../../SIMD-API/c_api/experimental/asc_get_pipe_idle_slot_count.md)：新增`asc_get_pipe_mte2_idle_slot_count`、`asc_get_pipe_mte3_idle_slot_count`、`asc_get_pipe_v_idle_slot_count`、`asc_get_pipe_m_idle_slot_count`、`asc_get_pipe_mte1_idle_slot_count`和`asc_get_pipe_fix_idle_slot_count`，用于查询指定流水线发射队列的空闲槽位数量。该组接口仅支持Ascend 950PR/Ascend 950DT，属于试验特性，使用时需包含`c_api/experimental/asc_get_pipe_idle_slot_count.h`并定义`CANN_ASC_USE_EXPERIMENTAL`。
- [asc_set_scalar_cache_mode](../../SIMD-API/c_api/cache_ctrl/asc_set_scalar_cache_mode.md)、[asc_get_scalar_load_cache_mode](../../SIMD-API/c_api/cache_ctrl/asc_get_scalar_load_cache_mode.md)、[asc_get_scalar_store_cache_mode](../../SIMD-API/c_api/cache_ctrl/asc_get_scalar_store_cache_mode.md)：支持设置和查询标量单元访问Global Memory时的L2 Cache管理策略。
- [asc_set_saturation_flag](../../SIMD-API/c_api/spr/asc_set_saturation_flag.md)、[asc_get_saturation_flag](../../SIMD-API/c_api/spr/asc_get_saturation_flag.md)、[asc_set_saturation_strategy](../../SIMD-API/c_api/spr/asc_set_saturation_strategy.md)、[asc_get_saturation_strategy](../../SIMD-API/c_api/spr/asc_get_saturation_strategy.md)：支持通过[asc_saturation_mode](../../SIMD-API/c_api/defs/enum/asc_saturation_mode.md)和[asc_override_strategy](../../SIMD-API/c_api/defs/enum/asc_override_strategy.md)设置、查询饱和模式开关及饱和控制策略。
- [asc_get_system_clock](../../SIMD-API/c_api/utils/sys_var/asc_get_system_clock.md)：读取系统时钟并按芯片频率换算，直接返回微秒级时间值。CANN 9.1.0中已有的`asc_get_system_cycle`仍用于读取原始cycle计数值。
- [asc_nop](../../SIMD-API/c_api/utils/sys_misc/asc_nop.md)：生成一条不执行实际计算的空操作指令。

### 矩阵计算与数据搬运

- [asc_disable_hf32](../../SIMD-API/c_api/cube_compute/asc_disable_hf32.md)、[asc_disable_hif8](../../SIMD-API/c_api/cube_compute/asc_disable_hif8.md)：分别关闭Mmad计算的HF32模式和HiF8模式。
- [asc_set_hf32_round_mode](../../SIMD-API/c_api/cube_compute/asc_set_hf32_round_mode.md)：使用`asc_hf32_round_mode`枚举配置HF32舍入方式。
- [asc_set_gm2l1_padding](../../SIMD-API/c_api/cube_datamove/asc_set_gm2l1_padding.md)、[asc_set_l12l0a_3d_padding](../../SIMD-API/c_api/cube_datamove/asc_set_l12l0a_3d_padding.md)、[asc_set_l12l0b_3d_padding](../../SIMD-API/c_api/cube_datamove/asc_set_l12l0b_3d_padding.md)和[asc_set_l13d_rpt_b](../../SIMD-API/c_api/cube_datamove/asc_set_l13d_rpt_b.md)：支持配置GM到L1 Buffer搬运填充值、L1 Buffer到L0A/L0B Buffer的3D搬运填充值，以及右矩阵3D搬运的repeat参数。
- [asc_set_l0c_copy_config](../../SIMD-API/c_api/cube_datamove/asc_set_l0c_copy_config.md)、[asc_set_l0c_copy_channel_para](../../SIMD-API/c_api/cube_datamove/asc_set_l0c_copy_channel_para.md)、[asc_set_l0c_copy_nz_para](../../SIMD-API/c_api/cube_datamove/asc_set_l0c_copy_nz_para.md)、[asc_set_l0c_copy_relu_alpha](../../SIMD-API/c_api/cube_datamove/asc_set_l0c_copy_relu_alpha.md)和[asc_set_l0c_copy_lrelu_alpha](../../SIMD-API/c_api/cube_datamove/asc_set_l0c_copy_lrelu_alpha.md)：支持配置L0C Buffer搬出过程中的量化、激活、UnitFlag、NZ到ND格式转换和Channel Split参数。
- [asc_load_dev](../../SIMD-API/c_api/scalar_compute/scalar_load/asc_load_dev.md)：支持不经过Data Cache直接从Global Memory读取整型标量数据。

### Memory矢量计算

- [asc_transto5hd](../../SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transto5hd.md)：将`asc_transto5hd_b8`、`asc_transto5hd_b16`和`asc_transto5hd_b32`正式加入公共头文件，支持NCHW到NC1HWC0格式转换及二维矩阵数据块转置。

### Reg计算与Reg数据搬运

- [asc_duplicate_highest](../../SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest.md)、[asc_duplicate_highest_merge](../../SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest_merge.md)、[asc_duplicate_merge](../../SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_merge.md)和[asc_duplicate_scalar_merge](../../SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md)：新增最高有效元素广播和保留目的操作数未选中元素的merge模式。
- [asc_mask](../../SIMD-API/c_api/reg_compute/reg_copy/asc_mask.md)和[asc_extract_mask](../../SIMD-API/c_api/reg_compute/reg_copy/asc_extract_mask.md)：支持通过函数返回值完成掩码复制，以及从矢量数据寄存器中提取掩码寄存器。
- [asc_pack_to_low](../../SIMD-API/c_api/reg_compute/reg_permute_sel/asc_pack_to_low.md)和[asc_pack_to_high](../../SIMD-API/c_api/reg_compute/reg_permute_sel/asc_pack_to_high.md)：以明确的接口名选择将窄化结果写入目的操作数的低半部分或高半部分。
- [asc_unsqueeze](../../SIMD-API/c_api/reg_compute/reg_permute_sel/asc_unsqueeze.md)新增`asc_unsqueeze_s8`、`asc_unsqueeze_u8`、`asc_unsqueeze_s16`、`asc_unsqueeze_u16`、`asc_unsqueeze_s32`和`asc_unsqueeze_u32`，支持通过函数返回值获取掩码前缀计数结果。
- [asc_fma](../../SIMD-API/c_api/reg_compute/reg_fused/asc_fma.md)、[asc_mula](../../SIMD-API/c_api/reg_compute/reg_fused/asc_mula.md)和[asc_mul_scalar_float2half_rn](../../SIMD-API/c_api/reg_compute/reg_fused/asc_mul_scalar_float2half_rn.md)：分别支持三源融合乘加、目的操作数参与累加，以及`float`矢量与标量相乘后转换为`half`的融合计算。其中，`asc_fma`在CANN 9.1.0中仅有Memory矢量计算接口，本版本新增的是Reg接口。
- [asc_exp_sub](../../SIMD-API/c_api/reg_compute/reg_fused/asc_exp_sub.md)新增`asc_exp_sub_half2float`接口，支持显式选择`half`源操作数的奇数或偶数索引元素，转换为`float`后计算指数差；[asc_log](../../SIMD-API/c_api/reg_compute/reg_arith/asc_log.md)新增Reg自然对数计算接口。
- [asc_squeeze_and_storeunalign](../../SIMD-API/c_api/reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)、[asc_squeeze_and_storeunalign_init](../../SIMD-API/c_api/reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_init.md)和[asc_squeeze_and_storeunalign_finalize](../../SIMD-API/c_api/reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)：支持筛选矢量寄存器中的有效元素，并连续搬出到非32字节对齐的UB地址。
- [asc_get_squeeze_status](../../SIMD-API/c_api/spr/asc_get_squeeze_status.md)、[asc_storealign_squeeze_status](../../SIMD-API/c_api/reg_compute/store/asc_storealign_squeeze_status.md)和[asc_storealign_squeeze_status_postupdate](../../SIMD-API/c_api/reg_compute/store/asc_storealign_squeeze_status_postupdate.md)：支持读取或搬出squeeze操作保存在AR特殊寄存器中的有效数据长度。
- Reg对齐搬入新增语义明确的模式接口，包括`asc_loadalign_brc_datablock`、`asc_loadalign_brc_elem`、`asc_loadalign_brc_elem2datablock`、`asc_loadalign_unpack4`及对应的`asc_loadalign_brc_datablock_postupdate`、`asc_loadalign_brc_elem_postupdate`、`asc_loadalign_brc_elem2datablock_postupdate`、`asc_loadalign_unpack4_postupdate`；新增[asc_loadalign_datablock_strided](../../SIMD-API/c_api/reg_compute/load/asc_loadalign_datablock_strided.md)、[asc_loadalign_mask](../../SIMD-API/c_api/reg_compute/load/asc_loadalign_mask.md)、[asc_loadalign_mask_downsample](../../SIMD-API/c_api/reg_compute/load/asc_loadalign_mask_downsample.md)和[asc_loadalign_mask_upsample](../../SIMD-API/c_api/reg_compute/load/asc_loadalign_mask_upsample.md)返回值接口。
- [asc_storealign_pack_quarter](../../SIMD-API/c_api/reg_compute/store/asc_storealign_pack_quarter.md)和[asc_storealign_pack_quarter_postupdate](../../SIMD-API/c_api/reg_compute/store/asc_storealign_pack_quarter_postupdate.md)：支持将32位Reg数据的低8位压缩搬出到UB，接口名显式表达四分之一宽度压缩语义。
- [asc_update_addr_reg](../../SIMD-API/c_api/reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口族新增`asc_update_addr_reg_b8`、`asc_update_addr_reg_b16`和`asc_update_addr_reg_b32`，使用`addr_reg`保存并更新循环中的地址偏移量，替代原`iter_reg`接口族。

## 增强

### Reg计算

- 为Reg算术、比较、逻辑、融合、规约、广播、类型转换、数据搬入和排布变换接口补充函数返回值形式的重载。涉及`asc_add`、`asc_sub`、`asc_mul`、`asc_div`、`asc_max`、`asc_min`、`asc_abs`、`asc_neg`、`asc_sqrt`、`asc_exp`、比较与标量比较接口、`asc_and`、`asc_or`、`asc_not`、`asc_xor`、移位接口、`asc_abs_sub`、`asc_relu`、规约接口、`asc_duplicate`、`asc_duplicate_scalar`、`asc_load`、`asc_loadalign`、`asc_gather`、`asc_gather_datablock`、`asc_half2int16`及其他Reg类型转换接口。
- `asc_div`新增`int16_t`、`uint16_t`、`int32_t`和`uint32_t`重载；`asc_gather`新增`vector_float`相关重载，并按UB源收集和寄存器源收集拆分资料及头文件归属。UB源收集接口使用`c_api/reg_compute/gather/ub_gather.h`，寄存器源收集接口使用`c_api/reg_compute/compute/reg_gather.h`。
- [asc_min_scalar](../../SIMD-API/c_api/reg_compute/reg_arith/asc_min_scalar.md)和[asc_ne](../../SIMD-API/c_api/reg_compute/reg_compare/asc_ne.md)新增`bfloat16_t`重载。
- `asc_duplicate`、`asc_duplicate_scalar`新增保留目的操作数未选中元素的merge模式，并补充函数返回值形式。
- 类型转换接口补充显式源/目的位置参数和函数返回值形式，统一由`asc_position_mode`或`asc_position_quarter_mode`表达位置，不再依赖`_v2`、`_v3`、`_v4`后缀推断位置。

### 矩阵计算与数据搬运

- [asc_mmad](../../SIMD-API/c_api/cube_compute/asc_mmad.md)和[asc_mmad_mx](../../SIMD-API/c_api/cube_compute/asc_mmad_mx.md)新增显式传入Bias起始地址的重载，支持使用BiasTable Buffer中的Bias矩阵初始化结果矩阵。
- `asc_copy_gm2l1`、`asc_copy_gm2l1_align`、`asc_copy_gm2l1_nd2nz`、`asc_copy_gm2l1_dn2nz`以及L0C Buffer搬出接口补充参数化重载，支持使用枚举配置L2 Cache、量化、激活、格式转换和UnitFlag等搬运行为。
- `asc_copy_l0c2l1`补充Ascend 950PR/Ascend 950DT的矩阵搬出重载及随路配置能力。该接口名在CANN 9.1.0中已经存在，本版本增强的是3510架构的原型和实现。
- 矩阵与矢量搬运配置新增`asc_load_l2_cache_mode`、`asc_store_l2_cache_mode`、`asc_channel_pad_mode`、`asc_l13d_repeat_direction`、`asc_unit_flag_mode`、`asc_quant_mode`、`asc_relu_pre_mode`和`asc_dual_dst_mode`等强类型枚举或别名，减少使用无类型整数配置硬件行为的歧义。
- `asc_hf32_round_mode`统一为`NEAREST_AWAY`和`NEAREST_EVEN`。旧枚举值`NEAREST_ZERO`作为兼容别名保留，其实际行为等同于`NEAREST_AWAY`。

### 系统与同步

- [asc_get_arch_ver](../../SIMD-API/c_api/utils/sys_var/asc_get_arch_ver.md)改为返回Ascend C对外使用的架构版本号，2201架构返回`2201`，3510架构返回`3510`，不再直接暴露芯片寄存器中的内部版本字段。
- [asc_sync_pipe](../../SIMD-API/c_api/sync/asc_sync_pipe.md)补齐2201架构Vector Core的`PIPE_V`单流水同步，并修正3510架构[asc_sync_vec](../../SIMD-API/c_api/sync/asc_sync_vec.md)带`id`重载中的冗余Vector流水屏障。
- [asc_lock](../../SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../SIMD-API/c_api/sync/asc_unlock.md)新增使用`asc_mutex_execute_mode`的重载，以强类型参数选择阻塞或非阻塞模式；原`ascMutexExecuteMode`类型名作为兼容别名保留。

### 公共头文件

- 公共头文件按`defs`、`utils`、`cube_compute`、`cube_datamove`、`vector_compute`、`vector_datamove`、`reg_compute`、`scalar_compute`、`atomic`、`cache_ctrl`、`sync`和`spr`等模块拆分，API资料同步使用最细粒度公共头文件路径。
- `c_api/asc_simd.h`仍作为全局聚合入口；新增`c_api/asc_cube.h`、`c_api/asc_memory_vector.h`和`c_api/asc_reg_vector.h`作为分域聚合入口。已有代码可以继续包含全局聚合头，新代码可按使用范围选择更细粒度的头文件。
- 旧兼容头`c_api/misc/misc.h`、`c_api/utils_intf.h`、`c_api/utils/union.h`、`c_api/sys_var/sys_var.h`、`c_api/reg_compute/reg_vector.h`、`c_api/reg_compute/reg_load.h`和`c_api/reg_compute/reg_store.h`仍保留，但会给出废弃提示；建议迁移到对应的`c_api/utils/*`、`c_api/defs/*`、`c_api/reg_compute/reg_compute.h`或Reg搬入、搬出、聚合、离散操作的最细粒度公共头文件。

## Bug修复

- 修复3510架构下`asc_copy_gm2ub_align`、`asc_copy_ub2gm_align`等对齐搬运接口的`n_burst`参数位宽不足问题，将相关重载的`n_burst`扩展为`uint32_t`。
- 修复3510架构下[asc_store](../../SIMD-API/c_api/reg_compute/store/asc_store.md)非对齐搬出时未执行地址Post Update后处理而导致搬运结果错误的问题。
- 修复Post Update搬运接口的地址或地址寄存器按值传递问题：[asc_loadunalign_postupdate](../../SIMD-API/c_api/reg_compute/load/asc_loadunalign_postupdate.md)的UB源地址、`addr_reg`偏移，以及`asc_storealign_postupdate`、[asc_storealign_1st_postupdate](../../SIMD-API/c_api/reg_compute/store/asc_storealign_1st_postupdate.md)、[asc_storealign_pack_postupdate](../../SIMD-API/c_api/reg_compute/store/asc_storealign_pack_postupdate.md)的UB目的地址改为引用传递，确保接口执行后的更新结果可回写调用方。
- 修复`asc_loadalign_postupdate`的`vector_bool`重载缺少`offset`参数的问题，使公开声明与实现保持一致。
- 修复`asc_squeeze_and_storeunalign`的目的矢量数据寄存器`dst`按值传递问题，改为引用传递，确保压缩结果可回写调用方。
- 修复2201架构下`asc_sync_subblock_wait`宏向底层接口多传`pipe`参数的问题。
- 修复2201架构下部分`asc_sub_relu_sync`重载错误调用非同步实现的问题。
- 修复2201架构下[asc_rsqrt](../../SIMD-API/c_api/vector_compute/vector_arith/asc_rsqrt.md)同步重载误调用非同步实现的问题。
- 修复[asc_init](../../SIMD-API/c_api/utils/sys_init/asc_init.md)在3510架构下未完整复位`CTRL`、Mask、搬运循环、Padding及数据搬运原子操作类型等状态的问题，避免前序算子遗留状态影响当前算子。
- 修复[asc_l13d_fmatrix_config](../../SIMD-API/c_api/defs/union/asc_l13d_fmatrix_config.md)中`l1_width`和`l1_height`字段顺序错误导致配置值与预期不一致的问题。
- 修复[asc_copy_gm2l1](../../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1.md)和[asc_copy_l0c2l1](../../SIMD-API/c_api/cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1.md)部分3510实现调用底层CCE指令时缺少参数的问题。
- 修复[asc_mrgsort4](../../SIMD-API/c_api/vector_compute/vector_sort/asc_mrgsort4.md)的源操作数声明与四路归并语义不一致的问题，`src`调整为包含4个UB地址的指针数组。
- 将原子接口中的`ASC_C_API_DEFAULIT_OFFSET`拼写修正为`ASC_C_API_DEFAULT_OFFSET`，并统一`int32_t`数据搬运原子接口的`_int32`命名。

## 接口废弃

本版本新增以下废弃标记。完整函数原型、产品支持情况和替代链接请参见[废弃接口](../deprecated_interface.md)。

| 废弃接口、重载或头文件 | 替代接口或迁移方式 | 迁移说明 |
| --- | --- | --- |
| `asc_set_atomic_none` | `asc_disable_dma_atomic` | 使用语义明确的原子操作关闭接口。 |
| `asc_set_atomic_add_int`、`asc_set_atomic_max_int`、`asc_set_atomic_min_int` | `asc_set_atomic_add_int32`、`asc_set_atomic_max_int32`、`asc_set_atomic_min_int32` | 使用带明确数据类型后缀的接口。 |
| `asc_get_block_num`、`asc_get_block_idx` | 内置变量`block_num`、`block_idx`；混合核Vector Core索引配合`asc_get_sub_block_num`和`asc_get_sub_block_id`计算 | 系统内置变量替代函数式查询。 |
| `asc_get_ar_spr`、`asc_clear_ar_spr` | `asc_get_squeeze_status`、`asc_squeeze_and_storeunalign_init` | 使用语义明确的squeeze状态读写接口。 |
| `iter_reg`、`asc_create_iter_reg_b8`、`asc_create_iter_reg_b16`、`asc_create_iter_reg_b32` | `addr_reg`、`asc_update_addr_reg_b8`、`asc_update_addr_reg_b16`、`asc_update_addr_reg_b32` | 地址寄存器类型和初始化接口统一更名。 |
| `asc_loadalign_brc_v2`、`asc_loadalign_brc_v3`、`asc_loadalign_unpack_v2`及其Post Update重载 | `asc_loadalign_brc_datablock`、`asc_loadalign_brc_elem2datablock`、`asc_loadalign_unpack4`及对应Post Update接口 | 接口名直接表达搬入模式，避免通过版本后缀区分语义。 |
| `asc_storealign_pack_v2`、`asc_storealign_pack_postupdate_v2` | `asc_storealign_pack_quarter`、`asc_storealign_pack_quarter_postupdate` | 使用接口名明确表达四分之一宽度的pack模式。 |
| `asc_storeunalign_post`及部分`asc_storeunalign_postupdate`旧重载 | `asc_squeeze_and_storeunalign`、`asc_squeeze_and_storeunalign_finalize`，或当前同名B16/B32重载 | 统一有效元素筛选与非对齐连续搬出流程；B8目的地址且源为`vector_bool`的重载不再推荐。 |
| `asc_pack`、`asc_pack_v2` | `asc_pack_to_low`、`asc_pack_to_high` | 使用接口名显式表达写入目的操作数的低半部分或高半部分。 |
| `asc_squeeze_with_status`、`asc_squeeze_v2` | `asc_squeeze_and_storeunalign`接口族，或当前`asc_squeeze`重载 | 按是否需要连续搬出选择替代接口。 |
| `asc_muls`、`asc_muls_v2` | `asc_mul_scalar_float2half_rn` | 使用语义明确并支持显式位置参数的融合接口。 |
| `asc_exp_sub`、`asc_exp_sub_v2`的`half`转`float`重载 | `asc_exp_sub_half2float` | 使用函数名明确表达输入、输出数据类型。 |
| `asc_and`、`asc_or`的浮点类型重载 | 按相同位宽选择整数类型重载 | 位逻辑运算不再接受浮点矢量类型。 |
| Reg类型转换中通过接口后缀或隐式规则控制源/目的位置的重载 | 显式传入`asc_position_mode`或`asc_position_quarter_mode`的当前同名重载 | 涉及`float`、`half`、`bfloat16_t`、整数、FP8、HiF8和FP4等类型转换接口；`_v2`、`_v3`、`_v4`位置后缀进入废弃期。 |
| `asc_mmad`、`asc_mmad_s4`、`asc_mmad_sparse`、`asc_mmad_mx`中`unit_flag`为`uint8_t`、带`feat_offset`参数的旧原型 | `unit_flag_mode`为`asc_unit_flag_mode`的当前同名接口 | 使用强类型枚举配置UnitFlag；不再传入无效的兼容参数。 |
| 矩阵计算、矩阵搬运、矢量数据搬运和Memory矢量计算的`*_sync`复合接口 | 对应非同步接口配合`asc_sync()` | 包括`asc_mmad*_sync`、`asc_copy_*_sync`、`asc_fill_*_sync`和Memory矢量计算`*_sync`接口族。 |
| `asc_sync_vec`旧原型 | `asc_sync` | 使用统一的同步接口。 |
| `asc_copy_gm2ub_align`、`asc_copy_ub2gm_align`、`asc_ndim_copy_gm2ub`中以整数参数表达L2 Cache策略的旧重载 | 使用`asc_load_l2_cache_mode`或`asc_store_l2_cache_mode`枚举的当前同名重载 | 使用强类型枚举配置L2 Cache管理策略。 |
| `c_api/misc/misc.h`、`c_api/utils_intf.h`、`c_api/utils/union.h`、`c_api/sys_var/sys_var.h`、`c_api/reg_compute/reg_vector.h`、`c_api/reg_compute/reg_load.h`、`c_api/reg_compute/reg_store.h` | 对应的`c_api/utils/*`、`c_api/defs/*`、`c_api/reg_compute/reg_compute.h`或Reg搬入、搬出、聚合、离散操作的最细粒度公共头文件 | 旧头文件仅作为兼容入口保留，新代码应使用新的模块化公共头文件。 |
