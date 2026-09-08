/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/vector_compute_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"

// asc_set_vector_mask_impl.h
__aicore__ inline void asc_set_vector_mask_impl(uint64_t mask_high, uint64_t mask_low)
{
    if ASC_IS_AIV {
        set_vector_mask(mask_high, mask_low);
    }
}

// asc_bitsort_impl.h
__aicore__ inline void asc_bitsort_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    if ASC_IS_AIV {
        uint64_t config = (static_cast<uint64_t>(repeat) & 0xff) << 56;
        vbs(dst, src0, src1, config);
    }
}

__aicore__ inline void asc_bitsort_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    if ASC_IS_AIV {
        uint64_t config = (static_cast<uint64_t>(repeat) & 0xff) << 56;
        vbs(dst, src0, src1, config);
    }
}

// asc_mrgsort4_impl.h
__aicore__ inline void asc_mrgsort4_impl(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    if ASC_IS_AIV {
        vmrgsort4(
            dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
            if_exhausted_suspension, valid_bit);
    }
}

__aicore__ inline void asc_mrgsort4_impl(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    if ASC_IS_AIV {
        vmrgsort4(
            dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
            if_exhausted_suspension, valid_bit);
    }
}

// asc_transpose_impl.h
__aicore__ inline void asc_transpose_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vtranspose(dst, src);
    }
}

__aicore__ inline void asc_transpose_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vtranspose(dst, src);
    }
}

// asc_transto5hd_impl.h
#define ASC_TRANSTO5HD_B8_CALL(dst_va, src_va, repeat, dst_stride, src_stride, dst_high_half, src_high_half) \
    do {                                                                                                     \
        if (!(dst_high_half)) {                                                                              \
            if (!(src_high_half)) {                                                                          \
                scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, false, false);          \
            } else {                                                                                         \
                scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, false, true);           \
            }                                                                                                \
        } else if (!(src_high_half)) {                                                                       \
            scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, true, false);               \
        } else {                                                                                             \
            scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, true, true);                \
        }                                                                                                    \
    } while (0)

__aicore__ inline void asc_transto5hd_b8_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    if ASC_IS_AIV {
        if (dst == ub_addr8_t::VA0) {
            if (src == ub_addr8_t::VA2) {
                ASC_TRANSTO5HD_B8_CALL(VA0, VA2, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA4) {
                ASC_TRANSTO5HD_B8_CALL(VA0, VA4, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA6) {
                ASC_TRANSTO5HD_B8_CALL(VA0, VA6, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        } else if (dst == ub_addr8_t::VA2) {
            if (src == ub_addr8_t::VA0) {
                ASC_TRANSTO5HD_B8_CALL(VA2, VA0, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA4) {
                ASC_TRANSTO5HD_B8_CALL(VA2, VA4, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA6) {
                ASC_TRANSTO5HD_B8_CALL(VA2, VA6, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        } else if (dst == ub_addr8_t::VA4) {
            if (src == ub_addr8_t::VA0) {
                ASC_TRANSTO5HD_B8_CALL(VA4, VA0, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA2) {
                ASC_TRANSTO5HD_B8_CALL(VA4, VA2, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA6) {
                ASC_TRANSTO5HD_B8_CALL(VA4, VA6, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        } else if (dst == ub_addr8_t::VA6) {
            if (src == ub_addr8_t::VA0) {
                ASC_TRANSTO5HD_B8_CALL(VA6, VA0, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA2) {
                ASC_TRANSTO5HD_B8_CALL(VA6, VA2, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA4) {
                ASC_TRANSTO5HD_B8_CALL(VA6, VA4, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        }
    }
}

#undef ASC_TRANSTO5HD_B8_CALL

__aicore__ inline void asc_transto5hd_b16_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    if ASC_IS_AIV {
        if (dst == ub_addr8_t::VA0) {
            if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b16(VA0, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b16(VA0, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b16(VA0, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA2) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b16(VA2, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b16(VA2, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b16(VA2, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA4) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b16(VA4, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b16(VA4, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b16(VA4, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA6) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b16(VA6, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b16(VA6, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b16(VA6, VA4, repeat, dst_stride, src_stride);
            }
        }
    }
}

__aicore__ inline void asc_transto5hd_b32_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    if ASC_IS_AIV {
        if (dst == ub_addr8_t::VA0) {
            if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b32(VA0, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b32(VA0, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b32(VA0, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA2) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b32(VA2, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b32(VA2, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b32(VA2, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA4) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b32(VA4, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b32(VA4, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b32(VA4, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA6) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b32(VA6, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b32(VA6, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b32(VA6, VA4, repeat, dst_stride, src_stride);
            }
        }
    }
}

// asc_set_va_reg_impl.h
__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ int8_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ uint8_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ int16_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ uint16_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ half** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ int32_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ uint32_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ float** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

// vector_compute_impl.h
// ==========asc_set_vector_mask==========
__aicore__ inline void asc_set_vector_mask(uint64_t mask_high, uint64_t mask_low)
{
    asc_set_vector_mask_impl(mask_high, mask_low);
}

// ==========asc_bitsort(half/float)==========
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl(dst, src0, src1, repeat);
}

__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl(dst, src0, src1, repeat);
}

// ==========asc_mrgsort4(half/float)==========
__aicore__ inline void asc_mrgsort4(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

__aicore__ inline void asc_mrgsort4(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

// ==========asc_transpose(int16_t/uint16_t)==========
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src) { asc_transpose_impl(dst, src); }

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src) { asc_transpose_impl(dst, src); }

// ==========asc_transto5hd(b8/b16/b32)==========
__aicore__ inline void asc_transto5hd_b8(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    asc_transto5hd_b8_impl(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
}

__aicore__ inline void asc_transto5hd_b16(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b16_impl(dst, src, repeat, dst_stride, src_stride);
}

__aicore__ inline void asc_transto5hd_b32(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b32_impl(dst, src, repeat, dst_stride, src_stride);
}

// ===================asc_set_va_reg=====================
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
