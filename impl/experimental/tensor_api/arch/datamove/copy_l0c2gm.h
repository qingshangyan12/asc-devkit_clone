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
#warning                                                                                                               \
    "impl/tensor_api/arch/datamove/copy_l0c2gm.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy_l0c2gm.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ATOM_DATAMOVE_COPY_L0C2GM_H
#define IMPL_TENSOR_API_ATOM_DATAMOVE_COPY_L0C2GM_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/arch/utils/copy_fixpipe_utils.h"
#include "impl/experimental/tensor_api/arch/datamove/copy_l0c2gm_impl.h"

namespace AscendC {
namespace Te {

template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplate<T, U>, void>::type
CopyL0C2GM(const T& dst, const U& src, const FixpipeParams& params = FixpipeParams{})
{}

template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename S>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplate<T, U, S>, void>::type
CopyL0C2GM(const T& dst, const U& src, const S& quant, const FixpipeParams& params = FixpipeParams{})
{}

template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplateWithCoord<T, U, Coord>, void>::type
CopyL0C2GM(const T& dst, const U& src, const Coord& coord, const FixpipeParams& params = FixpipeParams{})
{
    auto sliceTensor = dst(coord, src);
    CopyL0C2GM<trait>(sliceTensor, src, params);
}

template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename S, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplateWithCoord<T, U, S, Coord>, void>::type
CopyL0C2GM(const T& dst, const U& src, const S& quant, const Coord& coord,
           const FixpipeParams& params = FixpipeParams{})
{
    auto sliceTensor = dst(coord, src);
    CopyL0C2GM<trait>(sliceTensor, src, quant, params);
}

struct CopyL0C2GM {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            return;
        }
        CopyL0C2GM<traits>(args...);
    }
};

struct CopyL0C2GMWith {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        // custom function CopyL0C2GM<traits, Args...>(args...);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ATOM_DATAMOVE_COPY_L0C2GM_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif