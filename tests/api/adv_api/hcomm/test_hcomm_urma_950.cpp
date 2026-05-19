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
#include <vector>
#define private public
#include "kernel_operator.h"

using namespace AscendC;

namespace {

constexpr uint32_t URMA_SQ_DEPTH = 10;
constexpr uint32_t URMA_WQE_SIZE = 64;
constexpr uint32_t URMA_CQE_SIZE = 64;

class UrmaChannelResource {
public:
    UrmaChannelResource()
        : sqBuffer_(URMA_SQ_DEPTH * HCOMM_URMA_MAX_SQE_BB_NUM * URMA_WQE_SIZE, 0),
          cqBuffer_(URMA_SQ_DEPTH * HCOMM_URMA_MAX_SQE_BB_NUM * URMA_CQE_SIZE, 0)
    {
        channel_.engine = 4;
        channel_.sqNum = 1;
        channel_.cqNum = 1;
        channel_.remoteBufferNum = 1;
        channel_.localBufferNum = 1;
        channel_.sqContextAddr = &sqCtx_;
        channel_.cqContextAddr = &cqCtx_;
        channel_.remoteBufferAddr = &remoteBuffer_;
        channel_.localBufferAddr = &localBuffer_;

        sqCtx_.type = AscendC::SQ_CONTEXT_TYPE_UB_JFS;
        sqCtx_.contextInfo.ubJfs.sqVa = reinterpret_cast<uint64_t>(sqBuffer_.data());
        sqCtx_.contextInfo.ubJfs.headAddr = reinterpret_cast<uint64_t>(&sqHead_);
        sqCtx_.contextInfo.ubJfs.tailAddr = reinterpret_cast<uint64_t>(&sqTail_);
        sqCtx_.contextInfo.ubJfs.dbVa = reinterpret_cast<uint64_t>(&sqDoorbell_);
        sqCtx_.contextInfo.ubJfs.jfsID = 1;
        sqCtx_.contextInfo.ubJfs.wqeSize = URMA_WQE_SIZE;
        sqCtx_.contextInfo.ubJfs.sqDepth = URMA_SQ_DEPTH;
        sqCtx_.contextInfo.ubJfs.tpID = 1;

        cqCtx_.type = AscendC::CQ_CONTEXT_TYPE_UB_JFC;
        cqCtx_.contextInfo.ubJfc.scqVa = reinterpret_cast<uint64_t>(cqBuffer_.data());
        cqCtx_.contextInfo.ubJfc.headAddr = reinterpret_cast<uint64_t>(&cqHead_);
        cqCtx_.contextInfo.ubJfc.tailAddr = reinterpret_cast<uint64_t>(&cqTail_);
        cqCtx_.contextInfo.ubJfc.dbVa = reinterpret_cast<uint64_t>(&cqDoorbell_);
        cqCtx_.contextInfo.ubJfc.jfcID = 1;
        cqCtx_.contextInfo.ubJfc.cqeSize = URMA_CQE_SIZE;
        cqCtx_.contextInfo.ubJfc.cqDepth = URMA_SQ_DEPTH * HCOMM_URMA_MAX_SQE_BB_NUM;

        remoteBuffer_.type = AscendC::REGED_BUFFER_RMA;
        remoteBuffer_.bufferInfo.rma.addr = 0x1000;
        remoteBuffer_.bufferInfo.rma.size = 0x1000;
        remoteBuffer_.bufferInfo.rma.protectionInfo.type = AscendC::PROTECTION_TYPE_UB;
        remoteBuffer_.bufferInfo.rma.protectionInfo.memInfo.ub.tokenId = 0x123456;
        remoteBuffer_.bufferInfo.rma.protectionInfo.memInfo.ub.tokenValue = 0x654321;

        localBuffer_.type = AscendC::REGED_BUFFER_RMA;
        localBuffer_.bufferInfo.rma.addr = 0x2000;
        localBuffer_.bufferInfo.rma.size = 0x1000;
        localBuffer_.bufferInfo.rma.protectionInfo.type = AscendC::PROTECTION_TYPE_UB;
        localBuffer_.bufferInfo.rma.protectionInfo.memInfo.ub.tokenId = 0x111111;
        localBuffer_.bufferInfo.rma.protectionInfo.memInfo.ub.tokenValue = 0x222222;
    }

    AscendC::ChannelHandle GetHandle()
    {
        return reinterpret_cast<AscendC::ChannelHandle>(&channel_);
    }

    void CompleteCurrentSq()
    {
        cqTail_ = sqHead_;
        sqTail_ = sqHead_;
    }

private:
    AscendC::ChannelEntity channel_ = {};
    AscendC::SqContext sqCtx_ = {};
    AscendC::CqContext cqCtx_ = {};
    AscendC::RegedBufferEntity remoteBuffer_ = {};
    AscendC::RegedBufferEntity localBuffer_ = {};
    std::vector<uint8_t> sqBuffer_;
    std::vector<uint8_t> cqBuffer_;
    uint32_t sqHead_ = 0;
    uint32_t sqTail_ = 0;
    uint32_t cqHead_ = 0;
    uint32_t cqTail_ = 0;
    uint32_t sqDoorbell_ = 0;
    uint32_t cqDoorbell_ = 0;
};

} // namespace

class HcommUrmaTestSuite : public testing::Test {
protected:
    void SetUp() override
    {
        blockIdxBak_ = block_idx;
    }

    void TearDown() override
    {
        block_idx = blockIdxBak_;
    }

private:
    int64_t blockIdxBak_;
};

TEST_F(HcommUrmaTestSuite, Aiv_Urma_Read)
{
    UrmaChannelResource channel;

    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::URMA> hcomm;
    AscendC::HcommHandle handleId =
        hcomm.ReadNbi(channel.GetHandle(), reinterpret_cast<GM_ADDR>(0x11), reinterpret_cast<GM_ADDR>(0x22), 1);
    EXPECT_EQ(handleId, 0);
    int32_t ret = hcomm.Commit(handleId);
    EXPECT_EQ(ret, -1);
}

TEST_F(HcommUrmaTestSuite, Aiv_Urma_Write)
{
    UrmaChannelResource channel;

    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::URMA> hcomm;
    AscendC::HcommHandle handleId =
        hcomm.WriteNbi<false>(channel.GetHandle(), reinterpret_cast<GM_ADDR>(0x11), reinterpret_cast<GM_ADDR>(0x22), 1);
    EXPECT_EQ(handleId, 0);
    int32_t ret = hcomm.Commit(handleId);
    EXPECT_EQ(ret, 0);
    channel.CompleteCurrentSq();
    ret = hcomm.Wait(handleId);
    EXPECT_EQ(ret, 0);
}

TEST_F(HcommUrmaTestSuite, Aiv_Urma_WriteWithNotify)
{
    UrmaChannelResource channel;

    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::URMA> hcomm;
    AscendC::HcommHandle handleId = hcomm.WriteWithNotifyNbi<false>(channel.GetHandle(),
        reinterpret_cast<GM_ADDR>(0x11), reinterpret_cast<GM_ADDR>(0x22), 1, reinterpret_cast<GM_ADDR>(0x33), 1);
    EXPECT_EQ(handleId, 0);
    int32_t ret = hcomm.Commit(handleId);
    EXPECT_EQ(ret, 0);
    channel.CompleteCurrentSq();
    ret = hcomm.Wait(handleId);
    EXPECT_EQ(ret, 0);
}
