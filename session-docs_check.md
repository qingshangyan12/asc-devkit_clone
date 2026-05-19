# C API gap/stride 参数名不一致检查

**Session ID:** ses_1c0e7e5bfffeyHdo1JZE69aYsW
**Created:** 5/19/2026, 3:16:44 PM
**Updated:** 5/19/2026, 4:09:07 PM

---

## User

当前C API文档目录为 @docs/api/c_api/ 其对应的实现目录为 @impl/c_api/instr_impl/ ， 其对外接口声明目录为， @include/c_api/  ，其中包含有2201和3510两个架构的C API, 检查对应的实现，列举其中参数名不一致的地方。重点检查，gap和stride的使用，其中gap为上一个结束地址与下一个起始地址之间的距离，stride为上一个起始地址与下一个起始地址之间的距离。检查文档中，是否有参数名存在语义误用。将检查结果输出为md文件

---

## Assistant (Build · DeepSeek V4 Pro · 821.9s)

_Thinking:_

The user wants me to check for inconsistencies in parameter names across documentation, implementation, and header files for C API. Specifically, they want me to:

1. Compare parameter names between:
   - Documentation: `docs/api/c_api/`
   - Implementation: `impl/c_api/instr_impl/` (2201 and 3510 architectures)
   - Header files: `include/c_api/`

2. Focus on `gap` and `stride` usage:
   - `gap`: distance between end of previous and start of next
   - `stride`: distance between start of previous and start of next

3. Check for semantic misuse of parameter names

4. Output results as a markdown file

Let me start by exploring the directory structures more deeply to understand the organization.

**Tool: task**

**Input:**
```json
{
  "description": "Explore API directory structure",
  "prompt": "Explore the directory structures of these three directories thoroughly:\n1. docs/api/c_api/ (all subdirectories and files)\n2. impl/c_api/instr_impl/npu_arch_2201/ (all subdirectories and files)\n3. impl/c_api/instr_impl/npu_arch_3510/ (all subdirectories and files)\n4. include/c_api/ (all subdirectories and files)\n\nFor each, list all .md, .h, .c, .cpp files found. Return the complete file listings grouped by directory.",
  "subagent_type": "explore"
}
```

**Output:**
```
task_id: ses_1c0e7aa8bffeyhyRvMFOteweT9 (for resuming to continue this task if needed)

<task_result>
Here is the complete file listing for all four directories, grouped by directory:

---

## 1. `docs/api/c_api/` — 401 `.md` files (0 `.h`, 0 `.c`, 0 `.cpp`)

### Top-level files (3)
```
docs/api/c_api/README.md
docs/api/c_api/c_api_list.md
docs/api/c_api/general_instruction.md
```

### `cache_ctrl/` (5)
```
docs/api/c_api/cache_ctrl/asc_datacache_preload.md
docs/api/c_api/cache_ctrl/asc_dcci.md
docs/api/c_api/cache_ctrl/asc_dci.md
docs/api/c_api/cache_ctrl/asc_get_icache_preload_status.md
docs/api/c_api/cache_ctrl/asc_icache_preload.md
```

### `cube_compute/` (13)
```
docs/api/c_api/cube_compute/asc_enable_hf32.md
docs/api/c_api/cube_compute/asc_enable_hf32_trans.md
docs/api/c_api/cube_compute/asc_get_l0c2gm_prequant.md
docs/api/c_api/cube_compute/asc_get_l0c2gm_relu.md
docs/api/c_api/cube_compute/asc_get_l0c2gm_unitflag.md
docs/api/c_api/cube_compute/asc_mmad.md
docs/api/c_api/cube_compute/asc_mmad_mx.md
docs/api/c_api/cube_compute/asc_mmad_sparse.md
docs/api/c_api/cube_compute/asc_set_fp32_mode.md
docs/api/c_api/cube_compute/asc_set_l0c2gm_config.md
docs/api/c_api/cube_compute/asc_set_l0c2gm_nz2nd.md
docs/api/c_api/cube_compute/asc_set_mmad_direction_m.md
docs/api/c_api/cube_compute/asc_set_mmad_direction_n.md
```

### `cube_datamove/` (65)
```
docs/api/c_api/cube_datamove/asc_copy_gm2l0a.md
docs/api/c_api/cube_datamove/asc_copy_gm2l0b.md
docs/api/c_api/cube_datamove/asc_copy_gm2l1_align.md
docs/api/c_api/cube_datamove/asc_copy_gm2l1_dn2nz.md
docs/api/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l0c2ub.md
docs/api/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l12fb_v2.md
docs/api/c_api/cube_datamove/asc_copy_l12gm.md
docs/api/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l12l0a_mx.md
docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md
docs/api/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l12l0b_mx.md
docs/api/c_api/cube_datamove/asc_copy_l12l0b_sparse.md
docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md
docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md
docs/api/c_api/cube_datamove/asc_copy_l12l0c.md
docs/api/c_api/cube_datamove/asc_copy_l12ub.md
docs/api/c_api/cube_datamove/asc_fill_l0a.md
docs/api/c_api/cube_datamove/asc_fill_l0b.md
docs/api/c_api/cube_datamove/asc_fill_l1.md
docs/api/c_api/cube_datamove/asc_load_image_to_cbuf.md
docs/api/c_api/cube_datamove/asc_set_gm2l1_loop1_stride.md
docs/api/c_api/cube_datamove/asc_set_gm2l1_loop2_stride.md
docs/api/c_api/cube_datamove/asc_set_gm2l1_loop_size.md
docs/api/c_api/cube_datamove/asc_set_gm2l1_pad.md
docs/api/c_api/cube_datamove/asc_set_l0c2gm_lrelu_alpha.md
docs/api/c_api/cube_datamove/asc_set_l0c_copy_params.md
docs/api/c_api/cube_datamove/asc_set_l0c_copy_prequant.md
docs/api/c_api/cube_datamove/asc_set_l13d_fmatrix_b.md
docs/api/c_api/cube_datamove/asc_set_l13d_fmatrix.md
docs/api/c_api/cube_datamove/asc_set_l13d_padding.md
docs/api/c_api/cube_datamove/asc_set_l13d_rpt.md
docs/api/c_api/cube_datamove/asc_set_l13d_size.md
```

### `misc/` (1)
```
docs/api/c_api/misc/asc_init.md
```

### `reg/` (1 + subdirs)
```
docs/api/c_api/reg/reg数据类型定义.md
docs/api/c_api/reg/reg_load/asc_gather_datablock.md
docs/api/c_api/reg/reg_load/asc_gather.md
docs/api/c_api/reg/reg_load/asc_get_mask_spr.md
docs/api/c_api/reg/reg_load/asc_loadalign/asc_loadalign_v1.md
docs/api/c_api/reg/reg_load/asc_loadalign/asc_loadalign_v2.md
docs/api/c_api/reg/reg_load/asc_loadalign/asc_loadalign_v3.md
docs/api/c_api/reg/reg_load/asc_loadalign/asc_loadalign_v4.md
docs/api/c_api/reg/reg_load/asc_loadalign/asc_loadalign_v5.md
docs/api/c_api/reg/reg_load/asc_loadalign/asc_loadalign_v6.md
docs/api/c_api/reg/reg_load/asc_loadalign_postupdate.md
docs/api/c_api/reg/reg_load/asc_load.md
docs/api/c_api/reg/reg_load/asc_loadunalign.md
docs/api/c_api/reg/reg_load/asc_loadunalign_postupdate.md
docs/api/c_api/reg/reg_load/asc_loadunalign_pre.md
docs/api/c_api/reg/reg_load/asc_scatter.md
docs/api/c_api/reg/reg_store/asc_storealign/asc_storealign_v1.md
docs/api/c_api/reg/reg_store/asc_storealign/asc_storealign_v2.md
docs/api/c_api/reg/reg_store/asc_storealign/asc_storealign_v3.md
docs/api/c_api/reg/reg_store/asc_storealign/asc_storealign_v4.md
docs/api/c_api/reg/reg_store/asc_storealign/asc_storealign_v5.md
docs/api/c_api/reg/reg_store/asc_store.md
docs/api/c_api/reg/reg_store/asc_storeunalign.md
docs/api/c_api/reg/reg_store/asc_storeunalign_post.md
docs/api/c_api/reg/reg_store/asc_storeunalign_post_postupdate.md
docs/api/c_api/reg/reg_store/asc_storeunalign_postupdate.md
```
And ~88 `reg/reg_vector/*.md` files (abs, add, addc, add_scalar, and, arange, axpy, bfloat162e1m2x2, bfloat162e2m1x2, bfloat162float, bfloat162half, bfloat162int32, clear_ar_spr, copy, create_iter_reg, create_mask, cumulative_histogram, deintlv, div, duplicate, duplicate_scalar, e1m2x22bfloat16, e2m1x22bfloat16, e4m32float, e5m22float, eq, eq_scalar, exp, exp_sub, float2bfloat16, float2e4m3, float2e5m2, float2half, float2hif8, float2int16, float2int32, float2int64, frequency_histogram, ge, ge_scalar, gt, gt_scalar, half2bf16, half2float, half2hif8, half2int16, half2int32, half2int4x2, half2int8, half2uint8, hif82half, int162float, int162half, int162int32, int162uint32, int162uint8, int322float, int322int16, int322int64, int322uint16, int322uint8, int4x22bfloat16, int4x22half, int4x22int16, int642float, int642int32, int82half, int82int16, int82int32, intlv, leakyrelu, le, le_scalar, ln, lt, lt_scalar, madd, max, max_scalar, mem_bar, min, min_scalar, mull, mul, mul_scalar, muls, neg, ne, ne_scalar, not, or, pack, pair_reduce_sum, prelu, reduce_max_datablock, reduce_max, reduce_min_datablock, reduce_min, reduce_sum_datablock, reduce_sum, relu, select, shiftleft, shiftleft_scalar, shiftright, shiftright_scalar, sqrt, squeeze, subc, sub, truncate, uint162uint32, uint162uint8, uint322int16, uint322uint16, uint322uint8, uint82half, uint82uint16, uint82uint32, unpack, unsqueeze, update_mask, xor)

### `scalar_compute/` (10)
```
docs/api/c_api/scalar_compute/asc_clear_nthbit.md
docs/api/c_api/scalar_compute/asc_clz.md
docs/api/c_api/scalar_compute/asc_ffs.md
docs/api/c_api/scalar_compute/asc_ffz.md
docs/api/c_api/scalar_compute/asc_float2int32.md
docs/api/c_api/scalar_compute/asc_popc.md
docs/api/c_api/scalar_compute/asc_set_nthbit.md
docs/api/c_api/scalar_compute/asc_sflbits.md
docs/api/c_api/scalar_compute/asc_store_dev.md
docs/api/c_api/scalar_compute/asc_zero_bits_cnt.md
```

### `simd_atomic/` (7)
```
docs/api/c_api/simd_atomic/asc_get_store_atomic_config.md
docs/api/c_api/simd_atomic/asc_set_atomic_add.md
docs/api/c_api/simd_atomic/asc_set_atomic_max.md
docs/api/c_api/simd_atomic/asc_set_atomic_min.md
docs/api/c_api/simd_atomic/asc_set_atomic_none.md
docs/api/c_api/simd_atomic/asc_set_store_atomic_config_v1.md
docs/api/c_api/simd_atomic/asc_set_store_atomic_config_v2.md
```

### `struct/` (5)
```
docs/api/c_api/struct/asc_fill_value_config.md
docs/api/c_api/struct/asc_l13d_fmatrix_config.md
docs/api/c_api/struct/asc_load3d_v2_config.md
docs/api/c_api/struct/asc_ndim_pad_count_config.md
docs/api/c_api/struct/asc_store_atomic_config.md
```

### `sync/` (14)
```
docs/api/c_api/sync/asc_lock.md
docs/api/c_api/sync/asc_sync.md
docs/api/c_api/sync/asc_sync_block_arrive.md
docs/api/c_api/sync/asc_sync_block_wait.md
docs/api/c_api/sync/asc_sync_data_barrier.md
docs/api/c_api/sync/asc_sync_inter_wait.md
docs/api/c_api/sync/asc_sync_intra_arrive.md
docs/api/c_api/sync/asc_sync_intra_wait.md
docs/api/c_api/sync/asc_sync_mte2.md
docs/api/c_api/sync/asc_sync_mte3.md
docs/api/c_api/sync/asc_sync_notify.md
docs/api/c_api/sync/asc_sync_pipe.md
docs/api/c_api/sync/asc_sync_vec.md
docs/api/c_api/sync/asc_sync_wait.md
docs/api/c_api/sync/asc_unlock.md
```

### `sys_var/` (24)
```
docs/api/c_api/sys_var/asc_get_arch_ver.md
docs/api/c_api/sys_var/asc_get_ar_spr.md
docs/api/c_api/sys_var/asc_get_block_idx.md
docs/api/c_api/sys_var/asc_get_block_num.md
docs/api/c_api/sys_var/asc_get_core_id.md
docs/api/c_api/sys_var/asc_get_ctrl.md
docs/api/c_api/sys_var/asc_get_ffts_base_addr.md
docs/api/c_api/sys_var/asc_get_phy_buf_addr.md
docs/api/c_api/sys_var/asc_get_phy_stack_base.md
docs/api/c_api/sys_var/asc_get_program_counter.md
docs/api/c_api/sys_var/asc_get_smmu_tag_version.md
docs/api/c_api/sys_var/asc_get_status.md
docs/api/c_api/sys_var/asc_get_sub_block_id.md
docs/api/c_api/sys_var/asc_get_sub_block_num.md
docs/api/c_api/sys_var/asc_get_system_cycle.md
docs/api/c_api/sys_var/asc_get_sys_virtual_base.md
docs/api/c_api/sys_var/asc_get_vf_len.md
docs/api/c_api/sys_var/asc_set_ctrl.md
docs/api/c_api/sys_var/asc_set_ffts_base_addr.md
docs/api/c_api/sys_var/asc_set_gm2l1_nz_para.md
docs/api/c_api/sys_var/asc_set_l0c2gm_channel_para.md
docs/api/c_api/sys_var/asc_set_l0c2gm_quant_post.md
docs/api/c_api/sys_var/asc_set_l0c2gm_relu_alpha.md
docs/api/c_api/sys_var/asc_set_l12l0_padding_val.md
docs/api/c_api/sys_var/asc_set_l3d_rpt_b.md
```

### `vector_compute/` (98)
```
docs/api/c_api/vector_compute/asc_abs.md
docs/api/c_api/vector_compute/asc_add.md
docs/api/c_api/vector_compute/asc_add_relu.md
docs/api/c_api/vector_compute/asc_add_scalar.md
docs/api/c_api/vector_compute/asc_and.md
docs/api/c_api/vector_compute/asc_axpy.md
docs/api/c_api/vector_compute/asc_bfloat162float.md
docs/api/c_api/vector_compute/asc_bfloat162int32.md
docs/api/c_api/vector_compute/asc_bitsort.md
docs/api/c_api/vector_compute/asc_brcb.md
docs/api/c_api/vector_compute/asc_copy.md
docs/api/c_api/vector_compute/asc_datablock_reduce_max.md
docs/api/c_api/vector_compute/asc_datablock_reduce_min.md
docs/api/c_api/vector_compute/asc_datablock_reduce_sum.md
docs/api/c_api/vector_compute/asc_deq_int162b8.md
docs/api/c_api/vector_compute/asc_deq_int322half.md
docs/api/c_api/vector_compute/asc_div.md
docs/api/c_api/vector_compute/asc_duplicate.md
docs/api/c_api/vector_compute/asc_eq.md
docs/api/c_api/vector_compute/asc_eq_scalar.md
docs/api/c_api/vector_compute/asc_exp.md
docs/api/c_api/vector_compute/asc_float2bf16.md
docs/api/c_api/vector_compute/asc_float2float.md
docs/api/c_api/vector_compute/asc_float2half.md
docs/api/c_api/vector_compute/asc_float2int16.md
docs/api/c_api/vector_compute/asc_float2int32.md
docs/api/c_api/vector_compute/asc_float2int64.md
docs/api/c_api/vector_compute/asc_fma.md
docs/api/c_api/vector_compute/asc_gather_datablock.md
docs/api/c_api/vector_compute/asc_gather.md
docs/api/c_api/vector_compute/asc_ge.md
docs/api/c_api/vector_compute/asc_ge_scalar.md
docs/api/c_api/vector_compute/asc_get_cmp_mask.md
docs/api/c_api/vector_compute/asc_get_reduce_max_cnt.md
docs/api/c_api/vector_compute/asc_get_reduce_min_cnt.md
docs/api/c_api/vector_compute/asc_get_rsvd_count.md
docs/api/c_api/vector_compute/asc_get_vms4_sr.md
docs/api/c_api/vector_compute/asc_gt.md
docs/api/c_api/vector_compute/asc_gt_scalar.md
docs/api/c_api/vector_compute/asc_half2float.md
docs/api/c_api/vector_compute/asc_half2int16.md
docs/api/c_api/vector_compute/asc_half2int32.md
docs/api/c_api/vector_compute/asc_half2int4.md
docs/api/c_api/vector_compute/asc_half2int8.md
docs/api/c_api/vector_compute/asc_half2uint8.md
docs/api/c_api/vector_compute/asc_int162float.md
docs/api/c_api/vector_compute/asc_int162half.md
docs/api/c_api/vector_compute/asc_int322float.md
docs/api/c_api/vector_compute/asc_int322int16.md
docs/api/c_api/vector_compute/asc_int322int64.md
docs/api/c_api/vector_compute/asc_int42half.md
docs/api/c_api/vector_compute/asc_int642float.md
docs/api/c_api/vector_compute/asc_int642int32.md
docs/api/c_api/vector_compute/asc_int82half.md
docs/api/c_api/vector_compute/asc_leakyrelu.md
docs/api/c_api/vector_compute/asc_le.md
docs/api/c_api/vector_compute/asc_le_scalar.md
docs/api/c_api/vector_compute/asc_log.md
docs/api/c_api/vector_compute/asc_lt.md
docs/api/c_api/vector_compute/asc_lt_scalar.md
docs/api/c_api/vector_compute/asc_max.md
docs/api/c_api/vector_compute/asc_max_scalar.md
docs/api/c_api/vector_compute/asc_min.md
docs/api/c_api/vector_compute/asc_min_scalar.md
docs/api/c_api/vector_compute/asc_mrgsort4.md
docs/api/c_api/vector_compute/asc_mul_add.md
docs/api/c_api/vector_compute/asc_mul_add_relu.md
docs/api/c_api/vector_compute/asc_mul_cast_half2int8.md
docs/api/c_api/vector_compute/asc_mul_cast_half2uint8.md
docs/api/c_api/vector_compute/asc_mul.md
docs/api/c_api/vector_compute/asc_mul_scalar.md
docs/api/c_api/vector_compute/asc_ne.md
docs/api/c_api/vector_compute/asc_ne_scalar.md
docs/api/c_api/vector_compute/asc_not.md
docs/api/c_api/vector_compute/asc_or.md
docs/api/c_api/vector_compute/asc_pair_reduce_sum.md
docs/api/c_api/vector_compute/asc_rcp.md
docs/api/c_api/vector_compute/asc_reduce.md
docs/api/c_api/vector_compute/asc_relu.md
docs/api/c_api/vector_compute/asc_repeat_reduce_max.md
docs/api/c_api/vector_compute/asc_repeat_reduce_min.md
docs/api/c_api/vector_compute/asc_repeat_reduce_sum.md
docs/api/c_api/vector_compute/asc_rsqrt.md
docs/api/c_api/vector_compute/asc_select.md
docs/api/c_api/vector_compute/asc_set_cmp_mask.md
docs/api/c_api/vector_compute/asc_set_deq_scale.md
docs/api/c_api/vector_compute/asc_set_mask_count.md
docs/api/c_api/vector_compute/asc_set_mask_norm.md
docs/api/c_api/vector_compute/asc_set_va_reg.md
docs/api/c_api/vector_compute/asc_set_vector_mask.md
docs/api/c_api/vector_compute/asc_shiftleft.md
docs/api/c_api/vector_compute/asc_shiftright.md
docs/api/c_api/vector_compute/asc_sqrt.md
docs/api/c_api/vector_compute/asc_squeeze.md
docs/api/c_api/vector_compute/asc_sub.md
docs/api/c_api/vector_compute/asc_sub_relu.md
docs/api/c_api/vector_compute/asc_sub_scalar.md
docs/api/c_api/vector_compute/asc_transpose.md
docs/api/c_api/vector_compute/asc_transto5hd.md
docs/api/c_api/vector_compute/asc_uint82half.md
docs/api/c_api/vector_compute/asc_vdeq_int162b8.md
```

### `vector_datamove/` (19)
```
docs/api/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_2201.md
docs/api/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md
docs/api/c_api/vector_datamove/asc_copy_gm2ub.md
docs/api/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_2201.md
docs/api/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md
docs/api/c_api/vector_datamove/asc_copy_ub2gm.md
docs/api/c_api/vector_datamove/asc_copy_ub2l1.md
docs/api/c_api/vector_datamove/asc_copy_ub2ub.md
docs/api/c_api/vector_datamove/asc_ndim_copy_gm2ub.md
docs/api/c_api/vector_datamove/asc_set_copy_pad_val.md
docs/api/c_api/vector_datamove/asc_set_gm2ub_loop1_stride.md
docs/api/c_api/vector_datamove/asc_set_gm2ub_loop2_stride.md
docs/api/c_api/vector_datamove/asc_set_gm2ub_loop_size.md
docs/api/c_api/vector_datamove/asc_set_ndim_loop_stride.md
docs/api/c_api/vector_datamove/asc_set_ndim_pad_count.md
docs/api/c_api/vector_datamove/asc_set_ndim_pad_value.md
docs/api/c_api/vector_datamove/asc_set_ub2gm_loop1_stride.md
docs/api/c_api/vector_datamove/asc_set_ub2gm_loop2_stride.md
docs/api/c_api/vector_datamove/asc_set_ub2gm_loop_size.md
```

**Summary for `docs/api/c_api/`**: 401 `.md` files total; 0 `.h`, 0 `.c`, 0 `.cpp`

---

## 2. `impl/c_api/instr_impl/npu_arch_2201/` — 251 `.h` files (0 `.md`, 0 `.c`, 0 `.cpp`)

### Top-level files (10)
```
impl/c_api/instr_impl/npu_arch_2201/atomic_impl.h
impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h
impl/c_api/instr_impl/npu_arch_2201/misc_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl.h
impl/c_api/instr_impl/npu_arch_2201/sync_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl.h
```

### `atomic_impl/` (21)
```
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_get_store_atomic_config_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_add_bfloat_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_add_float16_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_add_float_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_add_int16_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_add_int8_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_add_int_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_max_bfloat_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_max_float16_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_max_float_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_max_int16_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_max_int8_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_max_int_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_min_bfloat_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_min_float16_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_min_float_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_min_int16_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_min_int8_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_min_int_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_atomic_none_impl.h
impl/c_api/instr_impl/npu_arch_2201/atomic_impl/asc_set_store_atomic_config_impl.h
```

### `cache_ctrl_impl/` (4)
```
impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl/asc_data_cache_preload_impl.h
impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl/asc_dcci_impl.h
impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl/asc_get_icache_preload_status_impl.h
impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl/asc_icache_preload_impl.h
```

### `cube_compute_impl/` (9)
```
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_enable_hf32_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_enable_hf32_trans_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_get_l0c2gm_config_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_mmad_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_mmad_sparse_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_set_fp32_mode_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_set_l0c2gm_config_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_set_mmad_direction_m_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/asc_set_mmad_direction_n_impl.h
```

### `cube_datamove_impl/` (27)
```
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_gm2l0a_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_gm2l0b_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_gm2l1_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_gm2l1_nd2nz_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l0c2gm_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l0c2l1_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12bt.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12fb_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12gm_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0a_2d_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0a_3d_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0a_trans_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0b_2d_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0b_3d_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0b_sparse_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0b_trans_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0c_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_fill_l0a_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_fill_l0b_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_fill_l1_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_load_image_to_cbuf_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l0c2gm_lrelu_alpha_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l0c_copy_params_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l0c_copy_prequant_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l13d_fmatrix_b_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l13d_fmatrix_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l13d_padding_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l13d_rpt_impl.h
impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_set_l13d_size_impl.h
```

### `misc_impl/` (1)
```
impl/c_api/instr_impl/npu_arch_2201/misc_impl/init_soc_state_impl.h
```

### `scalar_compute_impl/` (8)
```
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_clear_nthbit_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_clz_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_ffz_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_popc_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_set_nthbit_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_sflbits_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/asc_zero_bits_cnt_impl.h
impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl/ffs_impl.h
```

### `sync_impl/` (5)
```
impl/c_api/instr_impl/npu_arch_2201/sync_impl/asc_pipe_barrier_impl.h
impl/c_api/instr_impl/npu_arch_2201/sync_impl/asc_set_flag_impl.h
impl/c_api/instr_impl/npu_arch_2201/sync_impl/asc_sync_block_arrive_impl.h
impl/c_api/instr_impl/npu_arch_2201/sync_impl/asc_sync_data_barrier_impl.h
impl/c_api/instr_impl/npu_arch_2201/sync_impl/asc_wait_flag_impl.h
```

### `sys_var_impl/` (18)
```
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_arch_ver_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_block_idx_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_block_num_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_core_id_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_ctrl_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_ffts_base_addr_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_overflow_status_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_phy_buf_addr_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_phy_stack_base_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_smmu_tag_version_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_status_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_sub_block_id_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_sub_block_num_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_sys_cnt_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_get_sys_virtual_base_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_set_ctrl_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/asc_set_ffts_base_addr_impl.h
impl/c_api/instr_impl/npu_arch_2201/sys_var_impl/get_program_counter_impl.h
```

### `utils_impl/` (2)
```
impl/c_api/instr_impl/npu_arch_2201/utils_impl/utils_impl.h
impl/c_api/instr_impl/npu_arch_2201/utils_impl/utils_type.h
```

### `vector_compute_impl/` (69 + vconv/47 = 116)
```
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_abs_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_add_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_add_relu_cast_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_add_relu_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_add_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_and_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_axpy_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_bitsort_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_brcb_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_copy_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_datablock_reduce_max_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_datablock_reduce_min_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_datablock_reduce_sum_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_div_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_duplicate_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_eq_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_eq_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_exp_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_fma_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_gather_datablock_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_gather_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_ge_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_ge_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_get_acc_val_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_get_cmp_mask_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_get_reduce_max_cnt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_get_reduce_min_cnt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_get_rsvd_cnt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_get_vms4_sr_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_gt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_gt_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_leakyrelu_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_le_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_le_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_log_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_lt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_lt_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_max_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_max_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_min_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_min_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mrgsort4_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_add_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_add_relu_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_cast_half2int8_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_cast_half2uint8_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_mul_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_ne_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_ne_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_not_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_or_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_pair_reduce_sum_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_rcp_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_reduce_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_relu_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_repeat_reduce_max_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_repeat_reduce_min_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_repeat_reduce_sum_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_rsqrt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_select_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_set_cmp_mask_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_set_deq_scale_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_set_mask_count_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_set_mask_norm_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_set_va_reg_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_set_vector_mask_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_shiftleft_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_shiftright_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_shiftright_round_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_sqrt_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_squeeze_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_sub_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_sub_relu_cast_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_sub_relu_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_sub_scalar_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_transpose_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/asc_transto5hd_impl.h
```
Plus 47 files in `vconv/`:
```
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_bfloat162float_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_bfloat162int32_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_deq_int162b8_h_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_deq_int162b8_l_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_deqint322half_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2bfloat16_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2float_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2half_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int16_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int16_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int16_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int16_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int16_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int32_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int32_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int32_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int32_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int32_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int64_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int64_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int64_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int64_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_float2int64_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2float_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2int16_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2int32_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2int4_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2int8_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2uint8_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2uint8_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2uint8_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2uint8_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2uint8_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_half2uint8_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162float_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162half_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162half_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162half_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162half_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162half_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int162half_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322float_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322float_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322float_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322float_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322float_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322float_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322int16_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int322int64_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int42half_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int642float_rd_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int642float_rna_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int642float_rn_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int642float_ru_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int642float_rz_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int642int32_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_int82half_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_uint82half_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_vdeqs162b8h_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl/vconv/asc_vdeqs162b8l_impl.h
```

### `vector_datamove_impl/` (6)
```
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_gm2ub_align_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_gm2ub_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_ub2gm_align_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_ub2gm_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_ub2ub_impl.h
impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_set_copy_pad_val_impl.h
```

**Summary for `npu_arch_2201/`**: 251 `.h` files total; 0 `.md`, 0 `.c`, 0 `.cpp`

---

## 3. `impl/c_api/instr_impl/npu_arch_3510/` — 333 `.h` files (0 `.md`, 0 `.c`, 0 `.cpp`)

### Top-level files (8)
```
impl/c_api/instr_impl/npu_arch_3510/atomic_impl.h
impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h
impl/c_api/instr_impl/npu_arch_3510/misc_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl.h
impl/c_api/instr_impl/npu_arch_3510/utils_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h
```

### `atomic_impl/` (15)
```
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_get_store_atomic_config_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_add_bfloat_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_add_float16_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_add_float_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_add_int16_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_add_int8_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_add_int_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_max_bfloat_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_max_float16_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_max_float_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_min_bfloat_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_min_float16_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_min_float_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_atomic_none_impl.h
impl/c_api/instr_impl/npu_arch_3510/atomic_impl/asc_set_store_atomic_config_impl.h
```

### `cache_ctrl_impl/` (5)
```
impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_data_cache_preload_impl.h
impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dcci_impl.h
impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dci_impl.h
impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_get_icache_preload_status_impl.h
impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_icache_preload_impl.h
```

### `cube_compute_impl/` (5)
```
impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/asc_mmad_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/asc_mmad_mx_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/asc_set_l0c2gm_config_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/asc_set_l0c2gm_nz2nd_impl.h
```

### `cube_datamove_impl/` (22)
```
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_align_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_dn2nz_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_nd2nz_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2gm_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2l1_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l0c2ub_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12bt_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12fb_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12fb_v2_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0a_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0a_mx_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_mx_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_trans_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12ub_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_fill_l1_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop1_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop2_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_loop_size_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_gm2l1_pad_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l0c2gm_lrelu_alpha_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l0c_copy_prequant_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_fmatrix_b_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_fmatrix_impl.h
impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_set_l13d_rpt_impl.h
```

### `misc_impl/` (1)
```
impl/c_api/instr_impl/npu_arch_3510/misc_impl/init_soc_state_impl.h
```

### `scalar_compute_impl/` (10)
```
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_clear_nthbit_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_clz_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_ffs_impl.h
impl/c_api/instr_arch_3510/scalar_compute_impl/asc_ffz_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_float2int32_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_popc_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_set_nthbit_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_sflbits_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_store_dev_impl.h
impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl/asc_zero_bits_cnt_impl.h
```

### `sync_impl/` (7)
```
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_lock_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_sync_block_arrive_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_sync_data_barrier_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_sync_notify_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_sync_pipe_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_sync_wait_impl.h
impl/c_api/instr_impl/npu_arch_3510/sync_impl/asc_unlock_impl.h
```

### `sys_var_impl/` (21)
```
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_arch_ver_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_ar_spr_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_block_num_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_core_id_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_ctrl_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_phy_buf_addr_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_phy_stack_base_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_program_counter_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_smmu_tag_version_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_status_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_system_cycle_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_sys_virtual_base_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_get_vf_len_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_ctrl_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_gm2l1_nz_para_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l0c2gm_channel_para_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l0c2gm_quant_post_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l0c2gm_relu_alpha_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l12l0_padding_val_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l13d_padding_impl.h
impl/c_api/instr_impl/npu_arch_3510/sys_var_impl/asc_set_l3d_rpt_b_impl.h
```

### `utils_impl/` (2)
```
impl/c_api/instr_impl/npu_arch_3510/utils_impl/utils_impl.h
impl/c_api/instr_impl/npu_arch_3510/utils_impl/utils_type.h
```

### `vector_compute_impl/` (112 + vconv/106 = 218)
Includes files like:
```
asc_abs_impl.h, asc_abs_sub_impl.h, asc_addc_impl.h, asc_add_impl.h, asc_add_scalar_impl.h,
asc_and_impl.h, asc_arange_impl.h, asc_axpy_impl.h, asc_bitsort_impl.h, asc_clear_ar_spr_impl.h,
asc_copy_impl.h, asc_create_iter_reg_impl.h, asc_cumulative_histogram_impl.h, asc_deintlv_impl.h,
asc_div_impl.h, asc_duplicate_impl.h, asc_duplicate_scalar_impl.h, asc_eq_impl.h, asc_eq_scalar_impl.h,
asc_exp_impl.h, asc_exp_sub_impl.h, asc_frequency_histogram_impl.h, asc_ge_impl.h, asc_ge_scalar_impl.h,
asc_gt_impl.h, asc_gt_scalar_impl.h, asc_intlv_impl.h, asc_leakyrelu_impl.h, asc_le_impl.h,
asc_le_scalar_impl.h, asc_ln_impl.h, asc_lt_impl.h, asc_lt_scalar_impl.h,
asc_madd_impl.h, asc_max_impl.h, asc_max_scalar.h, asc_min_impl.h, asc_min_scalar_impl.h,
asc_mrgsort4_impl.h, asc_mul_impl.h, asc_mull_impl.h, asc_mul_scalar_impl.h, asc_muls_impl.h,
asc_neg_impl.h, asc_ne.h, asc_ne_scalar.h, asc_not_impl.h, asc_or_impl.h, asc_pack_impl.h,
asc_pair_reduce_sum_impl.h, asc_prelu_impl.h, asc_reduce_max_datablock_impl.h, asc_reduce_max_impl.h,
asc_reduce_min_datablock_impl.h, asc_reduce_min_impl.h, asc_reduce_sum_datablock.h, asc_reduce_sum.h,
asc_relu_impl.h, asc_select_impl.h, asc_set_va_reg_impl.h,
asc_shiftleft_impl.h, asc_shiftleft_scalar_impl.h, asc_shiftright_impl.h, asc_shiftright_scalar_impl.h,
asc_sqrt_impl.h, asc_squeeze_impl.h, asc_subc_impl.h, asc_sub_impl.h,
asc_transpose_impl.h, asc_transto5hd_impl.h, asc_unpack_impl.h, asc_unsqueeze_impl.h,
asc_update_mask_impl.h, asc_xor_impl.h
```
Plus ~106 files under `vconv/` in sub-subdirectories (`asc_bfloat162e1m2x2_impl/`, `asc_bfloat162e2m1x2_impl/`, `asc_bfloat162half_impl/`, `asc_bfloat162int32_impl/`, `asc_float2bfloat16_impl/`, `asc_float2half_impl/`, `asc_float2hif8_impl/`, `asc_float2int16_impl/`, `asc_float2int32_impl/`, `asc_float2int64_impl/`, `asc_half2bfloat16_impl/`, `asc_half2int16_impl/`, `asc_half2int32_impl/`, `asc_half2int4x2_impl/`, `asc_half2uint8_impl/`, `asc_int162half_impl/`, `asc_int642float_impl/`, `asc_uint322int16_impl/`, etc.) each with multiple rounding variant files.

### `vector_datamove_impl/` (22)
```
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_copy_gm2ub_align_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_copy_ub2gm_align_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_copy_ub2l1_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_copy_ub2ub_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_get_mask_spr_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_ndim_copy_dci_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_ndim_copy_gm2ub_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_scatter_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_copy_pad_val_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_gm2ub_loop1_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_gm2ub_loop2_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_gm2ub_loop_size_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ndim_loop_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ndim_pad_count_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ndim_pad_value_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ub2gm_loop1_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ub2gm_loop2_stride_impl.h
impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ub2gm_loop_size_impl.h
```
Plus subdirectories `reg_load/` (10) and `reg_store/` (6):
```
reg_load/asc_gather_datablock_impl.h, asc_gather_impl.h, asc_loadalign_impl.h,
asc_loadalign_postupdate_impl.h, asc_loadalign_v2_impl.h, asc_load_impl.h,
asc_loadunalign_impl.h, asc_loadunalign_postupdate_impl.h, asc_loadunalign_pre_impl.h
reg_store/asc_storealign_impl.h, asc_store_impl.h, asc_storeunalign_impl.h,
asc_storeunalign_post_impl.h, asc_storeunalign_post_postupdate_impl.h,
asc_storeunalign_postupdate_impl.h
```

**Summary for `npu_arch_3510/`**: 333 `.h` files total; 0 `.md`, 0 `.c`, 0 `.cpp`

---

## 4. `include/c_api/` — 17 `.h` files (0 `.md`, 0 `.c`, 0 `.cpp`)

```
include/c_api/asc_simd.h
include/c_api/atomic/atomic.h
include/c_api/cache_ctrl/cache_ctrl.h
include/c_api/cube_compute/cube_compute.h
include/c_api/cube_datamove/cube_datamove.h
include/c_api/misc/misc.h
include/c_api/reg_compute/reg_convert.h
include/c_api/reg_compute/reg_load.h
include/c_api/reg_compute/reg_store.h
include/c_api/reg_compute/reg_vector.h
include/c_api/scalar_compute/scalar_compute.h
include/c_api/sync/sync.h
include/c_api/sys_var/sys_var.h
include/c_api/utils_intf.h
include/c_api/utils/union.h
include/c_api/vector_compute/vector_compute.h
include/c_api/vector_datamove/vector_datamove.h
```

**Summary for `include/c_api/`**: 17 `.h` files total; 0 `.md`, 0 `.c`, 0 `.cpp`

---

## Grand Totals

| Directory | `.md` | `.h` | `.c` | `.cpp` |
|-----------|------:|-----:|-----:|-------:|
| `docs/api/c_api/` | 401 | 0 | 0 | 0 |
| `impl/.../npu_arch_2201/` | 0 | 251 | 0 | 0 |
| `impl/.../npu_arch_3510/` | 0 | 333 | 0 | 0 |
| `include/c_api/` | 0 | 17 | 0 | 0 |
| **Total** | **401** | **601** | **0** | **0** |

All paths are rooted at `/home/joey/code/joey/asc-devkit/`.
</task_result>
```

---

## Assistant (Build · DeepSeek V4 Pro · 12.9s)

_Thinking:_

This is a large task. I need to:
1. Search for `gap` and `stride` references across docs, impl, and include files
2. Compare parameter names between documentation and implementation
3. Check for semantic misuse

Let me start by searching for `gap` and `stride` across all three directories. This is a big task, so I'll use multiple agents in parallel to search for these terms.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
      "status": "in_progress",
      "priority": "high"
    },
    {
      "content": "Compare parameter names between docs and implementation for stride-related APIs",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Identify semantic misuse of gap vs stride across all files",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Check general parameter name inconsistencies beyond gap/stride",
      "status": "pending",
      "priority": "medium"
    },
    {
      "content": "Generate final markdown report",
      "status": "pending",
      "priority": "high"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "Compare parameter names between docs and implementation for stride-related APIs",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Identify semantic misuse of gap vs stride across all files",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Check general parameter name inconsistencies beyond gap/stride",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "Generate final markdown report",
    "status": "pending",
    "priority": "high"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 16.9s)

_Thinking:_

Let me search for gap and stride references in parallel across all three directories.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i '\\bgap\\b' docs/api/c_api/ --type md 2>/dev/null | head -200",
  "description": "Search for gap in docs"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i '\\bgap\\b' impl/c_api/instr_impl/ include/c_api/ --type h 2>/dev/null | head -200",
  "description": "Search for gap in impl and include"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i '\\bstride\\b' docs/api/c_api/ --type md 2>/dev/null | head -300",
  "description": "Search for stride in docs"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i '\\bstride\\b' impl/c_api/instr_impl/ include/c_api/ --type h 2>/dev/null | head -300",
  "description": "Search for stride in impl and include"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Pro · 15.8s)

_Thinking:_

Word boundary search didn't work. Let me try without word boundaries.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'gap' docs/api/c_api/ --type md 2>/dev/null | head -200",
  "description": "Search for gap in docs (no word boundary)"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'gap' impl/c_api/instr_impl/ include/c_api/ --type h 2>/dev/null | head -200",
  "description": "Search for gap in impl and include (no word boundary)"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'stride' docs/api/c_api/ --type md 2>/dev/null | head -400",
  "description": "Search for stride in docs"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'stride' impl/c_api/instr_impl/ include/c_api/ --type h 2>/dev/null | head -400",
  "description": "Search for stride in impl and include"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Pro · 12.0s)

_Thinking:_

It seems like `rg` is not finding anything, or maybe the `type` filter is not matching well. Let me verify the file types are properly recognized.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'stride' docs/api/c_api/ 2>/dev/null | head -50",
  "description": "Search stride without type filter"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'stride' impl/c_api/instr_impl/ 2>/dev/null | head -50",
  "description": "Search stride in impl without type filter"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rg -n -i 'stride' include/c_api/ 2>/dev/null | head -50",
  "description": "Search stride in include without type filter"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Pro · 6.3s)

_Thinking:_

No results at all? That seems very odd. Let me check if `rg` is available and the files exist.

**Tool: bash**

**Input:**
```json
{
  "command": "which rg && ls docs/api/c_api/vector_datamove/asc_ndim_copy_gm2ub.md 2>/dev/null && head -10 docs/api/c_api/vector_datamove/asc_ndim_copy_gm2ub.md",
  "description": "Check rg and sample file"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Pro · 5.8s)

_Thinking:_

It seems like these files don't actually exist on the filesystem! The user might be referencing paths that are part of a different context, or the paths might be symbolic. Let me check what actually exists in the working directory.

**Tool: bash**

**Input:**
```json
{
  "command": "ls /home/joey/code/joey/asc-devkit/",
  "description": "List top-level directory"
}
```

**Output:**
```
build.sh
CHANGELOG_en.md
CHANGELOG.md
classify_rule.yaml
cmake
CMakeLists.txt
CONTRIBUTING_en.md
CONTRIBUTING.md
docs
examples
impl
include
LICENSE
OAT.xml
README_en.md
README.md
scripts
SECURITY_en.md
SECURITY.md
tests
Third_Party_Open_Source_Software_List.yaml
Third_Party_Open_Source_Software_Notice
tools
version.cmake

```

---

## Assistant (Build · DeepSeek V4 Pro · 6.8s)

_Thinking:_

Good, the directory exists. Let me check if the specific subdirectories exist.

**Tool: bash**

**Input:**
```json
{
  "command": "ls /home/joey/code/joey/asc-devkit/docs/api/c_api/ 2>/dev/null && echo \"---\" && ls /home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/ 2>/dev/null && echo \"---\" && ls /home/joey/code/joey/asc-devkit/include/c_api/ 2>/dev/null",
  "description": "Verify subdirectories exist"
}
```

**Output:**
```
cache_ctrl
c_api_list.md
cube_compute
cube_datamove
figures
general_instruction.md
misc
README.md
reg
scalar_compute
simd_atomic
struct
sync
sys_var
vector_compute
vector_datamove
---
npu_arch_2201
npu_arch_3510
---
asc_simd.h
atomic
cache_ctrl
cube_compute
cube_datamove
misc
reg_compute
scalar_compute
sync
sys_var
utils
utils_intf.h
vector_compute
vector_datamove

```

---

## Assistant (Build · DeepSeek V4 Pro · 11.8s)

_Thinking:_

The directories exist. Now let me check a specific file to make sure it has content.

**Tool: bash**

**Input:**
```json
{
  "command": "head -30 /home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_ndim_copy_gm2ub.md",
  "description": "Check if doc file has content"
}
```

**Output:**
```
# asc_ndim_copy_gm2ub

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

多维数据搬运接口，将数据从Global Memory (GM) 搬运到 Unified Buffer (UB)。相比于基础数据搬移接口，可以更加自由配置搬入的维度信息。
本功能最多能处理5个维度，每个维度都可以单独配置Stride，且支持随路Padding功能。相关配置参数有很多，需要通过不同的接口传入。

表1 Stride配置涉及的所有参数及其传入接口，其中i表示第i维，可取值[0,4]
| 参数 | 传入接口 | 描述 |
| :--- | :------- | :--- |
| loopi_size | 本接口 | 表示每个维度内，处理的元素个数（不包含Padding元素）。<br> 单位为元素个数，取值范围为[0, 2^20-1]。 |
| loopi_src_stride | [asc_set_ndim_loopi_stride](./asc_set_ndim_loop_stride.md) | 表示每个维度内，该源操作数元素与下一个元素间的间隔。<br> 单位为元素个数，取值范围为[0, 2^40-1]。 |
| loopi_dst_stride | [asc_set_ndim_loopi_stride](./asc_set_ndim_loop_stride.md) | 表示每个维度内，该目的操作数元素与下一个元素间的间隔。<br> 单位为元素个数，取值范围为[0, 2^40-1]。 |

表2 Padding配置涉及的所有参数及其传入接口其中i表示第i维，可取值[0,4]
| 参数 | 传入接口 | 描述 |
| :--- | :------- | :--- |
| padding_mode | 本接口 | 表示Padding值填取方式，类型为bool。<br>&bull; True：使能常数填充方式，即所有Padding值为固定值pad_value。<br>&bull; False：使能最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。 |
| pad_value | [asc_set_ndim_pad_value](./asc_set_ndim_pad_value.md) | 当padding_mode为True时，该值将作为Padding的填充固定值。当padding_mode为False时，该值不生效。 |
| loopi_lp_count | <br>&bull; 0维：本接口 <br>&bull; 1至4维：[asc_set_ndim_pad_count](./asc_set_ndim_pad_count.md) | 表示每个维度内，左侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]。 |
| loopi_rp_count | <br>&bull; 0维：本接口 <br>&bull; 1至4维：[asc_set_ndim_pad_count](./asc_set_ndim_pad_count.md) | 表示每个维度内，右侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]。 |

以下以2维的例子，介绍几个典型使用场景。


```

**Tool: bash**

**Input:**
```json
{
  "command": "head -30 /home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_gm2ub_align_impl.h",
  "description": "Check if impl file has content"
}
```

**Output:**
```
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_copy_gm2ub_align_impl.h
 * \brief
 */


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning "impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl/asc_copy_gm2ub_align_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_2201_VECTOR_DATAMOVE_IMPL_ASC_COPY_GM2UB_ALIGN_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_2201_VECTOR_DATAMOVE_IMPL_ASC_COPY_GM2UB_ALIGN_IMPL_H

#include "instr_impl/npu_arch_2201/utils_impl/utils_impl.h"

#ifndef ASC_C_API_DEFAULT_ZERO_VALUE
#define ASC_C_API_DEFAULT_ZERO_VALUE 0
#endif

```

**Tool: bash**

**Input:**
```json
{
  "command": "head -30 /home/joey/code/joey/asc-devkit/include/c_api/vector_datamove/vector_datamove.h",
  "description": "Check if include file has content"
}
```

**Output:**
```
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H
#define INCLUDE_C_API_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "instr_impl/npu_arch_2201/vector_datamove_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "instr_impl/npu_arch_3510/vector_datamove_impl.h"

#endif

//===========asc_set_copy_pad_val(int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float)===========

```

---

## Assistant (Build · DeepSeek V4 Pro · 5.1s)

_Thinking:_

Good, files are real and have content. The issue was with `rg` not finding anything. Let me check if `rg` is actually available.

**Tool: bash**

**Input:**
```json
{
  "command": "rg --version 2>&1 || echo \"rg not found\"",
  "description": "Check rg availability"
}
```

**Output:**
```
/bin/bash: line 1: rg: command not found
rg not found

```

---

## Assistant (Build · DeepSeek V4 Pro · 18.4s)

_Thinking:_

`rg` is not available. I need to use `grep` or the Grep tool. Let me use the Grep tool for content search.

Wait, actually the tool restriction says to use Grep tool instead of `grep`. Let me use that. But actually the Grep tool might also have the same issue. Let me use grep directly since rg is not available. The instruction says to use Grep but this is a bash command that needs grep. Let me just use Grep properly.

Actually wait, it says "DO NOT use grep or rg" - I should use the Grep tool. But the Grep tool might not handle the `\b` word boundaries well. Let me try using the Grep tool.

Actually, for this kind of analysis, I'll need to read many files. Let me try using Grep tool first for `stride` and `gap` patterns. Then I'll compare the actual parameter names.

**Tool: grep**

**Input:**
```json
{
  "pattern": "stride|Stride|STRIDE",
  "path": "/home/joey/code/joey/asc-devkit/docs/api/c_api",
  "include": "*.md"
}
```

**Output:**
```
Found 1828 matches (showing first 100)
/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_ndim_copy_gm2ub.md:
  Line 12: 本功能最多能处理5个维度，每个维度都可以单独配置Stride，且支持随路Padding功能。相关配置参数有很多，需要通过不同的接口传入。

  Line 14: 表1 Stride配置涉及的所有参数及其传入接口，其中i表示第i维，可取值[0,4]

  Line 18: | loopi_src_stride | [asc_set_ndim_loopi_stride](./asc_set_ndim_loop_stride.md) | 表示每个维度内，该源操作数元素与下一个元素间的间隔。<br> 单位为元素个数，取值范围为[0, 2^40-1]。 |

  Line 19: | loopi_dst_stride | [asc_set_ndim_loopi_stride](./asc_set_ndim_loop_stride.md) | 表示每个维度内，该目的操作数元素与下一个元素间的间隔。<br> 单位为元素个数，取值范围为[0, 2^40-1]。 |

  Line 96: - 一条指令所能获取的所有数据的地址范围宽度不能超过40位（1TB），即：源操作数的i维大小为（loopi_lp_size + loopi_size + loopi_rp_size - 1） * loop0_src_stride，目的操作数的i维大小为：（loopi_lp_size + loopi_size + loopi_rp_size - 1） * loop0_dst_stride，所有的维度的大小加起来不超过2^40位。

  Line 97: - 当每维的loopi_dst_stride为升序序列，则不同循环间的地址空间不能交织或重叠。

  Line 114: uint64_t loop0_src_stride = 1;

  Line 115: uint64_t loop1_src_stride = 3;

  Line 116: uint64_t loop2_src_stride = 0;

  Line 117: uint64_t loop3_src_stride = 0;

  Line 118: uint64_t loop4_src_stride = 0;

  Line 119: uint64_t loop0_dst_stride = 1;

  Line 120: uint64_t loop1_dst_stride = 6;

  Line 121: uint64_t loop2_dst_stride = 0;

  Line 122: uint64_t loop3_dst_stride = 0;

  Line 123: uint64_t loop4_dst_stride = 0;

  Line 132: asc_set_ndim_loop0_stride(loop0_dst_stride, loop0_src_stride);

  Line 133: asc_set_ndim_loop1_stride(loop1_dst_stride, loop1_src_stride);

  Line 134: asc_set_ndim_loop2_stride(loop2_dst_stride, loop2_src_stride);

  Line 135: asc_set_ndim_loop3_stride(loop3_dst_stride, loop3_src_stride);

  Line 136: asc_set_ndim_loop4_stride(loop4_dst_stride, loop4_src_stride);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_ndim_loop_stride.md:
  Line 1: # asc_set_ndim_loop_stride

  Line 16: __aicore__ inline void asc_set_ndim_loop0_stride(uint64_t dst_stride, uint64_t src_stride)

  Line 17: __aicore__ inline void asc_set_ndim_loop1_stride(uint64_t dst_stride, uint64_t src_stride)

  Line 18: __aicore__ inline void asc_set_ndim_loop2_stride(uint64_t dst_stride, uint64_t src_stride)

  Line 19: __aicore__ inline void asc_set_ndim_loop3_stride(uint64_t dst_stride, uint64_t src_stride)

  Line 20: __aicore__ inline void asc_set_ndim_loop4_stride(uint64_t dst_stride, uint64_t src_stride)

  Line 27: | dst_stride | 输入 | 目的操作数的元素之间的间隔，单位为元素个数，默认值：0。取值范围：[0, 2^20 - 1]。 |

  Line 28: | src_stride | 输入 | 源操作数的元素之间的间隔，单位为元素个数，默认值：0。取值范围：[0, 2^40 - 1]。 |

  Line 45: uint64_t dst_stride = 8;

  Line 46: uint64_t src_stride = 8;

  Line 47: asc_set_ndim_loop0_stride(dst_stride, src_stride);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md:
  Line 13: 循环搬运时，需要通过接口[asc_set_gm2ub_loop_size](../asc_set_gm2ub_loop_size.md)、[asc_set_gm2ub_loop1_stride](../asc_set_gm2ub_loop1_stride.md)和[asc_set_gm2ub_loop2_stride](../asc_set_gm2ub_loop2_stride.md)设置循环模式，和该接口配合使用。

  Line 66: asc_set_gm2ub_loop1_stride(96, 128);

  Line 67: asc_set_gm2ub_loop2_stride(192, 288);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_gm2ub_loop1_stride.md:
  Line 1: # asc_set_gm2ub_loop1_stride

  Line 20: __aicore__ inline void asc_set_gm2ub_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)

  Line 27: | loop1_src_stride     | 输入     | 内层循环中相邻迭代源操作数的数据块间的间隔，单位为Byte，取值范围为[0,2^40]。|

  Line 28: | loop1_dst_stride     | 输入     | 内层循环中相邻迭代目标操作数的数据块间的间隔，单位为Byte，取值范围为[0,2^21]，且必须32B对齐。|

  Line 46: asc_set_gm2ub_loop1_stride(96, 128);

  Line 47: asc_set_gm2ub_loop2_stride(192, 288);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_ub2gm_loop2_stride.md:
  Line 1: # asc_set_ub2gm_loop2_stride

  Line 20: __aicore__ inline void asc_set_ub2gm_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)

  Line 27: | loop2_src_stride | 输入 | 用于设置外层循环中相邻迭代源操作数的数据块间的间隔，单位为Byte，取值范围为[0, 2^21)，并且loop2_src_stride必须32B对齐。 |

  Line 28: | loop2_dst_stride | 输入 | 用于设置外层循环中相邻迭代目的操作数的数据块间的间隔，单位为Byte，取值范围为[0, 2^40)。 |

  Line 47: uint64_t loop1_src_stride = 96;

  Line 48: uint64_t loop1_dst_stride = 128;

  Line 49: uint32_t loop2_src_stride = 196;

  Line 50: uint32_t loop2_dst_stride = 288;

  Line 52: asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride);

  Line 53: asc_set_ub2gm_loop2_stride(loop2_src_stride, loop2_dst_stride);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_gm2ub_loop_size.md:
  Line 46: asc_set_gm2ub_loop1_stride(96, 128);

  Line 47: asc_set_gm2ub_loop2_stride(192, 288);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_gm2ub_loop2_stride.md:
  Line 1: # asc_set_gm2ub_loop2_stride

  Line 20: __aicore__ inline void asc_set_gm2ub_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)

  Line 27: | loop2_src_stride     | 输入     | 外层循环中相邻迭代源操作数的数据块间的间隔，单位为Byte，取值范围为[0,2^40]。|

  Line 28: | loop2_dst_stride     | 输入     | 外层循环中相邻迭代目标操作数的数据块间的间隔，单位为Byte，取值范围为[0,2^21]，且必须32B对齐。|

  Line 46: asc_set_gm2ub_loop1_stride(96, 128);

  Line 47: asc_set_gm2ub_loop2_stride(192, 288);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_ub2gm_loop_size.md:
  Line 47: uint64_t loop1_src_stride = 96;

  Line 48: uint64_t loop1_dst_stride = 128;

  Line 49: uint32_t loop2_src_stride = 196;

  Line 50: uint32_t loop2_dst_stride = 288;

  Line 52: asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride);

  Line 53: asc_set_ub2gm_loop2_stride(loop2_src_stride, loop2_dst_stride);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md:
  Line 13: 循环搬运时，需要通过接口[asc_set_ub2gm_loop_size](../asc_set_ub2gm_loop_size.md)、[asc_set_ub2gm_loop1_stride](../asc_set_ub2gm_loop1_stride.md)和[asc_set_ub2gm_loop2_stride](../asc_set_ub2gm_loop2_stride.md)设置循环模式，和该接口配合使用。

  Line 63: uint64_t loop1_src_stride = 96;

  Line 64: uint64_t loop1_dst_stride = 128;

  Line 65: uint32_t loop2_src_stride = 196;

  Line 66: uint32_t loop2_dst_stride = 288;

  Line 68: asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride);

  Line 69: asc_set_ub2gm_loop2_stride(loop2_src_stride, loop2_dst_stride);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_datamove/asc_set_ub2gm_loop1_stride.md:
  Line 1: # asc_set_ub2gm_loop1_stride

  Line 20: __aicore__ inline void asc_set_ub2gm_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)

  Line 27: | loop1_src_stride | 输入 | 用于设置内层循环中相邻迭代源操作数的数据块间的间隔，单位为Byte，取值范围为[0, 2^21)，并且loop1_src_stride必须32B对齐。 |

  Line 28: | loop1_dst_stride | 输入 | 用于设置内层循环中相邻迭代目的操作数的数据块间的间隔，单位为Byte，取值范围为[0, 2^40)。 |

  Line 47: uint64_t loop1_src_stride = 96;

  Line 48: uint64_t loop1_dst_stride = 128;

  Line 49: uint32_t loop2_src_stride = 196;

  Line 50: uint32_t loop2_dst_stride = 288;

  Line 52: asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride);

  Line 53: asc_set_ub2gm_loop2_stride(loop2_src_stride, loop2_dst_stride);


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_compute/asc_sub_relu.md:
  Line 34:   __aicore__ inline void asc_sub_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)

  Line 35:   __aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)

  Line 36:   __aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)

  Line 37:   __aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)

  Line 38:   __aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)

  Line 39:   __aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)

  Line 62: | dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长。 |

  Line 63: | src0_block_stride | 输入 | 源操作数0单次迭代内不同DataBlock间地址步长。 |

  Line 64: | src1_block_stride | 输入 | 源操作数1单次迭代内不同DataBlock间地址步长。 |

  Line 65: | dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长。 |

  Line 66: | src0_repeat_stride | 输入 | 源操作数0相邻迭代间相同DataBlock的地址步长。 |

  Line 67: | src1_repeat_stride | 输入 | 源操作数1相邻迭代间相同DataBlock的地址步长。 |


/home/joey/code/joey/asc-devkit/docs/api/c_api/vector_compute/asc_shiftright.md:
  Line 34:     __aicore__ inline void asc_shiftright(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)

  Line 35:     __aicore__ inline void asc_shiftright(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)

  Line 36:     __aicore__ inline void asc_shiftright(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)

  Line 37:     __aicore__ inline void asc_shiftright(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)

  Line 38:     __aicore__ inline void asc_shiftright_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)

  Line 39:     __aicore__ inline void asc_shiftright_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)


(Results truncated: showing 100 of 1828 matches (1728 hidden). Consider using a more specific path or pattern.)
```

**Tool: grep**

**Input:**
```json
{
  "pattern": "stride|Stride|STRIDE",
  "path": "/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl",
  "include": "*.h"
}
```

**Output:**
```
Found 5567 matches (showing first 100)
/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/reg_store/asc_storealign_impl.h:
  Line 1821:         uint32_t repeat_stride : 16;

  Line 1822:         uint32_t block_stride : 16;

  Line 1827: __simd_callee__ inline void asc_storealign_impl(__ubuf__ int8_t* dst_align32b, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1831:         config.block_stride = block_stride;

  Line 1832:         config.repeat_stride = repeat_stride;

  Line 1837: __simd_callee__ inline void asc_storealign_impl(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1841:         config.block_stride = block_stride;

  Line 1842:         config.repeat_stride = repeat_stride;

  Line 1847: __simd_callee__ inline void asc_storealign_impl(__ubuf__ int16_t* dst_align32b, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1851:         config.block_stride = block_stride;

  Line 1852:         config.repeat_stride = repeat_stride;

  Line 1857: __simd_callee__ inline void asc_storealign_impl(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1861:         config.block_stride = block_stride;

  Line 1862:         config.repeat_stride = repeat_stride;

  Line 1867: __simd_callee__ inline void asc_storealign_impl(__ubuf__ int32_t* dst_align32b, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1871:         config.block_stride = block_stride;

  Line 1872:         config.repeat_stride = repeat_stride;

  Line 1877: __simd_callee__ inline void asc_storealign_impl(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1881:         config.block_stride = block_stride;

  Line 1882:         config.repeat_stride = repeat_stride;

  Line 1887: __simd_callee__ inline void asc_storealign_impl(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1891:         config.block_stride = block_stride;

  Line 1892:         config.repeat_stride = repeat_stride;

  Line 1897: __simd_callee__ inline void asc_storealign_impl(__ubuf__ hifloat8_t* dst_align32b, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1901:         config.block_stride = block_stride;

  Line 1902:         config.repeat_stride = repeat_stride;

  Line 1908: __simd_callee__ inline void asc_storealign_impl(__ubuf__ fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1912:         config.block_stride = block_stride;

  Line 1913:         config.repeat_stride = repeat_stride;

  Line 1919: __simd_callee__ inline void asc_storealign_impl(__ubuf__ fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1923:         config.block_stride = block_stride;

  Line 1924:         config.repeat_stride = repeat_stride;

  Line 1930: __simd_callee__ inline void asc_storealign_impl(__ubuf__ fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1934:         config.block_stride = block_stride;

  Line 1935:         config.repeat_stride = repeat_stride;

  Line 1941: __simd_callee__ inline void asc_storealign_impl(__ubuf__ half* dst_align32b, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1945:         config.block_stride = block_stride;

  Line 1946:         config.repeat_stride = repeat_stride;

  Line 1951: __simd_callee__ inline void asc_storealign_impl(__ubuf__ float* dst_align32b, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1955:         config.block_stride = block_stride;

  Line 1956:         config.repeat_stride = repeat_stride;

  Line 1964:         uint32_t repeat_stride : 16;

  Line 1965:         uint32_t block_stride : 16;

  Line 1970: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ int8_t* dst_align32b, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1974:         config.block_stride = block_stride;

  Line 1975:         config.repeat_stride = repeat_stride;

  Line 1980: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1984:         config.block_stride = block_stride;

  Line 1985:         config.repeat_stride = repeat_stride;

  Line 1990: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ int16_t* dst_align32b, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 1994:         config.block_stride = block_stride;

  Line 1995:         config.repeat_stride = repeat_stride;

  Line 2000: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2004:         config.block_stride = block_stride;

  Line 2005:         config.repeat_stride = repeat_stride;

  Line 2010: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ int32_t* dst_align32b, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2014:         config.block_stride = block_stride;

  Line 2015:         config.repeat_stride = repeat_stride;

  Line 2020: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2024:         config.block_stride = block_stride;

  Line 2025:         config.repeat_stride = repeat_stride;

  Line 2030: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2034:         config.block_stride = block_stride;

  Line 2035:         config.repeat_stride = repeat_stride;

  Line 2040: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ hifloat8_t* dst_align32b, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2044:         config.block_stride = block_stride;

  Line 2045:         config.repeat_stride = repeat_stride;

  Line 2051: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2055:         config.block_stride = block_stride;

  Line 2056:         config.repeat_stride = repeat_stride;

  Line 2062: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2066:         config.block_stride = block_stride;

  Line 2067:         config.repeat_stride = repeat_stride;

  Line 2073: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2077:         config.block_stride = block_stride;

  Line 2078:         config.repeat_stride = repeat_stride;

  Line 2084: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ half* dst_align32b, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2088:         config.block_stride = block_stride;

  Line 2089:         config.repeat_stride = repeat_stride;

  Line 2094: __simd_callee__ inline void asc_storealign_postupdate_impl(__ubuf__ float* dst_align32b, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 2098:         config.block_stride = block_stride;

  Line 2099:         config.repeat_stride = repeat_stride;


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h:
  Line 25: #include "instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ub2gm_loop1_stride_impl.h"

  Line 26: #include "instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ub2gm_loop2_stride_impl.h"

  Line 31: #include "instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_gm2ub_loop1_stride_impl.h"

  Line 32: #include "instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_gm2ub_loop2_stride_impl.h"

  Line 33: #include "instr_impl/npu_arch_3510/vector_datamove_impl/asc_set_ndim_loop_stride_impl.h"

  Line 4697: // repeat stride模式读入

  Line 4698: __simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 4700:     asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);

  Line 4703: __simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 4705:     asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);

  Line 4708: __simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 4710:     asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);

  Line 4713: __simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 4715:     asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);

  Line 4718: __simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 4720:     asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);

  Line 4723: __simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

  Line 4725:     asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);


(Results truncated: showing 100 of 5567 matches (5467 hidden). Consider using a more specific path or pattern.)
```

**Tool: grep**

**Input:**
```json
{
  "pattern": "stride|Stride|STRIDE",
  "path": "/home/joey/code/joey/asc-devkit/include/c_api",
  "include": "*.h"
}
```

**Output:**
```
Found 1188 matches (showing first 100)
/home/joey/code/joey/asc-devkit/include/c_api/reg_compute/reg_load.h:
  Line 1057: // repeat stride模式读入

  Line 1058: __simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1060: __simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1062: __simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1064: __simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1066: __simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1068: __simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1070: __simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1072: __simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1074: __simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1076: __simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1078: __simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1080: __simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1082: __simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1084: __simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1086: __simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1088: __simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1090: __simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1092: __simd_callee__ inline void asc_loadalign_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1094: __simd_callee__ inline void asc_loadalign_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1096: __simd_callee__ inline void asc_loadalign_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1098: __simd_callee__ inline void asc_loadalign_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1100: __simd_callee__ inline void asc_loadalign_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1102: __simd_callee__ inline void asc_loadalign_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1104: __simd_callee__ inline void asc_loadalign_postupdate(vector_int64_t& dst, __ubuf__ int64_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1106: __simd_callee__ inline void asc_loadalign_postupdate(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1108: __simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1110: __simd_callee__ inline void asc_loadalign_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1112: __simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1114: __simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1116: __simd_callee__ inline void asc_loadalign_postupdate(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1118: __simd_callee__ inline void asc_loadalign_postupdate(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1120: __simd_callee__ inline void asc_loadalign_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1122: __simd_callee__ inline void asc_loadalign_postupdate(vector_half& dst, __ubuf__ half*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

  Line 1124: __simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst, __ubuf__ float*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);


/home/joey/code/joey/asc-devkit/include/c_api/cube_compute/cube_compute.h:
  Line 131: __aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride);


/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h:
  Line 300: __aicore__ inline void asc_copy_gm2l1(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 302: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 304: __aicore__ inline void asc_copy_gm2l1(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 306: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 308: __aicore__ inline void asc_copy_gm2l1(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 310: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 312: __aicore__ inline void asc_copy_gm2l1(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 314: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 316: __aicore__ inline void asc_copy_gm2l1(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 318: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 320: __aicore__ inline void asc_copy_gm2l1(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 322: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 324: __aicore__ inline void asc_copy_gm2l1(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 326: __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

  Line 330:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 331:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 334:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 335:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 338:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 339:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 342:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 343:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 347:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 348:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 351:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 352:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 355:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 356:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 359:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 360:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 363:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 364:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 367:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 368:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 372:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 373:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 376:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 377:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 380:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 381:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 384:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 385:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 388:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 389:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 392:     uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,

  Line 393:     uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

  Line 397:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 400:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 403:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 406:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 409:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 412:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 415:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 418:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 421:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 424:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 427:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 430:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 433:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 436:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 439:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 442:     uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

  Line 446:     uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,

  Line 451:     uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,


(Results truncated: showing 100 of 1188 matches (1088 hidden). Consider using a more specific path or pattern.)
```

---

## Assistant (Build · DeepSeek V4 Pro · 502.5s)

_Thinking:_

There's a lot of data here. I found something very interesting right away in the include/c_api/cube_datamove/cube_datamove.h:

```cpp
uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap
```

So `src_stride` and `dst_gap` in the same function signature! This is exactly the kind of semantic issue the user is asking about - using `src_stride` for source (stride = distance between consecutive starts) and `dst_gap` for destination (gap = distance between end and next start). These are inconsistent if they mean the same thing (both should be either stride or gap, not mixed).

Also, I see that `cube_datamove.h` has `asc_copy_gm2l1_dn2nz` and `asc_copy_gm2l1_nd2nz` functions using `dst_nz_c0_stride`, `dst_nz_n_stride`, `dst_nz_matrix_stride`, `src_nd_matrix_stride`, `src_d_value` parameters.

Let me now do a more detailed analysis. I need to:

1. Read the docs for all functions that use stride/gap parameters
2. Read the corresponding include headers for interface declarations
3. Read the corresponding implementation files
4. Compare parameter names

Let me start by focusing on the most likely sources of issues - the datamove and vector compute APIs that use stride.

Let me first read the key include files and their corresponding docs.

**Tool: task**

**Input:**
```json
{
  "description": "Find all gap vs stride inconsistencies",
  "subagent_type": "general",
  "prompt": "You are analyzing a codebase to find parameter name inconsistencies between C API documentation, header files, and implementations. The key semantic concepts are:\n\n- **gap**: Distance between the END address of one element and the START address of the next element. gap = address difference between consecutive elements minus element size.\n- **stride**: Distance between the START address of one element and the START address of the next element. stride = address difference between consecutive elements, including element size itself.\n\n**TASK**: In the files below, identify every place where `gap` and `stride` are used as parameter names or in documentation. For each, determine if the semantics match the definition above or if there's a mismatch.\n\nDo these steps:\n\n1. Read these key include header files:\n   - /home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h\n   - /home/joey/code/joey/asc-devkit/include/c_api/vector_datamove/vector_datamove.h\n   - /home/joey/code/joey/asc-devkit/include/c_api/sys_var/sys_var.h\n\n2. For each `gap` or `stride` parameter, read the corresponding documentation in docs/api/c_api/. Also read the implementation files in impl/c_api/instr_impl/npu_arch_2201/ and impl/c_api/instr_impl/npu_arch_3510/ for each API.\n\n3. Identify ALL inconsistencies including:\n   a. Parameter named `stride` in docs but `gap` in header, or vice versa\n   b. Same parameter with different names between doc/header/impl\n   c. `gap` and `stride` used in the same function signature inconsistently (e.g., src_stride vs dst_gap)\n   d. Documentation describing stride semantics but parameter named gap\n\n4. Also check for non-stride/gap parameter name inconsistencies between doc, header, and impl.\n\nReturn a comprehensive list of ALL inconsistencies found with exact file paths and line numbers."
}
```

**Output:**
```
task_id: ses_1c0d92447ffeMtoWgLlBb8O3wm (for resuming to continue this task if needed)

<task_result>
Here is the comprehensive list of all inconsistencies found:

---

## COMPREHENSIVE GAP/STRIDE INCONSISTENCIES

### INCONSISTENCY 1: `asc_copy_l12l0a` / `asc_copy_l12l0b` (2201 arch, 2D) — Asymmetric `src_stride` vs `dst_gap`

**src uses "stride", dst uses "gap" for the same semantic concept (inter-element spacing).**

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:397` — `src_stride, dst_gap`
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:497` — same for l12l0a variant
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md:76-77` — `src_stride` described as "源操作数前一个分形与后一个分形起始地址的间隔" while `dst_gap` described as "目的操作数相邻连续数据块的间隔"
- **Impl**: `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h:69-161` and `282-344` — uses same `src_stride, dst_gap`
- **Impl body**: `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0b_impl.h` and `asc_copy_l12l0a_impl.h` — same naming

---

### INCONSISTENCY 2: `asc_copy_l12l0b_trans` (2201 arch) — Assymetric `src_stride` vs `dst_gap`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:674` — `src_stride, dst_gap, dst_frac_gap`
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md:64-70` — `src_stride, dst_gap, dst_frac_gap`
- **Impl**: `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h:1167-1260` — `src_stride, dst_gap, dst_frac_gap`
- All three agree but the inter-iteration gap param is `dst_gap` for dest vs `src_stride` for source.

---

### INCONSISTENCY 3: `asc_copy_l12l0a_trans` vs `asc_copy_l12l0b_trans` (2201 arch) — `dst_stride` (l0a) vs `dst_gap` (l0b) for same concept

Both APIs describe the same thing ("目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔"), but use different names:

- **Header l0a_trans**: `include/c_api/cube_datamove/cube_datamove.h:631` — `dst_stride`
- **Doc l0a_trans**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md:65-71` — `dst_stride`
  - **Doc parameter table l0a_trans**: line 95 — `dst_stride` but description says "结束地址到...起始地址的间隔" (GAP semantics, not STRIDE semantics!)
- **Header l0b_trans**: `include/c_api/cube_datamove/cube_datamove.h:674` — `dst_gap`
- **Doc l0b_trans**: `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md:64-70` — `dst_gap`
  - **Doc parameter table l0b_trans**: line 93 — `dst_gap` with same gap-describing text
- Same semantic, different parameter name between the l0a and l0b variants of `_trans`.

---

### INCONSISTENCY 4: `asc_copy_l12l0b_trans` (3510 arch) — Header uses `dst_frac_stride` / `src_frac_stride`, Doc and Impl use `dst_frac_gap` / `src_frac_gap`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:1317` — `dst_frac_stride, src_frac_stride`
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md:80-91` — `dst_frac_gap, src_frac_gap`
- **Doc parameter table**: same doc, lines 124-125 — `dst_frac_gap, src_frac_gap`
- **Impl dispatch**: `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h:938` — `dst_frac_gap, src_frac_gap`
- **Impl body**: `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_trans_impl.h:29` — `dst_frac_gap, src_frac_gap`
- Header says `_stride` but Doc, Impl dispatch, and Impl body all say `_gap`.

---

### INCONSISTENCY 5: `asc_copy_l12l0a_trans` (2201 arch) — Dispatch layer uses `dst_frac_gap`, but header, doc, and impl body use `dst_frac_stride`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:631` — `dst_frac_stride`
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md:65-71` — `dst_frac_stride`
- **Impl dispatch**: `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h:1081-1083` — `dst_frac_gap` (parameter name in dispatch function)
- **Impl body**: `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/asc_copy_l12l0a_trans_impl.h:28` — `dst_frac_stride`
- The dispatch layer between header and impl body uses a different name.

---

### INCONSISTENCY 6: `asc_copy_gm2l0a` (2201 arch) — Capital `S` in `src_Stride` vs lowercase `stride` in `dst_stride`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:795` — `src_Stride, dst_stride`
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:801,807,813,819,825,831,837` — all `src_Stride` (capital S)
- Every other `src_stride` in the entire codebase uses lowercase `s`.

---

### INCONSISTENCY 7: `asc_copy_l12bt` (2201 arch) — `source_gap` instead of `src_gap`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:45` — `source_gap` (full word)
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_2201.md:40` — `source_gap`
- Every other API in the codebase uses the abbreviated `src_gap`.

---

### INCONSISTENCY 8: `asc_copy_l12fb` (2201 arch) — `src_gap_size` / `dst_gap_size` with `_size` suffix

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:36` — `src_gap_size, dst_gap_size`
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_2201.md:40-41` — `src_gap_size, dst_gap_size`
- Every other API uses bare `src_gap`/`dst_gap` without the `_size` suffix. Self-consistent but naming convention deviation.

---

### INCONSISTENCY 9: `asc_set_ndim_loop*_stride` (3510 arch) — Argument order `dst, src` reversed from `asc_set_*_loop*_stride` which uses `src, dst`

- **Header ub2gm/gm2ub loops**: `include/c_api/vector_datamove/vector_datamove.h:299-307` — `asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride)` (src first, then dst)
- **Header ndim loops**: `include/c_api/vector_datamove/vector_datamove.h:310-318` — `asc_set_ndim_loop0_stride(dst_stride, src_stride)` (dst first, then src)
- **Doc**: `docs/api/c_api/vector_datamove/asc_set_ndim_loop_stride.md:27-28` — confirms `dst_stride, src_stride` order
- **Doc**: `docs/api/c_api/vector_datamove/asc_set_ub2gm_loop1_stride.md:27-28` — confirms `loop1_src_stride, loop1_dst_stride` order
- **Doc**: `docs/api/c_api/vector_datamove/asc_set_gm2ub_loop1_stride.md:27-28` — confirms `loop1_src_stride, loop1_dst_stride` order
- **Impl**: All three confirmed in `impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h:5521-5569`

---

### INCONSISTENCY 10: `asc_copy_ub2gm_align` (3510 arch) — `dst_gap` before `src_gap` (reversed from all other APIs)

- **Header**: `include/c_api/vector_datamove/vector_datamove.h:262` — `l2_cache_mode, dst_gap, src_gap` (dst before src!)
- **Header** (gm2ub 3510): `include/c_api/vector_datamove/vector_datamove.h:225` — `l2_cache_mode, src_gap, dst_gap` (src before dst)
- **Header** (ub2gm 2201): `include/c_api/vector_datamove/vector_datamove.h:138` — `src_gap, dst_gap` (src before dst)
- **Header** (gm2ub 2201): `include/c_api/vector_datamove/vector_datamove.h:74` — `src_gap, dst_gap` (src before dst)
- Every other gap/stride pair in the entire codebase places source before destination, except the 3510 `asc_copy_ub2gm_align`.

---

### INCONSISTENCY 11: `asc_copy_gm2l1` (2201 arch) — Same function, different naming: void* overload uses `src_gap`/`dst_gap`, typed overload uses `src_stride`/`dst_stride`

- **Header void***: `include/c_api/cube_datamove/cube_datamove.h:229` — `src_gap, dst_gap`
- **Header typed**: `include/c_api/cube_datamove/cube_datamove.h:300-326` — `src_stride, dst_stride`
- **Doc void*** mode (line 101): `src_gap` = "gap between end of one block and start of next" (GAP semantics)
- **Doc 2D mode** (line 112): `src_stride` = "distance between start addresses of two adjacent blocks" (STRIDE semantics)
- Different overloads of the same function use different naming conventions for what are conceptually related spacing parameters.

---

### INCONSISTENCY 12: `asc_copy_l12l0a_trans` (2201 arch) — Doc describes `dst_stride` with GAP semantics

- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md:95` — parameter named `dst_stride` but description is "目的操作数前一个迭代第一个分形的**结束地址**到下一个迭代第一个分形起始地址的间隔" which matches GAP definition (end to start), not STRIDE (start to start)
- Compare with doc for `asc_copy_l12l0b_trans_arch_2201.md:93` which names this `dst_gap` with identical description text.

---

### NON-GAP/STRIDE INCONSISTENCIES:

### N1: `asc_copy_l12l0b_trans` (2201 arch) — `half` variant uses `uint16_t dst_frac_gap`, all others use `uint64_t`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:674-675` — half variant: `uint16_t dst_frac_gap`
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:677-703` — all other types: `uint64_t dst_frac_gap`
- This is a type inconsistency between the half overload and all other type overloads of the same function.
</task_result>
```

---

## Assistant (Build · DeepSeek V4 Pro · 12.9s)

_Thinking:_

The agent did a great job finding many inconsistencies. Let me now also check the general parameter name inconsistencies beyond gap/stride. Let me also verify some of the findings directly.

Let me now do some more targeted searches to ensure completeness:
1. Search for `_gap` and `_stride` parameter declarations across all files and cross-reference
2. Find other parameter name inconsistencies not related to gap/stride

Let me also read some key files directly to verify the findings and get exact line numbers.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Compare parameter names between docs and implementation for stride-related APIs",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Identify semantic misuse of gap vs stride across all files",
      "status": "in_progress"
    },
    {
      "priority": "medium",
      "content": "Check general parameter name inconsistencies beyond gap/stride",
      "status": "pending"
    },
    {
      "priority": "high",
      "content": "Generate final markdown report",
      "status": "pending"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Compare parameter names between docs and implementation for stride-related APIs",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Identify semantic misuse of gap vs stride across all files",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "Check general parameter name inconsistencies beyond gap/stride",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "Generate final markdown report",
    "status": "pending",
    "priority": "high"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 878.7s)

_Thinking:_

Let me verify the key findings and also check for general parameter name inconsistencies. I'll use multiple parallel reads to verify the specific issues found.

Let me verify the key findings and look for more general parameter name inconsistencies.

**Tool: task**

**Input:**
```json
{
  "description": "Verify gap/stride findings in detail",
  "subagent_type": "general",
  "prompt": "Verify these specific gap/stride findings by reading the exact source files. Return the exact line numbers and content for each:\n\n1. In include/c_api/cube_datamove/cube_datamove.h, find all function declarations that use BOTH `gap` AND `stride` in the same parameter list (e.g., `src_stride, dst_gap`). List every such function with line numbers.\n\n2. In include/c_api/cube_datamove/cube_datamove.h, find `asc_copy_gm2l0a` and check if there's a capital S in `src_Stride`. List all occurrences.\n\n3. Read docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md and find the parameter table row for `dst_stride`. Read the description and verify if it uses \"结束地址\" (end address) in the description (indicating GAP semantics). Return the exact text.\n\n4. Read docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md and find `dst_gap` parameter description. Return exact text.\n\n5. Compare the two above - verify if they describe the same concept with different names.\n\n6. In include/c_api/cube_datamove/cube_datamove.h, find the 3510 arch `asc_copy_l12l0b_trans` function. Check if it uses `dst_frac_stride` or `dst_frac_gap`. Find exact line numbers and content.\n\n7. In the corresponding doc: docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md, find `dst_frac_gap` or `dst_frac_stride` and confirm the name used.\n\n8. Check if `asc_copy_ub2gm_align` in 3510 has `dst_gap` before `src_gap` (reversed order). Verify this in both header and impl.\n\nReturn ALL exact file paths, line numbers, and surrounding code context for each finding."
}
```

**Output:**
```
task_id: ses_1c0d176fbffe5Vu1MN13sLwMrW (for resuming to continue this task if needed)

<task_result>
Here are the verified findings:

---

## 1. Functions with BOTH `gap` AND `stride` in same parameter list

### In 2201 block (line 224–841):

**`asc_copy_l12l0b` (2D) — `src_stride, dst_gap`**
- `cube_datamove.h:397` — `__aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);`
- `cube_datamove.h:399-400` — sync variant
- `cube_datamove.h:402-403, 405-406, 408-409, 411-412, 414-415, 417-418, 420-421, 423-424, 426-427, 429-430, 432-433, 435-436, 438-439, 441-442` — all type variants

**`asc_copy_l12l0a` (2D) — `src_stride, dst_gap`**
- `cube_datamove.h:497` — `__aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);`
- `cube_datamove.h:499-542` — all type + sync variants

**`asc_copy_l12l0b_trans` — `src_stride, dst_gap, dst_frac_gap`**
- `cube_datamove.h:673-675` — `__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool addrmode, uint16_t dst_frac_gap);`
- `cube_datamove.h:677-727` — all type + sync variants (some use `uint64_t dst_frac_gap`)

### In 3510 block (line 842+):

**`asc_copy_l12l0b_trans` — `src_stride, dst_gap, dst_frac_stride` (HEADER)**
- `cube_datamove.h:1316-1317` — `... uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);`
- `cube_datamove.h:1319-1392` — all type + sync variants

---

## 2. `asc_copy_gm2l0a` with capital S `src_Stride`

Non-sync variants use `src_Stride` (capital S), sync variants use `src_stride`:
- `cube_datamove.h:794-795` — `uint16_t src_Stride, uint16_t dst_stride);` (bfloat16_t)
- `cube_datamove.h:797-798` — `uint16_t src_stride, uint16_t dst_stride);` (bfloat16_t sync — lowercase!)
- `cube_datamove.h:800-801, 806-807, 812-813, 818-819, 824-825, 830-831, 836-837` — all non-sync use `src_Stride`
- `cube_datamove.h:803-804, 809-810, 815-816, 821-822, 827-828, 833-834, 839-840` — all sync use `src_stride`

---

## 3. `asc_copy_l12l0a_trans.md` — `dst_stride` description (GAP semantics)

`docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md:95`:
```
| dst_stride | 输入|表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_stride∈[0, 65535]。 |
```

The parameter is named `dst_stride` but the description uses **"结束地址"** (end address) — defining it as GAP (offset from end to next start), not STRIDE (offset from start to next start). Meanwhile, the explanatory text on line 22-23 actually uses `dst_gap`:
```
dst_gap = 1，表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔为1
```

---

## 4. `asc_copy_l12l0b_trans_arch_2201.md` — `dst_gap` parameter description

`docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md:93`:
```
| dst_gap | 输入|表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。 |
```

---

## 5. Same concept, different names

**Identical descriptions, different parameter names:**
- `asc_copy_l12l0a_trans.md:95`: parameter named **`dst_stride`**, description: `前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔`
- `asc_copy_l12l0b_trans_arch_2201.md:93`: parameter named **`dst_gap`**, description: `前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔`

Both describe GAP semantics ("结束地址"/end-address based), but one calls it `stride` and the other `gap`.

---

## 6. 3510 `asc_copy_l12l0b_trans` in header vs impl

**HEADER** at `include/c_api/cube_datamove/cube_datamove.h:1316-1317`:
```cpp
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
```

**IMPL** at `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h:937-938`:
```cpp
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
```

**MISMATCH**: header says `dst_frac_stride`, impl (and internal impl at `impl/.../asc_copy_l12l0b_trans_impl.h:29`) says `dst_frac_gap`.

---

## 7. 3510 doc confirms `dst_frac_gap`

`docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md:124`:
```
| dst_frac_gap | 输入 | 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。 |
```

Doc uses `dst_frac_gap` — consistent with the impl but **inconsistent with the header** (`dst_frac_stride` at line 1317).

---

## 8. `asc_copy_ub2gm_align` — GAP order reversed in 3510

**2201 version** at `include/c_api/vector_datamove/vector_datamove.h:137-138`:
```cpp
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t n_burst, uint32_t len_burst,
            uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
```
Order: **`src_gap`** first, then **`dst_gap`**

**3510 version** at `include/c_api/vector_datamove/vector_datamove.h:261-262`:
```cpp
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t n_burst, uint32_t len_burst,
            uint8_t l2_cache_mode, uint64_t dst_gap, uint32_t src_gap);
```
Order: **`dst_gap`** first, then **`src_gap`** — REVERSED

**Impl confirms**: `impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/asc_copy_ub2gm_align_impl.h:28` — `uint64_t dst_gap, uint32_t src_gap`

**3510 doc confirms**: `docs/api/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md:18` — `uint64_t dst_gap, uint32_t src_gap`

---

### Summary of confirmed issues:

| # | Issue | Files |
|---|-------|-------|
| 2 | `src_Stride` (capital S) in non-sync `asc_copy_gm2l0a` | `include/.../cube_datamove.h:794-837` |
| 3/4/5 | `dst_stride` named but described as GAP (same desc as `dst_gap`) | `asc_copy_l12l0a_trans.md:95` vs `asc_copy_l12l0b_trans_arch_2201.md:93` |
| 6/7 | `dst_frac_stride` in header vs `dst_frac_gap` in impl/doc | `cube_datamove.h:1317` vs `cube_datamove_impl.h:938` vs doc line 124 |
| 8 | `dst_gap` before `src_gap` in 3510 (reversed from 2201's `src_gap, dst_gap`) | `vector_datamove.h:262` vs `vector_datamove.h:138` |
</task_result>
```

**Tool: task**

**Input:**
```json
{
  "description": "Find general param name inconsistencies",
  "subagent_type": "general",
  "prompt": "Search for general parameter name inconsistencies between docs/api/c_api/ documentation files and their corresponding include/c_api/ header files. Focus on:\n\n1. For each API documented in docs/api/c_api/ that has multiple architecture versions (2201 and 3510), compare the parameter names in the doc with those in the corresponding header declarations in include/c_api/.\n\n2. Key areas to check:\n   - cube_datamove functions (asc_copy_gm2l1, asc_copy_l0c2gm, asc_copy_l12l0a, asc_copy_l12l0b, asc_copy_l12bt, asc_copy_l12fb, asc_copy_l12fb_v2, etc.)\n   - vector_datamove functions  \n   - vector_compute functions with block_stride/repeat_stride parameters\n   - cube_compute functions (asc_mmad, asc_mmad_sparse, etc.)\n\n3. For each function, create a table comparing:\n   - Parameter name in doc (under \"参数说明\" or similar table)\n   - Parameter name in include header function declaration\n   - Parameter name in impl file function signature\n   - Whether they match\n\n4. Pay special attention to:\n   - `loop_size` vs `loop1_size` vs `size`\n   - `base_idx` vs `base_index` vs `start_index`\n   - `repeat` vs `repeat_times`\n   - `src_stride` vs `src_gap`\n   - `block_stride` vs `block_repeat_stride`\n   - `mask` vs `dst_mask` / `src_mask`\n   - `value` vs `val` / `scalar`\n   - `mode` vs `l2_cache_mode`\n\nReturn a comprehensive table of ALL parameter name inconsistencies found, with exact file paths and line numbers."
}
```

**Output:**
```
task_id: ses_1c0d13e74ffe3GPEFraRHTqc64 (for resuming to continue this task if needed)

<task_result>
Now I have comprehensive data. Here is the complete analysis:

---

## Parameter Name Consistency Report: Docs vs Headers vs Impl Files

### Summary

After exhaustive comparison of all `docs/api/c_api/` documentation files against `include/c_api/` header declarations and `impl/c_api/` implementation signatures across both NPU architectures (2201 & 3510), **parameter names are largely consistent between docs, headers, and impl files**. Below is the complete table of all findings.

---

### cube_datamove Functions

| Function | Doc Param Name | Header Param Name | Impl Param Name | Match? | Files |
|---|---|---|---|---|---|
| `asc_copy_gm2l1` (2201, void* burst) | `n_burst`, `len_burst`, `src_gap`, `dst_gap` | `n_burst`, `len_burst`, `src_gap`, `dst_gap` | Same as header | **MATCH** | doc: `docs/api/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md:34`<br>header: `include/c_api/cube_datamove/cube_datamove.h:228-229`<br>impl: `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h:1265` |
| `asc_copy_gm2l1` (2201, 2D typed) | `base_idx`, `repeat`, `src_stride`, `dst_stride` | `base_idx`, `repeat`, `src_stride`, `dst_stride` | Same as header | **MATCH** | doc: `...arch_2201.md:69-73`<br>header: `...cube_datamove.h:300` |
| `asc_copy_gm2l1` (3510, fill mode) | `pad_func_mode` | N/A (in impl) | `pad_func_mode` | **MATCH** | doc: `...arch_3510.md:20`<br>impl: `impl/.../npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_impl.h:22` |
| `asc_copy_gm2l1` (3510, 2D mode) | `l2_cache_ctl`, `decomp_mode` | N/A (in impl) | `l2_cache_ctl`, `decomp_mode` | **MATCH** | doc: `...arch_3510.md:32`<br>impl: `.../asc_copy_gm2l1_impl.h:39` |
| `asc_copy_l0c2gm` (2201) | `dst_stride_dst_d`, `unit_flag_mode`, `channel_split`, `nz2nd_en` | N/A (in impl) | `dst_stride_dst_d`, `unit_flag_mode`, `channel_split`, `nz2nd_en` | **MATCH** | doc: `.../asc_copy_l0c2gm_arch_2201.md:29-37`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:1630-1631` |
| `asc_copy_l0c2gm` (3510) | `loop_dst_stride`, `loop_src_stride`, `l2_cache_ctl`, `clip_relu_pre`, `unit_flag_ctl`, `split_en`, `NZ2ND_en`, `C0_pad_en`, `broadcast_en`, `NZ2DN_en`, `eltwise_op`, `eltwise_antq_en` | N/A (in impl) | Same as doc | **MATCH** | doc: `.../asc_copy_l0c2gm_arch_3510.md:58-129`<br>impl: `impl/.../npu_arch_3510/cube_datamove_impl/asc_copy_l0c2gm_impl.h:23-28` |
| `asc_copy_l12l0a` (2201, 2D) | `start_index`, `repeat`, `src_stride`, `dst_gap` | `start_index`, `repeat`, `src_stride`, `dst_gap` | Same as header | **MATCH** | doc: `.../asc_copy_l12l0a_arch_2201.md:18`<br>header: `...cube_datamove.h:496-497`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h` (matches) |
| `asc_copy_l12l0a` (3510, 2D) | `m_start_position`, `k_start_position`, `m_step`, `k_step`, `src_stride`, `dst_stride` | N/A (in impl) | Same as doc | **MATCH** | doc: `.../asc_copy_l12l0a_arch_3510.md:19`<br>impl: `impl/.../npu_arch_3510/cube_datamove_impl.h:211-212` |
| `asc_copy_l12l0b` (2201, 2D) | `start_index`, `repeat`, `src_stride`, `dst_gap` | `start_index`, `repeat`, `src_stride`, `dst_gap` | Same as header | **MATCH** | doc: `.../asc_copy_l12l0b_arch_2201.md:18`<br>header: `...cube_datamove.h:397` |
| `asc_copy_l12l0b` (3510, 2D) | `m_start_position`, `k_start_position`, `m_step`, `k_step`, `src_stride`, `dst_stride` | N/A (in impl) | Same as doc | **MATCH** | doc: `.../asc_copy_l12l0b_arch_3510.md:19`<br>impl: same naming pattern as 3510 l12l0a |
| `asc_copy_l12bt` (2201 & 3510) | `source_gap`, `dst_gap`, `conv_control` | `source_gap`, `dst_gap`, `conv_control` | Same as header | **MATCH** | doc: `.../asc_copy_l12bt_arch_2201.md:23`<br>header: `...cube_datamove.h:44-45`<br>impl 3510: `.../asc_copy_l12bt_impl.h:23-24` |
| `asc_copy_l12fb` (2201 & 3510) | `src_gap_size`, `dst_gap_size` | `src_gap_size`, `dst_gap_size` | `src_gap_size`, `dst_gap_size` | **MATCH** | doc: `.../asc_copy_l12fb_arch_2201.md:23`<br>header: `...cube_datamove.h:36`<br>impl 3510: `.../asc_copy_l12fb_impl.h:23` |
| `asc_copy_l12fb_v2` (3510) | `src_stride`, `dst_stride` | N/A (in impl) | `src_stride`, `dst_stride` | **MATCH** | doc: `.../asc_copy_l12fb_v2.md:24`<br>impl: `.../asc_copy_l12fb_v2_impl.h:22-23` |
| `asc_copy_l12l0a_trans` (2201) | `index_id`, `repeat`, `src_stride`, `dst_stride`, `addrmode`, `dst_frac_stride` | `index_id`, `repeat`, `src_stride`, `dst_stride`, `addrmode`, `dst_frac_stride` | Same as header | **MATCH** | doc: `.../asc_copy_l12l0a_trans.md:65`<br>header: `...cube_datamove.h:630-631`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:1080-1081` |
| `asc_copy_l12l0b_trans` (2201, half) | doc says `uint64_t dst_frac_gap` | `uint16_t dst_frac_gap` | `uint16_t dst_frac_gap` | **TYPE mismatch** (name `dst_frac_gap` same, but type: doc `uint64_t`, header/impl `uint16_t`) | doc: `.../asc_copy_l12l0b_trans_arch_2201.md:66`<br>header: `...cube_datamove.h:673-675`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:1165-1167` |
| `asc_copy_gm2l1_nd2nz` (2201) | `nd_num`, `n_value`, `d_value`, `src_nd_matrix_stride`, `src_d_value`, `dst_nz_c0_stride`, `dst_nz_n_stride`, `dst_nz_matrix_stride` | `nd_num`, `n_value`, `d_value`, `src_nd_matrix_stride`, `src_d_value`, `dst_nz_c0_stride`, `dst_nz_n_stride`, `dst_nz_matrix_stride` | Same as header | **MATCH** | doc: `.../asc_copy_gm2l1_nd2nz_arch_2201.md:19`<br>header: `...cube_datamove.h:329-331`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:1418-1420` |
| `asc_copy_l12l0b_sparse` (2201) | `index`, `start_index`, `repeat` | `index`, `start_index`, `repeat` | `index`, `start_index`, `repeat` | **MATCH** | header: `...cube_datamove.h:625`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:50` |

### vector_datamove Functions

| Function | Doc Param Name | Header Param Name | Impl Param Name | Match? | Files |
|---|---|---|---|---|---|
| `asc_copy_gm2ub_align` (2201) | `left_padding_num`, `right_padding_num`, `src_gap`, `dst_gap` | Same | Same | **MATCH** | doc: `.../asc_copy_gm2ub_align_arch_2201.md`<br>header: `...vector_datamove.h:73-74` |
| `asc_copy_gm2ub_align` (3510) | `l2_cache_mode`, `enable_constant_pad` | `l2_cache_mode`, `enable_constant_pad` | Same as header | **MATCH** | doc: `.../asc_copy_gm2ub_align_arch_3510.md:18`<br>header: `...vector_datamove.h:224-225`<br>impl: `.../npu_arch_3510/vector_datamove_impl.h:6297` |
| `asc_copy_ub2gm_align` (2201) | `left_padding_num`, `right_padding_num`, `src_gap`, `dst_gap` | Same | Same | **MATCH** | header: `...vector_datamove.h:137-138` |
| `asc_copy_ub2gm_align` (3510) | `l2_cache_mode` | `l2_cache_mode` | Same as header | **MATCH** | doc: `.../asc_copy_ub2gm_align_arch_3510.md:18`<br>header: `...vector_datamove.h:261-262`<br>impl: `.../npu_arch_3510/vector_datamove_impl.h:6369-6370` |
| `asc_ndim_copy_gm2ub` (3510) | `loop0_size` .. `loop4_size`, `loop0_lp_count`, `loop0_rp_count`, `padding_mode`, `cache_mode` | `loop0_size` .. `loop4_size`, `loop0_lp_count`, `loop0_rp_count`, `padding_mode`, `cache_mode` | Same as header | **MATCH** | doc: `.../asc_ndim_copy_gm2ub.md:50`<br>header: `...vector_datamove.h:343-345`<br>impl: `.../npu_arch_3510/vector_datamove_impl.h:6137-6139` |
| `asc_set_gm2ub_loop_size` | `loop1_size`, `loop2_size` | `loop1_size`, `loop2_size` | Same as header | **MATCH** | doc: `.../asc_set_gm2ub_loop_size.md:20`<br>header: `...vector_datamove.h:303` |
| `asc_set_ub2gm_loop_size` | `loop1_size`, `loop2_size` | `loop1_size`, `loop2_size` | Same as header | **MATCH** | header: `...vector_datamove.h:297` |
| `asc_set_gm2l1_loop_size` | `loop1_size`, `loop2_size` | N/A (in impl) | `loop1_size`, `loop2_size` | **MATCH** | doc: `.../asc_set_gm2l1_loop_size.md:20`<br>impl: `.../npu_arch_3510/cube_datamove_impl.h:178` |
| `asc_set_copy_pad_val` | `pad_value` | `pad_value` | N/A | **MATCH** | doc: `.../asc_set_copy_pad_val.md`<br>header: `...vector_datamove.h:31-47` |
| `asc_set_ndim_pad_value` | `pad_value` | `pad_value` | N/A | **MATCH** | header: `...vector_datamove.h:324-340` |
| `asc_set_ndim_loop_stride` | `dst_stride`, `src_stride` | `dst_stride`, `src_stride` | N/A | **MATCH** | header: `...vector_datamove.h:310-318` |
| `asc_copy_ub2ub` (2201 & 3510) | `n_burst`, `len_burst`, `src_gap`, `dst_gap` | Same | Same | **MATCH** | header: `...vector_datamove.h:51-52` |

### vector_compute Functions

| Function | Doc Param Name | Header Param Name | Impl Param Name | Match? | Files |
|---|---|---|---|---|---|
| `asc_add` (block_stride variant) | `repeat`, `dst_block_stride`, `src0_block_stride`, `src1_block_stride`, `dst_repeat_stride`, `src0_repeat_stride`, `src1_repeat_stride` | Same as doc | Same | **MATCH** | doc: `.../asc_add.md:32-43`<br>header: `...vector_compute.h:75-77` |
| `asc_eq` | `repeat`, `dst_block_stride`, `src0_block_stride`, `src1_block_stride`, `dst_repeat_stride`, `src0_repeat_stride`, `src1_repeat_stride` | Same as doc | Same | **MATCH** | doc: `.../asc_eq.md:19-24`<br>header: `...vector_compute.h:370-372` |
| `asc_add_scalar` | `value`, `repeat`, `dst_block_stride`, `src_block_stride`, `dst_repeat_stride`, `src_repeat_stride` | Same as doc | Same | **MATCH** | doc: `.../asc_add_scalar.md`<br>header: `...vector_compute.h:106-109` |
| `asc_set_vector_mask` | `mask1`, `mask0` | `mask1`, `mask0` | Same | **MATCH** | doc: `.../asc_set_vector_mask.md:20`<br>header: `...vector_compute.h:61` |
| `asc_set_cmp_mask` | `sel_mask` | `sel_mask` | Same | **MATCH** | doc: `.../asc_set_cmp_mask.md:17`<br>header: `...vector_compute.h:55` |
| `asc_get_cmp_mask` | `dst` | `dst` | Same | **MATCH** | doc: `.../asc_get_cmp_mask.md:17`<br>header: `...vector_compute.h:41` |
| `asc_datablock_reduce_sum/max/min` | `repeat`, `dst_repeat_stride`, `src_block_stride`, `src_repeat_stride` | Same as doc | Same | **MATCH** | header: `...vector_compute.h:200-201` |
| `asc_repeat_reduce_*` | `repeat`, `dst_repeat_stride`, `src_block_stride`, `src_repeat_stride` | Same as doc | Same | **MATCH** | header: `...vector_compute.h:243-244` |
| `asc_bfloat162float` / `asc_float2half` / etc. | `repeat`, `dst_block_stride`, `src_block_stride`, `dst_repeat_stride`, `src_repeat_stride` | Same as doc | Same | **MATCH** | header: `...vector_compute.h:411-412` |

### cube_compute Functions

| Function | Doc Param Name | Header Param Name | Impl Param Name | Match? | Files |
|---|---|---|---|---|---|
| `asc_mmad` (2201) | `c_matrix`, `a_matrix`, `b_matrix`, `left_height`, `n_dim`, `right_width`, `feat_offset`, `smask_offset`, `unit_flag`, `k_direction_align`, `is_weight_offset`, `c_matrix_source`, `c_matrix_init_val` | Same as doc | Same as header | **MATCH** | doc: `.../asc_mmad.md:23-32`<br>header: `...cube_compute.h:34-47` |
| `asc_mmad` (3510) | `c_matrix`, `a_matrix`, `b_matrix`, `left_height`, `n_dim`, `right_width`, `unit_flag`, `disable_gemv`, `c_matrix_source`, `c_matrix_init_val` | Same as doc | Same as header | **MATCH** | doc: `.../asc_mmad.md:39-46`<br>header: `...cube_compute.h:199-201` |
| `asc_mmad_mx` (3510) | `c_matrix`, `a_matrix`, `b_matrix`, `left_height`, `n_dim`, `right_width`, `unit_flag`, `disable_gemv`, `c_matrix_source`, `c_matrix_init_val` | Same as doc | Same as header | **MATCH** | header: `...cube_compute.h:134-136` |
| `asc_mmad_sparse` (2201) | `c`, `a`, `b`, `m`, `k`, `n`, `unit_flag`, `cmatrix_source`, `cmatrix_init_val` | Same as doc | Same as header | **MATCH** | doc: `.../asc_mmad_sparse.md:18`<br>header: `...cube_compute.h:106` |
| `asc_mmad_s4` (2201) | `c_matrix`, `a_matrix`, `b_matrix`, `left_height`, `n_dim`, `right_width`, `unit_flag`, `k_direction_align`, `c_matrix_source`, `c_matrix_init_val` | Same as doc | Same | **MATCH** | header: `...cube_compute.h:92-93` |
| `asc_set_l0c2gm_config` | `relu_pre`, `quant_pre`, `enable_unit_flag` | Same as doc | Same as doc | **MATCH** | doc: `.../asc_set_l0c2gm_config.md:17`<br>header: `...cube_compute.h:30`<br>impl 2201: `impl/.../npu_arch_2201/cube_compute_impl.h:238`<br>impl 3510: `impl/.../npu_arch_3510/cube_compute_impl.h:19` |
| `asc_set_l0c2gm_nz2nd` (3510) | `nd_num`, `src_nd_stride`, `dst_nd_stride` | N/A (in impl) | Same as doc | **MATCH** | doc: `.../asc_set_l0c2gm_nz2nd.md:17`<br>impl: `.../npu_arch_3510/cube_compute_impl.h:24` |
| `asc_enable_hf32_trans` | `mode` | `mode` | `mode` | **MATCH** | doc: `.../asc_enable_hf32_trans.md:18`<br>header: `...cube_compute.h:117` |

### Utility Functions (config-related)

| Function | Doc Param Name | Header Param Name | Impl Param Name | Match? | Files |
|---|---|---|---|---|---|
| `asc_set_l13d_size` | `value` | `value` | `value` | **MATCH** | header: `...cube_datamove.h:729`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:568` |
| `asc_set_l13d_padding` | `config` | `config` | `config` | **MATCH** | header: `...cube_datamove.h:731`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:1609` |
| `asc_set_l13d_rpt` | `config` | `config` | `config` | **MATCH** | header: `...cube_datamove.h:30`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:62` |
| `asc_set_l13d_fmatrix` | `config` | `config` | `config` | **MATCH** | header: `...cube_datamove.h:32` |
| `asc_set_l0c_copy_prequant` | `config` | `config` | `config` | **MATCH** | header: `...cube_datamove.h:756`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:1604` |
| `asc_set_l0c_copy_params` | `nd_num`, `src_nd_stride`, `dst_nd_stride` | `nd_num`, `src_nd_stride`, `dst_nd_stride` | Same | **MATCH** | doc: `.../asc_set_l0c_copy_params.md:16`<br>header: `...cube_datamove.h:758`<br>impl: `impl/.../npu_arch_2201/cube_datamove_impl.h:563` |
| `asc_fill_l0a` / `asc_fill_l0b` / `asc_fill_l1` | `value`, `config` | `value`, `config` | Same | **MATCH** | header: `...cube_datamove.h:52` |

---

### Findings Summary

**1 confirmed inconsistency found:**

| # | Function | Issue | Doc | Header | Impl |
|---|---|---|---|---|---|
| 1 | `asc_copy_l12l0b_trans` (2201, `half` overload) | **Type mismatch**: param name is `dst_frac_gap` in all three, but doc declares `uint64_t` while header & impl declare `uint16_t` | `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md:66` | `include/c_api/cube_datamove/cube_datamove.h:673-675` | `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h:1165-1167` |

**All other parameter names are consistent** between documentation and source code across both architectures. Key observations:

- `l2_cache_ctl` (cube_datamove) vs `l2_cache_mode` (vector_datamove) are separate parameters in different module domains and are consistently named in both docs and code per-domain -- this is not a doc-vs-code inconsistency.
- `start_index` (l12l0a/l12l0b 2201), `base_idx` (gm2l1 2201 2D), and `index_id` (l12l0a_trans/l12l0b_trans 2201) are separate parameter names in separate functions, each consistent between docs and code.
- `src_gap` (void* burst), `src_gap_size` (l12fb), `src_stride` (2D typed / l12fb_v2), and `source_gap` (l12bt) are consistently used per-function between docs and code.
- `dst_block_stride` vs `dst_repeat_stride` are consistently two separate parameter types in vector_compute, matched in all three sources.
</task_result>
```

---

## Assistant (Build · DeepSeek V4 Pro · 547.5s)

_Thinking:_

Now I have a comprehensive view. Let me also check for some additional potential issues:

1. Need to verify the `asc_copy_l12l0b_trans` 3510 header vs impl vs doc inconsistency (dst_frac_stride vs dst_frac_gap)
2. Need to check the `asc_copy_gm2l0a` capital S issue more carefully
3. Need to check the `dst_stride` in l12l0a_trans which has GAP semantics
4. Need to check parameter order inversion in ub2gm_align 3510

Let me verify a few more files and then also look for other potential issues like:
- Docs referencing parameter names that don't exist (or vice versa)
- Type mismatches between doc and header

Let me now do a few more checks.

**Tool: task**

**Input:**
```json
{
  "description": "Find additional doc vs header type mismatches",
  "subagent_type": "general",
  "prompt": "Search for TYPE mismatches (not name mismatches) between documentation and header/implementation files.\n\n1. Search docs/api/c_api/ for declarations that specify types (like `uint64_t`, `uint16_t`, `uint32_t`, `uint8_t`, etc.)\n2. Search include/c_api/ headers for corresponding function declarations\n3. Find any case where the TYPE of a parameter in the doc differs from the TYPE in the header/implementation\n\nFocus especially on these files:\n- cube_datamove functions\n- vector_datamove functions  \n- cube_compute functions\n\nAlso check for:\n- Any parameter that exists in doc but not in header (or vice versa)\n- Default values mentioned in doc that differ from implementation defaults\n\nReturn a detailed list with exact file paths and line numbers."
}
```

**Output:**
```
task_id: ses_1c0c41cadffejDN45KA09yAfsa (for resuming to continue this task if needed)

<task_result>
Based on my systematic comparison of all documentation files against their corresponding headers, here is the complete list of findings:

---

## TYPE MISMATCHES FOUND

### 1. `asc_copy_l12l0b_trans` (arch 2201) - half variant

**File:** `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md:66`

Doc says:
```c
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src,
    uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool addrmode, uint64_t dst_frac_gap)
```

**File:** `include/c_api/cube_datamove/cube_datamove.h:674-675`

Header says:
```c
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src,
    uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool addrmode, uint16_t dst_frac_gap)
```

**Mismatch:** `dst_frac_gap` is documented as `uint64_t` but the header declares it as `uint16_t` for the `half` variant. (All other datatype variants correctly use `uint64_t` in both doc and header.)

---

### 2. `asc_fill_l0a` - missing `half`/`uint32_t` value overloads for `bfloat16_t*`

**File:** `docs/api/c_api/cube_datamove/asc_fill_l0a.md:25-27`

Doc lists:
```c
__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
```

**File:** `include/c_api/cube_datamove/cube_datamove.h:100`

Header **only** has:
```c
__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);
```

**Mismatch:** The `half value` and `uint32_t value` overloads exist in the doc but not in the header.

---

### 3. `asc_fill_l0a_sync` - same missing overloads

**File:** `docs/api/c_api/cube_datamove/asc_fill_l0a.md:45-47`

Doc lists `half value` and `uint32_t value` overloads for `bfloat16_t* dst`.

**File:** `include/c_api/cube_datamove/cube_datamove.h:102`

Header only has `bfloat16_t value` overload.

---

### 4. `asc_fill_l0b` - missing `half`/`uint32_t` value overloads for `bfloat16_t*`

**File:** `docs/api/c_api/cube_datamove/asc_fill_l0b.md:25-27`

Doc lists:
```c
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, bfloat16_t value, ...)
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, half value, ...)
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, uint32_t value, ...)
```

**File:** `include/c_api/cube_datamove/cube_datamove.h:153`

Header **only** has:
```c
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, bfloat16_t value, ...);
```

---

### 5. `asc_fill_l0b_sync` - same missing overloads

**File:** `docs/api/c_api/cube_datamove/asc_fill_l0b.md:45-47`

Doc lists `half value` and `uint32_t value` overloads for `bfloat16_t* dst`.

**File:** `include/c_api/cube_datamove/cube_datamove.h:155`

Header only has `bfloat16_t value` overload.

---

## SUMMARY

| # | Function | File:Line (Doc) | File:Line (Header) | Doc Type | Header Type |
|---|----------|-----------------|---------------------|----------|-------------|
| 1 | `asc_copy_l12l0b_trans` (half, 2201) | `..._arch_2201.md:66` | `cube_datamove.h:675` | `uint64_t dst_frac_gap` | `uint16_t dst_frac_gap` |
| 2 | `asc_fill_l0a` | `asc_fill_l0a.md:26` | `cube_datamove.h:(missing)` | `half value` overload | **not declared** |
| 3 | `asc_fill_l0a` | `asc_fill_l0a.md:27` | `cube_datamove.h:(missing)` | `uint32_t value` overload | **not declared** |
| 4 | `asc_fill_l0a_sync` | `asc_fill_l0a.md:46` | `cube_datamove.h:(missing)` | `half value` overload | **not declared** |
| 5 | `asc_fill_l0a_sync` | `asc_fill_l0a.md:47` | `cube_datamove.h:(missing)` | `uint32_t value` overload | **not declared** |
| 6 | `asc_fill_l0b` | `asc_fill_l0b.md:26` | `cube_datamove.h:(missing)` | `half value` overload | **not declared** |
| 7 | `asc_fill_l0b` | `asc_fill_l0b.md:27` | `cube_datamove.h:(missing)` | `uint32_t value` overload | **not declared** |
| 8 | `asc_fill_l0b_sync` | `asc_fill_l0b.md:46` | `cube_datamove.h:(missing)` | `half value` overload | **not declared** |
| 9 | `asc_fill_l0b_sync` | `asc_fill_l0b.md:47` | `cube_datamove.h:(missing)` | `uint32_t value` overload | **not declared** |

No type mismatches were found in the `vector_datamove` or `cube_compute` function sets. The `cube_datamove` region accounts for all mismatches.
</task_result>
```

**Tool: task**

**Input:**
```json
{
  "description": "Check semantic gap vs stride misuse in docs",
  "subagent_type": "general",
  "prompt": "In the codebase at /home/joey/code/joey/asc-devkit, the correct definitions are:\n- **gap**: Distance between the END address of the previous element and the START address of the next element.\n- **stride**: Distance between the START address of the previous element and the START address of the next element.\n\nSearch ALL documentation files under docs/api/c_api/ for `stride` parameters and read their descriptions in the parameter tables. Find cases where:\n1. A parameter named `*stride*` is described with GAP semantics (mentioning \"结束地址\" / end address)\n2. A parameter named `*gap*` is described with STRIDE semantics (mentioning \"起始地址\" / start address without mentioning end address)\n\nAlso check the converse: find any parameter named `*stride*` in the header files but `*gap*` in the documentation, and check if the semantic descriptions match the naming.\n\nSearch comprehensively across all .md files in docs/api/c_api/, and cross-reference with include/c_api/ headers.\n\nReturn the exact file paths, line numbers, parameter names, and the text of their descriptions."
}
```

**Output:**
```
task_id: ses_1c0c3f24dffe5H5cYeiihiy8xg (for resuming to continue this task if needed)

<task_result>
Now I have all the data needed. Here is the comprehensive report:

---

## Results: stride/gap Naming vs Semantic Mismatches

### Category 1 — Parameter named `*stride*` described with GAP semantics ("结束地址" / "尾")

**1. `asc_copy_l12l0a_trans.md` — `dst_stride` and `dst_frac_stride`**

- **File**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md`
- **Line 95**, parameter `dst_stride`:
  > 表示相邻迭代间，目的操作数前一个迭代第一个分形的**结束地址**到下一个迭代第一个分形起始地址的间隔。取值范围：dst_stride∈[0, 65535]。
- **Line 97**, parameter `dst_frac_stride`:
  > 每个迭代内目的操作数转置前一个分形**结束地址**与后一个分形起始地址的间隔，单位为512B
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:631` — uses `dst_stride`, `dst_frac_stride`
- **Verdict**: Both `dst_stride` and `dst_frac_stride` describe **end-address→start-address** (GAP semantics), but are named `*stride`.

**2. `asc_copy_l12fb_v2.md` — `src_stride` and `dst_stride`**

- **File**: `docs/api/c_api/cube_datamove/asc_copy_l12fb_v2.md`
- **Line 41**, parameter `src_stride`:
  > 源操作数相邻连续数据块的间隔（前面一个数据块的**尾**与后面一个数据块的头**的间隔）。<br>单位为DataBlock（32字节）。
- **Line 42**, parameter `dst_stride`:
  > 目的操作数相邻连续数据块的间隔（前面一个数据块的**尾**与后面一个数据块的头**的间隔）。<br>单位为DataBlock（32字节）。
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:1888` — uses `src_stride`, `dst_stride`
- **Verdict**: Both `src_stride` and `dst_stride` describe **tail→head** (GAP semantics), but are named `*stride`. Notably, the older `asc_copy_l12fb` (`cube_datamove.h:35–36`) used `src_gap_size`/`dst_gap_size` with the same GAP description; the `_v2` renamed to `stride` but kept the GAP description.

---

### Category 2 — Doc uses `*gap` but header uses `*stride` (with GAP semantic description)

**3. `asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md` — `dst_frac_gap` / `src_frac_gap`**

- **File**: `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md`
- **Line 124**, doc name `dst_frac_gap`:
  > 每个迭代内目的操作数转置前一个分形**结束地址**与后一个分形起始地址的间隔。单位为512B。
- **Line 125**, doc name `src_frac_gap`:
  > 每个迭代内源操作数转置前一个分形**结束地址**与后一个分形起始地址的间隔。单位为512B。
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:1316–1317` — uses `dst_frac_stride`, `src_frac_stride`
- **Verdict**: Documentation uses `*_frac_gap` (correct, matches GAP semantics: end→start) but the header uses `*_frac_stride`. The semantic description is GAP, so the **doc naming is correct** but the **header naming is wrong**.

---

### Additionally confirmed consistent (for reference):

| File | Param | Doc Desc | Header | Status |
|------|-------|----------|--------|--------|
| `asc_copy_l12l0b_trans_arch_2201.md:93` | `dst_gap` | end→start (GAP) | `dst_gap` ✓ | Consistent |
| `asc_copy_l12l0b_trans_arch_2201.md:92` | `src_stride` | start→start (STRIDE) | `src_stride` ✓ | Consistent |
| `asc_copy_l12l0b_trans_arch_3510.md:122` | `src_stride` | start→start (STRIDE) | `src_stride` ✓ | Consistent |
| `asc_copy_l12l0c.md:55–56` | `src_stride`, `dst_stride` | start→start (STRIDE) | `src_stride`, `dst_stride` ✓ | Consistent |
| `asc_copy_l12gm.md:36–37` | `src_stride`, `dst_stride` | start→start (STRIDE) | `src_stride`, `dst_stride` ✓ | Consistent |
| `asc_copy_gm2l0a.md:48–49` | `src_stride`, `dst_stride` | start→start (STRIDE) | `src_stride`, `dst_stride` ✓ | Consistent |
| `asc_copy_l12l0a_trans.md:94` | `src_stride` | start→start (STRIDE) | `src_stride` ✓ | Consistent |

---

### Summary of 3 actionable issues:

| # | File:Line | Doc Param Name | Header Param Name | Description Semantics | Problem |
|---|-----------|----------------|-------------------|----------------------|---------|
| 1 | `asc_copy_l12l0a_trans.md:95` | `dst_stride` | `dst_stride` | GAP (结束→起始) | Named `stride` but describes GAP |
| 2 | `asc_copy_l12l0a_trans.md:97` | `dst_frac_stride` | `dst_frac_stride` | GAP (结束→起始) | Named `stride` but describes GAP |
| 3 | `asc_copy_l12fb_v2.md:41–42` | `src_stride`, `dst_stride` | `src_stride`, `dst_stride` | GAP (尾→头) | Named `stride` but describes GAP |
| 4 | `asc_copy_l12l0b_trans_arch_3510.md:124–125` | `dst_frac_gap`, `src_frac_gap` | `dst_frac_stride`, `src_frac_stride` | GAP | Doc says `gap`, header says `stride` |
</task_result>
```

---

## Assistant (Build · DeepSeek V4 Pro · 26.3s)

_Thinking:_

Now I have a comprehensive picture. Let me also check for a few more specific things:

1. The `asc_copy_gm2l1` 2201 has different overloads with different names: void* uses `src_gap`/`dst_gap`, typed uses `src_stride`/`dst_stride`. Need to verify if these actually describe different semantics.
2. The `asc_copy_l12l0a` / `asc_copy_l12l0b` (2D, 2201) use `src_stride`/`dst_gap` in same signature - need to verify semantics.
3. The `asc_set_ndim_loop_stride` parameter order inversion.

Let me read a few more files.

**Tool: bash**

**Input:**
```json
{
  "description": "Check gm2l1 doc for gap vs stride semantics",
  "command": "grep -n -A2 'src_gap\\|dst_gap\\|src_stride\\|dst_stride' /home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md 2>/dev/null | head -60"
}
```

**Output:**
```
34:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
35-    __aicore__ inline void asc_copy_gm2l1_pad1(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
36:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
37-    __aicore__ inline void asc_copy_gm2l1_pad2(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
38:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
39-    __aicore__ inline void asc_copy_gm2l1_pad3(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
40:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
41-    __aicore__ inline void asc_copy_gm2l1_pad4(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
42:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
43-    __aicore__ inline void asc_copy_gm2l1_pad5(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
44:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
45-    __aicore__ inline void asc_copy_gm2l1_pad6(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
46:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
47-    __aicore__ inline void asc_copy_gm2l1_pad7(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
48:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
49-    __aicore__ inline void asc_copy_gm2l1_pad8(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst,
50:                                      uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
51-    ```
52-
--
69:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
70:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
71:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
72:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
73:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
74:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
75:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
76-    ```
77-
--
81:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
82:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
83:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
84:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
85:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
86:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
87:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
88-    ```
89-
--
101:| src_gap   | 输入    | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。  |
102:| dst_gap   | 输入    | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |
103-
104-### 2D搬运模式参数
--
112:| src_stride |输入| 输入数据中两个相邻连续数据块之间的距离。 |
113:| dst_stride | 输入 | 输出数据中两个相邻连续数据块之间的距离。 |
114-
115-## 返回值说明
--
142:constexpr uint16_t src_gap = 0;
143-// 目的操作数连续排布
144:constexpr uint16_t dst_gap = 0;
145:asc_copy_gm2l1((__cbuf__ void*)dst, (__gm__ void*)src, n_burst, len_burst, src_gap, dst_gap);
146-
147-// 2D数据搬运模式
--
153:constexpr uint16_t src_stride = 0;
154:constexpr uint16_t dst_stride = 1;
155-__gm__ half src[256];

```

**Tool: bash**

**Input:**
```json
{
  "description": "Check l12l0a 2201 doc for gap vs stride",
  "command": "grep -n -A2 'src_stride\\|dst_gap' /home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md 2>/dev/null | head -40"
}
```

**Output:**
```
18:    __aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
19:    __aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
20:    __aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
21:    __aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
22:    __aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
23:    __aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
24:    __aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
25:    __aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
26-    ```
27-
--
31:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
32:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
33:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
34:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
35:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
36:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
37:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
38:    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
39-    ```
40-
--
76:| src_stride | 输入 | 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。 |
77:| dst_gap | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。 |
78-
79-表2 3D格式参数说明

```

**Tool: bash**

**Input:**
```json
{
  "description": "Check l12l0b 2201 doc for gap vs stride",
  "command": "grep -n -A2 'src_stride\\|dst_gap' /home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_2201.md 2>/dev/null | head -40"
}
```

**Output:**
```
18:    __aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
19:    __aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
20:    __aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
21:    __aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
22:    __aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
23:    __aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
24:    __aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
25:    __aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
26-    ```
27-
--
31:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
32:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
33:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
34:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
35:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
36:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
37:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
38:    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
39-    ```
40-
--
70:| src_stride | 输入 | 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。 |
71:| dst_gap | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。 |
72-
73-表2 3D格式参数说明

```

**Tool: bash**

**Input:**
```json
{
  "description": "Check gm2l1 typed 2201 doc for src_stride/dst_stride",
  "command": "grep -n -A2 'src_stride\\|dst_stride' /home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md 2>/dev/null | head -40"
}
```

**Output:**
```
69:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
70:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
71:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
72:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
73:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
74:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
75:    __aicore__ inline void asc_copy_gm2l1(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
76-    ```
77-
--
81:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
82:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
83:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
84:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
85:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
86:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
87:    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride)
88-    ```
89-
--
112:| src_stride |输入| 输入数据中两个相邻连续数据块之间的距离。 |
113:| dst_stride | 输入 | 输出数据中两个相邻连续数据块之间的距离。 |
114-
115-## 返回值说明
--
153:constexpr uint16_t src_stride = 0;
154:constexpr uint16_t dst_stride = 1;
155-__gm__ half src[256];
156-__cbuf__ half dst[256];
157:asc_copy_gm2l1(dst, src, base_idx, repeat, src_stride, dst_stride);
158-```

```

---

## Assistant (Build · DeepSeek V4 Pro · 19.1s)

_Thinking:_

Now I have a very clear picture. Let me also check for the `src_Stride` capital S issue and verify the 3510 `asc_copy_l12l0b_trans` header vs impl in more detail.

Also let me check if there are other type inconsistencies between doc and header.

Let me also check a few more things that might be interesting:
- The `asc_copy_l12l0b_trans` 2201 half/doc type mismatch
- The `asc_copy_gm2l0a` 2201 capital S issue
- The `asc_set_ndim_loop_stride` argument order vs `asc_set_*_loop*_stride` argument order

Now I have enough data to write the final report. Let me also quickly verify the `asc_copy_l12l0b_trans` 3510 header vs impl condition.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h",
  "offset": 1310,
  "limit": 30
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h</path>
<type>file</type>
<content>
1310: 
1311: __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
1312:     uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
1313: 
1314: // ==========asc_copy_l12l0b_trans, bfloat16_t/fp8_e4m3fn_t/fp8_e5m2_t/half/float/hifloat8_t/int32_t/int8_t/uint32_t/uint8_t/fp4x2_e1m2_t/fp4x2_e2m1_t/int4b_t==========
1315: 
1316: __aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1317:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1318: 
1319: __aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1320:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1321: 
1322: __aicore__ inline void asc_copy_l12l0b_trans(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1323:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1324: 
1325: __aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1326:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1327: 
1328: __aicore__ inline void asc_copy_l12l0b_trans(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1329:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1330: 
1331: __aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1332:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1333: 
1334: __aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1335:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1336: 
1337: __aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
1338:     uint16_t dst_gap, uint16_t dst_frac_stride, uint16_t src_frac_stride);
1339: 

(Showing lines 1310-1339 of 2528. Use offset=1340 to continue.)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h",
  "offset": 790,
  "limit": 50
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h</path>
<type>file</type>
<content>
790: 
791: __aicore__ inline void asc_copy_gm2l0b_sync(__cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
792: 
793: // ==========asc_l0c2l1_l0c2l1  bfloat16_t/half/float/int32_t/uint8_t/int8_t/uint32_t/===========
794: __aicore__ inline void asc_copy_gm2l0a(__ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat,
795:     uint16_t src_Stride, uint16_t dst_stride);
796: 
797: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat,
798:     uint16_t src_stride, uint16_t dst_stride);
799: 
800: __aicore__ inline void asc_copy_gm2l0a(__ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat,
801:     uint16_t src_Stride, uint16_t dst_stride);
802: 
803: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat,
804:     uint16_t src_Stride, uint16_t dst_stride);
805: 
806: __aicore__ inline void asc_copy_gm2l0a(__ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat,
807:     uint16_t src_Stride, uint16_t dst_stride);
808: 
809: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat,
810:     uint16_t src_Stride, uint16_t dst_stride);
811: 
812: __aicore__ inline void asc_copy_gm2l0a(__ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat,
813:     uint16_t src_Stride, uint16_t dst_stride);
814: 
815: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat,
816:     uint16_t src_Stride, uint16_t dst_stride);
817: 
818: __aicore__ inline void asc_copy_gm2l0a(__ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat,
819:     uint16_t src_Stride, uint16_t dst_stride);
820: 
821: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat,
822:     uint16_t src_Stride, uint16_t dst_stride);
823: 
824: __aicore__ inline void asc_copy_gm2l0a(__ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat,
825:     uint16_t src_Stride, uint16_t dst_stride);
826: 
827: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat,
828:     uint16_t src_Stride, uint16_t dst_stride);
829: 
830: __aicore__ inline void asc_copy_gm2l0a(__ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat,
831:     uint16_t src_Stride, uint16_t dst_stride);
832: 
833: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat,
834:     uint16_t src_Stride, uint16_t dst_stride);
835: 
836: __aicore__ inline void asc_copy_gm2l0a(__ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat,
837:     uint16_t src_Stride, uint16_t dst_stride);
838: 
839: __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat,

(Showing lines 790-839 of 2528. Use offset=840 to continue.)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h",
  "offset": 630,
  "limit": 50
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/include/c_api/cube_datamove/cube_datamove.h</path>
<type>file</type>
<content>
630: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ half* dst, __cbuf__ half* src, uint16_t index_id,
631:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
632: 
633: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t index_id,
634:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
635: 
636: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id,
637:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
638: 
639: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id,
640:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
641: 
642: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ float* dst, __cbuf__ float* src, uint16_t index_id,
643:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
644: 
645: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t index_id,
646:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
647: 
648: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id,
649:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
650: 
651: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id,
652:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
653: 
654: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id,
655:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
656: 
657: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id,
658:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
659: 
660: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id,
661:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
662: 
663: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id,
664:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
665: 
666: __aicore__ inline void asc_copy_l12l0a_trans(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id,
667:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
668: 
669: __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id,
670:     uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride);
671: 
672: // ==========asc_copy_l12l0b_trans=========
673: __aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat,
674:                                              uint16_t src_stride, uint16_t dst_gap, bool addrmode,
675:                                              uint16_t dst_frac_gap);
676: 
677: __aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id,
678:                                              uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool addrmode,
679:                                              uint64_t dst_frac_gap);

(Showing lines 630-679 of 2528. Use offset=680 to continue.)
</content>
```

---

## Assistant (Build · DeepSeek V4 Pro · 21.1s)

_Thinking:_

Now let me look at a few more things:

1. The `asc_copy_gm2l0a` issue with `src_Stride` is real. In the sync variants, it's `src_stride`, but in non-sync it's `src_Stride`. Actually, let me look more carefully - at line 803-804, the sync variant has `src_Stride` too. Let me recheck.

Wait, looking back: at line 797-798 (bfloat sync), it's `src_stride` (lowercase). But at line 803-804 (half sync), it's `src_Stride` (capital S). So it's inconsistent even among sync variants.

Actually no, let me re-read: at 794-795 (bfloat non-sync): `src_Stride`, at 797-798 (bfloat sync): `src_stride` (lowercase!). So for bfloat, non-sync uses `src_Stride` and sync uses `src_stride`. For half at 800-801 (non-sync): `src_Stride`, at 803-804 (sync): `src_Stride`. So the bfloat sync variant is the odd one out.

Now let me also check if there are any issues with the `asc_copy_l12l0a_trans` doc. At doc line 22-23, there's mention of `dst_gap` but the parameter is named `dst_stride`.

Let me also look at `asc_copy_l12fb_v2.md` to verify the gap/stride semantic issue.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md",
  "limit": 50,
  "offset": 60
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md</path>
<type>file</type>
<content>
60: ## 函数原型
61: 
62: - 高维切分搬运
63: 
64:     ```cpp
65:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
66:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
67:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
68:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
69:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
70:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
71:     __aicore__ inline void asc_copy_l12l0a_trans(__ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
72:     ```
73: 
74: - 同步高维切分搬运 
75: 
76:     ```cpp
77:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
78:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
79:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
80:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
81:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
82:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
83:     __aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t dst_frac_stride)
84:     ```
85: 
86: ## 参数说明
87: 
88: | 参数名       | 输入/输出 | 描述               |
89: | --------- | ----- | ---------------- |
90: | dst       | 输出    | 目的操作数。            |
91: | src | 输入    | 源操作数。            |
92: | index_id |输入 |方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0 为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。|
93: | repeat |输入 |迭代重复次数。repeat∈[0, 255]。<br> 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；<br> 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；<br> 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。|
94: | src_stride | 输入|表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。<br> 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； <br> 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；<br> 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； <br> 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。|
95: | dst_stride | 输入|表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_stride∈[0, 65535]。 |
96: | addrmode | 输入| 控制地址更新方式。<br>false: 递增，每次迭代在前一个地址的基础上加上src_stride。<br>true: 递减，每次迭代在前一个地址的基础上减去src_stride。 |
97: | dst_frac_stride | 输入  | 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。|
98: 
99: ## 返回值说明
100: 
101: 无
102: 
103: ## 流水类型
104: 
105: PIPE_MTE1
106: 
107: ## 约束说明
108: 
109: - repeat=0表示不执行搬运操作。

(Showing lines 60-109 of 132. Use offset=110 to continue.)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12fb_v2.md",
  "limit": 50,
  "offset": 1
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12fb_v2.md</path>
<type>file</type>
<content>
1: # asc_copy_l12fb_v2
2: 
3: ## AI处理器支持情况
4: 
5: | AI处理器类型 | 是否支持  |
6: | :----------------------- | :------: |
7: |<cann-filter npu_type = "950"> Ascend 950PR/Ascend 950DT | √</cann-filter> |
8: | Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    x    |
9: | Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    x    |
10: 
11: ## 功能说明
12: 
13: 将数据从L1 Buffer搬运到Fixpipe Buffer中，Fixpipe Buffer用于存放量化参数。
14: 
15: ## 函数原型
16: 
17: - 前n个数据搬运
18:     ```c++
19:     __aicore__ inline void asc_copy_l12fb_v2(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
20:     ```
21: 
22: - 高维切分搬运
23:     ```cpp
24:     __aicore__ inline void asc_copy_l12fb_v2(__fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_stride, uint16_t dst_stride)
25:     ```
26: 
27: - 同步计算
28:     ```c++
29:     __aicore__ inline void asc_copy_l12fb_v2_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
30:     ```
31: 
32: ## 参数说明
33: 
34: | 参数名 | 输入/输出 | 描述 |
35: | :--- | :--- | :--- |
36: | dst | 输出 | 目的操作数起始地址。 |
37: | src | 输入 | 源操作数起始地址。 |
38: | size | 输入 | 搬运数据大小（字节）。|
39: | n_burst | 输入 | 待搬运的连续传输数据块个数。取值范围：[1, 4095]。 |
40: | len_burst | 输入 | 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。 |
41: | src_stride | 输入 | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |
42: | dst_stride | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |
43: 
44: ## 返回值说明
45: 
46: 无
47: 
48: ## 流水类型
49: 
50: PIPE_MTE1

(Showing lines 1-50 of 67. Use offset=51 to continue.)
</content>
```

---

## Assistant (Build · DeepSeek V4 Pro · 13.2s)

_Thinking:_

Now I also need to check one more thing: the `asc_copy_l12l0a_trans.md` at line 22 mentions `dst_gap` but the parameter is named `dst_stride`. Let me verify.

Let me also look at more of the doc for `asc_copy_l12l0a_trans.md` around line 20-25 to see the `dst_gap` usage in examples.

And for `asc_copy_l12l0b_trans_arch_3510.md`, I need to confirm the doc uses `dst_frac_gap` vs header uses `dst_frac_stride`.

Let me read those specific lines.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md",
  "limit": 30,
  "offset": 15
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md</path>
<type>file</type>
<content>
15: 
16: - 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据，可处理2个分形（一个分形512B），每次迭代中，源操作数中2个连续的16 × 32分形将被合并为1个32 × 32的方块矩阵，基于方块矩阵做转置，转置后分裂为2个16 × 32分形，根据目的操作数分形间隔等参数可以有不同的排布。
17: 
18: 如下图示例：
19: 
20: - 共处理3072B的数据，每次迭代处理32 × 32 × 1B数据，需要3次迭代可以完成，repeat_time = 3；
21: - src_stride = 1，表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔为1（单位：32 × 32 × 1B），这里的单位实际上是拼接后的方块矩阵的大小；
22: - dst_gap = 1，表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔为1（单位：512B）；
23: - dst_frac_gap = 0，表示每个迭代内目的操作数前一个分形的结束地址与后一个分形起始地址的间隔为0（单位：512B）。
24: 
25: ![](../figures/asc_copy_l12l0b_trans_b8示例一.png)
26: 
27: 如下图示例：
28: 
29: - repeat_time和src_stride的解释和上图示例一致。
30: - dst_gap = 0，表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址和下一个迭代第一个分形起始地址无间隔。
31: - dst_frac_gap = 2，表示每个迭代内目的操作数前一个分形的结束地址与后一个分形起始地址的间隔为2（单位：512B）。
32: 
33: ![](../figures/asc_copy_l12l0b_trans_b8示例二.png)
34: 
35: - 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据，可处理1个分形（一个分形512B），每次迭代中，源操作数中1个16 × 16分形将被转置。
36:   - 因为每次迭代处理16 × 16 × 2B数据，需要3次迭代可以完成，repeat_time = 3；
37:   - src_stride = 1，表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔为1 （单位：16 × 16 × 2B）；
38:   - dst_gap = 0，表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址无间隔；
39:   - 该场景下，因为其分形即为方块矩阵，每个迭代处理一个分形，不存在迭代内分形的间隔，该参数设置无效。
40: 
41: ![](../figures/asc_copy_l12l0b_trans_b16示例.png)
42: 
43: - 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据，可处理2个分形（一个分形512B），每次迭代中，源操作数2个连续的16 × 8分形将被合并为1个16 × 16的方块矩阵，基于方块矩阵做转置，转置后分裂为2个16 × 8分形，根据目的操作数分形间隔等参数可以有不同的排布。
44: 如下图示例：

(Showing lines 15-44 of 132. Use offset=45 to continue.)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md",
  "limit": 60,
  "offset": 110
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md</path>
<type>file</type>
<content>
110:   __aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);
111: 
112:   ```
113: 
114: ## 参数说明
115: 
116: | 参数名              | 输入/输出 | 描述                                                                                                                                                                                               |
117: |:-----------------| :--- |:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
118: | dst              | 输出 | 目的L0B Buffer地址。 |
119: | src              | 输入 | 源L1 Buffer地址。 |
120: | index_id | 输入 | 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。 |
121: |repeat | 输入 | 迭代次数。<br> 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16*64*0.5B数据；<br> 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16*32*1B数据；<br>对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16*16*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16*8*4B数据。取值范围：[1, 255]。 |
122: | src_stride | 输入 | 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。 |
123: | dst_gap | 输入 | 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。 |
124: | dst_frac_gap | 输入 | 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。 |
125: | src_frac_gap | 输入 | 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。 |
126: 
127: ## 返回值说明
128: 
129: 无
130: 
131: ## 流水类型
132: 
133: PIPE_MTE1
134: 
135: ## 约束说明
136: 
137: - repeat=0表示不执行搬运操作。
138: - 开发者需要保证目的操作数转置后的分形没有重叠。
139: - 操作数地址重叠约束请参考[通用地址重叠约束](../../general_instruction.md#通用地址重叠约束)。
140: 
141: ## 调用示例
142: 
143: ```cpp
144: //设置源操作数和目的操作数，total_length 指参与计算的数据长度
145: constexpr uint64_t total_length = 512;
146: __cbuf__ int32_t src[total_length];
147: __cb__ int32_t dst[total_length];
148: 
149: // 设置搬运过程中的配置
150: uint8_t n = 64;
151: uint8_t nBlockSize = 32;
152: uint8_t repeat = n / nBlockSize;
153: uint16_t index_id = 0;
154: uint16_t src_stride = 1;
155: uint16_t dst_gap = 1;
156: uint16_t dst_frac_gap = 15;
157: uint16_t src_frac_gap = 8;
158: 
159: asc_copy_l12l0b_trans(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
160: ```

(End of file - total 160 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Pro · 18.1s)

_Thinking:_

Now I have all the data I need to compile the comprehensive report. Let me also check one more thing - the `asc_copy_l12l0a_trans.md` doc example lines (22-23) where it uses `dst_gap` and `dst_frac_gap` in the text but the function signature uses `dst_stride` and `dst_frac_stride`. This is another manifestation of the naming inconsistency within the same doc.

Let me now also verify the exact line in `asc_copy_l12l0a_trans.md` where the parameter table defines `dst_stride` with GAP semantics.

I read it at line 95 of that file:
- `dst_stride`: "前一个迭代第一个分形的**结束地址**到下一个迭代第一个分形起始地址的间隔" — this is GAP semantics
- `dst_frac_stride`: "每个迭代内目的操作数转置前一个分形**结束地址**与后一个分形起始地址的间隔" — also GAP semantics

And at line 22 in the same doc, the examples use `dst_gap` instead of `dst_stride`, and `dst_frac_gap` instead of `dst_frac_stride`.

OK, now let me also check for the `asc_copy_l12l0a_trans.md` example text that uses `dst_gap` to confirm it actually uses `dst_gap` for examples but `dst_stride` as the parameter name.

Let me compile the final report now. I have a comprehensive picture. Let me also do a few more quick checks for completeness.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Compare parameter names between docs and implementation for stride-related APIs",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Identify semantic misuse of gap vs stride across all files",
      "status": "completed"
    },
    {
      "priority": "medium",
      "content": "Check general parameter name inconsistencies beyond gap/stride",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Generate final markdown report",
      "status": "in_progress"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Compare parameter names between docs and implementation for stride-related APIs",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Identify semantic misuse of gap vs stride across all files",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Check general parameter name inconsistencies beyond gap/stride",
    "status": "completed",
    "priority": "medium"
  },
  {
    "content": "Generate final markdown report",
    "status": "in_progress",
    "priority": "high"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 147.6s)

_Thinking:_

Now let me compile the complete report. I have all the data needed. Let me write the markdown file.

Now let me compile the final report.

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/C_API_参数名一致性检查报告.md",
  "content": "# C API 参数名一致性检查报告\n\n## 检查范围\n\n- **文档目录**: `docs/api/c_api/`\n- **对外接口声明目录**: `include/c_api/`\n- **实现目录**: `impl/c_api/instr_impl/npu_arch_2201/`、`impl/c_api/instr_impl/npu_arch_3510/`\n\n---\n\n## 关键术语定义\n\n| 术语 | 定义 |\n|:---|:---|\n| **gap** | 上一个数据块的**结束地址**与下一个数据块的**起始地址**之间的距离（即：尾到头的间隔，不含数据块自身大小） |\n| **stride** | 上一个数据块的**起始地址**与下一个数据块的**起始地址**之间的距离（即：头到头的间隔，包含数据块自身大小） |\n\n---\n\n## 一、gap/stride 参数名语义误用\n\n### 问题 1: `asc_copy_l12l0a_trans` — `dst_stride` / `dst_frac_stride` 名为 stride 实际为 gap 语义\n\n- **文档**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md`\n  - **第 95 行**: 参数名 `dst_stride`，描述为\"目的操作数前一个迭代第一个分形的**结束地址**到下一个迭代第一个分形起始地址的间隔\" → **这是 GAP 定义**\n  - **第 97 行**: 参数名 `dst_frac_stride`，描述为\"每个迭代内目的操作数转置前一个分形**结束地址**与后一个分形起始地址的间隔\" → **这是 GAP 定义**\n  - **第 22-23 行**: 示例讲解中使用 `dst_gap`、`dst_frac_gap` 但参数名声明为 `dst_stride`、`dst_frac_stride`\n- **Header**: `include/c_api/cube_datamove/cube_datamove.h:631` — 参数名为 `dst_stride`、`dst_frac_stride`\n- **对比**: 在 `asc_copy_l12l0b_trans_arch_2201.md:93` 中，完全相同的描述对应参数名 `dst_gap`（命名正确）\n- **判定**: **参数应命名为 `dst_gap` / `dst_frac_gap` 或描述应改为 stride 定义**\n\n### 问题 2: `asc_copy_l12fb_v2` — `src_stride` / `dst_stride` 名为 stride 实际为 gap 语义\n\n- **文档**: `docs/api/c_api/cube_datamove/asc_copy_l12fb_v2.md`\n  - **第 41 行**: 参数名 `src_stride`，描述为\"前面一个数据块的**尾**与后面一个数据块的头**的间隔**\" → **这是 GAP 定义**\n  - **第 42 行**: 参数名 `dst_stride`，描述为\"前面一个数据块的**尾**与后面一个数据块的头**的间隔**\" → **这是 GAP 定义**\n- **Header**: `include/c_api/cube_datamove/cube_datamove.h:1888` — 参数名为 `src_stride`、`dst_stride`\n- **对比**: 旧版 `asc_copy_l12fb` 使用 `src_gap_size` / `dst_gap_size` 且描述一致（命名与语义一致）\n- **判定**: **参数应命名为 `src_gap` / `dst_gap`，或描述应改为 stride 定义。`_v2` 版本将 `gap` 重命名为 `stride` 但描述未更新，可能是命名错误**\n\n---\n\n## 二、gap/stride 参数名跨文件不一致\n\n### 问题 3: `asc_copy_l12l0b_trans` (3510) — Header 用 `dst_frac_stride` / `src_frac_stride`，Doc 与 Impl 用 `dst_frac_gap` / `src_frac_gap`\n\n| 位置 | 参数名 |\n|:---|:---|\n| **Header** `include/c_api/cube_datamove/cube_datamove.h:1316-1317` | `dst_frac_stride`, `src_frac_stride` |\n| **Doc** `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md:124-125` | `dst_frac_gap`, `src_frac_gap` |\n| **Impl dispatch** `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h:938` | `dst_frac_gap`, `src_frac_gap` |\n| **Impl body** `impl/.../npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_trans_impl.h:29` | `dst_frac_gap`, `src_frac_gap` |\n\n- **描述语义**: \"前一个分形**结束地址**与后一个分形起始地址的间隔\" → GAP 语义\n- **判定**: **Header 中的 `dst_frac_stride` / `src_frac_stride` 应改为 `dst_frac_gap` / `src_frac_gap`，与 Doc 和 Impl 保持一致**\n\n### 问题 4: `asc_copy_l12l0a` / `asc_copy_l12l0b` (2201, 2D) — 同一函数签名中混用 `src_stride` 与 `dst_gap`\n\n- **Header**: `include/c_api/cube_datamove/cube_datamove.h:397`\n  ```c\n  uint16_t src_stride, uint16_t dst_gap\n  ```\n- **Doc** (l12l0a): `...asc_copy_l12l0a_arch_2201.md:76-77`\n  - `src_stride`: \"相邻迭代间，源操作数前一个分形与后一个分形**起始地址**的间隔\" → STRIDE 语义 ✓\n  - `dst_gap`: \"目的操作数相邻连续数据块的间隔（前面一个数据块的**尾**与后面一个数据块的头**的间隔**）\" → GAP 语义 ✓\n- **Doc** (l12l0b): `...asc_copy_l12l0b_arch_2201.md:70-71` — 描述相同\n- **判定**: **问题在于 src 侧使用 `stride`（头到头），dst 侧使用 `gap`（尾到头）。如果两者语义相同（都是相邻数据块间距离），则命名应统一为 `stride` 或 `gap`**。需要硬件确认实际语义。\n\n### 问题 5: `asc_copy_gm2l0a` (2201) — `src_Stride` 含大写字母 S\n\n- **Header**: `include/c_api/cube_datamove/cube_datamove.h:794-837`\n  - 非 sync 版本: `uint16_t src_Stride` （大写 S）\n  - sync 版本: bfloat16_t 为 `uint16_t src_stride`（小写 s），其余类型为 `uint16_t src_Stride`（大写 S）\n- **整个代码库中其他所有地方均使用 `src_stride`（全小写）**\n- **判定**: **笔误，应为 `src_stride`（全小写）**\n\n### 问题 6: `asc_copy_l12l0a_trans` (2201) 内部 — Dispatch 层用 `dst_frac_gap`，Header/Doc/Impl body 用 `dst_frac_stride`\n\n| 位置 | 参数名 |\n|:---|:---|\n| **Header** `include/c_api/cube_datamove/cube_datamove.h:631` | `dst_frac_stride` |\n| **Doc** `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md:65` | `dst_frac_stride` |\n| **Impl dispatch** `impl/.../npu_arch_2201/cube_datamove_impl.h:1081-1083` | `dst_frac_gap` |\n| **Impl body** `impl/.../npu_arch_2201/cube_datamove_impl/asc_copy_l12l0a_trans_impl.h:28` | `dst_frac_stride` |\n\n- **判定**: **Dispatch 层的 `dst_frac_gap` 应改为 `dst_frac_stride` 以与接口一致（或整体改为 gap）**\n\n### 问题 7: `asc_copy_l12bt` — 使用 `source_gap` 而非 `src_gap`\n\n- **Header**: `include/c_api/cube_datamove/cube_datamove.h:45` — `source_gap`\n- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_2201.md:40` — `source_gap`\n- **对比**: 所有其他 API 使用缩写 `src_gap`\n- **判定**: **命名风格不一致，应统一为 `src_gap`**\n\n### 问题 8: `asc_copy_l12fb` — 使用 `src_gap_size` / `dst_gap_size` (含 `_size` 后缀)\n\n- **Header**: `include/c_api/cube_datamove/cube_datamove.h:36` — `src_gap_size`, `dst_gap_size`\n- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_2201.md:40-41` — `src_gap_size`, `dst_gap_size`\n- **对比**: `asc_copy_l12fb_v2` 使用 `src_stride` / `dst_stride`（无后缀）\n- **判定**: **`_size` 后缀与其他 API 命名风格不一致，应统一为 `src_gap` / `dst_gap`**\n\n---\n\n## 三、参数顺序不一致\n\n### 问题 9: `asc_set_ndim_loop*_stride` 与 `asc_set_*_loop*_stride` 参数顺序相反\n\n| API | 参数顺序 | 文件 |\n|:---|:---|:---|\n| `asc_set_ub2gm_loop1_stride` | `loop1_src_stride` **先**, `loop1_dst_stride` **后** | `vector_datamove.h:299-301` |\n| `asc_set_ub2gm_loop2_stride` | `loop2_src_stride` **先**, `loop2_dst_stride` **后** | `vector_datamove.h:305-307` |\n| `asc_set_gm2ub_loop1_stride` | `loop1_src_stride` **先**, `loop1_dst_stride` **后** | `vector_datamove.h:299-301` |\n| `asc_set_gm2ub_loop2_stride` | `loop2_src_stride` **先**, `loop2_dst_stride` **后** | `vector_datamove.h:305-307` |\n| **`asc_set_ndim_loop0_stride`** | **`dst_stride` 先, `src_stride` 后** | `vector_datamove.h:310-318` |\n| **`asc_set_ndim_loop1_stride`** | **`dst_stride` 先, `src_stride` 后** | `vector_datamove.h:310-318` |\n| **... (loop2-4 同理)** | **`dst_stride` 先, `src_stride` 后** | `vector_datamove.h:310-318` |\n\n- **判定**: **`ndim` 系列接口的 dst/src 参数顺序与其他 loop stride 接口相反，容易引起调用错误**\n\n### 问题 10: `asc_copy_ub2gm_align` (3510) — gap 顺序与 2201 相反\n\n| 架构 | gap 参数顺序 | 文件 |\n|:---|:---|:---|\n| **2201** | `src_gap` **先**, `dst_gap` **后** | `vector_datamove.h:138` |\n| **3510** | `dst_gap` **先**, `src_gap` **后** | `vector_datamove.h:262` |\n\n- 所有其他 API 均使用 src 在前、dst 在后的顺序\n- **判定**: **3510 版本应改为 `src_gap` 在前，与 2201 和所有其他 API 保持一致**\n\n---\n\n## 四、参数类型不一致\n\n### 问题 11: `asc_copy_l12l0b_trans` (2201, half 重载) — Doc 与 Header 类型不同\n\n| 位置 | `dst_frac_gap` 类型 |\n|:---|:---|\n| **Doc** `...asc_copy_l12l0b_trans_arch_2201.md:66` | `uint64_t` |\n| **Header** `cube_datamove.h:674-675` | `uint16_t` |\n| **其他类型重载** | 均为 `uint64_t` |\n\n- **判定**: **half 重载的 `dst_frac_gap` 在 header 中为 `uint16_t`，与 doc 和其他重载的 `uint64_t` 不一致**\n\n---\n\n## 五、Doc 与 Header 函数声明不一致\n\n### 问题 12: `asc_fill_l0a` / `asc_fill_l0b` — Doc 中有 `half`/`uint32_t` value 重载但 Header 中缺失\n\n| 函数 | Doc 中声明的重载 | Header 中是否存在 | 文件 |\n|:---|:---|:---|:---|\n| `asc_fill_l0a` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:100` |\n| `asc_fill_l0a` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:100` |\n| `asc_fill_l0a_sync` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:102` |\n| `asc_fill_l0a_sync` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:102` |\n| `asc_fill_l0b` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:153` |\n| `asc_fill_l0b` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:153` |\n| `asc_fill_l0b_sync` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:155` |\n| `asc_fill_l0b_sync` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:155` |\n\n- **判定**: **需确认是 Doc 多写了重载，还是 Header 遗漏了重载声明**\n\n---\n\n## 六、同一功能 Doc 内部命名不一致\n\n### 问题 13: `asc_copy_l12l0a_trans.md` — 示例用 `dst_gap` 但参数表用 `dst_stride`\n\n- **文档**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md`\n  - **第 22 行**: 示例讲解使用 `dst_gap = 1`\n  - **第 23 行**: 示例讲解使用 `dst_frac_gap = 0`\n  - **第 95 行**: 参数表声明为 `dst_stride`\n  - **第 97 行**: 参数表声明为 `dst_frac_stride`\n- **判定**: **同一文档内示例和参数表使用不同的命名，应统一**\n\n---\n\n## 总结\n\n| 类别 | 问题数 | 严重程度 |\n|:---|:---:|:---|\n| gap/stride 语义误用 | 3 | **高** — 影响用户理解 |\n| gap/stride 跨文件不一致 | 5 | **高** — 接口声明与实现不同名 |\n| 参数顺序不一致 | 2 | **中** — 易导致调用错误 |\n| 参数类型不一致 | 1 | **中** — 类型不匹配 |\n| Doc/Header 声明不一致 | 1（8 个重载） | **中** — 文档与接口不同步 |\n| 命名风格不一致 | 2 | **低** — 风格偏差 |\n| 文档内部不一致 | 1 | **中** — 示例与参数表不同名 |\n\n### 建议优先修复\n\n1. **问题 3**: Header 中 `dst_frac_stride` / `src_frac_stride` → 改为 `dst_frac_gap` / `src_frac_gap`（与 impl 和 doc 对齐）\n2. **问题 1**: `asc_copy_l12l0a_trans` 的 `dst_stride` / `dst_frac_stride` 参数，如确实描述 gap 语义，应更名为 `dst_gap` / `dst_frac_gap`\n3. **问题 2**: `asc_copy_l12fb_v2` 的 `src_stride` / `dst_stride` 参数，如确实描述 gap 语义，应更名为 `src_gap` / `dst_gap`\n4. **问题 5**: `asc_copy_gm2l0a` 中 `src_Stride` 改为 `src_stride`\n5. **问题 9/10**: 统一参数顺序\n"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 7.7s)

_Thinking:_

The report has been written. Let me update the todos and summarize for the user.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Compare parameter names between docs and implementation for stride-related APIs",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Identify semantic misuse of gap vs stride across all files",
      "status": "completed"
    },
    {
      "priority": "medium",
      "content": "Check general parameter name inconsistencies beyond gap/stride",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Generate final markdown report",
      "status": "completed"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Search for gap/stride usage across all three source trees (docs, impl, include)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Compare parameter names between docs and implementation for stride-related APIs",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Identify semantic misuse of gap vs stride across all files",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Check general parameter name inconsistencies beyond gap/stride",
    "status": "completed",
    "priority": "medium"
  },
  {
    "content": "Generate final markdown report",
    "status": "completed",
    "priority": "high"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 14.2s)

_Thinking:_

The report has been generated and saved. Let me provide a brief summary to the user.

报告已生成至 `C_API_参数名一致性检查报告.md`，共发现 **13 个问题**，分布在 6 个类别中。重点发现：

**gap/stride 语义误用（高优先级）：**
- `asc_copy_l12l0a_trans` — 参数命名为 `dst_stride` 但描述的是 gap 语义（结束地址到起始地址）
- `asc_copy_l12fb_v2` — 参数命名为 `src_stride` / `dst_stride` 但描述为 gap（尾到头的间隔），旧版 `asc_copy_l12fb` 使用 `gap_size` 是正确的

**跨文件不一致（高优先级）：**
- `asc_copy_l12l0b_trans` (3510): Header 用 `dst_frac_stride`，但 Doc 和 Impl 用 `dst_frac_gap`
- `asc_copy_l12l0a` / `asc_copy_l12l0b` (2201): 同一签名混用 `src_stride` + `dst_gap`
- `asc_copy_gm2l0a`: `src_Stride` 含大写 S 笔误

**参数顺序不一致：**
- `asc_set_ndim_loop*_stride` 是 `dst, src` 顺序，但其他 loop stride 接口是 `src, dst` 顺序
- `asc_copy_ub2gm_align` 3510 版是 `dst_gap, src_gap` 顺序，与 2201 版和其他所有 API 相反

---

