/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/vector_compute/vector_compute.h"

static void test_host_c_api_vector_compute_0()
{
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_abs);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_abs_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_add);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_add_relu);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_add_relu_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_add_scalar);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_add_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_add_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* __ubuf__ int16_t* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_and);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_and_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_axpy);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_axpy_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ bfloat16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_bfloat162float);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ bfloat16_t* uint32_t)>(&asc_bfloat162float_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_bfloat162int32_rd);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint32_t)>(&asc_bfloat162int32_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_bfloat162int32_rn);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint32_t)>(&asc_bfloat162int32_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_bfloat162int32_rna);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint32_t)>(&asc_bfloat162int32_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_bfloat162int32_ru);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint32_t)>(&asc_bfloat162int32_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_bfloat162int32_rz);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ bfloat16_t* uint32_t)>(&asc_bfloat162int32_rz_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ uint32_t* int32_t)>(&asc_bitsort);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ uint32_t* int32_t)>(&asc_bitsort_sync);
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* uint16_t uint16_t uint8_t)>(&asc_brcb);
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* uint16_t uint16_t uint8_t)>(&asc_brcb_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_copy);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_copy_sync);
}

static void test_host_c_api_vector_compute_1()
{
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_datablock_reduce_max);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_datablock_reduce_max_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_datablock_reduce_min);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_datablock_reduce_min_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_datablock_reduce_sum);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_datablock_reduce_sum_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_deq_int162b8_h);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_deq_int162b8_h_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_deq_int162b8_l);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_deq_int162b8_l_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int32_t* uint8_t uint16_t uint16_t uint8_t uint8_t)>(&asc_deq_int322half);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int32_t* uint32_t)>(&asc_deq_int322half_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_div);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_div_sync);
    (void)static_cast<void(*)(__ubuf__ half* half uint8_t uint16_t uint16_t)>(&asc_duplicate);
    (void)static_cast<void(*)(__ubuf__ half* half uint32_t)>(&asc_duplicate_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_eq);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_eq_scalar);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_eq_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_eq_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_exp);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_exp_sync);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2bfloat16_rd);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint32_t)>(&asc_float2bfloat16_rd_sync);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2bfloat16_rn);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint32_t)>(&asc_float2bfloat16_rn_sync);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2bfloat16_rna);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint32_t)>(&asc_float2bfloat16_rna_sync);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2bfloat16_ru);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint32_t)>(&asc_float2bfloat16_ru_sync);
}

static void test_host_c_api_vector_compute_2()
{
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2bfloat16_rz);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __ubuf__ float* uint32_t)>(&asc_float2bfloat16_rz_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2float_rd);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint32_t)>(&asc_float2float_rd_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2float_rn);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint32_t)>(&asc_float2float_rn_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2float_rna);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint32_t)>(&asc_float2float_rna_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2float_ru);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint32_t)>(&asc_float2float_ru_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2float_rz);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ float* uint32_t)>(&asc_float2float_rz_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half_rd);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_rd_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half_rn);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_rn_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half_rna);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_rna_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half_ro);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_ro_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half_ru);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_ru_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2half_rz);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_rz_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ float* uint32_t)>(&asc_float2half_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int16_rd);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint32_t)>(&asc_float2int16_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int16_rn);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint32_t)>(&asc_float2int16_rn_sync);
}

static void test_host_c_api_vector_compute_3()
{
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int16_rna);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint32_t)>(&asc_float2int16_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int16_ru);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint32_t)>(&asc_float2int16_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int16_rz);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ float* uint32_t)>(&asc_float2int16_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int32_rd);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint32_t)>(&asc_float2int32_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int32_rn);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint32_t)>(&asc_float2int32_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int32_rna);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint32_t)>(&asc_float2int32_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int32_ru);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint32_t)>(&asc_float2int32_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int32_rz);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ float* uint32_t)>(&asc_float2int32_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int64_rd);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint32_t)>(&asc_float2int64_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int64_rn);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint32_t)>(&asc_float2int64_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int64_rna);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint32_t)>(&asc_float2int64_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int64_ru);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint32_t)>(&asc_float2int64_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* const uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_float2int64_rz);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ float* uint32_t)>(&asc_float2int64_rz_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_fma);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_fma_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* __ubuf__ uint32_t* uint16_t uint8_t)>(&asc_gather);
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* __ubuf__ uint32_t* uint16_t uint8_t uint8_t)>(&asc_gather_datablock);
}

static void test_host_c_api_vector_compute_4()
{
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* __ubuf__ uint32_t* uint16_t uint8_t uint8_t)>(&asc_gather_datablock_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* __ubuf__ uint32_t* uint32_t)>(&asc_gather_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_ge);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_ge_scalar);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_ge_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_ge_sync);
    (void)static_cast<int64_t(*)()>(&asc_get_acc_val);
    (void)static_cast<void(*)(__ubuf__ void*)>(&asc_get_cmp_mask);
    (void)static_cast<void(*)(half& uint32_t&)>(&asc_get_reduce_max_cnt);
    (void)static_cast<void(*)(half& uint32_t&)>(&asc_get_reduce_min_cnt);
    (void)static_cast<int64_t(*)()>(&asc_get_rsvd_count);
    (void)static_cast<void(*)(uint16_t sorted_num[4])>(&asc_get_vms4_sr);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_gt);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_gt_scalar);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_gt_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_gt_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2float);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ half* uint32_t)>(&asc_half2float_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int16_rd);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint32_t)>(&asc_half2int16_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int16_rn);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint32_t)>(&asc_half2int16_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int16_rna);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint32_t)>(&asc_half2int16_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int16_ru);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint32_t)>(&asc_half2int16_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int16_rz);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ half* uint32_t)>(&asc_half2int16_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int32_rd);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint32_t)>(&asc_half2int32_rd_sync);
}

static void test_host_c_api_vector_compute_5()
{
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int32_rn);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint32_t)>(&asc_half2int32_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int32_rna);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint32_t)>(&asc_half2int32_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int32_ru);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint32_t)>(&asc_half2int32_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int32_rz);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ half* uint32_t)>(&asc_half2int32_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int4);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int4_rd);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint32_t)>(&asc_half2int4_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int4_rn);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint32_t)>(&asc_half2int4_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int4_rna);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint32_t)>(&asc_half2int4_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int4_ru);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint32_t)>(&asc_half2int4_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int4_rz);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint32_t)>(&asc_half2int4_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int4b_t* __ubuf__ half* uint32_t)>(&asc_half2int4_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int8);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int8_rd);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint32_t)>(&asc_half2int8_rd_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int8_rn);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint32_t)>(&asc_half2int8_rn_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int8_rna);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint32_t)>(&asc_half2int8_rna_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int8_ru);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint32_t)>(&asc_half2int8_ru_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2int8_rz);
}

static void test_host_c_api_vector_compute_6()
{
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint32_t)>(&asc_half2int8_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* uint32_t)>(&asc_half2int8_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2uint8);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2uint8_rd);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint32_t)>(&asc_half2uint8_rd_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2uint8_rn);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint32_t)>(&asc_half2uint8_rn_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2uint8_rna);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint32_t)>(&asc_half2uint8_rna_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2uint8_ru);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint32_t)>(&asc_half2uint8_ru_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_half2uint8_rz);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint32_t)>(&asc_half2uint8_rz_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* uint32_t)>(&asc_half2uint8_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162float);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int16_t* uint32_t)>(&asc_int162float_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162half);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162half_rd);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint32_t)>(&asc_int162half_rd_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162half_rn);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint32_t)>(&asc_int162half_rn_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162half_rna);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint32_t)>(&asc_int162half_rna_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162half_ru);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint32_t)>(&asc_int162half_ru_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int162half_rz);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint32_t)>(&asc_int162half_rz_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int16_t* uint32_t)>(&asc_int162half_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rd);
}

static void test_host_c_api_vector_compute_7()
{
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rd_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rn);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rn_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rna);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rna_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_ru);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_ru_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rz);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_rz_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int32_t* uint32_t)>(&asc_int322float_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int32_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int322int16);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int32_t* uint32_t)>(&asc_int322int16_sync);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ int32_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int322int64);
    (void)static_cast<void(*)(__ubuf__ int64_t* __ubuf__ int32_t* uint32_t)>(&asc_int322int64_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int4b_t* uint8_t uint16_t uint16_t uint8_t uint8_t)>(&asc_int42half);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int4b_t* uint32_t)>(&asc_int42half_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int642float_rd);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint32_t)>(&asc_int642float_rd_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int642float_rn);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint32_t)>(&asc_int642float_rn_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int642float_rna);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint32_t)>(&asc_int642float_rna_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int642float_ru);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint32_t)>(&asc_int642float_ru_sync);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int642float_rz);
    (void)static_cast<void(*)(__ubuf__ float* __ubuf__ int64_t* uint32_t)>(&asc_int642float_rz_sync);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ int64_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int642int32);
    (void)static_cast<void(*)(__ubuf__ int32_t* __ubuf__ int64_t* uint32_t)>(&asc_int642int32_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int8_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_int82half);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ int8_t* uint32_t)>(&asc_int82half_sync);
}

static void test_host_c_api_vector_compute_8()
{
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_le);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_le_scalar);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_le_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_le_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_leakyrelu);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_leakyrelu_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_log);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_log_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_lt);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_lt_scalar);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_lt_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_lt_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_max);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_max_scalar);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_max_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_max_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_min);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_min_scalar);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_min_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_min_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN] uint8_t uint16_t uint16_t uint16_t uint16_t bool uint8_t)>(&asc_mrgsort4);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN] uint8_t uint16_t uint16_t uint16_t uint16_t bool uint8_t)>(&asc_mrgsort4_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul_add);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_mul_add_relu);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul_add_relu_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul_add_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_mul_cast_half2int8);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul_cast_half2int8_sync);
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_mul_cast_half2uint8);
}

static void test_host_c_api_vector_compute_9()
{
    (void)static_cast<void(*)(__ubuf__ uint8_t* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul_cast_half2uint8_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_mul_scalar);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_mul_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_mul_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_ne);
    (void)static_cast<void(*)(__ubuf__ uint8_t *dst __ubuf__ half *src half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_ne_scalar);
    (void)static_cast<void(*)(__ubuf__ uint8_t *dst __ubuf__ half *src half uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_ne_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_ne_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_not);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_not_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_or);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* __ubuf__ int16_t* uint32_t)>(&asc_or_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_pair_reduce_sum);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_pair_reduce_sum_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_rcp);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_rcp_sync);
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* __ubuf__ uint16_t* uint16_t uint8_t uint8_t uint16_t uint8_t)>(&asc_reduce);
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* __ubuf__ uint16_t* uint32_t)>(&asc_reduce_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_relu);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_relu_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_max_index_value);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_max_index_value_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_max_only_index);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_max_only_index_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_max_only_value);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_max_only_value_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_max_value_index);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_max_value_index_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_min_index_value);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_min_index_value_sync);
}

static void test_host_c_api_vector_compute_10()
{
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_min_only_index);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_min_only_index_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_min_only_value);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_min_only_value_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_min_value_index);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_min_value_index_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t)>(&asc_repeat_reduce_sum);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_repeat_reduce_sum_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_rsqrt);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_rsqrt_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_select);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_select_sync);
    (void)static_cast<void(*)(__ubuf__ void*)>(&asc_set_cmp_mask);
    (void)static_cast<void(*)(float int16_t bool)>(&asc_set_deq_scale);
    (void)static_cast<void(*)()>(&asc_set_mask_count);
    (void)static_cast<void(*)()>(&asc_set_mask_norm);
    (void)static_cast<void(*)(ub_addr8_t __ubuf__ int8_t**)>(&asc_set_va_reg);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_vector_mask);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* uint32_t uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_shiftleft);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* uint32_t uint32_t)>(&asc_shiftleft_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* int32_t uint32_t)>(&asc_shiftright);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* int32_t uint32_t)>(&asc_shiftright_round);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* int32_t uint32_t)>(&asc_shiftright_round_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t* int32_t uint32_t)>(&asc_shiftright_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_sqrt);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* uint32_t)>(&asc_sqrt_sync);
    (void)static_cast<void(*)(__ubuf__ uint16_t* __ubuf__ uint16_t* __ubuf__ uint16_t* uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t uint8_t)>(&asc_squeeze);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_sub);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_sub_relu);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_sub_relu_sync);
}

static void test_host_c_api_vector_compute_11()
{
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_sub_scalar);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* half uint32_t)>(&asc_sub_scalar_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ half* __ubuf__ half* uint32_t)>(&asc_sub_sync);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t*)>(&asc_transpose);
    (void)static_cast<void(*)(__ubuf__ int16_t* __ubuf__ int16_t*)>(&asc_transpose_sync);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ uint8_t* uint8_t uint16_t uint16_t uint16_t uint16_t)>(&asc_uint82half);
    (void)static_cast<void(*)(__ubuf__ half* __ubuf__ uint8_t* uint32_t)>(&asc_uint82half_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_vdeq_int162b8_h);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_vdeq_int162b8_h_sync);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint8_t uint16_t uint16_t uint8_t uint8_t)>(&asc_vdeq_int162b8_l);
    (void)static_cast<void(*)(__ubuf__ int8_t* __ubuf__ int16_t* uint32_t)>(&asc_vdeq_int162b8_l_sync);
}

