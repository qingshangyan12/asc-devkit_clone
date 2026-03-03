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
 * \file instruction.h
 * \brief
 */
#ifndef IMPL_EXPERIMENTAL_TENSOR_API_ARCH_COMPUTE_MMAD_NPU_ARCH_3510_INSTRUCTION_H
#define IMPL_EXPERIMENTAL_TENSOR_API_ARCH_COMPUTE_MMAD_NPU_ARCH_3510_INSTRUCTION_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/tensor/pointer_impl.h"
#include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"

namespace AscendC {
namespace Te {

class MmadInstr {
public:
    template <const MmadTrait& trait, typename T, typename U, typename S, typename Params, typename TupleParams, size_t... Is>
    __aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const Params& params, const TupleParams& tupleParams, Std::index_sequence<Is...>)
    {
        // MTE2
        MmadImpl(dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), Std::get<Is>(tupleParams)...);
    }
private:
    template <typename T, typename U, typename S>
    __aicore__ inline void MmadImpl(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint16_t m, uint16_t k, uint16_t n,
        int8_t unitFlag, bool disableGemv, bool cmatrixSource, bool cmatrixInitVal) {
        if ASCEND_IS_AIV {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            mad(dst, fm, filter, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
        }
    }
};

class MmadBiasInstr {
public:
    template <const MmadTrait& trait, typename T, typename U, typename S, typename V, typename Params, typename TupleParams, size_t... Is>
    __aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const V& bias, const Params& params, const TupleParams& tupleParams, Std::index_sequence<Is...>)
    {
        // MTE2
        MmadImpl(dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), 
            reinterpret_cast<uint64_t>(bias.Data().Get()), Std::get<Is>(tupleParams)...);
    }
private:
    template <typename T, typename U, typename S>
    __aicore__ inline void MmadImpl(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint64_t bias, uint16_t m, uint16_t k, uint16_t n,
        int8_t unitFlag, bool disableGemv, bool cmatrixSource, bool cmatrixInitVal) {
        if ASCEND_IS_AIV {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            mad(dst, fm, filter, bias, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
        }
    }
};

class MmadMxInstr {
public:
    template <const MmadTrait& trait, typename T, typename U, typename S, typename Params, typename TupleParams, size_t... Is>
    __aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const Params& params, const TupleParams& tupleParams, Std::index_sequence<Is...>)
    {
        // MTE2
        MmadImpl(dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), Std::get<Is>(tupleParams)...);
    }
private:
    template <typename T, typename U, typename S>
    __aicore__ inline void MmadImpl(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint16_t m, uint16_t k, uint16_t n,
        int8_t unitFlag, bool disableGemv, bool btBufCtrl, bool cmatrixInitVal) {
        if ASCEND_IS_AIV {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            mad_mx(dst, fm, filter, m, k, n, unitFlag, disableGemv, btBufCtrl, cmatrixInitVal);
        }
    }
};

class MmadMxBiasInstr {
public:
    template <const MmadTrait& trait, typename T, typename U, typename S, typename V, typename Params, typename TupleParams, size_t... Is>
    __aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const V& bias, const Params& params, const TupleParams& tupleParams, Std::index_sequence<Is...>)
    {
        // MTE2
        MmadImpl(dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), 
            reinterpret_cast<float8_e5m2x2_t>(bias.Data().Get()), Std::get<Is>(tupleParams)...);
    }
private:
    template <typename T, typename U, typename S>
    __aicore__ inline void MmadImpl(__cc__ T* dst, __ca__ U* fm, __cb__ S* filter, float8_e5m2x2_t bias, uint16_t m, uint16_t k, uint16_t n,
        int8_t unitFlag, bool disableGemv, bool btBufCtrl, bool cmatrixInitVal) {
        if ASCEND_IS_AIV {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            mad_mx(dst, fm, filter, bias, m, k, n, unitFlag, disableGemv, btBufCtrl, cmatrixInitVal);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif