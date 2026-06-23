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

template <typename T>
AIN_DEVICE AinSignalDescriptor GetSignalDescriptor(const Ain& ain, T action)
{
    (void)ain;
    AinSignalDescriptor desc{};
    desc.type = AIN_SIGNAL_TYPE_INDEXED;
    desc.indexedSignal.signalId = action.signalId;
    return desc;
}

template <typename T>
AIN_DEVICE uint64_t GetSignalOpArg(T action)
{
    if constexpr (AinIsSame<T, AinSignalInc>::value) {
        return 1;
    } else {
        return action.value;
    }
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

template <typename RemoteAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::signal(
    HcclTeam team, uint32_t peer, RemoteAction action, const DescriptorUbuf& ubuf, AinCommitFlags commitFlags)
{
    static_assert(
        !AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "signal requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    AinSignalDescriptor signalDesc = GetSignalDescriptor(*this, action);
    uint64_t signalValue = GetSignalOpArg(action);
    auto devCommTable = reinterpret_cast<__gm__ uint64_t*>(devComm_);
    uint32_t signalWindowIdx = static_cast<uint32_t>(devCommTable[2]);
    AscendC::ChannelHandle channel = devCommTable[4 + peer];
    auto channelEntity = reinterpret_cast<__gm__ AscendC::ChannelEntity*>(channel);
    auto remoteSignal =
        reinterpret_cast<__gm__ uint64_t*>(channelEntity->remoteBufferAddr[signalWindowIdx].bufferInfo.rma.addr) +
        signalDesc.indexedSignal.signalId;

    GM_ADDR signalAddr = reinterpret_cast<GM_ADDR>(remoteSignal);
    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.WriteWithNotifyNbi<false>(channel, signalAddr, signalAddr, 0, signalAddr, signalValue);
        RecordChannel(channel);
    } else {
        hcomm_.WriteWithNotifyNbi(channel, signalAddr, signalAddr, 0, signalAddr, signalValue);
    }
}

AIN_DEVICE uint64_t Ain::readSignal(HcclAinSignal_t signalId, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto devCommTable = reinterpret_cast<__gm__ uint64_t*>(devComm_);
    uint32_t myRank = static_cast<uint32_t>(devCommTable[0]);
    uint32_t rankSize = static_cast<uint32_t>(devCommTable[1]);
    uint32_t signalWindowIdx = static_cast<uint32_t>(devCommTable[2]);
    __gm__ uint64_t* signal = nullptr;
    for (uint32_t peer = 0; peer < rankSize; ++peer) {
        if (peer == myRank || devCommTable[4 + peer] == 0) {
            continue;
        }
        auto channelEntity = reinterpret_cast<__gm__ AscendC::ChannelEntity*>(devCommTable[4 + peer]);
        signal =
            reinterpret_cast<__gm__ uint64_t*>(channelEntity->localBufferAddr[signalWindowIdx].bufferInfo.rma.addr) +
            signalId;
        break;
    }
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    return (*signal) & mask;
}

AIN_DEVICE void Ain::waitSignal(HcclAinSignal_t signalId, uint64_t least, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto devCommTable = reinterpret_cast<__gm__ uint64_t*>(devComm_);
    uint32_t myRank = static_cast<uint32_t>(devCommTable[0]);
    uint32_t rankSize = static_cast<uint32_t>(devCommTable[1]);
    uint32_t signalWindowIdx = static_cast<uint32_t>(devCommTable[2]);
    __gm__ uint64_t* signal = nullptr;
    for (uint32_t peer = 0; peer < rankSize; ++peer) {
        if (peer == myRank || devCommTable[4 + peer] == 0) {
            continue;
        }
        auto channelEntity = reinterpret_cast<__gm__ AscendC::ChannelEntity*>(devCommTable[4 + peer]);
        signal =
            reinterpret_cast<__gm__ uint64_t*>(channelEntity->localBufferAddr[signalWindowIdx].bufferInfo.rma.addr) +
            signalId;
        break;
    }
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    while (((*signal) & mask) < (least & mask)) {
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
