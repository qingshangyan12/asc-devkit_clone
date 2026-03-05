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
    channel.sqContextAddr->cxt.rdmaSqContext.qpn = 1;
    channel.sqContextAddr->cxt.rdmaSqContext.sqVa = 0x1000;
    channel.sqContextAddr->cxt.rdmaSqContext.wqSize = 10;
    channel.sqContextAddr->cxt.rdmaSqContext.depth = 10;
    channel.sqContextAddr->cxt.rdmaSqContext.headAddr = 0x2000;
    channel.sqContextAddr->cxt.rdmaSqContext.tailAddr = 0x3000;
    channel.sqContextAddr->cxt.rdmaSqContext.sl = 1;
    channel.sqContextAddr->cxt.rdmaSqContext.dbVa = 0x4000;
    channel.sqContextAddr->cxt.rdmaSqContext.dbModel = 0;

    channel->remoteBuffAddr->pti.rdmaMemProtectionInfo.rkey = 123456;
    channel->localBuffAddr->pti.rdmaMemProtectionInfo.lkey = 654321;

    Hcomm hcomm;
    ChannelHandle channelHandle = static_cast<ChannelHandle>(&channel);
    HcommHandle handleId =
        hcomm.Read(channelHandle, reinterpret_cast<GM_ADDR>(0x11), reinterpret_cast<GM_ADDR>(0x22), 10);

    EXPECT_EQ(handleId, 0);
}