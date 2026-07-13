/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/cube_datamove/cube_datamove.h"

static void test_host_c_api_cube_datamove_0()
{
    (void)static_cast<void(*)(__ca__ bfloat16_t* __gm__ bfloat16_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_gm2l0a);
    (void)static_cast<void(*)(__ca__ bfloat16_t* __gm__ bfloat16_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_gm2l0a_sync);
    (void)static_cast<void(*)(__cb__ bfloat16_t* __gm__ bfloat16_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_gm2l0b);
    (void)static_cast<void(*)(__cb__ bfloat16_t* __gm__ bfloat16_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_gm2l0b_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1);
    (void)static_cast<void(*)(__cbuf__ bfloat16_t* __gm__ bfloat16_t* uint32_t uint32_t uint8_t uint8_t bool uint8_t uint64_t uint32_t)>(&asc_copy_gm2l1_align);
    (void)static_cast<void(*)(__cbuf__ bfloat16_t* __gm__ bfloat16_t* uint32_t uint32_t uint8_t uint8_t bool uint8_t uint64_t uint32_t)>(&asc_copy_gm2l1_align_sync);
    (void)static_cast<void(*)(__cbuf__ bfloat16_t* __gm__ bfloat16_t* uint64_t uint8_t uint16_t uint32_t uint64_t bool)>(&asc_copy_gm2l1_dn2nz);
    (void)static_cast<void(*)(__cbuf__ bfloat16_t* __gm__ bfloat16_t* uint64_t uint8_t uint16_t uint32_t uint64_t bool)>(&asc_copy_gm2l1_dn2nz_sync);
    (void)static_cast<void(*)(__cbuf__ int8_t* __gm__ int8_t* uint16_t uint16_t uint16_t uint16_t uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_nd2nz);
    (void)static_cast<void(*)(__cbuf__ int8_t* __gm__ int8_t* uint16_t uint16_t uint16_t uint16_t uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_nd2nz_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad1);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad1_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad2);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad2_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad3);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad3_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad4);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad4_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad5);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad5_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad6);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad6_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad7);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad7_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2l1_pad8);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_pad8_sync);
    (void)static_cast<void(*)(__cbuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2l1_sync);
    (void)static_cast<void(*)(__gm__ half* __cc__ float* uint16_t uint16_t uint32_t uint16_t uint8_t uint64_t uint8_t bool bool)>(&asc_copy_l0c2gm);
    (void)static_cast<void(*)(__gm__ half* __cc__ float* uint16_t uint16_t uint32_t uint16_t uint8_t uint64_t uint8_t bool bool)>(&asc_copy_l0c2gm_sync);
}

static void test_host_c_api_cube_datamove_1()
{
    (void)static_cast<void(*)(__cbuf__ half* __cc__ float* uint16_t uint16_t uint32_t uint16_t uint8_t uint64_t uint8_t bool bool)>(&asc_copy_l0c2l1);
    (void)static_cast<void(*)(__cbuf__ half* __cc__ float* uint16_t uint16_t uint32_t uint16_t uint8_t uint64_t uint8_t bool bool)>(&asc_copy_l0c2l1_sync);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t *dst_addr __cc__ float *src_addr uint16_t uint16_t uint32_t uint16_t uint8_t bool uint8_t uint8_t uint64_t uint8_t bool bool uint64_t uint8_t bool uint8_t bool bool bool bool)>(&asc_copy_l0c2ub);
    (void)static_cast<void(*)(__ubuf__ bfloat16_t* __cc__ float* uint16_t uint16_t uint32_t uint16_t uint8_t bool uint8_t uint8_t uint64_t uint8_t bool bool uint64_t uint8_t bool uint8_t bool bool bool bool)>(&asc_copy_l0c2ub_sync);
    (void)static_cast<void(*)(uint64_t __cbuf__ void* uint16_t uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12bt);
    (void)static_cast<void(*)(uint64_t __cbuf__ void* uint32_t)>(&asc_copy_l12bt_sync);
    (void)static_cast<void(*)(__fbuf__ void* __cbuf__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12fb);
    (void)static_cast<void(*)(__fbuf__ void* __cbuf__ void* uint32_t)>(&asc_copy_l12fb_sync);
    (void)static_cast<void(*)(__gm__ void* __cbuf__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12gm);
    (void)static_cast<void(*)(__gm__ void* __cbuf__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12gm_sync);
    (void)static_cast<void(*)(__ca__ int4b_t* __cbuf__ int4b_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0a);
    (void)static_cast<void(*)(uint64_t __cbuf__ fp8_e8m0_t* uint16_t uint16_t uint8_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0a_mx);
    (void)static_cast<void(*)(uint64_t __cbuf__ fp8_e8m0_t* uint16_t uint16_t uint8_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0a_mx_sync);
    (void)static_cast<void(*)(__ca__ int4b_t* __cbuf__ int4b_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0a_sync);
    (void)static_cast<void(*)(__ca__ half* __cbuf__ half* uint16_t uint8_t uint16_t uint16_t bool uint16_t)>(&asc_copy_l12l0a_trans);
    (void)static_cast<void(*)(__ca__ half* __cbuf__ half* uint16_t uint8_t uint16_t uint16_t bool uint16_t)>(&asc_copy_l12l0a_trans_sync);
    (void)static_cast<void(*)(__ca__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint16_t uint8_t uint8_t int16_t uint16_t)>(&asc_copy_l12l0a_transpose);
    (void)static_cast<void(*)(__ca__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint16_t uint8_t uint8_t int16_t uint16_t)>(&asc_copy_l12l0a_transpose_sync);
    (void)static_cast<void(*)(__cb__ int4b_t* __cbuf__ int4b_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0b);
    (void)static_cast<void(*)(uint64_t __cbuf__ fp8_e8m0_t* uint16_t uint16_t uint8_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0b_mx);
    (void)static_cast<void(*)(uint64_t __cbuf__ fp8_e8m0_t* uint16_t uint16_t uint8_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0b_mx_sync);
    (void)static_cast<void(*)(__cb__ int8_t* __cbuf__ int8_t* __cbuf__ int8_t* uint16_t uint8_t)>(&asc_copy_l12l0b_sparse);
    (void)static_cast<void(*)(__cb__ int8_t* __cbuf__ int8_t* __cbuf__ int8_t* uint16_t uint8_t)>(&asc_copy_l12l0b_sparse_sync);
    (void)static_cast<void(*)(__cb__ int4b_t* __cbuf__ int4b_t* uint16_t uint8_t uint16_t uint16_t)>(&asc_copy_l12l0b_sync);
    (void)static_cast<void(*)(__cb__ half* __cbuf__ half* uint16_t uint8_t uint16_t uint16_t bool uint16_t)>(&asc_copy_l12l0b_trans);
    (void)static_cast<void(*)(__cb__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint8_t uint16_t uint16_t bool uint16_t)>(&asc_copy_l12l0b_trans_sync);
    (void)static_cast<void(*)(__cb__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint16_t uint8_t uint8_t int16_t uint16_t)>(&asc_copy_l12l0b_transpose);
    (void)static_cast<void(*)(__cb__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint16_t uint8_t uint8_t int16_t uint16_t)>(&asc_copy_l12l0b_transpose_sync);
    (void)static_cast<void(*)(__cc__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12l0c);
    (void)static_cast<void(*)(__cc__ bfloat16_t* __cbuf__ bfloat16_t* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12l0c_sync);
}

static void test_host_c_api_cube_datamove_2()
{
    (void)static_cast<void(*)(__ubuf__ void * __cbuf__ void * bool uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12ub);
    (void)static_cast<void(*)(__ubuf__ void * __cbuf__ void * bool uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_l12ub_sync);
    (void)static_cast<void(*)(__ca__ half* half const asc_fill_value_config&)>(&asc_fill_l0a);
    (void)static_cast<void(*)(__ca__ half* half const asc_fill_value_config&)>(&asc_fill_l0a_sync);
    (void)static_cast<void(*)(__cb__ half* half const asc_fill_value_config&)>(&asc_fill_l0b);
    (void)static_cast<void(*)(__cb__ half* half const asc_fill_value_config&)>(&asc_fill_l0b_sync);
    (void)static_cast<void(*)(__cbuf__ half* half const asc_fill_value_config&)>(&asc_fill_l1);
    (void)static_cast<void(*)(__cbuf__ half* half const asc_fill_value_config&)>(&asc_fill_l1_sync);
    (void)static_cast<void(*)(__cbuf__ half* uint16_t uint16_t uint16_t uint16_t uint16_t uint8_t uint8_t uint16_t uint16_t)>(&asc_load_image_to_cbuf);
    (void)static_cast<void(*)(__cbuf__ half* uint16_t uint16_t uint16_t uint16_t uint16_t uint8_t uint8_t uint16_t uint16_t)>(&asc_load_image_to_cbuf_sync);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_gm2l1_loop1_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_gm2l1_loop2_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_gm2l1_loop_size);
    (void)static_cast<void(*)(uint32_t)>(&asc_set_gm2l1_pad);
    (void)static_cast<void(*)(half&)>(&asc_set_l0c2gm_lrelu_alpha);
    (void)static_cast<void(*)(uint16_t uint16_t uint16_t)>(&asc_set_l0c_copy_params);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l0c_copy_prequant);
    (void)static_cast<void(*)(asc_l13d_fmatrix_config&)>(&asc_set_l13d_fmatrix);
    (void)static_cast<void(*)(asc_l13d_fmatrix_config&)>(&asc_set_l13d_fmatrix_b);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l13d_padding);
    (void)static_cast<void(*)(asc_load3d_v2_config&)>(&asc_set_l13d_rpt);
    (void)static_cast<void(*)(uint64_t)>(&asc_set_l13d_size);
}

