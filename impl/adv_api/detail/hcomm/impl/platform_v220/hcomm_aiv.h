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
    return -1;
}

template <bool commit, pipe_t commit_pipe, pipe_t req_pipe>
__aicore__ inline HcommHandle HcommImpl<COMM_ENGINE::AIV, COMM_PROTOCOL::ROCE>::Read(ChannelHandle channelHandle,
                                                                                     GM_ADDR dst, GM_ADDR src,
                                                                                     uint64_t len)
{
    return -1;
}
} // namespace AscendC

#endif // IMPL_V220_HCOMM_AIV_H
