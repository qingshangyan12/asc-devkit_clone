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
    "impl/tensor_api/atom/cube_compute/mmad_test.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file mmad_test.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ATOM_CUBE_COMPUTE_CUBE_MAD_TEST_H
#define IMPL_TENSOR_API_ATOM_CUBE_COMPUTE_CUBE_MAD_TEST_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"

#include "impl/experimental/tensor_api/atom/mad_traits_impl.h"
#include "impl/experimental/tensor_api/arch/cube_compute/mmad/mmad_impl.h"

namespace AscendC {
namespace Te {

struct MmadTraitDefaultV2 {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = DEFAULT_MMAD_TRAIT;
};

struct MmadOperationV2 {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Mad(const Args& ...args)
    {
        MmadV2<traits, Args...>(args...);
    }
};

template <typename MadTraits>
struct MmadTraits<MmadOperationV2, MadTraits> : public MmadTraits<MmadOperationV2, MadTraits, MmadOperationV2, MmadTraitDefaultV2> {};

template <>
struct MmadTraits<MmadOperationV2> : public MmadTraits<MmadOperationV2, MmadTraitDefaultV2> {};

}
}

#endif // IMPL_TENSOR_API_ATOM_CUBE_COMPUTE_CUBE_MAD_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif