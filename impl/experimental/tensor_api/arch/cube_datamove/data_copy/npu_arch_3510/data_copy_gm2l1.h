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
 * \file data_copy_gm2l1.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_GM2L1_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_GM2L1_H

#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/dn2nz.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/dn2zn.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nd2nd.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nd2nz.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nz2nz.h"

namespace AscendC {
namespace Te {

template <typename T>
__aicore__ inline void SetMTE2NzPara(const T& para) {
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
        set_mte2_nz_para(para);
    }
}

class CopyGmToCbufAlignV2Base {
public:
    template <const DataCopyTrait& trait, typename T, typename U, typename V>
    __aicore__ inline void DataCopy(const T& dst, const U& src, const V& tupleParams) {
        DataCopyImpl<trait, T, U, V>(dst, src, tupleParams, tuple_sequence<V>{});
    }

private:
    template <const DataCopyTrait& trait, typename T, typename U, typename V, size_t... Is>
    __aicore__ inline void DataCopyImpl(const T& dst, const U& src, const V& tupleParams, Std::index_sequence<Is...>)
    {
        using srcType = typename U::elementType;
        if constexpr (sizeof(srcType) == sizeof(uint32_t)) {
            CopyGmToCbufAlignV2(dst.Data().Get(), src.Data().Get(), Std::get<Is>(tupleParams)...);
        } else if constexpr (sizeof(srcType) == sizeof(uint16_t)) {
            CopyGmToCbufAlignV2(dst.Data().Get(), src.Data().Get(), Std::get<Is>(tupleParams)...);
        } else if constexpr (sizeof(srcType) == sizeof(uint8_t)) {
            CopyGmToCbufAlignV2(dst.Data().Get(), src.Data().Get(), Std::get<Is>(tupleParams)...);
        }
    }

    template <typename T>
    __aicore__ inline void CopyGmToCbufAlignV2(__cbuf__ T* dst, __gm__ T* src, uint32_t blockCount, uint32_t blockLen, 
        uint8_t leftPaddingCnt, uint8_t rigntPaddingCnt, uint8_t cacheMode, int64_t srcStride, int64_t dstStride)
    {
        if ASCEND_IS_AIV {
            return;
        }

        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            copy_gm_to_cbuf_align_v2(dst, src, 0, blockCount, blockLen, leftPaddingCnt, rigntPaddingCnt, true,
                cacheMode, srcStride, dstStride);
        }
    }
};



class DataCopyFourDim3510GM2L1 : public CopyGmToCbufMultiNd2nzBase, public CopyGmToCbufMultiDn2nzBase,
    public CopyGmToCbufAlignV2NZBase, public CopyGmToCbufAlignV2NDBase {
public:
    template <const DataCopyTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        // auto fourDimSrc = PreProcess(src);
        Execute<trait>(dst, fourDimSrc, coord);
    }

private:
    template <const DataCopyTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline void Execute(const T& dst, const U& src, const Coord& coord) {
        if constexpr (IsNZFormat<U>::value && IsNZFormat<T>::value) {
            CopyGmToCbufAlignV2NZBase::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsNDFormat<U>::value && IsNZFormat<T>::value) {
            CopyGmToCbufMultiNd2nzBase::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsDNFormat<U>::value && IsNZFormat<T>::value) {
            CopyGmToCbufMultiDn2nzBase::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsNDFormat<U>::value && IsNDFormat<T>::value) {
            CopyGmToCbufAlignV2NDBase::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsDNFormat<U>::value && IsNZFormat<T>::value) {
            CopyGmToCbufMultiDn2ZnBase::Run<trait, T, U, Coord>(dst, src, coord);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_GM2L1_H