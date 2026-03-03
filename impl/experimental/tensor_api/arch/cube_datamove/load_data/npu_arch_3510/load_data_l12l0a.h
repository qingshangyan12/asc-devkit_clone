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
 * \file load_data_l12l0a.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0A_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0A_H

#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/load_data_l12l0a/nz2nz.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/load_data_l12l0a/zn2nz.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/load_data_l12l0a/zn2nzb8.h"

namespace AscendC {
namespace Te {

class LoadDataFourDim3510L12L0A : public LoadDataFourDim3510L12L0ANZ2NZ, public LoadDataFourDim3510L12L0AZN2NZB8,
    public LoadDataFourDim3510L12L0AZN2NZ {
public:
    template <const LoadDataTrait& trait, typename T, typename U, class Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        if constexpr (IsNZFormat<U>::value && IsNZFormat<T>::value) {
            LoadDataFourDim3510L12L0ANZ2NZ::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsZNFormat<U>::value && IsNZFormat<T>::value && (sizeof(typename U::elementType) == 1)) {
            LoadDataFourDim3510L12L0AZN2NZB8::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsZNFormat<U>::value && IsNZFormat<T>::value) {
            LoadDataFourDim3510L12L0AZN2NZ::Run<trait, T, U, Coord>(dst, src, coord);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0A_H