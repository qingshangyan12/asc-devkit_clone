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

constexpr uint32_t kRankNum = 8U;
constexpr size_t workSpaceSize = sizeof(HcclMsgArea);

} // namespace

class HcommCommonTestSuite : public testing::Test {
protected:
    virtual void SetUp()
    {
        blockIdxBak_ = block_idx;
    }
    virtual void TearDown()
    {
        block_idx = blockIdxBak_;
    }

private:
    int64_t blockIdxBak_;
};

TEST_F(HcommCommonTestSuite, Aiv_Read)
{
    Channel channel;
    // 初始化类型
    channel.type = 0; // RDMA

    // 为 SQ 相关字段分配内存并设置
    channel.sqNum = 1;
    std::unique_ptr<SqContext> sqContext(new SqContext());
    channel.sqContextAddr = sqContext.get();
    channel.sqContextAddr->type = 0; // RDMA
    channel.sqContextAddr->ctx.rdmaSqContext.qpn = 1;
    channel.sqContextAddr->ctx.rdmaSqContext.sqVa = 0x1000;
    channel.sqContextAddr->ctx.rdmaSqContext.wqeSize = 10;
    channel.sqContextAddr->ctx.rdmaSqContext.depth = 10;
    channel.sqContextAddr->ctx.rdmaSqContext.headAddr = 0x2000;
    channel.sqContextAddr->ctx.rdmaSqContext.tailAddr = 0x3000;
    channel.sqContextAddr->ctx.rdmaSqContext.sl = 1;
    channel.sqContextAddr->ctx.rdmaSqContext.dbVa = 0x4000;
    channel.sqContextAddr->ctx.rdmaSqContext.dbMode = 0;

    // 为 Buffer 相关字段分配内存并设置
    channel.remoteBufferNum = 1;
    std::unique_ptr<ProtectionInfo> remoteBuffer(new ProtectionInfo());
    channel.remoteBufferAddr = remoteBuffer.get();
    channel.remoteBufferAddr->type = 0; // RDMA
    channel.remoteBufferAddr->pti.rdmaMemProtectionInfo.rkey = 123456;

    channel.localBufferNum = 1;
    std::unique_ptr<ProtectionInfo> localBuffer(new ProtectionInfo());
    channel.localBufferAddr = localBuffer.get();
    channel.localBufferAddr->type = 0; // RDMA
    channel.localBufferAddr->pti.rdmaMemProtectionInfo.lkey = 654321;

    Hcomm hcomm;
    // 确保类型转换安全
    ChannelHandle channelHandle = reinterpret_cast<ChannelHandle>(&channel);
    HcommHandle handleId = hcomm.Read(channelHandle, 
                                     reinterpret_cast<GM_ADDR>(0x11), 
                                     reinterpret_cast<GM_ADDR>(0x22), 
                                     1);

    EXPECT_EQ(handleId, 0);
}