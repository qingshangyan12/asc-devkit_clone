/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_CUBE_DATAMOVE_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_CUBE_DATAMOVE_C_API_IMPL_H
 
#include "get_fpc_impl/asc_2201/get_fpc_impl.h"
#include "set_fmatrix_impl/asc_2201/set_fmatrix_impl.h"
#include "set_fmatrix_b_impl/asc_2201/set_fmatrix_b_impl.h"
#include "set_fpc_impl/asc_2201/set_fpc_impl.h"
#include "set_l3d_rpt_impl/asc_2201/set_l3d_rpt_impl.h"
#include "set_nd_para_impl/asc_2201/set_nd_para_impl.h"
#include "set_padding_impl/asc_2201/set_padding_impl.h"
#include "set_quant_pre_impl/asc_2201/set_quant_pre_impl.h"
#include "set_mm_layout_transform_impl/asc_2201/set_mm_layout_transform_impl.h"
#include "set_l3d_rpt_impl/asc_2201/set_l3d_rpt_impl.h"
#include "set_l0c_copy_params_impl/npu_arch_2201/asc_set_l0c_copy_params_impl.h"
#include "set_l1_3d_size_impl/npu_arch_2201/asc_set_l1_3d_size_impl.h"
#include "load_image_to_cbuf_impl/npu_arch_2201/asc_load_image_to_cbuf_impl.h"
#include "fill_value_c_api_impl/fill_l0a_value_impl/npu_arch_2201/asc_fill_l0a_value_impl.h"
#include "fill_value_c_api_impl/fill_l0b_value_impl/npu_arch_2201/asc_fill_l0b_value_impl.h"
#include "fill_value_c_api_impl/fill_l1_value_impl/npu_arch_2201/asc_fill_l1_value_impl.h"

__aicore__ inline void asc_set_mm_layout_transform(bool mmLayoutMode)
{
    CApiInternal::set_mm_layout_transform_impl(mmLayoutMode);
}

__aicore__ inline void asc_set_l1_3d_rpt(uint64_t value)
{
    CApiInternal::set_l1_3d_rpt_impl(value);
}

// ==========asc_l0c_copy_params()==========
__aicore__ inline void asc_set_l0c_copy_params(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)
{
    CApiInternal::set_l0c_copy_params_impl(ndNum, srcNdStride, dstNdStride);
}

__aicore__ inline void asc_set_l1_3d_size(uint64_t value)
{
    asc_set_l1_3d_size_impl(value);
}

// ==========asc_load_image_to_cbuf(half/int8_t)==========
__aicore__ inline void asc_load_image_to_cbuf(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
{
    asc_load_image_to_cbuf_impl(dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_res, top_pad_size,
                                bot_pad_size, left_pad_size, right_pad_size, sid);
}

__aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
{
    asc_load_image_to_cbuf_sync_impl(dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_res, top_pad_size,
                                     bot_pad_size, left_pad_size, right_pad_size, sid);
}

__aicore__ inline void asc_load_image_to_cbuf(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
{
    asc_load_image_to_cbuf_impl(dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_res, top_pad_size,
                                bot_pad_size, left_pad_size, right_pad_size, sid);
}

__aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
{
    asc_load_image_to_cbuf_sync_impl(dst, hor_size, ver_size, hor_start_pos, ver_start_pos, src_hor_res, top_pad_size,
                                     bot_pad_size, left_pad_size, right_pad_size, sid);
}

// ==========asc_fill_l0a_value(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l0a_value(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_sync_impl(dst, value, config);
}

// ==========asc_fill_l0b_value(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l0b_value(__cb__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0b_value_sync_impl(dst, value, config);
}

// ==========asc_fill_l1_value(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l1_value(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_sync_impl(dst, value, config);
}

#endif