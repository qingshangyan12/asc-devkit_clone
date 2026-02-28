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
 * \file sys_impl.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_UTILS_SYS_IMPL_H
#define IMPL_TENSOR_API_UTILS_SYS_IMPL_H

#include "impl/experimental/tensor_api/tensor/pointer_impl.h"
#include "impl/experimental/tensor_api/utils/constant_impl.h"

namespace AscendC {
namespace Te {
struct ArchVersion {
    static constexpr uint32_t V3510 = 3510;
    static constexpr uint32_t V2201 = 2201;
};

struct GetArchVersion {
    __aicore__ inline constexpr uint32_t operator()() const {
#ifdef __NPU_ARCH__
        return __NPU_ARCH__;
#else
        return 0;
#endif
    }
};

constexpr uint32_t CURRENT_ARCH_VERSION = GetArchVersion{}();
template <typename T>
__aicore__ inline uint8_t GetCacheModeFromTensor(__gm__ T* src) {
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
        return static_cast<uint8_t>((reinterpret_cast<uint64_t>(src)) >> L2_CACHE_OFFSET);
    } else {
        return 0;
    }
}

template<typename T>
__aicore__ inline constexpr auto GetHardPos()
{
   return T::iterator::hardPos;
}
} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_UTILS_SYS_IMPL_H
