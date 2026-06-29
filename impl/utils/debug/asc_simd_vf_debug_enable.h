/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_simd_vf_debug_enable.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_SIMD_VF_DEBUG_ENABLE_H
#define IMPL_UTILS_DEBUG_ASC_SIMD_VF_DEBUG_ENABLE_H

#include "impl/utils/debug/asc_simd_vf_debug_traits.h"

template <>
struct SimdVfTraits<SimdVfDebugTag> {
    static constexpr bool enabled = true;
};

#endif // IMPL_UTILS_DEBUG_ASC_SIMD_VF_DEBUG_ENABLE_H
