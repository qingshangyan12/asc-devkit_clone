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
 * \file dn2zn.h
 * \brief
 */
#ifndef IMPL_EXPERIMENTAL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_GM2L1_DN2ZN_H
#define IMPL_EXPERIMENTAL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_GM2L1_DN2ZN_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/instruction.h"

namespace AscendC {
namespace Te {

class CopyGmToCbufMultiDn2ZnBase {
public:
    template <const DataCopyTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        using type = typename U::elementType;
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        uint16_t ndNum = 1;
        uint16_t nValue = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout);
        uint32_t dValue = GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout);
        uint64_t srcNdMatrixStride = 0;

        auto srcRowStride = GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout);
        uint64_t srcDValue = srcRowStride;
        uint16_t dstNzC0Stride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout) * sizeof(type) / C0_SIZE;
        
        uint16_t dstNzNStride = 1;
        uint32_t dstNzMatrixStride = 0;

        uint64_t loop1SrcStride = srcDValue * sizeof(type);
        uint64_t loop4SrcStride = srcNdMatrixStride * sizeof(type);

        uint16_t loop2DstStride = dstNzNStride;  // loop2_dst_stride = dst_nz_n_stride
        uint16_t loop3DstStride = dstNzC0Stride; // loop3_dst_stride = dst_nz_c0_stride
        // loop4_dst_stride : dst_nz_matrix_stride * size_of_dst_type / C0_SIZE
        uint16_t loop4DstStride = static_cast<uint16_t>(dstNzMatrixStride * sizeof(type) / C0_SIZE);

        uint8_t cacheMode = GetCacheModeFromTensor(src.Data().Get());

        uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48; // MTE2_NZ_PARA[63:48]
        mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;         // MTE2_NZ_PARA[47:32]
        mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;         // MTE2_NZ_PARA[31:16]
        mte2NzPara |= static_cast<uint64_t>(ndNum);            // MTE2_NZ_PARA[15:0]
        SetMTE2NzPara(mte2NzPara);   // CCE: store parameters for ND2NZ DMA instructions
        copy_gm_to_cbuf_multi_nd2nz(dst.Data().Get(), src.Data().Get(), 0, loop1SrcStride, cacheMode, nValue, dValue, loop4SrcStride, false);
    }
};

} // namespace Te
} // namespace AscendC

#endif