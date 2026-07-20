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
    "impl/tensor_api/arch/cube/mmad/mmad_impl/mmad.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file mmad.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_IMPL_MMAD_H
#define IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_IMPL_MMAD_H

#include "impl/tensor_api/arch/cube/mmad/mmad_impl/instruction.h"
#include "kernel_npu_debug.h"

namespace AscendC {
namespace Te {

class Mmad {
public:
    template <const MmadTrait& trait, typename T, typename U, typename S, typename Params>
    __aicore__ inline static void Run(const T& dst, const U& fm, const S& filter, const Params& params)
    {
        MmadImpl<trait, T, U, S>(dst, fm, filter, params);
    }

private:
    template <const MmadTrait& trait, typename T, typename U, typename S>
    __aicore__ inline static constexpr void CheckTemplateForNormal()
    {
        CheckLayoutPattern<T, U, S>();
        CheckDataType::CheckMmadDataType<T, U, S>();
    }

    template <const MmadTrait& trait, typename T, typename U, typename S>
    __aicore__ inline static constexpr void CheckTemplateForMx()
    {
        CheckLayoutPattern<T, U, S>();
        CheckDataType::CheckMxMmadDataType<T, U, S>();
    }

    template <const MmadTrait& trait, typename T, typename U, typename S, typename Params>
    __aicore__ inline static void MmadImpl(const T& dst, const U& fm, const S& filter, const Params& params)
    {
        ASCENDC_DEBUG_ASSERT((GetTotalRowShape(fm.Layout()) == GetTotalRowShape(dst.Layout())),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm shape in Mmad, fm row shape should be equal to dst row shape."));
        ASCENDC_DEBUG_ASSERT((GetTotalColumnShape(filter.Layout()) == GetTotalColumnShape(dst.Layout())),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check filter shape in Mmad, filter column shape should be equal to dst column shape."));
        ASCENDC_DEBUG_ASSERT((GetTotalColumnShape(fm.Layout()) == GetTotalRowShape(filter.Layout())),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check fm and filter shape in Mmad, fm column shape should be equal to filter row shape."));
        ASCENDC_DEBUG_ASSERT((params.m <= GetTotalRowShape(dst.Layout())),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.m in Mmad, MmadParams.m should be no greater than dst row shape."));
        ASCENDC_DEBUG_ASSERT((params.n <= GetTotalColumnShape(dst.Layout())),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.n in Mmad, MmadParams.n should be no greater than dst column shape."));
        ASCENDC_DEBUG_ASSERT((params.k <= GetTotalColumnShape(fm.Layout()) &&
            params.k <= GetTotalRowShape(filter.Layout())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.k in Mmad, MmadParams.k should be no greater than fm column shape and filter row shape."));
        if constexpr (trait.mmadType == MmadType::NORMAL) {
            CheckTemplateForNormal<trait, T, U, S>();
            MmadInstr::Mmad(
                dst, fm, filter, params.m, params.k, params.n, params.unitFlag, trait.disableGemv, trait.cmatrixSource,
                params.cmatrixInitVal);
        } else if constexpr (trait.mmadType == MmadType::MX) {
            CheckTemplateForMx<trait, T, U, S>();
            MmadMxInstr::Mmad(
                dst, fm, filter, params.m, params.k, params.n, params.unitFlag, trait.disableGemv, trait.cmatrixSource,
                params.cmatrixInitVal);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_IMPL_MMAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
