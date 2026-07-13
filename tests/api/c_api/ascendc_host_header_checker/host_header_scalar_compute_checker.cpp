/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/scalar_compute/scalar_compute.h"

static void test_host_c_api_scalar_compute_0()
{
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_add);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_and);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t int32_t)>(&asc_atomic_cas);
    (void)static_cast<uint32_t(*)(__gm__ uint32_t* uint32_t)>(&asc_atomic_dec);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_exch);
    (void)static_cast<uint32_t(*)(__gm__ uint32_t* uint32_t)>(&asc_atomic_inc);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_max);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_min);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_or);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_sub);
    (void)static_cast<int32_t(*)(__gm__ int32_t* int32_t)>(&asc_atomic_xor);
    (void)static_cast<uint64_t(*)(uint64_t int64_t)>(&asc_clear_nthbit);
    (void)static_cast<int64_t(*)(uint64_t)>(&asc_clz);
    (void)static_cast<int64_t(*)(uint64_t)>(&asc_ffs);
    (void)static_cast<int64_t(*)(uint64_t)>(&asc_ffz);
    (void)static_cast<int32_t(*)(float)>(&asc_float2int32_rd);
    (void)static_cast<int32_t(*)(float)>(&asc_float2int32_rn);
    (void)static_cast<int32_t(*)(float)>(&asc_float2int32_rna);
    (void)static_cast<int32_t(*)(float)>(&asc_float2int32_ru);
    (void)static_cast<int64_t(*)(uint64_t)>(&asc_popc);
    (void)static_cast<uint64_t(*)(uint64_t int64_t)>(&asc_set_nthbit);
    (void)static_cast<int64_t(*)(int64_t)>(&asc_sflbits);
    (void)static_cast<void(*)(__gm__ int8_t* int8_t)>(&asc_store_dev);
    (void)static_cast<int64_t(*)(uint64_t)>(&asc_zero_bits_cnt);
}

