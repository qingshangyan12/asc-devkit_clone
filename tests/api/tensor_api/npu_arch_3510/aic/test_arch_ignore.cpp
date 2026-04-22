/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>
#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/tensor_api/tensor.h"

namespace {
using namespace AscendC::Te;

constexpr CopyGM2UBTrait GM2UB_TRAIT = CopyGM2UBTrait{};
constexpr CopyL0C2GMTrait L0C2GM_TRAIT = CopyL0C2GMTrait{};
constexpr CopyL0C2UBTrait L0C2UB_TRAIT = CopyL0C2UBTrait{};
constexpr CopyL12UBTrait L12UB_TRAIT = CopyL12UBTrait{};
constexpr CopyUB2GMTrait UB2GM_TRAIT = CopyUB2GMTrait{};
constexpr CopyL12FBTrait L12FB_TRAIT = CopyL12FBTrait{};
constexpr CopyL12BTTrait L12BT_TRAIT = CopyL12BTTrait{};
constexpr CopyUB2L1Trait UB2L1_TRAIT = CopyUB2L1Trait{};
} // namespace

TEST(TensorApiDatamoveRoutingIgnore, CopyGM2UBIgnoreRun)
{
    CopyGM2UBIgnore::Run<GM2UB_TRAIT>(1, 2);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL0C2GMIgnoreRun)
{
    CopyL0C2GMIgnore::Run<L0C2GM_TRAIT>(1, 2, 3);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL0C2UBIgnoreRun)
{
    CopyL0C2UBIgnore::Run<L0C2UB_TRAIT>(1, 2, 3);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL12UBIgnoreRun)
{
    CopyL12UBIgnore::Run<L12UB_TRAIT>(1, 2);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyUB2GMIgnoreRun)
{
    CopyUB2GMIgnore::Run<UB2GM_TRAIT>(1, 2);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL12FBIgnoreRun)
{
    CopyL12FBIgnore::Run<L12FB_TRAIT>(1, 2);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL12BTIgnoreRun)
{
    CopyL12BTIgnore::Run<L12BT_TRAIT>(1, 2);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyUB2L1IgnoreRun)
{
    CopyUB2L1Ignore::Run<UB2L1_TRAIT>(1, 2);
    SUCCEED();
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL12L0AIgnoreRouteFallback)
{
    using Fallback = typename CopyL12L0ATensor2Tensor<
        Location::UB, Location::L1, ArchVersion::V3510, NZLayoutPtn, NZLayoutPtn, CopyMode::NORMAL>::type;
    EXPECT_TRUE((std::is_same_v<Fallback, CopyL12L0AIgnore>));
}

TEST(TensorApiDatamoveRoutingIgnore, CopyL12L0BIgnoreRouteFallback)
{
    using Fallback = typename CopyL12L0BTensor2Tensor<
        Location::UB, Location::L1, ArchVersion::V3510, ZNLayoutPtn, ZNLayoutPtn, CopyMode::NORMAL>::type;
    EXPECT_TRUE((std::is_same_v<Fallback, CopyL12L0BIgnore>));
}

TEST(TensorApiDatamoveRoutingIgnore, CopyGM2L1IgnoreRouteFallback)
{
    using Fallback = typename CopyGM2L1Tensor2Tensor<
        Location::UB, Location::GM, ArchVersion::V3510, NDExtLayoutPtn, NDExtLayoutPtn>::type;
    EXPECT_TRUE((std::is_same_v<Fallback, CopyGM2L1Ignore>));
}
