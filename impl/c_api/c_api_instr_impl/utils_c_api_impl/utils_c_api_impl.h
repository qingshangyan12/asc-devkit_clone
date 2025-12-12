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
 * \file utils_c_api_impl.h
 * \brief
 */
#ifndef IMPL_C_API_C_API_INSTR_IMPL_UTILS_C_API_IMPL_UTILS_C_API_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_UTILS_C_API_IMPL_UTILS_C_API_IMPL_H

#include "c_api_utils_intf.h"

#define C_API_AIC_TYPE 1
#define C_API_AIV_TYPE 2
#define C_API_MIX_TYPE 3
#if (defined(__DAV_CUBE__) && __NPU_ARCH__ == 2201)
#define C_API_CV_SPLIT_CORE_TYPE C_API_AIC_TYPE
#elif (defined(__DAV_VEC__) && __NPU_ARCH__ == 2201)
#define C_API_CV_SPLIT_CORE_TYPE C_API_AIV_TYPE
#else
#define C_API_CV_SPLIT_CORE_TYPE C_API_MIX_TYPE
#endif

__aicore__ inline void asc_set_mask_count_begin(uint32_t count)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    set_mask_count();
    set_vector_mask(static_cast<uint64_t>(0), static_cast<uint64_t>(count));
#endif
}

__aicore__ inline void asc_set_mask_count_end()
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    set_mask_norm();
#endif
}

#endif // IMPL_C_API_C_API_INSTR_IMPL_UTILS_C_API_IMPL_UTILS_C_API_IMPL_H
