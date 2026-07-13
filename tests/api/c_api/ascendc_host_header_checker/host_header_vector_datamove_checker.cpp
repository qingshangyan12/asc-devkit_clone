/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/vector_datamove/vector_datamove.h"

static void test_host_c_api_vector_datamove_0()
{
    (void)static_cast<void(*)(__ubuf__ void* __gm__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_gm2ub);
    (void)static_cast<void(*)(__ubuf__ int8_t* __gm__ int8_t* uint16_t uint32_t uint8_t uint8_t uint32_t uint32_t)>(&asc_copy_gm2ub_align);
    (void)static_cast<void(*)(__ubuf__ int8_t* __gm__ int8_t* uint32_t)>(&asc_copy_gm2ub_align_sync);
    (void)static_cast<void(*)(__ubuf__ void* __gm__ void* uint32_t)>(&asc_copy_gm2ub_sync);
    (void)static_cast<void(*)(__gm__ void* __ubuf__ void* uint32_t)>(&asc_copy_ub2gm);
    (void)static_cast<void(*)(__gm__ uint8_t* __ubuf__ uint8_t* uint16_t uint32_t uint8_t uint8_t uint32_t uint32_t)>(&asc_copy_ub2gm_align);
    (void)static_cast<void(*)(__gm__ uint8_t* __ubuf__ uint8_t* uint32_t)>(&asc_copy_ub2gm_align_sync);
    (void)static_cast<void(*)(__gm__ void* __ubuf__ void* uint32_t)>(&asc_copy_ub2gm_sync);
    (void)static_cast<void(*)(__cbuf__ void* __ubuf__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_ub2l1);
    (void)static_cast<void(*)(__cbuf__ void* __ubuf__ void* uint32_t)>(&asc_copy_ub2l1_sync);
    (void)static_cast<void(*)(__ubuf__ void* __ubuf__ void* uint16_t uint16_t uint16_t uint16_t)>(&asc_copy_ub2ub);
    (void)static_cast<void(*)(__ubuf__ void* __ubuf__ void* uint32_t)>(&asc_copy_ub2ub_sync);
    (void)static_cast<void(*)()>(&asc_ndim_copy_dci);
    (void)static_cast<void(*)(__ubuf__ int8_t* __gm__ int8_t* uint32_t uint32_t uint32_t uint32_t uint32_t uint8_t uint8_t bool uint8_t)>(&asc_ndim_copy_gm2ub);
    (void)static_cast<void(*)(int8_t)>(&asc_set_copy_pad_val);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_gm2ub_loop1_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_gm2ub_loop2_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_gm2ub_loop_size);
    (void)static_cast<void(*)(uint32_t)>(&asc_set_gm2ub_pad);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ndim_loop0_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ndim_loop1_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ndim_loop2_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ndim_loop3_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ndim_loop4_stride);
    (void)static_cast<void(*)(asc_ndim_pad_count_config&)>(&asc_set_ndim_pad_count);
    (void)static_cast<void(*)(int8_t)>(&asc_set_ndim_pad_value);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ub2gm_loop1_stride);
    (void)static_cast<void(*)(uint64_t uint64_t)>(&asc_set_ub2gm_loop2_stride);
    (void)static_cast<void(*)(uint32_t uint32_t)>(&asc_set_ub2gm_loop_size);
}

