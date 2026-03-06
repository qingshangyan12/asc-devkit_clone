/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file ceil_div.h
 * \brief
 */
#ifndef IMPL_STD_CEIL_DIV_H
#define IMPL_STD_CEIL_DIV_H

#ifdef ASCENDC_CPU_DEBUG
#include <cmath>
#endif

namespace AscendC {
namespace Std {
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || \
    (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113)) || defined(__ASC_NPU_HOST__)
__aicore__ constexpr inline int32_t CeilDivision(int32_t num1, int32_t num2)
{
    if (num2 == 0) {
        return 0;
    }
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1 || !defined(SPLIT_CORE_VEC)
    return (num1 + num2 - 1) / num2;
#else
    return get_repeat_ceiling(num1, num2);
#endif
}
#else
__aicore__ inline int32_t CeilDivision(int32_t num1, int32_t num2)
{
    if (num2 == 0) {
        return 0;
    }
    return (num1 + num2 - 1) / num2;
}
#endif
}
}
#endif