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
 * \file set_mm_layout_transform_impl.h
 * \brief
 */

#ifndef IMPL_C_API_INSTR_CUBE_DMAMOVE_SET_MM_LAYOUT_TRANSFORM_ASC_2201_SET_MM_LAYOUT_TRANSFORM_IMPL_H
#define IMPL_C_API_INSTR_CUBE_DMAMOVE_SET_MM_LAYOUT_TRANSFORM_ASC_2201_SET_MM_LAYOUT_TRANSFORM_IMPL_H
#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

__aicore__ inline void set_mm_layout_transform_impl(bool layout_mode)
{
    constexpr int32_t CAPI_MM_LAYOUT_MODE_BIT = 51;
    if (layout_mode) {
        set_ctrl(sbitset1(get_ctrl(), CAPI_MM_LAYOUT_MODE_BIT));
    } else {
        set_ctrl(sbitset0(get_ctrl(), CAPI_MM_LAYOUT_MODE_BIT));
    }
}

} // namespace CApiInternal

#endif