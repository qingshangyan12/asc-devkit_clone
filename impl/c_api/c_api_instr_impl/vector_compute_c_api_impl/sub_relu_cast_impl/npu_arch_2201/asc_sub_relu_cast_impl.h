/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file add_impl.h
 * \brief
 */
#ifndef IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_SUB_RELU_CAST_IMPL_NPU_ARCH_2201_ASC_SUB_RELU_CAST_IMPL_H
#define IMPL_CAPI_C_API_INSTR_IMPL_VECTOR_COMPUTE_C_API_IMPL_SUB_RELU_CAST_IMPL_NPU_ARCH_2201_ASC_SUB_RELU_CAST_IMPL_H

namespace CApiInternal {

__aicore__ inline void asc_sub_relu_castf162s8_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
{
    vsubreluconv_f162s8(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride), false);
}

__aicore__ inline void asc_sub_relu_castf162s8_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_binary_config config{};
    config.dst_block_stride = 1;
    config.src0_block_stride = 1;
    config.src1_block_stride = 1;
    config.dst_repeat_stride = 8;
    config.src0_repeat_stride = 4;
    config.src1_repeat_stride = 4;
    config.repeat = 1;
    asc_sub_relu_castf162s8_impl(dst, src0, src1, config);
    set_mask_norm();
}

__aicore__ inline void asc_sub_relu_castf162s8_sync_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_castf162s8_impl(dst, src0, src1, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_sub_relu_castf322f16_impl(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
{
    vsubreluconv_f322f16(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride), false);
}

__aicore__ inline void asc_sub_relu_castf322f16_impl(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_binary_config config{};
    config.dst_block_stride = 1;
    config.src0_block_stride = 1;
    config.src1_block_stride = 1;
    config.dst_repeat_stride = 8;
    config.src0_repeat_stride = 4;
    config.src1_repeat_stride = 4;
    config.repeat = 1;
    asc_sub_relu_castf322f16_impl(dst, src0, src1, config);
    set_mask_norm();
}

__aicore__ inline void asc_sub_relu_castf322f16_sync_impl(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_castf322f16_impl(dst, src0, src1, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_sub_relu_casts162s8_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    vsubreluconv_s162s8(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride), false);
}

__aicore__ inline void asc_sub_relu_casts162s8_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
    asc_binary_config config{};
    config.dst_block_stride = 1;
    config.src0_block_stride = 1;
    config.src1_block_stride = 1;
    config.dst_repeat_stride = 8;
    config.src0_repeat_stride = 4;
    config.src1_repeat_stride = 4;
    config.repeat = 1;
    asc_sub_relu_casts162s8_impl(dst, src0, src1, config);
    set_mask_norm();
}

__aicore__ inline void asc_sub_relu_casts162s8_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_casts162s8_impl(dst, src0, src1, count);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_sub_relu_cast_vdeqs162b8_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    vsubreluconv_vdeqs162b8(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride), true);
}

__aicore__ inline void asc_sub_relu_cast_vdeqs162b8_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, const asc_binary_config& config)
{
    vsubreluconv_vdeqs162b8(dst, src0, src1, static_cast<uint8_t>(config.repeat), static_cast<uint8_t>(config.dst_block_stride),
         static_cast<uint8_t>(config.src0_block_stride), static_cast<uint8_t>(config.src1_block_stride),
         static_cast<uint8_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.src0_repeat_stride),
         static_cast<uint8_t>(config.src1_repeat_stride), true);
}

}

#endif