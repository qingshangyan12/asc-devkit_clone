/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_L12L0B_IMPL_NPU_ARCH_2201_ASC_COPY_L12L0B_2D_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_COPY_L12L0B_IMPL_NPU_ARCH_2201_ASC_COPY_L12L0B_2D_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ int4x2_t* dst, __cbuf__ int4x2_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4((__cb__ void*)dst, (__cbuf__ void*)src,
            start_index, repeat, src_stride, dst_gap, sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ int4x2_t* dst, __cbuf__ int4x2_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ int8_t* dst, __cbuf__ int8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ int8_t* dst, __cbuf__ int8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ uint8_t* dst, __cbuf__ uint8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ uint8_t* dst, __cbuf__ uint8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ int16_t* dst, __cbuf__ int16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ int16_t* dst, __cbuf__ int16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ uint16_t* dst, __cbuf__ uint16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ uint16_t* dst, __cbuf__ uint16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ half* dst, __cbuf__ half* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ half* dst, __cbuf__ half* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ int32_t* dst, __cbuf__ int32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ int32_t* dst, __cbuf__ int32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ uint32_t* dst, __cbuf__ uint32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ uint32_t* dst, __cbuf__ uint32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_impl(__cb__ float* dst, __cbuf__ float* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, start_index, repeat, src_stride, dst_gap,
            sid, transpose, static_cast<addr_cal_mode_t>(addr_mode));
    }
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(__cb__ float* dst, __cbuf__ float* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode)
{
    asc_copy_l12l0b_impl(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
    asc_sync_post_process();
}

#endif