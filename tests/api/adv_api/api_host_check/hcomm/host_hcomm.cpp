/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "hccl/hccl_common.h"
#include "hcomm/hcomm.h"

__aicore__ void TestHcommWrite0() {
    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::ROCE> obj;
    AscendC::ChannelHandle channelHandle;
    GM_ADDR dst;
    GM_ADDR src;
    uint64_t len = 0;
    obj.Write(channelHandle, dst, src, len);
}

__aicore__ void TestHcommRead1() {
    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::ROCE> obj;
    AscendC::ChannelHandle channelHandle;
    GM_ADDR dst;
    GM_ADDR src;
    uint64_t len = 0;
    obj.Read(channelHandle, dst, src, len);
}

__aicore__ void TestHcommCommit2() {
    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::ROCE> obj;
    AscendC::HcommHandle handleId;
    obj.Commit(handleId);
}

__aicore__ void TestHcommWait3() {
    AscendC::Hcomm<AscendC::CommEngine::AIV, AscendC::CommProtocol::ROCE> obj;
    AscendC::HcommHandle handleId;
    obj.Wait(handleId);
}