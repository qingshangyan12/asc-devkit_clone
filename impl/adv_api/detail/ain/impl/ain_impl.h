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
#pragma message("impl/adv_api/detail/ain/impl/ain_impl.h is an internal header file and must not be used directly. " \
                "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#include "ain_impl_def.h"

namespace AscendC {

AIN_DEVICE AinCommMem GetCommMemPtr(
    const __gm__ hcomm::HcommTeam* team, const CommMem* mems, uint32_t memberId, uint64_t offset, uint64_t bytes)
{
    if (team == nullptr || memberId >= team->memberNum || mems == nullptr) {
        return nullptr;
    }

    CommMem mem = mems[memberId];

    if (mem.addr == nullptr || offset > mem.size || bytes > mem.size - offset) {
        return nullptr;
    }

    volatile uint64_t memHandle = reinterpret_cast<uint64_t>(mem.addr) + offset;
    return reinterpret_cast<AinCommMem>(memHandle);
}

template <CommProtocol commProtocol>
AIN_DEVICE Ain<commProtocol>::Ain(uint32_t contextIndex) : contextIndex_(contextIndex)
{}

template <CommProtocol commProtocol>
AIN_DEVICE Ain<commProtocol>::~Ain()
{}

template <CommProtocol commProtocol>
AIN_DEVICE void Ain<commProtocol>::flush(AinTeam team)
{
    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    uint32_t rankSize = hcommTeam->memberNum;
    uint32_t myRank = hcommTeam->selfMemberId;

    for (uint32_t i = 0; i < rankSize; i++) {
        if (i == myRank) {
            continue;
        }
        ChannelHandle channel = hcommTeam->channels[i][contextIndex_];
        hcomm_.Drain(channel);
    }
}

template <CommProtocol commProtocol>
AIN_DEVICE void Ain<commProtocol>::flushAsync(AinTeam team, uint32_t peer, ChannelHandle* outChannelHandle)
{
    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    *outChannelHandle = hcommTeam->channels[peer][contextIndex_];
}

template <CommProtocol commProtocol>
AIN_DEVICE void Ain<commProtocol>::wait(ChannelHandle& channelHandle)
{
    hcomm_.Drain(channelHandle);
}

template <CommProtocol commProtocol>
template <typename RemoteAction, typename LocalAction, typename DescriptorUbuf, auto const& Config>
AIN_DEVICE void Ain<commProtocol>::put(
    AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset, AinCommSymWindow srcWin,
    uint64_t srcOffset, uint64_t bytes, RemoteAction remoteAction, LocalAction localAction, const DescriptorUbuf& ubuf,
    AinCommitFlags commitFlags)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "put requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)localAction;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    auto srcSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(srcWin));
    auto dstSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(dstWin));

    auto remoteAddr =
        reinterpret_cast<__gm__ uint8_t*>(GetCommMemPtr(hcommTeam, dstSymWin->mems, peer, dstOffset, bytes));
    auto localAddr = reinterpret_cast<__gm__ uint8_t*>(
        GetCommMemPtr(hcommTeam, srcSymWin->mems, hcommTeam->selfMemberId, srcOffset, bytes));

    ChannelHandle channel = hcommTeam->channels[peer][contextIndex_];

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.template WriteNbi<false, PIPE_S, PIPE_MTE3, Config>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.template WriteNbi<true, PIPE_S, PIPE_MTE3, Config>(channel, remoteAddr, localAddr, bytes);
    }

    if constexpr (IsSameType<RemoteAction, AinSignalInc>::value || IsSameType<RemoteAction, AinSignalAdd>::value) {
        this->signal(team, peer, remoteAction, ubuf, commitFlags);
    }
}

template <CommProtocol commProtocol>
template <typename DescriptorUbuf, auto const& Config>
AIN_DEVICE void Ain<commProtocol>::get(
    AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset, AinCommSymWindow srcWin,
    uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf& ubuf, AinCommitFlags commitFlags)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "get requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    auto srcSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(srcWin));
    auto dstSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(dstWin));

    auto localAddr = reinterpret_cast<__gm__ uint8_t*>(
        GetCommMemPtr(hcommTeam, dstSymWin->mems, hcommTeam->selfMemberId, srcOffset, bytes));
    auto remoteAddr =
        reinterpret_cast<__gm__ uint8_t*>(GetCommMemPtr(hcommTeam, srcSymWin->mems, peer, dstOffset, bytes));

    ChannelHandle channel = hcommTeam->channels[peer][contextIndex_];

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.template ReadNbi<false, PIPE_S, PIPE_MTE3, Config>(channel, localAddr, remoteAddr, bytes);
    } else {
        hcomm_.template ReadNbi<true, PIPE_S, PIPE_MTE3, Config>(channel, localAddr, remoteAddr, bytes);
    }
}

template <typename T>
AIN_DEVICE uint64_t GetSignalOpArg(T action)
{
    if constexpr (IsSameType<T, AinSignalInc>::value) {
        return 1;
    } else {
        return action.value;
    }
}

template <CommProtocol commProtocol>
template <typename RemoteAction, typename DescriptorUbuf, auto const& Config>
AIN_DEVICE void Ain<commProtocol>::signal(
    AinTeam team, uint32_t peer, RemoteAction action, const DescriptorUbuf& ubuf, AinCommitFlags commitFlags)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "signal requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    ChannelHandle channel = hcommTeam->channels[peer][contextIndex_];

    uint64_t contextBaseId = (hcommTeam->workspace.workSpaceRequirement.signalCount +
                              hcommTeam->workspace.workSpaceRequirement.counterCount +
                              hcommTeam->workspace.workSpaceRequirement.barrierCount) *
                             contextIndex_;

    uint64_t signalOpArg = GetSignalOpArg(action);
    uint64_t signalOffset = (contextBaseId + action.signalId) * sizeof(uint64_t);

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t*>(
        GetCommMemPtr(hcommTeam, hcommTeam->workspace.remoteMems, peer, signalOffset, sizeof(uint64_t)));

    // amoAddr: the address to store the fetched old value required by AtomicFAA.
    // reuse the slot corresponding to the selfMemberId (which is idle) in barrier region of shadow memory as amoAddr.
    auto localBase = hcommTeam->workspace.shadowMem.addr;
    uint64_t amoOffset = (contextBaseId + hcommTeam->workspace.workSpaceRequirement.signalCount +
                          hcommTeam->workspace.workSpaceRequirement.counterCount + hcommTeam->selfMemberId) *
                         sizeof(uint64_t);
    auto amoAddr = reinterpret_cast<__gm__ uint8_t*>(reinterpret_cast<uintptr_t>(localBase) + amoOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.template AtomicFAA<uint64_t, false, PIPE_S, PIPE_MTE3, Config>(
            channel, remoteAddr, amoAddr, signalOpArg);
    } else {
        hcomm_.template AtomicFAA<uint64_t, true, PIPE_S, PIPE_MTE3, Config>(channel, remoteAddr, amoAddr, signalOpArg);
    }
}

template <CommProtocol commProtocol>
AIN_DEVICE __gm__ uint64_t* Ain<commProtocol>::getSignalPtr(AinTeam team, AinSignal signalId) const
{
    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    uint64_t contextBaseId = (hcommTeam->workspace.workSpaceRequirement.signalCount +
                              hcommTeam->workspace.workSpaceRequirement.counterCount +
                              hcommTeam->workspace.workSpaceRequirement.barrierCount) *
                             contextIndex_;
    uint64_t signalOffset = (contextBaseId + signalId) * sizeof(uint64_t);
    AinCommSymWindow signalAddr = GetCommMemPtr(
        hcommTeam, hcommTeam->workspace.remoteMems, hcommTeam->selfMemberId, signalOffset, sizeof(uint64_t));
    return reinterpret_cast<__gm__ uint64_t*>(signalAddr);
}

template <CommProtocol commProtocol>
AIN_DEVICE uint64_t
Ain<commProtocol>::readSignal(AinTeam team, AinSignal signalId, uint32_t bits, AinMemoryOrder order) const
{
    __gm__ uint64_t* ptr = getSignalPtr(team, signalId);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    return mask & ld_dev(ptr, 0);
}

template <CommProtocol commProtocol>
AIN_DEVICE void Ain<commProtocol>::waitSignal(
    AinTeam team, AinSignal signalId, uint64_t least, uint32_t bits, AinMemoryOrder order) const
{
    __gm__ uint64_t* ptr = getSignalPtr(team, signalId);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    while (true) {
        if (least <= (ld_dev(ptr, 0) & mask)) {
            break;
        }
    }
}

template <CommProtocol commProtocol>
AIN_DEVICE __gm__ uint64_t* Ain<commProtocol>::getSignalShadowPtr(AinTeam team, AinSignal signalId) const
{
    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    auto signalShadow = hcommTeam->workspace.shadowMem.addr;
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    return reinterpret_cast<__gm__ uint64_t*>(reinterpret_cast<uintptr_t>(signalShadow) + signalOffset);
}

template <CommProtocol commProtocol>
AIN_DEVICE AinBarrierSession<commProtocol>::AinBarrierSession(Ain<commProtocol>* ain, AinTeam team, uint32_t index)
{
    barrierImpl_.ain = ain;
    barrierImpl_.team = team;
    barrierImpl_.index = index;
    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    barrierImpl_.signal = hcommTeam->workspace.workSpaceRequirement.signalCount +
                          hcommTeam->workspace.workSpaceRequirement.counterCount + index * hcommTeam->memberNum;
}

template <CommProtocol commProtocol>
template <typename DescriptorUbuf>
AIN_DEVICE void AinBarrierSession<commProtocol>::sync(AinMemoryOrder order, const DescriptorUbuf& ubuf)
{
    barrierImpl_.template syncImpl<false>(order, 0, ubuf);
}

template <CommProtocol commProtocol>
template <typename DescriptorUbuf>
AIN_DEVICE int32_t
AinBarrierSession<commProtocol>::sync(AinMemoryOrder order, uint64_t timeoutCycles, const DescriptorUbuf& ubuf)
{
    return barrierImpl_.template syncImpl<true>(order, timeoutCycles, ubuf);
}

template <CommProtocol commProtocol>
template <bool EnableTimeout, typename DescriptorUbuf>
AIN_DEVICE int32_t
AinBarrierSessionImpl<commProtocol>::syncImpl(AinMemoryOrder order, uint64_t timeoutCycles, const DescriptorUbuf& ubuf)
{
    auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(this->team));

    for (int32_t i = 0; i < hcommTeam->memberNum - 1; ++i) {
        int peer = 1 + hcommTeam->selfMemberId + i;
        if (hcommTeam->memberNum <= peer) {
            peer -= hcommTeam->memberNum;
        }

        this->ain->signal(this->team, peer, AinSignalInc{this->signal + hcommTeam->selfMemberId}, ubuf);

        __gm__ uint64_t* shadowPtr = this->ain->getSignalShadowPtr(this->team, this->signal + peer);
        uint64_t waitVal = ++(*shadowPtr);

        if constexpr (EnableTimeout) {
            uint64_t retry = 0;
            while (true) {
                uint64_t got = this->ain->readSignal(this->team, this->signal + peer, 64);
                if (got >= waitVal) {
                    break;
                }
                if (retry++ >= timeoutCycles) {
                    return -1;
                }
            }
        } else {
            this->ain->waitSignal(this->team, this->signal + peer, waitVal, 64);
        }
    }

    return 0;
}

} // namespace AscendC

#endif // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif
