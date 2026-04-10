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
    "impl/tensor_api/arch/cube_datamove/l1_to_l0/routing.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file routing.h
 * \brief
 */

#ifndef IMPL_TENSOR_API_ARCH_DATAMOVE_L1_TO_L0_ROUTING_H
#define IMPL_TENSOR_API_ARCH_DATAMOVE_L1_TO_L0_ROUTING_H

#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0a/nz2nz.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0a/zn2nz.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0a/zn2nzb8b4.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0a/nz2nz_with_coord.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0a/zn2nz_with_coord.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0a/zn2nzb8b4_with_coord.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0b/zn2zn.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0b/nz2zn.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0b/nz2znb8b4.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0b/zn2zn_with_coord.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0b/nz2zn_with_coord.h"
#include "impl/experimental/tensor_api/arch/datamove/l1_to_l0/npu_arch_3510/l12l0b/nz2znb8b4_with_coord.h"

namespace AscendC {
namespace Te {

class LoadDataIgnore {
public:
    template <const LoadDataTrait& trait, typename ...Args>
    __aicore__ inline void static Run(const Args&... args) {}
};

template <Hardware dstPos, Hardware srcPos, uint32_t Version, typename DstLayoutPattern, typename SrcLayoutPattern, CopyMode copyMode>
struct LoadDataTensor2Tensor {
    using type = LoadDataIgnore;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0A, Hardware::L1, ArchVersion::V3510, NzLayoutPattern, NzLayoutPattern, CopyMode::NORMAL>
{
    using type = LoadDataL12L0ANZ2NZ3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0A, Hardware::L1, ArchVersion::V3510, NzLayoutPattern, NzLayoutPattern, CopyMode::NORMAL_COORD>
{
    using type = LoadDataL12L0ANZ2NZWithCoord3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0A, Hardware::L1, ArchVersion::V3510, ZnLayoutPattern, NzLayoutPattern, CopyMode::TRANS>
{
    using type = LoadDataL12L0AZN2NZ3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0A, Hardware::L1, ArchVersion::V3510, ZnLayoutPattern, NzLayoutPattern, CopyMode::TRANS_COORD>
{
    using type = LoadDataL12L0AZN2NZWithCoord3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0A, Hardware::L1, ArchVersion::V3510, ZnLayoutPattern, NzLayoutPattern, CopyMode::TRANSB8B4>
{
    using type = LoadDataL12L0AZN2NZB8B43510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0A, Hardware::L1, ArchVersion::V3510, ZnLayoutPattern, NzLayoutPattern, CopyMode::TRANSB8B4_COORD>
{
    using type = LoadDataL12L0AZN2NZB8B4WithCoord3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0B, Hardware::L1, ArchVersion::V3510, ZnLayoutPattern, ZnLayoutPattern, CoordMode::NORMAL>
{
    using type = LoadDataL12L0BZN2ZN3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0B, Hardware::L1, ArchVersion::V3510, ZnLayoutPattern, ZnLayoutPattern, CopyMode::NORMAL_COORD>
{
    using type = LoadDataL12L0BZN2ZNWithCoord3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0B, Hardware::L1, ArchVersion::V3510, NzLayoutPattern, ZnLayoutPattern, CopyMode::TRANS>
{
    using type = LoadDataL12L0BNZ2ZN3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0B, Hardware::L1, ArchVersion::V3510, NzLayoutPattern, ZnLayoutPattern, CopyMode::TRANS_COORD>
{
    using type = LoadDataL12L0BNZ2ZNWithCoord3510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0B, Hardware::L1, ArchVersion::V3510, NzLayoutPattern, ZnLayoutPattern, CopyMode::TRANSB8B4>
{
    using type = LoadDataL12L0BNZ2ZNB8B43510;
};

template <>
struct LoadDataTensor2Tensor<Hardware::L0B, Hardware::L1, ArchVersion::V3510, NzLayoutPattern, ZnLayoutPattern, CopyMode::TRANSB8B4_COORD>
{
    using type = LoadDataL12L0BNZ2ZNB8B4WithCoord3510;
};
} // namespace Te
} // namespace AscendC
#endif // IMPL_TENSOR_API_ARCH_DATAMOVE_L1_TO_L0_ROUTING_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
