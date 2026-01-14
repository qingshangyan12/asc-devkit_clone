/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl/utils_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl/utils_type.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_sub_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_min_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_neg_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ge_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_reduce_max_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_reduce_min_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_reduce_min_datablock_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_axpy_impl.h"

// ==========asc_sub(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_sub(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_sub_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_sub_impl(carry, dst, src0, src1, mask);
}

__simd_callee__ inline void asc_sub(vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_sub_impl(carry, dst, src0, src1, mask);
}

// ==========asc_min(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_min(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_min(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_min_impl(dst, src0, src1, mask);
}

// ==========asc_neg(int8_t/int16_t/half/int32_t/float)==========
__simd_callee__ inline void asc_neg(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_neg_impl(dst, src, mask);
}

__simd_callee__ inline void asc_neg(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_neg_impl(dst, src, mask);
}

__simd_callee__ inline void asc_neg(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_neg_impl(dst, src, mask);
}

__simd_callee__ inline void asc_neg(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_neg_impl(dst, src, mask);
}

__simd_callee__ inline void asc_neg(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_neg_impl(dst, src, mask);
}

// ==========asc_ge(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int8_t src0, int8_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int16_t src0, int16_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int32_t src0, int32_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint8_t src0, uint8_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint16_t src0, uint16_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint32_t src0, uint32_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_half src0, half src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_bfloat16_t src0, bfloat16_t src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_float src0, float src1, vector_bool mask)
{
    asc_ge_impl(dst, src0, src1, mask);
}

// ==========asc_reduce_max(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_max(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

// ==========asc_reduce_min(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_min(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

// ==========asc_reduce_min_datablock(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_min_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

// ==========asc_axpy(half/float)==========
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src0, half src1, vector_bool mask)
{
    asc_axpy_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_axpy(vector_float& dst, vector_float src0, float src1, vector_bool mask)
{
    asc_axpy_impl(dst, src0, src1, mask);
}
#endif