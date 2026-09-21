/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/reg_base_impl/npu_arch_3510/cube_datamove_composite_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_CUBE_DATAMOVE_COMPOSITE_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_mx and asc_sync)
__aicore__ inline void asc_copy_l12l0a_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_mx(dst, (__cbuf__ void*)src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_mx and asc_sync)
__aicore__ inline void asc_copy_l12l0b_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_mx(dst, (__cbuf__ void*)src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 10. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 11. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 12. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 13. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 14. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 15. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 16. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 17. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 18. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 19. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride,
    uint16_t dst_stride) /* asc_copy_l12l0a_transpose_sync overload */
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 1. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 2. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 3. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 4. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 5. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 6. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 7. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 8. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/* asc_copy_l12l0a_transpose_sync composite path 9. */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 8. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 9. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 10. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 11. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 12. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 13. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 14. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 15. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 16. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 17. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride,
    uint16_t dst_stride) /* asc_copy_l12l0b_transpose_sync overload */
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 1. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 2. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 3. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 4. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 5. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 6. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 7. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 8. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/* asc_copy_l12l0b_transpose_sync composite path 9. */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 4. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 5. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 6. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 7. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 8. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 9. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 10. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 11. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/* asc_copy_l12l0b_trans_sync composite path 12. */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap,
            src_frac_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    /* asc_copy_l12l0a_sync composite path 20. */
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}
/* asc_copy_l12l0a_sync composite path 36. */

/* asc_copy_l12l0a_sync composite path 21. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
    /* asc_copy_l12l0a_sync composite path 22. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 23. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
/* asc_copy_l12l0a_sync composite path 37. */
{
    /* asc_copy_l12l0a_sync composite path 24. */
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 38. */
__aicore__ inline void asc_copy_l12l0a_sync(
    /* asc_copy_l12l0a_sync composite path 25. */
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        /* asc_copy_l12l0a_sync composite path 39. */
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 26. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 27. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
/* asc_copy_l12l0a_sync composite path 40. */
{
    /* asc_copy_l12l0a_sync composite path 28. */
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/* asc_copy_l12l0a_sync composite path 41. */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    /* asc_copy_l12l0a_sync composite path 29. */
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 42. */
    }
    /* asc_copy_l12l0a_sync composite path 30. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    /* asc_copy_l12l0a_sync composite path 43. */
    if ASC_IS_AIC {
        /* asc_copy_l12l0a_sync composite path 31. */
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    /* asc_copy_l12l0a_sync composite path 44. */
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    /* asc_copy_l12l0a_sync composite path 32. */
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 45. */
    }
    /* asc_copy_l12l0a_sync composite path 33. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 34. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0a_sync composite path 35. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
/* asc_copy_l12l0b_sync composite path 31. */
{
    /* asc_copy_l12l0b_sync composite path 18. */
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 32. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
    /* asc_copy_l12l0b_sync composite path 19. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 20. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
/* asc_copy_l12l0b_sync composite path 33. */
{
    /* asc_copy_l12l0b_sync composite path 21. */
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 34. */
__aicore__ inline void asc_copy_l12l0b_sync(
    /* asc_copy_l12l0b_sync composite path 22. */
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        /* asc_copy_l12l0b_sync composite path 35. */
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 23. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 24. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
/* asc_copy_l12l0b_sync composite path 36. */
{
    /* asc_copy_l12l0b_sync composite path 25. */
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 37. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 26. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
/* asc_copy_l12l0b_sync composite path 38. */
{
    /* asc_copy_l12l0b_sync composite path 27. */
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/* asc_copy_l12l0b_sync composite path 39. */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 28. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 29. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
        asc_sync_post_process();
        /* asc_copy_l12l0b_sync composite path 30. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{ /* asc_copy_gm2l1_align_sync overload */
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/* asc_copy_gm2l1_align_sync composite path 1. */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 2. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 3. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 4. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 5. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 6. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 7. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 8. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
        asc_sync_post_process();
    }
    /* asc_copy_gm2l1_align_sync composite path 9. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{ /* asc_copy_gm2l1_dn2nz_sync overload */
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 1. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 2. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 3. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 4. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 5. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 6. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 7. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 8. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_dn2nz_sync composite path 9. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/* asc_copy_gm2l1_nd2nz_sync composite path 6. */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 7. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 8. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 9. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 10. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 11. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 12. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 13. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 14. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_nd2nz_sync composite path 15. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(dst, src, 0, n_burst, len_burst, pad_func_mode, 0, src_stride, dst_stride);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12fb and asc_sync)
__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_fbuf(
            /* asc_copy_l12fb_sync composite path 1. */
            dst, src, static_cast<uint16_t>(1), size / ASC_C_API_ONE_DATABLOCK_SIZE, static_cast<uint16_t>(0),
            static_cast<uint16_t>(0));
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12ub and asc_sync)
__aicore__ inline void asc_copy_l12ub_sync(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst,
    uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_ubuf(dst_addr, src_addr, static_cast<int8_t>(sub_blockid), n_burst, len_burst, src_gap, dst_gap);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(
            /* asc_copy_l12bt_sync composite path 1. */
            dst, src, static_cast<uint16_t>(0), static_cast<uint16_t>(1), size / ASC_C_API_ONE_DATABLOCK_SIZE,
            static_cast<uint16_t>(0), static_cast<uint16_t>(0));
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{ /* asc_copy_l12bt_sync overload */
    if ASC_IS_AIC {
        copy_cbuf_to_bt(
            dst, src, static_cast<uint16_t>(0), static_cast<uint16_t>(1), size / ASC_C_API_ONE_DATABLOCK_SIZE,
            static_cast<uint16_t>(0), static_cast<uint16_t>(0));
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(
            dst, src, static_cast<uint16_t>(0), static_cast<uint16_t>(1), size / ASC_C_API_ONE_DATABLOCK_SIZE,
            static_cast<uint16_t>(0), static_cast<uint16_t>(0));
        asc_sync_post_process();
    }
}

/* asc_copy_l12bt_sync composite path 2. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(
            dst, src, static_cast<uint16_t>(0), static_cast<uint16_t>(1), size / ASC_C_API_ONE_DATABLOCK_SIZE,
            static_cast<uint16_t>(0), static_cast<uint16_t>(0));
        asc_sync_post_process();
    }
}

/* asc_copy_l12bt_sync composite path 3. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(
            dst, src, static_cast<uint16_t>(0), static_cast<uint16_t>(1), size / ASC_C_API_ONE_DATABLOCK_SIZE,
            static_cast<uint16_t>(0), static_cast<uint16_t>(0));
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            /* asc_copy_l0c2l1_sync composite path 7. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            /* asc_copy_l0c2l1_sync composite path 8. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            /* asc_copy_l0c2gm_sync composite path 9. */
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 10. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            /* asc_copy_l0c2gm_sync composite path 11. */
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            /* asc_copy_l0c2gm_sync composite path 12. */
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 13. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 14. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 15. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            /* asc_copy_l0c2gm_sync composite path 16. */
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 17. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 18. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            /* asc_copy_l0c2gm_sync composite path 19. */
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            /* asc_copy_l0c2gm_sync composite path 20. */
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            /* asc_copy_l0c2gm_sync composite path 21. */
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            /* asc_copy_l0c2gm_sync composite path 22. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 1. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{ /* asc_copy_l0c2ub_sync overload */
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}
/* asc_copy_l0c2ub_sync composite path 2. */

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 3. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 4. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 5. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 6. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 7. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 8. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 9. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 10. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 11. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            /* asc_copy_l0c2ub_sync composite path 12. */
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
            /* asc_copy_l0c2ub_sync composite path 13. */
            enable_clip_relu_pre, unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode,
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post,
            false, eltwise_op, eltwise_antq_en, false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
            /* asc_copy_l0c2ub_sync composite path 14. */
            enable_clip_relu_pre, unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode,
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post,
            false, eltwise_op, eltwise_antq_en, false, c0_pad_en, false, broadcast_en, enable_nz2dn);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/* asc_copy_gm2l1_sync composite path 3. */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 4. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 5. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 6. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 7. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 8. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 9. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 10. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 11. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 12. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 13. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

/* asc_copy_gm2l1_sync composite path 14. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        /* asc_fill_l1_sync composite path 1. */
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        /* asc_fill_l1_sync composite path 15. */
        asc_sync_post_process();
        /* asc_fill_l1_sync composite path 2. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
        /* asc_fill_l1_sync composite path 16. */
    }
    /* asc_fill_l1_sync composite path 3. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
    /* asc_fill_l1_sync composite path 17. */
}
/* asc_fill_l1_sync composite path 4. */

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
}
/* asc_fill_l1_sync composite path 18. */

/* asc_fill_l1_sync composite path 5. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
}

/* asc_fill_l1_sync composite path 19. */
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
/* asc_fill_l1_sync composite path 6. */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
/* asc_fill_l1_sync composite path 20. */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
/* asc_fill_l1_sync composite path 7. */
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
/* asc_fill_l1_sync composite path 21. */
{
    /* asc_fill_l1_sync composite path 8. */
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    /* asc_fill_l1_sync composite path 22. */
    if ASC_IS_AIC {
        /* asc_fill_l1_sync composite path 9. */
        create_cbuf_matrix(dst, config.config, value);
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        /* asc_fill_l1_sync composite path 23. */
        create_cbuf_matrix(dst, config.config, value);
        /* asc_fill_l1_sync composite path 10. */
        asc_sync_post_process();
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
        /* asc_fill_l1_sync composite path 24. */
        asc_sync_post_process();
        /* asc_fill_l1_sync composite path 11. */
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_bf16(dst, config.config, value);
        asc_sync_post_process();
        /* asc_fill_l1_sync composite path 25. */
    }
    /* asc_fill_l1_sync composite path 12. */
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_h(dst, config.config, value);
        asc_sync_post_process();
    }
    /* asc_fill_l1_sync composite path 26. */
}
/* asc_fill_l1_sync composite path 13. */

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_ui(dst, config.config, value);
        asc_sync_post_process();
    }
}
/* asc_fill_l1_sync composite path 27. */

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
/* asc_fill_l1_sync composite path 14. */
#endif
