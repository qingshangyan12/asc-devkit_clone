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
 * \file asc_fill_l1_value_impl.h
 * \brief
 */
#ifndef IMPL_C_API_INSTR_CUBE_DMAMOVE_FILL_VALUE_FILL_L1_VALUE_NPU_ARCH_2201_ASC_FILL_L1_VALUE_IMPL_H
#define IMPL_C_API_INSTR_CUBE_DMAMOVE_FILL_VALUE_FILL_L1_VALUE_NPU_ARCH_2201_ASC_FILL_L1_VALUE_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

// asc_fill_l1_value half
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

// asc_fill_l1_value float
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

// asc_fill_l1_value int16_t
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

// asc_fill_l1_value int32_t
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

// asc_fill_l1_value uint16_t
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

// asc_fill_l1_value uint32_t
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

// asc_fill_l1_value bfloat16_t
__aicore__ inline void asc_fill_l1_value_impl(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIC_TYPE))
    create_cbuf_matrix_bf16(dst, config.config, value);
#endif
}

__aicore__ inline void asc_fill_l1_value_sync_impl(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_value_impl(dst, value, config);
    pipe_barrier(pipe_t::PIPE_ALL);
}

#endif