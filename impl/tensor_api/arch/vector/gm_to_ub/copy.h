/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/vector/gm_to_ub/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_H
#define IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/atom/copy_traits_impl.h"
#include "impl/tensor_api/arch/vector/gm_to_ub/routing.h"
#include "impl/tensor_api/utils/npu_debug_check.h"

namespace AscendC {
namespace Te {

constexpr CopyGM2UBTrait DEFAULT_COPY_GM_TO_UB_TRAIT = CopyGM2UBTrait{};

struct CopyGM2UBTraitDefault {
    using TraitType = CopyGM2UBTrait;
    static constexpr const TraitType value = DEFAULT_COPY_GM_TO_UB_TRAIT;
};

struct CopyGM2UB {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            DataCopyImpl<traits, Args...>(args...);
        }
    }

private:
    template <const CopyGM2UBTrait& trait = DEFAULT_COPY_GM_TO_UB_TRAIT, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        using dstTPos = GetMemLocation<T>;
        using srcTPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstTPos, Location::UB>, "When Copy tensor from GM to UB, dst tensor must on UB");
        static_assert(Std::is_same_v<srcTPos, Location::GM>, "When Copy tensor from GM to UB, src tensor must on GM");
        if constexpr (sizeof(typename U::elementType) == sizeof(uint64_t)) {
            ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(src.Data().Get()) % sizeof(uint32_t) == 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                    "Failed to check src address in CopyGM2UB, src address must be 4-byte aligned for 64-bit src "
                    "elements."));
        } else {
            ASCENDC_DEBUG_ASSERT(
                (reinterpret_cast<uintptr_t>(src.Data().Get()) % sizeof(typename U::elementType) == 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src address in CopyGM2UB, src address must be aligned to the src element size."));
        }
        ASCENDC_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 32 == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst address in CopyGM2UB, dst address must be 32-byte aligned."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor shape in CopyGM2UB, all dst tensor shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor stride in CopyGM2UB, all dst tensor strides must be greater than 0; a zero "
            "stride is allowed only when the dst tensor format defines it and the corresponding shape size is 1."));
        ASCENDC_DEBUG_ASSERT((IsShapeValid(src.Layout().Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor shape in CopyGM2UB, all src tensor shape sizes must be greater than 0."));
        ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(src.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor stride in CopyGM2UB, all src tensor strides must be greater than 0; a zero "
            "stride is allowed only when the src tensor format defines it and the corresponding shape size is 1."));
        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;
        using CopyGM2UBImpl = typename CopyGM2UBRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyGM2UBImpl::template Run<trait, T, U>(dst, src);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_VECTOR_GM_TO_UB_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
