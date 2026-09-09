/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_9201/utils/test_prefetch_l2cache_instr_utils.h"
#include "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_prefetch_l2cache_impl.h"

//================asc_prefetch_gm2l2cache_impl AIC================

TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, int8_t, 1);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, uint8_t, 2);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, hifloat8_t, 3);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, fp8_e5m2_t, 4);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, fp8_e4m3fn_t, 5);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, int16_t, 6);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, uint16_t, 7);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, half, 8);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, bfloat16_t, 9);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, int32_t, 10);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, uint32_t, 11);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIC(PrefetchL2, asc_prefetch_gm2l2cache_impl, float, 12);

//================asc_prefetch_gm2l2cache_impl AIV================
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, int8_t, 101);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, uint8_t, 102);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, hifloat8_t, 103);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, fp8_e5m2_t, 104);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, fp8_e4m3fn_t, 105);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, int16_t, 106);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, uint16_t, 107);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, half, 108);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, bfloat16_t, 109);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, int32_t, 110);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, uint32_t, 111);
TEST_CUBE_DATAMOVE_PREFETCH_GM2L2CACHE_AIV(PrefetchL2, asc_prefetch_gm2l2cache_impl, float, 112);

//================asc_prefetch_gm2l2cache_dn2nz_impl================
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, int8_t, 201);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, uint8_t, 202);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, hifloat8_t, 203);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, fp8_e5m2_t, 204);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, fp8_e4m3fn_t, 205);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, int16_t, 206);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, uint16_t, 207);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, half, 208);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, bfloat16_t, 209);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, int32_t, 210);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, uint32_t, 211);
TEST_CUBE_DATAMOVE_PREFETCH_DN2NZ(PrefetchL2, asc_prefetch_gm2l2cache_dn2nz_impl, float, 212);

//================asc_prefetch_gm2l2cache_nd2nz_impl================
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, int8_t, 301);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, uint8_t, 302);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, hifloat8_t, 303);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, fp8_e5m2_t, 304);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, fp8_e4m3fn_t, 305);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, int16_t, 306);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, uint16_t, 307);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, half, 308);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, bfloat16_t, 309);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, int32_t, 310);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, uint32_t, 311);
TEST_CUBE_DATAMOVE_PREFETCH_ND2NZ(PrefetchL2, asc_prefetch_gm2l2cache_nd2nz_impl, float, 312);

//================asc_prefetch_stop_impl================
TEST_CUBE_DATAMOVE_PREFETCH_STOP(PrefetchL2, asc_prefetch_stop_impl, 401);
