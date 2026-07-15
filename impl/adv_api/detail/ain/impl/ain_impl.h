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

AIN_DEVICE AinCommSymWindow GetWindowPtr(
    const hcomm::HcommTeam* team, 
    const hcomm::HcommWindow& win,
    uint32_t memberId,
    uint64_t offset,
    uint64_t bytes)
{
    CommMem* mem = &win.mems[memberId];
    uint64_t memHandle = (uint64_t)mem->addr + offset;
    return (AinCommSymWindow)memHandle;
}

template <unsigned commEngineMask>
AIN_DEVICE Ain::Ain(int contextIndex)
{}

AIN_DEVICE Ain::~Ain() {}

AIN_DEVICE void Ain::flush(AinTeam team)
{
    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    uint32_t rankSize = hcommTeam->memberNum;
    uint32_t myRank = hcommTeam->selfMemberId;

    for (uint32_t i = 0; i < rankSize; i++) {
        if (i == myRank) {
            continue;
        }
        ChannelHandle channel = hcommTeam->channels[i][0];
        hcomm_.Drain(channel);
    }
}

template <typename RemoteAction, typename LocalAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::put(
    AinTeam team, 
    uint32_t peer, 
    AinCommSymWindow dstWin, uint64_t dstOffset, 
    AinCommSymWindow srcWin, uint64_t srcOffset, 
    uint64_t bytes, 
    RemoteAction remoteAction, LocalAction localAction, 
    const DescriptorUbuf& ubuf,
    AinCommitFlags commitFlags
)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "put requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed"
    );
    (void)localAction;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    auto srcSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(srcWin));
    auto dstSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(dstWin));

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t*>(GetWindowPtr(hcommTeam, *dstSymWin, peer, dstOffset, bytes));
    auto localAddr = reinterpret_cast<__gm__ uint8_t*>(GetWindowPtr(hcommTeam, *srcSymWin, hcommTeam->selfMemberId, srcOffset, bytes));

    ChannelHandle channel = hcommTeam->channels[peer][0];

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.WriteNbi<false>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.WriteNbi(channel, remoteAddr, localAddr, bytes);
    }

    if constexpr (!IsSameType<RemoteAction, AinRemoteNone>::value) {
        this->signal(team, peer, remoteAction, ubuf, commitFlags);
    }
}

template <typename DescriptorUbuf>
AIN_DEVICE void Ain::get(
    AinTeam team, 
    uint32_t peer, 
    AinCommSymWindow dstWin, uint64_t dstOffset, 
    AinCommSymWindow srcWin, uint64_t srcOffset, 
    uint64_t bytes, 
    const DescriptorUbuf& ubuf, 
    AinCommitFlags commitFlags
)
{
    static_assert(
        !IsSameType<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "get requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    auto srcSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(srcWin));
    auto dstSymWin = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(dstWin));

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t*>(GetWindowPtr(hcommTeam, *dstSymWin, peer, dstOffset, bytes));
    auto localAddr = reinterpret_cast<__gm__ uint8_t*>(GetWindowPtr(hcommTeam, *srcSymWin, hcommTeam->selfMemberId, srcOffset, bytes));

    ChannelHandle channel = hcommTeam->channels[peer][0];

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.ReadNbi<false>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.ReadNbi(channel, remoteAddr, localAddr, bytes);
    }
}


template <typename T>
AIN_DEVICE AinSignalDescriptor GetSignalDescriptor(T action)
{
    AinSignalDescriptor desc{};
    if constexpr (IsSameType<T, AinVASignalInc>::value || IsSameType<T, AinVASignalAdd>::value) {
        desc.signalOffset = action.signalOffset;
        // desc.signalWindow = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(action.signalWindow)); 
        desc.signalWindow = action.signalWindow;
    } else {
        desc.signalOffset = action.signalId * sizeof(uint64_t);        
    }    
    return desc;
}

template <typename T>
AIN_DEVICE uint64_t GetSignalOpArg(T action)
{
    if constexpr (IsSameType<T, AinSignalInc>::value ||
        IsSameType<T, AinVASignalInc>::value) {
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

    // auto hcommTeam = reinterpret_cast<__gm__ hcomm::HcommTeam*>(team);
    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));

    ChannelHandle channel = hcommTeam->channels[peer][0];

    AinSignalDescriptor signalDesc = GetSignalDescriptor(action);
    uint64_t signalOpArg = GetSignalOpArg(action);

    AinCommSymWindow remoteAddr;
    if (signalDesc.signalWindow == nullptr) {
        remoteAddr = GetWindowPtr(hcommTeam, hcommTeam->workspace.window, peer, signalDesc.signalOffset, sizeof(uint64_t));
    } else {
        auto signalWindow = reinterpret_cast<hcomm::HcommWindow*>(reinterpret_cast<uint64_t>(signalDesc.signalWindow));
        remoteAddr = GetWindowPtr(hcommTeam, *signalWindow, peer, signalDesc.signalOffset, sizeof(uint64_t));
    }
    auto remoteSignal = reinterpret_cast<__gm__ uint8_t*>(remoteAddr);

    auto amoBase = hcommTeam->workspace.shadowMem.addr;
    uint64_t amoOffset =
        (hcommTeam->workspace.signalCount + hcommTeam->workspace.counterCount + hcommTeam->selfMemberId) * sizeof(uint64_t);
    auto amoSignal = reinterpret_cast<__gm__ uint8_t*>(reinterpret_cast<uintptr_t>(amoBase) + amoOffset);
    
    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.AtomicFAA<uint64_t, false>(channel, remoteSignal, amoSignal, signalOpArg);
    } else {
        hcomm_.AtomicFAA(channel, remoteSignal, amoSignal, signalOpArg);
    }
}

AIN_DEVICE uint64_t Ain::readSignal(AinTeam team, AinSignal signalId, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    AinCommSymWindow signalAddr = GetWindowPtr(
        hcommTeam, hcommTeam->workspace.window, hcommTeam->selfMemberId, signalOffset, sizeof(uint64_t));
    __gm__ uint64_t* ptr = reinterpret_cast<__gm__ uint64_t*>(signalAddr);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    uint64_t got = ld_dev(ptr, 0);
    return mask & got;
}

// AIN_DEVICE uint64_t Ain::readSignal(
//     AinTeam team, AinCommSymWindow signalWin, uint64_t signalOffset, uint32_t bits, AinMemoryOrder order) const
// {
//     (void)order;
//     auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
//     auto signalWindow = reinterpret_cast<hcomm::HcommWindow*>(signalWin);
//     AinCommSymWindow signalAddr = GetWindowPtr(
//         hcommTeam, signalWin, hcommTeam->selfMemberId, signalOffset, sizeof(uint64_t));
//     __gm__ uint64_t* ptr = reinterpret_cast<__gm__ uint64_t*>(signalAddr);
//     uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
//     uint64_t got = ld_dev(ptr, 0);
//     return mask & got;
// }

AIN_DEVICE void Ain::waitSignal(
    AinTeam team, AinSignal signalId, uint64_t least, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    AinCommSymWindow signalAddr = GetWindowPtr(
        hcommTeam, hcommTeam->workspace.window, hcommTeam->selfMemberId, signalOffset, sizeof(uint64_t));
    __gm__ uint64_t* ptr = reinterpret_cast<__gm__ uint64_t*>(signalAddr);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    while (true) {
        uint64_t got = ld_dev(ptr, 0);
        if (least <= (got & mask)) {
            break;
        }
    }
}

// AIN_DEVICE void Ain::waitSignal(
//     AinTeam team, AinCommSymWindow signalWin, uint64_t signalOffset,
//     uint64_t least, uint32_t bits, AinMemoryOrder order) const
// {
//     (void)order;
//     auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
//     auto signalWindow = reinterpret_cast<hcomm::HcommWindow*>(signalWin);
//     AinCommSymWindow signalAddr = GetWindowPtr(
//         hcommTeam, signalWindow, hcommTeam->selfMemberId, signalOffset, sizeof(uint64_t));    
//     __gm__ uint64_t* ptr = reinterpret_cast<__gm__ uint64_t*>(signalAddr);
//     uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
//     while (true) {
//         uint64_t got = ld_dev(ptr, 0);
//         if (least <= (got & mask)) {
//             break;
//         }
//     }
// }

AIN_DEVICE __gm__ uint64_t* Ain::getSignalShadowPtr(AinTeam team, AinSignal signalId) const
{
    auto hcommTeam = reinterpret_cast<hcomm::HcommTeam*>(reinterpret_cast<uint64_t>(team));
    auto signalShadow = hcommTeam->workspace.shadowMem.addr;
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    return reinterpret_cast<__gm__ uint64_t*>(
        reinterpret_cast<uintptr_t>(signalShadow) + signalOffset);
}

}  // namespace AscendC

#endif  // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif
