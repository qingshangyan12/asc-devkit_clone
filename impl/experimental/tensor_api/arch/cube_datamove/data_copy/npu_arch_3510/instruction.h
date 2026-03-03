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
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_INSTRUCTION_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "include/experimental/tensor_api/arch/cube_datamove/cube_datamove_struct.h"
#include "impl/experimental/tensor_api/utils/format_impl.h"
#include "impl/experimental/tensor_api/utils/macro_impl.h"
#include "impl/experimental/tensor_api/tensor/pointer_impl.h"
#include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"

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
        uint8_t leftPaddingCnt, uint8_t rightPaddingCnt, uint8_t cacheMode, int64_t srcStride, int64_t dstStride)
    {
        if ASCEND_IS_AIV {
            return;
        }

        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            copy_gm_to_cbuf_align_v2(dst, src, 0, blockCount, blockLen, leftPaddingCnt, rightPaddingCnt, true,
                cacheMode, srcStride, dstStride);
        }
    }
};

class CopyGmToCbufMultiNd2nzInstr {
public:
    template <typename T, typename U, typename... Params>
    __aicore__ inline void DataCopy(const T& dst, const U& src, const Params& ...params) {
        CopyGmToCbufMultiNd2nz(dst.Data().Get(), src.Data().Get(), params...);
    }

private:
    template <typename T>
    __aicore__ inline void CopyGmToCbufMultiNd2nz(__cbuf__ T* dst, __gm__ T* src, uint16_t ndNum, uint16_t loop2DstStride, 
        uint16_t loop3DstStride, uint16_t loop4DstStride, uint64_t loop1SrcStride, uint8_t cacheMode, uint16_t nValue, 
        uint32_t dValue, uint64_t loop4SrcStride, bool enableSmallC0)
    {
        if ASCEND_IS_AIV {
            return;
        }

        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48; // MTE2_NZ_PARA[63:48]
            mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;         // MTE2_NZ_PARA[47:32]
            mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;         // MTE2_NZ_PARA[31:16]
            mte2NzPara |= static_cast<uint64_t>(ndNum);            // MTE2_NZ_PARA[15:0]
            SetMTE2NzPara(mte2NzPara);   // CCE: store parameters for ND2NZ DMA instructions
            copy_gm_to_cbuf_multi_nd2nz(dst, src, 0, loop1SrcStride, cacheMode, nValue, dValue, loop4SrcStride, enableSmallC0);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif
