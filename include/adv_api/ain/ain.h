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
 * \file ain.h
 * \brief Ain device interface
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_H__
#endif

#ifndef INCLUDE_ADV_API_AIN_AIN_H
#define INCLUDE_ADV_API_AIN_AIN_H

#include <cstddef>
#include <cstdint>

#include "adv_api/hcomm/hcomm.h"
#include "../../../impl/adv_api/detail/ain/impl/ain_impl_def.h"

namespace ain {
namespace device {

enum AinMemoryOrder {
    AIN_MEMORY_ORDER_RELAXED,
    AIN_MEMORY_ORDER_ACQUIRE,
    AIN_MEMORY_ORDER_RELEASE,
    AIN_MEMORY_ORDER_ACQ_REL
};

class Ain {
public:
    AIN_DEVICE Ain(__gm__ void *devComm, int contextIndex);
    AIN_DEVICE ~Ain();

    AIN_DEVICE void flush();

    template <typename RemoteAction = AinRemoteNone, typename LocalAction = AinLocalNone,
        typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE void put(void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
        __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf = DescriptorUbuf{},
        RemoteAction remoteAction = RemoteAction{}, LocalAction localAction = LocalAction{},
        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

    template <typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE void get(void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
        __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf = DescriptorUbuf{},
        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

private:
    static constexpr uint32_t MAX_PENDING_CHANNELS = 64;

    AIN_DEVICE uint32_t DecodeWindow(__gm__ void *window) const;
    AIN_DEVICE void RecordChannel(AscendC::ChannelHandle channel);

private:
    uint64_t devComm_ = 0;
    int contextIndex_ = 0;
    AscendC::Hcomm<AscendC::CommProtocol::COMM_PROTOCOL_UBC_CTP> hcomm_;
    AscendC::ChannelHandle pendingChannels_[MAX_PENDING_CHANNELS] = {};
    uint32_t pendingChannelCount_ = 0;
};

}  // namespace device
}  // namespace ain

#include "../../../impl/adv_api/detail/ain/impl/ain_impl.h"

#endif  // INCLUDE_ADV_API_AIN_AIN_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_H__
#endif
