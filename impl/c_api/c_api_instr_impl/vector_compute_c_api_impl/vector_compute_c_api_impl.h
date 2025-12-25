/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_H

#include "get_acc_val_impl/npu_arch_2201/asc_get_acc_val_impl.h"
#include "get_cmp_mask_impl/asc_2201/get_cmp_mask_impl.h"
#include "get_max_min_cnt_impl/npu_arch_2201/asc_get_reduce_max_min_cnt_impl.h"
#include "get_rsvd_cnt_impl/asc_2201/get_rsvd_cnt_impl.h"
#include "get_vms4_sr_impl/npu_arch_2201/asc_get_vms4_sr_impl.h"
#include "set_cmp_mask_impl/asc_2201/set_cmp_mask_impl.h"
#include "set_mask_count_impl/npu_arch_2201/asc_set_mask_count_impl.h"
#include "set_mask_norm_impl/npu_arch_2201/asc_set_mask_norm_impl.h"
#include "set_vector_mask_impl/npu_arch_2201/asc_set_vector_mask_impl.h"
#include "clear_nthbit_impl/npu_arch_2201/asc_clear_nthbit_impl.h"
#include "vabs_impl/npu_arch_2201/asc_vabs_impl.h"
#include "vadd_impl/npu_arch_2201/asc_add_impl.h"
#include "vadds_impl/npu_arch_2201/asc_add_scalar_impl.h"
#include "set_deqscale_impl/npu_arch_2201/asc_set_deq_scale_impl.h"
#include "vaddrelu_impl/asc_2201/add_relu_impl.h"
#include "vbitsort_impl/npu_arch_2201/bitsort_impl.h"
#include "vbrcb_impl/npu_arch_2201/asc_brcb_impl.h"
#include "vcgadd_impl/npu_arch_2201/asc_datablock_reduce_sum_impl.h"
#include "vcgmax_impl/npu_arch_2201/asc_datablock_reduce_max_impl.h"
#include "vcgmin_impl/npu_arch_2201/asc_datablock_reduce_min_impl.h"
#include "vcadd_impl/npu_arch_2201/asc_repeat_reduce_sum_impl.h"
#include "vcmax_impl/npu_arch_2201/asc_repeat_reduce_max_impl.h"
#include "vcmin_impl/npu_arch_2201/asc_repeat_reduce_min_impl.h"
#include "vcmp_eq_impl/npu_arch_2201/asc_eq_impl.h"
#include "vcmp_ge_impl/asc_2201/ge_impl.h"
#include "vcmp_c_api_impl/vcmp_ne_impl/npu_arch_2201/asc_ne_impl.h"
#include "vcmpvs_c_api_impl/vcmpvs_ge_impl/npu_arch_2201/asc_ge_scalar_impl.h"
#include "vconv_c_api_impl/vconv_bf162float_impl/npu_arch_2201/asc_bf162float_impl.h"
#include "vconv_c_api_impl/vconv_bf162int32_impl/npu_arch_2201/asc_bf162int32_impl.h"
#include "vconv_c_api_impl/vconv_deqint322half_impl/npu_arch_2201/asc_deqint322half_impl.h"
#include "vconv_c_api_impl/vconv_float2float_impl/npu_arch_2201/asc_float2float_impl.h"
#include "vconv_c_api_impl/vconv_float2bf16_impl/npu_arch_2201/asc_float2bf16_impl.h"
#include "vconv_c_api_impl/vconv_float2half_impl/npu_arch_2201/asc_float2half_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_a_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_c_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_f_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_r_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_z_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_a_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_c_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_f_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_r_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_z_impl.h"
#include "vconv_c_api_impl/vconv_half2float_impl/npu_arch_2201/asc_half2float_impl.h"
#include "vconv_c_api_impl/vconv_half2int4_impl/npu_arch_2201/asc_half2int4_impl.h"
#include "vconv_c_api_impl/vconv_half2int16_impl/npu_arch_2201/asc_half2int16_impl.h"
#include "vconv_c_api_impl/vconv_half2int32_impl/npu_arch_2201/asc_half2int32_impl.h"
#include "vconv_c_api_impl/vconv_half2int8_impl/npu_arch_2201/asc_half2int8_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_a_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_c_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_f_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_r_impl.h"
#include "vconv_c_api_impl/vconv_half2uint8_impl/npu_arch_2201/asc_half2uint8_z_impl.h"
#include "vconv_c_api_impl/vconv_vdeqs162b8h_impl/asc_2201/vdeqs162b8h_impl.h"
#include "vconv_c_api_impl/vconv_vdeqs162b8l_impl/asc_2201/vdeqs162b8l_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_a_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_c_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_f_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_r_impl.h"
#include "vconv_c_api_impl/vconv_float2int16_impl/npu_arch_2201/asc_vconv_float2int16_z_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_a_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_c_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_f_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_r_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/npu_arch_2201/asc_int322float_z_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/asc_2201/asc_int642float_a_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/asc_2201/asc_int642float_c_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/asc_2201/asc_int642float_f_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/asc_2201/asc_int642float_r_impl.h"
#include "vconv_c_api_impl/vconv_int642float_impl/asc_2201/asc_int642float_z_impl.h"
#include "vconv_c_api_impl/vconv_int42half_impl/npu_arch_2201/asc_int42half_impl.h"
#include "vconv_c_api_impl/vconv_int82half_impl/npu_arch_2201/asc_int82half_impl.h"
#include "vconv_c_api_impl/vconv_uint82half_impl/npu_arch_2201/asc_uint82half_impl.h"
#include "vconv_c_api_impl/vconv_int162float_impl/npu_arch_2201/asc_int162float_impl.h"
#include "vconv_c_api_impl/vconv_int322int16_impl/npu_arch_2201/asc_int322int16_impl.h"
#include "vconv_c_api_impl/vconv_int322int64_impl/npu_arch_2201/asc_int322int64_impl.h"
#include "vconv_c_api_impl/vconv_int642int32_impl/npu_arch_2201/asc_int642int32_impl.h"
#include "vconv_c_api_impl/vconv_deqint162b8_impl/npu_arch_2201/asc_deq_int162b8_h_impl.h"
#include "vconv_c_api_impl/vconv_deqint162b8_impl/npu_arch_2201/asc_deq_int162b8_l_impl.h"
#include "vaddreluconv/vaddreluconv_f162s8_impl/npu_arch_2201/asc_addreluconv_f162s8_impl.h"
#include "vaddreluconv/vaddreluconv_f322f16_impl/npu_arch_2201/asc_addreluconv_f322f16_impl.h"
#include "vaddreluconv/vaddreluconv_s162s8_impl/npu_arch_2201/asc_addreluconv_s162s8_impl.h"
#include "vdiv_impl/npu_arch_2201/asc_div_impl.h"
#include "vector_dup_impl/npu_arch_2201/asc_duplicate_impl.h"
#include "vexp_impl/npu_arch_2201/asc_exp_impl.h"
#include "vgt_scalar_impl/npu_arch_2201/gt_scalar_impl.h"
#include "vlog_impl/npu_arch_2201/log_impl.h"
#include "vmax_impl/npu_arch_2201/asc_max_impl.h"
#include "vmrgsort4_impl/npu_arch_2201/asc_mrgsort4_impl.h"
#include "vmin_impl/npu_arch_2201/asc_min_impl.h"
#include "vmaxs_impl/npu_arch_2201/asc_max_scalar_impl.h"
#include "vmul_add_dst_impl/npu_arch_2201/mul_add_dst_impl.h"
#include "vmul_impl/npu_arch_2201/asc_mul_impl.h"
#include "vmuls_impl/npu_arch_2201/asc_mul_scalar_impl.h"
#include "vor_impl/npu_arch_2201/or_impl.h"
#include "vmulconv_c_api_impl/mul_cast_half2int8_impl.h/npu_arch_2201/asc_mul_cast_half2int8_impl.h"
#include "vmulconv_c_api_impl/mul_cast_half2uint8_impl.h/npu_arch_2201/asc_mul_cast_half2uint8_impl.h"
#include "vsel_impl/npu_arch_2201/asc_select_impl.h"
#include "vsub_impl/npu_arch_2201/asc_sub_impl.h"
#include "vsubs_impl/npu_arch_2201/asc_sub_scalar_impl.h"
#include "vrelu_impl/npu_arch_2201/asc_relu_impl.h"
#include "vcmp_gt_impl/npu_arch_2201/asc_gt_impl.h"
#include "vrsqrt_impl/asc_2201/rsqrt_impl.h"
#include "vmins_impl/npu_arch_2201/asc_min_scalar_impl.h"
#include "vtranspose_impl/npu_arch_2201/asc_transpose_impl.h"
#include "vrec_impl/npu_arch_2201/asc_rcp_impl.h"
#include "vshl_impl/npu_arch_2201/asc_shift_left_impl.h"
#include "vshr_impl/npu_arch_2201/asc_shift_right_impl.h"
#include "vshr_impl/npu_arch_2201/asc_shift_right_round_impl.h"
#include "vmadd_impl/npu_arch_2201/asc_mul_add_impl.h"
#include "vcmpvs_c_api_impl/vcmpvs_eq_impl/npu_arch_2201/asc_eq_scalar_impl.h"
#include "vnot_impl/npu_arch_2201/asc_not_impl.h"
#include "vmaddrelu_impl/npu_arch_2201/asc_mul_add_relu_impl.h"
#include "vsubrelu_impl/npu_arch_2201/asc_subrelu_impl.h"
#include "vcmp_c_api_impl/vcmp_le_impl/npu_arch_2201/asc_le_impl.h"
#include "vcmpvs_c_api_impl/vcmpvs_le_impl/npu_arch_2201/asc_le_scalar_impl.h"
#include "vsqrt_impl/npu_arch_2201/asc_sqrt_impl.h"
#include "vaxpy_impl/npu_arch_2201/asc_axpy_impl.h"
#include "vcmp_c_api_impl/vcmp_lt_impl/npu_arch_2201/asc_lt_impl.h"
#include "vcmpvs_c_api_impl/vcmpvs_ne_impl/npu_arch_2201/asc_ne_scalar_impl.h"
#include "set_nthbit_impl/asc_2201/set_nthbit_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_a_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_c_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_f_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_r_impl.h"
#include "vconv_c_api_impl/vconv_int162half_impl/npu_arch_2201/asc_int162half_z_impl.h"
#include "leakyrelu_scalar_impl/npu_arch_2201/asc_leakyrelu_scalar_impl.h"
#include "lt_scalar_impl/npu_arch_2201/asc_lt_scalar_impl.h"
#include "sub_relu_cast_f162s8_impl/npu_arch_2201/asc_sub_relu_cast_f162s8_impl.h"
#include "sub_relu_cast_f322f16_impl/npu_arch_2201/asc_sub_relu_cast_f322f16_impl.h"
#include "sub_relu_cast_s162s8_impl/npu_arch_2201/asc_sub_relu_cast_s162s8_impl.h"
#include "sub_relu_cast_vdeqs162b8_impl/npu_arch_2201/asc_sub_relu_cast_vdeqs162b8_impl.h"
#include "and_impl/npu_arch_2201/asc_and_impl.h"
#include "vgather_impl/npu_arch_2201/asc_gather_impl.h"
#include "vgatherb_impl/npu_arch_2201/asc_gather_datablock_impl.h"
#include "set_va_reg_impl/npu_arch_2201/asc_set_va_reg_impl.h"
#include "transdata_to_5hd_impl/npu_arch_2201/asc_transdata_to_5hd_impl.h"

__aicore__ inline int64_t asc_get_acc_val()
{
    return asc_get_acc_val_impl();
}

__aicore__ inline void asc_get_cmp_mask(__ubuf__ void* dst)
{
    asc_get_cmp_mask_impl(dst);
}

__aicore__ inline void asc_get_reduce_max_min_cnt(half& val, uint32_t& index)
{
    asc_get_reduce_max_min_cnt_impl(val, index);
}

__aicore__ inline void asc_get_reduce_max_min_cnt(float& val, uint32_t& index)
{
    asc_get_reduce_max_min_cnt_impl(val, index);
}

__aicore__ inline int64_t asc_get_rsvd_count()
{
    return CApiInternal::asc_get_rsvd_count_impl();
}

__aicore__ inline void asc_get_vms4_sr(uint16_t sorted_num[4])
{
    asc_get_vms4_sr_impl(sorted_num);
}

__aicore__ inline void asc_set_cmp_mask(__ubuf__ void* sel_mask)
{
    asc_set_cmp_mask_impl(sel_mask);
}

__aicore__ inline void asc_set_mask_count()
{
    asc_set_mask_count_impl();
}

__aicore__ inline void asc_set_mask_norm()
{
    asc_set_mask_norm_impl();
}

__aicore__ inline void asc_set_vector_mask(uint64_t mask1, uint64_t mask0)
{
    asc_set_vector_mask_impl(mask1, mask0);
}

__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx)
{
    return asc_clear_nthbit_impl(bits, idx);
}

__aicore__ inline void asc_set_deq_scale(const asc_deq_scale_config config)
{
    asc_set_deq_scale_impl(config);
}

__aicore__ inline void asc_set_deq_scale(__ubuf__ uint64_t* config)
{
    asc_set_deq_scale_impl(config);
}

// ==========asc_add(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_add_sync_impl(dst, src0, src1, count);
}

// ==========asc_add_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_impl (dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl (dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_sync_impl (dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_bitsort(half/float)==========
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl_half(dst, src0, src1, repeat);
}

__aicore__ inline void asc_bitsort_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_sync_impl_half(dst, src0, src1, repeat);
}

__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl_float(dst, src0, src1, repeat);
}

__aicore__ inline void asc_bitsort_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_sync_impl_float(dst, src0, src1, repeat);
}
// ==========asc_axpy(half/float/float half)==========
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_axpy_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_axpy_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_axpy_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_axpy_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_axpy_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_sync_impl(dst, src, value, count);
}

// ==========asc_brcb(uint16_t/uint32_t)==========
__aicore__ inline void asc_brcb(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_sync_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_brcb_sync_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

// ==========asc_datablock_reduce(half/float)==========
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_datablock_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_sum_sync_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_datablock_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_sum_sync_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_datablock_reduce_max_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_sync_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_datablock_reduce_max_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_sync_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_datablock_reduce_min_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_min_sync_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_datablock_reduce_min_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_min_sync_impl(dst, src, count);
}

// ==========asc_repeat_reduce(half/float)==========
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_sum_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_sum_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_sync_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_sync_impl(dst, src, count);
}

// ==========asc_eq(half/float/uint8_t-half/uint8_t-float/uint8_t-int32_t)==========
__aicore__ inline void asc_eq(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
            dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_bf162float==========
__aicore__ inline void asc_bf162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bf162float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bf162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162float_impl(dst, src, count);
}

__aicore__ inline void asc_bf162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162float_sync_impl(dst, src, count);
}

// ==========asc_bf162int32==========
__aicore__ inline void asc_bf162int32_a(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bf162int32_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bf162int32_a(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_a_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_a_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_c(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bf162int32_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bf162int32_c(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_c_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_c_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_f(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bf162int32_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bf162int32_f(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_f_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_f_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_r(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bf162int32_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bf162int32_r(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_r_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_r_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_z(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bf162int32_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bf162int32_z(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_z_impl(dst, src, count);
}

__aicore__ inline void asc_bf162int32_z_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bf162int32_z_sync_impl(dst, src, count);
}

// ==========asc_float2bf16==========
__aicore__ inline void asc_float2bf16_r(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bf16_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bf16_r(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_r_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_r_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_a(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bf16_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bf16_a(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_a_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_a_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_f(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bf16_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bf16_f(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_f_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_f_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_c(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bf16_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bf16_c(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_c_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_c_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_z(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bf16_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bf16_z(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_z_impl(dst, src, count);
}

__aicore__ inline void asc_float2bf16_z_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bf16_z_sync_impl(dst, src, count);
}

// ==========asc_float2float==========
__aicore__ inline void asc_float2float_r(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_r(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_r_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_r_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_f(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_f(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_f_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_f_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_c(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_c(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_c_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_c_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_a(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_a(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_a_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_a_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_z(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_z(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_z_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_z_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_z_sync_impl(dst, src, count);
}

// ==========asc_float2half==========
__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_r(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_r(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_r_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_r_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_a(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_a(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_a_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_a_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_f(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_f(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_f_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_f_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_c(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_c(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_c_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_c_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_z(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_z(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_z_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_z_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_z_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_o(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_o_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_o(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_o_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_o_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_o_sync_impl(dst, src, count);
}

// ==========asc_float2int32==========
__aicore__ inline void asc_float2int32_a(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_a(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_a_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_a_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_c(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_c(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_c_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_c_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_f(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_f(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_f_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_f_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_r(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_r(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_r_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_r_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_z(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_z(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_z_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_z_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_z_sync_impl(dst, src, count);
}

// ==========asc_float2int64==========
__aicore__ inline void asc_float2int64_a(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_a(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_a_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_a_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_c(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_c(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_c_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_c_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_f(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_f(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_f_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_f_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_r(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_r(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_r_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_r_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_z(__ubuf__ int64_t* dst, __ubuf__ float* src, const uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_z(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_z_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_z_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_z_sync_impl(dst, src, count);
}

// ==========asc_half2float==========
__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_impl(dst, src, count);
}

__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_sync_impl(dst, src, count);
}

// ==========asc_half2int4==========
__aicore__ inline void asc_half2int4(__ubuf__ void* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_a(__ubuf__ void* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_a(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_a_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_a_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_c(__ubuf__ void* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_c(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_c_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_c_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_f(__ubuf__ void* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_f(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_f_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_f_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_r(__ubuf__ void* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_r(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_r_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_r_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_z(__ubuf__ void* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_z(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_z_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_z_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_z_sync_impl(dst, src, count);
}

// ==========asc_half2int8==========
__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_a(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_a(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_a_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_a_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_c(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_c(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_c_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_c_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_f(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_f(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_f_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_f_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_r(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_r(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_r_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_r_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_z(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_z(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_z_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_z_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_z_sync_impl(dst, src, count);
}

// ==========asc_half2int16==========
__aicore__ inline void asc_half2int16_a(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_a(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_a_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_a_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_c(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_c(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_c_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_c_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_f(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_f(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_f_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_f_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_r(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_r(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_r_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_r_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_z(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_z(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_z_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_z_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_z_sync_impl(dst, src, count);
}

// ==========asc_half2int32==========
__aicore__ inline void asc_half2int32_a(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_a(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_a_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_a_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_c(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_c(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_c_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_c_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_f(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_f(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_f_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_f_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_r(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_r(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_r_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_r_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_z(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_z(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_z_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_z_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_z_sync_impl(dst, src, count);
}

// ==========asc_half2uint8==========
__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_a(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_a(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_a_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_a_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_c(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_c(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_c_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_c_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_f(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_f(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_f_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_f_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_r(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_r(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_r_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_r_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_z(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_z(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_z_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_z_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_z_sync_impl(dst, src, count);
}

// ==========asc_div(half/float)==========
__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_div_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_div_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_div_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_div_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_div_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_div_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_div_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_div_sync_impl(dst, src0, src1, count);
}

// ==========asc_duplicate(half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t)==========
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_sync_impl(dst, src, count);
}

// ==========asc_exp(half/float)==========
__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_exp_impl (dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_impl (dst, src, count);
}

__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_sync_impl(dst, src, count);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_exp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
}

__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_sync_impl(dst, src, count);
}

// ==========asc_gt_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_gt_scalar_impl_half(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_gt_scalar_sync_impl_half(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_gt_scalar_impl_float(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_gt_scalar_sync_impl_float(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_log(half/float)==========
__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_log_impl_half(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_count_impl_half(dst, src, count);
}

__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_sync_impl_half(dst, src, count);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_log_impl_float(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_count_impl_float(dst, src, count);
}

__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_sync_impl_float(dst, src, count);
}

// ==========asc_relu(half/float/int32_t)==========
__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_sync_impl(dst, src, count);
}

// ==========asc_lt(half/float)==========
__aicore__ inline void asc_lt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
        uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
        uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
        uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
        uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_abs(half/float)==========
__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_abs_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
}

__aicore__ inline void asc_abs_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_sync_impl(dst, src, count);
}

__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_abs_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
}

__aicore__ inline void asc_abs_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_sync_impl(dst, src, count);
}

// ==========asc_shift_left(int16_t/uint16_t/int32_t/uint32_t)==========
__aicore__ inline void asc_shift_left(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_left_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shift_left(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count)
{
    asc_shift_left_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shift_left_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src,  uint32_t distance, uint32_t count)
{
    asc_shift_left_sync_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shift_left(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_left_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shift_left(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count)
{
    asc_shift_left_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shift_left_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,  uint32_t distance, uint32_t count)
{
    asc_shift_left_sync_impl(dst, src, distance, count);
}
__aicore__ inline void asc_shift_left(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_left_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shift_left(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count)
{
    asc_shift_left_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shift_left_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src,  uint32_t distance, uint32_t count)
{
    asc_shift_left_sync_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shift_left(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_left_impl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shift_left(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count)
{
    asc_shift_left_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shift_left_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,  uint32_t distance, uint32_t count)
{
    asc_shift_left_sync_impl(dst, src, distance, count);
}

// ==========asc_mul_cast_half2int8==========
__aicore__ inline void asc_mul_cast_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_cast_half2int8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_cast_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2int8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_cast_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2int8_sync_impl(dst, src0, src1, count);
}

// ==========asc_mul_cast_half2uint8==========
__aicore__ inline void asc_mul_cast_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_cast_half2uint8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_cast_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2uint8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_cast_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2uint8_sync_impl(dst, src0, src1, count);
}

// ==========asc_ne(half/float)==========
__aicore__ inline void asc_ne(__ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ int8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ int8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_sync_impl(dst, src0, src1, repeat, 
        dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_ge_scalar(half/float)==========
__aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(dst, src, value, repeat,
        dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_sync_impl(dst, src, value, repeat,
        dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(dst, src, value, repeat,
        dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_sync_impl(dst, src, value, repeat,
        dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_deq_int322half==========
__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int322half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int322half_sync(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_sync_impl(dst, src, count);
}

// ==========asc_mrgsort4(half/float)==========
__aicore__ inline void asc_mrgsort4(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3,
    bool if_exhausted_suspension, uint8_t valid_bit)
{
    asc_mrgsort4_impl(dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

__aicore__ inline void asc_mrgsort4_sync(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3,
    bool if_exhausted_suspension, uint8_t valid_bit)
{
    asc_mrgsort4_sync_impl(dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

__aicore__ inline void asc_mrgsort4(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3,
    bool if_exhausted_suspension, uint8_t valid_bit)
{
    asc_mrgsort4_impl(dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

__aicore__ inline void asc_mrgsort4_sync(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3,
    bool if_exhausted_suspension, uint8_t valid_bit)
{
    asc_mrgsort4_sync_impl(dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

// ==========asc_max(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_max_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_max_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_max_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_max_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_max_sync_impl(dst, src0, src1, count);
}

// ==========asc_max_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_min(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_min_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_min_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_min_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_min_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_min_sync_impl(dst, src0, src1, count);
}

// ==========asc_mul_add_dst(half/float)==========
__aicore__ inline void asc_mul_add_dst(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_dst_count_impl_half_half(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_dst_impl_half_half(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_dst_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_dst_sync_impl_half_half(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_dst_count_impl_float_float(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_dst_impl_float_float(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_dst_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_dst_sync_impl_float_float(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_dst_count_impl_float_half(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_dst_impl_float_half(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_dst_sync(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_dst_sync_impl_float_half(dst, src0, src1, count);
}

// ==========asc_mul(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_mul_sync_impl(dst, src0, src1, count);
}

// ==========asc_mul_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_impl (dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl (dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_sync_impl (dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_select(half/float)==========
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_select_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
                    dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_select_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
                    dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_sync_impl(dst, src0, src1, count);
}

// ==========asc_sqrt(half/float)==========
__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
        uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_sync_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
        uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_sync_impl(dst, src, count);
}

// ==========asc_sub(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride,
        dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_sub_sync_impl(dst, src0, src1, count);
}

// ==========asc_sub_scalar(half)==========
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_gt(half/float)==========
__aicore__ inline void asc_gt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(__ubuf__ uint8_t*dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ uint8_t*dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(__ubuf__ uint8_t*dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt_sync(__ubuf__ uint8_t*dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_add_relu_cast_f162s8==========
__aicore__ inline void asc_add_relu_cast_f162s8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_cast_f162s8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_cast_f162s8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_relu_cast_f162s8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_relu_cast_f162s8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_cast_f162s8_sync_impl(dst, src0, src1, count);
}

// ==========asc_add_relu_cast_f322f16==========
__aicore__ inline void asc_add_relu_cast_f322f16(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_cast_f322f16_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_cast_f322f16(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_relu_cast_f322f16_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_relu_cast_f322f16_sync(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_cast_f322f16_sync_impl(dst, src0, src1, count);
}

// ==========asc_add_relu_cast_s162s8==========
__aicore__ inline void asc_add_relu_cast_s162s8(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_cast_s162s8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_cast_s162s8(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, 
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_add_relu_cast_s162s8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_relu_cast_s162s8_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_cast_s162s8_sync_impl(dst, src0, src1, count);
}

// ==========asc_vdeq_int162b8_h==========
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_impl<int8_t, int16_t>(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, const asc_unary_config& config)
{
    CApiInternal::vdeqs162b8h_impl<int8_t, int16_t>(dst, src, config);
}

__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_sync_impl<int8_t, int16_t>(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_impl<uint8_t, int16_t>(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, const asc_unary_config& config)
{
    CApiInternal::vdeqs162b8h_impl<uint8_t, int16_t>(dst, src, config);
}

__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_sync_impl<uint8_t, int16_t>(dst, src, count);
}

// ==========asc_int322float==========
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_impl(dst, src, count);
}

__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_sync_impl(dst, src, count);
}

// ==========asc_int322float_a==========
__aicore__ inline void asc_int322float_a(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_a_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_a(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_a_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_a_sync_impl(dst, src, count);
}

// ==========asc_int322float_c==========
__aicore__ inline void asc_int322float_c(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_c_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_c(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_c_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_c_sync_impl(dst, src, count);
}

// ==========asc_int322float_f==========
__aicore__ inline void asc_int322float_f(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_f_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_f(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_f_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_f_sync_impl(dst, src, count);
}

// ==========asc_int322float_r==========
__aicore__ inline void asc_int322float_r(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_r_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_r(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_r_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_r_sync_impl(dst, src, count);
}

// ==========asc_int322float_z==========
__aicore__ inline void asc_int322float_z(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_z_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_z(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322float_z_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_z_sync_impl(dst, src, count);
}

// ==========asc_transpose(int16_t/uint16_t)==========
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_impl(dst, src);
}

__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_impl(dst, src);
}

__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

// ==========asc_not(int16_t/uint16_t)==========
__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
}

__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_not_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_not_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_sync_impl(dst, src, count);
}

__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
}

__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_not_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_not_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_sync_impl(dst, src, count);
}

// ==========asc_mul_add_relu(half/float)==========
__aicore__ inline void asc_mul_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_relu_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_relu_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_relu_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_relu_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_relu_sync_impl(dst, src0, src1, count);
}

// ==========asc_sub_relu(half/float/int16_t)==========
__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_sync_impl(dst, src0, src1, count);
}

// ==========asc_le(half/float)==========
__aicore__ inline void asc_le(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ int8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ int8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_sync_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_le_scalar(half/float)==========
__aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_or(int16_t/uint16_t)==========
__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_or_count_impl_int16_t(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_or_impl_int16_t(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_or_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_or_sync_impl_int16_t(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_or_count_impl_uint16_t(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_or_impl_uint16_t(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_or_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_or_sync_impl_uint16_t(dst, src0, src1, count);
}

// ==========asc_rsqrt(half/float)==========
__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::rsqrt_impl<half>(dst, src, count);
}

__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::rsqrt_impl<half>(dst, src, config);
}

__aicore__ inline void asc_rsqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::rsqrt_impl<half>(dst, src, count);
}

__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::rsqrt_impl<float>(dst, src, count);
}

__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::rsqrt_impl<float>(dst, src, config);
}

__aicore__ inline void asc_rsqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::rsqrt_impl<float>(dst, src, count);
}

// ==========asc_add_relu(half/float/int16_t)==========
__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::add_relu_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::add_relu_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::add_relu_sync_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::add_relu_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::add_relu_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::add_relu_sync_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::add_relu_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::add_relu_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::add_relu_sync_impl<int16_t>(dst, src0, src1, count);
}

// ==========asc_ge(half/float/uint16_t)==========
__aicore__ inline void asc_ge(__ubuf__ half* scr0, __ubuf__ half* scr1, asc_binary_config& config)
{
    CApiInternal::ge_impl<half>(scr0, scr1, config);
}

__aicore__ inline void asc_ge_sync(__ubuf__ half* scr0, __ubuf__ half* scr1)
{
    CApiInternal::ge_sync_impl<half>(scr0, scr1);
}

__aicore__ inline void asc_ge(__ubuf__ float* scr0, __ubuf__ float* scr1, asc_binary_config& config)
{
    CApiInternal::ge_impl<float>(scr0, scr1, config);
}

__aicore__ inline void asc_ge_sync(__ubuf__ float* scr0, __ubuf__ float* scr1)
{
    CApiInternal::ge_sync_impl<float>(scr0, scr1);
}

__aicore__ inline void asc_ge(__ubuf__ uint8_t* dst, __ubuf__ half* scr0, __ubuf__ half* scr1, asc_binary_config& config)
{
    CApiInternal::ge_impl<half>(dst, scr0, scr1, config);
}

__aicore__ inline void asc_ge_sync(__ubuf__ uint8_t* dst, __ubuf__ half* scr0, __ubuf__ half* scr1)
{
    CApiInternal::ge_sync_impl<half>(dst, scr0, scr1);
}

__aicore__ inline void asc_ge(__ubuf__ uint8_t* dst, __ubuf__ float* scr0, __ubuf__ float* scr1, asc_binary_config& config)
{
    CApiInternal::ge_impl<float>(dst, scr0, scr1, config);
}

__aicore__ inline void asc_ge_sync(__ubuf__ uint8_t* dst, __ubuf__ float* scr0, __ubuf__ float* scr1)
{
    CApiInternal::ge_sync_impl<float>(dst, scr0, scr1);
}

// ==========asc_int642float_a==========
__aicore__ inline void asc_int642float_a(__ubuf__ float* dst, __ubuf__ int64_t* src, const asc_unary_config& config)
{
    CApiInternal::int642float_a_impl(dst, src, config);
}

__aicore__ inline void asc_int642float_a(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_a_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_a_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_a_sync_impl(dst, src, count);
}

// ==========asc_int642float_c==========
__aicore__ inline void asc_int642float_c(__ubuf__ float* dst, __ubuf__ int64_t* src, const asc_unary_config& config)
{
    CApiInternal::int642float_c_impl(dst, src, config);
}

__aicore__ inline void asc_int642float_c(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_c_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_c_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_c_sync_impl(dst, src, count);
}

// ==========asc_int642float_f==========
__aicore__ inline void asc_int642float_f(__ubuf__ float* dst, __ubuf__ int64_t* src, const asc_unary_config& config)
{
    CApiInternal::int642float_f_impl(dst, src, config);
}

__aicore__ inline void asc_int642float_f(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_f_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_f_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_f_sync_impl(dst, src, count);
}

// ==========asc_int642float_r==========
__aicore__ inline void asc_int642float_r(__ubuf__ float* dst, __ubuf__ int64_t* src, const asc_unary_config& config)
{
    CApiInternal::int642float_r_impl(dst, src, config);
}

__aicore__ inline void asc_int642float_r(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_r_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_r_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_r_sync_impl(dst, src, count);
}

// ==========asc_int642float_z==========
__aicore__ inline void asc_int642float_z(__ubuf__ float* dst, __ubuf__ int64_t* src, const asc_unary_config& config)
{
    CApiInternal::int642float_z_impl(dst, src, config);
}

__aicore__ inline void asc_int642float_z(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_z_impl(dst, src, count);
}

__aicore__ inline void asc_int642float_z_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    CApiInternal::int642float_z_sync_impl(dst, src, count);
}

// ==========asc_deq_int162b8_l(int8_t/uint8_t)==========
__aicore__ inline void asc_vdeq_int162b8l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, const asc_unary_config& config)
{
    CApiInternal::vdeq_int162b8l_impl<int8_t, int16_t>(dst, src, config);
}

__aicore__ inline void asc_vdeq_int162b8l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeq_int162b8l_impl<int8_t, int16_t>(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeq_int162b8l_sync_impl<int8_t, int16_t>(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, const asc_unary_config& config)
{
    CApiInternal::vdeq_int162b8l_impl<uint8_t, int16_t>(dst, src, config);
}

__aicore__ inline void asc_vdeq_int162b8l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeq_int162b8l_impl<uint8_t, int16_t>(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeq_int162b8l_sync_impl<uint8_t, int16_t>(dst, src, count);
}

// ==========asc_min_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_sync_impl(dst, src, value, count);
}

// ==========asc_rcp(half/float)==========
__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rcp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rcp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_sync_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rcp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rcp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_sync_impl(dst, src, count);
}

// ==========asc_shift_right(int16_t/int32_t/uint16_t/uint32_t)==========
__aicore__ inline void asc_shift_right(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_right_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_shift_right_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shift_right_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_right_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_shift_right_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shift_right_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_right_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_shift_right_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value,
    uint32_t count)
{
    asc_shift_right_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_shift_right_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shift_right_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_shift_right_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value,
    uint32_t count)
{
    asc_shift_right_sync_impl(dst, src, value, count);
}

// ==========asc_shift_right_round(int16_t/int32_t)==========
__aicore__ inline void asc_shift_right_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value,
    uint32_t count)
{
    asc_shift_right_round_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value,
    uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
    uint16_t src_repeat_stride)
{
    asc_shift_right_round_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_shift_right_round_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value,
    uint32_t count)
{
    asc_shift_right_round_sync_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value,
    uint32_t count)
{
    asc_shift_right_round_impl(dst, src, value, count);
}

__aicore__ inline void asc_shift_right_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value,
    uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
    uint16_t src_repeat_stride)
{
    asc_shift_right_round_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_shift_right_round_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value,
    uint32_t count)
{
    asc_shift_right_round_sync_impl(dst, src, value, count);
}

// ==========asc_mul_add(half/float)==========
__aicore__ inline void asc_mul_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
          src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_mul_add_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
          src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_sync_impl(dst, src0, src1, count);
}

// ==========asc_eq_scalar(half/float/int32_t)==========
__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

// ==========asc_deq_int162b8==========
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_sync_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_sync_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_sync_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_sync_impl(dst, src, count);
}

// ==========asc_int42half==========
__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ void* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_int42half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ void* src, uint32_t count)
{
    asc_int42half_impl(dst, src, count);
}

__aicore__ inline void asc_int42half_sync(__ubuf__ half* dst, __ubuf__ void* src, uint32_t count)
{
    asc_int42half_sync_impl(dst, src, count);
}

// ==========asc_int82half==========
__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int82half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_impl(dst, src, count);
}

__aicore__ inline void asc_int82half_sync(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_sync_impl(dst, src, count);
}

// ==========asc_uint82half==========
__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_uint82half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_impl(dst, src, count);
}

__aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_sync_impl(dst, src, count);
}

// ==========asc_int162float==========
__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_impl(dst, src, count);
}

__aicore__ inline void asc_int162float_sync(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_sync_impl(dst, src, count);
}

// ==========asc_int322int16==========
__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322int16_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_impl(dst, src, count);
}

__aicore__ inline void asc_int322int16_sync(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_sync_impl(dst, src, count);
}

// ==========asc_int322int64==========
__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322int64_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_impl(dst, src, count);
}

__aicore__ inline void asc_int322int64_sync(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_sync_impl(dst, src, count);
}

// ==========asc_int642int32==========
__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642int32_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_impl(dst, src, count);
}

__aicore__ inline void asc_int642int32_sync(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_sync_impl(dst, src, count);
}

// ==========asc_ne_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t *dst, __ubuf__ half *src, half value, uint8_t repeat, uint16_t dst_block_stride,
           uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar_sync(__ubuf__ uint8_t *dst, __ubuf__ half *src, half value, uint8_t repeat, uint16_t dst_block_stride,
           uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t *dst, __ubuf__ float *src, float value, uint8_t repeat, uint16_t dst_block_stride,
           uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar_sync(__ubuf__ uint8_t *dst, __ubuf__ float *src, float value, uint8_t repeat, uint16_t dst_block_stride,
           uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_sync_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_set_nthbit==========
__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx) 
{
    return set_nthbit_impl(bits, idx);
}

// ==========asc_int162half==========
__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_sync_impl(dst, src, count);
}

// ==========asc_int162half_a==========
__aicore__ inline void asc_int162half_a(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_a(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_a_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_a_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_a_sync_impl(dst, src, count);
}

// ==========asc_int162half_c==========
__aicore__ inline void asc_int162half_c(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_c(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_c_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_c_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_c_sync_impl(dst, src, count);
}

// ==========asc_int162half_f==========
__aicore__ inline void asc_int162half_f(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_f(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_f_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_f_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_f_sync_impl(dst, src, count);
}

// ==========asc_int162half_r==========
__aicore__ inline void asc_int162half_r(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_r(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_r_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_r_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_r_sync_impl(dst, src, count);
}

// ==========asc_int162half_z==========
__aicore__ inline void asc_int162half_z(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_z(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_z_impl(dst, src, count);
}

__aicore__ inline void asc_int162half_z_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
   asc_int162half_z_sync_impl(dst, src, count);
}

// ==========asc_and(int16_t/uint16_t)==========
__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_and_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_and_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    and_sync_impl(dst, src0, src1, count);
}
__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_and_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_and_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    and_sync_impl(dst, src0, src1, count);
}

// ==========asc_sub_relu_cast==========
__aicore__ inline void asc_sub_relu_castf162s8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_castf162s8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu_castf162s8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride, bool h)
{
    asc_sub_relu_castf162s8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, h);
}

__aicore__ inline void asc_sub_relu_castf162s8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_castf162s8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu_castf322f16(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_castf322f16_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu_castf322f16(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride, bool h)
{
    asc_sub_relu_castf322f16_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, h);
}

__aicore__ inline void asc_sub_relu_castf322f16_sync(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_castf322f16_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu_casts162s8(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_casts162s8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu_casts162s8(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride, bool h)
{
    asc_sub_relu_casts162s8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, h);
}

__aicore__ inline void asc_sub_relu_casts162s8_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_casts162s8_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu_cast_vdeqs162b8(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride, bool h)
{
    asc_sub_relu_cast_vdeqs162b8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, h);
}

__aicore__ inline void asc_sub_relu_cast_vdeqs162b8(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride, bool h)
{
    asc_sub_relu_cast_vdeqs162b8_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride, h);
}

// ==========asc_leakyrelu_scalar(half/float)==========
__aicore__ inline void asc_leakyrelu_scalar(__ubuf__ half* dst, __ubuf__ half* src0, half src1, uint32_t count)
{
    asc_leakyrelu_scalar_impl(dst, src0, src1, count);
}
__aicore__ inline void asc_leakyrelu_scalar(__ubuf__ half* dst, __ubuf__ half* src0, half src1, uint8_t repeat, uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride, uint16_t src0_repeat_stride)
{
    asc_leakyrelu_scalar_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride, src0_repeat_stride);
}

__aicore__ inline void asc_leakyrelu_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src0, half src1, uint32_t count)
{
    asc_leakyrelu_scalar_sync_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_leakyrelu_scalar(__ubuf__ float* dst, __ubuf__ float* src0, float src1, uint32_t count)
{
    asc_leakyrelu_scalar_impl(dst, src0, src1, count);
}
__aicore__ inline void asc_leakyrelu_scalar(__ubuf__ float* dst, __ubuf__ float* src0, float src1, uint8_t repeat, uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride, uint16_t src0_repeat_stride)
{
    asc_leakyrelu_scalar_impl(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride, src0_repeat_stride);
}

__aicore__ inline void asc_leakyrelu_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src0, float src1, uint32_t count)
{
    asc_leakyrelu_scalar_sync_impl(dst, src0, src1, count);
}

// ==========asc_lt_scalar(half/float)==========
__aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src0, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride, uint16_t src0_repeat_stride)
{
    asc_lt_scalar_impl(dst, src0, value, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride, src0_repeat_stride);
}

__aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src0, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride, uint16_t src0_repeat_stride)
{
    asc_lt_scalar_impl(dst, src0, value, repeat, dst_block_stride, src0_block_stride, dst_repeat_stride, src0_repeat_stride);
}

// ==========asc_float2int16==========
__aicore__ inline void asc_float2int16_a(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_a_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_a(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_a_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_a_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_a_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_c(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_c_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_c(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_c_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_c_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_c_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_f(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_f_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_f(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_f_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_f_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_f_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_r(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_r_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_r(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_r_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_r_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_r_sync_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_z(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_z_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_z(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_z_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_z_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_z_sync_impl(dst, src, count);
}

// ==========asc_gather(uint16_t/uint32_t)==========
__aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_gather_impl(dst, src, offset_addr, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
{
    asc_gather_impl(dst, src, offset_addr, count);
}

__aicore__ inline void asc_gather_sync(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint32_t count)
{
    asc_gather_sync_impl(dst, src, offset_addr, count);
}

__aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_gather_impl(dst, src, offset_addr, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
{
    asc_gather_impl(dst, src, offset_addr, count);
}

__aicore__ inline void asc_gather_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr,
    uint32_t count)
{
    asc_gather_sync_impl(dst, src, offset_addr, count);
}

// ==========asc_gather_datablock(uint16_t/uint32_t)==========
__aicore__ inline void asc_gather_datablock(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, 
            uint16_t dst_repeat_stride, uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_impl(dst, src, offset_addr, dst_repeat_stride, dst_block_stride, repeat);
}

__aicore__ inline void asc_gather_datablock(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
{
    asc_gather_datablock_impl(dst, src, offset_addr, count);
}

__aicore__ inline void asc_gather_datablock_sync(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
{
    asc_gather_datablock_sync_impl(dst, src, offset_addr, count);
}

__aicore__ inline void asc_gather_datablock(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, 
            uint16_t dst_repeat_stride, uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_impl(dst, src, offset_addr, dst_repeat_stride, dst_block_stride, repeat);
}

__aicore__ inline void asc_gather_datablock(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
{
    asc_gather_datablock_impl(dst, src, offset_addr, count);
}

__aicore__ inline void asc_gather_datablock_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
{
    asc_gather_datablock_sync_impl(dst, src, offset_addr, count);
}

#endif
