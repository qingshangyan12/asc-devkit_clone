# C API

CANN 9.1.0继续完善Ascend 950PR、Ascend 950DT（3510架构）C API，新增矢量与Cube数据搬运、Memory矢量计算、标量原子操作、同步和系统变量接口，并修复9.0.0中的函数命名、类型声明和地址自更新签名问题。

## 新增特性

### 矢量数据搬运

- `asc_copy_gm2ub`、`asc_copy_gm2ub_sync`：将数据从Global Memory搬运到Unified Buffer，提供多段搬运和按总字节数搬运两类接口。
- `asc_copy_ub2gm`、`asc_copy_ub2gm_sync`：将数据从Unified Buffer搬运到Global Memory，提供多段搬运和按总字节数搬运两类接口。
- `asc_copy_ub2ub`、`asc_copy_ub2ub_sync`：在Unified Buffer内搬运数据。
- `asc_copy_ub2l1`、`asc_copy_ub2l1_sync`：将数据从Unified Buffer搬运到L1 Buffer。
- `asc_set_copy_pad_val`：配置`asc_copy_gm2ub_align`使用的固定填充值。Ascend 950PR、Ascend 950DT支持8位、16位和32位整数及浮点类型；FP8、FP4和HiFloat8数据需要先转换为`int8_t`再传入。
- `asc_ndim_copy_dci`：执行N维搬运场景所需的Data Cache Invalidate操作。

### Memory矢量计算

- `asc_bitsort`、`asc_bitsort_sync`：每次迭代对32个元素按Score从大到小排序，并将结果以`(Score, Index)`结构写入目的地址。
- `asc_mrgsort4`、`asc_mrgsort4_sync`：将最多四路已按Score排序的队列归并为一路降序队列。
- `asc_transpose`、`asc_transpose_sync`：对Unified Buffer中的16 x 16二维数据块执行转置。
- `asc_transto5hd_b8`、`asc_transto5hd_b8_sync`、`asc_transto5hd_b16`、`asc_transto5hd_b16_sync`、`asc_transto5hd_b32`、`asc_transto5hd_b32_sync`：分别将8位、16位或32位数据转换为5HD排布。
- `asc_set_va_reg`：将16个DataBlock地址与VA地址寄存器关联，地址分为两组，每组八个，供`asc_transpose`使用。

### 矩阵数据搬运

- `asc_copy_gm2l1`、`asc_copy_gm2l1_sync`：将数据从Global Memory搬运到L1 Buffer。
- `asc_copy_gm2l1_align`、`asc_copy_gm2l1_align_sync`：执行带左右填充和L2 Cache控制的Global Memory到L1 Buffer搬运。
- `asc_copy_gm2l1_nd2nz`、`asc_copy_gm2l1_nd2nz_sync`：在Global Memory到L1 Buffer搬运过程中完成ND到NZ格式转换。
- `asc_copy_gm2l1_dn2nz`、`asc_copy_gm2l1_dn2nz_sync`：在Global Memory到L1 Buffer搬运过程中完成DN到NZ格式转换。
- `asc_copy_l12l0a`、`asc_copy_l12l0a_sync`、`asc_copy_l12l0b`、`asc_copy_l12l0b_sync`：将矩阵从L1 Buffer搬运到L0A Buffer或L0B Buffer，并支持3D搬运（img2col）。
- `asc_copy_l12l0b_trans`、`asc_copy_l12l0b_trans_sync`：将矩阵从L1 Buffer转置搬运到L0B Buffer。
- `asc_copy_l12l0b_mx`、`asc_copy_l12l0b_mx_sync`：将MX量化的scale矩阵从L1 Buffer搬运到L0B Buffer。
- `asc_copy_l0c2gm`、`asc_copy_l0c2gm_sync`：将L0C Buffer中的数据搬运到Global Memory，支持量化、ReLU、LeakyReLU、通道拆分和NZ2ND随路控制。
- `asc_copy_l0c2ub`、`asc_copy_l0c2ub_sync`：将L0C Buffer中的数据搬运到Unified Buffer，支持量化、ReLU、LeakyReLU和通道拆分随路控制。
- `asc_copy_l12fb`、`asc_copy_l12fb_sync`：将量化参数从L1 Buffer搬运到Fixpipe Buffer。
- `asc_copy_l12bt`、`asc_copy_l12bt_sync`：将矩阵乘使用的Bias数据从L1 Buffer搬运到BiasTable Buffer。
- `asc_copy_l12ub`、`asc_copy_l12ub_sync`：将数据从L1 Buffer搬运到Unified Buffer。
- `asc_fill_l1`、`asc_fill_l1_sync`：使用`value`的二进制表示填充L1 Buffer中的指定区域。
- `asc_set_l13d_fmatrix_b`：配置从右矩阵获取Feature Map属性时使用的3D fmatrix参数。
- `asc_set_l0c2gm_lrelu_alpha`：为`asc_copy_l0c2l1`和`asc_copy_l0c2gm`配置LeakyReLU的alpha值，alpha支持half和float类型。

### 矩阵计算

- `asc_mmad`、`asc_mmad_sync`：为3510架构新增常规矩阵乘加，支持bfloat16、half、float、int8、HiFloat8以及FP8同精度和混合精度输入组合。
- `asc_enable_fp8`、`asc_enable_hif8`：启用FP8、HiFloat8矩阵计算模式。
- `asc_enable_hf32_trans`、`asc_enable_hf32`、`asc_set_fp32_mode`：配置HF32转换、HF32计算和FP32计算模式。
- `asc_set_mmad_direction_m`、`asc_set_mmad_direction_n`：配置矩阵乘的M、N方向模式。

### 标量计算

- `asc_clear_nthbit`、`asc_set_nthbit`：清除或设置`uint64_t`值中的指定bit。
- `asc_atomic_add`、`asc_atomic_sub`、`asc_atomic_max`、`asc_atomic_min`、`asc_atomic_exch`、`asc_atomic_cas`：对Global Memory中的32位或64位整数及float数据执行标量原子操作，并返回操作前的值。
- `asc_atomic_and`、`asc_atomic_or`、`asc_atomic_xor`：对Global Memory中的32位或64位整数执行原子按位运算。
- `asc_atomic_inc`：对Global Memory中的`uint32_t`或`uint64_t`计数器执行带上限回绕的原子递增；当前值大于等于`val`时写0，否则加1。
- `asc_atomic_dec`：对Global Memory中的`uint32_t`或`uint64_t`计数器执行带下限回绕的原子递减；当前值为0或大于`val`时写`val`，否则减1。

### 同步控制

- `asc_lock`、`asc_unlock`：通过互斥锁同步异步流水线，支持阻塞和非阻塞执行模式。
- `asc_sync_intra_arrive`、`asc_sync_intra_wait`：在同一Block内同步不同Core的流水线。
- `asc_sync_subblock_arrive`、`asc_sync_subblock_wait`：在同一组内同步不同Subblock。
- `asc_sync_inter_arrive`、`asc_sync_inter_wait`：在不同组之间同步Block。
- `asc_sync_block_arrive`、`asc_sync_block_wait`：在同一组内同步Block和Subblock。
- `asc_sync_data_barrier`：等待指定范围内先前发起的内存访问完成。

### 系统变量

- `asc_get_smmu_tag_version`、`asc_get_phy_stack_base`、`asc_get_status`、`asc_get_sys_virtual_base`：分别获取SMMU标签版本、物理栈基址、系统状态和系统虚拟基址。
- `asc_get_block_idx`：获取当前Block的索引。
- `asc_get_sub_block_num`：在分离模式下获取一个AI Core上的Cube Core或Vector Core数量。
- `asc_get_sub_block_id`：获取AI Core上的Vector Core ID。
- `asc_get_phy_buf_addr`：根据偏移量获取片上实际物理地址。
- `asc_get_core_id`、`asc_get_arch_ver`：分别获取核ID和架构版本。
- `asc_get_vf_len`：在编译期获取矢量长度VL。

### 矩阵数据搬运配置

- `asc_set_l0c2gm_channel_para`：配置`asc_copy_l0c2l1`或`asc_copy_l0c2gm`的通道步长参数，仅在开启NZ2DN模式时生效。
- `asc_set_l0c2gm_quant_post`：配置Fixpipe后处理阶段使用的后量化参数。
- `asc_set_l0c2gm_relu_alpha`：配置Fixpipe或Cube指令执行Scalar ReLU时预处理和后处理阶段使用的alpha值。
- `asc_set_l3d_rpt_b`：配置`asc_copy_l12l0a`和`asc_copy_l12l0b`执行L1 Buffer到L0A Buffer或L0B Buffer的2D搬运时使用的repeat参数。
- `asc_set_gm2l1_nz_para`：配置Global Memory到L1 Buffer的NZ格式搬运参数。
- `asc_set_l13d_padding`、`asc_set_l12l0_padding_val`：分别配置L1 Buffer的3D搬运，以及L1 Buffer到L0A Buffer或L0B Buffer搬运使用的填充值。

### 缓存控制

- `asc_dci`：使整个Data Cache失效，不回写Cache Line，未同步到主存的修改会被丢弃。调用前必须先执行`asc_sync_data_barrier(mem_dsb_t::DSB_ALL)`，等待此前的内存访问完成。

## 增强

### 矢量与Reg接口

- `asc_copy_gm2ub_align`、`asc_copy_gm2ub_align_sync`、`asc_copy_ub2gm_align`、`asc_copy_ub2gm_align_sync`新增仅传入`dst`、`src`和`size`的简化重载。`enable_constant_pad`、`l2_cache_mode`和FP8、HiFloat8支持在9.0.0中已经存在，不属于本版本新增。
- `asc_create_iter_reg_b8`、`asc_create_iter_reg_b16`、`asc_create_iter_reg_b32`从单个偏移扩展为支持1至4个偏移。
- `asc_gather`为16位数据新增使用`vector_uint32_t`索引的重载。
- `asc_gather_datablock`新增`int4b_t`重载和不传mask的`uint64_t`重载。
- `asc_loadalign`、`asc_loadalign_brc`、`asc_loadalign_brc_v2`、`asc_loadalign_upsample`、`asc_loadalign_downsample`、`asc_loadalign_unpack`、`asc_loadalign_unpack_v2`、`asc_loadalign_deintlv`新增`int4b_t`、`vector_int4x2_t`重载。
- `asc_loadalign`、`asc_loadalign_upsample`、`asc_loadalign_downsample`、`asc_loadalign_postupdate`新增`vector_bool`重载。
- `asc_storealign`、`asc_storealign_1st`、`asc_storealign_intlv`、`asc_storeunalign`、`asc_storeunalign_postupdate`、`asc_storeunalign_post`、`asc_storeunalign_post_postupdate`、`asc_store`新增INT4重载。
- `asc_storealign`、`asc_storealign_pack`、`asc_storealign_postupdate`、`asc_storealign_pack_postupdate`、`asc_storeunalign_postupdate`新增`vector_bool`输出重载。
- `asc_copy`新增从`vector_uint16_t`、`vector_uint32_t`提取数据到`vector_bool`的重载。
- `asc_duplicate_scalar`新增不传mask的重载。
- 新增参数顺序明确的`asc_scatter(__ubuf__ T* dst, vector_T src, index, mask)`九组重载；根据索引将源矢量寄存器元素分散写入Unified Buffer。

### 矩阵与原子操作

- `asc_copy_l0c2l1`新增`asc_copy_l0c2l1_sync`，并新增`bfloat16_t`目的数据与float源数据、`int16_t`目的数据与`int32_t`源数据的组合，以及目的参数为`__cbuf__ void*`、源参数分别为`__cc__ float*`或`__cc__ int32_t*`的通用重载。
- `asc_copy_l12l0a_mx`新增`asc_copy_l12l0a_mx_sync`。
- `asc_set_atomic_add_int`、`asc_set_atomic_add_int8`、`asc_set_atomic_add_int16`：为后续从Unified Buffer、L0C Buffer或L1 Buffer到Global Memory的数据搬运配置32位、8位或16位整数原子累加。

## Bug修复

### 函数命名

- 修复`asc_half2int4x2_ru`、`asc_half2int4x2_rz`、`asc_half2int4x2_rn`和`asc_half2int4x2_rna`四组接口的舍入模式命名，修复范围包含每组接口的饱和版本和v2、v3、v4重载。9.0.0将这些接口声明为向下舍入函数名，9.1.0恢复为与实际舍入模式一致的公开函数名。
- 将`asc_int162half_rc`修正为`asc_int162half_rz`。
- 将`asc_reduce_min_datablock_`修正为`asc_reduce_min_datablock`，去除多余的尾部下划线。
- 将`asc_half2uint8_rd_sat_v2_impl`、`asc_half2uint8_rn_sat_v2_impl`、`asc_half2uint8_rna_sat_v2_impl`、`asc_half2uint8_ru_sat_v2_impl`、`asc_half2uint8_rz_sat_v2_impl`分别修正为`asc_half2uint8_rd_sat_v2`、`asc_half2uint8_rn_sat_v2`、`asc_half2uint8_rna_sat_v2`、`asc_half2uint8_ru_sat_v2`、`asc_half2uint8_rz_sat_v2`。
- 将`asc_int162half_impl`修正为`asc_int162half`。

### 类型与地址更新

- 将`asc_bfloat162e1m2x2_rd`、`asc_bfloat162e1m2x2_rn`、`asc_bfloat162e1m2x2_rna`、`asc_bfloat162e1m2x2_ru`、`asc_bfloat162e1m2x2_rz`五组接口及其v2、v3、v4重载的目的类型从`vector_f4e1m2x2`修正为`vector_fp4x2_e1m2_t`。
- 将`asc_half2uint8`和`asc_min_scalar`的非标准类型别名`vector_u8`、`vector_f16`修正为`vector_uint8_t`、`vector_half`。
- 将`asc_loadalign_postupdate`、`asc_loadalign_brc_postupdate`、`asc_loadalign_brc_postupdate_v2`、`asc_loadalign_brc_postupdate_v3`、`asc_loadalign_upsample_postupdate`、`asc_loadalign_downsample_postupdate`、`asc_loadalign_unpack_postupdate`、`asc_loadalign_unpack_postupdate_v2`、`asc_loadalign_deintlv_postupdate`中需要自动更新的Unified Buffer源指针参数修正为指针引用，确保调用后地址变化对调用方可见。
- 将`asc_storealign_postupdate`、`asc_storealign_1st_postupdate`、`asc_storealign_pack_postupdate`、`asc_storealign_pack_postupdate_v2`中需要自动更新的Unified Buffer目的指针参数修正为指针引用，确保调用后地址变化对调用方可见。
- 将不更新地址的`asc_storeunalign`目的参数由指针引用修正为普通指针，并补齐`asc_storeunalign_postupdate`的地址更新重载。

### 头文件与参数一致性

- 修复`cache_ctrl.h`复用原子操作头文件保护宏的问题，避免包含顺序导致缓存控制声明被跳过。
- 统一`asc_simd.h`、`misc.h`、`cache_ctrl.h`、`sync.h`、`sys_var.h`和`utils_intf.h`中的头文件引用路径，避免安装目录变化导致包含失败。
- `asc_get_arch_ver`参数名由`coreVersion`统一为`core_version`。
- `asc_arange`、`asc_arange_descend`参数名由`index`统一为`value`。
- `asc_update_mask_b8`、`asc_update_mask_b16`、`asc_update_mask_b32`参数名由`scalar`统一为`value`。

## 接口签名与兼容性

- `asc_sync_block_arrive`由9.0.0的`asc_sync_block_arrive(pipe, mode, flagID)`调整为`asc_sync_block_arrive(pipe, flag_id)`，删除`mode`参数并统一事件标号命名。
- `asc_sync_block_wait`由9.0.0的`asc_sync_block_wait(flagID)`调整为`asc_sync_block_wait(pipe, flag_id)`，新增流水线参数并统一事件标号命名。

## 接口废弃

本版本标记以下接口或特定重载为`deprecated`：

| 废弃接口或重载 | 替代接口 | 说明 |
| :--- | :--- | :--- |
| `asc_get_store_atomic_config`（Ascend 950PR、Ascend 950DT） | `asc_atomic_add` | 标量原子加无需预先配置和查询数据搬运原子状态。 |
| `asc_set_store_atomic_config_v2` | `asc_atomic_add` | 标量原子加改为直接调用原子接口。 |
| `asc_exp_sub_v2(vector_float&, vector_float, vector_float, vector_bool)` | `asc_exp_sub` | 仅float输入的该重载废弃；half输入的`asc_exp_sub_v2`仍保留，用于读取奇数位。 |
| `asc_set_gm2ub_pad` | `asc_set_copy_pad_val` | 使用通用搬运填充值配置接口。 |
| 旧参数顺序的`asc_scatter(vector_T& src, __ubuf__ T* dst, index, mask)` | `asc_scatter(__ubuf__ T* dst, vector_T src, index, mask)` | 使用目的Unified Buffer地址在前、源寄存器在后的新签名。 |

**注：** 详情可参考[废弃接口](../deprecated_interface.md)。
