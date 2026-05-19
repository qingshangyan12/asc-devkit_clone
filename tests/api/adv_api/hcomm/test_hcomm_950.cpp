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

using namespace std;
using namespace AscendC;

namespace {

ChannelEntity GetChannelEntity()
{
    ChannelEntity channel;

    // 为 SQ 相关字段分配内存并设置
    channel.sqNum = 1;
    std::unique_ptr<SqContext> sqContext(new SqContext());
    channel.sqContextAddr = sqContext.get();
    channel.sqContextAddr->contextInfo.roceSq.qpn = 1;

    // 分配实际内存
    uint8_t* sqBuffer = new uint8_t[10 * 10]; // 10个wqe，每个wqe大小为10
    channel.sqContextAddr->contextInfo.roceSq.sqVa = reinterpret_cast<uint64_t>(sqBuffer);
    channel.sqContextAddr->contextInfo.roceSq.wqeSize = 10;
    channel.sqContextAddr->contextInfo.roceSq.depth = 10;

    // 分配head和tail内存
    uint32_t* head = new uint32_t(0);
    uint32_t* tail = new uint32_t(0);
    channel.sqContextAddr->contextInfo.roceSq.headAddr = reinterpret_cast<uint64_t>(head);
    channel.sqContextAddr->contextInfo.roceSq.tailAddr = reinterpret_cast<uint64_t>(tail);

    channel.sqContextAddr->contextInfo.roceSq.sl = 1;

    // 分配doorbell内存
    uint64_t* dbBuffer = new uint64_t(0);
    channel.sqContextAddr->contextInfo.roceSq.dbVa = reinterpret_cast<uint64_t>(dbBuffer);
    channel.sqContextAddr->contextInfo.roceSq.dbMode = 0;

    std::unique_ptr<CqContext> cqContext(new CqContext());
    channel.cqContextAddr = cqContext.get();
    uint32_t* cqhead = new uint32_t(0);
    uint32_t* cqtail = new uint32_t(0);
    channel.cqContextAddr->contextInfo.roceCq.headAddr = reinterpret_cast<uint64_t>(cqhead);
    channel.cqContextAddr->contextInfo.roceCq.tailAddr = reinterpret_cast<uint64_t>(cqtail);

    // 为 Buffer 相关字段分配内存并设置
    channel.remoteBufferNum = 1;
    std::unique_ptr<RegedBufferEntity> remoteBuffer(new RegedBufferEntity());
    channel.remoteBufferAddr = remoteBuffer.get();
    channel.remoteBufferAddr->type = RegedBufferType::REGED_BUFFER_RMA; // RDMA
    channel.remoteBufferAddr->bufferInfo.rma.protectionInfo.memInfo.roce.rkey = 123456;

    channel.localBufferNum = 1;
    std::unique_ptr<RegedBufferEntity> localBuffer(new RegedBufferEntity());
    channel.localBufferAddr = localBuffer.get();
    channel.localBufferAddr->type = RegedBufferType::REGED_BUFFER_RMA; // RDMA
    channel.localBufferAddr->bufferInfo.rma.protectionInfo.memInfo.roce.lkey = 654321;

    return channel;
}

} // namespace

class HcommCommonTestSuite : public testing::Test {
protected:
    virtual void SetUp()
    {
        blockIdxBak_ = block_idx;
        GetTPipePtr()->InitBuffer(hcommBuf_, 256);
        tempTensor_ = hcommBuf_.AllocTensor<uint8_t>();
        addr_ = (uint64_t)(tempTensor_.GetPhyAddr());
    }
    virtual void TearDown()
    {
        block_idx = blockIdxBak_;
        hcommBuf_.FreeTensor(tempTensor_);
    }

private:
    int64_t blockIdxBak_;
    TBuf<TPosition::VECOUT> hcommBuf_;
    LocalTensor<uint8_t> tempTensor_;
    uint64_t addr_;
};

TEST_F(HcommCommonTestSuite, Aiv_Read)
{
    ChannelEntity channel = GetChannelEntity();

    Hcomm hcomm;
    hcomm.Init((__ubuf__ uint8_t*)(addr_), 256);
    ChannelPtr channelHandle = reinterpret_cast<ChannelPtr>(&channel);
    HcommHandle handleId =
        hcomm.ReadNbi(channelHandle, reinterpret_cast<GM_ADDR>(0x11), reinterpret_cast<GM_ADDR>(0x22), 1);
    EXPECT_EQ(handleId, 0);
    int32_t ret = hcomm.Commit(handleId);
    EXPECT_EQ(ret, -1);
}

TEST_F(HcommCommonTestSuite, Aiv_Write)
{
    ChannelEntity channel = GetChannelEntity();

    Hcomm hcomm;
    ChannelPtr channelHandle = reinterpret_cast<ChannelPtr>(&channel);
    HcommHandle handleId =
        hcomm.Write<false>(channelHandle, reinterpret_cast<GM_ADDR>(0x11), reinterpret_cast<GM_ADDR>(0x22), 1);
    EXPECT_EQ(handleId, 0);
    int32_t ret = hcomm.Commit(handleId);
    EXPECT_EQ(ret, 0);
    ret = hcomm.Wait(handleId);
    EXPECT_EQ(ret, 0);
}
