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
 * \file load_data_l12l0b.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0B_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0B_H

#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/load_data_l12l0b/zn2zn.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/load_data_l12l0b/nz2zn.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/load_data/npu_arch_3510/load_data_l12l0b/nz2znb8.h"

namespace AscendC {
namespace Te {
class LoadDataFourDim3510L12L0B : public LoadDataFourDim3510L12L0BZN2ZN, public LoadDataFourDim3510L12L0BNZ2ZNB8,
    public LoadDataFourDim3510L12L0BNZ2ZN {
public:
    template <const LoadDataTrait& trait, typename T, typename U, class Coord>
    __aicore__ inline void Run(const T& dst, const U& src, const Coord& coord) {
        if constexpr (IsZNFormat<U>::value && IsZNFormat<T>::value) {
            LoadDataFourDim3510L12L0BZN2ZN::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsNZFormat<U>::value && IsZNFormat<T>::value && (sizeof(typename U::elementType) == 1)) {
            LoadDataFourDim3510L12L0BNZ2ZNB8::Run<trait, T, U, Coord>(dst, src, coord);
        } else if constexpr (IsNZFormat<U>::value && IsZNFormat<T>::value) {
            LoadDataFourDim3510L12L0BNZ2ZN::Run<trait, T, U, Coord>(dst, src, coord);
        } else {
            static_assert(Std::always_false_v<T>);
        }
    }
};
} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_LOAD_DATA_L12L0B_H