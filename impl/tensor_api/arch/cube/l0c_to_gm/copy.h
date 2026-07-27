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
    "impl/tensor_api/arch/cube/l0c_to_gm/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/atom/copy_traits_impl.h"
#include "impl/tensor_api/arch/cube/utils/l0c2out_utils.h"
#include "impl/tensor_api/arch/cube/l0c_to_gm/routing.h"
#include "impl/tensor_api/utils/npu_debug_check.h"

namespace AscendC {
namespace Te {

constexpr CopyL0C2GMTrait DEFAULT_COPY_L0C2GM_TRAIT = CopyL0C2GMTrait{};
struct CopyL0C2GMTraitDefault {
    using TraitType = CopyL0C2GMTrait;
    static constexpr const TraitType value = DEFAULT_COPY_L0C2GM_TRAIT;
};

struct CopyL0C2GMBase {
public:
    template <const CopyL0C2GMTrait& trait = DEFAULT_COPY_L0C2GM_TRAIT, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(
        const T& dst, const U& src, const FixpipeParams& params = DEFAULT_FIXPIPE_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::GM>, "When Copy tensor from L0C to GM, dst tensor must on GM");
        static_assert(Std::is_same_v<srcPos, Location::L0C>, "When Copy tensor from L0C to GM, src tensor must on L0C");
        TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(src.Data().Get()) % 64 == 0),
            TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src address in CopyL0C2GM, src L0C address must be 64-byte aligned."));

        TENSOR_API_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor shape in CopyL0C2GM, all dst tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor stride in CopyL0C2GM, all dst tensor strides must be greater than 0; a zero "
            "stride is allowed only when the dst tensor format defines it and the corresponding shape size is 1."));
        TENSOR_API_DEBUG_ASSERT((IsShapeValid(src.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor shape in CopyL0C2GM, all src tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(src.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor stride in CopyL0C2GM, all src tensor strides must be greater than 0; a zero "
            "stride is allowed only when the src tensor format defines it and the corresponding shape size is 1."));

        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;
        if constexpr (Std::is_same_v<DstLayoutPtn, NZLayoutPtn>) {
            TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 32 == 0),
                TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst address in CopyL0C2GM, NZ dst GM address must be 32-byte aligned."));
        }

        using CopyL0C2GMImpl = typename CopyL0C2GMRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL0C2GMImpl::template Run<trait>(dst, src, params);
    }

    template <const CopyL0C2GMTrait& trait = DEFAULT_COPY_L0C2GM_TRAIT, typename T, typename U, typename S>
    __aicore__ inline static typename Std::enable_if<Std::is_same_v<S, uint64_t>, void>::type DataCopyImpl(
        const T& dst, const U& src, const S& quant, const FixpipeParams& params = DEFAULT_FIXPIPE_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::GM>, "When Copy tensor from L0C to GM, dst tensor must on GM");
        static_assert(Std::is_same_v<srcPos, Location::L0C>, "When Copy tensor from L0C to GM, src tensor must on L0C");
        TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(src.Data().Get()) % 64 == 0),
            TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src address in CopyL0C2GM, src L0C address must be 64-byte aligned."));

        TENSOR_API_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor shape in CopyL0C2GM, all dst tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor stride in CopyL0C2GM, all dst tensor strides must be greater than 0; a zero "
            "stride is allowed only when the dst tensor format defines it and the corresponding shape size is 1."));
        TENSOR_API_DEBUG_ASSERT((IsShapeValid(src.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor shape in CopyL0C2GM, all src tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(src.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor stride in CopyL0C2GM, all src tensor strides must be greater than 0; a zero "
            "stride is allowed only when the src tensor format defines it and the corresponding shape size is 1."));

        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;
        if constexpr (Std::is_same_v<DstLayoutPtn, NZLayoutPtn>) {
            TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 32 == 0),
                TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst address in CopyL0C2GM, NZ dst GM address must be 32-byte aligned."));
        }

        using CopyL0C2GMImpl = typename CopyL0C2GMRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL0C2GMImpl::template Run<trait>(dst, src, quant, params);
    }

    template <const CopyL0C2GMTrait& trait = DEFAULT_COPY_L0C2GM_TRAIT, typename T, typename U, typename S>
    __aicore__ inline static typename Std::enable_if<IsAttrTensorV<S>, void>::type DataCopyImpl(
        const T& dst, const U& src, const S& quant, const FixpipeParams& params = DEFAULT_FIXPIPE_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::GM>, "When Copy tensor from L0C to GM, dst tensor must on GM");
        static_assert(Std::is_same_v<srcPos, Location::L0C>, "When Copy tensor from L0C to GM, src tensor must on L0C");
        TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(src.Data().Get()) % 64 == 0),
            TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src address in CopyL0C2GM, src L0C address must be 64-byte aligned."));
        TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(quant.Data().Get()) % 32 == 0),
            TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quant address in CopyL0C2GM, quant L1 address must be 32-byte aligned."));

        TENSOR_API_DEBUG_ASSERT((IsShapeValid(dst.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor shape in CopyL0C2GM, all dst tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(dst.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst tensor stride in CopyL0C2GM, all dst tensor strides must be greater than 0; a zero "
            "stride is allowed only when the dst tensor format defines it and the corresponding shape size is 1."));
        TENSOR_API_DEBUG_ASSERT((IsShapeValid(src.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor shape in CopyL0C2GM, all src tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(src.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src tensor stride in CopyL0C2GM, all src tensor strides must be greater than 0; a zero "
            "stride is allowed only when the src tensor format defines it and the corresponding shape size is 1."));
        TENSOR_API_DEBUG_ASSERT((IsShapeValid(quant.Layout().Shape())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quant tensor shape in CopyL0C2GM, all quant tensor shape sizes must be greater than 0."));
        TENSOR_API_DEBUG_ASSERT((IsLayoutStrideValid(quant.Layout())), TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check quant tensor stride in CopyL0C2GM, all quant tensor strides must be greater than 0; a "
            "zero "
            "stride is allowed only when the quant tensor format defines it and the corresponding shape size is 1."));

        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;
        if constexpr (Std::is_same_v<DstLayoutPtn, NZLayoutPtn>) {
            TENSOR_API_DEBUG_ASSERT((reinterpret_cast<uintptr_t>(dst.Data().Get()) % 32 == 0),
                TENSOR_API_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst address in CopyL0C2GM, NZ dst GM address must be 32-byte aligned."));
        }

        using CopyL0C2GMImpl = typename CopyL0C2GMRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL0C2GMImpl::template Run<trait>(dst, src, quant, params);
    }
};

struct CopyL0C2GM : public CopyL0C2GMBase {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            return;
        }
        DataCopyImpl<trait>(args...);
    }
};

struct CopyL0C2GMWith : public CopyL0C2GMBase {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            return;
        }
        DataCopyImpl<trait>(args...);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
