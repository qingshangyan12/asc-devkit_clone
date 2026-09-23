/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>
#define private public
#include "kernel_operator.h"
#undef private

using namespace AscendC;
using namespace HcclApi;

namespace {
class PeerOnlyPrepareTest : public ::testing::Test {
protected:
    using Impl = HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, DEFAULT_CFG>;
    Impl impl;
    CommonPrepareParamCcu handles[2]{};
    HcclCombineOpParam context{};
    Mc2CcTilingInner tiling{};
    void SetUp() override
    {
        context.rankNum = 2;
        context.rankId = 1;
        impl.newCcuFlag_ = false;
        impl.hcclContext_ = &context;
        impl.handleParamGM_ = handles;
        impl.curVersion_ = HcclTilingVersion::NEW_TILING_VERSION;
        tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        tiling.srcDataType = static_cast<uint8_t>(HCCL_DATA_TYPE_INT8);
        tiling.dstDataType = tiling.srcDataType;
        tiling.reduceType = static_cast<uint32_t>(HCCL_REDUCE_MAX);
        impl.ccOpTilingDataTable_[tiling.opType] = reinterpret_cast<uint64_t>(&tiling);
        for (auto& handle : handles) {
            handle.commType.prepareType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
            handle.sendBuf = reinterpret_cast<GM_ADDR>(0x100000);
            handle.recvBuf = reinterpret_cast<GM_ADDR>(0x200000);
            handle.count = 257;
            handle.strideCount = 1024;
        }
    }
};

TEST_F(PeerOnlyPrepareTest, SnapshotSurvivesTilingChangeAndRepeat)
{
    tiling.skipLocalRankCopy = 1;
    impl.InitCcuParam(0);
    tiling.skipLocalRankCopy = 0;
    impl.InitCcuParam(1);
    EXPECT_EQ(handles[0].skipLocalRankCopy, 1);
    EXPECT_EQ(handles[1].skipLocalRankCopy, 0);
    for (uint32_t repeat = 0; repeat < 3; ++repeat) {
        impl.ccuParam_.repeatIndex = repeat;
        for (uint32_t handle = 0; handle < 2; ++handle) {
            impl.CcuPrepareForReduceScatterPeerOnlyM2M(&handles[handle]);
            EXPECT_EQ(impl.xnData_[1], 0x100000U + repeat * 257U);
            EXPECT_EQ(impl.xnData_[2], 0x200000U + repeat * 257U);
            EXPECT_EQ(impl.xnData_[4], 1024U);
            EXPECT_EQ(impl.xnData_[5], handle == 0 ? 1U : 0U);
            EXPECT_EQ(impl.xnData_[7], 257U);
            EXPECT_EQ(impl.xnData_[8], UINT64_MAX - 1);
        }
    }
}

TEST_F(PeerOnlyPrepareTest, MissingTilingResetsToSafeDefault)
{
    handles[0].skipLocalRankCopy = 1;
    impl.curVersion_ = HcclTilingVersion::DEPRECATED_TILING_VERSION;
    impl.InitCcuParam(0);
    EXPECT_EQ(handles[0].skipLocalRankCopy, 0);
}

TEST_F(PeerOnlyPrepareTest, OnlineTilingUsesRelativeOffset)
{
    struct TilingBlock {
        Mc2InitTilingInner init{};
        Mc2CcTilingInner cc{};
    } block;
    block.cc = tiling;
    block.cc.skipLocalRankCopy = 1;
    impl.curVersion_ = HcclTilingVersion::ONLINE_COMPILATION_TILING_VERSION;
    impl.tilingBaseAddr_ = reinterpret_cast<uint64_t>(&block);
    impl.ccOpTilingDataTable_[tiling.opType] = offsetof(TilingBlock, cc);
    impl.InitCcuParam(0);
    EXPECT_EQ(handles[0].skipLocalRankCopy, 1);
}

TEST_F(PeerOnlyPrepareTest, RankStrideAndChunkBoundaries)
{
    const uint64_t chunk = 256ULL * 1024 * 1024;
    for (uint32_t rank = 0; rank < 2; ++rank) {
        context.rankId = rank;
        for (uint64_t count : std::vector<uint64_t>{0, chunk, chunk + 128, 2 * chunk}) {
            handles[0].count = count;
            handles[0].strideCount = 0;
            impl.InitCcuParam(0);
            impl.ccuParam_.repeatIndex = 0;
            impl.CcuPrepareForReduceScatterPeerOnlyM2M(&handles[0]);
            EXPECT_EQ(impl.xnData_[4], rank * count);
            EXPECT_EQ(impl.xnData_[5], 0U);
            const uint64_t chunks = count == 0 ? 0 : (count - 1) / chunk + 1;
            EXPECT_EQ(impl.xnData_[8], UINT64_MAX - chunks);
            EXPECT_EQ(impl.xnData_[7], chunks == 0 ? 0 : count - (chunks - 1) * chunk);
        }
    }
}

TEST_F(PeerOnlyPrepareTest, WorkspaceLayoutUnchanged)
{
    EXPECT_EQ(offsetof(CommonPrepareParamCcu, paramExt), offsetof(CommonPrepareParam, paramExt));
    EXPECT_EQ(offsetof(CommonPrepareParamCcu, wParamExt), offsetof(CommonPrepareParam, wParamExt));
    EXPECT_EQ(offsetof(CommonPrepareParamCcu, skipLocalRankCopy), offsetof(CommonPrepareParamCcu, repeat) + 1);
    EXPECT_EQ(sizeof(CommonPrepareParamCcu) % 64, 0U);
}
} // namespace
