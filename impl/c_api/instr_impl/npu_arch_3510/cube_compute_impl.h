/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CUBE_COMPUTE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_CUBE_COMPUTE_IMPL_H

#include "instr_impl/npu_arch_3510/cube_compute_impl/asc_set_l0c2gm_config_impl.h"
#include "instr_impl/npu_arch_3510/cube_compute_impl/asc_set_l0c2gm_nz2nd_impl.h"
#include "instr_impl/npu_arch_3510/cube_compute_impl/asc_mmad_mx_impl.h"
#include "instr_impl/npu_arch_3510/cube_compute_impl/asc_mmad_impl.h"

__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre_mode, uint64_t quant_pre_mode, bool enable_unit_flag)
{
    asc_set_l0c2gm_config_impl(relu_pre_mode, quant_pre_mode, enable_unit_flag);
}

__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride)
{
    asc_set_l0c2gm_nz2nd_impl(nd_num, src_nd_stride, dst_nd_stride);
}

// ==========mmad_mx(e1m2/e1m2, e4m3/e5m2)==========
__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
    __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
    __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
    __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
    __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
    __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
    __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
    __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
    __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
    __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
    __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
    __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
    __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e5m2_t* a,
    __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e5m2_t* a,
    __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e5m2_t* a,
    __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e5m2_t* a,
    __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_mx_sync_impl(c, a, b, m, n, k,
        unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
}

// ==========mmad==========
__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ fp8_e4m3fn_t* a, __cb__ fp8_e4m3fn_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
                                     __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n,
                                     uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                     bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ fp8_e4m3fn_t* a, __cb__ fp8_e5m2_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
                                     __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n,
                                     uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                     bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ fp8_e5m2_t* a, __cb__ fp8_e4m3fn_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ fp8_e5m2_t* a,
                                     __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n,
                                     uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                     bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ fp8_e5m2_t* a, __cb__ fp8_e5m2_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ fp8_e5m2_t* a,
                                     __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n,
                                     uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                     bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ half* a, __cb__ half* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ half* a, __cb__ half* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float* a, __cb__ float* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float* a, __cb__ float* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                  enable_cube_init, enable_cube_zero_init);
}

__aicore__ inline void asc_mmad_sync(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init)
{
    asc_mmad_sync_impl(c, a, b, m, n, k, unit_flag, disable_gemv,
                       enable_cube_init, enable_cube_zero_init);
}

#endif