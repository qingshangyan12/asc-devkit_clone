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

template <COMM_PROTOCOL commProtocol = ROCE>
class HcommImpl<commProtocol> {
public:
    template <bool commit = true, pipe_t pipe = pipe_mte3>
    __aicore__ inline HcommHandle Write(ChannelHandle channelHandle, void* dst, void* src, uint64_t len);

    template <bool commit = true, pipe_t pipe = pipe_mte3>
    __aicore__ inline HcommHandle Read(ChannelHandle channelHandle, void* dst, void* src, uint64_t len);

public:
    __aicore__ inline void doorBell(RdmaSqContext rdmaSqContext, GM_ADDR destDmaAddr, GM_ADDR srcDmaAddr, uint64_t messageLen);

private:
    LocalTensor<uint64_t> ubLocal;

};

} // namespace AscendC

#endif // IMPL_V220_HCOMM_AIV_DEF_H