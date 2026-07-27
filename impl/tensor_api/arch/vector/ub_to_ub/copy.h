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
    "impl/tensor_api/arch/vector/ub_to_ub/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_UB_TO_UB_COPY_H
#define IMPL_TENSOR_API_ARCH_VECTOR_UB_TO_UB_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/atom/copy_traits_impl.h"
#include "impl/tensor_api/arch/vector/ub_to_ub/routing.h"
#include "impl/tensor_api/utils/npu_debug_check.h"

namespace AscendC {
namespace Te {

constexpr CopyUB2UBTrait DEFAULT_COPY_UB_TO_UB_TRAIT = CopyUB2UBTrait{};

struct CopyUB2UBTraitDefault {
    using TraitType = CopyUB2UBTrait;
    static constexpr const TraitType value = DEFAULT_COPY_UB_TO_UB_TRAIT;
};

struct CopyUB2UB {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            DataCopyImpl<traits, Args...>(args...);
        }
    }

private:
    template <const CopyUB2UBTrait& trait = DEFAULT_COPY_UB_TO_UB_TRAIT, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        using dstTPos = GetMemLocation<T>;
        using srcTPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstTPos, Location::UB>, "When Copy tensor from UB to UB, dst tensor must on UB");
        static_assert(Std::is_same_v<srcTPos, Location::UB>, "When Copy tensor from UB to UB, src tensor must on UB");
        if constexpr (sizeof(typename U::elementType) == sizeof(uint64_t)) {
            TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(src.Data().Get()) % sizeof(uint32_t) == 0),
                TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
                    "Failed to check src address in CopyUB2UB, src address must be 4-byte aligned for 64-bit src "
                    "elements."));
        } else {
            TENSOR_API_DEBUG_ASSERT(
                (reinterpret_cast<uintptr_t>(src.Data().Get()) % sizeof(typename U::elementType) == 0),
                TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src address in CopyUB2UB, src address must be aligned to the src element size."));
        }
        TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 32 == 0),
            TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst address in CopyUB2UB, dst address must be 32-byte aligned."));
        TENSOR_API_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor shape in CopyUB2UB, all dst tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor stride in CopyUB2UB, all dst tensor strides must be greater than 0; a zero "
            "stride is allowed only when the dst tensor format defines it and the corresponding shape size is 1."));
        TENSOR_API_DEBUG_ASSERT((IsShapeValid(src.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor shape in CopyUB2UB, all src tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(src.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor stride in CopyUB2UB, all src tensor strides must be greater than 0; a zero "
            "stride is allowed only when the src tensor format defines it and the corresponding shape size is 1."));
        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;
        using CopyUB2UBImpl = typename CopyUB2UBRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyUB2UBImpl::template Run<trait, T, U>(dst, src);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_VECTOR_UB_TO_UB_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
