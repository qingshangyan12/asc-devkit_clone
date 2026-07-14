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
    "impl/tensor_api/atom/vector/reg_load_atom_impl.h is an internal header file and must not be used directly."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file reg_load_atom_impl.h
* \brief UB aligned load/store for RegTensor operands (replaces shadow DataCopy for the common path).
*/
#ifndef IMPL_TENSOR_API_ATOM_VECTOR_REG_LOAD_ATOM_IMPL_H
#define IMPL_TENSOR_API_ATOM_VECTOR_REG_LOAD_ATOM_IMPL_H

#include "tensor_api/arch/vector/load_op.h"
#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace AscendC {
namespace Te {

namespace load {
struct UbAlign {};
struct UbStoreAlign {};
} // namespace load

namespace Inst {
namespace RegLoad {

class UbAlign {
public:
    template <typename T, typename Dst>
    __simd_callee__ inline static void Run(Dst& dst, __ubuf__ T* src)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            using RegType = typename Dst::RegType;
            asc_loadalign(static_cast<RegType&>(dst), src);
        }
    }
};

class UbStoreAlign {
public:
    template <typename T, typename Src, typename MaskT>
    __simd_callee__ inline static void Run(__ubuf__ T* dst, Src& src, MaskT& mask)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            using RegType = typename Src::RegType;
            asc_storealign(dst, static_cast<RegType&>(src), mask);
        }
    }
};

} // namespace RegLoad
} // namespace Inst

template <typename Op>
struct LoadAtom {
    template <typename Dst, typename T>
    __simd_callee__ inline void Call(Dst& dst, __ubuf__ T* src) const
    {
        Inst::RegLoad::UbAlign::Run<T>(dst, src);
    }
};

template <>
struct LoadAtom<load::UbAlign> {
    template <typename Dst, typename T>
    __simd_callee__ inline void Call(Dst& dst, __ubuf__ T* src) const
    {
        Inst::RegLoad::UbAlign::Run<T>(dst, src);
    }
};

template <typename Op>
struct StoreAtom {
    template <typename T, typename Src, typename MaskT>
    __simd_callee__ inline void Call(__ubuf__ T* dst, Src& src, MaskT& mask) const
    {
        Inst::RegLoad::UbStoreAlign::Run<T>(dst, src, mask);
    }
};

template <>
struct StoreAtom<load::UbStoreAlign> {
    template <typename T, typename Src, typename MaskT>
    __simd_callee__ inline void Call(__ubuf__ T* dst, Src& src, MaskT& mask) const
    {
        Inst::RegLoad::UbStoreAlign::Run<T>(dst, src, mask);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ATOM_VECTOR_REG_LOAD_ATOM_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
