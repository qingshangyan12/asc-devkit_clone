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
#pragma message(                                                                                         \
    "impl/adv_api/detail/ain/impl/ain_impl.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#include "ain_impl_def.h"

namespace AscendC {

static AIN_DEVICE AinTeam AinTeamWorld(AinDevComm devComm)
{
    __gm__ HcclDevComm* hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm);
    AinTeam team;
    team.nRanks = hcclDevComm->rankSize;
    team.rank = hcclDevComm->rankId;
    team.stride = 1;
    return team;
}

template <unsigned commEngineMask>
AIN_DEVICE Ain::Ain(AinDevComm devComm, int contextIndex)
    : devComm_(reinterpret_cast<uint64_t>(devComm)), contextIndex_(contextIndex)
{}

AIN_DEVICE Ain::~Ain() {}

AIN_DEVICE void Ain::flush()
{
    __gm__ HcclDevComm* hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    uint32_t myRank = hcclDevComm->rankId;
    uint32_t rankSize = hcclDevComm->rankSize;
    AivRes* aivRes = reinterpret_cast<AivRes*>(hcclDevComm->AivRes);

    for (uint32_t i = 0; i < rankSize; ++i) {
        if (i == myRank) {
            continue;
        }
        ChannelHandle channel = aivRes->entity[i][0];
        hcomm_.Drain(channel);
    }
}

template <typename RemoteAction, typename LocalAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::put(
    AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset, AinCommSymWindow srcWin,
    uint64_t srcOffset, uint64_t bytes, RemoteAction remoteAction, LocalAction localAction, const DescriptorUbuf& ubuf,
    AinCommitFlags commitFlags)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "put requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    (void)remoteAction;
    (void)localAction;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    __gm__ SymmetricWindow* dstSymWin = reinterpret_cast<__gm__ SymmetricWindow*>(reinterpret_cast<uint64_t>(dstWin));
    __gm__ SymmetricWindow* srcSymWin = reinterpret_cast<__gm__ SymmetricWindow*>(reinterpret_cast<uint64_t>(srcWin));
    __gm__ HcclDevComm* hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    AivRes* aivRes = reinterpret_cast<AivRes*>(hcclDevComm->AivRes);
    // Each rank currently has only one channel, so channel index 0 is used by default.
    ChannelHandle channel = aivRes->entity[peer][0];

    auto remoteAddr =
        reinterpret_cast<__gm__ uint8_t*>(reinterpret_cast<uintptr_t>(dstSymWin->remoteMems[peer].addr) + dstOffset);
    auto localAddr = reinterpret_cast<__gm__ uint8_t*>(reinterpret_cast<uintptr_t>(srcSymWin->userVa) + srcOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.WriteNbi<false>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.WriteNbi(channel, remoteAddr, localAddr, bytes);
    }
}

template <typename DescriptorUbuf>
AIN_DEVICE void Ain::get(
    AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset, AinCommSymWindow srcWin,
    uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf& ubuf, AinCommitFlags commitFlags)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "get requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    __gm__ SymmetricWindow* dstSymWin = reinterpret_cast<__gm__ SymmetricWindow*>(reinterpret_cast<uint64_t>(dstWin));
    __gm__ SymmetricWindow* srcSymWin = reinterpret_cast<__gm__ SymmetricWindow*>(reinterpret_cast<uint64_t>(srcWin));
    __gm__ HcclDevComm* hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    AivRes* aivRes = reinterpret_cast<AivRes*>(hcclDevComm->AivRes);
    // Each rank currently has only one channel, so channel index 0 is used by default.
    ChannelHandle channel = aivRes->entity[peer][0];

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t*>(reinterpret_cast<uintptr_t>(dstSymWin->userVa) + dstOffset);
    auto localAddr =
        reinterpret_cast<__gm__ uint8_t*>(reinterpret_cast<uintptr_t>(srcSymWin->remoteMems[peer].addr) + srcOffset);
    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.ReadNbi<false>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.ReadNbi(channel, remoteAddr, localAddr, bytes);
    }
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
    if constexpr (IsSameType<T, AinSignalInc>::value) {
        return 1;
    } else {
        return action.value;
    }
}

template <typename RemoteAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::signal(AinTeam team, uint32_t peer, RemoteAction action, const DescriptorUbuf& ubuf,
    AinCommitFlags commitFlags)
{
    static_assert(!IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "signal requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    AinSignalDescriptor signalDesc = GetSignalDescriptor(*this, action);
    uint64_t signalOpArg = GetSignalOpArg(action);

    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    auto aivRes = reinterpret_cast<AivRes*>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    ChannelHandle channel = aivRes->entity[peer][0];

    uint64_t signalOffset = signalDesc.indexedSignal.signalId * sizeof(uint64_t);
    auto remoteSignal = reinterpret_cast<__gm__ uint8_t*>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + signalOffset);

    auto amoSignalId = aivRes->ainWorldBarrier.signal0 + team.rank;
    uint64_t amoOffset = amoSignalId * sizeof(uint64_t);
    auto amoSignal = reinterpret_cast<__gm__ uint8_t*>(
        reinterpret_cast<uintptr_t>(aivRes->ainSignalShadows) + amoOffset);
}

AIN_DEVICE uint64_t Ain::readSignal(AinSignal signalId, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    auto aivRes = reinterpret_cast<AivRes*>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    __gm__ uint64_t* ptr = reinterpret_cast<__gm__ uint64_t*>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + signalOffset);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    uint64_t got = ld_dev(ptr, 0);
    return mask & got;
}

AIN_DEVICE void Ain::waitSignal(AinSignal signalId, uint64_t least, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    auto aivRes = reinterpret_cast<AivRes*>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    __gm__ uint64_t* ptr = reinterpret_cast<__gm__ uint64_t*>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + signalOffset);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    while (true) {
        uint64_t got = ld_dev(ptr, 0);
        if (least <= (got & mask)) {
            break;
        }
    }
}

AIN_DEVICE __gm__ uint64_t* Ain::getSignalShadowPtr(AinSignal signalId) const
{
    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm*>(devComm_);
    auto aivRes = reinterpret_cast<AivRes*>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    return reinterpret_cast<__gm__ uint64_t*>(
        reinterpret_cast<uintptr_t>(aivRes->ainSignalShadows) + signalOffset);
}

}  // namespace AscendC

#endif  // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif
