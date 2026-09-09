# 废弃接口

## 矩阵计算

- **[asc_mmad（废弃）](asc_mmad_deprecated.md)**
- **[asc_mmad_mx（废弃）](asc_mmad_mx_deprecated.md)**
- **[asc_mmad_sparse（废弃）](asc_mmad_sparse_deprecated.md)**

## Memory矢量计算

- **[asc_abs_sync（废弃）](asc_abs_sync_deprecated.md)**

## 矢量数据搬运

- **[asc_set_gm2ub_pad（废弃）](asc_set_gm2ub_pad_deprecated.md)**
- **[asc_copy_gm2ub_align（废弃）](asc_copy_gm2ub_align_arch_3510_deprecated.md)**
- **[asc_copy_ub2gm_align（废弃）](asc_copy_ub2gm_align_arch_3510_deprecated.md)**
- **[asc_ndim_copy_gm2ub（废弃）](asc_ndim_copy_gm2ub_deprecated.md)**

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

### 逻辑计算

- **[asc_and（废弃）](asc_and_deprecated.md)**
- **[asc_or（废弃）](asc_or_deprecated.md)**

### 复合计算

- **[asc_exp_sub（废弃）](asc_exp_sub_deprecated.md)**
- **[asc_muls (废弃)](asc_muls_deprecated.md)**

### 排布变换

- **[asc_clear_ar_spr（废弃）](asc_clear_ar_spr_deprecated.md)**
- **[asc_pack (废弃)](asc_pack_deprecated.md)**
- **[asc_squeeze_v2 (废弃)](asc_squeeze_deprecated.md)**
- **[asc_squeeze_with_status（废弃）](asc_squeeze_with_status_deprecated.md)**

### 地址寄存器操作

- **[asc_create_iter_reg (废弃)](asc_create_iter_reg_deprecated.md)**

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
- **[asc_bfloat162e1m2x2（废弃）](asc_bfloat162e1m2x2_deprecated.md)**
- **[asc_bfloat162e2m1x2（废弃）](asc_bfloat162e2m1x2_deprecated.md)**
- **[asc_e1m2x22bfloat16（废弃）](asc_e1m2x22bfloat16_deprecated.md)**
- **[asc_e2m1x22bfloat16（废弃）](asc_e2m1x22bfloat16_deprecated.md)**
- **[asc_e4m32float（废弃）](asc_e4m32float_deprecated.md)**
- **[asc_e5m22float（废弃）](asc_e5m22float_deprecated.md)**
- **[asc_float2e4m3（废弃）](asc_float2e4m3_deprecated.md)**
- **[asc_float2e5m2（废弃）](asc_float2e5m2_deprecated.md)**
- **[asc_float2hif8（废弃）](asc_float2hif8_deprecated.md)**
- **[asc_half2int4x2（废弃）](asc_half2int4x2_deprecated.md)**
- **[asc_hif82float（废弃）](asc_hif82float_deprecated.md)**
- **[asc_int322uint8（废弃）](asc_int322uint8_deprecated.md)**
- **[asc_int4x22bfloat16（废弃）](asc_int4x22bfloat16_deprecated.md)**
- **[asc_int4x22half（废弃）](asc_int4x22half_deprecated.md)**
- **[asc_int4x22int16（废弃）](asc_int4x22int16_deprecated.md)**
- **[asc_int82int32（废弃）](asc_int82int32_deprecated.md)**
- **[asc_uint322uint8（废弃）](asc_uint322uint8_deprecated.md)**
- **[asc_uint82uint32（废弃）](asc_uint82uint32_deprecated.md)**

## 同步控制

- **[asc_sync_vec（废弃）](asc_sync_vec_deprecated.md)**

## 特殊寄存器访问

- **[asc_get_ar_spr (废弃)](asc_get_ar_spr_deprecated.md)**

## 原子操作

- **[asc_get_store_atomic_config（废弃）](asc_get_store_atomic_config_deprecated.md)**
- **[asc_set_atomic_add_int（废弃）](asc_set_atomic_add_int_deprecated.md)**
- **[asc_set_atomic_max_int（废弃）](asc_set_atomic_max_int_deprecated.md)**
- **[asc_set_atomic_min_int（废弃）](asc_set_atomic_min_int_deprecated.md)**
- **[asc_set_atomic_none（废弃）](asc_set_atomic_none_deprecated.md)**
- **[asc_set_store_atomic_config_v2（废弃）](asc_set_store_atomic_config_v2_deprecated.md)**

## 系统变量

- **[asc_get_block_idx（废弃）](asc_get_block_idx_deprecated.md)**
- **[asc_get_block_num（废弃）](asc_get_block_num_deprecated.md)**
