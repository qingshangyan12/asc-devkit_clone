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
    "impl/tensor_api/arch/vector/binary/binary_vf.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file binary_vf.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_BINARY_BINARY_VF_H
#define IMPL_TENSOR_API_ARCH_VECTOR_BINARY_BINARY_VF_H

#include "impl/tensor_api/arch/vector/binary/instruction.h"
#include "impl/tensor_api/arch/vector/utils/mask_utils.h"

namespace AscendC {
namespace Te {

template<typename CalcFunc, typename TraitType>
class Transform2BinaryReg {
public:
    template<typename T, typename U, typename V>
    __simd_callee__ inline static void Run(T& dst, U& src0, V& src1, vector_bool mask)
    {
        CalcFunc::template Run<T>(dst, src0, src1, mask);
        // asc_add(dst, src0, src1, mask);
    }
};

}
}

#endif // IMPL_TENSOR_API_ARCH_VECTOR_BINARY_BINARY_VF_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
