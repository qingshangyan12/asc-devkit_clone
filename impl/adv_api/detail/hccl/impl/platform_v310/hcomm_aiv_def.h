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
 * \file hcomm_urma.h
 * \brief
 */
#ifndef IMPL_V310_HCOMM_URMA_DEF_H
#define IMPL_V310_HCOMM_URMA_DEF_H

namespace AscendC {

template <COMM_PROTOCAL::ROCE>
class Hcomm {
public:
    template <bool commit = true, pipe_t pipe = pipe_mte3>
    __aicore__ inline Hanle Write(ChannelHandle channel, void* dst, const void* src, uint64_t len);
    template <bool commit = true, pipe_t pipe = pipe_mte3>
    __aicore__ inline Hanle Read(ChannelHandle channel, void* dst, const void* src, uint64_t len);
    template <pipe_t pipe = pipe_mte3>
    __aicore__ inline int32_t Commit(Handle handle);
    template <pipe_t pipe = pipe_mte3>
    __aicore__ inline int32_t Wait(Handle handle);

private:
    TPipe pipe;
    TQue<TPosition::VECCALC> wqeBuf_;
    TQue<TPosition::VECCALC> cqeBuf_;
    TQue<TPosition::VECCALC> sqPIBuf_;
    TQue<TPosition::VECCALC> sqCIBuf_;
    TQue<TPosition::VECCALC> cqCIBuf_;
    TQue<TPosition::VECCALC> doorBellBuf_;
    LocalTensor<uint8_t> wqeItem_;
    LocalTensor<uint64_t> cqeItem_;
    LocalTensor<uint32_t> sqPI_;
    LocalTensor<uint32_t> sqCI_;
    LocalTensor<uint32_t> cqCI_;
    LocalTensor<uint32_t> doorBell_;
    ChannelHandle channelList_[HCOMM_MAX_HANDLE_ID] = {nullptr};
    HcommHandle curHandleId_ = INVALID_HANDLE_ID;
}
} // namespace AscendC

#endif