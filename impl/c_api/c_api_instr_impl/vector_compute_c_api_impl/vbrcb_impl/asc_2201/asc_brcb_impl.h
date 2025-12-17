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
 * \file brcb_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VBRCB_IMPL_ASC_2201_ASC_BRCB_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VBRCB_IMPL_ASC_2201_ASC_BRCB_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_brcb_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, const asc_brcb_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vbrcb(dst, src, static_cast<uint16_t>(config.dst_block_stride),
          static_cast<uint16_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.repeat));
#endif
}

__aicore__ inline void asc_brcb_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, const asc_brcb_config& config)
{
    asc_brcb_impl(dst, src, config);
    asc_sync_impl();
}

__aicore__ inline void asc_brcb_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, const asc_brcb_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    vbrcb(dst, src, static_cast<uint16_t>(config.dst_block_stride),
          static_cast<uint16_t>(config.dst_repeat_stride), static_cast<uint8_t>(config.repeat));
#endif
}

__aicore__ inline void asc_brcb_sync_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, const asc_brcb_config& config)
{
    asc_brcb_impl(dst, src, config);
    asc_sync_impl();
}

#endif