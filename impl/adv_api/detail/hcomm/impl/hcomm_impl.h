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
 * \file hcomm_impl.h
 * \brief
 */
#ifndef IMPL_HCOMM_HCOMM_IMPL_H
#define IMPL_HCOMM_HCOMM_IMPL_H

#if __NPU_ARCH__ == 2201
#include "platform_v220/hcomm_aiv.h"
#elif __NPU_ARCH__ == 3101
#include "platform_v310/hcomm_aiv.h"
#endif

namespace AscendC {

template <COMM_ENGINE commEngine, COMM_PROTOCOL commProtocol>
template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle Hcomm<commEngine, commProtocol>::Write(ChannelHandle channelHandle, GM_ADDR dst,
                                                                     GM_ADDR src, uint64_t len)
{
    return impl_.template Write<commit, commit_pipe, req_pipe>(channelHandle, dst, src, len);
}

template <COMM_ENGINE commEngine, COMM_PROTOCOL commProtocol>
template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle Hcomm<commEngine, commProtocol>::Read(ChannelHandle channelHandle, GM_ADDR dst,
                                                                    GM_ADDR src, uint64_t len)
{
    return impl_.template Read<commit, commit_pipe, req_pipe>(channelHandle, dst, src, len);
}

template <COMM_ENGINE commEngine, COMM_PROTOCOL commProtocol>
template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<commEngine, commProtocol>::Commit(HcommHandle handleId)
{
    return impl_.template Commit<pipe>(handleId);
}

template <COMM_ENGINE commEngine, COMM_PROTOCOL commProtocol>
template <pipe_t pipe>
__aicore__ inline int32_t Hcomm<commEngine, commProtocol>::Wait(HcommHandle handleId)
{
    return impl_.template Wait<pipe>(handleId);
}
} // namespace AscendC

#endif // IMPL_HCOMM_HCOMM_IMPL_H
