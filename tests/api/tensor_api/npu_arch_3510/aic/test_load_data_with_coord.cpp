/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/tensor_api/tensor.h"

#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#include "impl/tensor_api/arch/datamove/l1_to_l0a/npu_arch_3510/zn2nzb8b4_with_coord.h"
#include "impl/tensor_api/arch/datamove/l1_to_l0b/npu_arch_3510/zn2zn_with_coord.h"
#include "impl/tensor_api/arch/datamove/l1_to_l0b/npu_arch_3510/nz2zn_with_coord.h"
#include "impl/tensor_api/arch/datamove/l1_to_l0b/npu_arch_3510/nz2znb8b4_with_coord.h"
#include "impl/tensor_api/arch/datamove/l1_to_l0a/npu_arch_3510/zn2nz_with_coord.h"
#include "impl/tensor_api/arch/datamove/l1_to_l0a/npu_arch_3510/nz2nz_with_coord.h"
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS

using namespace AscendC::Te;

namespace {
constexpr CopyL12L0ATrait L12L0A_TRAIT = CopyL12L0ATrait{};
constexpr CopyL12L0BTrait L12L0B_TRAIT = CopyL12L0BTrait{};
} // namespace

class TestLoadDataWithCoordInterface : public testing::Test {
protected:
    void SetUp() override
    {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override
    {
        AscendC::SetGCoreType(0);
    }
};

TEST_F(TestLoadDataWithCoordInterface, L1ToL0BZn2ZnWithCoordRun)
{
    using DataType = float;
    constexpr uint32_t M = 8;
    constexpr uint32_t N = 16;
    __cb__ DataType dst[M * N] = {0};
    __cbuf__ DataType src[M * N] = {0};
    auto dstTensor =
        MakeTensor(MakeMemPtr<Location::L0B>(dst), MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto srcTensor =
        MakeTensor(MakeMemPtr<Location::L1>(src), MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadDataL12L0BZN2ZNWithCoord3510::Run<L12L0B_TRAIT>(dstTensor, srcTensor, coord);
    SUCCEED();
}

TEST_F(TestLoadDataWithCoordInterface, L1ToL0BNz2ZnWithCoordRun)
{
    using DataType = float;
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 16;
    __cb__ DataType dst[M * N] = {0};
    __cbuf__ DataType src[M * N] = {0};
    auto dstTensor =
        MakeTensor(MakeMemPtr<Location::L0B>(dst), MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto srcTensor =
        MakeTensor(MakeMemPtr<Location::L1>(src), MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadDataL12L0BNZ2ZNWithCoord3510::Run<L12L0B_TRAIT>(dstTensor, srcTensor, coord);
    SUCCEED();
}

TEST_F(TestLoadDataWithCoordInterface, L1ToL0BNz2ZnB8B4WithCoordRun)
{
    using DataType = int8_t;
    constexpr uint32_t M = 32;
    constexpr uint32_t N = 32;
    __cb__ DataType dst[M * N] = {0};
    __cbuf__ DataType src[M * N] = {0};
    auto dstTensor =
        MakeTensor(MakeMemPtr<Location::L0B>(dst), MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto srcTensor =
        MakeTensor(MakeMemPtr<Location::L1>(src), MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadDataL12L0BNZ2ZNB8B4WithCoord3510::Run<L12L0B_TRAIT>(dstTensor, srcTensor, coord);
    SUCCEED();
}

TEST_F(TestLoadDataWithCoordInterface, L1ToL0AZn2NzWithCoordRun)
{
    using DataType = float;
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 16;
    __ca__ DataType dst[M * N] = {0};
    __cbuf__ DataType src[M * N] = {0};
    auto dstTensor =
        MakeTensor(MakeMemPtr<Location::L0A>(dst), MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto srcTensor =
        MakeTensor(MakeMemPtr<Location::L1>(src), MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadDataL12L0AZN2NZWithCoord3510::Run<L12L0A_TRAIT>(dstTensor, srcTensor, coord);
    SUCCEED();
}

TEST_F(TestLoadDataWithCoordInterface, L1ToL0ANz2NzWithCoordRun)
{
    using DataType = float;
    constexpr uint32_t M = 16;
    constexpr uint32_t N = 8;
    __ca__ DataType dst[M * N] = {0};
    __cbuf__ DataType src[M * N] = {0};
    auto dstTensor =
        MakeTensor(MakeMemPtr<Location::L0A>(dst), MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto srcTensor =
        MakeTensor(MakeMemPtr<Location::L1>(src), MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadDataL12L0ANZ2NZWithCoord3510::Run<L12L0A_TRAIT>(dstTensor, srcTensor, coord);
    SUCCEED();
}

TEST_F(TestLoadDataWithCoordInterface, L1ToL0AZn2NzB8B4WithCoordRun)
{
    using DataType = int8_t;
    constexpr uint32_t M = 32;
    constexpr uint32_t N = 32;
    __ca__ DataType dst[M * N] = {0};
    __cbuf__ DataType src[M * N] = {0};
    auto dstTensor =
        MakeTensor(MakeMemPtr<Location::L0A>(dst), MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto srcTensor =
        MakeTensor(MakeMemPtr<Location::L1>(src), MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<DataType>>(M, N));
    auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
    LoadDataL12L0AZN2NZB8B4WithCoord3510::Run<L12L0A_TRAIT>(dstTensor, srcTensor, coord);
    SUCCEED();
}
