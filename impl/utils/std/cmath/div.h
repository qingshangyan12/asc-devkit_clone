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
 * \file abs.h
 * \brief
 */
#ifndef IMPL_STD_ABS_H
#define IMPL_STD_ABS_H

#ifdef ASCENDC_CPU_DEBUG
#include <cmath>
#endif

namespace AscendC {
namespace Std {
__aicore__ constexpr inline uint32_t ceil_div(uint32_t a, uint32_t b)
{
    return (a + b - 1) / b;
}

__aicore__ constexpr inline uint32_t align_up(uint32_t a, uint32_t b)
{
    return ceil_div(a, b) * b;
}
}
}
#endif