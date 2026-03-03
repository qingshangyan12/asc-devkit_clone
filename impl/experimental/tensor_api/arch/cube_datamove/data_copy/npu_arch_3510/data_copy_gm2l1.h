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

#include "impl/experimental/tensor_api/tensor/pointer_impl.h"
#include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"
#include "impl/experimental/tensor_api/arch/arch_utils.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/dn2nz.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/dn2zn.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nd2nd.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nd2nz.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nd2zn.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/data_copy/npu_arch_3510/data_copy_gm2l1/nz2nz.h"


namespace AscendC {
namespace Te {

class DataCopyFourDim3510GM2L1 : public CopyGmToCbufMultiNd2nzBase, public CopyGmToCbufMultiDn2nzBase,
    public CopyGmToCbufAlignV2NZBase, public CopyGmToCbufAlignV2NDBase , public CopyGmToCbufMultiDn2ZnBase {
public:
    template <const DataCopyTrait& trait, typename T, typename U, typename Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        auto fourDimSrc = PreProcess(src);
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
        } else if constexpr (IsNDFormat<U>::value && IsZDFormat<T>::value) {
            CopyGmToCbufMultiDn2ZnBase::Run<trait, T, U, Coord>(dst, src, coord);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_GM2L1_H