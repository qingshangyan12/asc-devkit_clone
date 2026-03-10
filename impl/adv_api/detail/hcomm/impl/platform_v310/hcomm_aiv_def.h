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
 * \brief Hcomm AIV definition for V310
 */
#ifndef IMPL_V310_HCOMM_AIV_DEF_H
#define IMPL_V310_HCOMM_AIV_DEF_H

namespace AscendC {
constexpr int8_t INVALID_HANDLE_ID = -1;
constexpr uint32_t HCOMM_MAX_HANDLE_ID = 1024;
constexpr uint32_t DEFAULT_WQE_ITEM_SIZE = 64;
constexpr uint32_t ROCE_SQ_DOORBELL_TYPE = 2;
constexpr uint32_t ROCE_INIT_SQ_DB_SGIT_IDX = 1;
constexpr uint32_t HCOMM_MEM_BLOCK_SIZE = 32;

template <>
class HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE> {
public:
    __aicore__ inline HcommImpl();
    __aicore__ inline ~HcommImpl();
    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Write(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Read(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Commit(HcommHandle handleId);
    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Wait(HcommHandle handleId);

private:
    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Operate(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len,
        uint32_t opType);
private:
    TPipe pipe_;
    LocalTensor<uint8_t> wqeItem_;
    LocalTensor<uint32_t> cqeItem_;
    LocalTensor<uint32_t> sqPI_;
    LocalTensor<uint32_t> sqCI_;
    LocalTensor<uint32_t> cqCI_;
    LocalTensor<uint64_t> doorBell_;
    ChannelHandle channelList_[HCOMM_MAX_HANDLE_ID] = {0};
    HcommHandle curHandleId_ = INVALID_HANDLE_ID;
};
} // namespace AscendC
#endif