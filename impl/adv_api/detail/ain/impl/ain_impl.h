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
 * \file ain_impl.h
 * \brief Ain implementation
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/ain/impl/ain_impl.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#include "ain_impl_def.h"

namespace ain {
namespace device {

AIN_DEVICE Ain::Ain(__gm__ void *devComm, int contextIndex)
    : devComm_(reinterpret_cast<uint64_t>(devComm)), contextIndex_(contextIndex)
{
}

AIN_DEVICE Ain::~Ain()
{
}

AIN_DEVICE void Ain::flush()
{
    for (uint32_t i = 0; i < pendingChannelCount_; ++i) {
        hcomm_.Commit(pendingChannels_[i]);
        hcomm_.Drain(pendingChannels_[i]);
    }
    pendingChannelCount_ = 0;
}

template <typename RemoteAction, typename LocalAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::put(void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
    __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf,
    RemoteAction remoteAction, LocalAction localAction, AinCommitFlags commitFlags)
{
    static_assert(!AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "put requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    (void)remoteAction;
    (void)localAction;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    uint32_t dstWinIdx = DecodeWindow(dstWin);
    uint32_t srcWinIdx = DecodeWindow(srcWin);
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(devComm_);
    size_t actualPos = 4 + peer;
    AscendC::ChannelHandle channel = devCommTable[actualPos];
    auto channelEntity = reinterpret_cast<__gm__ AscendC::ChannelEntity *>(channel);

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->remoteBufferAddr[dstWinIdx].bufferInfo.rma.addr + dstOffset);
    auto localAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->localBufferAddr[srcWinIdx].bufferInfo.rma.addr + srcOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.WriteNbi<false>(channel, remoteAddr, localAddr, bytes);
        RecordChannel(channel);
    } else {
        hcomm_.WriteNbi(channel, remoteAddr, localAddr, bytes);
    }
}

template <typename DescriptorUbuf>
AIN_DEVICE void Ain::get(void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
    __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf,
    AinCommitFlags commitFlags)
{
    static_assert(!AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "get requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    uint32_t dstWinIdx = DecodeWindow(dstWin);
    uint32_t srcWinIdx = DecodeWindow(srcWin);
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(devComm_);
    size_t actualPos = 4 + peer;
    AscendC::ChannelHandle channel = devCommTable[actualPos];
    auto channelEntity = reinterpret_cast<__gm__ AscendC::ChannelEntity *>(channel);

    auto localAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->localBufferAddr[dstWinIdx].bufferInfo.rma.addr + dstOffset);
    auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->remoteBufferAddr[srcWinIdx].bufferInfo.rma.addr + srcOffset);
    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.ReadNbi<false>(channel, localAddr, remoteAddr, bytes);
        RecordChannel(channel);
    } else {
        hcomm_.ReadNbi(channel, localAddr, remoteAddr, bytes);
    }
}

AIN_DEVICE uint32_t Ain::DecodeWindow(__gm__ void *window) const
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(window));
}

AIN_DEVICE void Ain::RecordChannel(AscendC::ChannelHandle channel)
{
    for (uint32_t i = 0; i < pendingChannelCount_; ++i) {
        if (pendingChannels_[i] == channel) {
            return;
        }
    }

    if (pendingChannelCount_ >= MAX_PENDING_CHANNELS) {
        flush();
    }
    pendingChannels_[pendingChannelCount_++] = channel;
}

}  // namespace device
}  // namespace ain

#endif  // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif
