# C API

CANN 9.0.0首次面向Ascend 950PR/Ascend 950DT（3510架构）提供实验性C API。接口覆盖矢量与矩阵数据搬运、Reg矢量计算、矩阵计算、标量计算、同步、系统变量、缓存控制和数据搬运原子操作等能力，支持基于指针和数组的C语言编程方式。

## 特性

### 配置数据结构

- `asc_set_l0c_copy_params_config`：封装L0C搬运时的NZ2ND格式转换参数。
- `asc_load3d_v2_config`：封装Load3Dv2的重复执行参数。
- `asc_ndim_pad_count_config`：封装N维搬运各层循环的左右填充元素个数。
- `asc_l13d_fmatrix_config`：封装L1 3D搬运的Feature Map尺寸及四周填充参数。

### 矢量数据搬运

- `asc_copy_gm2ub_align` / `asc_copy_gm2ub_align_sync`：将数据从Global Memory搬运到Unified Buffer，支持按字节指定搬运长度、左右填充、填充值模式、L2 Cache策略和源/目的步长。
- `asc_copy_ub2gm_align` / `asc_copy_ub2gm_align_sync`：将数据从Unified Buffer搬运到Global Memory，支持按字节指定搬运长度、L2 Cache策略和源/目的步长。
- `asc_ndim_copy_gm2ub`：执行GM到UB的N维搬运，最多支持五个维度，并可分别配置各维度的源/目的步长和左右填充。
- `asc_set_gm2ub_loop_size`、`asc_set_gm2ub_loop1_stride`、`asc_set_gm2ub_loop2_stride`：配置GM到UB的循环搬运参数。
- `asc_set_ub2gm_loop_size`、`asc_set_ub2gm_loop1_stride`、`asc_set_ub2gm_loop2_stride`：配置UB到GM的循环搬运参数。
- `asc_set_gm2ub_pad`：配置GM到UB搬运的填充值。
- `asc_set_ndim_loop0_stride`、`asc_set_ndim_loop1_stride`、`asc_set_ndim_loop2_stride`、`asc_set_ndim_loop3_stride`、`asc_set_ndim_loop4_stride`：分别配置N维搬运五个维度的源/目的步长。
- `asc_set_ndim_pad_count` / `asc_set_ndim_pad_value`：配置N维搬运各维度的左右填充元素个数和固定填充值。

### Reg矢量计算

提供使用`__simd_callee__`限定符的寄存器级搬运和计算接口。接口覆盖`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`、`bfloat16_t`、FP8和FP4等数据类型，具体支持组合以各接口说明为准。

**寄存器与掩码：**

- `asc_create_iter_reg_b8` / `asc_create_iter_reg_b16` / `asc_create_iter_reg_b32`：使用一个地址偏移初始化地址寄存器。
- `asc_create_mask_b8` / `asc_create_mask_b16` / `asc_create_mask_b32`：根据掩码模式生成掩码寄存器。
- `asc_get_mask_spr_b16` / `asc_get_mask_spr_b32`：读取特殊寄存器MASK0和MASK1并返回对应的掩码值。
- `asc_update_mask_b8` / `asc_update_mask_b16` / `asc_update_mask_b32`：根据有效元素个数更新掩码寄存器。

**Reg数据搬入：**

- `asc_loadalign`：从UB的32字节对齐地址加载数据。
- `asc_loadalign_brc` / `asc_loadalign_brc_v2` / `asc_loadalign_brc_v3`：从UB的32字节对齐地址加载数据并执行广播。
- `asc_loadalign_unpack` / `asc_loadalign_unpack_v2`：从UB的32字节对齐地址加载数据并执行解包。
- `asc_loadalign_upsample` / `asc_loadalign_downsample` / `asc_loadalign_deintlv`：从UB的32字节对齐地址加载数据并执行上采样、下采样或解交织。
- `asc_loadalign_postupdate`、`asc_loadalign_brc_postupdate`、`asc_loadalign_brc_postupdate_v2`、`asc_loadalign_brc_postupdate_v3`：完成对齐加载或广播后，按偏移更新源地址。
- `asc_loadalign_unpack_postupdate` / `asc_loadalign_unpack_postupdate_v2`、`asc_loadalign_upsample_postupdate`、`asc_loadalign_downsample_postupdate`、`asc_loadalign_deintlv_postupdate`：完成解包、上采样、下采样或解交织加载后，按偏移更新源地址。
- `asc_loadunalign_pre` / `asc_loadunalign` / `asc_loadunalign_postupdate`：初始化非对齐加载状态、从UB非32字节对齐地址加载数据，并可按偏移更新源地址。
- `asc_load`：从UB加载一个矢量长度的数据，不要求源地址32字节对齐。
- `asc_gather` / `asc_gather_datablock`：根据元素索引或DataBlock索引从UB收集数据到矢量寄存器。

**Reg数据搬出：**

- `asc_storealign`：从矢量寄存器向UB对齐地址写出数据，支持NORM、FIRST、PACK、PACKV2和INTLV模式。
- `asc_storealign_1st`：FIRST模式，忽略mask，仅将源矢量寄存器的第一个元素写入UB。
- `asc_storealign_pack` / `asc_storealign_pack_v2`：按照mask将有效元素的低半部分bit或低8 bit连续写入UB。
- `asc_storealign_intlv`：将两个源矢量寄存器的元素交织写入UB。
- `asc_storealign_postupdate` / `asc_storealign_1st_postupdate`：完成普通写出或FIRST模式写出后，按偏移更新目的地址。
- `asc_storealign_pack_postupdate` / `asc_storealign_pack_postupdate_v2`：完成PACK或PACKV2模式写出后，按偏移更新目的地址。
- `asc_storeunalign`：向UB的非32字节对齐地址写出主块数据。
- `asc_storeunalign_postupdate`：向UB的非32字节对齐地址写出主块数据，并按偏移更新目的地址。
- `asc_storeunalign_post` / `asc_storeunalign_post_postupdate`：写出非对齐搬运暂存的尾块；后者同时更新目的地址。
- `asc_scatter`：根据索引将源矢量寄存器中的元素分散写入UB。
- `asc_store`：从矢量寄存器向UB写出数据，不区分地址是否32字节对齐；对性能敏感的场景优先选择匹配的对齐或非对齐接口。
- `asc_copy`：在矢量寄存器之间复制数据。

**计算能力：**

- 基础算术：`asc_add`、`asc_addc`、`asc_add_scalar`、`asc_sub`、`asc_subc`、`asc_mul`、`asc_mul_scalar`、`asc_madd`、`asc_mull`、`asc_max`、`asc_max_scalar`、`asc_min`、`asc_min_scalar`、`asc_abs`、`asc_neg`、`asc_abs_sub`、`asc_axpy`、`asc_relu`、`asc_leakyrelu`、`asc_prelu`、`asc_exp`、`asc_ln`、`asc_sqrt`、`asc_div`。
- 逻辑与移位：`asc_and`、`asc_or`、`asc_xor`、`asc_not`、`asc_shiftleft`、`asc_shiftleft_scalar`、`asc_shiftright`、`asc_shiftright_scalar`。
- 比较与选择：`asc_eq`、`asc_ne`、`asc_lt`、`asc_le`、`asc_gt`、`asc_ge`、`asc_eq_scalar`、`asc_ne_scalar`、`asc_lt_scalar`、`asc_le_scalar`、`asc_gt_scalar`、`asc_ge_scalar`、`asc_select`、`asc_squeeze` / `asc_squeeze_v2`。
- 类型转换：支持float、half、bfloat16、FP8、FP4以及有符号/无符号整数之间的转换；接口名明确区分向下、向上、向零、最近偶数和最近远离零舍入模式，以及饱和与非饱和模式。
- 浮点取整：`asc_ceil`、`asc_floor`、`asc_rint`、`asc_round`、`asc_trunc`，结果仍保持源浮点数据类型。
- 归约：`asc_reduce_max`、`asc_reduce_min`、`asc_reduce_sum`、`asc_reduce_max_datablock`、`asc_reduce_min_datablock`、`asc_reduce_sum_datablock`、`asc_pair_reduce_sum`。
- 压缩与解压：`asc_pack` / `asc_pack_v2`、`asc_unsqueeze`。
- 填充与重排：`asc_duplicate`、`asc_duplicate_scalar`、`asc_intlv`、`asc_deintlv`。
- 直方图与索引：`asc_cumulative_histogram`、`asc_frequency_histogram`、`asc_arange`、`asc_arange_descend`。
- Reg流水同步和状态清理：`asc_mem_bar`、`asc_clear_ar_spr`。

### 矩阵数据搬运

- `asc_set_l13d_rpt` / `asc_set_l13d_fmatrix`：配置L1 3D搬运的重复执行参数和Feature Map属性。
- `asc_set_l0c_copy_prequant` / `asc_set_l0c_copy_params`：配置L0C搬运的预量化参数和NZ2ND格式转换参数。
- `asc_copy_l0c2l1`：将矩阵计算结果从L0C Buffer搬运到L1 Buffer，支持量化、ReLU、clip_relu、通道拆分、NZ2ND和逐元素反量化随路控制。`clip_relu`表示裁剪式ReLU控制，不是独立的通用裁剪操作。
- `asc_copy_l12l0a_mx`：将MX量化的scale矩阵从L1 Buffer搬运到L0A Buffer。
- `asc_set_l0c2gm_config`：配置L0C搬运使用的随路量化参数。
- `asc_set_l0c2gm_nz2nd`：配置L0C搬运使用的NZ2ND格式转换参数。
- `asc_set_gm2l1_loop_size`、`asc_set_gm2l1_loop1_stride`、`asc_set_gm2l1_loop2_stride`、`asc_set_gm2l1_pad`：配置GM到L1 Buffer搬运的循环参数和填充值。

### 矩阵计算

- `asc_mmad_mx` / `asc_mmad_mx_sync`：执行带MX scale的矩阵乘加，支持FP4和FP8输入组合。

### 标量计算

- `asc_sflbits`：统计从最高数值位开始、与符号位相同的连续bit数。
- `asc_ffz` / `asc_ffs`：从最低有效位开始查找第一个0 / 第一个1的位置，未找到时返回-1。
- `asc_popc`：统计`uint64_t`值中为1的bit数。
- `asc_store_dev`：将标量数据写入Global Memory。

### 同步控制

- `asc_sync` / `asc_sync_vec`：等待所有流水线操作完成。
- `asc_sync_mte2` / `asc_sync_mte3`：等待指定MTE流水线操作完成。
- `asc_sync_notify` / `asc_sync_wait`：设置和等待流水同步标志。
- `asc_sync_pipe`：等待指定流水线操作完成。

### 系统变量

- `asc_get_ctrl` / `asc_set_ctrl`：读取CTRL控制寄存器，并在保留原值的基础上修改所需控制位域。
- `asc_get_block_num`：获取AI核数。
- `asc_get_system_cycle`：获取当前系统cycle数。
- `asc_get_program_counter`：获取程序计数器的值。
- `asc_get_ar_spr`：读取指定特殊寄存器的值。

### 缓存控制

- `asc_icache_preload` / `asc_get_icache_preload_status`：预取指令Cache并查询预取状态。
- `asc_datacache_preload`：将指定GM数据预取到Data Cache。
- `asc_dcci_single` / `asc_ub_dcci_single`：分别刷新指定GM地址或UB地址所在的单个Cache Line。
- `asc_dcci_entire_all` / `asc_dcci_entire_out` / `asc_dcci_entire_atomic`：分别以ALL、OUT或ATOMIC模式刷新全部Cache Line，以维护Scalar访问与Global Memory之间的Cache一致性。

### 数据搬运原子操作

- `asc_set_atomic_add_float` / `asc_set_atomic_add_float16`：为后续到GM的数据搬运配置float / half原子累加。
- `asc_set_atomic_max_float` / `asc_set_atomic_max_float16`：为后续到GM的数据搬运配置float / half原子最大值操作。
- `asc_set_store_atomic_config_v2`：通过数据类型和操作码配置后续从UB、L0C或L1到GM的数据搬运原子操作。

### 其他操作

- `asc_init`：初始化NPU状态。
