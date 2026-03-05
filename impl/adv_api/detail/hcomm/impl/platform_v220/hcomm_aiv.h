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
 * \file hcomm_aiv.h
 * \brief
 */
#ifndef IMPL_V220_HCOMM_AIV_H
#define IMPL_V220_HCOMM_AIV_H

#include "../../common/hcomm_utils.h"
#include "../../common/hcomm_inner_def.h"

namespace AscendC {

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Write(ChannelHandle channelHandle,
                                                                                      GM_ADDR dst, GM_ADDR src,
                                                                                      uint64_t len)
{
    HcommHandle handleId = ++curHandleId_;
    doorBell(channelHandle, dst, src, len);
    KERNEL_LOG(KERNEL_ERROR, "Hcomm Write finish handleId:%u", handleId);
    return handleId;
}

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Read(ChannelHandle channelHandle,
                                                                                     GM_ADDR dst, GM_ADDR src,
                                                                                     uint64_t len)
{
    HcommHandle handleId = ++curHandleId_;
    doorBell(channelHandle, dst, src, len);
    KERNEL_LOG(KERNEL_ERROR, "Hcomm Read finish andleId:%u", handleId);
    return handleId;
}

__aicore__ inline void HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::doorBell(ChannelHandle channelHandle,
                                                                                  GM_ADDR dst, GM_ADDR src,
                                                                                  uint64_t len)
{
    __gm__ Channel* channelPtr = (__gm__ Channel*)channelHandle;
    auto qpNum = channelPtr->sqContextAddr->ctx.rdmaSqContext.qpn;



}

} // namespace AscendC

#endif // IMPL_V220_HCOMM_AIV_H
