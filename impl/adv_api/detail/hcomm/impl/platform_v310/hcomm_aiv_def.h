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
#ifndef IMPL_V310_HCOMM_AIV_DEF_H
#define IMPL_V310_HCOMM_AIV_DEF_H

#include "../../common/hcomm_inner_def.h"

namespace AscendC {

template <>
class HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE> {
public:
    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Write(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Read(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Commit(HcommHandle handleId);
    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Wait(HcommHandle handleId);

private:
    TPipe pipe;
    TQue<TPosition::VECCALC, 1> wqeBuf_;
    TQue<TPosition::VECCALC, 1> cqeBuf_;
    TQue<TPosition::VECCALC, 1> sqPIBuf_;
    TQue<TPosition::VECCALC, 1> sqCIBuf_;
    TQue<TPosition::VECCALC, 1> cqCIBuf_;
    TQue<TPosition::VECCALC, 1> doorBellBuf_;
    LocalTensor<uint8_t> wqeItem_;
    LocalTensor<uint64_t> cqeItem_;
    LocalTensor<uint32_t> sqPI_;
    LocalTensor<uint32_t> sqCI_;
    LocalTensor<uint32_t> cqCI_;
    LocalTensor<uint32_t> doorBell_;
    ChannelHandle channelList_[HCOMM_MAX_HANDLE_ID] = {nullptr};
    HcommHandle curHandleId_ = INVALID_HANDLE_ID;
};
} // namespace AscendC

#endif