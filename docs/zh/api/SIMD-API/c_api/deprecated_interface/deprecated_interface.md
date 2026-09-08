# 废弃接口

## 矩阵数据搬运

- **[asc_copy_gm2l1_align（废弃）](asc_copy_gm2l1_align_deprecated.md)**（旧常规重载和同步接口）

## 矩阵计算

- **[asc_enable_fp8（废弃）](asc_enable_fp8_deprecated.md)**
- **[asc_enable_hf32_trans（废弃）](asc_enable_hf32_trans_deprecated.md)**
- **[asc_set_fp32_mode（废弃）](asc_set_fp32_mode_deprecated.md)**
- **[asc_set_l0c2gm_quant_post（废弃）](asc_set_l0c2gm_quant_post_deprecated.md)**
- **[asc_mmad（废弃）](asc_mmad_deprecated.md)**
- **[asc_mmad_mx（废弃）](asc_mmad_mx_deprecated.md)**
- **[asc_mmad_sparse（废弃）](asc_mmad_sparse_deprecated.md)**
- **[asc_set_l0c2gm_channel_para（废弃）](asc_set_l0c2gm_channel_para_deprecated.md)**
- **[asc_set_l0c2gm_config（废弃）](asc_set_l0c2gm_config_deprecated.md)**
- **[asc_set_l0c2gm_lrelu_alpha（废弃）](asc_set_l0c2gm_lrelu_alpha_deprecated.md)**
- **[asc_set_l0c2gm_nz2nd（废弃）](asc_set_l0c2gm_nz2nd_deprecated.md)**
- **[asc_set_l0c2gm_relu_alpha（废弃）](asc_set_l0c2gm_relu_alpha_deprecated.md)**
- **[asc_set_l12l0_padding_val（废弃）](asc_set_l12l0_padding_val_deprecated.md)**
- **[asc_set_l13d_padding（废弃）](asc_set_l13d_padding_deprecated.md)**
- **[asc_copy_gm2l1_dn2nz（废弃）](asc_copy_gm2l1_dn2nz_deprecated.md)**
- **[asc_copy_gm2l1_nd2nz（废弃）](asc_copy_gm2l1_nd2nz_deprecated.md)**
- **[asc_copy_gm2l1（高维切分旧形式，废弃）](asc_copy_gm2l1_highdim_deprecated.md)**
- **[asc_copy_gm2l1（2D矩阵搬运旧形式，废弃）](asc_copy_gm2l1_2d_deprecated.md)**
- **[asc_copy_l12ub_sync（废弃）](asc_copy_l12ub_sync_deprecated.md)**
- **[asc_copy_l0c2gm（废弃）](asc_copy_l0c2gm_deprecated.md)**
- **[asc_copy_l0c2l1（废弃）](asc_copy_l0c2l1_deprecated.md)**
- **[asc_copy_l0c2ub（废弃）](asc_copy_l0c2ub_deprecated.md)**

## Memory矢量计算

- **[asc_abs_sync（废弃）](asc_abs_sync_deprecated.md)**

## Reg矢量计算

### Reg数据搬入

- **[asc_loadalign (废弃)](asc_loadalign_v1_deprecated.md)**（BRCV2、BRCV3和UNPACKV2无偏移量原型）
- **[asc_loadalign (废弃)](asc_loadalign_v2_deprecated.md)**（BRCV2、BRCV3和UNPACKV2的`addr_reg`偏移原型）
- **[asc_loadalign (废弃)](asc_loadalign_v3_deprecated.md)**（BRCV2、BRCV3和UNPACKV2的`int32_t`偏移原型）
- **[asc_loadalign (废弃)](asc_loadalign_v4_deprecated.md)**（BRCV2、BRCV3和UNPACKV2的`int32_t`偏移Post Update原型）

### Reg数据搬出

- **[asc_storealign_pack_v2（废弃）](asc_storealign_v1_deprecated.md)**（无偏移量原型）
- **[asc_storealign_pack_v2（废弃）](asc_storealign_v2_deprecated.md)**（`addr_reg`偏移原型）
- **[asc_storealign_pack_v2（废弃）](asc_storealign_v3_deprecated.md)**（`int32_t`偏移原型）
- **[asc_storealign_pack_postupdate_v2（废弃）](asc_storealign_v4_deprecated.md)**（`int32_t`偏移Post Update原型）
- **[asc_scatter（废弃）](asc_scatter_deprecated.md)**
- **[asc_storeunalign_post（废弃）](asc_storeunalign_post_deprecated.md)**
- **[asc_storeunalign_postupdate（废弃）](asc_storeunalign_postupdate_deprecated.md)**

### 复合计算

- **[asc_exp_sub（废弃）](asc_exp_sub_deprecated.md)**

### 类型转换

- **[asc_bfloat162float（废弃）](asc_bfloat162float_deprecated.md)**
- **[asc_bfloat162int32（废弃）](asc_bfloat162int32_deprecated.md)**
- **[asc_float2bfloat16（废弃）](asc_float2bfloat16_deprecated.md)**
- **[asc_float2half（废弃）](asc_float2half_deprecated.md)**
- **[asc_float2int16（废弃）](asc_float2int16_deprecated.md)**
- **[asc_float2int64（废弃）](asc_float2int64_deprecated.md)**
- **[asc_half2float（废弃）](asc_half2float_deprecated.md)**
- **[asc_half2hif8（废弃）](asc_half2hif8_deprecated.md)**
- **[asc_half2int32（废弃）](asc_half2int32_deprecated.md)**
- **[asc_half2int8（废弃）](asc_half2int8_deprecated.md)**
- **[asc_half2uint8（废弃）](asc_half2uint8_deprecated.md)**
- **[asc_hif82half（废弃）](asc_hif82half_deprecated.md)**
- **[asc_int162float（废弃）](asc_int162float_deprecated.md)**
- **[asc_int162int32（废弃）](asc_int162int32_deprecated.md)**
- **[asc_int162uint32（废弃）](asc_int162uint32_deprecated.md)**
- **[asc_int162uint8（废弃）](asc_int162uint8_deprecated.md)**
- **[asc_int322int16（废弃）](asc_int322int16_deprecated.md)**
- **[asc_int322int64（废弃）](asc_int322int64_deprecated.md)**
- **[asc_int322uint16（废弃）](asc_int322uint16_deprecated.md)**
- **[asc_int642float（废弃）](asc_int642float_deprecated.md)**
- **[asc_int642int32（废弃）](asc_int642int32_deprecated.md)**
- **[asc_int82half（废弃）](asc_int82half_deprecated.md)**
- **[asc_int82int16（废弃）](asc_int82int16_deprecated.md)**
- **[asc_uint162uint32（废弃）](asc_uint162uint32_deprecated.md)**
- **[asc_uint162uint8（废弃）](asc_uint162uint8_deprecated.md)**
- **[asc_uint322int16（废弃）](asc_uint322int16_deprecated.md)**
- **[asc_uint322uint16（废弃）](asc_uint322uint16_deprecated.md)**
- **[asc_uint82half（废弃）](asc_uint82half_deprecated.md)**
- **[asc_uint82uint16（废弃）](asc_uint82uint16_deprecated.md)**

## 原子操作

- **[asc_set_atomic_add_int（废弃）](asc_set_atomic_add_int_deprecated.md)**
- **[asc_set_atomic_max_int（废弃）](asc_set_atomic_max_int_deprecated.md)**
- **[asc_set_atomic_min_int（废弃）](asc_set_atomic_min_int_deprecated.md)**
- **[asc_set_atomic_none（废弃）](asc_set_atomic_none_deprecated.md)**
- **[asc_set_store_atomic_config_v2（废弃）](asc_set_store_atomic_config_v2_deprecated.md)**

## 系统变量

- **[asc_get_block_idx（废弃）](asc_get_block_idx_deprecated.md)**
- **[asc_get_block_num（废弃）](asc_get_block_num_deprecated.md)**
