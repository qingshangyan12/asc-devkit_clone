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

#include "get_acc_val_impl/asc_2201/get_acc_val_impl.h"
#include "get_cmp_mask_impl/asc_2201/get_cmp_mask_impl.h"
#include "get_max_min_cnt_impl/asc_2201/get_max_min_cnt_impl.h"
#include "get_rsvd_cnt_impl/asc_2201/get_rsvd_cnt_impl.h"
#include "get_vms4_sr_impl/asc_2201/get_vms4_sr_impl.h"
#include "get_arch_ver_impl/asc_2201/get_arch_ver_impl.h"
#include "set_cmp_mask_impl/asc_2201/set_cmp_mask_impl.h"
#include "set_mask_count_impl/asc_2201/set_mask_count_impl.h"
#include "set_mask_norm_impl/asc_2201/set_mask_norm_impl.h"
#include "set_vector_mask_impl/asc_2201/set_vector_mask_impl.h"
#include "clear_nthbit_impl/asc_2201/clear_nthbit_impl.h"
#include "vadd_impl/asc_2201/add_impl.h"
#include "vadds_impl/asc_2201/add_scalar_impl.h"
#include "vbitsort_impl/npu_arch_2201/bitsort_impl.h"
#include "vbrcb_impl/asc_2201/brcb_impl.h"
#include "vcadd_impl/asc_2201/repeat_reduce_sum_impl.h"
#include "vcgadd_impl/asc_2201/data_block_reduce_sum_impl.h"
#include "vcgmax_impl/asc_2201/data_block_reduce_max_impl.h"
#include "vcgmin_impl/asc_2201/data_block_reduce_min_impl.h"
#include "vcmax_impl/asc_2201/repeat_reduce_max_impl.h"
#include "vcmin_impl/asc_2201/repeat_reduce_min_impl.h"
#include "vcmp_eq_impl/asc_2201/asc_eq_impl.h"
#include "vconv_c_api_impl/vconv_bf162float_impl/asc_2201/bf162float_impl.h"
#include "vconv_c_api_impl/vconv_bf162int32_impl/asc_2201/bf162int32_impl.h"
#include "vconv_c_api_impl/vconv_float2float_impl/asc_2201/float2float_impl.h"
#include "vconv_c_api_impl/vconv_float2bf16_impl/asc_2201/float2bf16_impl.h"
#include "vconv_c_api_impl/vconv_float2half_impl/asc_2201/float2half_impl.h"
#include "vconv_c_api_impl/vconv_float2int32_impl/npu_arch_2201/float2int32_impl.h"
#include "vconv_c_api_impl/vconv_float2int64_impl/npu_arch_2201/float2int64_impl.h"
#include "vconv_c_api_impl/vconv_half2float_impl/asc_2201/half2float_impl.h"
#include "vconv_c_api_impl/vconv_half2int4_impl/asc_2201/half2int4_impl.h"
#include "vconv_c_api_impl/vconv_half2int8_impl/asc_2201/asc_half2int8_impl.h"
#include "vconv_c_api_impl/vconv_half2int16_impl/asc_2201/half2int16_impl.h"
#include "vconv_c_api_impl/vconv_half2int32_impl/asc_2201/half2int32_impl.h"
#include "vconv_c_api_impl/vconv_vdeqs162b8h_impl/asc_2201/vdeqs162b8h_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/asc_2201/asc_int322float_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/asc_2201/asc_int322float_a_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/asc_2201/asc_int322float_c_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/asc_2201/asc_int322float_f_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/asc_2201/asc_int322float_r_impl.h"
#include "vconv_c_api_impl/vconv_int322float_impl/asc_2201/asc_int322float_z_impl.h"
#include "vaddreluconv_f162s8_impl/asc_2201/addreluconv_f162s8_impl.h"
#include "vaddreluconv_f322f16_impl/asc_2201/addreluconv_f322f16_impl.h"
#include "vaddreluconv_s162s8_impl/asc_2201/addreluconv_s162s8_impl.h"
#include "vdiv_impl/asc_2201/div_impl.h"
#include "vector_dup_impl/asc_2201/duplicate_impl.h"
#include "vexp_impl/asc_2201/exp_impl.h"
#include "vgt_scalar_impl/npu_arch_2201/gt_scalar_impl.h"
#include "vlog_impl/npu_arch_2201/log_impl.h"
#include "vmax_impl/asc_2201/max_impl.h"
#include "vmaxs_impl/asc_2201/max_scalar_impl.h"
#include "vmin_impl/asc_2201/min_impl.h"
#include "vmul_add_dst_impl/npu_arch_2201/mul_add_dst_impl.h"
#include "vmul_impl/asc_2201/mul_impl.h"
#include "vmuls_impl/asc_2201/mul_scalar_impl.h"
#include "vor_impl/npu_arch_2201/or_impl.h"
#include "vrelu_impl/asc_2201/asc_relu_impl.h"
#include "vsel_impl/asc_2201/select_impl.h"
#include "vsub_impl/asc_2201/sub_impl.h"
#include "vsubs_impl/asc_2201/sub_scalar_impl.h"
#include "vcmp_gt_impl/asc_2201/gt_impl.h"
#include "vtranspose_impl/asc_2201/asc_transpose_impl.h"

__aicore__ inline int64_t asc_get_acc_val()
{
    return CApiInternal::asc_get_acc_val_impl();
}

__aicore__ inline void asc_get_cmp_mask(__ubuf__ void* dst)
{
    CApiInternal::asc_get_cmp_mask_impl(dst);
}

__aicore__ inline void asc_get_reduce_max_min_cnt(half& val, uint32_t& index)
{
    CApiInternal::asc_get_reduce_max_min_cnt_impl(val, index);
}

__aicore__ inline void asc_get_reduce_max_min_cnt(float& val, uint32_t& index)
{
    CApiInternal::asc_get_reduce_max_min_cnt_impl(val, index);
}

__aicore__ inline int64_t asc_get_rsvd_count()
{
    return CApiInternal::asc_get_rsvd_count_impl();
}

__aicore__ inline void asc_get_vms4_sr(uint16_t sorted_num[4])
{
    CApiInternal::asc_get_vms4_sr_impl(sorted_num);
}

__aicore__ inline int64_t asc_get_arch_ver()
{
    return CApiInternal::get_arch_ver_impl();
}

__aicore__ inline void asc_set_cmp_mask(__ubuf__ void* sel_mask)
{
    CApiInternal::asc_set_cmp_mask_impl(sel_mask);
}

__aicore__ inline void asc_set_mask_count()
{
    CApiInternal::asc_set_mask_count_impl();
}

__aicore__ inline void asc_set_mask_norm()
{
    CApiInternal::asc_set_mask_norm_impl();
}

__aicore__ inline void asc_set_vector_mask(uint64_t mask1, uint64_t mask0)
{
    CApiInternal::asc_set_vector_mask_impl(mask1, mask0);
}

__aicore__ inline uint64_t asc_clear_nthbit_impl(uint64_t bits, int64_t idx)
{
    CApiInternal::clear_nthbit_impl(bits, idx);
}
// ==========asc_add(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_add_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_add_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_add_sync_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_add_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_add_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_add_sync_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_add_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_add_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_add_sync_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_add_impl<int32_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_add_impl<int32_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_add_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_add_sync_impl<int32_t>(dst, src0, src1, count);
}

// ==========asc_add_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_add_scalar_impl<half> (dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, const asc_unary_config& config)
{
    CApiInternal::asc_add_scalar_impl<half> (dst, src, a, config);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_add_scalar_sync_impl<half> (dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_add_scalar_impl<float>(dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, const asc_unary_config& config)
{
    CApiInternal::asc_add_scalar_impl<float>(dst, src, a, config);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_add_scalar_sync_impl<float>(dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_add_scalar_impl<int16_t>(dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, const asc_unary_config& config)
{
    CApiInternal::asc_add_scalar_impl<int16_t>(dst, src, a, config);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_add_scalar_sync_impl<int16_t>(dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_add_scalar_impl<int32_t>(dst, src, a, count);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, const asc_unary_config& config)
{
    CApiInternal::asc_add_scalar_impl<int32_t>(dst, src, a, config);
}

__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_add_scalar_sync_impl<int32_t>(dst, src, a, count);
}

// ==========asc_bitsort(half/float)==========
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    CApiInternal::asc_bitsort_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_bitsort_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_bitsort_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_bitsort_sync_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    CApiInternal::asc_bitsort_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_bitsort_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_bitsort_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_bitsort_sync_impl<float>(dst, src0, src1, config);
}

// ==========asc_brcb(uint16_t/uint32_t)==========
__aicore__ inline void asc_brcb(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, const asc_brcb_config& config)
{
    CApiInternal::asc_brcb_impl<uint16_t>(dst, src, config);
}

__aicore__ inline void asc_brcb_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, const asc_brcb_config& config)
{
    CApiInternal::asc_brcb_sync_impl<uint16_t>(dst, src, config);
}

__aicore__ inline void asc_brcb(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, const asc_brcb_config& config)
{
    CApiInternal::asc_brcb_impl<uint32_t>(dst, src, config);
}

__aicore__ inline void asc_brcb_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, const asc_brcb_config& config)
{
    CApiInternal::asc_brcb_sync_impl<uint32_t>(dst, src, config);
}

// ==========asc_datablock_reduce(half/float)==========
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, const asc_block_reduce_config& config)
{
    CApiInternal::asc_data_block_reduce_sum_impl<half>(dst, src, config);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_sum_impl<half>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_sum_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, const asc_block_reduce_config& config)
{
    CApiInternal::asc_data_block_reduce_sum_impl<float>(dst, src, config);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_sum_impl<float>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_sum_sync_impl<float>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, const asc_block_reduce_config& config)
{
    CApiInternal::asc_data_block_reduce_max_impl<half>(dst, src, config);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_max_impl<half>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_max_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, const asc_block_reduce_config& config)
{
    CApiInternal::asc_data_block_reduce_max_impl<float>(dst, src, config);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_max_impl<float>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_max_sync_impl<float>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, const asc_block_reduce_config& config)
{
    CApiInternal::asc_data_block_reduce_min_impl<half>(dst, src, config);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_min_impl<half>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_min_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, const asc_block_reduce_config& config)
{
    CApiInternal::asc_data_block_reduce_min_impl<float>(dst, src, config);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_min_impl<float>(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_data_block_reduce_min_sync_impl<float>(dst, src, count);
}

// ==========asc_repeat_reduce(half/float)==========
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, const asc_repeat_reduce_config& config)
{
    CApiInternal::asc_repeat_reduce_sum_impl<half>(dst, src, config);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_repeat_reduce_sum_impl<half>(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_repeat_reduce_sum_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, const asc_repeat_reduce_config& config)
{
    CApiInternal::asc_repeat_reduce_sum_impl<float>(dst, src, config);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_repeat_reduce_sum_impl<float>(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_repeat_reduce_sum_sync_impl<float>(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, const asc_repeat_reduce_config& config, order_t order)
{
    CApiInternal::asc_repeat_reduce_max_impl<half>(dst, src, config, order);
}

__aicore__ inline void asc_repeat_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_max_impl<half>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_max_sync_impl<half>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, const asc_repeat_reduce_config& config, order_t order)
{
    CApiInternal::asc_repeat_reduce_max_impl<float>(dst, src, config, order);
}

__aicore__ inline void asc_repeat_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_max_impl<float>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_max_sync_impl<float>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, const asc_repeat_reduce_config& config, order_t order)
{
    CApiInternal::asc_repeat_reduce_min_impl<half>(dst, src, config, order);
}

__aicore__ inline void asc_repeat_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_min_impl<half>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_min_sync_impl<half>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, const asc_repeat_reduce_config& config, order_t order)
{
    CApiInternal::asc_repeat_reduce_min_impl<float>(dst, src, config, order);
}

__aicore__ inline void asc_repeat_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_min_impl<float>(dst, src, count, order);
}

__aicore__ inline void asc_repeat_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count, order_t order)
{
    CApiInternal::asc_repeat_reduce_min_sync_impl<float>(dst, src, count, order);
}

// ==========asc_eq(half/float/uint8_t-half/uint8_t-float/uint8_t-int32_t)==========
__aicore__ inline void asc_eq(__ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_impl<half>(src0, src1, config);
}

__aicore__ inline void asc_eq_sync(__ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_sync_impl<half>(src0, src1, config);
}

__aicore__ inline void asc_eq(__ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_impl<float>(src0, src1, config);
}

__aicore__ inline void asc_eq_sync(__ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_sync_impl<float>(src0, src1, config);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_impl<uint8_t, half>(dst, src0, src1, config);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_sync_impl<uint8_t, half>(dst, src0, src1, config);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_impl<uint8_t, float>(dst, src0, src1, config);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_sync_impl<uint8_t, float>(dst, src0, src1, config);
}

__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_impl<uint8_t, int32_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_eq_sync_impl<uint8_t, int32_t>(dst, src0, src1, config);
}

// ==========asc_bf162float==========
__aicore__ inline void asc_bf162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_bf162float_impl<float, bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_bf162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162float_impl<float, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162float_sync_impl<float, bfloat16_t>(dst, src, count);
}

// ==========asc_bf162int32==========
__aicore__ inline void asc_bf162int32_a(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_bf162int32_a_impl<int32_t, bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_bf162int32_a(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_a_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_a_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_a_sync_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_c(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_bf162int32_c_impl<int32_t, bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_bf162int32_c(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_c_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_c_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_c_sync_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_f(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_bf162int32_f_impl<int32_t, bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_bf162int32_f(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_f_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_f_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_f_sync_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_r(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_bf162int32_r_impl<int32_t, bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_bf162int32_r(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_r_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_r_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_r_sync_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_z(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_bf162int32_z_impl<int32_t, bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_bf162int32_z(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_z_impl<int32_t, bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_bf162int32_z_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    CApiInternal::asc_bf162int32_z_sync_impl<int32_t, bfloat16_t>(dst, src, count);
}

// ==========asc_float2bf16==========
__aicore__ inline void asc_float2bf16_r(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2bf16_r_impl<bfloat16_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2bf16_r(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_r_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_r_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_r_sync_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_a(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2bf16_a_impl<bfloat16_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2bf16_a(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_a_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_a_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_a_sync_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_f(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2bf16_f_impl<bfloat16_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2bf16_f(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_f_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_f_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_f_sync_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_c(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2bf16_c_impl<bfloat16_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2bf16_c(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_c_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_c_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_c_sync_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_z(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2bf16_z_impl<bfloat16_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2bf16_z(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_z_impl<bfloat16_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2bf16_z_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2bf16_z_sync_impl<bfloat16_t, float>(dst, src, count);
}

// ==========asc_float2float==========
__aicore__ inline void asc_float2float_r(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2float_r_impl<float, float>(dst, src, config);
}

__aicore__ inline void asc_float2float_r(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_r_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_r_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_r_sync_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_f(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2float_f_impl<float, float>(dst, src, config);
}

__aicore__ inline void asc_float2float_f(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_f_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_f_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_f_sync_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_c(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2float_c_impl<float, float>(dst, src, config);
}

__aicore__ inline void asc_float2float_c(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_c_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_c_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_c_sync_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_a(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2float_a_impl<float, float>(dst, src, config);
}

__aicore__ inline void asc_float2float_a(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_a_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_a_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_a_sync_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_z(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2float_z_impl<float, float>(dst, src, config);
}

__aicore__ inline void asc_float2float_z(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_z_impl<float, float>(dst, src, count);
}

__aicore__ inline void asc_float2float_z_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2float_z_sync_impl<float, float>(dst, src, count);
}

// ==========asc_float2half==========
__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_sync_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_r(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_r_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half_r(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_r_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_r_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_r_sync_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_a(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_a_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half_a(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_a_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_a_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_a_sync_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_f(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_f_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half_f(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_f_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_f_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_f_sync_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_c(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_c_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half_c(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_c_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_c_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_c_sync_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_z(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_z_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half_z(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_z_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_z_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_z_sync_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_o(__ubuf__ half* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2half_o_impl<half, float>(dst, src, config);
}

__aicore__ inline void asc_float2half_o(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_o_impl<half, float>(dst, src, count);
}

__aicore__ inline void asc_float2half_o_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2half_o_sync_impl<half, float>(dst, src, count);
}

// ==========asc_float2int32==========
__aicore__ inline void asc_float2int32_a(__ubuf__ int32_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int32_a_impl<int32_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int32_a(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_a_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_a_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_a_sync_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_c(__ubuf__ int32_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int32_c_impl<int32_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int32_c(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_c_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_c_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_c_sync_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_f(__ubuf__ int32_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int32_f_impl<int32_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int32_f(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_f_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_f_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_f_sync_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_r(__ubuf__ int32_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int32_r_impl<int32_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int32_r(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_r_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_r_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_r_sync_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_z(__ubuf__ int32_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int32_z_impl<int32_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int32_z(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_z_impl<int32_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int32_z_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int32_z_sync_impl<int32_t, float>(dst, src, count);
}

// ==========asc_float2int64==========
__aicore__ inline void asc_float2int64_a(__ubuf__ int64_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int64_a_impl<int64_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int64_a(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_a_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_a_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_a_sync_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_c(__ubuf__ int64_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int64_c_impl<int64_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int64_c(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_c_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_c_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_c_sync_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_f(__ubuf__ int64_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int64_f_impl<int64_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int64_f(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_f_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_f_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_f_sync_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_r(__ubuf__ int64_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int64_r_impl<int64_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int64_r(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_r_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_r_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_r_sync_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_z(__ubuf__ int64_t* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_float2int64_z_impl<int64_t, float>(dst, src, config);
}

__aicore__ inline void asc_float2int64_z(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_z_impl<int64_t, float>(dst, src, count);
}

__aicore__ inline void asc_float2int64_z_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_float2int64_z_sync_impl<int64_t, float>(dst, src, count);
}

// ==========asc_half2float==========
__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2float_impl<float, half>(dst, src, config);
}

__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2float_impl<float, half>(dst, src, count);
}

__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2float_sync_impl<float, half>(dst, src, count);
}

// ==========asc_half2int4==========
__aicore__ inline void asc_half2int4(__ubuf__ void* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int4_impl<void, half>(dst, src, config);
}

__aicore__ inline void asc_half2int4(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_sync_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_a(__ubuf__ void* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int4_a_impl<void, half>(dst, src, config);
}

__aicore__ inline void asc_half2int4_a(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_a_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_a_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_a_sync_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_c(__ubuf__ void* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int4_c_impl<void, half>(dst, src, config);
}

__aicore__ inline void asc_half2int4_c(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_c_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_c_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_c_sync_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_f(__ubuf__ void* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int4_f_impl<void, half>(dst, src, config);
}

__aicore__ inline void asc_half2int4_f(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_f_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_f_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_f_sync_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_r(__ubuf__ void* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int4_r_impl<void, half>(dst, src, config);
}

__aicore__ inline void asc_half2int4_r(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_r_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_r_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_r_sync_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_z(__ubuf__ void* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int4_z_impl<void, half>(dst, src, config);
}

__aicore__ inline void asc_half2int4_z(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_z_impl<void, half>(dst, src, count);
}

__aicore__ inline void asc_half2int4_z_sync(__ubuf__ void* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int4_z_sync_impl<void, half>(dst, src, count);
}

// ==========asc_half2int8==========
__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int8_impl<int8_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_sync_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_a(__ubuf__ int8_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int8_a_impl<int8_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int8_a(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_a_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_a_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_a_sync_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_c(__ubuf__ int8_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int8_c_impl<int8_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int8_c(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_c_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_c_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_c_sync_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_f(__ubuf__ int8_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int8_f_impl<int8_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int8_f(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_f_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_f_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_f_sync_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_r(__ubuf__ int8_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int8_r_impl<int8_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int8_r(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_r_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_r_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_r_sync_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_z(__ubuf__ int8_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int8_z_impl<int8_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int8_z(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_z_impl<int8_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int8_z_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int8_z_sync_impl<int8_t, half>(dst, src, count);
}

// ==========asc_half2int16==========
__aicore__ inline void asc_half2int16_a(__ubuf__ int16_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int16_a_impl<int16_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int16_a(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_a_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_a_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_a_sync_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_c(__ubuf__ int16_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int16_c_impl<int16_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int16_c(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_c_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_c_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_c_sync_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_f(__ubuf__ int16_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int16_f_impl<int16_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int16_f(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_f_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_f_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_f_sync_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_r(__ubuf__ int16_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int16_r_impl<int16_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int16_r(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_r_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_r_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_r_sync_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_z(__ubuf__ int16_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int16_z_impl<int16_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int16_z(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_z_impl<int16_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int16_z_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int16_z_sync_impl<int16_t, half>(dst, src, count);
}

// ==========asc_half2int32==========
__aicore__ inline void asc_half2int32_a(__ubuf__ int32_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int32_a_impl<int32_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int32_a(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_a_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_a_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_a_sync_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_c(__ubuf__ int32_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int32_c_impl<int32_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int32_c(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_c_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_c_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_c_sync_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_f(__ubuf__ int32_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int32_f_impl<int32_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int32_f(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_f_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_f_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_f_sync_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_r(__ubuf__ int32_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int32_r_impl<int32_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int32_r(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_r_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_r_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_r_sync_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_z(__ubuf__ int32_t* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_half2int32_z_impl<int32_t, half>(dst, src, config);
}

__aicore__ inline void asc_half2int32_z(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_z_impl<int32_t, half>(dst, src, count);
}

__aicore__ inline void asc_half2int32_z_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_half2int32_z_sync_impl<int32_t, half>(dst, src, count);
}

// ==========asc_div(half/float)==========
__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_div_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_div_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_div_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_div_sync_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_div_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_div_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_div_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_div_sync_impl<float>(dst, src0, src1, count);
}

// ==========asc_duplicate(half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t)==========
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<half>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<half>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<int16_t>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<int16_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<int16_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<uint16_t>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<uint16_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<uint16_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<bfloat16_t>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<bfloat16_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<float>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<float>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<float>(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<int32_t>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<int32_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<int32_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, const asc_duplicate_config& config)
{
    CApiInternal::asc_duplicate_impl<uint32_t>(dst, src, config);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_impl<uint32_t>(dst, src, count);
}

__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    CApiInternal::asc_duplicate_sync_impl<uint32_t>(dst, src, count);
}

// ==========asc_exp(half/float)==========
__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_exp_impl<half> (dst, src, config);
}

__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_exp_impl<half> (dst, src, count);
}

__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_exp_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_exp_impl<float> (dst, src, config);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_exp_impl<float> (dst, src, count);
}

__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_exp_sync_impl<float>(dst, src, count);
}

// ==========asc_gt_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half src1, const asc_unary_config& config)
{
    CApiInternal::asc_gt_scalar_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half src1, const asc_unary_config &config)
{
    CApiInternal::asc_gt_scalar_sync_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float src1, const asc_unary_config& config)
{
    CApiInternal::asc_gt_scalar_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float src1, const asc_unary_config &config)
{
    CApiInternal::asc_gt_scalar_sync_impl<float>(dst, src0, src1, config);
}

// ==========asc_log(half/float)==========
__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_log_impl<half> (dst, src, config);
}

__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_log_impl<half> (dst, src, count);
}

__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_log_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_log_impl<float> (dst, src, config);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_log_impl<float> (dst, src, count);
}

__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_log_sync_impl<float>(dst, src, count);
}

// ==========asc_relu(half/float/int32_t)==========
__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, const asc_unary_config& config)
{
    CApiInternal::asc_relu_impl<half>(dst, src, config);
}

__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_relu_impl<half>(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    CApiInternal::asc_relu_sync_impl<half>(dst, src, count);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, const asc_unary_config& config)
{
    CApiInternal::asc_relu_impl<float>(dst, src, config);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_relu_impl<float>(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    CApiInternal::asc_relu_sync_impl<float>(dst, src, count);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_relu_impl<int32_t>(dst, src, config);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_relu_impl<int32_t>(dst, src, count);
}

__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_relu_sync_impl<int32_t>(dst, src, count);
}

// ==========asc_max(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_max_impl<half> (dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_max_impl<half> (dst, src0, src1, config);
}

__aicore__ inline void asc_max_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_max_sync_impl<half> (dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_max_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_max_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_max_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_max_sync_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_max_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_max_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_max_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_max_sync_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_max_impl<int32_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_max_impl<int32_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_max_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_max_sync_impl<int32_t>(dst, src0, src1, count);
}

// ==========asc_max_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_max_scalar_impl<half>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, const asc_unary_config& config)
{
    CApiInternal::asc_max_scalar_impl<half>(dst, src, a, config);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_max_scalar_sync_impl<half>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_max_scalar_impl<float>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, const asc_unary_config& config)
{
    CApiInternal::asc_max_scalar_impl<float>(dst, src, a, config);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_max_scalar_sync_impl<float>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_max_scalar_impl<int16_t>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, const asc_unary_config& config)
{
    CApiInternal::asc_max_scalar_impl<int16_t>(dst, src, a, config);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_max_scalar_sync_impl<int16_t>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_max_scalar_impl<int32_t>(dst, src, a, count);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, const asc_unary_config& config)
{
    CApiInternal::asc_max_scalar_impl<int32_t>(dst, src, a, config);
}

__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_max_scalar_sync_impl<int32_t>(dst, src, a, count);
}

// ==========asc_min(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_min_impl<half> (dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_min_impl<half> (dst, src0, src1, config);
}

__aicore__ inline void asc_min_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_min_sync_impl<half> (dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_min_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_min_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_min_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_min_sync_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_min_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_min_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_min_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_min_sync_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_min_impl<int32_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_min_impl<int32_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_min_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_min_sync_impl<int32_t>(dst, src0, src1, count);
}

// ==========asc_mul_add_dst(half/float)==========
__aicore__ inline void asc_mul_add_dst(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_mul_add_dst_impl<half, half>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_add_dst_impl<half, half>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_add_dst_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_mul_add_dst_sync_impl<half, half>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_mul_add_dst_impl<float, float>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_add_dst_impl<float, float>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_add_dst_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_mul_add_dst_sync_impl<float, float>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_mul_add_dst_impl<float, half>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_dst(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_add_dst_impl<float, half>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_add_dst_sync(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_mul_add_dst_sync_impl<float, half>(dst, src0, src1, count);
}

// ==========asc_mul(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_mul_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_mul_sync_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_mul_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_mul_sync_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_mul_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_mul_sync_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_mul_impl<int32_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_mul_impl<int32_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_mul_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_mul_sync_impl<int32_t>(dst, src0, src1, count);
}

// ==========asc_mul_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_impl<half> (dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, const asc_unary_config& config)
{
    CApiInternal::asc_mul_scalar_impl<half> (dst, src, a, config);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_sync_impl<half> (dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_impl<float>(dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, const asc_unary_config& config)
{
    CApiInternal::asc_mul_scalar_impl<float>(dst, src, a, config);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_sync_impl<float>(dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_impl<int16_t>(dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, const asc_unary_config& config)
{
    CApiInternal::asc_mul_scalar_impl<int16_t>(dst, src, a, config);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_sync_impl<int16_t>(dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_impl<int32_t>(dst, src, a, count);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, const asc_unary_config& config)
{
    CApiInternal::asc_mul_scalar_impl<int32_t>(dst, src, a, config);
}

__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_mul_scalar_sync_impl<int32_t>(dst, src, a, count);
}

// ==========asc_select(half/float)==========
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_select_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_select_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_select_sync_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_select_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_select_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_select_sync_impl<float>(dst, src0, src1, count);
}
// ==========asc_sub(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_sub_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::asc_sub_impl<half>(dst, src0, src1, config);
}

__aicore__ inline void asc_sub_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::asc_sub_sync_impl<half>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_sub_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::asc_sub_impl<float>(dst, src0, src1, config);
}

__aicore__ inline void asc_sub_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::asc_sub_sync_impl<float>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_sub_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_sub_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_sub_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_sub_sync_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_sub_impl<int32_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_sub_impl<int32_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_sub_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    CApiInternal::asc_sub_sync_impl<int32_t>(dst, src0, src1, count);
}

// ==========asc_sub_scalar(half)==========
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_impl<half> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half a, const asc_unary_config& config)
{
    CApiInternal::asc_sub_scalar_impl<half> (dst, src, a, config);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_sync_impl<half> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_impl<float> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float a, const asc_unary_config& config)
{
    CApiInternal::asc_sub_scalar_impl<float> (dst, src, a, config);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_sync_impl<float> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_impl<int16_t> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, const asc_unary_config& config)
{
    CApiInternal::asc_sub_scalar_impl<int16_t> (dst, src, a, config);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_sync_impl<int16_t> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_impl<int32_t> (dst, src, a, count);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, const asc_unary_config& config)
{
    CApiInternal::asc_sub_scalar_impl<int32_t> (dst, src, a, config);
}

__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t a, uint32_t count)
{
    CApiInternal::asc_sub_scalar_sync_impl<int32_t> (dst, src, a, count);
}

// ==========asc_gt(half/float)==========
__aicore__ inline void asc_gt(__ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::gt_impl<half> (src0, src1, config);
}

__aicore__ inline void asc_gt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::gt_sync_impl<half> (src0, src1, count);
}

__aicore__ inline void asc_gt(__ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::gt_impl<float> (src0, src1, config);
}

__aicore__ inline void asc_gt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::gt_sync_impl<float> (src0, src1, count);
}

__aicore__ inline void asc_gt(__ubuf__ uint8_t*dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::gt_impl<half> (dst, src0, src1, config);
}

__aicore__ inline void asc_gt_sync(__ubuf__ uint8_t*dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::gt_sync_impl<half> (dst, src0, src1, count);
}

__aicore__ inline void asc_gt(__ubuf__ uint8_t*dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::gt_impl<float> (dst, src0, src1, config);
}

__aicore__ inline void asc_gt_sync(__ubuf__ uint8_t*dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::gt_sync_impl<float> (dst, src0, src1, count);
}

// ==========asc_add_relu_cast_f162s8==========
__aicore__ inline void asc_add_relu_cast_f162s8(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::add_relu_cast_f162s8_impl<half> (dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_cast_f162s8(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    CApiInternal::add_relu_cast_f162s8_impl<half> (dst, src0, src1, config);
}

__aicore__ inline void asc_add_relu_cast_f162s8_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    CApiInternal::add_relu_cast_f162s8_sync_impl<half> (dst, src0, src1, count);
}

// ==========asc_add_relu_cast_f322f16==========
__aicore__ inline void asc_add_relu_cast_f322f16(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::add_relu_cast_f322f16_impl<float> (dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_cast_f322f16(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    CApiInternal::add_relu_cast_f322f16_impl<float> (dst, src0, src1, config);
}

__aicore__ inline void asc_add_relu_cast_f322f16_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    CApiInternal::add_relu_cast_f322f16_sync_impl<float> (dst, src0, src1, count);
}

// ==========asc_add_relu_cast_s162s8==========
__aicore__ inline void asc_add_relu_cast_s162s8(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::add_relu_cast_s162s8_impl<int16_t> (dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu_cast_s162s8(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::add_relu_cast_s162s8_impl<int16_t> (dst, src0, src1, config);
}

__aicore__ inline void asc_add_relu_cast_s162s8_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::add_relu_cast_s162s8_sync_impl<int16_t> (dst, src0, src1, count);
}

// ==========asc_vdeq_int162b8_h==========
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_impl<int8_t, int16_t> (dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, const asc_unary_config& config)
{
    CApiInternal::vdeqs162b8h_impl<int8_t, int16_t> (dst, src, config);
}

__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_sync_impl<int8_t, int16_t> (dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_impl<uint8_t, int16_t> (dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, const asc_unary_config& config)
{
    CApiInternal::vdeqs162b8h_impl<uint8_t, int16_t> (dst, src, config);
}

__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    CApiInternal::vdeqs162b8h_sync_impl<uint8_t, int16_t> (dst, src, count);
}

// ==========asc_int322float==========
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_impl<float, int32_t> (dst, src, count);
}

__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_int322float_impl<float, int32_t> (dst, src, config);
}

__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_sync_impl<float, int32_t> (dst, src, count);
}

// ==========asc_int322float_a==========
__aicore__ inline void asc_int322float_a(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_a_impl<float, int32_t> (dst, src, count);
}

__aicore__ inline void asc_int322float_a(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_int322float_a_impl<float, int32_t> (dst, src, config);
}

__aicore__ inline void asc_int322float_a_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_a_sync_impl<float, int32_t> (dst, src, count);
}

// ==========asc_int322float_c==========
__aicore__ inline void asc_int322float_c(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_c_impl<float, int32_t> (dst, src, count);
}

__aicore__ inline void asc_int322float_c(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_int322float_c_impl<float, int32_t> (dst, src, config);
}

__aicore__ inline void asc_int322float_c_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_c_sync_impl<float, int32_t> (dst, src, count);
}

// ==========asc_int322float_f==========
__aicore__ inline void asc_int322float_f(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_f_impl<float, int32_t> (dst, src, count);
}

__aicore__ inline void asc_int322float_f(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_int322float_f_impl<float, int32_t> (dst, src, config);
}

__aicore__ inline void asc_int322float_f_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_f_sync_impl<float, int32_t> (dst, src, count);
}

// ==========asc_int322float_r==========
__aicore__ inline void asc_int322float_r(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_r_impl<float, int32_t> (dst, src, count);
}

__aicore__ inline void asc_int322float_r(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_int322float_r_impl<float, int32_t> (dst, src, config);
}

__aicore__ inline void asc_int322float_r_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_r_sync_impl<float, int32_t> (dst, src, count);
}

// ==========asc_int322float_z==========
__aicore__ inline void asc_int322float_z(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_z_impl<float, int32_t> (dst, src, count);
}

__aicore__ inline void asc_int322float_z(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
{
    CApiInternal::asc_int322float_z_impl<float, int32_t> (dst, src, config);
}

__aicore__ inline void asc_int322float_z_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    CApiInternal::asc_int322float_z_sync_impl<float, int32_t> (dst, src, count);
}

// ==========asc_transpose(int16_t/uint16_t)==========
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    CApiInternal::asc_transpose_impl<int16_t>(dst, src);
}

__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    CApiInternal::asc_transpose_sync_impl<int16_t>(dst, src);
}

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    CApiInternal::asc_transpose_impl<uint16_t>(dst, src);
}

__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    CApiInternal::asc_transpose_sync_impl<uint16_t>(dst, src);
}

// ==========asc_or(int16_t/uint16_t)==========
__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_or_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_or_impl<int16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_or_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    CApiInternal::asc_or_sync_impl<int16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    CApiInternal::asc_or_impl<uint16_t>(dst, src0, src1, count);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, const asc_binary_config& config)
{
    CApiInternal::asc_or_impl<uint16_t>(dst, src0, src1, config);
}

__aicore__ inline void asc_or_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    CApiInternal::asc_or_sync_impl<uint16_t>(dst, src0, src1, count);
}
#endif