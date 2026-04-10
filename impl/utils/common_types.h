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
 * \file common_types.h
 * \brief
 */
#ifndef IMPL_UTILS_COMMON_TYPES_H
#define IMPL_UTILS_COMMON_TYPES_H

#include <cstdint>
#include <type_traits>

namespace AscendC {
enum class Hardware : uint8_t { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF, MAX };
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3510)) || defined(__ASC_NPU_HOST__)
    using fp4x2_e2m1_t = float4_e2m1x2_t;
    using fp4x2_e1m2_t = float4_e1m2x2_t;
    using fp8_e5m2_t = float8_e5m2_t;
    using fp8_e4m3fn_t = float8_e4m3_t;
    using fp8_e8m0_t = float8_e8m0_t;
#else
    using fp4x2_e2m1_t = uint8_t;
    using fp4x2_e1m2_t = uint8_t;
    using fp8_e5m2_t = uint8_t;
    using fp8_e4m3fn_t = uint8_t;
    using fp8_e8m0_t = uint8_t;
#endif

#endif