/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/cube_compute/cube_compute.h"

static void test_host_c_api_cube_compute_0()
{
    (void)static_cast<void(*)()>(&asc_enable_fp8);
    (void)static_cast<void(*)()>(&asc_enable_hf32);
    (void)static_cast<void(*)(uint32_t)>(&asc_enable_hf32_trans);
    (void)static_cast<void(*)()>(&asc_enable_hif8);
    (void)static_cast<uint64_t(*)()>(&asc_get_l0c2gm_prequant);
    (void)static_cast<uint64_t(*)()>(&asc_get_l0c2gm_relu);
    (void)static_cast<uint64_t(*)()>(&asc_get_l0c2gm_unitflag);
    (void)static_cast<void(*)(__cc__ float* __ca__ bfloat16_t* __cb__ bfloat16_t* uint16_t uint16_t uint16_t uint8_t bool bool bool)>(&asc_mmad);
    (void)static_cast<void(*)(__cc__ float* __ca__ fp4x2_e1m2_t* __cb__ fp4x2_e1m2_t* uint16_t uint16_t uint16_t uint8_t bool bool bool)>(&asc_mmad_mx);
    (void)static_cast<void(*)(__cc__ float* __ca__ fp4x2_e1m2_t* __cb__ fp4x2_e1m2_t* uint16_t uint16_t uint16_t uint8_t bool bool bool)>(&asc_mmad_mx_sync);
    (void)static_cast<void(*)(__cc__ int32_t* __ca__ int4b_t* __cb__ int4b_t* uint16_t uint16_t uint16_t uint8_t bool bool bool)>(&asc_mmad_s4);
    (void)static_cast<void(*)(__cc__ int32_t* __ca__ int4b_t* __cb__ int4b_t* uint16_t uint16_t uint16_t uint8_t bool bool bool)>(&asc_mmad_s4_sync);
    (void)static_cast<void(*)(__cc__ int32_t* __ca__ int8_t* __cb__ int8_t* uint16_t uint16_t uint16_t uint8_t bool bool)>(&asc_mmad_sparse);
    (void)static_cast<void(*)(__cc__ int32_t* __ca__ int8_t* __cb__ int8_t* uint16_t uint16_t uint16_t uint8_t bool bool)>(&asc_mmad_sparse_sync);
    (void)static_cast<void(*)(__cc__ float* __ca__ bfloat16_t* __cb__ bfloat16_t* uint16_t uint16_t uint16_t uint8_t bool bool bool)>(&asc_mmad_sync);
    (void)static_cast<void(*)()>(&asc_set_fp32_mode);
    (void)static_cast<void(*)(uint64_t uint64_t bool)>(&asc_set_l0c2gm_config);
    (void)static_cast<void(*)(uint64_t uint64_t uint64_t)>(&asc_set_l0c2gm_nz2nd);
    (void)static_cast<void(*)()>(&asc_set_mmad_direction_m);
    (void)static_cast<void(*)()>(&asc_set_mmad_direction_n);
}

