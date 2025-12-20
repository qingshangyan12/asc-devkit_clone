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
 * \file asc_fill_l0a_value_impl.h
 * \brief
 */
#ifndef IMPL_C_API_INSTR_CUBE_DMAMOVE_FILL_VALUE_FILL_L0A_VALUE_NPU_ARCH_2201_ASC_FILL_L0A_VALUE_IMPL_H
#define IMPL_C_API_INSTR_CUBE_DMAMOVE_FILL_VALUE_FILL_L0A_VALUE_NPU_ARCH_2201_ASC_FILL_L0A_VALUE_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

// asc_fill_l0a_value half
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_value_impl(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

// asc_fill_l0a_value float
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_value_impl(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

// asc_fill_l0a_value int16_t
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_value_impl(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

// asc_fill_l0a_value int32_t
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_value_impl(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

// asc_fill_l0a_value uint16_t
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_value_impl(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

// asc_fill_l0a_value uint32_t
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l0a_value_impl(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

// asc_fill_l0a_value bfloat16_t
__aicore__ inline void asc_fill_l0a_value_impl(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_ca_matrix_bf16(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l0a_value_sync_impl(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l0a_value_impl(dst, value, config);
    asc_sync_post_process();
}

#endif