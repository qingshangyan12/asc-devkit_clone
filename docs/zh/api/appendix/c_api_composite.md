# C API指令组合

本节汇总由多条硬件指令组合实现的Composite软仿真接口。接口资料仍归属对应业务章节，本页仅提供附录索引。

|业务接口资料|Composite接口|最细粒度公共头文件|
|---|---|---|
|[asc_abs_sync（废弃）](../SIMD-API/c_api/vector_compute/vector_arith/asc_abs_sync_deprecated.md)|`asc_abs_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_add](../SIMD-API/c_api/vector_compute/vector_arith/asc_add.md)|`asc_add_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_add_relu](../SIMD-API/c_api/vector_compute/vector_fused/asc_add_relu.md)|`asc_add_relu_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_add_scalar](../SIMD-API/c_api/vector_compute/vector_arith/asc_add_scalar.md)|`asc_add_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_and](../SIMD-API/c_api/vector_compute/vector_logic/asc_and.md)|`asc_and_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_axpy](../SIMD-API/c_api/vector_compute/vector_fused/asc_axpy.md)|`asc_axpy_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_bfloat162float](../SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162float.md)|`asc_bfloat162float_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_bfloat162int32](../SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162int32.md)|`asc_bfloat162int32_rd_sync`, `asc_bfloat162int32_rn_sync`, `asc_bfloat162int32_rna_sync`, `asc_bfloat162int32_ru_sync`, `asc_bfloat162int32_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_bitsort](../SIMD-API/c_api/vector_compute/vector_sort/asc_bitsort.md)|`asc_bitsort_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_brcb](../SIMD-API/c_api/vector_compute/vector_broadcast/asc_brcb.md)|`asc_brcb_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy](../SIMD-API/c_api/vector_datamove/asc_copy.md)|`asc_copy_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_gm2l0a](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l0a.md)|`asc_copy_gm2l0a_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l0b](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l0b.md)|`asc_copy_gm2l0b_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l1](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md)|`asc_copy_gm2l1_pad1_sync`, `asc_copy_gm2l1_pad2_sync`, `asc_copy_gm2l1_pad3_sync`, `asc_copy_gm2l1_pad4_sync`, `asc_copy_gm2l1_pad5_sync`, `asc_copy_gm2l1_pad6_sync`, `asc_copy_gm2l1_pad7_sync`, `asc_copy_gm2l1_pad8_sync`, `asc_copy_gm2l1_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l1](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)|`asc_copy_gm2l1_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l1_align](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_align.md)|`asc_copy_gm2l1_align_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l1_dn2nz](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_dn2nz.md)|`asc_copy_gm2l1_dn2nz_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l1_nd2nz](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_2201.md)|`asc_copy_gm2l1_nd2nz_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2l1_nd2nz](../SIMD-API/c_api/cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md)|`asc_copy_gm2l1_nd2nz_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_gm2ub](../SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_2201.md)|`asc_copy_gm2ub_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_gm2ub](../SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md)|`asc_copy_gm2ub_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_gm2ub_align](../SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_2201.md)|`asc_copy_gm2ub_align_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_l0c2gm](../SIMD-API/c_api/cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_2201.md)|`asc_copy_l0c2gm_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l0c2gm](../SIMD-API/c_api/cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)|`asc_copy_l0c2gm_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l0c2l1](../SIMD-API/c_api/cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_2201.md)|`asc_copy_l0c2l1_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l0c2l1](../SIMD-API/c_api/cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md)|`asc_copy_l0c2l1_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l0c2ub](../SIMD-API/c_api/cube_datamove/asc_copy_l0c2ub.md)|`asc_copy_l0c2ub_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12bt](../SIMD-API/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_2201.md)|`asc_copy_l12bt_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12bt](../SIMD-API/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md)|`asc_copy_l12bt_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12fb](../SIMD-API/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_2201.md)|`asc_copy_l12fb_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12fb](../SIMD-API/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_3510.md)|`asc_copy_l12fb_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12gm](../SIMD-API/c_api/cube_datamove/asc_copy_l12gm.md)|`asc_copy_l12gm_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0a](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md)|`asc_copy_l12l0a_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0a（2D）](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_2d_arch_3510.md)|`asc_copy_l12l0a_sync`, `asc_copy_l12l0a_transpose_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0a（3D）](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_3d_arch_3510.md)|`asc_copy_l12l0a_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0a_mx](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0a_mx.md)|`asc_copy_l12l0a_mx_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0a_trans](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0a_trans.md)|`asc_copy_l12l0a_trans_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_2201.md)|`asc_copy_l12l0b_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b（2D）](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_2d_arch_3510.md)|`asc_copy_l12l0b_sync`, `asc_copy_l12l0b_transpose_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b（3D）](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_3d_arch_3510.md)|`asc_copy_l12l0b_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b_mx](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b_mx.md)|`asc_copy_l12l0b_mx_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b_sparse](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b_sparse.md)|`asc_copy_l12l0b_sparse_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b_trans](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md)|`asc_copy_l12l0b_trans_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0b_trans](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md)|`asc_copy_l12l0b_trans_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12l0c](../SIMD-API/c_api/cube_datamove/asc_copy_l12l0c.md)|`asc_copy_l12l0c_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_l12ub](../SIMD-API/c_api/cube_datamove/asc_copy_l12ub.md)|`asc_copy_l12ub_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_copy_ub2gm](../SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_2201.md)|`asc_copy_ub2gm_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_ub2gm](../SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md)|`asc_copy_ub2gm_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_ub2gm_align](../SIMD-API/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_2201.md)|`asc_copy_ub2gm_align_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_copy_ub2ub](../SIMD-API/c_api/vector_datamove/asc_copy_ub2ub.md)|`asc_copy_ub2ub_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_datablock_reduce_max](../SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md)|`asc_datablock_reduce_max_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_datablock_reduce_min](../SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md)|`asc_datablock_reduce_min_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_datablock_reduce_sum](../SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md)|`asc_datablock_reduce_sum_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_deq_int162b8](../SIMD-API/c_api/vector_compute/vector_fused/asc_deq_int162b8.md)|`asc_deq_int162b8_h_sync`, `asc_deq_int162b8_l_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_deq_int322half](../SIMD-API/c_api/vector_compute/vector_fused/asc_deq_int322half.md)|`asc_deq_int322half_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_div](../SIMD-API/c_api/vector_compute/vector_arith/asc_div.md)|`asc_div_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_duplicate](../SIMD-API/c_api/vector_compute/vector_broadcast/asc_duplicate.md)|`asc_duplicate_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_eq](../SIMD-API/c_api/vector_compute/vector_compare/asc_eq.md)|`asc_eq_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_eq_scalar](../SIMD-API/c_api/vector_compute/vector_compare/asc_eq_scalar.md)|`asc_eq_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_exp](../SIMD-API/c_api/vector_compute/vector_arith/asc_exp.md)|`asc_exp_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_fill_l0a](../SIMD-API/c_api/cube_datamove/asc_fill_l0a.md)|`asc_fill_l0a_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_fill_l0b](../SIMD-API/c_api/cube_datamove/asc_fill_l0b.md)|`asc_fill_l0b_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_fill_l1](../SIMD-API/c_api/cube_datamove/asc_fill_l1.md)|`asc_fill_l1_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_float2bfloat16](../SIMD-API/c_api/vector_compute/vector_convert/asc_float2bfloat16.md)|`asc_float2bfloat16_rd_sync`, `asc_float2bfloat16_rn_sync`, `asc_float2bfloat16_rna_sync`, `asc_float2bfloat16_ru_sync`, `asc_float2bfloat16_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_float2float](../SIMD-API/c_api/vector_compute/vector_convert/asc_float2float.md)|`asc_float2float_rd_sync`, `asc_float2float_rn_sync`, `asc_float2float_rna_sync`, `asc_float2float_ru_sync`, `asc_float2float_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_float2half](../SIMD-API/c_api/vector_compute/vector_convert/asc_float2half.md)|`asc_float2half_rd_sync`, `asc_float2half_rn_sync`, `asc_float2half_rna_sync`, `asc_float2half_ro_sync`, `asc_float2half_ru_sync`, `asc_float2half_rz_sync`, `asc_float2half_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_float2int16](../SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md)|`asc_float2int16_rd_sync`, `asc_float2int16_rn_sync`, `asc_float2int16_rna_sync`, `asc_float2int16_ru_sync`, `asc_float2int16_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_float2int32](../SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md)|`asc_float2int32_rd_sync`, `asc_float2int32_rn_sync`, `asc_float2int32_rna_sync`, `asc_float2int32_ru_sync`, `asc_float2int32_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_float2int64](../SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md)|`asc_float2int64_rd_sync`, `asc_float2int64_rn_sync`, `asc_float2int64_rna_sync`, `asc_float2int64_ru_sync`, `asc_float2int64_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_fma](../SIMD-API/c_api/vector_compute/vector_fused/asc_fma.md)|`asc_fma_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_gather](../SIMD-API/c_api/vector_compute/vector_gather/asc_gather.md)|`asc_gather_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_gather_datablock](../SIMD-API/c_api/vector_compute/vector_gather/asc_gather_datablock.md)|`asc_gather_datablock_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_ge](../SIMD-API/c_api/vector_compute/vector_compare/asc_ge.md)|`asc_ge_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_ge_scalar](../SIMD-API/c_api/vector_compute/vector_compare/asc_ge_scalar.md)|`asc_ge_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_gt](../SIMD-API/c_api/vector_compute/vector_compare/asc_gt.md)|`asc_gt_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_gt_scalar](../SIMD-API/c_api/vector_compute/vector_compare/asc_gt_scalar.md)|`asc_gt_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_half2float](../SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md)|`asc_half2float_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_half2int16](../SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md)|`asc_half2int16_rd_sync`, `asc_half2int16_rn_sync`, `asc_half2int16_rna_sync`, `asc_half2int16_ru_sync`, `asc_half2int16_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_half2int32](../SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md)|`asc_half2int32_rd_sync`, `asc_half2int32_rn_sync`, `asc_half2int32_rna_sync`, `asc_half2int32_ru_sync`, `asc_half2int32_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_half2int4](../SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md)|`asc_half2int4_rd_sync`, `asc_half2int4_rn_sync`, `asc_half2int4_rna_sync`, `asc_half2int4_ru_sync`, `asc_half2int4_rz_sync`, `asc_half2int4_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_half2int8](../SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md)|`asc_half2int8_rd_sync`, `asc_half2int8_rn_sync`, `asc_half2int8_rna_sync`, `asc_half2int8_ru_sync`, `asc_half2int8_rz_sync`, `asc_half2int8_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_half2uint8](../SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md)|`asc_half2uint8_rd_sync`, `asc_half2uint8_rn_sync`, `asc_half2uint8_rna_sync`, `asc_half2uint8_ru_sync`, `asc_half2uint8_rz_sync`, `asc_half2uint8_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int162float](../SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md)|`asc_int162float_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int162half](../SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md)|`asc_int162half_rd_sync`, `asc_int162half_rn_sync`, `asc_int162half_rna_sync`, `asc_int162half_ru_sync`, `asc_int162half_rz_sync`, `asc_int162half_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int322float](../SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md)|`asc_int322float_rd_sync`, `asc_int322float_rn_sync`, `asc_int322float_rna_sync`, `asc_int322float_ru_sync`, `asc_int322float_rz_sync`, `asc_int322float_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int322int16](../SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md)|`asc_int322int16_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int322int64](../SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md)|`asc_int322int64_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int42half](../SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md)|`asc_int42half_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int642float](../SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md)|`asc_int642float_rd_sync`, `asc_int642float_rn_sync`, `asc_int642float_rna_sync`, `asc_int642float_ru_sync`, `asc_int642float_rz_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int642int32](../SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md)|`asc_int642int32_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_int82half](../SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md)|`asc_int82half_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_le](../SIMD-API/c_api/vector_compute/vector_compare/asc_le.md)|`asc_le_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_le_scalar](../SIMD-API/c_api/vector_compute/vector_compare/asc_le_scalar.md)|`asc_le_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_leakyrelu](../SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md)|`asc_leakyrelu_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_load](../SIMD-API/c_api/reg_compute/load/asc_load.md)|`asc_load`|`c_api/composite/loadunalign_composite.h`|
|[asc_load_image_to_cbuf](../SIMD-API/c_api/cube_datamove/asc_load_image_to_cbuf.md)|`asc_load_image_to_cbuf_sync`|`c_api/composite/cube_datamove_composite.h`|
|[asc_log](../SIMD-API/c_api/vector_compute/vector_arith/asc_log.md)|`asc_log_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_lt](../SIMD-API/c_api/vector_compute/vector_compare/asc_lt.md)|`asc_lt_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_lt_scalar](../SIMD-API/c_api/vector_compute/vector_compare/asc_lt_scalar.md)|`asc_lt_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_max](../SIMD-API/c_api/vector_compute/vector_arith/asc_max.md)|`asc_max_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_max_scalar](../SIMD-API/c_api/vector_compute/vector_arith/asc_max_scalar.md)|`asc_max_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_min](../SIMD-API/c_api/vector_compute/vector_arith/asc_min.md)|`asc_min_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_min_scalar](../SIMD-API/c_api/vector_compute/vector_arith/asc_min_scalar.md)|`asc_min_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mmad_sparse](../SIMD-API/c_api/cube_compute/asc_mmad_sparse.md)|`asc_mmad_sparse_sync`|`c_api/composite/cube_compute_composite.h`|
|[asc_mrgsort4](../SIMD-API/c_api/vector_compute/vector_sort/asc_mrgsort4.md)|`asc_mrgsort4_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mul](../SIMD-API/c_api/vector_compute/vector_arith/asc_mul.md)|`asc_mul_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mul_add](../SIMD-API/c_api/vector_compute/vector_fused/asc_mul_add.md)|`asc_mul_add_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mul_add_relu](../SIMD-API/c_api/vector_compute/vector_fused/asc_mul_add_relu.md)|`asc_mul_add_relu_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mul_cast_half2int8](../SIMD-API/c_api/vector_compute/vector_fused/asc_mul_cast_half2int8.md)|`asc_mul_cast_half2int8_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mul_cast_half2uint8](../SIMD-API/c_api/vector_compute/vector_fused/asc_mul_cast_half2uint8.md)|`asc_mul_cast_half2uint8_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_mul_scalar](../SIMD-API/c_api/vector_compute/vector_arith/asc_mul_scalar.md)|`asc_mul_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_ne](../SIMD-API/c_api/vector_compute/vector_compare/asc_ne.md)|`asc_ne_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_ne_scalar](../SIMD-API/c_api/vector_compute/vector_compare/asc_ne_scalar.md)|`asc_ne_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_not](../SIMD-API/c_api/vector_compute/vector_logic/asc_not.md)|`asc_not_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_or](../SIMD-API/c_api/vector_compute/vector_logic/asc_or.md)|`asc_or_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_pair_reduce_sum](../SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md)|`asc_pair_reduce_sum_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_rcp](../SIMD-API/c_api/vector_compute/vector_arith/asc_rcp.md)|`asc_rcp_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_reduce](../SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md)|`asc_reduce_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_relu](../SIMD-API/c_api/vector_compute/vector_fused/asc_relu.md)|`asc_relu_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_repeat_reduce_max](../SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md)|`asc_repeat_reduce_max_index_value_sync`, `asc_repeat_reduce_max_only_index_sync`, `asc_repeat_reduce_max_only_value_sync`, `asc_repeat_reduce_max_value_index_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_repeat_reduce_min](../SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md)|`asc_repeat_reduce_min_index_value_sync`, `asc_repeat_reduce_min_only_index_sync`, `asc_repeat_reduce_min_only_value_sync`, `asc_repeat_reduce_min_value_index_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_repeat_reduce_sum](../SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md)|`asc_repeat_reduce_sum_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_rsqrt](../SIMD-API/c_api/vector_compute/vector_arith/asc_rsqrt.md)|`asc_rsqrt_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_select](../SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md)|`asc_select_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_shiftleft](../SIMD-API/c_api/vector_compute/vector_logic/asc_shiftleft.md)|`asc_shiftleft_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_shiftright](../SIMD-API/c_api/vector_compute/vector_logic/asc_shiftright.md)|`asc_shiftright_round_sync`, `asc_shiftright_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_sqrt](../SIMD-API/c_api/vector_compute/vector_arith/asc_sqrt.md)|`asc_sqrt_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_store](../SIMD-API/c_api/reg_compute/store/asc_store.md)|`asc_store`|`c_api/composite/storeunalign_composite.h`|
|[asc_sub](../SIMD-API/c_api/vector_compute/vector_arith/asc_sub.md)|`asc_sub_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_sub_relu](../SIMD-API/c_api/vector_compute/vector_fused/asc_sub_relu.md)|`asc_sub_relu_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_sub_scalar](../SIMD-API/c_api/vector_compute/vector_arith/asc_sub_scalar.md)|`asc_sub_scalar_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_transpose](../SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md)|`asc_transpose_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_uint82half](../SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md)|`asc_uint82half_sync`|`c_api/composite/vector_compute_composite.h`|
|[asc_vdeq_int162b8](../SIMD-API/c_api/vector_compute/vector_fused/asc_vdeq_int162b8.md)|`asc_vdeq_int162b8_h_sync`, `asc_vdeq_int162b8_l_sync`|`c_api/composite/vector_compute_composite.h`|
