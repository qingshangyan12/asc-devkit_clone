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
 * \file hcomm_aiv_def.h
 * \brief
 */
#ifndef IMPL_V220_HCOMM_AIV_DEF_H
#define IMPL_V220_HCOMM_AIV_DEF_H

namespace AscendC {

template <>
class HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE> {
public:
    __aicore__ inline HcommImpl();

    __aicore__ inline ~HcommImpl();

    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Write(ChannelHandle channelHandle, GM_ADDR dst, GM_ADDR src, uint64_t len);

    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Read(ChannelHandle channelHandle, GM_ADDR dst, GM_ADDR src, uint64_t len);

    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Commit(HcommHandle handleId)
    {
        return 0;
    }

    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Wait(HcommHandle handleId)
    {
        return 0;
    }

private:
    __aicore__ inline void postSend(ChannelHandle channelHandle, GM_ADDR dst, GM_ADDR src, uint64_t len, bool isRead);

    __aicore__ inline void doorBell(__gm__ Channel* channelPtr, uint64_t curHead);

private:
    HcommHandle curHandleId_ = static_cast<int8_t>(-1);
    TPipe pipe_;
    LocalTensor<uint64_t> ubLocal_;
    LocalTensor<uint32_t> ubLocalHead_;
};

} // namespace AscendC

#endif // IMPL_V220_HCOMM_AIV_DEF_H
