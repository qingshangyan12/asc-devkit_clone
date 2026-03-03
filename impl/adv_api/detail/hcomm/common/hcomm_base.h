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
 * \file hcomm_base.h
 * \brief
 */
#ifndef IMPL_HCOMM_BASE_H
#define IMPL_HCOMM_BASE_H

#include "hcomm_inner_def.h"

namespace AscendC {
template <uint8_t commEngine, COMM_PROTOCOL commProtocol>
class HcommImpl {
public:
    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Write(ChannelHandle channelHandle, GM_ADDR dst, GM_ADDR src, uint64_t len)
    {
        return -1;
    }

    template <bool commit = true, pipe_t commit_pipe = PIPE_MTE3, pipe_t req_pipe = PIPE_MTE3>
    __aicore__ inline HcommHandle Read(ChannelHandle channelHandle, GM_ADDR dst, GM_ADDR src, uint64_t len)
    {
        return -1;
    }

    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Commit(HcommHandle handleId)
    {
        return -1;
    }

    template <pipe_t pipe = PIPE_MTE3>
    __aicore__ inline int32_t Wait(HcommHandle handleId)
    {
        return -1;
    }

};
} // namespace AscendC

#endif // IMPL_HCOMM_BASE_H
